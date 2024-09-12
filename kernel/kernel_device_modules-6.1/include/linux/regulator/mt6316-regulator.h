/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 MediaTek Inc.
 */

#ifndef __LINUX_REGULATOR_MT6316_H
#define __LINUX_REGULATOR_MT6316_H

#define MT6316_SLAVE_ID_3	3
#define MT6316_SLAVE_ID_6	6
#define MT6316_SLAVE_ID_7	7
#define MT6316_SLAVE_ID_8	8
#define MT6316_SLAVE_ID_15	15

#define MT6316_ID_3_MAX		3
#define MT6316_ID_6_MAX		3
#define MT6316_ID_7_MAX		3
#define MT6316_ID_8_MAX		3
#define MT6316_ID_15_MAX	2

enum {
	MT6316_ID_VBUCK1 = 0,
	MT6316_ID_VBUCK2,
	MT6316_ID_VBUCK3,
	MT6316_ID_VBUCK4,
	MT6316_ID_MAX,
};

/* Register */
#define MT6316_TOP_CFG_ELR4			0x143
#define MT6316_BUCK_TOP_CON0			0x1440
#define MT6316_BUCK_TOP_CON1			0x1443
#define MT6316_BUCK_TOP_ELR0			0x1448
#define MT6316_BUCK_TOP_ELR2			0x144A
#define MT6316_BUCK_TOP_ELR4			0x144C
#define MT6316_BUCK_TOP_ELR6			0x144E
#define MT6316_VBUCK1_DBG4			0x14A4
#define MT6316_VBUCK1_DBG8			0x14A8
#define MT6316_VBUCK2_DBG4			0x1524
#define MT6316_VBUCK2_DBG8			0x1528
#define MT6316_VBUCK3_DBG4			0x15A4
#define MT6316_VBUCK3_DBG8			0x15A8
#define MT6316_VBUCK4_DBG4			0x1624
#define MT6316_VBUCK4_DBG8			0x1628
#define MT6316_BUCK_TOP_4PHASE_TOP_ANA_CON0     0x1688
#define MT6316_BUCK_TOP_4PHASE_TOP_ELR_0	0x1690

#endif /* __LINUX_REGULATOR_MT6316_H */