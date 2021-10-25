/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_SCHED_CPUFREQ_H
#define _LINUX_SCHED_CPUFREQ_H

#include <linux/types.h>

/*
 * Interface between cpufreq drivers and the scheduler:
 */

#define SCHED_CPUFREQ_RT	(1U << 0)
#define SCHED_CPUFREQ_DL	(1U << 1)
#define SCHED_CPUFREQ_IOWAIT	(1U << 2)

#if defined (CONFIG_SCHED_WALT)  && defined (OPLUS_FEATURE_UIFIRST)
#define SCHED_CPUFREQ_BOOST (1U << 9)
#endif /* OPLUS_FEATURE_UIFIRST */

#ifdef CONFIG_CPU_FREQ
struct update_util_data {
       void (*func)(struct update_util_data *data, u64 time, unsigned int flags);
};

void cpufreq_add_update_util_hook(int cpu, struct update_util_data *data,
                       void (*func)(struct update_util_data *data, u64 time,
				    unsigned int flags));
void cpufreq_remove_update_util_hook(int cpu);
#endif /* CONFIG_CPU_FREQ */

#endif /* _LINUX_SCHED_CPUFREQ_H */
