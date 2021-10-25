/***************************************************************
** Copyright (C),  2020,  OPPO Mobile Comm Corp.,  Ltd
** VENDOR_EDIT
** File : oppo_display_onscreenfingerprint.h
** Description : oppo_display_onscreenfingerprint. implement
** Version : 1.0
** Date : 2020/05/13
** Author : JianBin.Zhang@PSW.MM.Display.Stability
**
** ------------------------------- Revision History: -----------
**  <author>        <data>        <version >        <desc>
**   Zhang.JianBin2020/05/13        1.0          Modify for MT6779_R
******************************************************************/
#ifndef _OPPO_DISPLAY_ONSCREENFINGERPRINT_H_
#define _OPPO_DISPLAY_ONSCREENFINGERPRINT_H_

#include <linux/err.h>
#include <linux/list.h>
#include <linux/of.h>
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/leds.h>
#include <linux/types.h>

int oplus_display_panel_set_finger_print(void *buf);

#endif /*_OPPO_DISPLAY_ONSCREENFINGERPRINT_H_*/

