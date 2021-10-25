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
#include "oplus_display_onscreenfingerprint.h"

extern bool oppo_fp_notify_down_delay;
extern bool oppo_fp_notify_up_delay;

int oplus_display_panel_set_finger_print(void *buf)
{
	unsigned int *fingerprint_op_mode = buf;

	if ((*fingerprint_op_mode) == 1) {
		oppo_fp_notify_down_delay = true;
	} else {
		oppo_fp_notify_up_delay = true;
	}

	printk(KERN_ERR "%s receive uiready %d\n", __func__, (*fingerprint_op_mode));
	return 0;
}

/* #endif */ /* OPLUS_FEATURE_ONSCREENFINGERPRINT */
