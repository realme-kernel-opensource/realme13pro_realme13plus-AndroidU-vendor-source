/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2021 MediaTek Inc.
 */

#ifndef _MTK_DRM_HELPER_H_
#define _MTK_DRM_HELPER_H_

enum DISP_HELPER_STAGE {
	DISP_HELPER_STAGE_NORMAL = 0,
	DISP_HELPER_STAGE_BRING_UP
};

enum MTK_DRM_HELPER_OPT {
	MTK_DRM_OPT_STAGE = 0,
	MTK_DRM_OPT_USE_CMDQ,
	MTK_DRM_OPT_USE_M4U,
	MTK_DRM_OPT_MMQOS_SUPPORT,
	MTK_DRM_OPT_MMDVFS_SUPPORT,
	/* Begin: lowpower option*/
	MTK_DRM_OPT_SODI_SUPPORT,
	MTK_DRM_OPT_IDLE_MGR,
	MTK_DRM_OPT_IDLEMGR_SWTCH_DECOUPLE,
	MTK_DRM_OPT_IDLEMGR_BY_REPAINT,
	MTK_DRM_OPT_IDLEMGR_BY_WB,
	MTK_DRM_OPT_IDLEMGR_ENTER_ULPS,
	MTK_DRM_OPT_IDLEMGR_KEEP_LP11,
	MTK_DRM_OPT_DYNAMIC_RDMA_GOLDEN_SETTING,
	MTK_DRM_OPT_IDLEMGR_DISABLE_ROUTINE_IRQ,
	MTK_DRM_OPT_IDLEMGR_ASYNC,
	MTK_DRM_OPT_MET_LOG, /* for met */
	/* End: lowpower option */
	MTK_DRM_OPT_USE_PQ,
	MTK_DRM_OPT_ESD_CHECK_RECOVERY,
	MTK_DRM_OPT_ESD_CHECK_SWITCH,
	MTK_DRM_OPT_PRESENT_FENCE,
	MTK_DRM_OPT_RDMA_UNDERFLOW_AEE,
	MTK_DRM_OPT_DSI_UNDERRUN_AEE,
	MTK_DRM_OPT_ODDMR_OD_AEE,
	MTK_DRM_OPT_ODDMR_DMR_AEE,
	MTK_DRM_OPT_HRT,
	MTK_DRM_OPT_HRT_MODE,
	MTK_DRM_OPT_DELAYED_TRIGGER,

	MTK_DRM_OPT_OVL_EXT_LAYER,
	MTK_DRM_OPT_AOD,
	MTK_DRM_OPT_RPO,
	MTK_DRM_OPT_DUAL_PIPE,
	MTK_DRM_OPT_DC_BY_HRT,
	MTK_DRM_OPT_OVL_WCG,
	/* OVL SBCH */
	MTK_DRM_OPT_OVL_SBCH,
	MTK_DRM_OPT_COMMIT_NO_WAIT_VBLANK,
	MTK_DRM_OPT_MET,
	MTK_DRM_OPT_REG_PARSER_RAW_DUMP,
	MTK_DRM_OPT_VP_PQ,
	MTK_DRM_OPT_GAME_PQ,
	MTK_DRM_OPT_MMPATH,
	MTK_DRM_OPT_HBM,
	MTK_DRM_OPT_LAYER_REC,
	MTK_DRM_OPT_CLEAR_LAYER,
	MTK_DRM_OPT_VDS_PATH_SWITCH,
	/*ARR4*/
	MTK_DRM_OPT_LFR,
	MTK_DRM_OPT_SF_PF,
	/*PQ*/
	MTK_DRM_OPT_PQ_34_COLOR_MATRIX,
	MTK_DRM_OPT_DYN_MIPI_CHANGE,
	MTK_DRM_OPT_PRIM_DUAL_PIPE,
	/*Msync2.0*/
	MTK_DRM_OPT_MSYNC2_0,
	MTK_DRM_OPT_MSYNC2_0_WAIT_EPT,
	/* MML primary display */
	MTK_DRM_OPT_MML_PRIMARY,
	MTK_DRM_OPT_MML_SUPPORT_CMD_MODE,
	MTK_DRM_OPT_MML_PQ,
	MTK_DRM_OPT_MML_IR,
	MTK_DRM_OPT_MML_TABLET_EXT,
	MTK_DRM_OPT_DUAL_TE,
	/* Resolution switch */
	MTK_DRM_OPT_RES_SWITCH,
	MTK_DRM_OPT_RES_SWITCH_ON_AP,
	MTK_DRM_OPT_PREFETCH_TE,
	MTK_DRM_OPT_VIDLE_APSRC_OFF,
	MTK_DRM_OPT_VIDLE_DSI_PLL_OFF,
	MTK_DRM_OPT_CHECK_TRIGGER_MERGE,
	/* Virtual Display via DISP HW */
	MTK_DRM_OPT_VIRTUAL_DISP,
	MTK_DRM_OPT_OVL_BW_MONITOR,
	MTK_DRM_OPT_GPU_CACHE,
	MTK_DRM_OPT_SPHRT,
	MTK_DRM_OPT_SDPA_OVL_SWITCH,
	/*Tile overhead*/
	MTK_DRM_OPT_TILE_OVERHEAD,
	MTK_DRM_OPT_HRT_BY_LARB,
	MTK_DRM_OPT_LAYERING_RULE_BY_LARB,
	MTK_DRM_OPT_VIDLE_TOP_EN,
	MTK_DRM_OPT_VIDLE_MTCMOS_DT_EN,
	MTK_DRM_OPT_VIDLE_MMINFRA_DT_EN,
	MTK_DRM_OPT_VIDLE_DVFS_DT_EN,
	MTK_DRM_OPT_VIDLE_QOS_DT_EN,
	MTK_DRM_OPT_VIDLE_GCE_TS_EN,
	MTK_DRM_OPT_VIDLE_DECOUPLE_MODE,
	MTK_DRM_OPT_VIDLE_VDO_PANEL,
	MTK_DRM_OPT_VIDLE_HOME_SCREEN_IDLE,
	MTK_DRM_OPT_DPC_PRE_TE_EN,
	MTK_DRM_OPT_OVLSYS_CASCADE,
	MTK_DRM_OPT_PARTIAL_UPDATE,
	MTK_DRM_OPT_SLC_ALL_CACHE,
	MTK_DRM_OPT_VBLANK_CONFIG_REC,
	MTK_DRM_OPT_OVL_WCG_BY_COLOR_MODE,
	MTK_DRM_OPT_PER_LARB_PORT_PEAK,
	MTK_DRM_OPT_NUM
};

struct mtk_drm_helper {
	enum MTK_DRM_HELPER_OPT opt;
	unsigned int val;
	const char *desc;
};

enum MTK_DRM_HELPER_STAGE {
	MTK_DRM_HELPER_STAGE_EARLY_PORTING,
	MTK_DRM_HELPER_STAGE_BRING_UP,
	MTK_DRM_HELPER_STAGE_NORMAL
};

enum DISP_HELPER_STAGE disp_helper_get_stage(void);
void disp_helper_set_stage(enum DISP_HELPER_STAGE stage);

int mtk_drm_helper_get_opt(struct mtk_drm_helper *helper_opt,
			   enum MTK_DRM_HELPER_OPT option);
int mtk_drm_helper_set_opt(struct mtk_drm_helper *helper_opt,
			   enum MTK_DRM_HELPER_OPT option, int value);
int mtk_drm_helper_set_opt_by_name(struct mtk_drm_helper *helper_opt,
				   const char *name, int value);
int mtk_drm_helper_get_opt_list(struct mtk_drm_helper *helper_opt,
				char *stringbuf, int buf_len);
void mtk_drm_helper_init(struct device *dev,
			 struct mtk_drm_helper **helper_opt);

enum MTK_DRM_HELPER_OPT
mtk_drm_helper_name_to_opt(struct mtk_drm_helper *helper_opt, const char *name);

#endif /* _MTK_DRM_HELPER_H_ */
