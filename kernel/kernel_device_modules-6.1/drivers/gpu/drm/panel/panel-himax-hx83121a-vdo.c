// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2015 MediaTek Inc.
 *
 */
#include <linux/backlight.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>
#include <drm/drm_modes.h>
#include <linux/delay.h>
#include <drm/drm_connector.h>
#include <drm/drm_device.h>
#include <linux/gpio/consumer.h>
#include <linux/regulator/consumer.h>
#include <video/mipi_display.h>
#include <video/of_videomode.h>
#include <video/videomode.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>
#define CONFIG_MTK_PANEL_EXT
#if defined(CONFIG_MTK_PANEL_EXT)
#include "../mediatek/mediatek_v2/mtk_panel_ext.h"
#include "../mediatek/mediatek_v2/mtk_log.h"
#include "../mediatek/mediatek_v2/mtk_drm_graphics_base.h"
#endif

#define HX83121A_120HZ_WITH_DSC 1
/* enable this to check panel self -bist pattern */
/* #define PANEL_BIST_PATTERN */
/* option function to read data from some panel address */
/* #define PANEL_SUPPORT_READBACK */
struct himax {
	struct device *dev;
	struct drm_panel panel;
	struct backlight_device *backlight;
	struct gpio_desc *reset_gpio;
	struct regulator *reg;
	bool prepared;
	bool enabled;
	int error;
};
#define himax_dcs_write_seq(ctx, seq...)                                     \
	({                                                                     \
		const u8 d[] = {seq};                                          \
		BUILD_BUG_ON_MSG(ARRAY_SIZE(d) > 64,                           \
				 "DCS sequence too big for stack");            \
		himax_dcs_write(ctx, d, ARRAY_SIZE(d));                      \
	})
#define himax_dcs_write_seq_static(ctx, seq...)                              \
	({                                                                     \
		static const u8 d[] = {seq};                                   \
		himax_dcs_write(ctx, d, ARRAY_SIZE(d));                      \
	})
static inline struct himax *panel_to_himax(struct drm_panel *panel)
{
	return container_of(panel, struct himax, panel);
}

#ifdef PANEL_SUPPORT_READBACK
static int himax_dcs_read(struct himax *ctx, u8 cmd, void *data, size_t len)
{
	struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
	ssize_t ret;

	if (ctx->error < 0)
		return 0;

	ret = mipi_dsi_dcs_read(dsi, cmd, data, len);
	if (ret < 0) {
		dev_err(ctx->dev, "error %d reading dcs seq:(%#x)\n", ret, cmd);
		ctx->error = ret;
	}

	return ret;
}

static void himax_panel_get_data(struct himax *ctx)
{
	u8 buffer[3] = {0};
	static int ret;

	if (ret == 0) {
		ret = himax_dcs_read(ctx, 0x0A, buffer, 1);
		dev_info(ctx->dev, "return %d data(0x%08x) to dsi engine\n",
			 ret, buffer[0] | (buffer[1] << 8));
	}
}
#endif

static void himax_dcs_write(struct himax *ctx, const void *data, size_t len)
{
	struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
	ssize_t ret;
	char *addr;

	if (ctx->error < 0)
		return;

	addr = (char *)data;
	if ((int)*addr < 0xB0)
		ret = mipi_dsi_dcs_write_buffer(dsi, data, len);
	else
		ret = mipi_dsi_generic_write(dsi, data, len);

	if (ret < 0) {
		dev_err(ctx->dev, "error %zd writing seq: %ph\n", ret, data);
		ctx->error = ret;
	}
}

static int himax_panel_power_enable(struct himax *ctx)
{
	int ret = 0;

	return ret;
}

static int himax_panel_power_disable(struct himax *ctx)
{
	int ret = 0;

	return ret;
}

/* rc_buf_thresh will right shift 6bits (which means the values here will be divided by 64)
 * when setting to PPS8~PPS11 registers in mtk_dsc_config() function, so the original values
 * need left sihft 6bit (which means the original values are multiplied by 64), so that
 * PPS8~PPS11 registers can get right setting
 */
unsigned int hx8312a_wqxga_dsi_vdo_120hz_dphy_buf_thresh[14] = {
	896, 1792, 2688, 3584, 4480, 5376, 6272, 6720, 7168, 7616, 7744, 7872, 8000, 8064};
unsigned int hx8312a_wqxga_dsi_vdo_120hz_dphy_range_min_qp[15] = {
	0, 0, 1, 1, 3, 3, 3, 3, 3, 3, 5, 5, 5, 7, 13};
unsigned int hx8312a_wqxga_dsi_vdo_120hz_dphy_range_max_qp[15] = {
	4, 4, 5, 6, 7, 7, 7, 8, 9, 10, 11, 12, 13, 13, 13};
int hx8312a_wqxga_dsi_vdo_120hz_dphy_range_bpg_ofs[15] = {
	2, 0, 0, -2, -4, -6, -8, -8, -8, -10, -10, -12, -12, -12, -12};

static void himax_panel_init(struct himax *ctx)
{
	pr_info("%s +\n", __func__);
	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	usleep_range(10 * 1000, 15 * 1000);
	gpiod_set_value(ctx->reset_gpio, 0);
	usleep_range(10 * 1000, 20 * 1000);
	gpiod_set_value(ctx->reset_gpio, 1);
	usleep_range(10 * 1000, 10 * 1000);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);
#if HX83121A_120HZ_WITH_DSC
	himax_dcs_write_seq_static(ctx, 0xB9, 0x83, 0x12, 0x1A, 0x55, 0x00);
	himax_dcs_write_seq_static(ctx, 0xB1, 0x1C, 0x6B, 0x6B, 0x27, 0xE7,
			0x00, 0x1B, 0x11, 0x26, 0x26, 0x2D, 0x2D, 0x17, 0x33,
			0x31, 0x40, 0xCD, 0xFF, 0x1A, 0x05, 0x15, 0x98, 0x00,
			0x88, 0xFF, 0xFF, 0xFF, 0xCF, 0x1A, 0xCC, 0x02, 0x00);
	himax_dcs_write_seq_static(ctx, 0xB2, 0x00, 0x6A, 0x40, 0x00, 0x00,
			0x14, 0x56, 0x40, 0x25, 0x02, 0x80, 0x21, 0x21, 0x00,
			0x00, 0x10, 0x27);
	himax_dcs_write_seq_static(ctx, 0xB4, 0x3E, 0x04, 0x08, 0x7F, 0x08,
			0x7F, 0x0A, 0x3A, 0x0A, 0x3A, 0x0A, 0x3A, 0x0A, 0x3A,
			0x00, 0x00, 0x05, 0x07, 0x00, 0x14, 0x05, 0x05, 0x05,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0xFF, 0x00, 0xFF, 0x10, 0x00, 0x02, 0x14,
			0x14, 0x14, 0x14);
		/* Vcom */
	himax_dcs_write_seq_static(ctx, 0xB6, 0x8F, 0x8F, 0x03);
	himax_dcs_write_seq_static(ctx, 0xBC, 0x06, 0x02);
	himax_dcs_write_seq_static(ctx, 0xC0, 0x37, 0x37, 0xCC, 0x44, 0xAA,
			0xD8);
		/* DGC */
	himax_dcs_write_seq_static(ctx, 0xC1, 0x00);
	himax_dcs_write_seq_static(ctx, 0xC3, 0x87, 0xF2, 0x30);
	himax_dcs_write_seq_static(ctx, 0xCC, 0x02);
	himax_dcs_write_seq_static(ctx, 0xD1, 0x07);
	himax_dcs_write_seq_static(ctx, 0xD3, 0x00, 0xC0, 0x08, 0x08, 0x08,
			0x04, 0x04, 0x04, 0x14, 0x02, 0x07, 0x07, 0x07, 0x31,
			0x13, 0x12, 0x12, 0x12, 0x03, 0x03, 0x03, 0x32, 0x10,
			0x11, 0x00, 0x11, 0x32, 0x10, 0x03, 0x00, 0x03, 0x32,
			0x10, 0x03, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x00);
	himax_dcs_write_seq_static(ctx, 0x51, 0x0F, 0xFF);
	himax_dcs_write_seq_static(ctx, 0x53, 0x24);
	himax_dcs_write_seq_static(ctx, 0xC9, 0x00, 0x1E, 0x80, 0xA5, 0x01);
	himax_dcs_write_seq_static(ctx, 0xCB, 0x00, 0x13, 0x38, 0x00, 0x05,
		0x8F);
	himax_dcs_write_seq_static(ctx, 0xD9, 0x00, 0x04, 0x02);
	himax_dcs_write_seq_static(ctx, 0xE0, 0x00, 0x18, 0x34, 0x3D, 0x47,
			0x80, 0x94, 0x9D, 0x9D, 0x9F, 0x9D, 0x9A, 0x94, 0x91,
			0x8F, 0x8A, 0x89, 0x8A, 0x8A, 0xA1, 0xB3, 0x48, 0x7F,
			0x00, 0x18, 0x34, 0x3D, 0x47, 0x80, 0x94, 0x9D, 0x9D,
			0x9F, 0x9D, 0x9A, 0x94, 0x91, 0x8F, 0x8A, 0x89, 0x8A,
			0x8A, 0xA1, 0xB3, 0x48, 0x7F);
	himax_dcs_write_seq_static(ctx, 0xD5, 0x19, 0x19, 0x18, 0x18, 0x02,
			0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06,
			0x07, 0x07, 0x00, 0x00, 0x01, 0x01, 0x18, 0x18, 0x40,
			0x40, 0x20, 0x20, 0x18, 0x18, 0x18, 0x18, 0x40, 0x40,
			0x18, 0x18, 0x2F, 0x2F, 0x31, 0x31, 0x2F, 0x2F, 0x31,
			0x31, 0x18, 0x18, 0x41, 0x41, 0x41, 0x41);
	himax_dcs_write_seq_static(ctx, 0xD6, 0x40, 0x40, 0x18, 0x18, 0x05,
			0x05, 0x04, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01,
			0x00, 0x00, 0x07, 0x07, 0x06, 0x06, 0x18, 0x18, 0x19,
			0x19, 0x20, 0x20, 0x18, 0x18, 0x18, 0x18, 0x40, 0x40,
			0x18, 0x18, 0x2F, 0x2F, 0x31, 0x31, 0x2F, 0x2F, 0x31,
			0x31, 0x18, 0x18, 0x41, 0x41, 0x41, 0x41);
	himax_dcs_write_seq_static(ctx, 0xE1, 0x11, 0x00, 0x00, 0x89, 0x30,
			0x80, 0x0A, 0x00, 0x03, 0x20, 0x00, 0x14, 0x03, 0x20,
			0x03, 0x20, 0x02, 0x00, 0x02, 0x91, 0x00, 0x20, 0x02,
			0x47, 0x00, 0x0B, 0x00, 0x0C, 0x05, 0x0E, 0x03, 0x68,
			0x18, 0x00, 0x10, 0xE0, 0x03, 0x0C, 0x20, 0x00, 0x06,
			0x0B, 0x0B, 0x33, 0x0E, 0x1C, 0x2A, 0x38, 0x46, 0x54,
			0x62, 0x69, 0x70, 0x77, 0x79, 0x7B, 0x7D, 0x7E, 0x01,
			0x02, 0x01, 0x00, 0x09);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x01);
	himax_dcs_write_seq_static(ctx, 0xB1, 0x01, 0x23, 0x00);
	himax_dcs_write_seq_static(ctx, 0xCB, 0x13, 0x55, 0x03, 0x28,
			0x0D, 0x08, 0x0B);
	himax_dcs_write_seq_static(ctx, 0xD6, 0x02, 0x04, 0x21, 0x02,
			0x04, 0x21);
	himax_dcs_write_seq_static(ctx, 0xD8, 0x20, 0x00, 0x02, 0x22, 0x00,
			0x00, 0x20, 0x00, 0x02, 0x22, 0x00, 0x00, 0x20, 0x00,
			0x02, 0x22, 0x00, 0x00, 0x20, 0x00, 0x02, 0x22, 0x00,
			0x00, 0x20, 0x00, 0x02, 0x22, 0x00, 0x00, 0x20, 0x00,
			0x02, 0x22, 0x00, 0x00);
	himax_dcs_write_seq_static(ctx, 0xE1, 0x40, 0x09, 0xBE, 0x19, 0xFC,
			0x19, 0xFA, 0x19, 0xF8, 0x1A, 0x38, 0x1A, 0x78, 0x1A,
			0xB6, 0x2A, 0xF6, 0x2B, 0x34, 0x2B, 0x74, 0x3B, 0x74,
			0x6B, 0x74);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x02);
	himax_dcs_write_seq_static(ctx, 0xD8, 0xAF, 0xFF, 0xFA, 0xFA, 0xBF,
		0xEA, 0xAF, 0xFF, 0xFA, 0xFA, 0xBF, 0xEA);
	himax_dcs_write_seq_static(ctx, 0xBF, 0x72);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x03);
	himax_dcs_write_seq_static(ctx, 0xD8, 0xAA, 0xAA, 0xAA, 0xAB, 0xBF,
			0xEA, 0xAA, 0xAA, 0xAA, 0xAB, 0xBF, 0xEA, 0xAA, 0xAA,
			0xAA, 0xFA, 0xBF, 0xEA, 0xAA, 0xAA, 0xAA, 0xFA, 0xBF,
			0xEA);
	himax_dcs_write_seq_static(ctx, 0xE1, 0x01, 0x3F);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x00);
	himax_dcs_write_seq_static(ctx, 0xD8, 0xEA, 0xAA, 0xAA, 0xAE, 0xAA,
			0xAF, 0xEA, 0xAA, 0xAA, 0xAE, 0xAA, 0xAF, 0xE0, 0x00,
			0x0A, 0x2E, 0x80, 0x2F, 0xE0, 0x00, 0x0A, 0x2E, 0x80,
			0x2F, 0xE0, 0x00, 0x0A, 0x2E, 0x80, 0x2F, 0xE0, 0x00,
			0x0A, 0x2E, 0x80, 0x2F);
	/* 120Hz */
	himax_dcs_write_seq_static(ctx, 0xE7, 0x37, 0x06, 0x06, 0x4D, 0x41,
			0x21, 0x00, 0x23, 0x42, 0x02, 0x02);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x01);
	/* 120Hz */
	himax_dcs_write_seq_static(ctx, 0xE7, 0x02, 0x00, 0xA4, 0x01, 0xB0,
			0x05, 0xA6, 0x06, 0x48, 0x55, 0x04, 0x26);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x02);
	/* 120Hz */
	himax_dcs_write_seq_static(ctx, 0xE7, 0x01, 0x05, 0x01, 0x03, 0x01,
			0x03, 0x04, 0x02, 0x02, 0x14, 0x00, 0x14, 0x81, 0x02,
			0x40, 0x00, 0x14, 0x3A, 0x03, 0x02, 0x01, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x00);
		/* 00:CABC disable; 01:CABC enable */
	himax_dcs_write_seq_static(ctx, 0x55, 0x00);
	himax_dcs_write_seq_static(ctx, 0xBF, 0xFD, 0x00, 0x80, 0x9C, 0x12,
			0x00, 0x81);
	himax_dcs_write_seq_static(ctx, 0xB0, 0x00, 0x00);
	himax_dcs_write_seq_static(ctx, 0xD0, 0x07, 0xC0, 0x08, 0x03, 0x11,
		0x00);
	himax_dcs_write_seq_static(ctx, 0xE9, 0xCF);
	himax_dcs_write_seq_static(ctx, 0xBA, 0x03);
	himax_dcs_write_seq_static(ctx, 0xE9, 0x3F);
	himax_dcs_write_seq_static(ctx, 0xB9, 0x00, 0x00, 0x00);
#else /*60hz without dsc*/
	himax_dcs_write_seq_static(ctx, 0xB9, 0x83, 0x12, 0x1A, 0x55, 0x00);
	himax_dcs_write_seq_static(ctx, 0x51, 0x08, 0x00);
	himax_dcs_write_seq_static(ctx, 0x53, 0x24);
	himax_dcs_write_seq_static(ctx, 0xB1, 0x1C, 0x6B, 0x6B, 0x27, 0xE7,
			0x00, 0x1B, 0x11, 0x26, 0x26, 0x2D, 0x2D, 0x1F, 0x33,
			0x31, 0x40, 0xCD, 0xFF, 0x1A, 0x05, 0x15, 0x98, 0x00,
			0x88, 0xFF, 0xFF, 0xFF, 0xCF);
	himax_dcs_write_seq_static(ctx, 0xB2, 0x00, 0x6A, 0x40, 0x00, 0x00,
			0x14, 0x3A, 0x40, 0x7A, 0x02, 0x80, 0x21, 0x21, 0x00,
			0x00, 0x10, 0x27);
	himax_dcs_write_seq_static(ctx, 0xB4, 0x08, 0x7F, 0x08, 0x7F, 0x08,
			0x7F, 0x0A, 0x7F, 0x0A, 0x7F, 0x0A, 0x7F, 0x0A, 0x7F,
			0x00, 0x00, 0x0E, 0x0F, 0x00, 0x2D, 0x0B, 0x06, 0x0B,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0xFF, 0x00, 0xFF, 0x14, 0x66);
	himax_dcs_write_seq_static(ctx, 0xB6, 0x8F, 0x8F, 0x03);
	himax_dcs_write_seq_static(ctx, 0xBC, 0x06, 0x02);
	himax_dcs_write_seq_static(ctx, 0xC0, 0x34, 0x34, 0x44, 0x00, 0x08,
			0xD8);
	himax_dcs_write_seq_static(ctx, 0xC3, 0x87, 0xF2, 0x30);
	himax_dcs_write_seq_static(ctx, 0xC9, 0x00, 0x1E, 0x80, 0xA5, 0x01);
	himax_dcs_write_seq_static(ctx, 0xCB, 0x00, 0x13, 0x38, 0x00, 0x0B,
			0x2D);
	himax_dcs_write_seq_static(ctx, 0xCC, 0x02);
	himax_dcs_write_seq_static(ctx, 0xD1, 0x07);
	himax_dcs_write_seq_static(ctx, 0xD3, 0x00, 0xC0, 0x08, 0x08, 0x08,
			0x04, 0x04, 0x04, 0x14, 0x02, 0x07, 0x07, 0x07, 0x31,
			0x13, 0x12, 0x12, 0x12, 0x03, 0x03, 0x03, 0x32, 0x10,
			0x11, 0x00, 0x11, 0x32, 0x10, 0x03, 0x00, 0x03, 0x32,
			0x10, 0x03, 0x00, 0x03, 0x00, 0x00, 0xFF, 0x00);
	himax_dcs_write_seq_static(ctx, 0xD5, 0x19, 0x19, 0x18, 0x18, 0x02,
			0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06,
			0x07, 0x07, 0x00, 0x00, 0x01, 0x01, 0x18, 0x18, 0x40,
			0x40, 0x20, 0x20, 0x18, 0x18, 0x18, 0x18, 0x40, 0x40,
			0x18, 0x18, 0x2F, 0x2F, 0x31, 0x31, 0x2F, 0x2F, 0x31,
			0x31, 0x18, 0x18, 0x41, 0x41, 0x41, 0x41);
	himax_dcs_write_seq_static(ctx, 0xD6, 0x40, 0x40, 0x18, 0x18, 0x05,
			0x05, 0x04, 0x04, 0x03, 0x03, 0x02, 0x02, 0x01, 0x01,
			0x00, 0x00, 0x07, 0x07, 0x06, 0x06, 0x18, 0x18, 0x19,
			0x19, 0x20, 0x20, 0x18, 0x18, 0x18, 0x18, 0x40, 0x40,
			0x18, 0x18, 0x2F, 0x2F, 0x31, 0x31, 0x2F, 0x2F, 0x31,
			0x31, 0x18, 0x18, 0x41, 0x41, 0x41, 0x41);
	himax_dcs_write_seq_static(ctx, 0xE1, 0x11, 0x00, 0x00, 0x89, 0x30,
			0x80, 0x0A, 0x00, 0x03, 0x20, 0x00, 0x14, 0x03, 0x20,
			0x03, 0x20, 0x02, 0x00, 0x02, 0x91, 0x00, 0x20, 0x02,
			0x47, 0x00, 0x0B, 0x00, 0x0C, 0x05, 0x0E, 0x03, 0x68,
			0x18, 0x00, 0x10, 0xE0, 0x03, 0x0C, 0x20, 0x00, 0x06,
			0x0B, 0x0B, 0x33, 0x0E, 0x1C, 0x2A, 0x38, 0x46, 0x54,
			0x62, 0x69, 0x70, 0x77, 0x79, 0x7B, 0x7D, 0x7E, 0x01,
			0x02, 0x01, 0x00, 0x09);
	himax_dcs_write_seq_static(ctx, 0xE7, 0x05, 0x0A, 0x0A, 0x25, 0x22,
			0x44, 0x02, 0x23, 0x44, 0x19, 0x02);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x01);
	himax_dcs_write_seq_static(ctx, 0xB1, 0x01, 0x23, 0x00);
	himax_dcs_write_seq_static(ctx, 0xD8, 0x20, 0x00, 0x02, 0x22, 0x00,
			0x00, 0x20, 0x00, 0x02, 0x22, 0x00, 0x00, 0x20, 0x00,
			0x02, 0x22, 0x00, 0x00, 0x20, 0x00, 0x02, 0x22, 0x00,
			0x00, 0x20, 0x00, 0x02, 0x22, 0x00, 0x00, 0x20, 0x00,
			0x02, 0x22, 0x00, 0x00);
	himax_dcs_write_seq_static(ctx, 0xE1, 0x40, 0x09, 0xBE, 0x19, 0xFC,
			0x19, 0xFA, 0x19, 0xF8, 0x1A, 0x38, 0x1A, 0x78, 0x1A,
			0xB6, 0x2A, 0xF6, 0x2B, 0x34, 0x2B, 0x74, 0x3B, 0x74,
			0x6B, 0xF4);
	himax_dcs_write_seq_static(ctx, 0xE7, 0x02, 0x00, 0xFA, 0x01, 0x02,
			0x09, 0xFE, 0x0A, 0x48, 0x04, 0x04, 0x26);
	himax_dcs_write_seq_static(ctx, 0xCB, 0x13, 0x55, 0x03, 0x28, 0x0D,
			0x08, 0x0B);
	himax_dcs_write_seq_static(ctx, 0xD6, 0x02, 0x04, 0x21, 0x02, 0x04,
			0x21);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x02);
	himax_dcs_write_seq_static(ctx, 0xD8, 0xAF, 0xFF, 0xFA, 0xFA, 0xBF,
			0xEA, 0xAF, 0xFF, 0xFA, 0xFA, 0xBF, 0xEA);
	himax_dcs_write_seq_static(ctx, 0xE7, 0x01, 0x03, 0x01, 0x03, 0x01,
			0x03, 0x02, 0x02, 0x02, 0x02, 0x00, 0x02, 0x81, 0x02,
			0x40, 0x00, 0x3A, 0x90, 0x01, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			0x00, 0x00);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x03);
	himax_dcs_write_seq_static(ctx, 0xD8, 0xAA, 0xAA, 0xAA, 0xAB, 0xBF,
			0xEA, 0xAA, 0xAA, 0xAA, 0xAB, 0xBF, 0xEA, 0xAA, 0xAA,
			0xAA, 0xFA, 0xBF, 0xEA, 0xAA, 0xAA, 0xAA, 0xFA, 0xBF,
			0xEA);
	himax_dcs_write_seq_static(ctx, 0xE1, 0x00, 0x3F);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x00);
	himax_dcs_write_seq_static(ctx, 0xE0, 0x00, 0x19, 0x34, 0x3C, 0x46,
			0x7F, 0x94, 0x9D, 0x9F, 0xA0, 0x9F, 0x9C, 0x97, 0x93,
			0x93, 0x8E, 0x8D, 0x90, 0x90, 0xA7, 0xB9, 0x4D, 0x67,
			0x00, 0x19, 0x34, 0x3C, 0x46, 0x7F, 0x94, 0x9D, 0x9F,
			0xA0, 0x9F, 0x9C, 0x97, 0x93, 0x93, 0x8E, 0x8D, 0x90,
			0x90, 0xA7, 0xB9, 0x4D, 0x67);
	himax_dcs_write_seq_static(ctx, 0xD8, 0xEA, 0xAA, 0xAA, 0xAE, 0xAA,
			0xAF, 0xEA, 0xAA, 0xAA, 0xAE, 0xAA, 0xAF, 0xE0, 0x00,
			0x0A, 0x2E, 0x80, 0x2F, 0xE0, 0x00, 0x0A, 0x2E, 0x80,
			0x2F, 0xE0, 0x00, 0x0A, 0x2E, 0x80, 0x2F, 0xE0, 0x00,
			0x0A, 0x2E, 0x80, 0x2F);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x02);
	himax_dcs_write_seq_static(ctx, 0xBF, 0x72);
	himax_dcs_write_seq_static(ctx, 0xBD, 0x00);
	himax_dcs_write_seq_static(ctx, 0xE9, 0xC7);
	himax_dcs_write_seq_static(ctx, 0xBF, 0x01);
	himax_dcs_write_seq_static(ctx, 0xE9, 0x3F);
	himax_dcs_write_seq_static(ctx, 0xE9, 0xDE);
	himax_dcs_write_seq_static(ctx, 0xB1, 0xCC, 0x03, 0x00);
	himax_dcs_write_seq_static(ctx, 0xE9, 0x3F);
	himax_dcs_write_seq_static(ctx, 0xD0, 0x07, 0xC0, 0x08, 0x03, 0x11,
			0x00);
	himax_dcs_write_seq_static(ctx, 0xB0, 0x00, 0x00);
	himax_dcs_write_seq_static(ctx, 0xB9, 0x83, 0x12, 0x1A);
#endif
	/* self check module */
	msleep(20);
	himax_dcs_write_seq_static(ctx, 0x11);

	msleep(120);
	/* Manufacturer Command Access Protect */
	himax_dcs_write_seq_static(ctx, 0x29);

	pr_info("%s -\n", __func__);
}

static int himax_disable(struct drm_panel *panel)
{
	struct himax *ctx = panel_to_himax(panel);

	pr_info("%s+%d\n", __func__, __LINE__);
	if (!ctx->enabled)
		return 0;

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_POWERDOWN;
		backlight_update_status(ctx->backlight);
	}

	ctx->enabled = false;
	pr_info("%s-%d\n", __func__, __LINE__);

	return 0;
}
static int himax_unprepare(struct drm_panel *panel)
{
	struct himax *ctx = panel_to_himax(panel);

	pr_info("%s+%d\n", __func__, __LINE__);
	if (!ctx->prepared)
		return 0;

	himax_dcs_write_seq_static(ctx, MIPI_DCS_SET_DISPLAY_OFF);

	msleep(20);//wait for touchpanel powerdown
	himax_dcs_write_seq_static(ctx, MIPI_DCS_ENTER_SLEEP_MODE);
	msleep(30);//4     4 frame

	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->reset_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);
	himax_panel_power_disable(ctx);
	ctx->error = 0;
	ctx->prepared = false;
	pr_info("%s-%d\n", __func__, __LINE__);

	return 0;
}
static int himax_prepare(struct drm_panel *panel)
{
	struct himax *ctx = panel_to_himax(panel);
	int ret;

	pr_info("%s+%d\n", __func__, __LINE__);
	if (ctx->prepared)
		return 0;

	himax_panel_power_enable(ctx);
	himax_panel_init(ctx);
	ret = ctx->error;
	if (ret < 0)
		himax_unprepare(panel);

	ctx->prepared = true;
#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_rst(panel);
#endif
#ifdef PANEL_SUPPORT_READBACK
	himax_panel_get_data(ctx);
#endif
	pr_info("%s-%d\n", __func__, __LINE__);

	return ret;
}
static int himax_enable(struct drm_panel *panel)
{
	struct himax *ctx = panel_to_himax(panel);

	pr_info("%s+%d\n", __func__, __LINE__);
	if (ctx->enabled)
		return 0;

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_UNBLANK;
		backlight_update_status(ctx->backlight);
	}

	ctx->enabled = true;
	pr_info("%s-%d\n", __func__, __LINE__);

	return 0;
}
static const struct drm_display_mode default_mode = {
#if HX83121A_120HZ_WITH_DSC
	.clock = 859000,
	.hdisplay = 1600,
	.hsync_start = 1600 + 60,
	.hsync_end = 1600 + 60 + 40,
	.htotal = 1600 + 60 + 40 + 60,
	.vdisplay = 2560,
	.vsync_start = 2560 + 191,
	.vsync_end = 2560 + 191 + 4,
	.vtotal = 2560 + 191 + 4 + 18,
#else
	.clock = 861000,
	.hdisplay = 1600,
	.hsync_start = 1600 + 60,
	.hsync_end = 1600 + 60 + 20,
	.htotal = 1600 + 60 + 20 + 40,
	.vdisplay = 2560,
	.vsync_start = 2560 + 60,
	.vsync_end = 2560 + 60 + 4,
	.vtotal = 2560 + 60 + 4 + 18,
#endif
};
#if defined(CONFIG_MTK_PANEL_EXT)
static struct mtk_panel_params ext_params = {
#if	HX83121A_120HZ_WITH_DSC
	.pll_clk = 502,
#else
	.pll_clk = 476,
#endif
	.physical_width_um = 70200,
	.physical_height_um = 152100,
	.cust_esd_check = 0,
	.esd_check_enable = 0,
	.output_mode = MTK_PANEL_DUAL_PORT,
	.lcm_cmd_if = MTK_PANEL_DUAL_PORT,
	.lcm_esd_check_table[0] = {
		.cmd = 0x53,
		.count = 1,
		.para_list[0] = 0x00,
	},
#if HX83121A_120HZ_WITH_DSC
	.dsc_params = {
		.enable = 1,
		.ver = 0x11, /* [7:4] major [3:0] minor */
		.slice_mode = 0,
		.rgb_swap = 0,
		.dsc_cfg = 34,
		.rct_on = 1,
		.bit_per_channel = 8,
		.dsc_line_buf_depth = 9,
		.bp_enable = 1,
		.bit_per_pixel = 128,
		.pic_height = 2560, /* need to check */
		.pic_width = 800,  /* need to check */
		.slice_height = 20,
		.slice_width = 800,
		.chunk_size = 800,
		.xmit_delay = 512,
		.dec_delay = 657,
		.scale_value = 32,
		.increment_interval = 583,
		.decrement_interval = 11,
		.line_bpg_offset = 12,
		.nfl_bpg_offset = 1294,
		.slice_bpg_offset = 872,
		.initial_offset = 6144,
		.final_offset = 4320,
		.flatness_minqp = 3,
		.flatness_maxqp = 12,
		.rc_model_size = 8192,
		.rc_edge_factor = 6,
		.rc_quant_incr_limit0 = 11,
		.rc_quant_incr_limit1 = 11,
		.rc_tgt_offset_hi = 3,
		.rc_tgt_offset_lo = 3,

		.ext_pps_cfg = {
			.enable = 1,
			.rc_buf_thresh = hx8312a_wqxga_dsi_vdo_120hz_dphy_buf_thresh,
			.range_min_qp = hx8312a_wqxga_dsi_vdo_120hz_dphy_range_min_qp,
			.range_max_qp = hx8312a_wqxga_dsi_vdo_120hz_dphy_range_max_qp,
			.range_bpg_ofs = hx8312a_wqxga_dsi_vdo_120hz_dphy_range_bpg_ofs,
			},
		},
#endif
};

static int panel_ext_reset(struct drm_panel *panel, int on)
{
	struct himax *ctx = panel_to_himax(panel);

	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->reset_gpio, on);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	return 0;
}

static int himax_setbacklight_cmdq(void *dsi, dcs_write_gce cb, void *handle,
				   unsigned int level)
{
	return 0;
}

static struct mtk_panel_funcs ext_funcs = {
	.reset = panel_ext_reset,
	.set_backlight_cmdq = himax_setbacklight_cmdq,
};
#endif

struct panel_desc {
	const struct drm_display_mode *modes;
	unsigned int num_modes;
	unsigned int bpc;
	struct {
		unsigned int width;
		unsigned int height;
	} size;
	/**
	 * @prepare: the time (in milliseconds) that it takes for the panel to
	 *           become ready and start receiving video data
	 * @enable: the time (in milliseconds) that it takes for the panel to
	 *          display the first valid frame after starting to receive
	 *          video data
	 * @disable: the time (in milliseconds) that it takes for the panel to
	 *           turn the display off (no content is visible)
	 * @unprepare: the time (in milliseconds) that it takes for the panel
	 *             to power itself down completely
	 */
	struct {
		unsigned int prepare;
		unsigned int enable;
		unsigned int disable;
		unsigned int unprepare;
	} delay;
};
static int himax_get_modes(struct drm_panel *panel,
					struct drm_connector *connector)
{
	struct drm_display_mode *mode;
//	struct drm_display_mode *mode2;

	pr_info("%s+%d\n", __func__, __LINE__);
	mode = drm_mode_duplicate(connector->dev, &default_mode);
	if (!mode) {
		pr_info("failed to add mode %ux%ux@%u\n",
			default_mode.hdisplay, default_mode.vdisplay,
			drm_mode_vrefresh(&default_mode));
		return -ENOMEM;
	}

	drm_mode_set_name(mode);
	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	drm_mode_probed_add(connector, mode);
	connector->display_info.width_mm = 70;
	connector->display_info.height_mm = 152;
	pr_info("%s-%d\n", __func__, __LINE__);

	return 1;
}

static const struct drm_panel_funcs himax_drm_funcs = {
	.disable = himax_disable,
	.unprepare = himax_unprepare,
	.prepare = himax_prepare,
	.enable = himax_enable,
	.get_modes = himax_get_modes,
};

static int himax_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct himax *ctx;
	struct device_node *backlight;
	int ret;
	struct device_node *dsi_node, *remote_node = NULL, *endpoint = NULL;

	pr_info("%s+%d\n", __func__, __LINE__);

	dsi_node = of_get_parent(dev->of_node);
	if (dsi_node) {
		endpoint = of_graph_get_next_endpoint(dsi_node, NULL);
		if (endpoint) {
			remote_node = of_graph_get_remote_port_parent(endpoint);
			if (!remote_node) {
				pr_info("No panel connected,skip probe lcm\n");
				return -ENODEV;
			}
			pr_info("device node name:%s\n", remote_node->name);
		}
	}

	if (remote_node != dev->of_node) {
		pr_info("%s+ skip probe due to not current lcm\n", __func__);
		return -ENODEV;
	}

	ctx = devm_kzalloc(dev, sizeof(struct himax), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	mipi_dsi_set_drvdata(dsi, ctx);
	ctx->dev = dev;
	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO | MIPI_DSI_MODE_VIDEO_SYNC_PULSE;

	backlight = of_parse_phandle(dev->of_node, "backlight", 0);
	if (backlight) {
		ctx->backlight = of_find_backlight_by_node(backlight);
		of_node_put(backlight);
		if (!ctx->backlight)
			return -EPROBE_DEFER;
	}

	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(dev, "cannot get reset-gpios %ld\n",
			PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	devm_gpiod_put(dev, ctx->reset_gpio);

#ifndef CONFIG_MTK_DISP_NO_LK
	ctx->prepared = true;
	ctx->enabled = true;
#endif
	drm_panel_init(&ctx->panel, dev, &himax_drm_funcs, DRM_MODE_CONNECTOR_DSI);

	ctx->panel.dev = dev;
	ctx->panel.funcs = &himax_drm_funcs;

	drm_panel_add(&ctx->panel);

	ret = mipi_dsi_attach(dsi);
	if (ret < 0) {
		drm_panel_remove(&ctx->panel);
		dev_err(dev, "mipi_dsi_attach fail, ret=%d\n", ret);
		return -EPROBE_DEFER;
	}
#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_handle_reg(&ctx->panel);
	ret = mtk_panel_ext_create(dev, &ext_params, &ext_funcs, &ctx->panel);
	if (ret < 0)
		return ret;
#endif
	pr_info("%s-\n", __func__);
	return ret;
}
static void himax_remove(struct mipi_dsi_device *dsi)
{
	struct himax *ctx = mipi_dsi_get_drvdata(dsi);
#if defined(CONFIG_MTK_PANEL_EXT)
	struct mtk_panel_ctx *ext_ctx = find_panel_ctx(&ctx->panel);
#endif

	mipi_dsi_detach(dsi);
	drm_panel_remove(&ctx->panel);
#if defined(CONFIG_MTK_PANEL_EXT)
	if (ext_ctx != NULL) {
		mtk_panel_detach(ext_ctx);
		mtk_panel_remove(ext_ctx);
	}
#endif
}
static const struct of_device_id himax_of_match[] = {
	{
		.compatible = "himax,hx83121a",
	},
	{} };
MODULE_DEVICE_TABLE(of, himax_of_match);
static struct mipi_dsi_driver himax_driver = {
	.probe = himax_probe,
	.remove = himax_remove,
	.driver = {
			.name = "panel-himax-hx83121a-vdo",
			.owner = THIS_MODULE,
			.of_match_table = himax_of_match,
		},
};
module_mipi_dsi_driver(himax_driver);
MODULE_AUTHOR("Huijuan Xie <huijuan.xie@mediatek.com>");
MODULE_DESCRIPTION("himax hx83121a wqxga Panel Driver");
MODULE_LICENSE("GPL");
