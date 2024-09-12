/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#ifndef MTK_HCP_ISP7sp_H
#define MTK_HCP_ISP7sp_H

#include "mtk-hcp.h"

#if SMVR_DECOUPLE
int isp7sp_release_working_buffer(struct mtk_hcp *hcp_dev, unsigned int mode);
int isp7sp_release_gce_working_buffer(struct mtk_hcp *hcp_dev);
int isp7sp_allocate_working_buffer(struct mtk_hcp *hcp_dev, unsigned int mode, unsigned int gmb_en);
//int isp7s_allocate_working_buffer2(struct mtk_hcp *hcp_dev, unsigned int mode);
int isp7sp_get_init_info(struct img_init_info *info);
void *isp7sp_get_gce_virt(unsigned int mode);
void *isp7sp_get_gce_token_virt(unsigned int mode);
void *isp7sp_get_hwid_virt(unsigned int mode);
int isp7sp_partial_flush(struct mtk_hcp *hcp_dev, struct flush_buf_info *b_info);
#else
int isp7sp_release_working_buffer(struct mtk_hcp *hcp_dev);
int isp7sp_allocate_working_buffer(struct mtk_hcp *hcp_dev, unsigned int mode);
int isp7sp_get_init_info(struct img_init_info *info);
void *isp7sp_get_gce_virt(void);
void *isp7sp_get_hwid_virt(void);
int isp7sp_partial_flush(struct mtk_hcp *hcp_dev, struct flush_buf_info *b_info);
#endif

extern struct mtk_hcp_data isp7sp_hcp_data;

#endif /* _MTK_HCP_ISP7sp_H */