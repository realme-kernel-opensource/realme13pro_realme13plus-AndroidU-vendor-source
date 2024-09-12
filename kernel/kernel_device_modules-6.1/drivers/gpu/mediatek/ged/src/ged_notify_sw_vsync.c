// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2019 MediaTek Inc.
 */


#include <linux/version.h>
#include <linux/workqueue.h>
#include <linux/sched/clock.h>
#include <linux/atomic.h>

#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/mutex.h>
#include <linux/timekeeping.h>

#include <asm/div64.h>

#include <mt-plat/mtk_gpu_utility.h>
#include "ged_notify_sw_vsync.h"
#include "ged_log.h"
#include "ged_tracepoint.h"
#include "ged_base.h"
#include "ged_monitor_3D_fence.h"
#include "ged.h"
#include "ged_dvfs.h"
#include "ged_dcs.h"
#include "ged_kpi.h"
#include "ged_eb.h"
#include "ged_perfetto_tracepoint.h"

#if defined(CONFIG_MTK_GPUFREQ_V2)
#include <ged_gpufreq_v2.h>
#include <gpufreq_v2.h>
#else
#include <ged_gpufreq_v1.h>
#endif /* CONFIG_MTK_GPUFREQ_V2 */

#ifndef OPLUS_ARCH_EXTENDS
#define OPLUS_ARCH_EXTENDS
#endif

#define GED_DVFS_FB_TIMER_TIMEOUT 100000000
#define GED_DVFS_TIMER_TIMEOUT g_fallback_time_out

#ifndef ENABLE_TIMER_BACKUP

#undef GED_DVFS_TIMER_TIMEOUT

#define GED_DVFS_FB_TIMER_TIMEOUT 100000000
#define GED_DVFS_TIMER_TIMEOUT g_fallback_time_out

#endif /* GED_DVFS_TIMER_TIMEOUT */

static u64 g_fallback_time_out = GED_DVFS_FB_TIMER_TIMEOUT;
static u64 g_last_pwr_ts;
static u64 g_last_pwr_update_ts_ms;

static struct hrtimer g_HT_hwvsync_emu;

#include "ged_dvfs.h"
#include "ged_global.h"

static struct workqueue_struct *g_psNotifyWorkQueue;

static struct mutex gsVsyncStampLock;

struct GED_NOTIFY_SW_SYNC {
	struct work_struct	sWork;
	unsigned long t;
	long phase;
	unsigned long ul3DFenceDoneTime;
	bool bUsed;
};

struct ged_gpu_power_state_time {
	//Unit: ns
	u64 start_ts;
	u64 end_ts;
	//Unit: ms
	u64 accumulate_time;
};

u32 g_curr_pwr_state;
static struct ged_gpu_power_state_time pwr_state_time[3];

#define MAX_NOTIFY_CNT 125
struct GED_NOTIFY_SW_SYNC loading_base_notify[MAX_NOTIFY_CNT];
int notify_index;
static enum gpu_dvfs_policy_state g_policy_state = POLICY_STATE_INIT;
static enum gpu_dvfs_policy_state g_prev_policy_state = POLICY_STATE_INIT;

#if IS_ENABLED(CONFIG_MTK_GPU_APO_SUPPORT)
#define GED_APO_THRESHOLD_US 2000
#define GED_APO_LP_THRESHOLD_US 4000

static spinlock_t g_sApoLock;

static unsigned long long g_apo_threshold_us;
static unsigned long long g_apo_threshold_ns;
static unsigned long long g_apo_wakeup_us;
static unsigned long long g_apo_wakeup_ns;
static unsigned long long g_apo_lp_threshold_us;
static unsigned long long g_apo_lp_threshold_ns;

static unsigned long long g_ns_gpu_active_ts;
static unsigned long long g_ns_gpu_idle_ts;
static long long g_ns_gpu_off_duration;

static unsigned long long g_ns_gpu_predict_active_ts;
static unsigned long long g_ns_gpu_predict_idle_ts;
static long long g_ns_gpu_predict_off_duration;

static bool g_bGPUAPO;
static bool g_bGPUPredictAPO;
static int g_apo_hint;
static int g_apo_force_hint;
#endif /* CONFIG_MTK_GPU_APO_SUPPORT */

int (*ged_sw_vsync_event_fp)(bool bMode) = NULL;
EXPORT_SYMBOL(ged_sw_vsync_event_fp);
static struct mutex gsVsyncModeLock;
static int ged_sw_vsync_event(bool bMode)
{
	static bool bCurMode;
	int ret;

	ret = 0;
	mutex_lock(&gsVsyncModeLock);

	if (bCurMode != bMode) {
		bCurMode = bMode;
		if (ged_sw_vsync_event_fp) {
			ret = ged_sw_vsync_event_fp(bMode);
			ged_log_buf_print(ghLogBuf_DVFS,
			"[GED_K] ALL mode change to %d ", bCurMode);
		} else
			ged_log_buf_print(ghLogBuf_DVFS,
			"[GED_K] LOCAL mode change to %d ", bCurMode);
		if (bCurMode)
			ret = 1;
	}

	mutex_unlock(&gsVsyncModeLock);
	return ret;
}

u64 ged_get_fallback_time(void)
{
	u64 temp = 0;

	if (is_fdvfs_enable()) {
		if (g_fallback_mode == ALIGN_INTERVAL)
			temp = (u64)g_fallback_time * 1000000;   //ms to ns
		else if (g_fallback_mode == ALIGN_FB)
			temp = fb_timeout * g_fallback_time / 10;
		else if (g_fallback_mode == ALIGN_LB)
			temp = lb_timeout * g_fallback_time / 10;
		else if (g_fallback_mode == ALIGN_FAST_DVFS)
			temp = ged_kpi_get_taget_time();
	} else {
		if (g_fallback_mode == 0)
			temp = (u64)g_fallback_time * 1000000;   //ms to ns
		else if (g_fallback_mode == 1)
			temp = fb_timeout * g_fallback_time / 10;
		else if (g_fallback_mode == 2)
			temp = lb_timeout * g_fallback_time / 10;
	}
	return temp;
}


enum gpu_dvfs_policy_state ged_get_policy_state(void)
{
	if (is_fdvfs_enable()) {
		if ((g_policy_state == POLICY_STATE_LB_FALLBACK ||
			g_policy_state == POLICY_STATE_FORCE_LB_FALLBACK ||
			g_policy_state == POLICY_STATE_FB_FALLBACK)) {
			mtk_gpueb_sysram_write(SYSRAM_GPU_EB_DESIRE_POLICY_STATE, 2);
		} else if (g_policy_state == POLICY_STATE_FB) {
			mtk_gpueb_sysram_write(SYSRAM_GPU_EB_DESIRE_POLICY_STATE, 1);
		} else if (g_policy_state == POLICY_STATE_LB ||
			g_policy_state == POLICY_STATE_FORCE_LB) {
			mtk_gpueb_sysram_write(SYSRAM_GPU_EB_DESIRE_POLICY_STATE, 0);
		}
	}
	return g_policy_state;
}

enum gpu_dvfs_policy_state ged_get_prev_policy_state(void)
{
	return g_prev_policy_state;
}

void ged_set_policy_state(enum gpu_dvfs_policy_state state)
{
	g_policy_state = state;
}

void ged_set_prev_policy_state(enum gpu_dvfs_policy_state state)
{
	g_prev_policy_state = state;
}

static unsigned long long sw_vsync_ts;
static void ged_notify_sw_sync_work_handle(struct work_struct *psWork)
{
	struct GED_NOTIFY_SW_SYNC *psNotify =
		GED_CONTAINER_OF(psWork, struct GED_NOTIFY_SW_SYNC, sWork);
	unsigned long long temp;
	GED_DVFS_COMMIT_TYPE eCommitType;
	u64 timeout_value;
	/*only one policy at a time*/
	if (psNotify) {
		mutex_lock(&gsPolicyLock);
		timeout_value = lb_timeout;
		psNotify->bUsed = false;

		if (hrtimer_get_remaining(&g_HT_hwvsync_emu) < 0) {
			enum gpu_dvfs_policy_state policy_state;

			policy_state = ged_get_policy_state();
			if (policy_state == POLICY_STATE_FB ||
					policy_state == POLICY_STATE_FB_FALLBACK ||
					policy_state == POLICY_STATE_LB_FALLBACK ||
					policy_state == POLICY_STATE_FORCE_LB_FALLBACK) {
				eCommitType = GED_DVFS_FALLBACK_COMMIT;
				if (policy_state == POLICY_STATE_LB_FALLBACK)
					ged_set_policy_state(POLICY_STATE_LB_FALLBACK);
				else if (policy_state == POLICY_STATE_FORCE_LB_FALLBACK)
					ged_set_policy_state(POLICY_STATE_FORCE_LB_FALLBACK);
				else
					ged_set_policy_state(POLICY_STATE_FB_FALLBACK);
				timeout_value = ged_get_fallback_time();
			} else {
				eCommitType = GED_DVFS_LOADING_BASE_COMMIT;
			}
			ged_set_backup_timer_timeout(timeout_value);   // set init value

			temp = 0;
			/* if callback is queued, send mode off to real driver */
			ged_sw_vsync_event(false);
#ifdef ENABLE_TIMER_BACKUP
			temp = ged_get_time();
			if (temp-sw_vsync_ts > GED_DVFS_TIMER_TIMEOUT) {
				do_div(temp, 1000);
				psNotify->t = temp;
				ged_dvfs_run(psNotify->t, psNotify->phase,
					psNotify->ul3DFenceDoneTime, eCommitType);
				ged_log_buf_print(ghLogBuf_DVFS,
					"[GED_K] Timer kicked	(ts=%llu) ", temp);
			} else {
				ged_log_buf_print(ghLogBuf_DVFS,
					"[GED_K] Timer kick giveup (ts=%llu)", temp);
			}
#endif
			hrtimer_start(&g_HT_hwvsync_emu,
				ns_to_ktime(GED_DVFS_TIMER_TIMEOUT), HRTIMER_MODE_REL);
		}
		mutex_unlock(&gsPolicyLock);
	}
#if IS_ENABLED(CONFIG_MTK_GPU_FW_IDLE)
	/* set initial idle time to 5ms if runtime policy stay default flavor */
	if (ged_kpi_is_fw_idle_policy_enable() == -1)
		mtk_set_gpu_idle(5);
#endif /* MTK_GPU_FW_IDLE */
}

#define GED_VSYNC_MISS_QUANTUM_NS 16666666

#ifdef ENABLE_COMMON_DVFS
static unsigned long long hw_vsync_ts;
#endif

static unsigned long long g_ns_gpu_off_ts;

static bool g_timer_on;
static unsigned long long g_timer_on_ts;
static bool g_bGPUClock;

/*
 * void timer_switch(bool bTock)
 * only set the staus, not really operating on real timer
 */
void timer_switch(bool bTock)
{
	mutex_lock(&gsVsyncStampLock);
	g_timer_on = bTock;
	if (bTock)
		g_timer_on_ts = ged_get_time();
	mutex_unlock(&gsVsyncStampLock);
}

void timer_switch_locked(bool bTock)
{
	g_timer_on = bTock;
	if (bTock)
		g_timer_on_ts = ged_get_time();
}

static void ged_timer_switch_work_handle(struct work_struct *psWork)
{
	struct GED_NOTIFY_SW_SYNC *psNotify =
		GED_CONTAINER_OF(psWork, struct GED_NOTIFY_SW_SYNC, sWork);
	if (psNotify) {
		ged_sw_vsync_event(false);

		psNotify->bUsed = false;
	}
}


void ged_set_backup_timer_timeout(u64 time_out)
{
	if (time_out != 0 && time_out < GED_DVFS_FB_TIMER_TIMEOUT)
		g_fallback_time_out = time_out;
	else
		g_fallback_time_out = GED_DVFS_FB_TIMER_TIMEOUT;
}


void ged_cancel_backup_timer(void)
{
	unsigned long long temp;

	temp = ged_get_time();
#ifdef ENABLE_TIMER_BACKUP
	if (g_ged_frame_base_optimize == 0 || g_bGPUClock) {
		if (hrtimer_try_to_cancel(&g_HT_hwvsync_emu)) {
			/* Timer is either queued or in cb
			 * cancel it to ensure it is not bother any way
			 */
			hrtimer_cancel(&g_HT_hwvsync_emu);
			hrtimer_start(&g_HT_hwvsync_emu,
				ns_to_ktime(GED_DVFS_TIMER_TIMEOUT), HRTIMER_MODE_REL);
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] Timer Restart (ts=%llu)", temp);
		} else {
			/*
			 * Timer is not existed
			 */
			hrtimer_start(&g_HT_hwvsync_emu,
				ns_to_ktime(GED_DVFS_TIMER_TIMEOUT), HRTIMER_MODE_REL);
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] New Timer Start (ts=%llu)", temp);
			timer_switch_locked(true);
		}
	}
#endif /*	#ifdef ENABLE_TIMER_BACKUP	*/
}


GED_ERROR ged_notify_sw_vsync(GED_VSYNC_TYPE eType,
	struct GED_DVFS_UM_QUERY_PACK *psQueryData)
{
	ged_notification(GED_NOTIFICATION_TYPE_SW_VSYNC);

	{
#ifdef ENABLE_COMMON_DVFS

	long phase = 0;
	unsigned long t;
	bool bHWEventKick = false;
	long long llDiff = 0;

	unsigned long long temp;
	unsigned long ul3DFenceDoneTime;


	psQueryData->bFirstBorn = ged_sw_vsync_event(true);

	ul3DFenceDoneTime = ged_monitor_3D_fence_done_time();

	psQueryData->ul3DFenceDoneTime = ul3DFenceDoneTime;

	hw_vsync_ts = temp = ged_get_time();

	if (g_gpu_timer_based_emu) {
		ged_log_buf_print(ghLogBuf_DVFS,
			"[GED_K] Vsync ignored (ts=%llu)", temp);
		return GED_ERROR_INTENTIONAL_BLOCK;
	}

/* TODO: temp defined to disable vsync_dvfs when COMMON_DVFS on*/
#ifdef ENABLE_COMMON_DVFS
	return GED_ERROR_INTENTIONAL_BLOCK;
#else

	/*critical session begin*/
	mutex_lock(&gsVsyncStampLock);

	if (eType == GED_VSYNC_SW_EVENT) {
		sw_vsync_ts = temp;
#ifdef ENABLE_TIMER_BACKUP
		if (hrtimer_try_to_cancel(&g_HT_hwvsync_emu)) {
			/* Timer is either queued or in cb
			 * cancel it to ensure it is not bother any way
			 */
			hrtimer_cancel(&g_HT_hwvsync_emu);
			hrtimer_start(&g_HT_hwvsync_emu,
			ns_to_ktime(GED_DVFS_TIMER_TIMEOUT), HRTIMER_MODE_REL);
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] Timer Restart (ts=%llu)", temp);
		} else {
			/*
			 * Timer is not existed
			 */
			hrtimer_start(&g_HT_hwvsync_emu,
			ns_to_ktime(GED_DVFS_TIMER_TIMEOUT), HRTIMER_MODE_REL);
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] New Timer Start (ts=%llu)", temp);
			timer_switch_locked(true);
		}
#endif // #ifdef ENABLE_TIMER_BACKUP
	} else {
		hw_vsync_ts = temp;

		llDiff = (long long)(hw_vsync_ts - sw_vsync_ts);

		if (llDiff > GED_VSYNC_MISS_QUANTUM_NS)
			bHWEventKick = true;
	}
#ifdef GED_DVFS_DEBUG
	if (eType == GED_VSYNC_HW_EVENT)
		GED_LOGD("HW VSYNC: llDiff=",
		"%lld, hw_vsync_ts=%llu, sw_vsync_ts=%llu\n", llDiff,
		hw_vsync_ts, sw_vsync_ts);
	else
		GED_LOGD("SW VSYNC: llDiff=",
		"%lld, hw_vsync_ts=%llu, sw_vsync_ts=%llu\n", llDiff,
		hw_vsync_ts, sw_vsync_ts);
#endif		///	#ifdef GED_DVFS_DEBUG


	if (eType == GED_VSYNC_HW_EVENT)
		ged_log_buf_print(ghLogBuf_DVFS,
		"[GED_K] HW VSYNC (ts=%llu) ", hw_vsync_ts);
	else
		ged_log_buf_print(ghLogBuf_DVFS,
		"[GED_K] SW VSYNC (ts=%llu) ", sw_vsync_ts);

	mutex_unlock(&gsVsyncStampLock);
	/*critical session end*/

	if (eType == GED_VSYNC_SW_EVENT) {
		do_div(temp, 1000);
		t = (unsigned long)(temp);

		// for some cases just align vsync to FenceDoneTime
		if (ul3DFenceDoneTime > t) {
			if (ul3DFenceDoneTime - t < GED_DVFS_DIFF_THRESHOLD)
				t = ul3DFenceDoneTime;
		}
		psQueryData->usT = t;
		ged_dvfs_run(t, phase, ul3DFenceDoneTime,
			GED_DVFS_LOADING_BASE_COMMIT);
		ged_dvfs_sw_vsync_query_data(psQueryData);
	} else {
		if (bHWEventKick) {
#ifdef GED_DVFS_DEBUG
			GED_LOGD("HW Event: kick!\n");
#endif							/// GED_DVFS_DEBUG
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] HW VSync: mending kick!");
			ged_dvfs_run(0, 0, 0, 0);
		}
	}
#endif
#else
	unsigned long long temp;

	temp = ged_get_time();
	ged_sw_vsync_event(true);
	return GED_ERROR_INTENTIONAL_BLOCK;
#endif

	return GED_OK;
	}
}


extern unsigned int gpu_loading;
enum hrtimer_restart ged_sw_vsync_check_cb(struct hrtimer *timer)
{
	unsigned long long temp;
	long long llDiff;
	unsigned int loading_mode = 0;
	unsigned int gpu_loading_temp = 0;
	struct GED_NOTIFY_SW_SYNC *psNotify;
	struct GpuUtilization_Ex util_ex;

	memset(&util_ex, 0, sizeof(util_ex));

	temp = cpu_clock(smp_processor_id());

	llDiff = (long long)(temp - sw_vsync_ts);

	if (llDiff > GED_VSYNC_MISS_QUANTUM_NS) {
		psNotify = &(loading_base_notify[((notify_index++) % MAX_NOTIFY_CNT)]);

		if (notify_index >= MAX_NOTIFY_CNT)
			notify_index = 0;

#ifndef ENABLE_TIMER_BACKUP
		ged_dvfs_cal_gpu_utilization_ex(&gpu_av_loading,
			&gpu_block, &gpu_idle, &util_ex);
		gpu_loading = gpu_av_loading;
#endif

		ged_get_gpu_utli_ex(&util_ex);
		mtk_get_dvfs_loading_mode(&loading_mode);
		if (loading_mode == LOADING_MAX_3DTA_COM) {
			gpu_loading_temp =
			MAX(util_ex.util_3d, util_ex.util_ta) +
			util_ex.util_compute;
		} else if (loading_mode == LOADING_MAX_3DTA) {
			gpu_loading_temp =
			MAX(util_ex.util_3d, util_ex.util_ta);
		} else if (loading_mode == LOADING_ITER) {
			gpu_loading_temp = util_ex.util_iter;
		} else if (loading_mode == LOADING_MAX_ITERMCU) {
			gpu_loading_temp = MAX(util_ex.util_iter, util_ex.util_mcu);
		} else {   // LOADING_ACTIVE or unknown mode
			gpu_loading_temp = util_ex.util_active;
		}

		if (false == g_bGPUClock && 0 == gpu_loading_temp
			&& (temp - g_ns_gpu_on_ts > GED_DVFS_TIMER_TIMEOUT)) {
			if (psNotify && psNotify->bUsed == false) {
				psNotify->bUsed = true;
				INIT_WORK(&psNotify->sWork,
					ged_timer_switch_work_handle);
				queue_work(g_psNotifyWorkQueue,
					&psNotify->sWork);
				timer_switch_locked(false);
			}
#ifdef GED_DVFS_DEBUG
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] Timer removed	(ts=%llu) ", temp);
#endif
			return HRTIMER_NORESTART;
		}

		if (psNotify) {
			if (psNotify->bUsed == false) {
				psNotify->bUsed = true;
				INIT_WORK(&psNotify->sWork,
					ged_notify_sw_sync_work_handle);
				psNotify->phase = GED_DVFS_TIMER_BACKUP;
				psNotify->ul3DFenceDoneTime = 0;
				queue_work(g_psNotifyWorkQueue, &psNotify->sWork);
#ifdef GED_DVFS_DEBUG
				ged_log_buf_print(ghLogBuf_DVFS,
					"[GED_K] Timer queue to kick (ts=%llu)", temp);
#endif
			}
			g_timer_on_ts = temp;
		}
	}
	return HRTIMER_NORESTART;
}

#if IS_ENABLED(CONFIG_MTK_GPU_APO_SUPPORT)
unsigned int ged_gpu_apo_support(void)
{
	return g_ged_apo_support;
}
EXPORT_SYMBOL(ged_gpu_apo_support);

unsigned long long ged_get_apo_threshold_us(void)
{
	return g_apo_threshold_us;
}
EXPORT_SYMBOL(ged_get_apo_threshold_us);

void ged_set_apo_threshold_us(unsigned long long apo_threshold_us)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_apo_threshold_us = apo_threshold_us;
	g_apo_threshold_ns = g_apo_threshold_us * 1000;

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_set_apo_threshold_us);

unsigned long long ged_get_apo_wakeup_us(void)
{
	return g_apo_wakeup_us;
}
EXPORT_SYMBOL(ged_get_apo_wakeup_us);

void ged_set_apo_wakeup_us(unsigned long long apo_wakeup_us)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_apo_wakeup_us = apo_wakeup_us;
	g_apo_wakeup_ns = g_apo_wakeup_us * 1000;

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_set_apo_wakeup_us);

unsigned long long ged_get_apo_lp_threshold_us(void)
{
	return g_apo_lp_threshold_us;
}
EXPORT_SYMBOL(ged_get_apo_lp_threshold_us);

void ged_set_apo_lp_threshold_us(unsigned long long apo_lp_threshold_us)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_apo_lp_threshold_us = apo_lp_threshold_us;
	g_apo_lp_threshold_ns = g_apo_lp_threshold_us * 1000;

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_set_apo_lp_threshold_us);

int ged_get_apo_hint(void)
{
	return g_apo_hint;
}
EXPORT_SYMBOL(ged_get_apo_hint);

int ged_get_apo_force_hint(void)
{
	return g_apo_force_hint;
}
EXPORT_SYMBOL(ged_get_apo_force_hint);

void ged_set_apo_force_hint(int apo_force_hint)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_apo_force_hint = apo_force_hint;

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_set_apo_force_hint);

void ged_get_active_time(void)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_ns_gpu_active_ts = ged_get_time();

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_get_active_time);

void ged_get_idle_time(void)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_ns_gpu_idle_ts = ged_get_time();

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_get_idle_time);

void ged_check_power_duration(void)
{
	long long llDiff = 0;
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_ns_gpu_off_duration =
		(long long)(g_ns_gpu_active_ts - g_ns_gpu_idle_ts);
	llDiff = g_ns_gpu_off_duration;

	if ((llDiff > 0) && (llDiff < g_apo_threshold_ns))
		g_bGPUAPO = true;
	else {
		g_bGPUAPO = false;

		if (g_apo_threshold_ns == 0) {
			if (g_ged_apo_support == APO_NORMAL_AND_LP_SUPPORT &&
				(llDiff >= g_apo_threshold_ns) &&
				(llDiff < g_apo_lp_threshold_ns))
				g_apo_hint = APO_LP_HINT;
			else
				g_apo_hint = APO_NORMAL_HINT;

			if (g_apo_force_hint >= APO_NORMAL_HINT &&
				g_apo_force_hint < APO_INVALID_HINT)
				g_apo_hint = g_apo_force_hint;

			ged_write_sysram_pwr_hint(g_apo_hint);
		}

		g_ns_gpu_active_ts = 0;
		g_ns_gpu_idle_ts = 0;
		g_ns_gpu_off_duration = 0;
	}

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_check_power_duration);

long long ged_get_power_duration(void)
{
	return g_ns_gpu_off_duration;
}
EXPORT_SYMBOL(ged_get_power_duration);

void ged_gpu_apo_reset(void)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_bGPUAPO = false;
	g_ns_gpu_active_ts = 0;
	g_ns_gpu_idle_ts = 0;
	g_ns_gpu_off_duration = 0;

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_gpu_apo_reset);

bool ged_gpu_apo_notify(void)
{
	if (g_ged_apo_support)
		return g_bGPUAPO;
	else
		return false;
}
EXPORT_SYMBOL(ged_gpu_apo_notify);

void ged_get_predict_active_time(void)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_ns_gpu_predict_active_ts = ged_get_time();

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_get_predict_active_time);

void ged_get_predict_idle_time(void)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_ns_gpu_predict_idle_ts = ged_get_time();

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_get_predict_idle_time);

void ged_check_predict_power_duration(void)
{
	long long llDiff = 0;
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_ns_gpu_predict_off_duration =
		(long long)(g_ns_gpu_predict_active_ts - g_ns_gpu_predict_idle_ts);
	llDiff = g_ns_gpu_predict_off_duration;

	if ((llDiff > 0) && (llDiff < g_apo_threshold_ns))
		g_bGPUPredictAPO = true;
	else {
		g_bGPUPredictAPO = false;

		if (g_ged_apo_support == APO_NORMAL_AND_LP_SUPPORT &&
			(llDiff >= g_apo_threshold_ns) &&
			(llDiff < g_apo_lp_threshold_ns))
			g_apo_hint = APO_LP_HINT;
		else
			g_apo_hint = APO_NORMAL_HINT;

		if (g_apo_force_hint >= APO_NORMAL_HINT && g_apo_force_hint < APO_INVALID_HINT)
			g_apo_hint = g_apo_force_hint;

		ged_write_sysram_pwr_hint(g_apo_hint);

		g_ns_gpu_predict_active_ts = 0;
		g_ns_gpu_predict_idle_ts = 0;
		g_ns_gpu_predict_off_duration = 0;

		g_bGPUAPO = false;
		g_ns_gpu_active_ts = 0;
		g_ns_gpu_idle_ts = 0;
		g_ns_gpu_off_duration = 0;
	}

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_check_predict_power_duration);

long long ged_get_predict_power_duration(void)
{
	return g_ns_gpu_predict_off_duration;
}
EXPORT_SYMBOL(ged_get_predict_power_duration);

void ged_gpu_predict_apo_reset(void)
{
	unsigned long ulIRQFlags;

	spin_lock_irqsave(&g_sApoLock, ulIRQFlags);

	g_bGPUPredictAPO = false;
	g_apo_hint = APO_NORMAL_HINT;
	g_ns_gpu_predict_active_ts = 0;
	g_ns_gpu_predict_idle_ts = 0;
	g_ns_gpu_predict_off_duration = 0;

	spin_unlock_irqrestore(&g_sApoLock, ulIRQFlags);
}
EXPORT_SYMBOL(ged_gpu_predict_apo_reset);

bool ged_gpu_predict_apo_notify(void)
{
	if (g_ged_apo_support)
		return g_bGPUPredictAPO;
	else
		return false;
}
EXPORT_SYMBOL(ged_gpu_predict_apo_notify);
#endif /* CONFIG_MTK_GPU_APO_SUPPORT */

unsigned long long ged_get_power_on_timestamp(void)
{
	return g_ns_gpu_on_ts;
}

static void ged_dvfs_update_power_state_time(
		enum ged_gpu_power_state power_state)
{
	u64 delta_time = 0;
	struct timespec64 tv = {0};

	g_last_pwr_ts = ged_get_time();
	pwr_state_time[g_curr_pwr_state].end_ts = g_last_pwr_ts;

	ktime_get_real_ts64(&tv);
	g_last_pwr_update_ts_ms = tv.tv_sec * 1000 + tv.tv_nsec / 1000000;

	// Check to prevent overflow error
	if (pwr_state_time[g_curr_pwr_state].end_ts >=
		pwr_state_time[g_curr_pwr_state].start_ts) {
		delta_time = pwr_state_time[g_curr_pwr_state].end_ts -
			pwr_state_time[g_curr_pwr_state].start_ts;
		pwr_state_time[g_curr_pwr_state].accumulate_time +=
			(delta_time / 1000000);
	} else {
		delta_time = (ULLONG_MAX -
			pwr_state_time[g_curr_pwr_state].start_ts) +
			pwr_state_time[g_curr_pwr_state].end_ts;
		pwr_state_time[g_curr_pwr_state].accumulate_time +=
			(delta_time / 1000000);
	}

	// Reset power state timestamps
	pwr_state_time[g_curr_pwr_state].start_ts = 0;
	pwr_state_time[power_state].start_ts =
		pwr_state_time[g_curr_pwr_state].end_ts;
	g_curr_pwr_state = power_state;
}

void ged_dvfs_gpu_clock_switch_notify(enum ged_gpu_power_state power_state)
{
	enum gpu_dvfs_policy_state policy_state;

	policy_state = ged_get_policy_state();

	if (power_state == GED_POWER_ON) {
		g_ns_gpu_on_ts = ged_get_time();
		g_bGPUClock = true;
		/* check overdue if timestamp is queuebuffer(0x2) */
		if (g_ged_frame_base_optimize &&
			(policy_state == POLICY_STATE_FB ||
			policy_state == POLICY_STATE_FB_FALLBACK) &&
			ged_kpi_get_fb_ulMask() == 0x2) {
			unsigned long long uncomplete_time =
				g_ns_gpu_on_ts - ged_kpi_get_fb_timestamp();
			/* get in fallback if uncomplete time is overdue */
			if (uncomplete_time >= fb_timeout) {
				ged_set_policy_state(POLICY_STATE_FB_FALLBACK);
				ged_set_backup_timer_timeout(ged_get_fallback_time());
			} else {
				u64 fb_tmp_timeout = fb_timeout - uncomplete_time;
				u64 timeout_val = ged_get_fallback_time();
				/* keep minimum value of fallback time*/
				if (fb_tmp_timeout < timeout_val)
					fb_tmp_timeout = timeout_val;
				ged_set_policy_state(POLICY_STATE_FB);
				ged_set_backup_timer_timeout(fb_tmp_timeout);
			}
		}
		if (g_timer_on) {
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] Timer Already Start");
		} else {
			hrtimer_start(&g_HT_hwvsync_emu,
				ns_to_ktime(GED_DVFS_TIMER_TIMEOUT), HRTIMER_MODE_REL);
			ged_log_buf_print(ghLogBuf_DVFS,
				"[GED_K] HW Start Timer");
			timer_switch(true);
		}
	} else if (power_state == GED_POWER_OFF ||
			power_state == GED_SLEEP) {
		g_ns_gpu_off_ts = ged_get_time();
		g_bGPUClock = false;
		if (g_ged_frame_base_optimize &&
			(policy_state == POLICY_STATE_FB ||
			 policy_state == POLICY_STATE_FB_FALLBACK)) {
			int timer_flag = 0;
			if (hrtimer_try_to_cancel(&g_HT_hwvsync_emu)) {
				/* frame base pass power off timer*/
				hrtimer_cancel(&g_HT_hwvsync_emu);
				timer_flag = 1;
			}
			/* avoid multi lock*/
			if (timer_flag)
				timer_switch(false);
		}
		ged_log_buf_print(ghLogBuf_DVFS, "[GED_K] Buck-off");
		ged_dvfs_notify_power_off();
	}
	// Update power on/off state
	trace_tracing_mark_write(5566, "gpu_state", power_state);
	trace_perfetto_tracing_mark_write("gpu_state", power_state);
	#ifdef OPLUS_ARCH_EXTENDS
	trace_oplus_tracing_mark_write(5566, "gpu_state", power_state);
	#endif /*OPLUS_ARCH_EXTENDS*/

	ged_dvfs_update_power_state_time(power_state);
}
EXPORT_SYMBOL(ged_dvfs_gpu_clock_switch_notify);

int ged_dvfs_query_power_state_time(u64 *off_time, u64 *idle_time,
		u64 *on_time, u64 *last_ts)
{
	if (off_time == NULL || idle_time == NULL || on_time == NULL ||
		last_ts == NULL) {
		WARN(1, "Invalid parameters");
		return -EINVAL;
	}

	*last_ts = g_last_pwr_update_ts_ms;
	*off_time = pwr_state_time[GED_POWER_OFF].accumulate_time;
	*idle_time = pwr_state_time[GED_SLEEP].accumulate_time;
	*on_time = pwr_state_time[GED_POWER_ON].accumulate_time;

	return 0;
}
EXPORT_SYMBOL(ged_dvfs_query_power_state_time);

#define GED_TIMER_BACKUP_THRESHOLD 3000

#if IS_BUILTIN(CONFIG_MTK_GPU_SUPPORT)
#endif /* CONFIG_MTK_GPU_SUPPORT */

GED_ERROR ged_notify_sw_vsync_system_init(void)
{
	g_psNotifyWorkQueue = alloc_ordered_workqueue("ged_notify_sw_vsync",
						WQ_HIGHPRI | WQ_FREEZABLE | WQ_MEM_RECLAIM);

	if (g_psNotifyWorkQueue == NULL)
		return GED_ERROR_OOM;

	mutex_init(&gsVsyncStampLock);
	mutex_init(&gsVsyncModeLock);

	hrtimer_init(&g_HT_hwvsync_emu, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	g_HT_hwvsync_emu.function = ged_sw_vsync_check_cb;

#if IS_ENABLED(CONFIG_MTK_GPU_APO_SUPPORT)
	g_apo_threshold_us = GED_APO_THRESHOLD_US;
	g_apo_threshold_ns = g_apo_threshold_us * 1000;

	g_apo_wakeup_us = g_apo_threshold_us + 1000;
	g_apo_wakeup_ns = g_apo_wakeup_us * 1000;

	g_apo_lp_threshold_us = GED_APO_LP_THRESHOLD_US;
	g_apo_lp_threshold_ns = g_apo_lp_threshold_us * 1000;

	g_ns_gpu_off_duration = 0;
	g_ns_gpu_predict_off_duration = 0;

	g_bGPUAPO = false;
	g_bGPUPredictAPO = false;

	g_apo_hint = APO_NORMAL_HINT;
	g_apo_force_hint = APO_INVALID_HINT;

	spin_lock_init(&g_sApoLock);
#endif /* CONFIG_MTK_GPU_APO_SUPPORT */

	//Initialize current power state timestamp
	g_curr_pwr_state = (u32) gpufreq_get_power_state();
	pwr_state_time[g_curr_pwr_state].start_ts = ged_get_time();

	return GED_OK;
}

void ged_notify_sw_vsync_system_exit(void)
{
	if (g_psNotifyWorkQueue != NULL) {
		flush_workqueue(g_psNotifyWorkQueue);
		destroy_workqueue(g_psNotifyWorkQueue);
		g_psNotifyWorkQueue = NULL;
	}

#ifdef ENABLE_COMMON_DVFS
	hrtimer_cancel(&g_HT_hwvsync_emu);
#endif
	mutex_destroy(&gsVsyncModeLock);
	mutex_destroy(&gsVsyncStampLock);
}
