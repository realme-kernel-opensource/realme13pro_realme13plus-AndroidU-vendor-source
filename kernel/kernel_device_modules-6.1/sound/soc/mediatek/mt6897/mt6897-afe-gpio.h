/* SPDX-License-Identifier: GPL-2.0 */
/*
 * mt6897-afe-gpio.h  --  Mediatek 6897 afe gpio ctrl definition
 *
 *  Copyright (c) 2021 MediaTek Inc.
 *  Author: Tina Tsai <tina.tsai@mediatek.com>
 */

#ifndef _MT6897_AFE_GPIO_H_
#define _MT6897_AFE_GPIO_H_

enum mt6897_afe_gpio {
	MT6897_AFE_GPIO_DAT_MISO0_OFF,
	MT6897_AFE_GPIO_DAT_MISO0_ON,
	MT6897_AFE_GPIO_DAT_MISO1_OFF,
	MT6897_AFE_GPIO_DAT_MISO1_ON,
	MT6897_AFE_GPIO_DAT_MISO2_OFF,
	MT6897_AFE_GPIO_DAT_MISO2_ON,
	MT6897_AFE_GPIO_DAT_MOSI_OFF,
	MT6897_AFE_GPIO_DAT_MOSI_ON,
	MT6897_AFE_GPIO_DAT_MOSI_CH34_OFF,
	MT6897_AFE_GPIO_DAT_MOSI_CH34_ON,
	MT6897_AFE_GPIO_I2SIN0_OFF,
	MT6897_AFE_GPIO_I2SIN0_ON,
	MT6897_AFE_GPIO_I2SOUT0_OFF,
	MT6897_AFE_GPIO_I2SOUT0_ON,
	MT6897_AFE_GPIO_VOW_DAT_OFF,
	MT6897_AFE_GPIO_VOW_DAT_ON,
	MT6897_AFE_GPIO_VOW_CLK_OFF,
	MT6897_AFE_GPIO_VOW_CLK_ON,
	MT6897_AFE_GPIO_VOW_SCP_DMIC_DAT_OFF,
	MT6897_AFE_GPIO_VOW_SCP_DMIC_DAT_ON,
	MT6897_AFE_GPIO_VOW_SCP_DMIC_CLK_OFF,
	MT6897_AFE_GPIO_VOW_SCP_DMIC_CLK_ON,
	MT6897_AFE_GPIO_GPIO_NUM
};

struct mtk_base_afe;

int mt6897_afe_gpio_init(struct mtk_base_afe *afe);
int mt6897_afe_gpio_request(struct mtk_base_afe *afe, bool enable,
			    int dai, int uplink);
bool mt6897_afe_gpio_is_prepared(enum mt6897_afe_gpio type);

#endif
