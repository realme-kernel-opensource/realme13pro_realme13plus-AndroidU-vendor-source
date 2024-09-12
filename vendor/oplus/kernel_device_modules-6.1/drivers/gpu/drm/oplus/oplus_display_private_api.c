/***************************************************************
** Copyright (C),  2018,  OPLUS Mobile Comm Corp.,  Ltd
** File : oplus_display_private_api.h
** Description : oplus display private api implement
** Version : 1.0
** Date : 2018/03/20
**
** ------------------------------- Revision History: -----------
**  <author>        <data>        <version >        <desc>
**   Hu.Jie          2018/03/20        1.0           Build this moudle
**   Guo.Ling        2018/10/11        1.1           Modify for SDM660
**   Guo.Ling        2018/11/27        1.2           Modify for mt6779
**   Li.Ping         2019/11/18        1.3           Modify for mt6885
******************************************************************/
#include "oplus_display_private_api.h"
#include "oplus_display_trace.h"
#include "mtk_disp_aal.h"
#include "oplus_display_panel_power.h"
#include <linux/notifier.h>
#include <linux/kobject.h>
#include <linux/signal.h>
#include <linux/leds-mtk.h>
#include "mtk_drm_drv.h"
#include <soc/oplus/system/oplus_project.h>
#include "oplus_adfr.h"

#ifdef OPLUS_FEATURE_DISPLAY_TEMP_COMPENSATION
#include "oplus_display_temp_compensation.h"
#endif /* OPLUS_FEATURE_DISPLAY_TEMP_COMPENSATION */

#ifdef OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT
#include "oplus_display_onscreenfingerprint.h"
#endif /* OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT */

#include "oplus_display_high_frequency_pwm.h"

#ifdef OPLUS_TRACKPOINT_REPORT
#include "oplus_display_trackpoint_report.h"
#define EXCEPTION_TRACKPOINT_REPORT(fmt, ...)	\
	do { \
			pr_err(fmt, ##__VA_ARGS__); \
			display_exception_trackpoint_report(fmt, ##__VA_ARGS__); \
		} while (0)
#endif /* OPLUS_TRACKPOINT_REPORT */

/*
 * we will create a sysfs which called /sys/kernel/oplus_display,
 * In that directory, oplus display private api can be called
 */
#define PANEL_SERIAL_NUM_REG 0xA1
#define PANEL_SERIAL_NUM_REG_SUMSUNG 0xD8
#define PANEL_REG_READ_LEN   10
#define BOE_PANEL_SERIAL_NUM_REG 0xA3
#define PANEL_SERIAL_NUM_REG_TIANMA 0xD7
#define PANEL_REG_READ_LEN_TIANMA   7

#define PANEL_TX_MAX_BUF 256

#define OPLUS_ATTR(_name, _mode, _show, _store) \
struct kobj_attribute oplus_attr_##_name = __ATTR(_name, _mode, _show, _store)

extern void oplus_mtk_read_ddic_v2(u8 ddic_reg, int ret_num, char ret_val[10]);
extern void oplus_mtk_read_ddic_v3(u8 ddic_reg, int ret_num, char ret_val[20]);
extern void oplus_ddic_dsi_send_cmd(unsigned int cmd_num, char val[20]);

uint64_t serial_number = 0x0;
EXPORT_SYMBOL(serial_number);
/***  oplus serial number (sn) dtsi config   *****************
oplus,dsi-serial-number-enabled=<1>;
oplus,dsi-serial-number-switch-page;
oplus,dsi-serial-number-index= <0>;
oplus,dsi-serial-number-switch-page-command= <0xFF 0x08 0x38 0x1D>;
oplus,dsi-serial-number-reg= <0x82>;
oplus,dsi-serial-number-default-switch-page-command= <0xFF 0x08 0x38 0x00>;
oplus,dsi-serial-number-read-count= <7>;
********************************************************/

/* oplus serial number initialize params ***************/
struct oplus_serial_number_params {
	unsigned int   config;									/* bit(0):enable temp compensation function, bit(1) and other spare no used */
	bool   page_config;										/* sn read before need switch page config */
	unsigned int   index;									/* sn read reg index, after reading the register, the starting position of conversion is required */
	unsigned int   *switch_page_command;					/* the switch page parsed from dtsi, if (oplus,dsi-serial-number-switch-page) config */
	unsigned char  switch_page_command_count;				/* the count of switch page parsed(sizeof(switch_page)+1) */
	unsigned int   *default_page_command;					/* the default page parsed from dtsi, panel default page, if (oplus,dsi-serial-number-switch-page) config */
	unsigned char  default_page_command_count;				/* the count of default page parsed(sizeof(default_page)+1) */
	unsigned int   read_reg;								/* the read reg parsed from dtsi */
	unsigned int   read_count;								/* the count of read reg from dtsi */
};

static struct oplus_serial_number_params g_oplus_serial_number_params = {0};
static struct oplus_serial_number_params *oplus_serial_number_get_params(void)
{
	return &g_oplus_serial_number_params;
}

struct aod_area oplus_aod_area[RAMLESS_AOD_AREA_NUM];
char send_cmd[RAMLESS_AOD_PAYLOAD_SIZE];

typedef struct panel_serial_info
{
	int reg_index;
	uint64_t year;
	uint64_t month;
	uint64_t day;
	uint64_t hour;
	uint64_t minute;
	uint64_t second;
	uint64_t reserved[2];
} PANEL_SERIAL_INFO;

int trig_db_enable = 0;
EXPORT_SYMBOL(trig_db_enable);
extern struct drm_device *get_drm_device(void);
extern int mtk_drm_setbacklight(struct drm_crtc *crtc, unsigned int level, unsigned int panel_ext_param, unsigned int cfg_flag, unsigned int lock);
#ifdef OPLUS_FEATURE_DISPLAY_APOLLO
extern int mtk_drm_setbacklight_without_lock(struct drm_crtc *crtc, unsigned int level, unsigned int panel_ext_param, unsigned int cfg_flag);
extern void apollo_set_brightness_for_show(unsigned int level);
#endif /* OPLUS_FEATURE_DISPLAY_APOLLO */
extern int oplus_mtk_drm_setseed(struct drm_crtc *crtc, unsigned int seed_mode);
extern PANEL_VOLTAGE_BAK panel_vol_bak[PANEL_VOLTAGE_ID_MAX];
unsigned int esd_mode = 0;
EXPORT_SYMBOL(esd_mode);
unsigned int seed_mode = 0;
extern int mtk_crtc_osc_freq_switch(struct drm_crtc *crtc, unsigned int en, unsigned int userdata);
unsigned long osc_mode = 0;
EXPORT_SYMBOL(osc_mode);

unsigned int oplus_display_brightness = 0;

bool pq_trigger = true;
bool atomic_set_bl_en = false;
unsigned int backup_bl_level;
EXPORT_SYMBOL(pq_trigger);
EXPORT_SYMBOL(atomic_set_bl_en);
EXPORT_SYMBOL(backup_bl_level);

int oplus_dsi_log_type = OPLUS_DEBUG_LOG_DISABLED;
EXPORT_SYMBOL(oplus_dsi_log_type);
unsigned int oplus_display_trace_enable = OPLUS_DISPLAY_DISABLE_TRACE;
EXPORT_SYMBOL(oplus_display_trace_enable);

unsigned int oplus_max_normal_brightness = 0;
unsigned int oplus_enhance_mipi_strength = 0;
unsigned int m_da;
unsigned int m_db;
EXPORT_SYMBOL(m_db);
unsigned int m_dc;
EXPORT_SYMBOL(m_dc);
unsigned int m_id;
EXPORT_SYMBOL(m_id);
unsigned char gamma93_120[5] = {0};
unsigned char gamma94_120[5] = {0};
unsigned char gamma93_90[5] = {0};
unsigned char gamma94_90[5] = {0};
EXPORT_SYMBOL(gamma93_120);
EXPORT_SYMBOL(gamma94_120);
EXPORT_SYMBOL(gamma93_90);
EXPORT_SYMBOL(gamma94_90);

bool g_dp_support;
char lcm_version[32] = "\0";
char lcm_manufacture[32] = "\0";
EXPORT_SYMBOL(lcm_version);
EXPORT_SYMBOL(lcm_manufacture);

bool oplus_hbm_max_en;
EXPORT_SYMBOL(oplus_hbm_max_en);


extern int oplus_mtk_drm_setcabc(struct drm_crtc *crtc, unsigned int hbm_mode);
extern int oplus_display_get_softiris_color_status(void *buf);
extern ssize_t oplus_get_hbm_max_debug(struct kobject *obj,
	struct kobj_attribute *attr, char *buf);
extern ssize_t oplus_set_hbm_max_debug(struct kobject *obj,
		struct kobj_attribute *attr,
		const char *buf, size_t count);

static BLOCKING_NOTIFIER_HEAD(lcdinfo_notifiers);
int register_lcdinfo_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&lcdinfo_notifiers, nb);
}
int unregister_lcdinfo_notifier(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&lcdinfo_notifiers, nb);
}

EXPORT_SYMBOL(register_lcdinfo_notifier);
EXPORT_SYMBOL(unregister_lcdinfo_notifier);

void lcdinfo_notify(unsigned long val, void *v)
{
	blocking_notifier_call_chain(&lcdinfo_notifiers, val, v);
}
EXPORT_SYMBOL(lcdinfo_notify);

static ssize_t oplus_display_get_brightness(struct kobject *obj,
                                struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", oplus_display_brightness);
}

static ssize_t oplus_display_set_brightness(struct kobject *obj,
		struct kobj_attribute *attr, const char *buf, size_t num)
{
	int ret;
	struct drm_crtc *crtc;
	struct drm_device *ddev = get_drm_device();
	unsigned int oplus_set_brightness = 0;

	ret = kstrtouint(buf, 10, &oplus_set_brightness);

	printk("%s %d\n", __func__, oplus_set_brightness);

	if (oplus_set_brightness > OPLUS_MAX_BRIGHTNESS || oplus_set_brightness < OPLUS_MIN_BRIGHTNESS) {
		printk(KERN_ERR "%s, brightness:%d out of scope\n", __func__, oplus_set_brightness);
		return num;
	}

	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
				typeof(*crtc), head);
	if (!crtc) {
		printk(KERN_ERR "find crtc fail\n");
		return 0;
	}
	mtk_drm_setbacklight(crtc, oplus_set_brightness, 0, 0x1 << SET_BACKLIGHT_LEVEL, 1);

	return num;
}

static ssize_t oplus_display_get_max_brightness(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", OPLUS_MAX_BRIGHTNESS);
}

static ssize_t oplus_display_get_maxbrightness(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", oplus_max_normal_brightness);
}

static ssize_t oplus_display_set_panel_pwr(struct kobject *obj,
                               struct kobj_attribute *attr,
                               const char *buf, size_t num)
{
	u32 panel_vol_value = 0, panel_vol_id = 0;
	int rc = 0;
	struct drm_crtc *crtc;
	struct mtk_drm_crtc *mtk_crtc;
	struct drm_device *ddev = get_drm_device();
	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
		typeof(*crtc), head);
	if (!crtc) {
		DDPPR_ERR("get hbm find crtc fail\n");
		return 0;
	}
	mtk_crtc = to_mtk_crtc(crtc);

	sscanf(buf, "%d %d", &panel_vol_id, &panel_vol_value);
	panel_vol_id = panel_vol_id & 0x0F;

	pr_err("debug for %s start\n", __func__);

	if (panel_vol_id < 0 || panel_vol_id >= PANEL_VOLTAGE_ID_MAX) {
		return -EINVAL;
	}

	if (panel_vol_value < panel_vol_bak[panel_vol_id].voltage_min ||
		panel_vol_id > panel_vol_bak[panel_vol_id].voltage_max) {
		return -EINVAL;
	}

	if (panel_vol_id == PANEL_VOLTAGE_ID_VG_BASE) {
		pr_err("%s: set the VGH_L pwr = %d \n", __func__, panel_vol_value);
		rc = oplus_panel_set_vg_base(panel_vol_value);
		if (rc < 0) {
			return rc;
		}

		return num;
	}

	if (mtk_crtc->panel_ext->funcs->oplus_set_power) {
		rc = mtk_crtc->panel_ext->funcs->oplus_set_power(panel_vol_id, panel_vol_value);
		if (rc) {
			pr_err("Set voltage(%s) fail, rc=%d\n",
				 __func__, rc);
			return -EINVAL;
		}
	}
	pr_err("debug for %s end\n", __func__);

	return num;
}

static ssize_t oplus_display_get_panel_pwr(struct kobject *obj,
	struct kobj_attribute *attr, char *buf) {
	int ret = 0;
	u32 i = 0;
	struct drm_crtc *crtc;
	struct mtk_drm_crtc *mtk_crtc;
	struct drm_device *ddev = get_drm_device();
	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
		typeof(*crtc), head);
	if (!crtc) {
		DDPPR_ERR("get hbm find crtc fail\n");
		return 0;
	}
	mtk_crtc = to_mtk_crtc(crtc);

	for (i = 0; i < (PANEL_VOLTAGE_ID_MAX-1); i++) {
		if (mtk_crtc->panel_ext->funcs->oplus_update_power_value) {
			ret = mtk_crtc->panel_ext->funcs->oplus_update_power_value(panel_vol_bak[i].voltage_id);
		}

		if (ret < 0) {
			pr_err("%s : update_current_voltage error = %d\n", __func__, ret);
		}
		else {
			panel_vol_bak[i].voltage_current = ret;
		}
	}

	return sprintf(buf, "%d %d %d %d %d %d %d %d %d %d %d %d\n",
		panel_vol_bak[0].voltage_id, panel_vol_bak[0].voltage_min,
		panel_vol_bak[0].voltage_current, panel_vol_bak[0].voltage_max,
		panel_vol_bak[1].voltage_id, panel_vol_bak[1].voltage_min,
		panel_vol_bak[1].voltage_current, panel_vol_bak[1].voltage_max,
		panel_vol_bak[2].voltage_id, panel_vol_bak[2].voltage_min,
		panel_vol_bak[2].voltage_current, panel_vol_bak[2].voltage_max);
}

static ssize_t oplus_display_get_dsi_log_switch(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "\
		dynamic conctrl debug log, 0x0 --> disable all debug log\n \
		1 -> enable  0-> disable\n \
		BIT(0) --> dump register log\n \
		BIT(1) --> backlight log\n \
		BIT(2) --> common log\n \
		BIT(3) --> ofp log\n \
		BIT(4) --> adfr log\n \
		BIT(5) --> temp compensation log\n \
		current value:0x%x\n", oplus_dsi_log_type);
}

static ssize_t oplus_display_set_dsi_log_switch(struct kobject *obj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%x", &oplus_dsi_log_type);
	pr_err("debug for %s, buf = [%s], oplus_dsi_log_type = %d , count = %d\n",
			__func__, buf, oplus_dsi_log_type, (int)count);

	return count;
}

static ssize_t oplus_display_get_trace_enable_attr(struct kobject *obj,
	struct kobj_attribute *attr, char *buf)
{
	if (!buf) {
		pr_err("Invalid params\n");
		return -EINVAL;
	}

	return sprintf(buf, "dynamic trace enable\n \
		0x0 --> disable all trace\n \
		BIT(0) --> enable ofp trace\n \
		BIT(1) --> enable adfr trace\n \
		BIT(2) --> enable temp compensation trace\n \
		current value:0x%x\n", oplus_display_trace_enable);
}

static ssize_t oplus_display_set_trace_enable_attr(struct kobject *obj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	if (!buf) {
		pr_err("Invalid params\n");
		return count;
	}

	sscanf(buf, "%u", &oplus_display_trace_enable);
	pr_info("oplus_display_trace_enable:0x%x\n", oplus_display_trace_enable);

	return count;
}

static ssize_t oplus_display_get_disp_trig_db(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", trig_db_enable);
}

static ssize_t oplus_display_set_disp_trig_db(struct kobject *obj,
		struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d", &trig_db_enable);
	pr_err("debug for %s, trig_db_enable = %d\n", __func__, trig_db_enable);

	if (get_eng_version() == AGING) {
		pr_err("disp_trig_db, AGING version\n");
	} else {
		pr_err("disp_trig_db, normal version\n");
	}

	return count;
}
/*#endif*/


int oplus_serial_number_probe(struct device *dev)
{
	int i = 0;
	int rc = 0;
	int length = 0;
	u32 config = 0;
	u32 sn_index = 0;
	u32 sn_reg = 0;
	u32 sn_read_count = 0;
	struct oplus_serial_number_params *serial_number_params = oplus_serial_number_get_params();

	rc = of_property_read_u32(dev->of_node, "oplus,dsi-serial-number-enabled", &config);
	if (rc == 0) {
		serial_number_params->config = config;
		pr_info("config = %d, oplus_serial_number_config = %d\n", config, serial_number_params->config);
	} else {
		serial_number_params->config = false;
		pr_info("oplus_serial_number_config = %d\n", serial_number_params->config);
	}

	if(serial_number_params->config == 0) {
		pr_info("this panel not support serial number\n");
		goto error;
	}

	if (of_property_read_bool(dev->of_node, "oplus,dsi-serial-number-switch-page")) {
		serial_number_params->page_config = true;

		length = of_property_count_elems_of_size(dev->of_node, "oplus,dsi-serial-number-switch-page-command", sizeof(unsigned int));
		pr_info("oplus,dsi-serial-number-switch-page-command length = %d\n", length);
		serial_number_params->switch_page_command = kzalloc(length * sizeof(unsigned int), GFP_KERNEL);
		if (!serial_number_params->switch_page_command) {
			TEMP_COMPENSATION_ERR("failed to kzalloc switch_page_command\n");
			rc = -ENOMEM;
			goto error_free_switch_page;
		}
		rc = of_property_read_u32_array(dev->of_node, "oplus,dsi-serial-number-switch-page-command", serial_number_params->switch_page_command, length);
		if (rc == 0) {
			for (i = 0; i < length; i++) {
				pr_info("%s serial_number_switch_page_command[%d]=0x%x\n",
					__func__, i, serial_number_params->switch_page_command[i]);
			}
		}
		serial_number_params->switch_page_command_count = length;

		length = of_property_count_elems_of_size(dev->of_node, "oplus,dsi-serial-number-default-switch-page-command", sizeof(unsigned int));
		pr_info("oplus,dsi-serial-number-default-switch-page-command length = %d\n", length);
		serial_number_params->default_page_command = kzalloc(length * sizeof(unsigned int), GFP_KERNEL);
		if (!serial_number_params->default_page_command) {
			TEMP_COMPENSATION_ERR("failed to kzalloc default_page_command\n");
			rc = -ENOMEM;
			goto error_free_default_page;
		}
		rc = of_property_read_u32_array(dev->of_node, "oplus,dsi-serial-number-default-switch-page-command", serial_number_params->default_page_command, length);
		if (rc == 0) {
			for (i = 0; i < length; i++) {
				pr_info("%s serial_number_default_switch_page_command[%d]=0x%x\n",
					__func__, i, serial_number_params->default_page_command[i]);
			}
		}
		serial_number_params->default_page_command_count = length;
	} else {
		serial_number_params->page_config = false;
	}

	rc = of_property_read_u32(dev->of_node, "oplus,dsi-serial-number-index", &sn_index);
	if (rc == 0) {
		serial_number_params->index = sn_index;
		pr_info("oplus_serial_number_params->index = %d\n", sn_index);
	} else {
		serial_number_params->index = 0;
		pr_info("default, oplus_serial_number_params->index = %d\n", serial_number_params->index);
	}

	rc = of_property_read_u32(dev->of_node, "oplus,dsi-serial-number-reg", &sn_reg);
	if (rc == 0) {
		serial_number_params->read_reg = sn_reg;
		pr_info("oplus_serial_number_params->read_reg, read read_reg = 0x%x\n", sn_reg);
	} else {
		serial_number_params->read_reg = 0;
		pr_info("oplus_serial_number_params->read_reg = 0x%x\n", serial_number_params->read_reg);
	}

	rc = of_property_read_u32(dev->of_node, "oplus,dsi-serial-number-read-count", &sn_read_count);
	if (rc == 0) {
		serial_number_params->read_count = sn_read_count;
		pr_info("oplus_serial_number_params->read_count = %d\n", sn_read_count);
	} else {
		serial_number_params->read_count = 0;
		pr_info("oplus_serial_number_params->read_count = %d\n", serial_number_params->read_count);
	}

	pr_info("oplus_serial_number_probe successful end\n");
	return 0;

error_free_default_page:
	kfree(serial_number_params->default_page_command);
	serial_number_params->default_page_command = NULL;

error_free_switch_page:
	kfree(serial_number_params->switch_page_command);
	serial_number_params->switch_page_command = NULL;

error:
	serial_number_params->config = 0;
	pr_info("error: oplus_serial_number_params config:0x%x\n", serial_number_params->config);
	return rc;
}

EXPORT_SYMBOL(oplus_serial_number_probe);

int panel_id_read(struct drm_crtc *crtc)
{
	struct mtk_ddp_comp *comp;
	struct mtk_drm_crtc *mtk_crtc;
	char *panel_name;
	char para[20] = {0};
	char switch_page_1d[4] = {0xFF, 0x08, 0x38, 0x1D};
	char switch_default_page[4] = {0xFF, 0x08, 0x38, 0x00};

	mtk_crtc = to_mtk_crtc(crtc);
	if (!mtk_crtc) {
		pr_err("[oplus]cannot get crtc\n");
		return 0;
	}
	comp = mtk_ddp_comp_request_output(mtk_crtc);
	if (!(mtk_crtc->enabled)) {
		pr_err("[oplus]crtc not enable\n");
		return 0;
	}

	if (!comp || !comp->funcs || !comp->funcs->io_cmd) {
		pr_err("[oplus]cannot find output component\n");
		return 0;
	}
	if (!mtk_drm_lcm_is_connect(mtk_crtc)) {
		pr_err("[oplus]lcm is not connect\n");
		return 0;
	}

	mtk_ddp_comp_io_cmd(comp, NULL, GET_PANEL_NAME, &panel_name);
	pr_info("[oplus]panelname=%s\n", panel_name);

	pr_info("[oplus]%s, 0xDA,0xDB,0xDC=0, get id\n", __func__);
	oplus_ddic_dsi_send_cmd(4, switch_default_page);
	oplus_mtk_read_ddic_v2(0xDA, 5, para);
	m_da = para[0] & 0xFF;

	oplus_mtk_read_ddic_v2(0xDB, 5, para);
	m_db = para[0] & 0xFF;

	oplus_mtk_read_ddic_v2(0xDC, 5, para);
	m_dc = para[0] & 0xFF;
	if (!strcmp(panel_name, "panel_ac094_p_b_a0004_dsi_cmd")) {
		oplus_ddic_dsi_send_cmd(4, switch_page_1d);
		oplus_mtk_read_ddic_v2(0x81, 5, para);
		oplus_ddic_dsi_send_cmd(4, switch_default_page);
		m_id = para[0] & 0xFF;
	}

	if (!strcmp(panel_name, "panel_ac094_p_3_a0004_dsi_cmd")) {
		if (!m_da || !m_db) {
			pr_err("[oplus]%s: read id fail, set 0xDA=0x3f, 0xDB=0x7, 0xDC=0x0\n", __func__);
			m_da = 0x3F;
			m_db = 0x7;
			m_dc = 0x0;
		}
	}
	pr_info("[oplus]%s: 0xDA=0x%x, 0xDB=0x%x, 0xDC=0x%x, 0x81=0x%x\n", __func__, m_da, m_db, m_dc, m_id);

#ifdef OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT
	if (oplus_ofp_is_supported()) {
		oplus_ofp_fp_type_compatible_mode_config();
	}
#endif /* OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT */

	return 0;
}

int panel_serial_number_read(struct drm_crtc *crtc, char cmd, int num)
{
	char para[20] = {0};
	char *panel_name;
	int count = 10;
	int i = 0;
	char switch_page_reg[20] = {0};
	PANEL_SERIAL_INFO panel_serial_info;
	struct mtk_ddp_comp *comp;
	struct mtk_drm_crtc *mtk_crtc;
	struct oplus_serial_number_params *serial_number_params = oplus_serial_number_get_params();

	mtk_crtc = to_mtk_crtc(crtc);
	if (!mtk_crtc) {
		pr_err("[panel_serial_number_read]cannot get crtc\n");
		return 0;
	}
	comp = mtk_ddp_comp_request_output(mtk_crtc);
	if (!(mtk_crtc->enabled)) {
		pr_err("[panel_serial_number_read]Sleep State set backlight stop --crtc not ebable\n");
		return 0;
	}

	if (!comp || !comp->funcs || !comp->funcs->io_cmd) {
		pr_err("[panel_serial_number_read]cannot find output component\n");
		return 0;
	}
	if (!mtk_drm_lcm_is_connect(mtk_crtc)) {
		pr_err("[oplus][panel_serial_number_read]lcm is not connect\n");
		return 0;
	}

	if(serial_number_params->config == 0) {
		pr_err("[oplus][panel_serial_number_read]not support\n");
		return 0;
	}

	mtk_ddp_comp_io_cmd(comp, NULL, GET_PANEL_NAME, &panel_name);
	pr_info("[oplus]panelname=%s\n", panel_name);

	if (mtk_crtc && mtk_crtc->panel_ext && mtk_crtc->panel_ext->params
		&& mtk_crtc->panel_ext->params->oplus_serial_para0) {
			cmd = mtk_crtc->panel_ext->params->oplus_serial_para0;
	}

	if (!strcmp(panel_name, "oplus21007_samsung_amb670yf01_qhd_dsi_cmd")) {
		g_dp_support = true;
	} else {
		g_dp_support = false;
	}

	if (serial_number_params->page_config) {
		for (i = 0; i < serial_number_params->switch_page_command_count; i++) {
			switch_page_reg[i] = (char)serial_number_params->switch_page_command[i];
		}
		oplus_ddic_dsi_send_cmd(serial_number_params->switch_page_command_count,switch_page_reg);
	}

	if (!strcmp(panel_name, "ac158_p_b_a0012_cmd_panel")) {
		para[0] = 0x00;
	}

	while (count > 0) {

		oplus_mtk_read_ddic_v2(serial_number_params->read_reg, serial_number_params->read_count, para);

		count--;

		if (!strcmp(panel_name, "oplus21121_boe_rm692e5_fhd_dsi_cmd")) {
			panel_serial_info.reg_index = 8;
			panel_serial_info.year      = ((para[panel_serial_info.reg_index] & 0xF0) >> 4) + 1;
		} else if (!strcmp(panel_name, "AC068_P_3_A0001_fhd_cmd")
			|| !strcmp(panel_name, "oplus21121_boe_nt37701a_fhd_dsi_cmd")
		|| !strcmp(panel_name, "oplus21861_boe_nt37701a_fhd_dsi_cmd")) {
			panel_serial_info.reg_index = 0;
			panel_serial_info.year      = ((para[panel_serial_info.reg_index] & 0xF0) >> 4) + 1;
		} else if (!strcmp(panel_name, "oplus21135_tm_nt37705_fhd_dsi_cmd")
		|| !strcmp(panel_name, "ac164_p_7_a0001_cmd_panel")
		|| !strcmp(panel_name, "ac164_p_3_a0001_cmd_panel")
		|| !strcmp(panel_name, "ac158_p_7_a0001_cmd_panel")) {
			panel_serial_info.reg_index = 0;
			panel_serial_info.year      = ((para[panel_serial_info.reg_index] & 0xF0) >> 4) + 10;
		} else if (!strcmp(panel_name, "oplus22023_samsung_amf680cm01_fhd_dsi_cmd")) {
			panel_serial_info.reg_index = 0;
			panel_serial_info.year      = (para[panel_serial_info.reg_index] & 0xF0) >> 4;
		} else {
			panel_serial_info.reg_index = 4;
			panel_serial_info.year      = (para[panel_serial_info.reg_index] & 0xF0) >> 4;
		}

		if (serial_number_params->page_config) {
			panel_serial_info.reg_index = serial_number_params->index;
			panel_serial_info.year      = (para[panel_serial_info.reg_index] & 0xF0) >> 4;
		}

		panel_serial_info.month     = para[panel_serial_info.reg_index] & 0x0F;
		panel_serial_info.day       = para[panel_serial_info.reg_index + 1] & 0x3F;
		panel_serial_info.hour      = para[panel_serial_info.reg_index + 2] & 0x3F;
		panel_serial_info.minute    = para[panel_serial_info.reg_index + 3] & 0x3F;
		panel_serial_info.second    = para[panel_serial_info.reg_index + 4] & 0x3F;
		panel_serial_info.reserved[0] = para[panel_serial_info.reg_index + 5];
		panel_serial_info.reserved[1] = para[panel_serial_info.reg_index + 6];

		if (!strcmp(panel_name, "ac158_p_b_a0012_cmd_panel")) {
			oplus_mtk_read_ddic_v2(0x00, 5, para);
			panel_serial_info.year    = (para[0] & 0xF0) >> 4;
			panel_serial_info.month   =  para[0] & 0x0F;
			oplus_mtk_read_ddic_v2(0x01, 5, para);
			panel_serial_info.day     =  para[0] & 0x1F;
			oplus_mtk_read_ddic_v2(0x02, 5, para);
			panel_serial_info.hour    =  para[0] & 0x1F;
			oplus_mtk_read_ddic_v2(0x03, 5, para);
			panel_serial_info.minute  =  para[0] & 0x3F;
			oplus_mtk_read_ddic_v2(0x04, 5, para);
			panel_serial_info.second  =  para[0] & 0x3F;
			oplus_mtk_read_ddic_v2(0x05, 5, para);
			panel_serial_info.reserved[0] = para[0] & 0xFF;
			oplus_mtk_read_ddic_v2(0x06, 5, para);
			panel_serial_info.reserved[1] = para[0] & 0xFF;
		}

		serial_number = (panel_serial_info.year     << 56)\
							+ (panel_serial_info.month      << 48)\
							+ (panel_serial_info.day        << 40)\
							+ (panel_serial_info.hour       << 32)\
							+ (panel_serial_info.minute << 24)\
							+ (panel_serial_info.second << 16)\
							+ (panel_serial_info.reserved[0] << 8)\
							+ (panel_serial_info.reserved[1]);
		pr_info("[oplus]%s year:0x%llx, month:0x%llx, day:0x%llx, hour:0x%llx, minute:0x%llx, second:0x%llx, msecond:0x%llx, 0x%llx!\n",
						__func__,
						panel_serial_info.year,
						panel_serial_info.month,
						panel_serial_info.day,
						panel_serial_info.hour,
						panel_serial_info.minute,
						panel_serial_info.second,
						panel_serial_info.reserved[0],
						panel_serial_info.reserved[1]);
		if (panel_serial_info.year <= 7) {
			continue;
		} else {
			printk("%s year:0x%llx, month:0x%llx, day:0x%llx, hour:0x%llx, minute:0x%llx, second:0x%llx,  msecond:0x%llx, 0x%llx!\n",
			            __func__,
			            panel_serial_info.year,
			            panel_serial_info.month,
			            panel_serial_info.day,
			            panel_serial_info.hour,
			            panel_serial_info.minute,
			            panel_serial_info.second,
			            panel_serial_info.reserved[0],
			            panel_serial_info.reserved[1]);
			break;
		}
	}

	if (serial_number_params->page_config) {
		for (i = 0; i < serial_number_params->switch_page_command_count; i++) {
			switch_page_reg[i] = (char)serial_number_params->default_page_command[i];
		}
		oplus_ddic_dsi_send_cmd(serial_number_params->default_page_command_count, switch_page_reg);
	}

	pr_err("[oplus]%s Get panel serial number=[%llx]\n", __func__, serial_number);

	return 1;
}

static ssize_t oplus_get_panel_serial_number(struct kobject *obj,
                struct kobj_attribute *attr, char *buf)
{
	struct drm_crtc *crtc;
	struct drm_device *ddev = get_drm_device();
	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
		typeof(*crtc), head);
	if (!crtc) {
		DDPPR_ERR("get_panel_serial_number find crtc fail\n");
		return 0;
	}

	if (oplus_display_brightness == 0) {
		pr_info("backlight is 0, skip get serial number!\n");
		return 0;
	}

	if (serial_number == 0) {
		printk(KERN_ERR "dsi_cmd %s Failed. serial_number == 0,run panel_serial_number_read()\n", __func__);
		panel_serial_number_read(crtc, PANEL_SERIAL_NUM_REG, PANEL_REG_READ_LEN);
		printk(KERN_ERR "%s .after read, serial_number: %llx, da=0x%x, db=0x%x, dc=0x%x\n", __func__, serial_number, m_da, m_db, m_dc);
	}

	return scnprintf(buf, PAGE_SIZE, "Get panel serial number: %llx\n", serial_number);
}

int panel_gamma_read(struct drm_crtc *crtc)
{
	char *panel_name;
	int i = 0;
	char switch_page_reg[20] = {0};
	char switch_page_2F[4] = {0xFF, 0x08, 0x38, 0x2F};
	char switch_gamma_hz[2] = {0x99, 0x18};
	struct mtk_ddp_comp *comp;
	struct mtk_drm_crtc *mtk_crtc;
	struct oplus_serial_number_params *serial_number_params = oplus_serial_number_get_params();

	mtk_crtc = to_mtk_crtc(crtc);
	if (!mtk_crtc) {
		pr_err("[panel_gamma_read]cannot get crtc\n");
		return 0;
	}
	comp = mtk_ddp_comp_request_output(mtk_crtc);
	if (!(mtk_crtc->enabled)) {
		pr_err("[panel_gamma_read]Sleep State set backlight stop --crtc not ebable\n");
		return 0;
	}

	if (!comp || !comp->funcs || !comp->funcs->io_cmd) {
		pr_err("[panel_gamma_read]cannot find output component\n");
		return 0;
	}
	if (!mtk_drm_lcm_is_connect(mtk_crtc)) {
		pr_err("[oplus][panel_gamma_read]lcm is not connect\n");
		return 0;
	}

	if(serial_number_params->config == 0) {
		pr_err("[oplus][panel_gamma_read]not support\n");
		return 0;
	}

	mtk_ddp_comp_io_cmd(comp, NULL, GET_PANEL_NAME, &panel_name);
	pr_info("[panel_gamma_read]panelname=%s\n", panel_name);
	if (!strcmp(panel_name, "panel_ac094_p_3_a0004_dsi_cmd") && (m_db == 0x07 && m_dc == 0)) {
		oplus_ddic_dsi_send_cmd(4, switch_page_2F);
		oplus_ddic_dsi_send_cmd(2, switch_gamma_hz);
		oplus_mtk_read_ddic_v2(0x93, 5, gamma93_120);
		oplus_mtk_read_ddic_v2(0x94, 5, gamma94_120);
		switch_gamma_hz[1] = 0x24;
		oplus_ddic_dsi_send_cmd(2, switch_gamma_hz);
		oplus_mtk_read_ddic_v2(0x93, 5, gamma93_90);
		oplus_mtk_read_ddic_v2(0x94, 5, gamma94_90);
		switch_gamma_hz[1] = 0x00;
		oplus_ddic_dsi_send_cmd(2, switch_gamma_hz);
		if (serial_number_params->page_config) {
			for (i = 0; i < serial_number_params->switch_page_command_count; i++) {
				switch_page_reg[i] = (char)serial_number_params->default_page_command[i];
			}
			oplus_ddic_dsi_send_cmd(serial_number_params->default_page_command_count, switch_page_reg);
		}
		for (i = 0; i < 5; i++) {
			pr_debug("[oplus]%s: 120-%d: 0x93=0x%x, 0x94=0x%x\n", __func__, i, gamma93_120[i], gamma94_120[i]);
			pr_debug("[oplus]%s: 90-%d: 0x93=0x%x, 0x94=0x%x\n", __func__, i, gamma93_90[i], gamma94_90[i]);
		}
	}
	return 0;
}
EXPORT_SYMBOL(panel_gamma_read);

static ssize_t panel_serial_store(struct kobject *obj,
                struct kobj_attribute *attr,
                const char *buf, size_t count)
{
        printk("[soso] Lcm read 0xA1 reg = 0x%llx\n", serial_number);
        return count;
}

static ssize_t oplus_set_panel_reg(struct kobject *obj,
                struct kobj_attribute *attr, const char *buf,
		size_t count)
{
	u32 value = 0;
	u32 step = 0;
	int len = 0;
	int i = 0;
	char *bufp = (char *)buf;
	char para[PANEL_TX_MAX_BUF] = {0};
	struct drm_crtc *crtc;
	struct drm_device *ddev = get_drm_device();
	struct mtk_ddp_comp *comp;
	struct mtk_drm_crtc *mtk_crtc;
	char read;

	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
		typeof(*crtc), head);
	if (!crtc) {
		DDPPR_ERR("get panel reg find crtc fail\n");
		return 0;
	}

	mtk_crtc = to_mtk_crtc(crtc);
	comp = mtk_ddp_comp_request_output(mtk_crtc);

	if (!(mtk_crtc->enabled)) {
		pr_err("[lh]Sleep State get panel reg stop --crtc not ebable\n");
		return 0;
	}

	if (!comp || !comp->funcs || !comp->funcs->io_cmd) {
		pr_err("[lh]cannot find output component\n");
		return 0;
	}

	if (sscanf(bufp, "%c%n", &read, &step) && read == 'r') {
		bufp += step;
		sscanf(bufp, "%x %d", &value, &len);

		if (len > PANEL_TX_MAX_BUF || value > PANEL_TX_MAX_BUF) {
			pr_err("reg or len than the max,stop\n");
			return 0;
		}

		oplus_mtk_read_ddic_v3(value, len, para);
		for(i = 0; i < len; i++) {
			printk("para[%d] = %d\n", i, para[i]);
		}
	}

	return count;
}

static ssize_t oplus_get_panel_reg(struct kobject *obj,
                struct kobj_attribute *attr, char *buf)
{
        printk("Lcm read serial_number = %llx\n", serial_number);
        return scnprintf(buf, PAGE_SIZE, "Get panel serial number: %llx\n", serial_number);
}

static ssize_t oplus_display_get_pq_trigger(struct kobject *obj,
                struct kobj_attribute *attr, char *buf)
{
        return sprintf(buf, "%d\n", pq_trigger);
}

static ssize_t oplus_display_set_pq_trigger(struct kobject *obj,
                struct kobj_attribute *attr, const char *buf, size_t count)
{
        int tmp = 0;
        sscanf(buf, "%d", &tmp);
        pq_trigger = !!tmp;
        pr_err("debug for %s, pq_trigger = %d\n", __func__, pq_trigger);

        return count;
}
int oplus_dc_threshold = 260;
int oplus_panel_alpha = 0;
int oplus_underbrightness_alpha = 0;
int alpha_save = 0;

EXPORT_SYMBOL(oplus_underbrightness_alpha);
EXPORT_SYMBOL(oplus_panel_alpha);


struct ba {
	u32 brightness;
	u32 alpha;
};
struct ba brightness_seed_alpha_lut_dc[] = {
	{0, 0xff},
	{1, 0xfc},
	{2, 0xfb},
	{3, 0xfa},
	{4, 0xf9},
	{5, 0xf8},
	{6, 0xf7},
	{8, 0xf6},
	{10, 0xf4},
	{15, 0xf0},
	{20, 0xea},
	{30, 0xe0},
	{45, 0xd0},
	{70, 0xbc},
	{100, 0x98},
	{120, 0x80},
	{140, 0x70},
	{160, 0x58},
	{180, 0x48},
	{200, 0x30},
	{220, 0x20},
	{240, 0x10},
	{260, 0x00},
};
struct ba brightness_alpha_lut[] = {
	{0, 0xFF},
	{5, 0xEE},
	{7, 0xED},
	{10, 0xE7},
	{20, 0xE3},
	{35, 0xDC},
	{60, 0xD1},
	{90, 0xCE},
	{150, 0xC1},
	{280, 0xAA},
	{460, 0x95},
	{650, 0x7F},
	{850, 0x79},
	{1000, 0x6E},
	{1150, 0x62},
	{1300, 0x52},
	{1500, 0x4C},
	{1700, 0x42},
	{1900, 0x35},
	{2047, 0x24},
};

struct ba brightness_alpha_lut_BOE[] = {
	{0, 0xFF},
	{12, 0xEE},
	{20, 0xE8},
	{35, 0xE5},
	{65, 0xDA},
	{100, 0xD8},
	{150, 0xCD},
	{210, 0xC5},
	{320, 0xB9},
	{450, 0xA9},
	{630, 0xA0},
	{870, 0x94},
	{1150, 0x86},
	{1500, 0x7B},
	{1850, 0x6B},
	{2250, 0x66},
	{2650, 0x55},
	{3050, 0x47},
	{3400, 0x39},
	{3515, 0x24},
};

struct ba brightness_alpha_lut_index[] = {
	{0, 0xFF},
	{150, 0xEE},
	{190, 0xEB},
	{230, 0xE6},
	{270, 0xE1},
	{310, 0xDA},
	{350, 0xD7},
	{400, 0xD3},
	{450, 0xD0},
	{500, 0xCD},
	{600, 0xC3},
	{700, 0xB8},
	{900, 0xA3},
	{1100, 0x90},
	{1300, 0x7B},
	{1400, 0x70},
	{1500, 0x65},
	{1700, 0x4E},
	{1900, 0x38},
	{2047, 0x23},
};

static int interpolate(int x, int xa, int xb, int ya, int yb)
{
	int bf, factor, plus;
	int sub = 0;

	bf = 2 * (yb - ya) * (x - xa) / (xb - xa);
	factor = bf / 2;
	plus = bf % 2;
	if ((xa - xb) && (yb - ya))
		sub = 2 * (x - xa) * (x - xb) / (yb - ya) / (xa - xb);

	return ya + factor + plus + sub;
}

int bl_to_alpha(int brightness)
{
	int alpha;
	struct drm_crtc *crtc;
	struct mtk_drm_crtc *mtk_crtc;

	int i = 0;
	int level = 0;
	struct drm_device *ddev = get_drm_device();

	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
				typeof(*crtc), head);
	if (!crtc) {
		printk(KERN_ERR "find crtc fail\n");
		return 0;
	}

	mtk_crtc = to_mtk_crtc(crtc);
	if (!mtk_crtc || !mtk_crtc->panel_ext || !mtk_crtc->panel_ext->params) {
		pr_err("falied to get lcd proc info\n");
		return 0;
	}

/*	if(!strcmp(mtk_crtc->panel_ext->params->manufacture, "boe_nt37800_2048")) {
		level = ARRAY_SIZE(brightness_alpha_lut_BOE);
		for (i = 0; i < ARRAY_SIZE(brightness_alpha_lut_BOE); i++) {
			if (brightness_alpha_lut_BOE[i].brightness >= brightness)
				break;
		}

		if (i == 0)
			alpha = brightness_alpha_lut_BOE[0].alpha;
		else if (i == level)
			alpha = brightness_alpha_lut_BOE[level - 1].alpha;
		else
			alpha = interpolate(brightness,
				brightness_alpha_lut_BOE[i-1].brightness,
				brightness_alpha_lut_BOE[i].brightness,
				brightness_alpha_lut_BOE[i-1].alpha,
				brightness_alpha_lut_BOE[i].alpha);
	}
	else {*/
		level = ARRAY_SIZE(brightness_alpha_lut);
		for (i = 0; i < ARRAY_SIZE(brightness_alpha_lut); i++) {
			if (brightness_alpha_lut[i].brightness >= brightness)
				break;
		}

		if (i == 0)
			alpha = brightness_alpha_lut[0].alpha;
		else if (i == level)
			alpha = brightness_alpha_lut[level - 1].alpha;
		else
			alpha = interpolate(brightness,
				brightness_alpha_lut[i-1].brightness,
				brightness_alpha_lut[i].brightness,
				brightness_alpha_lut[i-1].alpha,
				brightness_alpha_lut[i].alpha);
/*	} */

	return alpha;
}

int brightness_to_alpha(int brightness)
{
	int alpha;

	if (brightness <= 3)
		return alpha_save;

	alpha = bl_to_alpha(brightness);

	alpha_save = alpha;

	return alpha;
}

int oplus_seed_bright_to_alpha(int brightness)
{
	int level = ARRAY_SIZE(brightness_seed_alpha_lut_dc);
	int i = 0;
	int alpha;

	for (i = 0; i < ARRAY_SIZE(brightness_seed_alpha_lut_dc); i++) {
		if (brightness_seed_alpha_lut_dc[i].brightness >= brightness)
			break;
	}

	if (i == 0)
		alpha = brightness_seed_alpha_lut_dc[0].alpha;
	else if (i == level)
		alpha = brightness_seed_alpha_lut_dc[level - 1].alpha;
	else
		alpha = interpolate(brightness,
			brightness_seed_alpha_lut_dc[i-1].brightness,
			brightness_seed_alpha_lut_dc[i].brightness,
			brightness_seed_alpha_lut_dc[i-1].alpha,
			brightness_seed_alpha_lut_dc[i].alpha);

	return alpha;
}

int oplus_get_panel_brightness_to_alpha(void)
{
	if (oplus_panel_alpha)
		return oplus_panel_alpha;

	return brightness_to_alpha(oplus_display_brightness);
}
EXPORT_SYMBOL(oplus_get_panel_brightness_to_alpha);

static ssize_t oplus_display_get_dim_alpha(struct kobject *obj,
                                struct kobj_attribute *attr, char *buf)
{
#ifdef OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT
	if (!oplus_ofp_get_hbm_state()) {
		return sprintf(buf, "%d\n", 0);
	}
#endif /* OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT */

	oplus_underbrightness_alpha = oplus_get_panel_brightness_to_alpha();

	return sprintf(buf, "%d\n", oplus_underbrightness_alpha);
}


static ssize_t oplus_display_set_dim_alpha(struct kobject *obj,
                               struct kobj_attribute *attr,
                               const char *buf, size_t count)
{
	sscanf(buf, "%x", &oplus_panel_alpha);
	return count;
}

unsigned int silence_mode = 0;
EXPORT_SYMBOL(silence_mode);

static ssize_t silence_show(struct kobject *obj,
			struct kobj_attribute *attr, char *buf)
{
	printk("%s silence_mode=%d\n", __func__, silence_mode);
	return sprintf(buf, "%d\n", silence_mode);
}

static ssize_t silence_store(struct kobject *obj,
		struct kobj_attribute *attr, const char *buf, size_t num)
{
	int ret;
	msleep(1000);
	ret = kstrtouint(buf, 10, &silence_mode);
	printk("%s silence_mode=%d\n", __func__, silence_mode);
	return num;
}

static ssize_t oplus_display_get_ESD(struct kobject *obj,
                struct kobj_attribute *attr, char *buf)
{
	printk("%s esd=%d\n", __func__, esd_mode);
	return sprintf(buf, "%d\n", esd_mode);
}

static ssize_t oplus_display_set_ESD(struct kobject *obj,
        struct kobj_attribute *attr, const char *buf, size_t num)
{
	int ret = 0;

	ret = kstrtouint(buf, 10, &esd_mode);
	printk("%s,esd mode is %d\n", __func__, esd_mode);
	return num;
}

unsigned int cabc_mode = 1;
unsigned int cabc_true_mode = 1;
unsigned int cabc_sun_flag = 0;
unsigned int cabc_back_flag = 1;
extern void disp_aal_set_dre_en(struct mtk_ddp_comp *comp, int enable);

enum{
	CABC_LEVEL_0,
	CABC_LEVEL_1,
	CABC_LEVEL_2 = 3,
	CABC_EXIT_SPECIAL = 8,
	CABC_ENTER_SPECIAL = 9,
};

static ssize_t oplus_display_get_CABC(struct kobject *obj,
                struct kobj_attribute *attr, char *buf)
{
	printk("%s CABC_mode=%d\n", __func__, cabc_true_mode);
	return sprintf(buf, "%d\n", cabc_true_mode);
}

static ssize_t oplus_display_set_CABC(struct kobject *obj,
        struct kobj_attribute *attr, const char *buf, size_t num)
{
	int ret = 0;
	struct drm_crtc *crtc;
	struct mtk_drm_crtc *mtk_crtc;
	struct mtk_ddp_comp *comp;
	struct drm_device *ddev = get_drm_device();
	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
				typeof(*crtc), head);
	if (!crtc) {
		printk(KERN_ERR "find crtc fail\n");
		return 0;
	}

	mtk_crtc = to_mtk_crtc(crtc);

	comp = mtk_ddp_comp_sel_in_cur_crtc_path(mtk_crtc, MTK_DISP_AAL, 0);
	if (!comp) {
		DDPPR_ERR("%s, comp is null!\n", __func__);
		return 0;
	}

	ret = kstrtouint(buf, 10, &cabc_mode);
	cabc_true_mode = cabc_mode;
	printk("%s,cabc mode is %d, cabc_back_flag is %d\n", __func__, cabc_mode, cabc_back_flag);
	if(cabc_mode < 4)
		cabc_back_flag = cabc_mode;

	if (cabc_mode == CABC_ENTER_SPECIAL) {
		cabc_sun_flag = 1;
		cabc_true_mode = 0;
	} else if (cabc_mode == CABC_EXIT_SPECIAL) {
		cabc_sun_flag = 0;
		cabc_true_mode = cabc_back_flag;
	} else if (cabc_sun_flag == 1) {
		if (cabc_back_flag == CABC_LEVEL_0) {
			disp_aal_set_dre_en(comp, 1);
			printk("%s sun enable dre\n", __func__);
		} else {
			disp_aal_set_dre_en(comp, 0);
			printk("%s sun disable dre\n", __func__);
		}
		return num;
	}

	printk("%s,cabc mode is %d\n", __func__, cabc_true_mode);

	if (cabc_true_mode == CABC_LEVEL_0 && cabc_back_flag == CABC_LEVEL_0) {
		disp_aal_set_dre_en(comp, 1);
		printk("%s enable dre\n", __func__);
	} else {
		disp_aal_set_dre_en(comp, 0);
		printk("%s disable dre\n", __func__);
	}
	//oplus_mtk_drm_setcabc(crtc, cabc_true_mode);
	if (cabc_true_mode != cabc_back_flag) cabc_true_mode = cabc_back_flag;

	return num;
}

static ssize_t oplus_display_get_seed(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "seed_mode = %d\n", seed_mode);

        return sprintf(buf, "%d\n", seed_mode);
}

static ssize_t oplus_display_set_seed(struct kobject *obj,
                struct kobj_attribute *attr,
                const char *buf, size_t count) {
	struct drm_crtc *crtc;
	unsigned int temp_save = 0;
	int ret = 0;
	struct drm_device *ddev = get_drm_device();

	ret = kstrtouint(buf, 10, &temp_save);
	printk(KERN_INFO "seed = %d\n", temp_save);
	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
				typeof(*crtc), head);
	if (!crtc) {
		printk(KERN_ERR "find crtc fail\n");
		return 0;
	}

	oplus_mtk_drm_setseed(crtc, temp_save);
	seed_mode = temp_save;

	return count;
}

static ssize_t oplus_display_get_osc(struct kobject *kobj,
		struct kobj_attribute *attr, char *buf)
{
        printk(KERN_INFO "osc_mode = %lu\n", osc_mode);

        return sprintf(buf, "%lu\n", osc_mode);
}

static ssize_t oplus_display_set_osc(struct kobject *kobj,
                struct kobj_attribute *attr,
                const char *buf, size_t count) {
	struct drm_crtc *crtc;
	unsigned int temp_save = 0;
	int ret = 0;
	struct drm_device *ddev = get_drm_device();

	ret = kstrtouint(buf, 10, &temp_save);
	printk(KERN_INFO "osc mode = %d\n", temp_save);

	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
				typeof(*crtc), head);
	if (!crtc) {
		printk(KERN_ERR "find crtc fail\n");
		return 0;
	}

	mtk_crtc_osc_freq_switch(crtc, temp_save, 0);
	osc_mode = temp_save;

	return count;
}

struct softiris_color
{
	uint32_t color_vivid_status;
	uint32_t color_srgb_status;
	uint32_t color_softiris_status;
	uint32_t color_dual_panel_status;
	uint32_t color_dual_brightness_status;
	uint32_t color_oplus_calibrate_status;
};

static struct softiris_color *data;
static ssize_t oplus_display_get_color_status(struct kobject *obj,
				struct kobj_attribute *attr, char *buf)
{
	int ret = 0;

	data = kzalloc(sizeof(struct softiris_color), GFP_KERNEL);

	ret = oplus_display_get_softiris_color_status((void*)data);
	printk(KERN_INFO "ret = %d\n", ret);

	return sprintf(buf, "vivid:%s, srgb:%s,softiris:%s, dual_panel:%s, dual_brightness:%s, color_oplus_calibrate_status:%s\n", data->color_vivid_status ? "true" : "false", \
	 data->color_srgb_status ? "true" : "false", data->color_softiris_status ? "true" : "false", data->color_dual_panel_status ? "true" : "false", \
	 data->color_dual_brightness_status ? "true" : "false", data->color_oplus_calibrate_status ? "true" : "false");
}
int oplus_dc_alpha = 0;
EXPORT_SYMBOL(oplus_dc_alpha);
extern int oplus_dc_enable;
static ssize_t oplus_display_get_dc_enable(struct kobject *obj,
				struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", oplus_dc_enable);
}

static ssize_t oplus_display_set_dc_enable(struct kobject *obj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	sscanf(buf, "%x", &oplus_dc_enable);
	return count;
}

static ssize_t oplus_display_get_dim_dc_alpha(struct kobject *obj,
				struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", oplus_dc_alpha);
}

static ssize_t oplus_display_set_dim_dc_alpha(struct kobject *dev,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	sscanf(buf, "%x", &oplus_dc_alpha);
	return count;
}

#define PANEL_TX_MAX_BUF1 50
static char oplus_rx_reg[PANEL_TX_MAX_BUF1] = {0x0};
static char oplus_rx_len = 0;
static ssize_t oplus_display_get_panel_reg(struct kobject *obj,
		struct kobj_attribute *attr, char *buf)
{
	int i, cnt = 0;

	for (i = 0; i < oplus_rx_len; i++)
		cnt += snprintf(buf + cnt, PANEL_TX_MAX_BUF1 - cnt,
				"0x%02x ", oplus_rx_reg[i]);

	cnt += snprintf(buf + cnt, PANEL_TX_MAX_BUF1 - cnt, "\n");

	return cnt;
}

int dsi_display_read_panel_reg(char cmd, char *data, size_t len) {
	struct drm_crtc *crtc;
	struct mtk_drm_crtc *mtk_crtc;
	struct drm_device *ddev = get_drm_device();
	struct mtk_ddp_comp *comp;

	data[0] = cmd;
	data[1] = len;

	/* this debug cmd only for crtc0 */
	crtc = list_first_entry(&(ddev)->mode_config.crtc_list,
		typeof(*crtc), head);
	if (!crtc) {
		DDPPR_ERR("get_panel_reg - find crtc fail\n");
		return 0;
	}

	mtk_crtc = to_mtk_crtc(crtc);
	comp = mtk_ddp_comp_request_output(mtk_crtc);

	if (!comp || !comp->funcs || !comp->funcs->io_cmd) {
		DDPINFO("cannot find output component\n");
		return 0;
	}

	if (!(mtk_crtc->enabled)) {
		DDPINFO("get_panel_reg - crtc not ebable\n");
		return 0;
	}

	printk("%s - will read reg 0x%02x\n", __func__,  cmd);
	comp->funcs->io_cmd(comp, NULL, DSI_READ, data);
	return 0;
}

static ssize_t oplus_display_set_panel_reg(struct kobject *obj,
		struct kobj_attribute *attr,
		const char *buf, size_t count)
{
	char reg[PANEL_TX_MAX_BUF1] = {0x0};
	u32 value = 0, step = 0;
	int len = 0, index = 0;
	char *bufp = (char *)buf;
	char read;

	/*begin read*/
	if (sscanf(bufp, "%c%n", &read, &step) && read == 'r') {
		bufp += step;
		sscanf(bufp, "%x %d", &value, &len);

		if (len > PANEL_TX_MAX_BUF1) {
			pr_err("failed\n");
			return -EINVAL;
		}

		oplus_mtk_read_ddic_v2(value, len, reg);

		for (index = 0; index < len; index++) {
			pr_info("read reg[%d]:0x%x ", index, reg[index]);
		}

		memcpy(oplus_rx_reg, reg, PANEL_TX_MAX_BUF1);
		oplus_rx_len = len;

		return count;
	}
	/*end read*/

	while (sscanf(bufp, "%x%n", &value, &step) > 0) {
		reg[len++] = value;
		pr_err("value=%x\n", value);
		if (len >= PANEL_TX_MAX_BUF1) {
			pr_err("wrong input reg len\n");
			return -EFAULT;
		}

		bufp += step;
	}
	oplus_ddic_dsi_send_cmd(len, reg);

	return count;
}

#ifdef OPLUS_TRACKPOINT_REPORT
static ssize_t oplus_set_trackpoint_test_attr(struct kobject *obj,
	struct kobj_attribute *attr, const char *buf, size_t count)
{
	char data[8];

	if (strlen(buf) >= 8) {
		pr_err("[LCM_DEBUG] input buffer size[%lu] is out of range[%d]\n",
				strlen(buf), 8);
		return -1;
	}

	strlcpy(data, buf, 8);
	data[strlen(data)-1]='\0';

	if (!strcmp("501", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@501$$trackpoint_test: ovl_probe error");
	else if (!strcmp("502", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@502$$trackpoint_test: underflow cnt");
	else if (!strcmp("503", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@503$$trackpoint_test: invalid");
	else if (!strcmp("504", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@504$$trackpoint_test: mtk_ddp_probe failed to request irq");
	else if (!strcmp("505", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@505$$trackpoint_test: mtk_fb_wait error");
	else if (!strcmp("506", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@506$$trackpoint_test: underrun");
	else if (!strcmp("507", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@507$$trackpoint_test: ESD check failed");
	else if (!strcmp("508", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@508$$trackpoint_test: cmdq timeout Begin of Error");
	else if (!strcmp("509", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@509$$trackpoint_test: cmdq sec session init failed");
	else if (!strcmp("510", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@510$$trackpoint_test: disp_aal_config_mdp_aal");
	else if (!strcmp("511", data))
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@511$$trackpoint_test: TE timeout");
	else
		EXCEPTION_TRACKPOINT_REPORT("DisplayDriverID@@599$$trackpoint_test:%s", buf);
	return count;
}
#endif /* OPLUS_TRACKPOINT_REPORT */

static struct kobject *oplus_display_kobj;
static OPLUS_ATTR(oplus_brightness, S_IRUGO|S_IWUSR, oplus_display_get_brightness, oplus_display_set_brightness);
static OPLUS_ATTR(oplus_max_brightness, S_IRUGO|S_IWUSR, oplus_display_get_max_brightness, NULL);
static OPLUS_ATTR(oplus_get_color_status, S_IRUGO|S_IWUSR, oplus_display_get_color_status, NULL);
static OPLUS_ATTR(max_brightness, S_IRUGO|S_IWUSR, oplus_display_get_maxbrightness, NULL);
static OPLUS_ATTR(seed, S_IRUGO|S_IWUSR, oplus_display_get_seed, oplus_display_set_seed);
static OPLUS_ATTR(panel_pwr, S_IRUGO|S_IWUSR, oplus_display_get_panel_pwr, oplus_display_set_panel_pwr);
static OPLUS_ATTR(dim_alpha, S_IRUGO|S_IWUSR, oplus_display_get_dim_alpha, oplus_display_set_dim_alpha);
/* static OPLUS_ATTR(dimlayer_bl_en, S_IRUGO|S_IWUSR, oplus_display_get_dc_enable, oplus_display_set_dc_enable); */
/* static OPLUS_ATTR(dim_dc_alpha, S_IRUGO|S_IWUSR, oplus_display_get_dim_dc_alpha, oplus_display_set_dim_dc_alpha); */
static OPLUS_ATTR(panel_serial_number, S_IRUGO|S_IWUSR, oplus_get_panel_serial_number, panel_serial_store);
static OPLUS_ATTR(LCM_CABC, S_IRUGO|S_IWUSR, oplus_display_get_CABC, oplus_display_set_CABC);
static OPLUS_ATTR(esd, S_IRUGO|S_IWUSR, oplus_display_get_ESD, oplus_display_set_ESD);
static OPLUS_ATTR(sau_closebl_node, S_IRUGO|S_IWUSR, silence_show, silence_store);
static OPLUS_ATTR(dsi_log_switch, S_IRUGO | S_IWUSR, oplus_display_get_dsi_log_switch, oplus_display_set_dsi_log_switch);
static OPLUS_ATTR(trace_enable, S_IRUGO | S_IWUSR, oplus_display_get_trace_enable_attr, oplus_display_set_trace_enable_attr);
/* #ifdef OPLUS_ADFR */
static OPLUS_ATTR(adfr_debug, S_IRUGO|S_IWUSR, oplus_adfr_get_debug, oplus_adfr_set_debug);
static OPLUS_ATTR(adfr_params, S_IRUGO|S_IWUSR, oplus_adfr_get_params, oplus_adfr_set_params);
/* add for mux switch control */
static OPLUS_ATTR(vsync_switch, S_IRUGO|S_IWUSR, oplus_get_vsync_switch, oplus_set_vsync_switch);
/* #endif */
#ifdef OPLUS_FEATURE_DISPLAY_TEMP_COMPENSATION
static OPLUS_ATTR(temp_compensation_config, S_IRUGO | S_IWUSR, oplus_temp_compensation_get_config_attr, oplus_temp_compensation_set_config_attr);
static OPLUS_ATTR(ntc_temp, S_IRUGO | S_IWUSR, oplus_temp_compensation_get_ntc_temp_attr, oplus_temp_compensation_set_ntc_temp_attr);
static OPLUS_ATTR(shell_temp, S_IRUGO | S_IWUSR, oplus_temp_compensation_get_shell_temp_attr, oplus_temp_compensation_set_shell_temp_attr);
#endif /* OPLUS_FEATURE_DISPLAY_TEMP_COMPENSATION */
static OPLUS_ATTR(disp_trig_db, S_IRUGO|S_IWUSR, oplus_display_get_disp_trig_db,
                        oplus_display_set_disp_trig_db);
static OPLUS_ATTR(write_panel_reg, S_IRUGO|S_IWUSR, oplus_display_get_panel_reg, oplus_display_set_panel_reg);
static OPLUS_ATTR(panel_reg_cmd, S_IRUGO|S_IWUSR, oplus_get_panel_reg, oplus_set_panel_reg);
static OPLUS_ATTR(dimlayer_bl_en, S_IRUGO|S_IWUSR, oplus_display_get_dc_enable, oplus_display_set_dc_enable);
static OPLUS_ATTR(dim_dc_alpha, S_IRUGO|S_IWUSR, oplus_display_get_dim_dc_alpha, oplus_display_set_dim_dc_alpha);
static OPLUS_ATTR(osc, S_IRUGO|S_IWUSR, oplus_display_get_osc, oplus_display_set_osc);
static OPLUS_ATTR(pq_trigger, S_IRUGO|S_IWUSR, oplus_display_get_pq_trigger, oplus_display_set_pq_trigger);
static OPLUS_ATTR(pwm_turbo, S_IRUGO|S_IWUSR, oplus_display_get_high_pwm, oplus_display_set_high_pwm);
static OPLUS_ATTR(hbm_max, S_IRUGO|S_IWUSR, oplus_get_hbm_max_debug, oplus_set_hbm_max_debug);
static OPLUS_ATTR(pwm_onepulse, S_IRUGO|S_IWUSR, oplus_get_pwm_pulse_debug, oplus_set_pwm_pulse_debug);
#ifdef OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT
static OPLUS_ATTR(fp_type, S_IRUGO | S_IWUSR, oplus_ofp_get_fp_type_attr, oplus_ofp_set_fp_type_attr);
static OPLUS_ATTR(dimlayer_hbm, S_IRUGO | S_IWUSR, oplus_ofp_get_dimlayer_hbm_attr, oplus_ofp_set_dimlayer_hbm_attr);
static OPLUS_ATTR(oplus_notify_fppress, S_IRUGO|S_IWUSR, NULL, oplus_ofp_notify_fp_press_attr);
static OPLUS_ATTR(hbm, S_IRUGO|S_IWUSR, oplus_ofp_get_hbm_attr, oplus_ofp_set_hbm_attr);
static OPLUS_ATTR(aod_light_mode_set, S_IRUGO|S_IWUSR, oplus_ofp_get_aod_light_mode_attr, oplus_ofp_set_aod_light_mode_attr);
static OPLUS_ATTR(fake_aod, S_IRUGO | S_IWUSR, oplus_ofp_get_fake_aod_attr, oplus_ofp_set_fake_aod_attr);
static OPLUS_ATTR(ultra_low_power_aod_mode, S_IRUGO | S_IWUSR, oplus_ofp_get_ultra_low_power_aod_mode_attr, oplus_ofp_set_ultra_low_power_aod_mode_attr);
#endif /* OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT */
#ifdef OPLUS_FEATURE_DISPLAY_ADFR
static OPLUS_ATTR(test_te, S_IRUGO|S_IWUSR, oplus_adfr_get_test_te_attr, oplus_adfr_set_test_te_attr);
#endif
#ifdef OPLUS_TRACKPOINT_REPORT
static OPLUS_ATTR(trackpoint_test, S_IWUSR, NULL, oplus_set_trackpoint_test_attr);
#endif /* OPLUS_TRACKPOINT_REPORT */

EXPORT_SYMBOL(seed_mode);
EXPORT_SYMBOL(oplus_max_normal_brightness);
EXPORT_SYMBOL(oplus_display_brightness);
EXPORT_SYMBOL(cabc_mode);
EXPORT_SYMBOL(cabc_true_mode);
EXPORT_SYMBOL(cabc_sun_flag);
EXPORT_SYMBOL(cabc_back_flag);
EXPORT_SYMBOL(oplus_enhance_mipi_strength);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *oplus_display_attrs[] = {
	&oplus_attr_oplus_brightness.attr,
	&oplus_attr_oplus_max_brightness.attr,
	&oplus_attr_oplus_get_color_status.attr,
	&oplus_attr_max_brightness.attr,
	&oplus_attr_seed.attr,
	&oplus_attr_panel_pwr.attr,
	&oplus_attr_dim_alpha.attr,
	/* &oplus_attr_dimlayer_bl_en.attr, */
	/* &oplus_attr_dim_dc_alpha.attr, */
	&oplus_attr_panel_serial_number.attr,
	&oplus_attr_LCM_CABC.attr,
	&oplus_attr_esd.attr,
	&oplus_attr_sau_closebl_node.attr,
	&oplus_attr_dsi_log_switch.attr,
	&oplus_attr_trace_enable.attr,
/* #ifdef OPLUS_ADFR */
	&oplus_attr_adfr_debug.attr,
	&oplus_attr_adfr_params.attr,
	/* add for mux switch control */
	&oplus_attr_vsync_switch.attr,
/* #endif */
#ifdef OPLUS_FEATURE_DISPLAY_TEMP_COMPENSATION
	&oplus_attr_temp_compensation_config.attr,
	&oplus_attr_ntc_temp.attr,
	&oplus_attr_shell_temp.attr,
#endif /* OPLUS_FEATURE_DISPLAY_TEMP_COMPENSATION */
	&oplus_attr_disp_trig_db.attr,
	&oplus_attr_write_panel_reg.attr,
	&oplus_attr_panel_reg_cmd.attr,
	&oplus_attr_dimlayer_bl_en.attr,
	&oplus_attr_dim_dc_alpha.attr,
	&oplus_attr_osc.attr,
	&oplus_attr_pq_trigger.attr,
	&oplus_attr_pwm_turbo.attr,
	&oplus_attr_hbm_max.attr,
	&oplus_attr_pwm_onepulse.attr,
#ifdef OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT
	&oplus_attr_fp_type.attr,
	&oplus_attr_dimlayer_hbm.attr,
	&oplus_attr_oplus_notify_fppress.attr,
	&oplus_attr_hbm.attr,
	&oplus_attr_aod_light_mode_set.attr,
	&oplus_attr_fake_aod.attr,
	&oplus_attr_ultra_low_power_aod_mode.attr,
#endif /* OPLUS_FEATURE_DISPLAY_ONSCREENFINGERPRINT */
#ifdef OPLUS_FEATURE_DISPLAY_ADFR
	&oplus_attr_test_te.attr,
#endif
#ifdef OPLUS_TRACKPOINT_REPORT
	&oplus_attr_trackpoint_test.attr,
#endif /* OPLUS_TRACKPOINT_REPORT */
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group oplus_display_attr_group = {
	.attrs = oplus_display_attrs,
};

int oplus_display_private_api_init(void)
{
	int retval;

	oplus_display_kobj = kobject_create_and_add("oplus_display", kernel_kobj);
	if (!oplus_display_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(oplus_display_kobj, &oplus_display_attr_group);
	if (retval)
		kobject_put(oplus_display_kobj);
	printk("%s end\n", __func__);

	return retval;
}

void oplus_display_private_api_exit(void)
{
	kobject_put(oplus_display_kobj);
}
