/***************************************************************
** Copyright (C),  2020,  OPPO Mobile Comm Corp.,  Ltd
** VENDOR_EDIT
** File : oppo_display_dc.c
** Description : oppo dc feature
** Version : 1.0
** Date : 2020/07/1
** Author : JianBin.Zhang@MM.Display.LCD Driver
**
** ------------------------------- Revision History: -----------
**  <author>        <data>        <version >        <desc>
**  JianBin.Zhang   2020/07/01        1.0           Build this moudle
******************************************************************/
#ifndef _OPPO_DISPLAY_DC_H_
#define _OPPO_DISPLAY_DC_H_
#include <linux/err.h>
#include <linux/of.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>

int oppo_display_panel_set_dc_alpha(void *buf);
int oppo_display_panel_get_dc_alpha(void *buf);
int oplus_display_panel_get_dimlayer_enable(void *buf);
int oplus_display_panel_set_dimlayer_enable(void *buf);
int oplus_display_panel_set_dim_alpha(void *buf);
int oplus_display_panel_get_dim_alpha(void *buf);
int oplus_display_panel_set_dim_dc_alpha(void *buf);
int oplus_display_panel_get_dim_dc_alpha(void *buf);
#endif /*_OPPO_DISPLAY_DC_H_*/
