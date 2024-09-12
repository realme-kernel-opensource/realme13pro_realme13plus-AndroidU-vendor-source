/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 MediaTek Inc.
 * Author: Owen Chen <owen.chen@mediatek.com>
 */

#ifndef __DRV_CLKCHK_MT6989_H
#define __DRV_CLKCHK_MT6989_H

enum chk_sys_id {
	top = 0,
	ifrao = 1,
	apmixed = 2,
	bcrm_infra1_bus = 3,
	ifr_bus = 4,
	emi_reg0 = 5,
	emi_reg1 = 6,
	ssr_top = 7,
	bcrm_ssr_bus = 8,
	perao = 9,
	afe = 10,
	impc = 11,
	ufsao = 12,
	ufspdn = 13,
	pext = 14,
	imps = 15,
	impes = 16,
	impn = 17,
	gpu_eb_rpc = 18,
	mfg_ao = 19,
	mfgsc0_ao = 20,
	mfgsc1_ao = 21,
	mm = 22,
	mm1 = 23,
	ovl = 24,
	ovl1 = 25,
	img = 26,
	dip_top_dip1 = 27,
	dip_nr1_dip1 = 28,
	dip_nr2_dip1 = 29,
	wpe1_dip1 = 30,
	wpe2_dip1 = 31,
	wpe3_dip1 = 32,
	traw_dip1 = 33,
	traw_cap_dip1 = 34,
	img_v = 35,
	vde1 = 36,
	vde2 = 37,
	ven1 = 38,
	ven2 = 39,
	ven_c2 = 40,
	spm = 41,
	vlpcfg = 42,
	vlp_ck = 43,
	scp = 44,
	scp_iic = 45,
	scp_fast_iic = 46,
	hfrp_2_bus = 47,
	hfrp = 48,
	hfrp_1_bus = 49,
	cam_m = 50,
	camsys_rmsa = 51,
	cam_ra = 52,
	cam_ya = 53,
	camsys_rmsb = 54,
	cam_rb = 55,
	cam_yb = 56,
	camsys_rmsc = 57,
	cam_rc = 58,
	cam_yc = 59,
	cam_mr = 60,
	camsys_ipe = 61,
	ccu = 62,
	cam_vcore = 63,
	dvfsrc_apb = 64,
	mminfra_config = 65,
	mminfra_ao_config = 66,
	mdp = 67,
	mdp1 = 68,
	cci = 69,
	cpu_ll = 70,
	cpu_bl = 71,
	cpu_b = 72,
	ptp = 73,
	hwv = 74,
	hwv_ext = 75,
	hwv_wrt = 76,
	mm_hwv = 77,
	mm_hwv_ext = 78,
	mm_hwv_wrt = 79,
	hfrp_hwv = 80,
	hfrp_irq = 81,
	vlp_ao = 82,
	apmixed_ext = 83,
	bcrm_ifrao = 84,
	ifrao_bus_dbg = 85,
	chk_sys_num = 86,
};

enum chk_pd_id {
	MT6989_CHK_PD_MD1 = 0,
	MT6989_CHK_PD_CONN = 1,
	MT6989_CHK_PD_PERI_USB0 = 2,
	MT6989_CHK_PD_PEXTP_MAC0 = 3,
	MT6989_CHK_PD_PEXTP_MAC1 = 4,
	MT6989_CHK_PD_PEXTP_PHY0 = 5,
	MT6989_CHK_PD_PEXTP_PHY1 = 6,
	MT6989_CHK_PD_AUDIO = 7,
	MT6989_CHK_PD_ADSP_AO = 8,
	MT6989_CHK_PD_ADSP_TOP = 9,
	MT6989_CHK_PD_MM_INFRA = 10,
	MT6989_CHK_PD_ISP_VCORE = 11,
	MT6989_CHK_PD_ISP_MAIN = 12,
	MT6989_CHK_PD_ISP_TRAW = 13,
	MT6989_CHK_PD_ISP_DIP1 = 14,
	MT6989_CHK_PD_VDE_VCORE0 = 15,
	MT6989_CHK_PD_VDE0 = 16,
	MT6989_CHK_PD_VDE_VCORE1 = 17,
	MT6989_CHK_PD_VDE1 = 18,
	MT6989_CHK_PD_VEN0 = 19,
	MT6989_CHK_PD_VEN1 = 20,
	MT6989_CHK_PD_VEN2 = 21,
	MT6989_CHK_PD_CAM_VCORE = 22,
	MT6989_CHK_PD_CAM_MAIN = 23,
	MT6989_CHK_PD_CAM_MRAW = 24,
	MT6989_CHK_PD_CAM_SUBA = 25,
	MT6989_CHK_PD_CAM_SUBB = 26,
	MT6989_CHK_PD_CAM_SUBC = 27,
	MT6989_CHK_PD_CAM_CCU = 28,
	MT6989_CHK_PD_CAM_CCU_AO = 29,
	MT6989_CHK_PD_DISP_VCORE = 30,
	MT6989_CHK_PD_DIS1 = 31,
	MT6989_CHK_PD_MML0 = 32,
	MT6989_CHK_PD_MML1 = 33,
	MT6989_CHK_PD_DIS0 = 34,
	MT6989_CHK_PD_OVL0 = 35,
	MT6989_CHK_PD_OVL1 = 36,
	MT6989_CHK_PD_DP_TX = 37,
	MT6989_CHK_PD_CSI_RX = 38,
	MT6989_CHK_PD_SSRSYS = 39,
	MT6989_CHK_PD_APU = 40,
	MT6989_CHK_PD_NUM,
};

#ifdef CONFIG_MTK_DVFSRC_HELPER
extern int get_sw_req_vcore_opp(void);
#endif

extern void print_subsys_reg_mt6989(enum chk_sys_id id);
extern void set_subsys_reg_dump_mt6989(enum chk_sys_id id[]);
extern void get_subsys_reg_dump_mt6989(void);
extern u32 get_mt6989_reg_value(u32 id, u32 ofs);
#endif	/* __DRV_CLKCHK_MT6989_H */
