/*
 * Copyright (C) 2017 MediaTek Inc.
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
#ifndef __IMGSENSOR_HWCFG_mt6771_CTRL_H__
#define __IMGSENSOR_HWCFG_mt6771_CTRL_H__
#include "imgsensor_hwcfg_custom.h"


struct IMGSENSOR_SENSOR_LIST *oplus_gimgsensor_sensor_list = NULL;
struct IMGSENSOR_HW_CFG *oplus_imgsensor_custom_config = NULL;
struct IMGSENSOR_HW_POWER_SEQ *oplus_sensor_power_sequence = NULL;
struct IMGSENSOR_HW_POWER_SEQ *oplus_platform_power_sequence = NULL;
struct CAMERA_DEVICE_INFO gImgEepromInfo;

struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_18531[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(S5K3P9SP_MIPI_RAW)
	{S5K3P9SP_SENSOR_ID, SENSOR_DRVNAME_S5K3P9SP_MIPI_RAW,S5K3P9SP_MIPI_RAW_SensorInit},
#endif
#if defined(S5KGM1SP_MIPI_RAW)
	{S5KGM1SP_SENSOR_ID, SENSOR_DRVNAME_S5KGM1SP_MIPI_RAW, S5KGM1SP_MIPI_RAW_SensorInit},
#endif
#if defined(GC5035_MIPI_RAW)
	{GC5035_SENSOR_ID, SENSOR_DRVNAME_GC5035_MIPI_RAW, GC5035_MIPI_RAW_SensorInit},
#endif
#if defined(IMX398_MIPI_RAW)
	{IMX398_SENSOR_ID, SENSOR_DRVNAME_IMX398_MIPI_RAW, IMX398_MIPI_RAW_SensorInit},
#endif
#if defined(GC2375H_MIPI_RAW)
	{GC2375H_SENSOR_ID, SENSOR_DRVNAME_GC2375H_MIPI_RAW, GC2375H_MIPI_RAW_SensorInit},
#endif
    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};
struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_19531[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(S5K3P9SP_MIPI_RAW)
        {S5K3P9SP_SENSOR_ID, SENSOR_DRVNAME_S5K3P9SP_MIPI_RAW,S5K3P9SP_MIPI_RAW_SensorInit},
#endif
#if defined(S5KGM1SP_MIPI_RAW)
        {S5KGM1SP_SENSOR_ID, SENSOR_DRVNAME_S5KGM1SP_MIPI_RAW, S5KGM1SP_MIPI_RAW_SensorInit},
#endif
#if defined(OV02A10_MIPI_RAW2)
        {OV02A10_SENSOR_ID2, SENSOR_DRVNAME_OV02A10_MIPI_RAW2, OV02A10_MIPI_RAW2_SensorInit},
#endif
#if defined(OV02A10_MIPI_RAW1)
        {OV02A10_SENSOR_ID, SENSOR_DRVNAME_OV02A10_MIPI_RAW1, OV02A10_MIPI_RAW1_SensorInit},
#endif
#if defined(HI846_MIPI_RAW2)
        {HI846_SENSOR_ID, SENSOR_DRVNAME_HI846_MIPI_RAW2, HI846_MIPI_RAW2_SensorInit},
#endif
#if defined(GC02M0_MIPI_MONO1)
        {GC02M0_SENSOR_ID1, SENSOR_DRVNAME_GC02M0_MIPI_MONO1, GC02M0_MIPI_MONO1_SensorInit},
#endif
#if defined(GC02M0_MIPI_MONO)
        {GC02M0_SENSOR_ID, SENSOR_DRVNAME_GC02M0_MIPI_MONO, GC02M0_MIPI_MONO_SensorInit},
#endif
    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

struct IMGSENSOR_HW_CFG imgsensor_custom_config_18531[] = {
	{
		IMGSENSOR_SENSOR_IDX_MAIN,
		IMGSENSOR_I2C_DEV_0,
		{
			{IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
			{IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
		},
	},
	{
		IMGSENSOR_SENSOR_IDX_SUB,
		IMGSENSOR_I2C_DEV_1,
		{
			{IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
			{IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
		},
	},
	{
		IMGSENSOR_SENSOR_IDX_MAIN2,
		IMGSENSOR_I2C_DEV_2,
		{
			{IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
			{IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
		},
	},
	{
		IMGSENSOR_SENSOR_IDX_SUB2,
		IMGSENSOR_I2C_DEV_1,
		{
			{IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
			{IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
		},
	},
	{
		IMGSENSOR_SENSOR_IDX_MAIN3,
		IMGSENSOR_I2C_DEV_1,
		{
			{IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
			{IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
			{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
			{IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
		},
	},

	{IMGSENSOR_SENSOR_IDX_NONE}
};
/*weiriqin@Camera.Driver add for 19531 19391 board 20190605*/
struct IMGSENSOR_HW_CFG imgsensor_custom_config_19531[] = {
        {
                IMGSENSOR_SENSOR_IDX_MAIN,
                IMGSENSOR_I2C_DEV_0,
                {
                        {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                        {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
                },
        },
        {
                IMGSENSOR_SENSOR_IDX_SUB,
                IMGSENSOR_I2C_DEV_1,
                {
                        {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                        {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_MIPI_SWITCH_EN, IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
                },
        },
        {
                IMGSENSOR_SENSOR_IDX_MAIN2,
                IMGSENSOR_I2C_DEV_2,
                {
                        {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                        {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_MIPI_SWITCH_EN, IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
                },
        },
        {
                IMGSENSOR_SENSOR_IDX_SUB2,
                IMGSENSOR_I2C_DEV_1,
                {
                        {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                        {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
                },
        },
        {
                IMGSENSOR_SENSOR_IDX_MAIN3,
                IMGSENSOR_I2C_DEV_2,
                {
                        {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                        {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
                        {IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                        {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
                },
        },
        {IMGSENSOR_SENSOR_IDX_NONE}
};
/*weiriqin@Camera.Driver add for 10531 19391 special mipi switch 20191204*/
struct IMGSENSOR_HW_POWER_SEQ platform_power_sequence_19531[] = {
	{
		PLATFORM_POWER_SEQ_NAME,
		{
			{
				IMGSENSOR_HW_PIN_MIPI_SWITCH_EN,
                                IMGSENSOR_HW_PIN_STATE_LEVEL_HIGH,
				0,
				IMGSENSOR_HW_PIN_STATE_LEVEL_0,
				0
			},
		},
		IMGSENSOR_SENSOR_IDX_SUB,
	},
	{
		PLATFORM_POWER_SEQ_NAME,
		{
			{
				IMGSENSOR_HW_PIN_MIPI_SWITCH_EN,
				IMGSENSOR_HW_PIN_STATE_LEVEL_0,
				0,
				IMGSENSOR_HW_PIN_STATE_LEVEL_0,
				0
			},
		},
		IMGSENSOR_SENSOR_IDX_MAIN2,
	},
	{NULL}
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_18531[] = {
#if defined(S5K3P9SP_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5K3P9SP_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{RST, Vol_Low, 1},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1100, 0},
			{DOVDD, Vol_1800, 0},
			{AFVDD, Vol_2800, 5},
			{RST, Vol_High, 2},
		},
	},
#endif
#if defined(S5KGM1SP_MIPI_RAW)
	{
		SENSOR_DRVNAME_S5KGM1SP_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 0},
			{RST, Vol_Low, 1},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1100, 0},
			{DOVDD, Vol_1800, 0},
			{AFVDD, Vol_2800, 5},
			{RST, Vol_High, 2},
		},
	},
#endif
#if defined(GC5035_MIPI_RAW)
	{
		SENSOR_DRVNAME_GC5035_MIPI_RAW,
		{
			{SensorMCLK, Vol_High, 1},
			{RST, Vol_Low, 1},
			{DOVDD, Vol_1800, 1},
			{DVDD, Vol_1200, 1},
			{AVDD, Vol_2800, 1},
			{RST, Vol_High, 10}
		},
	},
#endif
#if defined(GC2375H_MIPI_RAW)
        {
                SENSOR_DRVNAME_GC2375H_MIPI_RAW,
                {
                        /*GC2375's PWDN is linked to GPIO RST on 18561 board,
                        so RST is set Vol_High first here*/
                        {RST, Vol_High, 20},
                        {DOVDD, Vol_1800, 5},
                        {AVDD, Vol_2800, 1},
                        {SensorMCLK, Vol_High, 2},
                        {RST, Vol_Low, 2, Vol_High}
                },
        },
#endif
#if defined(IMX398_MIPI_RAW)
	{
		SENSOR_DRVNAME_IMX398_MIPI_RAW,
		{
			{RST, Vol_Low, 1},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1100, 1},
			{SensorMCLK, Vol_High, 1},
			{DOVDD, Vol_1800, 0},
			{AFVDD, Vol_2800, 5},
			{RST, Vol_High, 2},
		},
	},
#endif
    /* add new sensor before this line */
    {NULL,},
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_19531[] = {
#if defined(S5K3P9SP_MIPI_RAW)
        {
                SENSOR_DRVNAME_S5K3P9SP_MIPI_RAW,
                {
                        {SensorMCLK, Vol_High, 0},
                        {RST, Vol_Low, 1},
                        {AVDD, Vol_2800, 0},
                        {DVDD, Vol_1100, 0},
                        {DOVDD, Vol_1800, 0},
                        {AFVDD, Vol_2800, 5},
                        {RST, Vol_High, 2},
                },
        },
#endif
#if defined(S5KGM1SP_MIPI_RAW)
        {
                SENSOR_DRVNAME_S5KGM1SP_MIPI_RAW,
                {
                        {SensorMCLK, Vol_High, 0},
                        {RST, Vol_Low, 1},
                        {AVDD, Vol_2800, 0},
                        {DVDD, Vol_1100, 0},
                        {DOVDD, Vol_1800, 0},
                        {AFVDD, Vol_2800, 5},
                        {RST, Vol_High, 2},
                },
        },
#endif
#if defined(OV02A10_MIPI_RAW2)
        {
                SENSOR_DRVNAME_OV02A10_MIPI_RAW2,
                {
			{RST, Vol_High, 1, Vol_High, 1},
			{DOVDD, Vol_1800, 2},
			{AVDD, Vol_2800, 5},
			//{DVDD, Vol_1200, 1},
			{RST, Vol_High, 1, Vol_High, 1},
			{SensorMCLK, Vol_High, 2},
			{RST, Vol_Low, 10}
                },
        },
#endif
#if defined(OV02A10_MIPI_RAW1)
        {
                SENSOR_DRVNAME_OV02A10_MIPI_RAW1,
                {
			{RST, Vol_High, 1, Vol_High, 1},
			{DOVDD, Vol_1800, 2},
			{AVDD, Vol_2800, 5},
			//{DVDD, Vol_1200, 1},
			{RST, Vol_High, 1, Vol_High, 1},
			{SensorMCLK, Vol_High, 2},
			{RST, Vol_Low, 10}
                },
        },
#endif
/*weiriqin@Camera.Driver add for 19531*/
#if defined(HI846_MIPI_RAW2)
	{
		SENSOR_DRVNAME_HI846_MIPI_RAW2,
		{
                        {DOVDD, Vol_1800, 1},
                        {AVDD, Vol_2800, 1},
                        {DVDD, Vol_1200, 5},
                        {SensorMCLK, Vol_High, 0},
                        {RST, Vol_Low, 5},
                        {RST, Vol_High, 10},
		},
	},
#endif
#if defined(GC02M0_MIPI_MONO)
	{
		SENSOR_DRVNAME_GC02M0_MIPI_MONO,
		{
			{DOVDD, Vol_1800, 2},
			//{DVDD, Vol_1800, 5},
			{AVDD, Vol_2800, 3},
			{RST, Vol_Low, 1},
			{SensorMCLK, Vol_High, 1},
			{RST, Vol_High, 1},
		},
	},
#endif
#if defined(GC02M0_MIPI_MONO1)
	{
		SENSOR_DRVNAME_GC02M0_MIPI_MONO1,
		{
			{DOVDD, Vol_1800, 2},
			//{DVDD, Vol_1800, 5},
			{AVDD, Vol_2800, 3},
			{RST, Vol_Low, 1},
			{SensorMCLK, Vol_High, 1},
			{RST, Vol_High, 1},
		},
	},
#endif
    /* add new sensor before this line */
    {NULL,},
};

struct CAMERA_DEVICE_INFO gImgEepromInfo_18531 = {
    .i4SensorNum = 3,
    .pCamModuleInfo = {
        {S5KGM1SP_SENSOR_ID ,  0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "s5kgm1sp"},
        {S5K3P9SP_SENSOR_ID,  0xA8, {0x00, 0x06}, 0xE0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f", "s5k3p9sp"},
        {GC5035_SENSOR_ID,  0xA0, {0x00, 0x06}, 0xE0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "gc5035"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {0xFFFF, 0xFFFF},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};

struct CAMERA_DEVICE_INFO gImgEepromInfo_18561 = {
    .i4SensorNum = 3,
    .pCamModuleInfo = {
        {IMX398_SENSOR_ID ,  0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "imx398"},
        {S5K3P9SP_SENSOR_ID,  0xA8, {0x00, 0x06}, 0xE0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f", "s5k3p9sp"},
        {GC2375H_SENSOR_ID,  0xA0, {0x00, 0x06}, 0xE0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "gc2375h"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {0x1640, 0x1020},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};

struct CAMERA_DEVICE_INFO gImgEepromInfo_19531 = {
    .i4SensorNum = 3,
    .pCamModuleInfo = {
        /*{sensor id, eeprom IIC Slave addr, i4CamInfoAddr[2], i4CamSNAddr, afSupport, i4AFCodeAddr[4], name, verison}*/
        {S5KGM1SP_SENSOR_ID ,  0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "s5kgm1sp"},
        {S5K3P9SP_SENSOR_ID,  0xA8, {0x00, 0x06}, 0xE0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f", "s5k3p9sp"},
        {HI846_SENSOR_ID,   0xA2, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "hi846"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {0xFFFF, 0xFFFF},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};
void oplus_imgsensor_hwcfg()
{
    if (is_project(OPPO_18531) || is_project(OPPO_18561)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_18531;
         oplus_imgsensor_custom_config = imgsensor_custom_config_18531;
         oplus_sensor_power_sequence = sensor_power_sequence_18531;
         gImgEepromInfo = gImgEepromInfo_18531;
         if (is_project(OPPO_18561)) {
            gImgEepromInfo = gImgEepromInfo_18561;
         }
    } else if (is_project(19531)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_19531;
         oplus_imgsensor_custom_config = imgsensor_custom_config_19531;
         oplus_sensor_power_sequence = sensor_power_sequence_19531;
         oplus_platform_power_sequence = platform_power_sequence_19531;
         gImgEepromInfo = gImgEepromInfo_19531;
    } else {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_18531;
         oplus_imgsensor_custom_config = imgsensor_custom_config_18531;
         oplus_sensor_power_sequence = NULL;
         gImgEepromInfo = gImgEepromInfo_18531;
    }
}
#endif
