/*
 * Copyright (C) 2015 MediaTek Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include <linux/string.h>
#include "ged_dvfs.h"
#include "ged_base.h"
#include "ged_oppo_ex.h"

struct oppo_opp_record g_oppo_rec;
int g_oppsize;
bool g_opp_init = false;

void oppo_update_opp_record(struct oppo_opp_record *opp_rec)
{
	if (opp_rec)
		memcpy(&g_oppo_rec, opp_rec, sizeof(struct oppo_opp_record));
}

int oppo_update_opp_cost(void)
{
	unsigned long long cur_time = ged_get_time();
	unsigned long long diff = (cur_time - g_oppo_rec.last_timestamp) >> 10;

	/* check the record is updated */
	if (true == g_opp_init) {
       	if (g_oppo_rec.oppidx  >= 0 && g_oppo_rec.oppidx < g_oppsize) {
			if ((g_oppo_rec.last_timestamp > 0) && (diff > 0)) {
				cur_time = cur_time >> 10;
				ged_dvfs_update_opp_cost(g_oppo_rec.loading, diff, cur_time, g_oppo_rec.oppidx);
			}
	    }
	}

	return 0;
}

int oppo_init_oppo_rec(unsigned int *oppsize) {
	if (oppsize) {
		g_opp_init = true;
	    g_oppo_rec.loading = 0;
	    g_oppo_rec.oppidx = -1;
	    g_oppo_rec.last_timestamp = 0;
	    g_oppsize = *oppsize;
	}
    return 0;
}