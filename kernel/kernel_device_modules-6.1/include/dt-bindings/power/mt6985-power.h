/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2022 MediaTek Inc.
 * Author: Owen Chen <owen.chen@mediatek.com>
 */

#ifndef _DT_BINDINGS_POWER_MT6985_POWER_H
#define _DT_BINDINGS_POWER_MT6985_POWER_H

/* GPU_EB_RPC */
#define MT6985_POWER_DOMAIN_MFG1			0
#define MT6985_POWER_DOMAIN_MFG2			1
#define MT6985_POWER_DOMAIN_MFG3			2
#define MT6985_POWER_DOMAIN_MFG4			3
#define MT6985_POWER_DOMAIN_MFG5			4
#define MT6985_POWER_DOMAIN_MFG6			5
#define MT6985_POWER_DOMAIN_MFG7			6
#define MT6985_POWER_DOMAIN_MFG8			7
#define MT6985_POWER_DOMAIN_MFG9			8
#define MT6985_POWER_DOMAIN_MFG10			9
#define MT6985_POWER_DOMAIN_MFG11			10
#define MT6985_POWER_DOMAIN_MFG12			11
#define MT6985_POWER_DOMAIN_MFG13			12
#define MT6985_POWER_DOMAIN_MFG14			13
#define MT6985_POWER_DOMAIN_MFG15			14
#define MT6985_POWER_DOMAIN_MFG16			15
#define MT6985_POWER_DOMAIN_MFG17			16
#define MT6985_POWER_DOMAIN_MFG18			17
#define MT6985_POWER_DOMAIN_MFG19			18
#define MT6985_GPU_EB_RPC_POWER_DOMAIN_NR		19

/* SPM */
#define MT6985_POWER_DOMAIN_MD				0
#define MT6985_POWER_DOMAIN_CONN			1
#define MT6985_POWER_DOMAIN_UFS0_SHUTDOWN		2
#define MT6985_POWER_DOMAIN_UFS0_PHY_SHUTDOWN		3
#define MT6985_POWER_DOMAIN_PEXTP_MAC0_SHUTDOWN		4
#define MT6985_POWER_DOMAIN_PEXTP_MAC1_SHUTDOWN		5
#define MT6985_POWER_DOMAIN_PEXTP_PHY0			6
#define MT6985_POWER_DOMAIN_PEXTP_PHY1			7
#define MT6985_POWER_DOMAIN_AUDIO			8
#define MT6985_POWER_DOMAIN_ADSP_TOP_DORMANT		9
#define MT6985_POWER_DOMAIN_MM_INFRA			10
#define MT6985_POWER_DOMAIN_MM_PROC_DORMANT		11
#define MT6985_POWER_DOMAIN_ISP_VCORE			12
#define MT6985_POWER_DOMAIN_ISP_MAIN			13
#define MT6985_POWER_DOMAIN_ISP_DIP1			14
#define MT6985_POWER_DOMAIN_VDE_VCORE0			15
#define MT6985_POWER_DOMAIN_VDE0			16
#define MT6985_POWER_DOMAIN_VDE_VCORE1			17
#define MT6985_POWER_DOMAIN_VDE1			18
#define MT6985_POWER_DOMAIN_VEN0			19
#define MT6985_POWER_DOMAIN_VEN1			20
#define MT6985_POWER_DOMAIN_VEN2			21
#define MT6985_POWER_DOMAIN_CAM_VCORE			22
#define MT6985_POWER_DOMAIN_CAM_MAIN			23
#define MT6985_POWER_DOMAIN_CAM_MRAW			24
#define MT6985_POWER_DOMAIN_CAM_SUBA			25
#define MT6985_POWER_DOMAIN_CAM_SUBB			26
#define MT6985_POWER_DOMAIN_CAM_SUBC			27
#define MT6985_POWER_DOMAIN_MDP0_SHUTDOWN		28
#define MT6985_POWER_DOMAIN_MDP1_SHUTDOWN		29
#define MT6985_POWER_DOMAIN_DIS0_SHUTDOWN		30
#define MT6985_POWER_DOMAIN_DIS1_SHUTDOWN		31
#define MT6985_POWER_DOMAIN_OVLSYS_SHUTDOWN		32
#define MT6985_POWER_DOMAIN_OVLSYS1_SHUTDOWN		33
#define MT6985_POWER_DOMAIN_DP_TX			34
#define MT6985_POWER_DOMAIN_CSI_RX			35
#define MT6985_POWER_DOMAIN_APU				36
#define MT6985_SPM_POWER_DOMAIN_NR			37


#endif /* _DT_BINDINGS_POWER_MT6985_POWER_H */
