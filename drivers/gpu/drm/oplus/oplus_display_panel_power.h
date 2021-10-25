/***************************************************************
** Copyright (C),  2020,  OPPO Mobile Comm Corp.,  Ltd
** VENDOR_EDIT
** File : oppo_display_panel_power.h
** Description : oppo display panel power control
** Version : 1.0
** Date : 2020/06/13
** Author : Li.Sheng@MULTIMEDIA.DISPLAY.LCD
**
** ------------------------------- Revision History: -----------
**  <author>        <data>        <version >        <desc>
**  Li.Sheng       2020/06/13        1.0           Build this moudle
******************************************************************/
#ifndef _OPPO_DISPLAY_PANEL_POWER_H_
#define _OPPO_DISPLAY_PANEL_POWER_H_

#include <linux/err.h>

enum PANEL_VOLTAGE_ENUM {
	PANEL_VOLTAGE_ID_VDDI = 0,
	PANEL_VOLTAGE_ID_VDDR,
	PANEL_VOLTAGE_ID_VG_BASE,
	PANEL_VOLTAGE_ID_MAX,
};

#define PANEL_VOLTAGE_VALUE_COUNT 4

typedef struct panel_voltage_bak {
	u32 voltage_id;
	u32 voltage_min;
	u32 voltage_current;
	u32 voltage_max;
	char pwr_name[20];
}PANEL_VOLTAGE_BAK;

int oplus_panel_set_vg_base(unsigned int panel_vol_value);

#endif /*_OPPO_DISPLAY_PANEL_POWER_H_*/
