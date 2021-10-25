/*
 * Copyright (C) 2017-2020 OPPO, Inc.
 * Author: Gaowei Pu <>
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/kobject.h>
#include <linux/threads.h>
#include <linux/sched/stat.h>
#include <linux/sched/nohz.h>
#include <linux/sched/topology.h>
#include <linux/sched/energy.h>
#include <uapi/linux/sched/types.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/suspend.h>
#include <linux/kernel_stat.h>
#include <linux/sort.h>
#include <mt-plat/mtk_devinfo.h>
#include <linux/pm_qos.h>
#include <linux/cpufreq.h>
#include <tick-internal.h>
#include <helio-dvfsrc-opp.h>
#include "../../../../kernel/sched/sched.h"
#include "debug_load.h"

#define POWER_TUNER_TAG "[POWER_DEBUG]"
#define POWER_TUNER_INFO(fmt, args...) pr_info(POWER_TUNER_TAG fmt, ##args)
#define POWER_TUNER_ERR(fmt, args...) pr_info(POWER_TUNER_TAG fmt, ##args)
#define MAX_PID (32768)
#define CTP_WINDOW_SZ (5)
#define NUM_PRINT_HEAVY_LOAD_TASK (10)
#define SYSTEM_POWER_ON_TIME (300)
#define POWER_DEBUG_TIME_INTERVAL_MIN (5)
#define POWER_DEBUG_TIME_INTERVAL_MAX (600)

struct power_debug power_debug_info;
static bool is_enter_suspend = false;
unsigned int debug_power_timer_interval_s = 5;
static unsigned int pm_qos_debug_flag = 0;
struct task_stat cpustats_saved[MAX_PID];

int __attribute__((weak)) get_cur_vcore_uv(void)
{
	return 0;
}
static int pm_qos_cpu_dma_latency_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_CPU_DMA_LATENCY) {
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_CPU_DMA_LATENCY, l);
		pm_qos_trace_dbg_dump(PM_QOS_CPU_DMA_LATENCY);
	}
	return NOTIFY_OK;
}

static int pm_qos_memory_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_MEMORY_BANDWIDTH) {
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_MEMORY_BANDWIDTH, l);
		pm_qos_trace_dbg_dump(PM_QOS_MEMORY_BANDWIDTH);
	}
	return NOTIFY_OK;
}

static int pm_qos_cpu_memory_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_CPU_MEMORY_BANDWIDTH) {
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_CPU_MEMORY_BANDWIDTH, l);
		pm_qos_trace_dbg_dump(PM_QOS_CPU_MEMORY_BANDWIDTH);
	}
	return NOTIFY_OK;
}

static int pm_qos_gpu_memory_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_GPU_MEMORY_BANDWIDTH)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_GPU_MEMORY_BANDWIDTH, l);
	return NOTIFY_OK;
}

static int pm_qos_mm_memory_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_MM_MEMORY_BANDWIDTH)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_MM_MEMORY_BANDWIDTH, l);
	return NOTIFY_OK;
}

static int pm_qos_other_memory_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_OTHER_MEMORY_BANDWIDTH)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_OTHER_MEMORY_BANDWIDTH, l);
	return NOTIFY_OK;
}

static int pm_qos_ddr_opp_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_DDR_OPP) {
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_DDR_OPP, l);
		pm_qos_trace_dbg_dump(PM_QOS_DDR_OPP);
	}
	return NOTIFY_OK;
}

static int pm_qos_vcore_opp_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_VCORE_OPP) {
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_VCORE_OPP, l);
		pm_qos_trace_dbg_dump(PM_QOS_VCORE_OPP);
	}
	return NOTIFY_OK;
}

static int pm_qos_scp_vcore_request_print_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_SCP_VCORE_REQUEST)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_SCP_VCORE_REQUEST, l);
	return NOTIFY_OK;
}

static int pm_qos_power_model_ddr_request_print_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_POWER_MODEL_DDR_REQUEST)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_POWER_MODEL_DDR_REQUEST, l);
	return NOTIFY_OK;
}

static int pm_qos_power_model_vcore_request_print_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_POWER_MODEL_VCORE_REQUEST)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_POWER_MODEL_VCORE_REQUEST, l);
	return NOTIFY_OK;
}

static int pm_qos_vcore_dvfs_force_opp_print_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_VCORE_DVFS_FORCE_OPP)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_VCORE_DVFS_FORCE_OPP, l);
	return NOTIFY_OK;
}

static int pm_qos_isp_hrt_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_ISP_HRT_BANDWIDTH)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_ISP_HRT_BANDWIDTH, l);
	return NOTIFY_OK;
}

static int pm_qos_apu_memory_bandwidth_notify(struct notifier_block *b, unsigned long l, void *v)
{
	if (pm_qos_debug_flag & PM_QOS_APU_MEMORY_BANDWIDTH)
		POWER_TUNER_INFO("PMQOS: %d/%lu\n", PM_QOS_APU_MEMORY_BANDWIDTH, l);
	return NOTIFY_OK;
}


static void pm_qos_add_debug_notifiers()
{
	power_debug_info.pm_qos_cpu_dma_latency_nb.notifier_call = pm_qos_cpu_dma_latency_notify;
	power_debug_info.pm_qos_memory_bandwidth_nb.notifier_call = pm_qos_memory_bandwidth_notify;
	power_debug_info.pm_qos_cpu_memory_bandwidth_nb.notifier_call = pm_qos_cpu_memory_bandwidth_notify;
	power_debug_info.pm_qos_gpu_memory_bandwidth_nb.notifier_call = pm_qos_gpu_memory_bandwidth_notify;
	power_debug_info.pm_qos_mm_memory_bandwidth_nb.notifier_call = pm_qos_mm_memory_bandwidth_notify;
	power_debug_info.pm_qos_other_memory_bandwidth_nb.notifier_call = pm_qos_other_memory_bandwidth_notify;
	power_debug_info.pm_qos_ddr_opp_bandwidth_nb.notifier_call = pm_qos_ddr_opp_bandwidth_notify;
	power_debug_info.pm_qos_vcore_opp_bandwidth_nb.notifier_call = pm_qos_vcore_opp_bandwidth_notify;
	power_debug_info.pm_qos_scp_vcore_request_print_nb.notifier_call = pm_qos_scp_vcore_request_print_notify;
	power_debug_info.pm_qos_power_model_ddr_request_print_nb.notifier_call = pm_qos_power_model_ddr_request_print_notify;
	power_debug_info.pm_qos_power_model_vcore_request_print_nb.notifier_call = pm_qos_power_model_vcore_request_print_notify;
	power_debug_info.pm_qos_vcore_dvfs_force_opp_print_nb.notifier_call = pm_qos_vcore_dvfs_force_opp_print_notify;
	power_debug_info.pm_qos_isp_hrt_bandwidth_nb.notifier_call = pm_qos_isp_hrt_bandwidth_notify;
	power_debug_info.pm_qos_apu_memory_bandwidth_nb.notifier_call = pm_qos_apu_memory_bandwidth_notify;

	pm_qos_add_notifier(PM_QOS_CPU_DMA_LATENCY, &power_debug_info.pm_qos_cpu_dma_latency_nb);
	pm_qos_add_notifier(PM_QOS_MEMORY_BANDWIDTH, &power_debug_info.pm_qos_memory_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_CPU_MEMORY_BANDWIDTH, &power_debug_info.pm_qos_cpu_memory_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_GPU_MEMORY_BANDWIDTH, &power_debug_info.pm_qos_gpu_memory_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_MM_MEMORY_BANDWIDTH, &power_debug_info.pm_qos_mm_memory_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_OTHER_MEMORY_BANDWIDTH, &power_debug_info.pm_qos_other_memory_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_DDR_OPP, &power_debug_info.pm_qos_ddr_opp_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_VCORE_OPP, &power_debug_info.pm_qos_vcore_opp_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_SCP_VCORE_REQUEST, &power_debug_info.pm_qos_scp_vcore_request_print_nb);
	pm_qos_add_notifier(PM_QOS_POWER_MODEL_DDR_REQUEST, &power_debug_info.pm_qos_power_model_ddr_request_print_nb);
	pm_qos_add_notifier(PM_QOS_POWER_MODEL_VCORE_REQUEST, &power_debug_info.pm_qos_power_model_vcore_request_print_nb);
	pm_qos_add_notifier(PM_QOS_VCORE_DVFS_FORCE_OPP, &power_debug_info.pm_qos_vcore_dvfs_force_opp_print_nb);
	pm_qos_add_notifier(PM_QOS_ISP_HRT_BANDWIDTH, &power_debug_info.pm_qos_isp_hrt_bandwidth_nb);
	pm_qos_add_notifier(PM_QOS_APU_MEMORY_BANDWIDTH, &power_debug_info.pm_qos_apu_memory_bandwidth_nb);
}

static void print_heavy_loads_tasks()
{
	int i = 0;
	for (; i < NUM_PRINT_HEAVY_LOAD_TASK; i++) {
		if (!cpustats_saved[i].pwr)
			break;
		POWER_TUNER_INFO("%s\t%u\t%u\t%u\n", cpustats_saved[i].comm, cpustats_saved[i].tgid, cpustats_saved[i].pwr, cpustats_saved[i].lcore_pwr);
	}
}

static int cpustats_saved_comp(const void *a, const void *b)
{
	const struct task_stat sa = *(const struct task_stat *)a;
	const struct task_stat sb = *(const struct task_stat *)b;
	return sb.pwr - sa.pwr;
}
static void parse_task_stats()
{
	int i = 0, num = 0;

	while (i < MAX_PID && num <MAX_PID) {
		while (num < MAX_PID && cpustats_saved[num].pwr)
			num++;
		while (i < MAX_PID && !cpustats_saved[i].pwr)
			i++;
		if (i < num) {
			i = num + 1;
			continue;
		}
		if (num < MAX_PID && i < MAX_PID) {
			cpustats_saved[num] = cpustats_saved[i];
			memset(&cpustats_saved[i], 0, sizeof(struct task_stat));
		}
		i++;
		num++;
	}
	if (num >= MAX_PID)
		num = MAX_PID - 1;
	sort(cpustats_saved, num, sizeof(struct task_stat), cpustats_saved_comp, NULL);
}

static void task_power_stats()
{
	int i, j;
	unsigned long begin = jiffies - CTP_WINDOW_SZ * HZ, end = jiffies;
	unsigned int tmp_pwr;
	memset(cpustats_saved, 0, sizeof(cpustats_saved));
	for_each_possible_cpu(i) {
		struct kernel_task_cpustat* kstat = &per_cpu(ktask_cpustat, i);
		for (j = 0; j < MAX_CTP_WINDOW; j++) {
			struct task_cpustat *ts = kstat->cpustat + j;
			unsigned long r_time = ts->end - ts->begin;
			if (ts->pid >= MAX_PID)
				continue;
			if (ts->begin >= begin && ts->end <= end) {
				struct task_stat *as = cpustats_saved + ts->pid;
				if (as->pwr == 0)
					memcpy(as->comm, ts->comm, TASK_COMM_LEN);
				/* 4 ms each tick */
#ifdef CONFIG_MTK_UNIFY_POWER
				tmp_pwr = get_power(i, ts->cap) * jiffies_to_msecs(r_time);
#else
				tmp_pwr = get_power(i, ts->freq) * jiffies_to_msecs(r_time);
#endif

#ifdef CONFIG_THREAD_INFO_IN_TASK
				if (ts->l_core)
					as->lcore_pwr += tmp_pwr;
#endif
				as->pwr += tmp_pwr;
				as->tgid = ts->tgid;
			}
		}
	}
	parse_task_stats();
	print_heavy_loads_tasks();
}

static inline unsigned long get_capacity_ac_freq(int cpu, unsigned long freq)
{
	int i;
	int tmp;
	int idx = -1;
	struct upower_tbl *tbl;

	tbl = upower_get_core_tbl(cpu);
	for (i = 0; i < tbl->row_num ; i++) {
		tmp = mt_cpufreq_get_cpu_freq(cpu, i);
		if (unlikely(!tmp)) {
			//WARN(1, "frequency not in opp table!\n");
			idx = tbl->row_num - 1;
			break;
		}
		if (tmp >= freq) {
			idx = i;
			break;
		}
	}

	if (idx < 0)
		idx = tbl->row_num - 1;

	return tbl->row[idx].cap;
}

static void debug_power_timer_func(unsigned long data)
{
	unsigned int load_rel = 0, load_abs = 0, gpu_load = 0;
	unsigned int cpu_freq = 0, gpu_freq = 0;
	int cpu;
	struct tick_sched *ts;

	if (is_enter_suspend)
		return;

	mtk_get_gpu_loading(&gpu_load);
	gpu_freq = mt_gpufreq_get_cur_freq() / 1000;
	for_each_possible_cpu(cpu) {
		cpu_freq = cpufreq_quick_get(cpu) / 1000;
		ts = tick_get_tick_sched(cpu);
		sched_get_percpu_load2(cpu, 0, &load_rel, &load_abs);
		POWER_TUNER_INFO("cpu_info(%d): %d(idle)\t%dMhz\t%u(rel)\t%u(abs)\t%d(util)\t%d(cap)\n",
			cpu, ts->idle_active, cpu_freq, load_rel, load_abs, boosted_cpu_util(cpu), get_capacity_ac_freq(cpu, cpu_freq));
	}
	POWER_TUNER_INFO("gpu_dram_info %uMhz(gpu)\t%d(gpu_load)\t%d(ddr opp)\t%d(vcore opp)\t%d(uv)\n",
		gpu_freq, gpu_load, get_cur_ddr_opp(), get_cur_vcore_opp(), get_cur_vcore_uv());

	task_power_stats();
	POWER_TUNER_INFO("print active wakeup sources\n");
	pm_print_active_wakeup_sources();
	mod_timer(&power_debug_info.debug_power_timer, jiffies + debug_power_timer_interval_s * HZ);

}

static int debug_power_pm_event_func(struct notifier_block *notifier, unsigned long pm_event, void *unused)
{
	switch (pm_event) {
		case PM_HIBERNATION_PREPARE:
		case PM_RESTORE_PREPARE:
		case PM_POST_HIBERNATION:
			return NOTIFY_DONE;
		case PM_SUSPEND_PREPARE:
			is_enter_suspend = true;
			POWER_TUNER_INFO("SUSPEND DETECTED\n");
			del_timer_sync(&power_debug_info.debug_power_timer);
			return NOTIFY_DONE;
		case PM_POST_SUSPEND:
			is_enter_suspend = false;
			POWER_TUNER_INFO("RESUME DETECTED\n");
			if (timer_pending(&power_debug_info.debug_power_timer))
				POWER_TUNER_ERR("timer error after suspend\n");
			else
				mod_timer(&power_debug_info.debug_power_timer, jiffies + debug_power_timer_interval_s * HZ);
			return NOTIFY_DONE;
	}
	return NOTIFY_DONE;
}

static ssize_t power_debug_interval_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", debug_power_timer_interval_s);
}
static ssize_t power_debug_interval_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n)
{
	unsigned int val;
	int err;
	err = -EINVAL;
	if (sscanf(buf, "%u", &val) == 1) {
		if (val > POWER_DEBUG_TIME_INTERVAL_MAX || val < POWER_DEBUG_TIME_INTERVAL_MIN) {
			POWER_TUNER_ERR("power_debug_interval out of limit(%d %d)\n", POWER_DEBUG_TIME_INTERVAL_MIN, POWER_DEBUG_TIME_INTERVAL_MAX);
			return err;
		}
		debug_power_timer_interval_s = val;
		POWER_TUNER_INFO("power_debug_interval saved to %u\n", debug_power_timer_interval_s);
		err = n;
	}
	else
		POWER_TUNER_ERR("Failed to save power_debug_interval.\n");
	return err;
}
power_debug_attr(power_debug_interval);

static ssize_t pm_qos_flag_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%u\n", pm_qos_debug_flag);
}
static ssize_t pm_qos_flag_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n)
{
	unsigned int val;
	int err;
	err = -EINVAL;
	if (sscanf(buf, "%u", &val) == 1) {
		if (val > QOS_FLAG_NUM_CLASSES) {
			POWER_TUNER_ERR("pm_qos_debug_flag out of limit\n");
			return err;
		}
		pm_qos_debug_flag = val;
		POWER_TUNER_INFO("pm_qos_debug_flag saved to %u\n", pm_qos_debug_flag);
		err = n;
	}
	else
		POWER_TUNER_ERR("Failed to save pm_qos_debug_flag.\n");
	return err;
}
power_debug_attr(pm_qos_flag);

static struct attribute * power_debug[] = {
	&power_debug_interval_attr.attr,
	&pm_qos_flag_attr.attr,
	NULL,
};

static const struct attribute_group power_debug_attr_group = {
	.attrs = power_debug,
};

static int __init init_debug_power(void)
{
	int res = 0;
	power_debug_info.debug_load_pm_event.notifier_call = debug_power_pm_event_func;
	setup_timer(&power_debug_info.debug_power_timer, debug_power_timer_func, (unsigned long)&power_debug_info.debug_power_timer);
	pm_qos_add_debug_notifiers();
#ifdef CONFIG_PM
	res = register_pm_notifier(&power_debug_info.debug_load_pm_event);
	if (res) {
		POWER_TUNER_ERR("Failed to register PM notifier.\n");
		return res;
	}
#endif

	mod_timer(&power_debug_info.debug_power_timer, jiffies + SYSTEM_POWER_ON_TIME * HZ);//fixme

	res = sysfs_create_group(power_kobj, &power_debug_attr_group);
	if (res)
		return res;
	return 0;
}

late_initcall(init_debug_power);
