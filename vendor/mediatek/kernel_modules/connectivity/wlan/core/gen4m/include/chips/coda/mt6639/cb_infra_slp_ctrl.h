/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 MediaTek Inc.
 */

#ifndef __CB_INFRA_SLP_CTRL_REGS_H__
#define __CB_INFRA_SLP_CTRL_REGS_H__

#include "hal_common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CB_INFRA_SLP_CTRL_BASE \
	0x70025000

#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_ADDR \
	(CB_INFRA_SLP_CTRL_BASE + 0x030)
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_SET_ADDR \
	(CB_INFRA_SLP_CTRL_BASE + 0x034)
#define CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_ADDR \
	(CB_INFRA_SLP_CTRL_BASE + 0x300)


#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_CRYPTO_TOP_MCU_OWN_RDY_ADDR \
	CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_ADDR
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_CRYPTO_TOP_MCU_OWN_RDY_MASK \
	0x00000004
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_CRYPTO_TOP_MCU_OWN_RDY_SHFT \
	2
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_CR_VLP_CRYPTO_TOP_MCU_OWN_ADDR \
	CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_ADDR
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_CR_VLP_CRYPTO_TOP_MCU_OWN_MASK \
	0x00000003
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_CR_VLP_CRYPTO_TOP_MCU_OWN_SHFT \
	0

#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_SET_CR_VLP_CRYPTO_TOP_MCU_OWN_SET_ADDR \
	CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_SET_ADDR
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_SET_CR_VLP_CRYPTO_TOP_MCU_OWN_SET_MASK \
	0x00000003
#define CB_INFRA_SLP_CTRL_CB_INFRA_CRYPTO_TOP_MCU_OWN_SET_CR_VLP_CRYPTO_TOP_MCU_OWN_SET_SHFT \
	0

#define CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_CR_CB_INFRA_SLP_CTRL_DBG_PROBE_SEL_HW1_ADDR \
	CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_ADDR
#define CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_CR_CB_INFRA_SLP_CTRL_DBG_PROBE_SEL_HW1_MASK \
	0x0000FF00
#define CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_CR_CB_INFRA_SLP_CTRL_DBG_PROBE_SEL_HW1_SHFT \
	8
#define CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_CR_CB_INFRA_SLP_CTRL_DBG_PROBE_SEL_HW0_ADDR \
	CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_ADDR
#define CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_CR_CB_INFRA_SLP_CTRL_DBG_PROBE_SEL_HW0_MASK \
	0x000000FF
#define CB_INFRA_SLP_CTRL_CB_INFRA_SLP_DBG_SEL_CR_CB_INFRA_SLP_CTRL_DBG_PROBE_SEL_HW0_SHFT \
	0

#ifdef __cplusplus
}
#endif

#endif
