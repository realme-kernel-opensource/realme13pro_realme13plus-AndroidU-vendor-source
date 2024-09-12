/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#ifndef __MTKCAM_DEFS_H
#define __MTKCAM_DEFS_H

/**
 *  This definition is for the following version:
 *
 *  MTK_CAM_IPI_VERSION_MAJOR: 0
 *  MTK_CAM_IPI_VERSION_MINOR: 1
 */

#include <linux/types.h>
#include <camsys/common/mtk_cam-fmt.h>

/*
 * Note:
 *	Following definitions are used in IPI-messaging.
 *	Values are used in software control flow only and cannot be applied to
 *	hw registers directly.
 */

/* camsys hw pipelines */
enum mtkcam_pipe_subdev {
	MTKCAM_SUBDEV_RAW_START = 0,
	MTKCAM_SUBDEV_RAW_0	= MTKCAM_SUBDEV_RAW_START,
	MTKCAM_SUBDEV_RAW_1,
	MTKCAM_SUBDEV_RAW_2,
	MTKCAM_SUBDEV_RAW_END,
	MTKCAM_SUBDEV_CAMSV_START = MTKCAM_SUBDEV_RAW_END,
	MTKCAM_SUBDEV_CAMSV_0 = MTKCAM_SUBDEV_CAMSV_START,
	MTKCAM_SUBDEV_CAMSV_1,
	MTKCAM_SUBDEV_CAMSV_2,
	MTKCAM_SUBDEV_CAMSV_3,
	MTKCAM_SUBDEV_CAMSV_4,
	MTKCAM_SUBDEV_CAMSV_5,
	MTKCAM_SUBDEV_CAMSV_6,
	MTKCAM_SUBDEV_CAMSV_7,
	MTKCAM_SUBDEV_CAMSV_8,
	MTKCAM_SUBDEV_CAMSV_9,
	MTKCAM_SUBDEV_CAMSV_10,
	MTKCAM_SUBDEV_CAMSV_11,
	MTKCAM_SUBDEV_CAMSV_12,
	MTKCAM_SUBDEV_CAMSV_13,
	MTKCAM_SUBDEV_CAMSV_14,
	MTKCAM_SUBDEV_CAMSV_15,
	MTKCAM_SUBDEV_CAMSV_END,
	MTKCAM_SUBDEV_MRAW_START = MTKCAM_SUBDEV_CAMSV_END,
	MTKCAM_SUBDEV_MRAW_0 = MTKCAM_SUBDEV_MRAW_START,
	MTKCAM_SUBDEV_MRAW_1,
	MTKCAM_SUBDEV_MRAW_2,
	MTKCAM_SUBDEV_MRAW_3,
	MTKCAM_SUBDEV_MRAW_END,
	MTKCAM_SUBDEV_MAX = MTKCAM_SUBDEV_MRAW_END,
};

#define MTKCAM_SUBDEV_RAW_NUM (MTKCAM_SUBDEV_RAW_END - MTKCAM_SUBDEV_RAW_START)
#define MTKCAM_SUBDEV_CAMSV_NUM (MTKCAM_SUBDEV_CAMSV_END - MTKCAM_SUBDEV_CAMSV_START)
#define MTKCAM_SUBDEV_MRAW_NUM (MTKCAM_SUBDEV_MRAW_END - MTKCAM_SUBDEV_MRAW_START)

#define MTKCAM_MSTREAM_MAX 2

#define MTKCAM_SUBDEV_RAW_MASK (1 << MTKCAM_SUBDEV_RAW_0| \
				1 << MTKCAM_SUBDEV_RAW_1| \
				1 << MTKCAM_SUBDEV_RAW_2)


static inline int is_raw_subdev(unsigned char subdev_id)
{
	return (subdev_id == MTKCAM_SUBDEV_RAW_0 ||
		subdev_id == MTKCAM_SUBDEV_RAW_1 ||
		subdev_id == MTKCAM_SUBDEV_RAW_2);
}

static inline int is_camsv_subdev(unsigned char subdev_id)
{
	return (subdev_id >= MTKCAM_SUBDEV_CAMSV_START &&
		subdev_id < MTKCAM_SUBDEV_CAMSV_END);
}

static inline int is_mraw_subdev(unsigned char subdev_id)
{
	return (subdev_id >= MTKCAM_SUBDEV_MRAW_START &&
		subdev_id < MTKCAM_SUBDEV_MRAW_END);
}

enum mtkcam_pipe_dev {
	MTKCAM_PIPE_RAW_A	= 0,
	MTKCAM_PIPE_RAW_B,
	MTKCAM_PIPE_RAW_C,
	MTKCAM_PIPE_CAMSV_0,
	MTKCAM_PIPE_CAMSV_1,
	MTKCAM_PIPE_CAMSV_2,
	MTKCAM_PIPE_CAMSV_3,
	MTKCAM_PIPE_CAMSV_4,
	MTKCAM_PIPE_CAMSV_5,
	MTKCAM_PIPE_CAMSV_6,
	MTKCAM_PIPE_CAMSV_7,
	MTKCAM_PIPE_CAMSV_8,
	MTKCAM_PIPE_CAMSV_9,
	MTKCAM_PIPE_CAMSV_10,
	MTKCAM_PIPE_CAMSV_11,
	MTKCAM_PIPE_CAMSV_12,
	MTKCAM_PIPE_CAMSV_13,
	MTKCAM_PIPE_CAMSV_14,
	MTKCAM_PIPE_CAMSV_15,
	MTKCAM_PIPE_MRAW_0,
	MTKCAM_PIPE_MRAW_1,
	MTKCAM_PIPE_MRAW_2,
	MTKCAM_PIPE_MRAW_3,
	MTKCAM_PIPE_MAX
};

enum mtkcam_ipi_video_id {
	MTKCAM_IPI_RAW_ID_UNKNOWN	= 0,
	MTKCAM_IPI_RAW_RAWI_2,		/* RAWI_R2 */
	MTKCAM_IPI_RAW_RAWI_3,		/* RAWI_R3 */
	MTKCAM_IPI_RAW_RAWI_5,		/* RAWI_R5 */
	MTKCAM_IPI_RAW_IPUI,		/* ADLRD */
	MTKCAM_IPI_RAW_IMGO,		/* IMGO_R1 */
	/* no need pure raw */
	MTKCAM_IPI_RAW_YUVO_1,		/* YUVO_R1 */
	MTKCAM_IPI_RAW_YUVO_2,		/* YUVO_R2 */
	MTKCAM_IPI_RAW_YUVO_3,		/* YUVO_R3 */
	MTKCAM_IPI_RAW_YUVO_4,		/* YUVO_R4 */
	MTKCAM_IPI_RAW_YUVO_5,		/* YUVO_R5 */
	MTKCAM_IPI_RAW_RZH1N2TO_2,	/* RZH1N2TO_R2 */
	MTKCAM_IPI_RAW_DRZS4NO_1,	/* DRZS4NO_R1 */
	MTKCAM_IPI_RAW_DRZS4NO_3,	/* DRZS4NO_R3 */
	MTKCAM_IPI_RAW_RZH1N2TO_3,	/* RZH1N2TO_R3 */
	MTKCAM_IPI_RAW_RZH1N2TO_1,	/* RZH1N2TO_R1 */
	MTKCAM_IPI_RAW_DRZB2NO_1,	/* DRZB2NO_R1 */
	MTKCAM_IPI_RAW_IPUO,		/* IPUO_R1 */
	MTKCAM_IPI_RAW_META_STATS_CFG,	/* All settings */
	MTKCAM_IPI_RAW_META_STATS_0,	/* statistics */

	/*
	 * MTKCAM_IPI_RAW_META_STATS_1 is for AFO only, the buffer can be
	 * dequeued once we got the  dma done.
	 */
	MTKCAM_IPI_RAW_META_STATS_1,

	/* following is for RGBW's w path */
	MTKCAM_IPI_RAW_IMGO_W,		/* IMGO_R1 */
	MTKCAM_IPI_RAW_RAWI_2_W,	/* RAWI_R2 */
	MTKCAM_IPI_RAW_RAWI_3_W,	/* RAWI_R3 */
	MTKCAM_IPI_RAW_RAWI_5_W,	/* RAWI_R5 */

	MTKCAM_IPI_RAW_ID_MAX,

	MTKCAM_IPI_CAMSV_MAIN_OUT = MTKCAM_IPI_RAW_ID_MAX,	/* imgo */

	MTKCAM_IPI_CAMSV_ID_MAX,

	MTKCAM_IPI_MRAW_ID_START = MTKCAM_IPI_CAMSV_ID_MAX,
	MTKCAM_IPI_MRAW_META_STATS_CFG = MTKCAM_IPI_MRAW_ID_START,
	MTKCAM_IPI_MRAW_META_STATS_0,
	MTKCAM_IPI_MRAW_ID_MAX,
};

/* Supported bayer pixel order: */
enum mtkcam_ipi_bayer_pxl_id {
	MTKCAM_IPI_BAYER_PXL_ID_B		= 0,
	MTKCAM_IPI_BAYER_PXL_ID_GB		= 1,
	MTKCAM_IPI_BAYER_PXL_ID_GR		= 2,
	MTKCAM_IPI_BAYER_PXL_ID_R		= 3,
	MTKCAM_IPI_BAYER_PXL_ID_UNKNOWN		= 4,
};

/* special input patterns */
enum mtkcam_ipi_sensor_pattern {
	MTKCAM_IPI_SENSOR_PATTERN_NORMAL		= 0,
	MTKCAM_IPI_SENSOR_PATTERN_DUAL_PIX		= 1,
	MTKCAM_IPI_SENSOR_PATTERN_QUADCODE		= 2, /* FIXME: ?? */
	MTKCAM_IPI_SENSOR_PATTERN_4CELL			= 3,
	MTKCAM_IPI_SENSOR_PATTERN_MONO			= 4,
	MTKCAM_IPI_SENSOR_PATTERN_IVHDR			= 5,
	MTKCAM_IPI_SENSOR_PATTERN_ZVHDR			= 6,
	MTKCAM_IPI_SENSOR_PATTERN_4CELL_IVHDR		= 7,
	MTKCAM_IPI_SENSOR_PATTERN_4CELL_ZVHDR		= 8,
	MTKCAM_IPI_SENSOR_PATTERN_DUAL_PIX_IVHDR	= 9,
	MTKCAM_IPI_SENSOR_PATTERN_DUAL_PIX_ZVHDR	= 10,
	/* FIXME: can be ignored? */
	MTKCAM_IPI_SENSOR_PATTERN_YUV			= 11,
	 /* FIXME: what's 'normal' pd? */
	MTKCAM_IPI_SENSOR_PATTERN_NORMAL_PD		= 12,
};

enum mtkcam_ipi_raw_path_control {
	MTKCAM_IPI_IMGO_UNPROCESSED		= 0,
	MTKCAM_IPI_IMGO_AFTER_BPC,
	MTKCAM_IPI_IMGO_AFTER_FRZ,
	MTKCAM_IPI_IMGO_AFTER_FUS,
	MTKCAM_IPI_IMGO_AFTER_DGN,
	MTKCAM_IPI_IMGO_AFTER_LSC,
	MTKCAM_IPI_IMGO_AFTER_HLR,
	MTKCAM_IPI_IMGO_AFTER_LTM,
	MTKCAM_IPI_IMGO_FULLY_PROCESSED = MTKCAM_IPI_IMGO_AFTER_LTM,
};

enum mtkcam_ipi_ADL_path_control {
	MTKCAM_IPI_ADL_AFTER_SEP_R1,
	MTKCAM_IPI_ADL_AFTER_BPC,
	MTKCAM_IPI_ADL_AFTER_LTM,
};

enum mtkcam_ipi_sw_feature_control {
	MTKCAM_IPI_SW_FEATURE_NORMAL				= 0,
	MTKCAM_IPI_SW_FEATURE_VHDR				= 1,
};

enum mtkcam_ipi_frame_order {
	MTKCAM_IPI_ORDER_BAYER_FIRST	= 0, /* bayer-w */
	MTKCAM_IPI_ORDER_W_FIRST	= 1, /* w-bayer */
};

enum mtkcam_ipi_exp_order {
	MTKCAM_IPI_ORDER_SE_NE		= 0,
	MTKCAM_IPI_ORDER_NE_SE		= 1,
	MTKCAM_IPI_ORDER_NE_ME_SE	= 2,
};

enum mtkcam_ipi_flow {
	MTKCAM_IPI_FLOW_NORMAL		= 0,
	MTKCAM_IPI_FLOW_STAGGER		= 1,
	MTKCAM_IPI_FLOW_MSTREAM		= 2,
	MTKCAM_IPI_FLOW_RGBW		= 3,
	MTKCAM_IPI_FLOW_ADL		= 4,
};

enum mtkcam_ipi_flow_variant {
	MTKCAM_IPI_FLOW_OTF,		/* 1. all direct-link. 2. DCIF + 1-exp direct-link */
	MTKCAM_IPI_FLOW_DC,			/* w.o. direct-link, DCIF */
	MTKCAM_IPI_FLOW_OFFLINE,	/* aka, m2m */
	MTKCAM_IPI_FLOW_ADLDC,		/* APU direct-couple */
};

enum mtkcam_ipi_tag_order {
	MTKCAM_IPI_ORDER_FIRST_TAG		= 0,
	MTKCAM_IPI_ORDER_NORMAL_TAG		= 1,
	MTKCAM_IPI_ORDER_LAST_TAG		= 2,
};

/*
 * bit[15:12]: flow
 * bit[11: 8]: variant
 * bit[ 7: 0]: unique id
 */
#define _HWPATH(id, flow, variant)		\
	(MTKCAM_IPI_FLOW_ ## flow << 12 | MTKCAM_IPI_FLOW_ ## variant << 8 | id)

#define HWPATH_ID(hw_path)		(hw_path & 0xFF)
#define HWPATH_VARIANT(hw_path)		((hw_path >> 8) & 0xF)
#define HWPATH_FLOW(hw_path)		((hw_path >> 12) & 0xF)

#define MTKCAM_IPI_HW_PATH_ON_THE_FLY		_HWPATH(0, NORMAL, OTF)
#define MTKCAM_IPI_HW_PATH_DC			_HWPATH(1, NORMAL, DC)
#define MTKCAM_IPI_HW_PATH_OFFLINE		_HWPATH(2, NORMAL, OFFLINE)
#define MTKCAM_IPI_HW_PATH_MSTREAM		_HWPATH(3, MSTREAM, OTF)
#define MTKCAM_IPI_HW_PATH_DC_MSTREAM		_HWPATH(4, MSTREAM, DC)
#define MTKCAM_IPI_HW_PATH_STAGGER		_HWPATH(5, STAGGER, OTF)
#define MTKCAM_IPI_HW_PATH_DC_STAGGER		_HWPATH(6, STAGGER, DC)
#define MTKCAM_IPI_HW_PATH_OFFLINE_STAGGER	_HWPATH(7, STAGGER, OFFLINE)
#define MTKCAM_IPI_HW_PATH_OTF_RGBW		_HWPATH(8, RGBW, OTF)
#define MTKCAM_IPI_HW_PATH_DC_RGBW		_HWPATH(9, RGBW, DC)
#define MTKCAM_IPI_HW_PATH_OFFLINE_RGBW		_HWPATH(10, RGBW, OFFLINE)
#define MTKCAM_IPI_HW_PATH_OFFLINE_ADL		_HWPATH(11, ADL, OFFLINE)
#define MTKCAM_IPI_HW_PATH_DC_ADL		_HWPATH(12, ADL, ADLDC)
#define MTKCAM_IPI_HW_PATH_OTF_RGBW_DOL		_HWPATH(13, RGBW, OTF)
#define MTKCAM_IPI_HW_PATH_OTF_STAGGER_LN_INTL	_HWPATH(14, STAGGER, OTF)

enum {
	BIN_AUTO	= 0,
	BIN_OFF		= BIN_AUTO,
	BIN_ON		= (1 << 0),
	CBN_2X2_ON	= (1 << 4),
	CBN_3X3_ON	= (1 << 5),
	CBN_4X4_ON	= (1 << 6),
	QBND_ON		= (1 << 8)
};

#endif /* __MTKCAM_DEFS_H */
