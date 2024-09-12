/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2020 MediaTek Inc.
 */

#ifndef PD_DPM_PDO_SELECT_H
#define PD_DPM_PDO_SELECT_H

#include "tcpci.h"

struct dpm_rdo_info_t {
	uint8_t pos;
	uint8_t type;
	bool mismatch;

	int vmin;
	int vmax;

	union {
		uint32_t max_uw;
		uint32_t max_ma;
	};

	union {
		uint32_t oper_uw;
		uint32_t oper_ma;
	};
};

#define DPM_PDO_TYPE_FIXED	TCPM_POWER_CAP_VAL_TYPE_FIXED
#define DPM_PDO_TYPE_VAR	TCPM_POWER_CAP_VAL_TYPE_VARIABLE
#define DPM_PDO_TYPE_BAT	TCPM_POWER_CAP_VAL_TYPE_BATTERY
#define DPM_PDO_TYPE_APDO	TCPM_POWER_CAP_VAL_TYPE_AUGMENT

#define DPM_APDO_TYPE_PPS	(TCPM_POWER_CAP_APDO_TYPE_PPS)
#define DPM_APDO_TYPE_PPS_CF	(TCPM_POWER_CAP_APDO_TYPE_PPS_CF)

extern void dpm_extract_pdo_info(
			uint32_t pdo, struct dpm_pdo_info_t *info);

extern bool dpm_find_match_req_info(struct pd_port *pd_port,
	struct dpm_rdo_info_t *req_info,
	struct dpm_pdo_info_t *sink, int cnt, uint32_t *src_pdos,
	int max_uw, uint32_t select_rule);

#endif	/* PD_DPM_PDO_SELECT_H */
