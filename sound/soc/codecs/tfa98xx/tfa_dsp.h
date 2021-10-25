/*
 * Copyright (C) 2014 NXP Semiconductors, All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

/*
 *internal functions for TFA layer (not shared with SRV and HAL layer!)
 */

#ifndef __TFA_DSP_H__
#define __TFA_DSP_H__

#ifdef VENDOR_EDIT
/*Jianfeng.Qiu@PSW.MM.AudioDriver.FTM.1226731, 2018/05/12, Add for FTM*/
extern int ftm_mode;
extern char ftm_SpeakerCalibration[17];
extern char ftm_spk_resistance[24];

#ifndef BOOT_MODE_FACTORY
#define BOOT_MODE_FACTORY 3
#endif

/* xiang.fei@PSW.MM.AudioDriver.Codec, 2018/03/12,
 * Add for speaker resistance
 */
bool g_speaker_resistance_fail;
#endif /* VENDOR_EDIT */

#endif /* __TFA_DSP_H__ */

