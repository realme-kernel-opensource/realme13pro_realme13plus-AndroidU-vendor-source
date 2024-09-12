// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2023 MediaTek Inc.
 * Author: Wendy-ST Lin <wendy-st.lin@mediatek.com>
 */
#include <dt-bindings/interconnect/mtk,mmqos.h>
#include <dt-bindings/interconnect/mtk,mt6873-emi.h>
#include <dt-bindings/memory/mt6878-larb-port.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include "mmqos-mtk.h"
static const struct mtk_node_desc node_descs_mt6878[] = {
	DEFINE_MNODE(common0,
		SLAVE_COMMON(0), 0, false, 0x0, MMQOS_NO_LINK), //DISP
	DEFINE_MNODE(common0_port0,
		MASTER_COMMON_PORT(0, 0), 0, false, 0x1, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port1,
		MASTER_COMMON_PORT(0, 1), 0, false, 0x2, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port2,
		MASTER_COMMON_PORT(0, 2), 0, false, 0x1, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port3,
		MASTER_COMMON_PORT(0, 3), 0, false, 0x2, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port4,
		MASTER_COMMON_PORT(0, 4), 0, false, 0x1, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port5,
		MASTER_COMMON_PORT(0, 5), 0, false, 0x2, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port6,
		MASTER_COMMON_PORT(0, 6), 0, false, 0x1, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port7,
		MASTER_COMMON_PORT(0, 7), 0, false, 0x2, SLAVE_COMMON(0)),
	DEFINE_MNODE(common0_port8,
		MASTER_COMMON_PORT(0, 8), 0, false, 0x1, SLAVE_COMMON(0)), // disp virtual larb
	/* SMI DISP COMMON */
	DEFINE_MNODE(DISP_larb0,  SLAVE_LARB(0),  0,   false, 0x0, MASTER_COMMON_PORT(0, 0)),
	DEFINE_MNODE(DISP_larb1,  SLAVE_LARB(1),  0,   false, 0x0, MASTER_COMMON_PORT(0, 1)),
	DEFINE_MNODE(MML_larb2,   SLAVE_LARB(2),  0,   false, 0x0, MASTER_COMMON_PORT(0, 2)),
	DEFINE_MNODE(IMG_larb9,   SLAVE_LARB(9),  0x6, false, 0x0, MASTER_COMMON_PORT(0, 3)), //img input1 read
	DEFINE_MNODE(IMG_larb11,  SLAVE_LARB(11), 0,   false, 0x0, MASTER_COMMON_PORT(0, 3)),
	DEFINE_MNODE(IMG_larb22,  SLAVE_LARB(22), 0x8, true,  0x0, MASTER_COMMON_PORT(0, 3)), //img input0 write
	DEFINE_MNODE(IMG_larb28,  SLAVE_LARB(28), 0x6, false, 0x0, MASTER_COMMON_PORT(0, 3)), //img input0 read
	DEFINE_MNODE(IMG_larb38,  SLAVE_LARB(38), 0,   false, 0x0, MASTER_COMMON_PORT(0, 3)),
	DEFINE_MNODE(IMG_larb12,  SLAVE_LARB(12), 0,   false, 0x0, MASTER_COMMON_PORT(0, 3)),
	DEFINE_MNODE(IMG_larb15,  SLAVE_LARB(15), 0x8, true,  0x0, MASTER_COMMON_PORT(0, 3)), //img input1 write
	DEFINE_MNODE(VENC_larb7,  SLAVE_LARB(7),  0x7, false, 0x0, MASTER_COMMON_PORT(0, 4)),
	DEFINE_MNODE(VENC_larb8,  SLAVE_LARB(8),  0x8, true,  0x0, MASTER_COMMON_PORT(0, 4)), //venc write
	DEFINE_MNODE(VDEC_larb4,  SLAVE_LARB(4),  0x7, false, 0x0, MASTER_COMMON_PORT(0, 5)),
	DEFINE_MNODE(VDEC_larb5,  SLAVE_LARB(5),  0x8, true,  0x0, MASTER_COMMON_PORT(0, 5)), // vdec write
	DEFINE_MNODE(CAM_larb16,  SLAVE_LARB(16), 0,   false, 0x0, MASTER_COMMON_PORT(0, 6)),
	DEFINE_MNODE(CAM_larb34,  SLAVE_LARB(34), 0,   false, 0x0, MASTER_COMMON_PORT(0, 6)),
	DEFINE_MNODE(CAM_larb13,  SLAVE_LARB(13), 0,   false, 0x0, MASTER_COMMON_PORT(0, 6)),
	DEFINE_MNODE(CAM_larb29,  SLAVE_LARB(29), 0,   false, 0x0, MASTER_COMMON_PORT(0, 6)),
	DEFINE_MNODE(CAM_larb14,  SLAVE_LARB(14), 0,   false, 0x0, MASTER_COMMON_PORT(0, 7)),
	DEFINE_MNODE(CAM_larb33,  SLAVE_LARB(33), 0,   false, 0x0, MASTER_COMMON_PORT(0, 7)),
	DEFINE_MNODE(CAM_larb17,  SLAVE_LARB(17), 0,   false, 0x0, MASTER_COMMON_PORT(0, 7)),
	DEFINE_MNODE(CAM_larb25,  SLAVE_LARB(25), 0,   false, 0x0, MASTER_COMMON_PORT(0, 7)),
	DEFINE_MNODE(CAM_larb30,  SLAVE_LARB(30), 0,   false, 0x0, MASTER_COMMON_PORT(0, 7)), //CCU
	/* Virtual larb */
	DEFINE_MNODE(DISP_larb39, SLAVE_LARB(39), 0, false, 0x0, MASTER_COMMON_PORT(0, 8)), //DISP
	/* LARB0 */
	DEFINE_MNODE(disp_postmask0_larb0_0,
		MASTER_LARB_PORT(M4U_L0_P0_DISP_POSTMASK0),        8, false, 0x0, SLAVE_LARB(0)),
	DEFINE_MNODE(disp_ovl0_2L_hdr_larb0_1,
		MASTER_LARB_PORT(M4U_L0_P1_DISP_OVL0_2L_HDR),      7, false, 0x0, SLAVE_LARB(0)),
	DEFINE_MNODE(disp_ovl0_2L_rdma0_larb0_2,
		MASTER_LARB_PORT(M4U_L0_P2_DISP_OVL0_2L_RDMA0),    8, false, 0x0, SLAVE_LARB(0)),
	DEFINE_MNODE(disp_ovl2_2L_hdr_larb0_3,
		MASTER_LARB_PORT(M4U_L0_P3_DISP_OVL2_2L_HDR),      7, false, 0x0, SLAVE_LARB(0)),
	DEFINE_MNODE(disp_ovl2_2L_rdma_larb0_4,
		MASTER_LARB_PORT(M4U_L0_P4_DISP_OVL2_2L_RDMA0),    8, false, 0x0, SLAVE_LARB(0)),
	DEFINE_MNODE(disp_ufbc_wdma0_larb0_5,
		MASTER_LARB_PORT(M4U_L0_P5_DISP_UFBC_WDMA0),       9, true,  0x0, SLAVE_LARB(0)),
	DEFINE_MNODE(disp_fake0_larb0_6,
		MASTER_LARB_PORT(M4U_L0_P6_DISP_FAKE0),            8, false, 0x0, SLAVE_LARB(0)),
	/* LARB1 */
	DEFINE_MNODE(disp_ovl1_2L_hdr_larb1_0,
		MASTER_LARB_PORT(M4U_L1_P0_DISP_OVL1_2L_HDR),      7, false, 0x0, SLAVE_LARB(1)),
	DEFINE_MNODE(disp_ovl1_2L_rdma0_larb1_1,
		MASTER_LARB_PORT(M4U_L1_P1_DISP_OVL1_2L_RDMA0),    8, false, 0x0, SLAVE_LARB(1)),
	DEFINE_MNODE(disp_ovl3_2L_hdr_larb1_2,
		MASTER_LARB_PORT(M4U_L1_P2_DISP_OVL3_2L_HDR),      7, false, 0x0, SLAVE_LARB(1)),
	DEFINE_MNODE(disp_ovl3_2L_rdma_larb1_3,
		MASTER_LARB_PORT(M4U_L1_P3_DISP_OVL3_2L_RDMA0),    8, false, 0x0, SLAVE_LARB(1)),
	DEFINE_MNODE(disp_wdma1_larb1_4,
		MASTER_LARB_PORT(M4U_L1_P4_DISP_WDMA1),            9, true,  0x0, SLAVE_LARB(1)),
	DEFINE_MNODE(disp_fake1_larb1_5,
		MASTER_LARB_PORT(M4U_L1_P5_DISP_FAKE1),            8, false, 0x0, SLAVE_LARB(1)),
	/* LARB2 */
	DEFINE_MNODE(mdp_rdma0_larb2_0,
		MASTER_LARB_PORT(M4U_L2_P0_MDP_RDMA0),             8, false, 0x0, SLAVE_LARB(2)),
	DEFINE_MNODE(mdp_rdma1_larb2_1,
		MASTER_LARB_PORT(M4U_L2_P1_MDP_RDMA1),             8, false, 0x0, SLAVE_LARB(2)),
	DEFINE_MNODE(mdp_wrot0_larb2_2,
		MASTER_LARB_PORT(M4U_L2_P2_MDP_WROT0),             9, true,  0x0, SLAVE_LARB(2)),
	DEFINE_MNODE(mdp_wdma0_larb2_3,
		MASTER_LARB_PORT(M4U_L2_P3_MDP_WDMA0),             9, true,  0x0, SLAVE_LARB(2)),
	DEFINE_MNODE(disp_fake0_larb2_4,
		MASTER_LARB_PORT(M4U_L2_P4_DISP_FAKE0),            8, false, 0x0, SLAVE_LARB(2)),
	/* LARB4 */
	DEFINE_MNODE(hw_vdec_mc_ext_larb4_0,
		MASTER_LARB_PORT(M4U_L4_P0_HW_VDEC_MC_EXT),        6, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_ufo_ext_larb4_1,
		MASTER_LARB_PORT(M4U_L4_P1_HW_VDEC_UFO_EXT),       8, true,  0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_pp_ext_larb4_2,
		MASTER_LARB_PORT(M4U_L4_P2_HW_VDEC_PP_EXT),        8, true,  0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_pred_rd_ext_larb4_3,
		MASTER_LARB_PORT(M4U_L4_P3_HW_VDEC_PRED_RD_EXT),   8, true,  0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_pred_wr_ext_larb4_4,
		MASTER_LARB_PORT(M4U_L4_P4_HW_VDEC_PRED_WR_EXT),   7, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_ppwrap_ext_larb4_5,
		MASTER_LARB_PORT(M4U_L4_P5_HW_VDEC_PPWRAP_EXT),    7, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_tile_ext_larb4_6,
		MASTER_LARB_PORT(M4U_L4_P6_HW_VDEC_TILE_EXT),      7, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_vld_ext_larb4_7,
		MASTER_LARB_PORT(M4U_L4_P7_HW_VDEC_VLD_EXT),       7, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_vld2_ext_larb4_8,
		MASTER_LARB_PORT(M4U_L4_P8_HW_VDEC_VLD2_EXT),      7, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_avc_mv_ext_larb4_9,
		MASTER_LARB_PORT(M4U_L4_P9_HW_VDEC_AVC_MV_EXT),    6, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_rg_ctrl_dma_ext_larb4_10,
		MASTER_LARB_PORT(M4U_L4_P10_HW_VDEC_RG_CTRL_DMA_EXT), 7, false, 0x0, SLAVE_LARB(4)),
	DEFINE_MNODE(hw_vdec_ufo_enc_ext_larb4_11,
		MASTER_LARB_PORT(M4U_L4_P11_HW_VDEC_UFO_ENC_EXT),  6, false, 0x0, SLAVE_LARB(4)),
	/* LARB7 */
	DEFINE_MNODE(vnec_rcpu_larb7_0,
		MASTER_LARB_PORT(M4U_L7_P0_VENC_RCPU),             7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_rec_larb7_1,
		MASTER_LARB_PORT(M4U_L7_P1_VENC_REC),              8, true,  0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_bsdma_larb7_2,
		MASTER_LARB_PORT(M4U_L7_P2_VENC_BSDMA),            8, true,  0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_sv_comv_larb7_3,
		MASTER_LARB_PORT(M4U_L7_P3_VENC_SV_COMV),          8, true,  0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_rd_comv_larb7_4,
		MASTER_LARB_PORT(M4U_L7_P4_VENC_RD_COMV),          7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(jpgenc_y_rdma_larb7_5,
		MASTER_LARB_PORT(M4U_L7_P5_JPGENC_Y_RDMA),         7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(jpgenc_c_rdma_larb7_6,
		MASTER_LARB_PORT(M4U_L7_P6_JPGENC_C_RDMA),         7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(jpgenc_q_table_larb7_7,
		MASTER_LARB_PORT(M4U_L7_P7_JPGENC_Q_RDMA),         7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_sub_w_luma_larb7_8,
		MASTER_LARB_PORT(M4U_L7_P8_VENC_SUB_W_LUMA),       8, true,  0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(jpgenc_bsdma_larb7_9,
		MASTER_LARB_PORT(M4U_L7_P9_JPGENC_BSDMA),          8, true,  0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_cur_luma_larb7_10,
		MASTER_LARB_PORT(M4U_L7_P10_VENC_CUR_LUMA),        7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_cur_chroma_larb7_11,
		MASTER_LARB_PORT(M4U_L7_P11_VENC_CUR_CHROMA),      7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_ref_luma_larb7_12,
		MASTER_LARB_PORT(M4U_L7_P12_VENC_REF_LUMA),        7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_ref_chroma_larb7_13,
		MASTER_LARB_PORT(M4U_L7_P13_VENC_REF_CHROMA),      7, false, 0x0, SLAVE_LARB(7)),
	DEFINE_MNODE(venc_sub_r_luma_larb7_14,
		MASTER_LARB_PORT(M4U_L7_P14_VENC_SUB_R_LUMA),      7, false, 0x0, SLAVE_LARB(7)),
	/* LARB13 */
	DEFINE_MNODE(camsv_b_cqi_e1_larb13_0,
		MASTER_LARB_PORT(M4U_L13_P0_CAMSV_B_CQI_E1),       7, false, 0x0, SLAVE_LARB(13)),
	DEFINE_MNODE(camsv_b0_wdma_larb13_1,
		MASTER_LARB_PORT(M4U_L13_P1_CAMSV_B0_WDMA),        8, true,  0x0, SLAVE_LARB(13)),
	DEFINE_MNODE(camsv_b1_wdma_larb13_2,
		MASTER_LARB_PORT(M4U_L13_P2_CAMSV_B1_WDMA),        8, true,  0x0, SLAVE_LARB(13)),
	DEFINE_MNODE(fake_eng_larb13_3,
		MASTER_LARB_PORT(M4U_L13_P3_FAKE_ENG),             7, false, 0x0, SLAVE_LARB(13)),
	DEFINE_MNODE(reserved_larb13_4,
		MASTER_LARB_PORT(M4U_L13_P4_RESERVED),             7, false, 0x0, SLAVE_LARB(13)),
	/* LARB14 */
	DEFINE_MNODE(camsv_cqi_e0_larb14_0,
		MASTER_LARB_PORT(M4U_L14_P0_CAMSV_A_CQI_E1),       7, false, 0x0, SLAVE_LARB(14)),
	DEFINE_MNODE(camsv_a0_wdma_larb14_1,
		MASTER_LARB_PORT(M4U_L14_P1_CAMSV_A0_WDMA),        8, true,  0x0, SLAVE_LARB(14)),
	DEFINE_MNODE(camsv_a1_wdma_larb14_2,
		MASTER_LARB_PORT(M4U_L14_P2_CAMSV_A1_WDMA),        8, true,  0x0, SLAVE_LARB(14)),
	DEFINE_MNODE(camsv_r_0__larb14_3,
		MASTER_LARB_PORT(M4U_L14_P3_RESERVED),             7, false, 0x0, SLAVE_LARB(14)),
	/* LARB16 */
	DEFINE_MNODE(cqi_r1_larb16_0,
		MASTER_LARB_PORT(M4U_L16_P0_CQI_R1_U0),            7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(rawi_r2_larb16_1,
		MASTER_LARB_PORT(M4U_L16_P1_RAWI_R2_U0),           7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(rawi_r3_larb16_2,
		MASTER_LARB_PORT(M4U_L16_P2_RAWI_R3_U0),           7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(rawi_r4_larb16_3,
		MASTER_LARB_PORT(M4U_L16_P3_RAWI_R4_U0),           7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(rawi_r5_larb16_4,
		MASTER_LARB_PORT(M4U_L16_P4_RAWI_R5_U0),           7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(imgo_r1_larb16_5,
		MASTER_LARB_PORT(M4U_L16_P5_IMGO_R1_U0),           8, true,  0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(imgo_r2_larb16_6,
		MASTER_LARB_PORT(M4U_L16_P6_IMGO_R2_U0),           8, true,  0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(fpri_r1_larb16_7,
		MASTER_LARB_PORT(M4U_L16_P7_FPRI_R1_U0),           7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(bpci_r1_larb16_8,
		MASTER_LARB_PORT(M4U_L16_P8_BPCI_R1_U0),           7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(bpci_r4_larb16_9,
		MASTER_LARB_PORT(M4U_L16_P9_BPCI_R4_U0),           7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(lsci_r1_larb16_10,
		MASTER_LARB_PORT(M4U_L16_P10_LSCI_R1_U0),          7, false, 0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(ufeo_r1_larb16_11,
		MASTER_LARB_PORT(M4U_L16_P11_UFEO_R1_U0),          8, true,  0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(ltmso_r1_larb16_12,
		MASTER_LARB_PORT(M4U_L16_P12_LTMSO_R1_U0),         8, true,  0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(drzb2no_r1_larb16_13,
		MASTER_LARB_PORT(M4U_L16_P13_RESERVED),       8, true,  0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(afo_r1_larb16_14,
		MASTER_LARB_PORT(M4U_L16_P14_AFO_R1_U0),           8, true,  0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(aao_r1_larb16_15,
		MASTER_LARB_PORT(M4U_L16_P15_AAO_R1_U0),           8, true,  0x0, SLAVE_LARB(16)),
	DEFINE_MNODE(reserved_larb16_16,
		MASTER_LARB_PORT(M4U_L16_P16_RESERVED),            7, false, 0x0, SLAVE_LARB(16)),
	/* LARB17 */
	DEFINE_MNODE(yuvo_r1_larb17_u0_0,
		MASTER_LARB_PORT(M4U_L17_P0_YUVO_R1_U0),           8, true,  0x0, SLAVE_LARB(17)),
	DEFINE_MNODE(yuvo_r3_larb17_u0_1,
		MASTER_LARB_PORT(M4U_L17_P1_YUVO_R3_U0),           8, true,  0x0, SLAVE_LARB(17)),
	DEFINE_MNODE(yuvo_r2_larb17_u0_2,
		MASTER_LARB_PORT(M4U_L17_P2_YUVO_R2_U0),           8, true,  0x0, SLAVE_LARB(17)),
	DEFINE_MNODE(yuvo_r5_larb17_u0_3,
		MASTER_LARB_PORT(M4U_L17_P3_YUVO_R5_U0),           8, true,  0x0, SLAVE_LARB(17)),
	DEFINE_MNODE(rgbwi_r1_larb17_u0_4,
		MASTER_LARB_PORT(M4U_L17_P4_RGBWI_R1_U0),          7, false, 0x0, SLAVE_LARB(17)),
	DEFINE_MNODE(tcyso_r1_larb17_u0_5,
		MASTER_LARB_PORT(M4U_L17_P5_TCYSO_R1_U0),          8, true,  0x0, SLAVE_LARB(17)),
	DEFINE_MNODE(drzhno_r3_larb17_u0_6,
		MASTER_LARB_PORT(M4U_L17_P6_DRZHNO_R3_U0),         8, true,  0x0, SLAVE_LARB(17)),
	DEFINE_MNODE(reserved_larb17_u0_7,
		MASTER_LARB_PORT(M4U_L17_P7_RESERVED),             7, false, 0x0, SLAVE_LARB(17)),
	/* LARB25 */
	DEFINE_MNODE(mraw0_cq1_m1_larb25_0,
		MASTER_LARB_PORT(M4U_L25_P0_MRAW0_CQ1_M1),         7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(mraw0_imgbo_m1_larb25_1,
		MASTER_LARB_PORT(M4U_L25_P1_MRAW0_IMGBO_M1),       8, true,  0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(reserved_larb25_2,
		MASTER_LARB_PORT(M4U_L25_P2_RESERVED),             7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(reserved_larb25_3,
		MASTER_LARB_PORT(M4U_L25_P3_RESERVED),             8, true,  0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(pdai_a_0_larb25_4,
		MASTER_LARB_PORT(M4U_L25_P4_PDAI_A_0),             7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(pdai_a_1_larb25_5,
		MASTER_LARB_PORT(M4U_L25_P5_PDAI_A_1),             7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(pdai_a_2_larb25_6,
		MASTER_LARB_PORT(M4U_L25_P6_PDAI_A_2),             7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(pdai_a_3_larb25_7,
		MASTER_LARB_PORT(M4U_L25_P7_PDAI_A_3),             7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(pdai_a_4_larb25_8,
		MASTER_LARB_PORT(M4U_L25_P8_PDAI_A_4),             7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(pdai_a_0_larb25_9,
		MASTER_LARB_PORT(M4U_L25_P9_PDAI_A_0),             8, true,  0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(mraw0_imgbo_m1_larb25_10,
		MASTER_LARB_PORT(M4U_L25_P10_MRAW0_IMGBO_M1),      8, true,  0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(reserved_larb25_11,
		MASTER_LARB_PORT(M4U_L25_P11_RESERVED),            8, true,  0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(reserved_larb25_12,
		MASTER_LARB_PORT(M4U_L25_P12_RESERVED),            7, false, 0x0, SLAVE_LARB(25)),
	DEFINE_MNODE(reserved_larb25_13,
		MASTER_LARB_PORT(M4U_L25_P13_RESERVED),            7, false, 0x0, SLAVE_LARB(25)),
	/* LARB29 */
	DEFINE_MNODE(camsv_cqi_e2_larb29_0,
		MASTER_LARB_PORT(M4U_L29_P0_CAMSV_CQI_E2),         7, false, 0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_cqi_e3_larb29_1,
		MASTER_LARB_PORT(M4U_L29_P1_CAMSV_CQI_E3),         7, false, 0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_cqi_e4_larb29_2,
		MASTER_LARB_PORT(M4U_L29_P2_CAMSV_CQI_E4),         7, false, 0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_cqi_e5_larb29_3,
		MASTER_LARB_PORT(M4U_L29_P3_CAMSV_CQI_E5),         7, false, 0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_e2_wdma_larb29_4,
		MASTER_LARB_PORT(M4U_L29_P4_CAMSV_E2_WDMA),        8, true,  0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_e3_wdma_larb29_5,
		MASTER_LARB_PORT(M4U_L29_P5_CAMSV_E3_WDMA),        8, true,  0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_e4_wdma_larb29_6,
		MASTER_LARB_PORT(M4U_L29_P6_CAMSV_E4_WDMA),        8, true,  0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_e5_wdma_larb29_7,
		MASTER_LARB_PORT(M4U_L29_P7_CAMSV_E5_WDMA),        8, true,  0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_r_0_larb29_8,
		MASTER_LARB_PORT(M4U_L29_P8_CAMSV_R_0),            7, false, 0x0, SLAVE_LARB(29)),
	DEFINE_MNODE(camsv_r_1_larb29_9,
		MASTER_LARB_PORT(M4U_L29_P9_CAMSV_R_1),            7, false, 0x0, SLAVE_LARB(29)),
	/* LARB30 */
	DEFINE_MNODE(ccui_0_larb30_0,
		MASTER_LARB_PORT(M4U_L30_P0_CCUI_0),               7, false, 0x0, SLAVE_LARB(30)),
	DEFINE_MNODE(ccuo_0_larb30_1,
		MASTER_LARB_PORT(M4U_L30_P1_CCUO_0),               8, true,  0x0, SLAVE_LARB(30)),
	DEFINE_MNODE(ccui_1_larb30_2,
		MASTER_LARB_PORT(M4U_L30_P2_CCUI_1),               7, false, 0x0, SLAVE_LARB(30)),
	DEFINE_MNODE(ccuo_1_larb30_3,
		MASTER_LARB_PORT(M4U_L30_P3_CCUO_1),               8, true,  0x0, SLAVE_LARB(30)),
	/* LARB33 */
	DEFINE_MNODE(cqi_r1_larb16_u1_0,
		MASTER_LARB_PORT(M4U_L33_P0_CQI_R1_U0),            7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(rawi_r2_larb16_u1_1,
		MASTER_LARB_PORT(M4U_L33_P1_RAWI_R2_U0),           7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(rawi_r3_larb16_u1_2,
		MASTER_LARB_PORT(M4U_L33_P2_RAWI_R3_U0),           7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(rawi_r4_larb16_u1_3,
		MASTER_LARB_PORT(M4U_L33_P3_RAWI_R4_U0),           7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(rawi_r5_larb16_u1_4,
		MASTER_LARB_PORT(M4U_L33_P4_RAWI_R5_U0),           7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(imgo_r1_larb16_u1_5,
		MASTER_LARB_PORT(M4U_L33_P5_IMGO_R1_U0),           8, true,  0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(imgo_r2_larb16_u1_6,
		MASTER_LARB_PORT(M4U_L33_P6_IMGO_R2_U0),           8, true,  0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(fpri_r1_larb16_u1_7,
		MASTER_LARB_PORT(M4U_L33_P7_FPRI_R1_U0),           7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(bpci_r1_larb16_u1_8,
		MASTER_LARB_PORT(M4U_L33_P8_BPCI_R1_U0),           7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(bpci_r4_larb16_u1_9,
		MASTER_LARB_PORT(M4U_L33_P9_BPCI_R4_U0),           7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(lsci_r1_larb16_u1_10,
		MASTER_LARB_PORT(M4U_L33_P10_LSCI_R1_U0),          7, false, 0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(ufeo_r1_larb16_u1_11,
		MASTER_LARB_PORT(M4U_L33_P11_UFEO_R1_U0),          8, true,  0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(ltmso_r1_larb16_u1_12,
		MASTER_LARB_PORT(M4U_L33_P12_LTMSO_R1_U0),         8, true,  0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(drzb2no_r1_larb16_u1_13,
		MASTER_LARB_PORT(M4U_L33_P13_RESERVED),            8, true,  0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(afo_r1_larb16_u1_14,
		MASTER_LARB_PORT(M4U_L33_P14_AFO_R1_U0),           8, true,  0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(aao_r1_larb16_u1_15,
		MASTER_LARB_PORT(M4U_L33_P15_AAO_R1_U0),           8, true,  0x0, SLAVE_LARB(33)),
	DEFINE_MNODE(reserved_larb16_u1_16,
		MASTER_LARB_PORT(M4U_L33_P16_RESERVED),            7, false, 0x0, SLAVE_LARB(33)),
	/* LARB34 */
	DEFINE_MNODE(yuvo_r1_larb17_u1_0,
		MASTER_LARB_PORT(M4U_L34_P0_YUVO_R1_U0),           8, true,  0x0, SLAVE_LARB(34)),
	DEFINE_MNODE(yuvo_r3_larb17_u1_1,
		MASTER_LARB_PORT(M4U_L34_P1_YUVO_R3_U0),           8, true,  0x0, SLAVE_LARB(34)),
	DEFINE_MNODE(yuvo_r2_larb17_u1_2,
		MASTER_LARB_PORT(M4U_L34_P2_YUVO_R2_U0),           8, true,  0x0, SLAVE_LARB(34)),
	DEFINE_MNODE(yuvo_r5_larb17_u1_3,
		MASTER_LARB_PORT(M4U_L34_P3_YUVO_R5_U0),           8, true,  0x0, SLAVE_LARB(34)),
	DEFINE_MNODE(rgbwi_r1_larb17_u1_4,
		MASTER_LARB_PORT(M4U_L34_P4_RGBWI_R1_U0),          7, false, 0x0, SLAVE_LARB(34)),
	DEFINE_MNODE(tcyso_r1_larb17_u1_5,
		MASTER_LARB_PORT(M4U_L34_P5_TCYSO_R1_U0),          8, true,  0x0, SLAVE_LARB(34)),
	DEFINE_MNODE(drzhno_r3_larb17_u1_6,
		MASTER_LARB_PORT(M4U_L34_P6_DRZHNO_R3_U0),         8, true,  0x0, SLAVE_LARB(34)),
	DEFINE_MNODE(reserved_larb17_u1_7,
		MASTER_LARB_PORT(M4U_L34_P7_RESERVED),             7, false, 0x0, SLAVE_LARB(34)),
};
static const char * const comm_muxes_mt6878[] = { "mm" };
static const char * const comm_icc_path_names_mt6878[] = { "icc-bw" };
static const char * const comm_icc_hrt_path_names_mt6878[] = { "icc-hrt-bw" };
static const struct mtk_mmqos_desc mmqos_desc_mt6878 = {
	.nodes = node_descs_mt6878,
	.num_nodes = ARRAY_SIZE(node_descs_mt6878),
	.comm_muxes = comm_muxes_mt6878,
	.comm_icc_path_names = comm_icc_path_names_mt6878,
	.comm_icc_hrt_path_names = comm_icc_hrt_path_names_mt6878,
	.max_ratio = 52,
	.hrt = {
		.hrt_bw = {5566, 0, 0},
		.hrt_total_bw = 22000, /* Use DRAMC API 5500*2(channel)*2(io width)*/
		.md_speech_bw = { 5566, 4941},
		.hrt_ratio = {1000, 860, 880, 880}, /* MD, CAM, DISP, MML */
		.blocking = true,
		.emi_ratio = 700,
	},
	.hrt_LPDDR4 = {
		.hrt_bw = {5566, 0, 0},
		.hrt_total_bw = 17064, /* Use DRAMC API 4266*2(channel)*2(io width)*/
		.md_speech_bw = { 5566, 4941},
		.hrt_ratio = {1000, 860, 880, 880}, /* MD, CAM, DISP, MML */
		.blocking = true,
		.emi_ratio = 800,
	},
	.comm_port_hrt_types = {
		{ HRT_MAX_BWL, HRT_MAX_BWL, HRT_NONE, HRT_NONE, HRT_NONE,
			HRT_NONE, HRT_CAM, HRT_CAM, HRT_DISP },
	},
	.report_bw_larbs = {SLAVE_LARB(22), SLAVE_LARB(28),
			SLAVE_LARB(9), SLAVE_LARB(15),
			SLAVE_LARB(7), SLAVE_LARB(8),
			SLAVE_LARB(5), SLAVE_LARB(4),},
	.report_bw_real_larbs = {SLAVE_LARB(22), SLAVE_LARB(28),
			SLAVE_LARB(9), SLAVE_LARB(15),
			SLAVE_LARB(7), SLAVE_LARB(8),
			SLAVE_LARB(5), SLAVE_LARB(4),},
	.disp_virt_larbs = { SLAVE_LARB(39), SLAVE_LARB(40), SLAVE_LARB(41) },
	.freq_mode = BY_REGULATOR,
};
static const struct of_device_id mtk_mmqos_mt6878_of_ids[] = {
	{
		.compatible = "mediatek,mt6878-mmqos",
		.data = &mmqos_desc_mt6878,
	},
	{}
};
MODULE_DEVICE_TABLE(of, mtk_mmqos_mt6878_of_ids);
static struct platform_driver mtk_mmqos_mt6878_driver = {
	.probe = mtk_mmqos_v2_probe,
	.remove = mtk_mmqos_remove,
	.driver = {
		.name = "mtk-mt6878-mmqos",
		.of_match_table = mtk_mmqos_mt6878_of_ids,
	},
};
module_platform_driver(mtk_mmqos_mt6878_driver);
MODULE_LICENSE("GPL");
