/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2023 MediaTek Inc.
 */

#ifndef __MDP_ENGINE_H__
#define __MDP_ENGINE_H__

enum CMDQ_ENG_ENUM {
	/* MDP */
	CMDQ_ENG_MDP_RDMA0,	/* 0 */
	CMDQ_ENG_MDP_RDMA2,
	CMDQ_ENG_MDP_DMA0_SEL,
	CMDQ_ENG_MDP_DLI0_SEL,
	CMDQ_ENG_MDP_HDR0,
	CMDQ_ENG_MDP_AAL0,
	CMDQ_ENG_MDP_PQ_AAL0_SEL,
	CMDQ_ENG_MDP_RSZ0,
	CMDQ_ENG_MDP_BIRSZ0,
	CMDQ_ENG_MDP_TDSHP0,
	CMDQ_ENG_MDP_COLOR0,
	CMDQ_ENG_MDP_DLO0_SOUT,
	CMDQ_ENG_MDP_WROT0,
	CMDQ_ENG_MAX = CMDQ_MAX_ENGINE_COUNT	/* ALWAYS keep at the end */
};

#define MDP_ENG_LARB2 ((1LL << CMDQ_ENG_MDP_RDMA0) | \
		       (1LL << CMDQ_ENG_MDP_RDMA2) | \
		       (1LL << CMDQ_ENG_MDP_WROT0))

#define CMDQ_ENG_MDP_GROUP_BITS	((1LL << CMDQ_ENG_MDP_RDMA0)	| \
				 (1LL << CMDQ_ENG_MDP_RDMA2)	| \
				 (1LL << CMDQ_ENG_MDP_HDR0)	| \
				 (1LL << CMDQ_ENG_MDP_AAL0)	| \
				 (1LL << CMDQ_ENG_MDP_RSZ0)	| \
				 (1LL << CMDQ_ENG_MDP_BIRSZ0)	| \
				 (1LL << CMDQ_ENG_MDP_TDSHP0)	| \
				 (1LL << CMDQ_ENG_MDP_COLOR0)	| \
				 (1LL << CMDQ_ENG_MDP_WROT0))

#define CMDQ_ENG_SUPPORT_READBACK_GROUP_BITS ((1LL << CMDQ_ENG_MDP_HDR0) | \
				 (1LL << CMDQ_ENG_MDP_AAL0))

#define CMDQ_ENG_MDP_GROUP_FLAG(flag)   ((flag) & (CMDQ_ENG_MDP_GROUP_BITS))

#define CMDQ_FOREACH_GROUP(ACTION_struct)\
	ACTION_struct(CMDQ_GROUP_MDP, MDP)

#define MDP_GENERATE_ENUM(_enum, _string) _enum,

enum CMDQ_GROUP_ENUM {
	CMDQ_FOREACH_GROUP(MDP_GENERATE_ENUM)
	CMDQ_MAX_GROUP_COUNT,	/* ALWAYS keep at the end */
};

enum MDP_ENG_BASE {
	ENGBASE_MMSYS_CONFIG = 0,
	ENGBASE_MDP_RDMA0,
	ENGBASE_MDP_RDMA2,
	ENGBASE_MDP_HDR0,
	ENGBASE_MDP_AAL0,
	ENGBASE_MDP_RSZ0,
	ENGBASE_MDP_BIRSZ0,
	ENGBASE_MDP_TDSHP0,
	ENGBASE_MDP_COLOR0,
	ENGBASE_MDP_WROT0,
	ENGBASE_MMSYS_MUTEX,
	ENGBASE_COUNT
};
#endif				/* __MDP_ENGINE_H__ */