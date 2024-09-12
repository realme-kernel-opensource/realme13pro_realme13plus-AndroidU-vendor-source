/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 OPLUS Inc.
 */

#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/regulator/consumer.h>
#include <linux/pinctrl/consumer.h>
#include <linux/pm_runtime.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>

#define DRIVER_NAME "ak7377b"
#define AK7377B_I2C_SLAVE_ADDR  0xE8

#define LOG_INF(format, args...)                                               \
	pr_info(DRIVER_NAME " [%s] " format, __func__, ##args)

#define AK7377B_NAME				"ak7377b"
#define AK7377B_MAX_FOCUS_POS			1023
#define AK7377B_ORIGIN_FOCUS_POS		0
/*
 * This sets the minimum granularity for the focus positions.
 * A value of 1 gives maximum accuracy for a desired focus position
 */
#define AK7377B_FOCUS_STEPS			1
#define AK7377B_SET_POSITION_ADDR		0x00

#define AK7377B_CMD_DELAY			0xff
#define AK7377B_CTRL_DELAY_US			10000
/*
 * This acts as the minimum granularity of lens movement.
 * Keep this value power of 2, so the control steps can be
 * uniformly adjusted for gradual lens movement, with desired
 * number of control steps.
 */
#define AK7377B_MOVE_STEPS			100
#define AK7377B_MOVE_DELAY_US			1000

#ifndef OPLUS_FEATURE_CAMERA_COMMON
#define OPLUS_FEATURE_CAMERA_COMMON
#endif /* OPLUS_FEATURE_CAMERA_COMMON */

/* ak7377b device structure */
struct ak7377b_device {
	struct v4l2_ctrl_handler ctrls;
	struct v4l2_subdev sd;
	struct v4l2_ctrl *focus;
	struct regulator *vin;
	struct regulator *vdd;
	struct pinctrl *vcamaf_pinctrl;
	struct pinctrl_state *vcamaf_on;
	struct pinctrl_state *vcamaf_off;
	/* active or standby mode */
	bool active;
};

#define VCM_IOC_POWER_ON         _IO('V', BASE_VIDIOC_PRIVATE + 3)
#define VCM_IOC_POWER_OFF        _IO('V', BASE_VIDIOC_PRIVATE + 4)

static inline struct ak7377b_device *to_ak7377b_vcm(struct v4l2_ctrl *ctrl)
{
	return container_of(ctrl->handler, struct ak7377b_device, ctrls);
}

static inline struct ak7377b_device *sd_to_ak7377b_vcm(struct v4l2_subdev *subdev)
{
	return container_of(subdev, struct ak7377b_device, sd);
}

struct regval_list {
	unsigned char reg_num;
	unsigned char value;
};


static int ak7377b_set_position(struct ak7377b_device *ak7377b, u16 val)
{
	struct i2c_client *client = v4l2_get_subdevdata(&ak7377b->sd);
	int retry = 3;
	int ret;

	while (--retry > 0) {
		ret = i2c_smbus_write_word_data(client, AK7377B_SET_POSITION_ADDR,
					 swab16(val << 6));
		if (ret < 0) {
			usleep_range(AK7377B_MOVE_DELAY_US,
				     AK7377B_MOVE_DELAY_US + 1000);
			LOG_INF("ak7377b Set postition:%u fail.", val);
		} else {
			break;
		}
	}
	return ret;
}

static int ak7377b_release(struct ak7377b_device *ak7377b)
{
	int ret, val;
	int diff_dac = 0;
	int nStep_count = 0;
	int i = 0;
	struct i2c_client *client = v4l2_get_subdevdata(&ak7377b->sd);

	diff_dac = AK7377B_ORIGIN_FOCUS_POS - ak7377b->focus->val;

	nStep_count = (diff_dac < 0 ? (diff_dac*(-1)) : diff_dac) /
		AK7377B_MOVE_STEPS;

	val = ak7377b->focus->val;

	for (i = 0; i < nStep_count; ++i) {
		val += (diff_dac < 0 ? (AK7377B_MOVE_STEPS*(-1)) : AK7377B_MOVE_STEPS);

		ret = ak7377b_set_position(ak7377b, val);
		if (ret) {
			LOG_INF("%s I2C failure: %d",
				__func__, ret);
			return ret;
		}
		usleep_range(AK7377B_MOVE_DELAY_US,
			     AK7377B_MOVE_DELAY_US + 1000);
	}

	// last step to origin
	ret = ak7377b_set_position(ak7377b, AK7377B_ORIGIN_FOCUS_POS);
	if (ret) {
		LOG_INF("%s I2C failure: %d",
			__func__, ret);
		return ret;
	}

	i2c_smbus_write_byte_data(client, 0x02, 0x20);
	ak7377b->active = false;
	LOG_INF("-\n");

	return 0;
}

static int ak7377b_init(struct ak7377b_device *ak7377b)
{
	struct i2c_client *client = v4l2_get_subdevdata(&ak7377b->sd);
	int ret = 0;

	LOG_INF("+\n");

	client->addr = AK7377B_I2C_SLAVE_ADDR >> 1;
	//ret = i2c_smbus_read_byte_data(client, 0x02);

	LOG_INF("Check HW version: %x\n", ret);

	/* 00:active mode , 10:Standby mode , x1:Sleep mode */
	ret = i2c_smbus_write_byte_data(client, 0x02, 0x00);
	ak7377b->active = true;
	LOG_INF("-\n");

	return 0;
}

/* Power handling */
static int ak7377b_power_off(struct ak7377b_device *ak7377b)
{
	int ret;

	LOG_INF("+\n");

	ret = ak7377b_release(ak7377b);
	if (ret)
		LOG_INF("ak7377b release failed!\n");

	ret = regulator_disable(ak7377b->vin);
	if (ret)
		return ret;

	ret = regulator_disable(ak7377b->vdd);
	if (ret)
		return ret;

	if (ak7377b->vcamaf_pinctrl && ak7377b->vcamaf_off)
		ret = pinctrl_select_state(ak7377b->vcamaf_pinctrl,
					ak7377b->vcamaf_off);

	LOG_INF("-\n");

	return ret;
}

static int ak7377b_power_on(struct ak7377b_device *ak7377b)
{
	int ret;

	LOG_INF("+\n");

	ret = regulator_enable(ak7377b->vin);
	if (ret < 0)
		return ret;

	ret = regulator_enable(ak7377b->vdd);
	if (ret < 0)
		return ret;

	if (ak7377b->vcamaf_pinctrl && ak7377b->vcamaf_on)
		ret = pinctrl_select_state(ak7377b->vcamaf_pinctrl,
					ak7377b->vcamaf_on);

	if (ret < 0)
		return ret;

	/*
	 * TODO(b/139784289): Confirm hardware requirements and adjust/remove
	 * the delay.
	 */
	usleep_range(AK7377B_CTRL_DELAY_US, AK7377B_CTRL_DELAY_US + 100);

	ret = ak7377b_init(ak7377b);
	if (ret < 0)
		goto fail;

	LOG_INF("-\n");

	return 0;

fail:
	regulator_disable(ak7377b->vin);
	regulator_disable(ak7377b->vdd);
	if (ak7377b->vcamaf_pinctrl && ak7377b->vcamaf_off) {
		pinctrl_select_state(ak7377b->vcamaf_pinctrl,
				ak7377b->vcamaf_off);
	}

	return ret;
}

static int ak7377b_set_ctrl(struct v4l2_ctrl *ctrl)
{
	int ret = 0;
	struct ak7377b_device *ak7377b = to_ak7377b_vcm(ctrl);

	if (ctrl->id == V4L2_CID_FOCUS_ABSOLUTE) {
		LOG_INF("pos(%d)\n", ctrl->val);
		ret = ak7377b_set_position(ak7377b, ctrl->val);
		if (ret) {
			LOG_INF("%s I2C failure: %d",
				__func__, ret);
			return ret;
		}
	}
	return 0;
}

static const struct v4l2_ctrl_ops ak7377b_vcm_ctrl_ops = {
	.s_ctrl = ak7377b_set_ctrl,
};

static int ak7377b_open(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	int ret;
	struct ak7377b_device *ak7377b = sd_to_ak7377b_vcm(sd);

	ret = ak7377b_power_on(ak7377b);
	if (ret < 0) {
		LOG_INF("power on fail, ret = %d\n", ret);
		return ret;
	}

	return 0;
}

static int ak7377b_close(struct v4l2_subdev *sd, struct v4l2_subdev_fh *fh)
{
	struct ak7377b_device *ak7377b = sd_to_ak7377b_vcm(sd);

	ak7377b_power_off(ak7377b);

	return 0;
}

static long ak7377b_ops_core_ioctl(struct v4l2_subdev *sd, unsigned int cmd, void *arg)
{
	int ret = 0;
	struct ak7377b_device *ak7377b = sd_to_ak7377b_vcm(sd);
	struct i2c_client *client = v4l2_get_subdevdata(&ak7377b->sd);
	LOG_INF("+\n");
	client->addr = AK7377B_I2C_SLAVE_ADDR >> 1;

	switch (cmd) {

	case VCM_IOC_POWER_ON:
	{
		// customized area
		/* 00:active mode , 10:Standby mode , x1:Sleep mode */
		if (ak7377b->active)
			return 0;
		ret = i2c_smbus_write_byte_data(client, 0x02, 0x00);
		if (ret) {
			LOG_INF("I2C failure!!!\n");
		} else {
			ak7377b->active = true;
			LOG_INF("stand by mode, power on!!!\n");
		}
		LOG_INF("active mode\n");
	}
	break;
	case VCM_IOC_POWER_OFF:
	{
		// customized area
		if (!ak7377b->active)
			return 0;
		ret = i2c_smbus_write_byte_data(client, 0x02, 0x40);
		if (ret) {
			LOG_INF("I2C failure!!!\n");
		} else {
			ak7377b->active = false;
			LOG_INF("stand by mode, power off !!!!!!!!!!!!\n");
		}
		LOG_INF("stand by mode\n");
	}
	break;
	default:
		ret = -ENOIOCTLCMD;
		break;
	}

	LOG_INF("-\n");
	return ret;
}

static const struct v4l2_subdev_internal_ops ak7377b_int_ops = {
	.open = ak7377b_open,
	.close = ak7377b_close,
};

static struct v4l2_subdev_core_ops ak7377b_ops_core = {
	.ioctl = ak7377b_ops_core_ioctl,
};

static const struct v4l2_subdev_ops ak7377b_ops = {
	.core = &ak7377b_ops_core,
};

static void ak7377b_subdev_cleanup(struct ak7377b_device *ak7377b)
{
	v4l2_async_unregister_subdev(&ak7377b->sd);
	v4l2_ctrl_handler_free(&ak7377b->ctrls);
#if IS_ENABLED(CONFIG_MEDIA_CONTROLLER)
	media_entity_cleanup(&ak7377b->sd.entity);
#endif
}

static int ak7377b_init_controls(struct ak7377b_device *ak7377b)
{
	struct v4l2_ctrl_handler *hdl = &ak7377b->ctrls;
	const struct v4l2_ctrl_ops *ops = &ak7377b_vcm_ctrl_ops;

	v4l2_ctrl_handler_init(hdl, 1);

	ak7377b->focus = v4l2_ctrl_new_std(hdl, ops, V4L2_CID_FOCUS_ABSOLUTE,
			  0, AK7377B_MAX_FOCUS_POS, AK7377B_FOCUS_STEPS, 0);

	if (hdl->error)
		return hdl->error;

	ak7377b->sd.ctrl_handler = hdl;

	return 0;
}

static int ak7377b_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct ak7377b_device *ak7377b;
	int ret;

	LOG_INF("+\n");

	ak7377b = devm_kzalloc(dev, sizeof(*ak7377b), GFP_KERNEL);
	if (!ak7377b)
		return -ENOMEM;

	ak7377b->vin = devm_regulator_get(dev, "vin");
	if (IS_ERR(ak7377b->vin)) {
		ret = PTR_ERR(ak7377b->vin);
		if (ret != -EPROBE_DEFER)
			LOG_INF("cannot get vin regulator\n");
		return ret;
	}

	ak7377b->vdd = devm_regulator_get(dev, "vdd");
	if (IS_ERR(ak7377b->vdd)) {
		ret = PTR_ERR(ak7377b->vdd);
		if (ret != -EPROBE_DEFER)
			LOG_INF("cannot get vdd regulator\n");
		return ret;
	}

	ak7377b->vcamaf_pinctrl = devm_pinctrl_get(dev);
	if (IS_ERR(ak7377b->vcamaf_pinctrl)) {
		ret = PTR_ERR(ak7377b->vcamaf_pinctrl);
		ak7377b->vcamaf_pinctrl = NULL;
		LOG_INF("cannot get pinctrl\n");
	} else {
		ak7377b->vcamaf_on = pinctrl_lookup_state(
			ak7377b->vcamaf_pinctrl, "vcamaf_on");

		if (IS_ERR(ak7377b->vcamaf_on)) {
			ret = PTR_ERR(ak7377b->vcamaf_on);
			ak7377b->vcamaf_on = NULL;
			LOG_INF("cannot get vcamaf_on pinctrl\n");
		}

		ak7377b->vcamaf_off = pinctrl_lookup_state(
			ak7377b->vcamaf_pinctrl, "vcamaf_off");

		if (IS_ERR(ak7377b->vcamaf_off)) {
			ret = PTR_ERR(ak7377b->vcamaf_off);
			ak7377b->vcamaf_off = NULL;
			LOG_INF("cannot get vcamaf_off pinctrl\n");
		}
	}

	v4l2_i2c_subdev_init(&ak7377b->sd, client, &ak7377b_ops);
	ak7377b->sd.flags |= V4L2_SUBDEV_FL_HAS_DEVNODE;
	ak7377b->sd.internal_ops = &ak7377b_int_ops;

	ret = ak7377b_init_controls(ak7377b);
	if (ret)
		goto err_cleanup;

#if IS_ENABLED(CONFIG_MEDIA_CONTROLLER)
	ret = media_entity_pads_init(&ak7377b->sd.entity, 0, NULL);
	if (ret < 0)
		goto err_cleanup;

	ak7377b->sd.entity.function = MEDIA_ENT_F_LENS;
#endif

	ret = v4l2_async_register_subdev(&ak7377b->sd);
	if (ret < 0)
		goto err_cleanup;

	LOG_INF("-\n");

	return 0;

err_cleanup:
	ak7377b_subdev_cleanup(ak7377b);
	return ret;
}

static void ak7377b_remove(struct i2c_client *client)
{
	struct v4l2_subdev *sd = i2c_get_clientdata(client);
	struct ak7377b_device *ak7377b = sd_to_ak7377b_vcm(sd);

	LOG_INF("+\n");

	ak7377b_subdev_cleanup(ak7377b);

	LOG_INF("-\n");
}

static const struct i2c_device_id ak7377b_id_table[] = {
	{ AK7377B_NAME, 0 },
	{ },
};
MODULE_DEVICE_TABLE(i2c, ak7377b_id_table);

static const struct of_device_id ak7377b_of_table[] = {
	{ .compatible = "oplus,ak7377b" },
	{ },
};
MODULE_DEVICE_TABLE(of, ak7377b_of_table);

static struct i2c_driver ak7377b_i2c_driver = {
	.driver = {
		.name = AK7377B_NAME,
		.of_match_table = ak7377b_of_table,
	},
	.probe_new  = ak7377b_probe,
	.remove = ak7377b_remove,
	.id_table = ak7377b_id_table,
};

module_i2c_driver(ak7377b_i2c_driver);

MODULE_AUTHOR("XXX");
MODULE_DESCRIPTION("AK7377B VCM driver");
MODULE_LICENSE("GPL v2");
