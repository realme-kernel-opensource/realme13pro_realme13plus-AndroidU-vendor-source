#include <linux/iio/consumer.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/thermal.h>

struct camera_thermal_info {
	struct device *dev;
	struct thermal_zone_device *tz_dev;
	struct iio_channel *channel;
	s32 *lookup_table;
	int nlookup_table;
};

static int camera_thermal_adc_to_temp(struct camera_thermal_info *gti, int val)
{
	int temp, temp_hi, temp_lo, adc_hi, adc_lo;
	int i;

	if (!gti->lookup_table)
		return val;

	for (i = 0; i < gti->nlookup_table; i++) {
		if (val >= gti->lookup_table[2 * i + 1])
			break;
	}

	if (i == 0) {
		temp = gti->lookup_table[0];
	} else if (i >= gti->nlookup_table) {
		temp = gti->lookup_table[2 * (gti->nlookup_table - 1)];
	} else {
		adc_hi = gti->lookup_table[2 * i - 1];
		adc_lo = gti->lookup_table[2 * i + 1];

		temp_hi = gti->lookup_table[2 * i - 2];
		temp_lo = gti->lookup_table[2 * i];

		temp = temp_hi + mult_frac(temp_lo - temp_hi, val - adc_hi,
					   adc_lo - adc_hi);
	}

	return temp;
}

static int camera_thermal_get_temp(struct thermal_zone_device *data, int *temp)
{
	int val;
	int ret;
	struct camera_thermal_info *gti = data->devdata;;


	ret = iio_read_channel_processed(gti->channel, &val);
	if (ret < 0) {
		dev_err(gti->dev, "IIO channel read failed %d\n", ret);
		return ret;
	}
	*temp = camera_thermal_adc_to_temp(gti, val);

	return 0;
}

static const struct thermal_zone_device_ops camera_thermal_ops = {
	.get_temp = camera_thermal_get_temp,
};

static int camera_thermal_read_linear_lookup_table(struct device *dev,
						 struct camera_thermal_info *gti)
{
	struct device_node *np = dev->of_node;
	enum iio_chan_type chan_type;
	int ntable;
	int ret;

	ntable = of_property_count_elems_of_size(np, "temperature-lookup-table",
						 sizeof(u32));
	if (ntable <= 0) {
		ret = iio_get_channel_type(gti->channel, &chan_type);
		if (ret || chan_type != IIO_TEMP)
			dev_notice(dev,
				   "no lookup table, assuming DAC channel returns milliCelcius\n");
		return 0;
	}

	if (ntable % 2) {
		dev_err(dev, "Pair of temperature vs ADC read value missing\n");
		return -EINVAL;
	}

	gti->lookup_table = devm_kcalloc(dev,
					 ntable, sizeof(*gti->lookup_table),
					 GFP_KERNEL);
	if (!gti->lookup_table)
		return -ENOMEM;

	ret = of_property_read_u32_array(np, "temperature-lookup-table",
					 (u32 *)gti->lookup_table, ntable);
	if (ret < 0) {
		dev_err(dev, "Failed to read temperature lookup table: %d\n",
			ret);
		return ret;
	}

	gti->nlookup_table = ntable / 2;

	return 0;
}

static int camera_thermal_probe(struct platform_device *pdev)
{
	struct camera_thermal_info *gti;
	int ret;

	if (!pdev->dev.of_node) {
		dev_err(&pdev->dev, "Only DT based supported\n");
		return -ENODEV;
	}

	gti = devm_kzalloc(&pdev->dev, sizeof(*gti), GFP_KERNEL);
	if (!gti)
		return -ENOMEM;

	gti->channel = devm_iio_channel_get(&pdev->dev, "sensor-channel");
	if (IS_ERR(gti->channel)) {
		ret = PTR_ERR(gti->channel);
		if (ret != -EPROBE_DEFER)
			dev_err(&pdev->dev, "IIO channel not found: %d\n", ret);
		return ret;
	}

	ret = camera_thermal_read_linear_lookup_table(&pdev->dev, gti);
	if (ret < 0)
		return ret;

	gti->dev = &pdev->dev;
	platform_set_drvdata(pdev, gti);

	gti->tz_dev = devm_thermal_of_zone_register(&pdev->dev, 0, gti,
							   &camera_thermal_ops);
	if (IS_ERR(gti->tz_dev)) {
		ret = PTR_ERR(gti->tz_dev);
		if (ret != -EPROBE_DEFER)
			dev_err(&pdev->dev,
				"Thermal zone sensor register failed: %d\n",
				ret);
		return ret;
	}

	return 0;
}

static const struct of_device_id of_camera_thermal_match[] = {
	{ .compatible = "cust-camera-ntc", },
	{},
};
MODULE_DEVICE_TABLE(of, of_camera_thermal_match);

static struct platform_driver camera_thermal_driver = {
	.driver = {
		.name = "cust-camera-ntc",
		.of_match_table = of_camera_thermal_match,
	},
	.probe = camera_thermal_probe,
};

module_platform_driver(camera_thermal_driver);

MODULE_AUTHOR("ZXJ");
MODULE_DESCRIPTION("camera(flashlight) ntc driver using IIO framework with DT");
MODULE_LICENSE("GPL v2");
