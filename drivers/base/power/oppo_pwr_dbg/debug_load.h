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
#define QOS_FLAG_CPU_DMA_LATENCY	(1 << PM_QOS_CPU_DMA_LATENCY)
#define QOS_FLAG_NETWORK_LATENC		(1 << PM_QOS_NETWORK_LATENCY)
#define QOS_FLAG_NETWORK_THROUGHPUT	(1 << PM_QOS_NETWORK_THROUGHPUT)
#define QOS_FLAG_MEMORY_BANDWIDTH	(1 << PM_QOS_MEMORY_BANDWIDTH)
#define QOS_FLAG_CPU_MEMORY_BANDWIDTH	(1 << PM_QOS_CPU_MEMORY_BANDWIDTH)
#define QOS_FLAG_GPU_MEMORY_BANDWIDTH	(1 << PM_QOS_GPU_MEMORY_BANDWIDTH)
#define QOS_FLAG_MM_MEMORY_BANDWIDTH	(1 << PM_QOS_MM_MEMORY_BANDWIDTH)
#define QOS_FLAG_MD_PERI_MEMORY_BANDWIDTH	(1 << PM_QOS_MD_PERI_MEMORY_BANDWIDTH)

#define QOS_FLAG_OTHER_MEMORY_BANDWIDTH	(1 << PM_QOS_OTHER_MEMORY_BANDWIDTH)
#define QOS_FLAG_MM0_BANDWIDTH_LIMITER	(1 << PM_QOS_MM0_BANDWIDTH_LIMITER)
#define QOS_FLAG_MM1_BANDWIDTH_LIMITER	(1 << PM_QOS_MM1_BANDWIDTH_LIMITER)
#define QOS_FLAG_DDR_OPP	(1 << PM_QOS_DDR_OPP)
#define QOS_FLAG_EMI_OPP	(1 << PM_QOS_EMI_OPP)
#define QOS_FLAG_VCORE_OPP	(1 << PM_QOS_VCORE_OPP)
#define QOS_FLAG_VCORE_DVFS_FIXED_OPP	(1 << PM_QOS_VCORE_DVFS_FIXED_OPP)
#define QOS_FLAG_SCP_VCORE_REQUEST	(1 << PM_QOS_SCP_VCORE_REQUEST)

#define QOS_FLAG_POWER_MODEL_DDR_REQUEST	(1 << PM_QOS_POWER_MODEL_DDR_REQUEST)
#define QOS_FLAG_POWER_MODEL_VCORE_REQUEST	(1 << PM_QOS_POWER_MODEL_VCORE_REQUEST)
#define QOS_FLAG_VCORE_DVFS_FORCE_OPP	(1 << PM_QOS_VCORE_DVFS_FORCE_OPP)
#define QOS_FLAG_DISP_FREQ	(1 << PM_QOS_DISP_FREQ)
#define QOS_FLAG_MDP_FREQ	(1 << PM_QOS_MDP_FREQ)
#define QOS_FLAG_VDEC_FREQ	(1 << PM_QOS_VDEC_FREQ)
#define QOS_FLAG_VENC_FREQ	(1 << PM_QOS_VENC_FREQ)
#define QOS_FLAG_IMG_FREQ	(1 << PM_QOS_IMG_FREQ)

#define QOS_FLAG_CAM_FREQ	(1 << PM_QOS_CAM_FREQ)
#define QOS_FLAG_DPE_FREQ	(1 << PM_QOS_DPE_FREQ)
#define QOS_FLAG_ISP_HRT_BANDWIDTH	(1 << PM_QOS_ISP_HRT_BANDWIDTH)
#define QOS_FLAG_APU_MEMORY_BANDWIDTH	(1 << PM_QOS_APU_MEMORY_BANDWIDTH)
#define QOS_FLAG_VVPU_OPP	(1 << PM_QOS_VVPU_OPP)
#define QOS_FLAG_NUM_CLASSES	(1 << PM_QOS_NUM_CLASSES)

#define power_debug_attr(_name) \
static struct kobj_attribute _name##_attr = {	\
	.attr	= {				\
		.name = __stringify(_name),	\
		.mode = 0644,			\
	},					\
	.show	= _name##_show,			\
	.store	= _name##_store,		\
}


extern int is_heavy_task(struct task_struct *p);
extern unsigned int mt_gpufreq_get_cur_freq(void);
extern unsigned int mt_cpufreq_get_cpu_freq(int cpu, int idx);
extern void sched_get_percpu_load2(int cpu, bool reset, unsigned int *rel_load, unsigned int *abs_load);
extern unsigned int mtk_dramc_get_data_rate(void); /* in Mhz */
extern bool mtk_get_gpu_loading(unsigned int *pLoading);
extern unsigned long boosted_cpu_util(int cpu);
extern void pm_qos_trace_dbg_dump(int pm_qos_class);
#ifdef CONFIG_MTK_UNIFY_POWER
extern int get_power(int cpu, unsigned long cap);
#else
extern int get_power(int cpu, int freq)
#endif
extern void pm_print_active_wakeup_sources(void);

struct task_stat {
	pid_t tgid;
	unsigned int pwr;
	unsigned int lcore_pwr;
	char comm[TASK_COMM_LEN];
};

struct power_debug {
	struct notifier_block	pm_qos_cpu_dma_latency_nb;
	struct notifier_block	pm_qos_memory_bandwidth_nb;
	struct notifier_block	pm_qos_cpu_memory_bandwidth_nb;
	struct notifier_block	pm_qos_gpu_memory_bandwidth_nb;
	struct notifier_block	pm_qos_mm_memory_bandwidth_nb;
	struct notifier_block	pm_qos_other_memory_bandwidth_nb;
	struct notifier_block	pm_qos_ddr_opp_bandwidth_nb;
	struct notifier_block	pm_qos_vcore_opp_bandwidth_nb;
	struct notifier_block	pm_qos_scp_vcore_request_print_nb;
	struct notifier_block	pm_qos_power_model_ddr_request_print_nb;
	struct notifier_block	pm_qos_power_model_vcore_request_print_nb;
	struct notifier_block	pm_qos_vcore_dvfs_force_opp_print_nb;
	struct notifier_block	pm_qos_isp_hrt_bandwidth_nb;
	struct notifier_block	pm_qos_apu_memory_bandwidth_nb;

	struct notifier_block 	debug_load_pm_event;
	struct timer_list 		debug_power_timer;
};
