/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2019 MediaTek Inc.
 */

#ifndef __GED_EB_H__
#define __GED_EB_H__

#include <linux/types.h>
#include "ged_type.h"
#include "ged_dvfs.h"

/**************************************************
 * GPU FAST DVFS Log Setting
 **************************************************/
#define GED_FAST_DVFS_TAG "[GPU/FDVFS]"
#define GPUFDVFS_LOGE(fmt, args...) \
	pr_info(GED_FAST_DVFS_TAG"[ERROR]@%s: "fmt"\n", __func__, ##args)
#define GPUFDVFS_LOGW(fmt, args...) \
	pr_debug(GED_FAST_DVFS_TAG"[WARN]@%s: "fmt"\n", __func__, ##args)
#define GPUFDVFS_LOGI(fmt, args...) \
	pr_info(GED_FAST_DVFS_TAG"[INFO]@%s: "fmt"\n", __func__, ##args)
#define GPUFDVFS_LOGD(fmt, args...) \
	pr_debug(GED_FAST_DVFS_TAG"[DEBUG]@%s: "fmt"\n", __func__, ##args)


/**************************************************
 * GPU FAST DVFS SYSRAM Setting
 **************************************************/
#define FASTDVFS_COUNTER_FIRST_ENTRY 48   // use to offset array
#define FASTDVFS_FEEDBACK_INFO_FIRST_ENTRY 88  // use to offset array

#define SYSRAM_LOG_SIZE sizeof(int)

// FDVFS SYSRAM space is allocated after QOSc & SLC, 48~77
enum gpu_fastdvfs_counter {
	FASTDVFS_COUNTER_CURRENT_FREQUENCY = FASTDVFS_COUNTER_FIRST_ENTRY,
	FASTDVFS_COUNTER_PREDICTED_FREQUENCY,
	FASTDVFS_COUNTER_FINISHED_WORKLOAD,
	FASTDVFS_COUNTER_PREDICTED_WORKLOAD,
	FASTDVFS_COUNTER_FRAGMENT_LOADING,
	FASTDVFS_COUNTER_KERNEL_FRAME_DONE_INTERVAL,
	FASTDVFS_COUNTER_EB_FRAME_DONE_INTERVAL,
	FASTDVFS_COUNTER_TARGET_TIME,
	FASTDVFS_COUNTER_FRAME_BOUNDARY,
	FASTDVFS_COUNTER_LEFT_WL,
	FASTDVFS_COUNTER_ELAPSED_TIME,
	FASTDVFS_COUNTER_LEFT_TIME,
	FASTDVFS_COUNTER_FRAME_END_HINT_COUNT,
	FASTDVFS_COUNTER_UNDER_HINT_WL,
	FASTDVFS_COUNTER_UNDER_HINT_CNT,
	FASTDVFS_COUNTER_JS0_DELTA,
	FASTDVFS_COUNTER_COMMIT_PROFILE,
	FASTDVFS_COUNTER_DCS,
	FASTDVFS_COUNTER_LAST_COMMIT_IDX,
	FASTDVFS_COUNTER_LAST_COMMIT_TOP_IDX,
	FASTDVFS_GPU_EB_DESIRE_FREQ_ID,
	FASTDVFS_GPU_EB_DESIRE_POLICY_STATE,
	FASTDVFS_GPU_EB_CUR_LOADING,
	FASTDVFS_GPU_EB_CUR_VIRTUAL_FREQ,
	FASTDVFS_GPU_EB_CUR_OPPIDX,
	FASTDVFS_GPU_EB_MARGIN_HIGH,
	FASTDVFS_GPU_EB_MARGIN_LOW,
	FASTDVFS_GPU_EB_MARGIN,
	FASTDVFS_GPU_EB_CUR_POLICY_STATE,
	FASTDVFS_GPU_EB_GED_MIN_OPPIDX,
	FASTDVFS_GPU_PWR_HINT,

	NR_FASTDVFS_COUNTER,
};

enum gpu_fastdvfs_share_info {
	FASTDVFS_FEEDBACK_INFO_UPDATED = FASTDVFS_FEEDBACK_INFO_FIRST_ENTRY,
	FASTDVFS_FEEDBACK_INFO_GPU_TIME,
	FASTDVFS_FEEDBACK_INFO_GPU_UTILS,
	FASTDVFS_FEEDBACK_INFO_CURR_FPS,
	FASTDVFS_ENABLE_FRAME_BASE_DVFS,
	FASTDVFS_SET_TARGET_FRAME_TIME,
	FASTDVFS_COMMIT_PLATFORM_FREQ_IDX,
	FASTDVFS_COMMIT_VIRTUAL_FREQUENCY,
	FASTDVFS_COMMIT_TYPE,
	FASTDVFS_SET_TARGET_MARGIN,
	FASTDVFS_TA_3D_COEF,

	MAX_FASTDVFS_SHARE_INFO,
};

/* 6989 0x117800~0x117C00 */
#define FASTDVFS_POWERMODEL_SYSRAM_BASE 0x117800U

#define SYSRAM_GPU_CURR_FREQ                         \
(                                                    \
(FASTDVFS_COUNTER_CURRENT_FREQUENCY*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_PRED_FREQ                           \
(                                                      \
(FASTDVFS_COUNTER_PREDICTED_FREQUENCY*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_FINISHED_WORKLOAD                 \
(                                                    \
(FASTDVFS_COUNTER_FINISHED_WORKLOAD*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_PRED_WORKLOAD                      \
(                                                     \
(FASTDVFS_COUNTER_PREDICTED_WORKLOAD*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_FRAGMENT_LOADING                 \
(                                                   \
(FASTDVFS_COUNTER_FRAGMENT_LOADING*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_KERNEL_FRAME_DONE_INTERVAL                 \
(                                                             \
(FASTDVFS_COUNTER_KERNEL_FRAME_DONE_INTERVAL*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_FRAME_DONE_INTERVAL                 \
(                                                         \
(FASTDVFS_COUNTER_EB_FRAME_DONE_INTERVAL*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_TARGET_TIME                 \
(                                              \
(FASTDVFS_COUNTER_TARGET_TIME*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_TARGET_FRAME_BOUNDARY          \
(                                                 \
(FASTDVFS_COUNTER_FRAME_BOUNDARY*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_LEFT_WL                 \
(                                          \
(FASTDVFS_COUNTER_LEFT_WL*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_ELAPSED_TIME                 \
(                                               \
(FASTDVFS_COUNTER_ELAPSED_TIME*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_LEFT_TIME                 \
(                                            \
(FASTDVFS_COUNTER_LEFT_TIME*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_FRAME_END_HINT_CNT                   \
(                                                       \
(FASTDVFS_COUNTER_FRAME_END_HINT_COUNT*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_UNDER_HINT_WL                 \
(                                                \
(FASTDVFS_COUNTER_UNDER_HINT_WL*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_UNDER_HINT_CNT                 \
(                                                 \
(FASTDVFS_COUNTER_UNDER_HINT_CNT*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_JS0_DELTA                 \
(                                            \
(FASTDVFS_COUNTER_JS0_DELTA*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_COMMIT_PROFILE            \
(                                            \
(FASTDVFS_COUNTER_COMMIT_PROFILE*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_DCS                 \
(                                      \
(FASTDVFS_COUNTER_DCS*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_LAST_COMMIT_IDX               \
(									   \
(FASTDVFS_COUNTER_LAST_COMMIT_IDX*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_LAST_COMMIT_TOP_IDX               \
(									   \
(FASTDVFS_COUNTER_LAST_COMMIT_TOP_IDX*SYSRAM_LOG_SIZE) \
)

#define SYSRAM_GPU_FEEDBACK_INFO_UPDATED \
(                                      \
(FASTDVFS_FEEDBACK_INFO_UPDATED*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_FEEDBACK_INFO_GPU_TIME \
(                                      \
(FASTDVFS_FEEDBACK_INFO_GPU_TIME*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_FEEDBACK_INFO_GPU_UTILS \
(                                      \
(FASTDVFS_FEEDBACK_INFO_GPU_UTILS*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_FEEDBACK_INFO_CURR_FPS \
(                                      \
(FASTDVFS_FEEDBACK_INFO_CURR_FPS*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_ENABLE_FRAME_BASE_DVFS \
(                                      \
(FASTDVFS_ENABLE_FRAME_BASE_DVFS*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_SET_TARGET_FRAME_TIME \
(                                      \
(FASTDVFS_SET_TARGET_FRAME_TIME*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_COMMIT_PLATFORM_FREQ_IDX \
(                                      \
(FASTDVFS_COMMIT_PLATFORM_FREQ_IDX*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_COMMIT_VIRTUAL_FREQ \
(                                      \
(FASTDVFS_COMMIT_VIRTUAL_FREQUENCY*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_COMMIT_TYPE \
(                                      \
(FASTDVFS_COMMIT_TYPE*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_SET_TARGET_MARGIN \
(                                      \
(FASTDVFS_SET_TARGET_MARGIN*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_TA_3D_COEF \
(                                      \
(FASTDVFS_TA_3D_COEF*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_DESIRE_FREQ_ID \
(                                     \
(FASTDVFS_GPU_EB_DESIRE_FREQ_ID*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_DESIRE_POLICY_STATE \
(                                          \
(FASTDVFS_GPU_EB_DESIRE_POLICY_STATE*SYSRAM_LOG_SIZE) \
)
/* debug mode on, write dvfs info in sysram */
#define SYSRAM_GPU_EB_CUR_LOADING \
(                                  \
(FASTDVFS_GPU_EB_CUR_LOADING*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_CUR_VIRTUAL_FREQ \
(                                       \
(FASTDVFS_GPU_EB_CUR_VIRTUAL_FREQ*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_CUR_OPPIDX  \
(                                  \
(FASTDVFS_GPU_EB_CUR_OPPIDX*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_MARGIN_HIGH  \
(                                   \
(FASTDVFS_GPU_EB_MARGIN_HIGH*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_MARGIN_LOW  \
(                                  \
(FASTDVFS_GPU_EB_MARGIN_LOW*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_MARGIN  \
(                              \
(FASTDVFS_GPU_EB_MARGIN*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_CUR_POLICY_STATE  \
(                                        \
(FASTDVFS_GPU_EB_CUR_POLICY_STATE*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_EB_GED_MIN_OPPIDX  \
(									   \
(FASTDVFS_GPU_EB_GED_MIN_OPPIDX*SYSRAM_LOG_SIZE) \
)
#define SYSRAM_GPU_PWR_HINT               \
(									   \
(FASTDVFS_GPU_PWR_HINT*SYSRAM_LOG_SIZE) \
)

enum action_map {
	ACTION_MAP_FASTDVFS = 0,
	ACTION_MAP_FULLTRACE = 1,

	NR_ACTION_MAP
};

/**************************************************
 * GPU FAST DVFS IPI CMD
 **************************************************/

#define FASTDVFS_IPI_TIMEOUT 2000 //ms
#define FDVFS_REDUCE_IPI 1

enum {
	GPUFDVFS_IPI_SET_FRAME_DONE         = 1,
	GPUFDVFS_IPI_GET_FRAME_LOADING      = 2,
	GPUFDVFS_IPI_SET_NEW_FREQ           = 3,
	GPUFDVFS_IPI_GET_CURR_FREQ          = 4,
	GPUFDVFS_IPI_PMU_START              = 5,
	GPUFDVFS_IPI_SET_FRAME_BASE_DVFS    = 6,
	GPUFDVFS_IPI_SET_TARGET_FRAME_TIME  = 7,
	GPUFDVFS_IPI_SET_FEEDBACK_INFO      = 8,
	GPUFDVFS_IPI_SET_MODE               = 9,
	GPUFDVFS_IPI_GET_MODE               = 10,
	GPUFDVFS_IPI_SET_GED_READY          = 11,
	GPUFDVFS_IPI_SET_POWER_STATE        = 12,
	GPUFDVFS_IPI_SET_DVFS_STRESS_TEST   = 13,

	NR_GPUFDVFS_IPI,
};

/* IPI data structure */
struct fdvfs_ipi_data {
	unsigned int cmd;
	union {
		struct {
			unsigned int arg[5];
		} set_para;
	} u;
};


struct fdvfs_ipi_rcv_data {
	unsigned int cmd;
	union {
		struct {
			unsigned int arg[5];
		} set_para;
	} u;
};

/**************************************************
 * GPU FAST DVFS EVENT IPI
 **************************************************/
enum {
	GPUFDVFS_IPI_EVENT_CLK_CHANGE = 1,
	GPUFDVFS_IPI_EVENT_DEBUG_MODE_ON = 2,

	NR_GPUFDVFS_IPI_EVENT_CMD,
};

struct GED_EB_EVENT {
	int cmd;
	unsigned int freq_new;
	struct work_struct sWork;
	bool bUsed;
};

struct fastdvfs_event_data {
	unsigned int cmd;
	union {
		struct {
		unsigned int arg[3];
		} set_para;
	} u;
};

/**************************************************
 * Definition
 **************************************************/
#define FDVFS_IPI_DATA_LEN \
	DIV_ROUND_UP(sizeof(struct fdvfs_ipi_data), MBOX_SLOT_SIZE)

extern void fdvfs_init(void);
extern void fdvfs_exit(void);
extern int ged_to_fdvfs_command(unsigned int cmd,
	struct fdvfs_ipi_data *fdvfs_d);
extern int mtk_gpueb_sysram_read(int offset);
extern int mtk_gpueb_sysram_write(int offset, int value);

/**************************************************
 * GPU FAST DVFS EXPORTED API
 **************************************************/
extern int mtk_gpueb_dvfs_set_frame_done(void);
extern unsigned int mtk_gpueb_dvfs_get_cur_freq(void);
extern unsigned int mtk_gpueb_dvfs_get_frame_loading(void);
extern void mtk_gpueb_dvfs_commit(unsigned long ui32NewFreqID,
		GED_DVFS_COMMIT_TYPE eCommitType, int *pbCommited);
extern void mtk_gpueb_dvfs_dcs_commit(unsigned int platform_freq_idx,
		GED_DVFS_COMMIT_TYPE eCommitType, unsigned int virtual_freq_in_MHz);
extern unsigned int mtk_gpueb_dvfs_set_frame_base_dvfs(unsigned int enable);
extern int mtk_gpueb_dvfs_set_taget_frame_time(unsigned int target_frame_time,
		unsigned int target_margin);
extern unsigned int
	mtk_gpueb_dvfs_set_feedback_info(int frag_done_interval_in_ns,
	struct GpuUtilization_Ex util_ex, unsigned int curr_fps);
extern unsigned int mtk_gpueb_dvfs_get_mode(unsigned int *pAction);
extern unsigned int mtk_gpueb_dvfs_set_mode(unsigned int action);
unsigned int mtk_gpueb_set_fallback_mode(int fallback_status);
unsigned int mtk_gpueb_set_stability_mode(int stability_status);
void mtk_gpueb_dvfs_get_desire_freq(unsigned long *ui32NewFreqID);
void mtk_gpueb_dvfs_get_desire_freq_dual(unsigned long *stackNewFreqID,
	unsigned long *topNewFreqID);
extern unsigned int is_fdvfs_enable(void);
extern int mtk_gpueb_power_modle_cmd(unsigned int enable);
extern void mtk_swpm_gpu_pm_start(void);
extern int mtk_set_ged_ready(int ged_ready_flag);
/* DVFS IPI */
void mtk_gpueb_set_power_state(enum ged_gpu_power_state power_state);


extern int fastdvfs_proc_init(void);
extern void fastdvfs_proc_exit(void);

#endif // __GED_EB_H__
