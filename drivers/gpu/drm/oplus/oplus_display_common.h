/***************************************************************
** Copyright (C),  2020,  OPPO Mobile Comm Corp.,  Ltd
** VENDOR_EDIT
** File : oppo_display_common.c
** Description : oppo common feature
** Version : 1.0
** Date : 2020/07/1
** Author : JianBin.Zhang@MM.Display.LCD Driver
**
** ------------------------------- Revision History: -----------
**  <author>        <data>        <version >        <desc>
**  JianBin.Zhang   2020/07/01        1.0           Build this moudle
******************************************************************/
#ifndef _OPPO_DISPLAY_COMMON_H_
#define _OPPO_DISPLAY_COMMON_H_

#include <oppo_display_private_api.h>

int oplus_display_panel_get_max_brightness(void *buf);
int oplus_display_panel_get_serial_number(void *buf);
int oplus_display_panel_set_closebl_flag(void *buf);
int oplus_display_panel_get_closebl_flag(void *buf);
int oplus_display_get_brightness(void *buf);
int oplus_display_set_brightness(void *buf);
int oplus_display_panel_set_cabc(void *buf);
int oplus_display_panel_get_cabc(void *buf);
int oplus_display_panel_set_esd(void *buf);
int oplus_display_panel_get_esd(void *buf);
#endif /*_OPPO_DISPLAY_COMMON_H_*/
