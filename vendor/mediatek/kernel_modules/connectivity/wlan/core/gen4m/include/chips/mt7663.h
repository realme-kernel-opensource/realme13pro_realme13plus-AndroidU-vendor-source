/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2021 MediaTek Inc.
 */

/*! \file  mt7663.h
 *    \brief This file contains the info of MT7663
 */

#ifdef MT7663

#ifndef _MT7663_H
#define _MT7663_H

/*******************************************************************************
 *                         C O M P I L E R   F L A G S
 *******************************************************************************
 */

/*******************************************************************************
 *                    E X T E R N A L   R E F E R E N C E S
 *******************************************************************************
 */

/*******************************************************************************
 *                              C O N S T A N T S
 *******************************************************************************
 */
#define MT7663_CHIP_ID                          (0x7663)
#define MT7663_SW_SYNC0                         CONN_CFG_ON_CONN_ON_MISC_ADDR
#define MT7663_SW_SYNC0_RDY_OFFSET \
	CONN_CFG_ON_CONN_ON_MISC_DRV_FM_STAT_SYNC_SHFT
#define MT7663_PATCH_START_ADDR                 (0x000DC000)
#define MT7663_TOP_CFG_BASE			NIC_CONNAC_CFG_BASE
#define MT7663_TX_DESC_APPEND_LENGTH            32
#define MT7663_RX_DESC_LENGTH                   16
#define MT7663_RX_INIT_EVENT_LENGTH             8
#define MT7663_RX_EVENT_HDR_LENGTH              12

/*******************************************************************************
 *                         D A T A   T Y P E S
 *******************************************************************************
 */

/*******************************************************************************
 *                            P U B L I C   D A T A
 *******************************************************************************
 */

/*******************************************************************************
 *                           P R I V A T E   D A T A
 *******************************************************************************
 */

/*******************************************************************************
 *                                 M A C R O S
 *******************************************************************************
 */

/*******************************************************************************
 *                  F U N C T I O N   D E C L A R A T I O N S
 *******************************************************************************
 */

/*******************************************************************************
 *                              F U N C T I O N S
 *******************************************************************************
 */

#endif /* _MT7663_H */

#endif /* MT7663 */

