/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2021 MediaTek Inc.
 */

#ifndef MT6878_CONSYS_REG_OFFSET_H
#define MT6878_CONSYS_REG_OFFSET_H

#include "mt6878_consys_reg.h"

/**********************************************************************
 * infracfg_ao
 * Base: 0x1000_1000
 * Min offset: 0x10001050
 * Max offset: 0x10001ee0
 * Size: 0xee4
 *********************************************************************/
#define INFRACFG_AO_REG_BASE				(CONN_REG_INFRACFG_AO_ADDR)
#define INFRACFG_AO_INFRASYS_PROTECT_EN_STA_0		(INFRACFG_AO_REG_BASE+0xC40)
#define INFRACFG_AO_INFRASYS_PROTECT_EN_STA_1		(INFRACFG_AO_REG_BASE+0xC50)
#define INFRACFG_AO_INFRASYS_PROTECT_RDY_STA_1		(INFRACFG_AO_REG_BASE+0xC5C)
#define INFRACFG_AO_MCU_CONNSYS_PROTECT_EN_STA_0	(INFRACFG_AO_REG_BASE+0xC90)
#define INFRACFG_AO_MCU_CONNSYS_PROTECT_RDY_STA_0	(INFRACFG_AO_REG_BASE+0xC9C)

/**********************************************************************
 * GPIO
 * Base: 0x1000_5000
 * Min offset: 0x0
 * Max offset: 0xa78
 * Size: 0xa7c
 *********************************************************************/
#define GPIO_REG_BASE	(CONN_REG_GPIO_ADDR)
//TODO
#define GPIO_DIR6_SET	(GPIO_REG_BASE + 0x0064)
#define GPIO_DOUT6_SET	(GPIO_REG_BASE + 0x0164)
#define GPIO_MODE16	(GPIO_REG_BASE + 0x0400)
#define GPIO_MODE23	(GPIO_REG_BASE + 0x0470)
#define GPIO_MODE24	(GPIO_REG_BASE + 0x0480)
#define GPIO_MODE25	(GPIO_REG_BASE + 0x0490)

/**********************************************************************
 * conn_infra_rgu_on
 * Base: 0x1800_0000
 * Size: 0x470
 *********************************************************************/
#include "CODA/conn_rgu_on.h"

/**********************************************************************
 * conn_infra_cfg_on
 * Base: 0x1800_1000
 * Size: 0x658
 *********************************************************************/
#include "CODA/conn_cfg_on.h"

/**********************************************************************
 * conn_wt_slp_ctl_reg
 * Base: 0x1800_3000
 * Size: 0x204
 *********************************************************************/
#include "CODA/conn_wt_slp_ctl_reg.h"

/**********************************************************************
 * conn_infra_bus_cr_on
 * Base: 0x1800_e000
 * Min offset: 0x1800e000
 * Max offset: 0x1800e120
 * Size: 0x124
 *********************************************************************/
#include "CODA/conn_bus_cr_on.h"

/**********************************************************************
 * conn_infra_cfg
 * Base: 0x1801_1000
 * Size: 0x138
 *********************************************************************/
#include "CODA/conn_cfg.h"
#define CONN_HW_VER	0x02050401

/**********************************************************************
 * conn_infra_clkgen_top
 * Base: 0x1801_2000
 * Size: 0x100
 *********************************************************************/
#include "CODA/conn_clkgen_top.h"

/**********************************************************************
 * conn_von_bus_bcrm
 * Base: 0x1802_0000
 * Size: 0x04c
 *********************************************************************/
#include "CODA/conn_von_bus_bcrm.h"

/**********************************************************************
 * conn_dbg_ctl
 * Base: 0x1802_3000
 * Size: 0xe28
 *********************************************************************/
#include "CODA/conn_dbg_ctl.h"

/**********************************************************************
 * conn_infra_on_bus_bcrm
 * Base: 0x1803_b000
 * Size: 0x018
 *********************************************************************/
#include "CODA/conn_on_bus_bcrm.h"

/**********************************************************************
 * conn_therm_ctl
 * Base: 0x1804_0000
 * Size: 0x2c
 *********************************************************************/
#include "CODA/conn_therm_ctl.h"

/**********************************************************************
 * conn_afe_ctl
 * Base: 0x1804_1000
 * Size: 0x128
 *********************************************************************/
#include "CODA/conn_afe_ctl.h"

/**********************************************************************
 * conn_rf_spi_mst_reg
 * Base: 0x1804_2000
 * Size: 0x324
 *********************************************************************/
#include "CODA/conn_rf_spi_mst_reg.h"
/* For RFSPI table usage */
#define CONN_RF_SPI_MST_REG_SPI_STA_OFFSET		0x0000
#define CONN_RF_SPI_MST_REG_SPI_WF_ADDR_OFFSET		0x0010
#define CONN_RF_SPI_MST_REG_SPI_WF_WDAT_OFFSET		0x0014
#define CONN_RF_SPI_MST_REG_SPI_WF_RDAT_OFFSET		0x0018
#define CONN_RF_SPI_MST_REG_SPI_BT_ADDR_OFFSET		0x0020
#define CONN_RF_SPI_MST_REG_SPI_BT_WDAT_OFFSET		0x0024
#define CONN_RF_SPI_MST_REG_SPI_BT_RDAT_OFFSET		0x0028
#define CONN_RF_SPI_MST_REG_SPI_FM_ADDR_OFFSET		0x0030
#define CONN_RF_SPI_MST_REG_SPI_FM_WDAT_OFFSET		0x0034
#define CONN_RF_SPI_MST_REG_SPI_FM_RDAT_OFFSET		0x0038
#define CONN_RF_SPI_MST_REG_SPI_TOP_ADDR_OFFSET		0x0050
#define CONN_RF_SPI_MST_REG_SPI_TOP_WDAT_OFFSET		0x0054
#define CONN_RF_SPI_MST_REG_SPI_TOP_RDAT_OFFSET		0x0058
#define CONN_RF_SPI_MST_REG_SPI_GPS_GPS_ADDR_OFFSET	0x0210
#define CONN_RF_SPI_MST_REG_SPI_GPS_GPS_WDAT_OFFSET	0x0214
#define CONN_RF_SPI_MST_REG_SPI_GPS_GPS_RDAT_OFFSET	0x0218

/**********************************************************************
 * conn_rf_spi_1_mst_reg
 * Base: 0x1804_6000
 * Size: 0x324
 *********************************************************************/
#include "CODA/conn_rf_spi_mst_reg.h"
/* For RFSPI 1 table usage, same as conn_rf_spi_mst_reg*/

/**********************************************************************
 * conn_infra_bus_cr
 * Base: 0x1804_b000
 * Size: 0x414
 *********************************************************************/
#include "CODA/conn_bus_cr.h"

/**********************************************************************
 * conn_infra_off_debug_ctrl_ao
 * Base: 0x1804_d000
 * Size: 0x41c
 *********************************************************************/
#include "CODA/conn_off_debug_ctrl_ao.h"

/**********************************************************************
 * conn_infra_off_bus_bcrm
 * Base: 0x1804_f000
 * Size: 0x148
 *********************************************************************/
#include "CODA/conn_off_bus_bcrm.h"

/**********************************************************************
 * conn_infra_sysram_sw_cr
 * Base: 0x1805_3800
 * Size: 4K (0x1805_3800~0x1805_4000)
 *********************************************************************/
#define CONN_INFRA_SYSRAM_SW_CR_BASE			(CONN_REG_CONN_INFRA_SYSRAM_SW_CR_ADDR)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_CHIP_ID		(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x000)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_0	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x004)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_1	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x008)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_2	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x00C)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_3	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x010)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_4	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x048)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_5	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x04C)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_6	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x050)
#define CONN_INFRA_SYSRAM_SW_CR_A_DIE_EFUSE_DATA_7	(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x054)

#define CONN_INFRA_SYSRAM_SW_CR_D_DIE_EFUSE		(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x020)

#define CONN_INFRA_SYSRAM_SW_CR_RADIO_STATUS		(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x034)
#define CONN_INFRA_SYSRAM_SW_CR_BUILD_MODE		(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x038)
#define CONN_INFRA_SYSRAM_SW_CR_CLOCK_TYPE		(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x03C)
#define CONN_INFRA_SYSRAM_SW_CR_MCU_LOG_CONTROL		(CONN_INFRA_SYSRAM_SW_CR_BASE + 0x040)
/* Need to clean full region */
#define CONN_INFRA_SYSRAM_BASE				0x18050000
#define CONN_INFRA_SYSRAM_SIZE				(20 * 1024)

/**********************************************************************
 * conn_host_csr_top
 * Base: 0x1806_0000
 * Size: 0x7cc
 *********************************************************************/
#include "CODA/conn_host_csr_top.h"

/**********************************************************************
 * conn_semaphore
 * Base: 0x1807_0000
 * Size: 0x8004
 *********************************************************************/
#include "CODA/conn_semaphore.h"

/**********************************************************************
 * spm
 * Base: 0x1c00_1000
 * Size: 0xfb0
 *********************************************************************/
#define SPM_REG_BASE			(CONN_REG_SPM_ADDR)
#define SPM_POWERON_CONFIG_EN		(SPM_REG_BASE + 0x0000)
#define SPM_CONN_PWR_CON		(SPM_REG_BASE + 0x0E04)
#define SPM_OTHER_PWR_STATUS		(SPM_REG_BASE + 0x0E04)
#define SPM_PWR_STATUS_2ND		(SPM_REG_BASE + 0x0E04)


/**********************************************************************
 * Misc
 *********************************************************************/
#define RC_CENTRAL_CFG1 0x1C00D004
#define DCXO_DIGCLK_ELR 0x7f4

#define CONSYS_GEN_CKSYS_REG_BASE_ADDR 0x10000000

#endif /* MT6878_CONSSY_REG_OFFSET_H */

