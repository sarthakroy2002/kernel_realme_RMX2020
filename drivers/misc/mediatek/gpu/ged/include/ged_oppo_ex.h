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


#ifndef __GED_OPPO_EX__
#define __GED_OPPO_EX__

#include <linux/types.h>
#include <ged_type.h>

/**
 * struct oppo_opp_record - record of last dvfs status
 *
 * @loading:	    gpu loading
 * @oppidx:	        current gpu opp index
 * @last_timestamp:	timestamp of last time calling ged_dvfs_update_opp_cost
 *
 */
struct oppo_opp_record {
	unsigned int loading;
	unsigned int oppidx;
	unsigned long long last_timestamp;
};

/**
 * @brief update oppo opp record
 *
 * @param[in] oppo_record the parameter of oppo_opp_record.
 */
void oppo_update_opp_record(struct oppo_opp_record *opp_rec);


/**
 * @brief update ged_dvfs_update_opp_cost
 *
 * Return: 0 on success or error code
 */
int oppo_update_opp_cost(void);


/**
 * @brief update oppo init oppo rec
 *
 * @param[in] oppo_record the parameter of oppo_init_oppo_rec.
 */

int oppo_init_oppo_rec(unsigned int *oppsize);

#endif /* __GED_OPPO_EX__ */