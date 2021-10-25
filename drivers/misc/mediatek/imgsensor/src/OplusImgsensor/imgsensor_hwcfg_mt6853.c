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
#ifndef __IMGSENSOR_HWCFG_20075_CTRL_H__
#define __IMGSENSOR_HWCFG_20075_CTRL_H__
#include "imgsensor_hwcfg_custom.h"


struct IMGSENSOR_SENSOR_LIST *oplus_gimgsensor_sensor_list = NULL;
struct IMGSENSOR_HW_CFG *oplus_imgsensor_custom_config = NULL;
struct IMGSENSOR_HW_POWER_SEQ *oplus_sensor_power_sequence = NULL;
struct IMGSENSOR_HW_POWER_SEQ *oplus_platform_power_sequence = NULL;
struct CAMERA_DEVICE_INFO gImgEepromInfo;

struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_20075[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(OV48B_MIPI_RAW)
{OV48B_SENSOR_ID, SENSOR_DRVNAME_OV48B_MIPI_RAW, OV48B_MIPI_RAW_SensorInit},
#endif
#if defined(OV32A_MIPI_RAW)
{OV32A_SENSOR_ID, SENSOR_DRVNAME_OV32A_MIPI_RAW, OV32A_MIPI_RAW_SensorInit},
#endif
#if defined(HI846_MIPI_RAW)
{HI846_SENSOR_ID, SENSOR_DRVNAME_HI846_MIPI_RAW, HI846_MIPI_RAW_SensorInit},
#endif
#if defined(GC02K0_MIPI_RAW)
{GC02K0_SENSOR_ID, SENSOR_DRVNAME_GC02K0_MIPI_RAW, GC02K0_MIPI_RAW_SensorInit},
#endif

    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_20151[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(OV48B_MIPI_RAW)
{OV48B_SENSOR_ID, SENSOR_DRVNAME_OV48B_MIPI_RAW, OV48B_MIPI_RAW_SensorInit},
#endif
#if defined(S5KGM1ST_MIPI_RAW)
{S5KGM1ST_SENSOR_ID, SENSOR_DRVNAME_S5KGM1ST_MIPI_RAW, S5KGM1ST_MIPI_RAW_SensorInit},
#endif
#if defined(IMX471_MIPI_RAW)
{IMX471_SENSOR_ID, SENSOR_DRVNAME_IMX471_MIPI_RAW, IMX471_MIPI_RAW_SensorInit},
#endif
#if defined(HI846_MIPI_RAW)
{HI846_SENSOR_ID, SENSOR_DRVNAME_HI846_MIPI_RAW, HI846_MIPI_RAW_SensorInit},
#endif
#if defined(GC02K0_MIPI_RAW)
{GC02K0_SENSOR_ID, SENSOR_DRVNAME_GC02K0_MIPI_RAW, GC02K0_MIPI_RAW_SensorInit},
#endif
#if defined(OV02B10_MIPI_RAW)
{OV02B10_SENSOR_ID, SENSOR_DRVNAME_OV02B10_MIPI_RAW, OV02B10_MIPI_RAW_SensorInit},
#endif
#if defined(OV02A1B_MIPI_MONO)
{OV02A1B_SENSOR_ID, SENSOR_DRVNAME_OV02A1B_MIPI_MONO, OV02A1B_MIPI_MONO_SensorInit},
#endif
#if defined(OV02B1B_MIPI_MONO)
{OV02B1B_SENSOR_ID1, SENSOR_DRVNAME_OV02B1B_MIPI_MONO, OV02B1B_MIPI_MONO_SensorInit},
#endif

    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

/* Shengguang.Zhu@Cam.Drv, 20200915,  sensor porting for athens-B athens-C!*/
struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_20630[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(S5KGW3_MIPI_RAW_ATHENSB)
{S5KGW3_SENSOR_ID, SENSOR_DRVNAME_S5KGW3_MIPI_RAW_ATHENSB, S5KGW3_MIPI_RAW_SensorInit},
#endif
#if defined(OV32A1Q_MIPI_RAW_ATHENSB)
{ATHENSB_OV32A1Q_SENSOR_ID, SENSOR_DRVNAME_OV32A1Q_MIPI_RAW_ATHENSB, ATHENSB_OV32A1Q_MIPI_RAW_SensorInit},
#endif
#if defined(HI846_MIPI_RAW_ATHENSB)
{ATHENSB_HI846_SENSOR_ID, SENSOR_DRVNAME_HI846_MIPI_RAW_ATHENSB, ATHENSB_HI846_MIPI_RAW_SensorInit},
#endif
#if defined(GC02K_MIPI_RAW_ATHENSB)
{ATHENSB_GC02K_SENSOR_ID, SENSOR_DRVNAME_GC02K_MIPI_RAW_ATHENSB, ATHENSB_GC02K_MIPI_RAW_SensorInit},
#endif
#if defined(GC02M1B_MIPI_RAW_ATHENSB)
{ATHENSB_GC02M1B_SENSOR_ID, SENSOR_DRVNAME_GC02M1B_MIPI_RAW_ATHENSB, ATHENSB_GC02M1B_MIPI_RAW_SensorInit},
#endif
    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */

};

struct IMGSENSOR_SENSOR_LIST gimgsensor_sensor_list_20633[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(S5KGM1ST_MIPI_RAW_ATHENSC)
    {ATHENSC_S5KGM1ST_SENSOR_ID, SENSOR_DRVNAME_S5KGM1ST_MIPI_RAW_ATHENSC, ATHENSC_S5KGM1ST_MIPI_RAW_SensorInit},
#endif
#if defined(IMX471_MIPI_RAW_ATHENSC)
    {ATHENSC_IMX471_SENSOR_ID, SENSOR_DRVNAME_IMX471_MIPI_RAW_ATHENSC, ATHENSC_IMX471_MIPI_RAW_SensorInit},
#endif
#if defined(HI846_MIPI_RAW_ATHENSC)
    {ATHENSC_HI846_SENSOR_ID, SENSOR_DRVNAME_HI846_MIPI_RAW_ATHENSC, ATHENSC_HI846_MIPI_RAW_SensorInit},
#endif
#if defined(GC02K_MIPI_RAW_ATHENSB)
    {ATHENSB_GC02K_SENSOR_ID, SENSOR_DRVNAME_GC02K_MIPI_RAW_ATHENSB, ATHENSB_GC02K_MIPI_RAW_SensorInit},
#endif
#if defined(GC02M1B_MIPI_RAW_ATHENSB)
    {ATHENSB_GC02M1B_SENSOR_ID, SENSOR_DRVNAME_GC02M1B_MIPI_RAW_ATHENSB, ATHENSB_GC02M1B_MIPI_RAW_SensorInit},
#endif
    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

/* Hao.Wu@Cam.Drv, 20201127, sensor porting for 20611!*/
struct IMGSENSOR_SENSOR_LIST gimgsensor_sensor_list_20611[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(S5KGM1ST_MIPI_RAW_20611)
    {S5KGM1ST_SENSOR_ID_20611, SENSOR_DRVNAME_S5KGM1ST_MIPI_RAW_20611, S5KGM1ST_MAIN_MIPI_RAW_20611_SensorInit},
#endif
//#if defined(OV16A10_MIPI_RAW_20611)
//    {OV16A10_SENSOR_ID_20611, SENSOR_DRVNAME_OV16A10_MIPI_RAW_20611, OV16A10_MAIN_MIPI_RAW_20611_SensorInit},
//#endif
#if defined(S5K3P9SP_MIPI_RAW_20611)
	{S5K3P9SP_SENSOR_ID_20611, SENSOR_DRVNAME_S5K3P9SP_MIPI_RAW_20611, S5K3P9SP_MIPI_RAW_20611_SensorInit},
#endif
#if defined(HI846_MIPI_RAW_20611)
	{HI846_SENSOR_ID_20611, SENSOR_DRVNAME_HI846_MIPI_RAW_20611, HI846_MIPI_RAW_20611_SensorInit},
#endif
#if defined(GC02K_MIPI_RAW_20611)
	{GC02K_SENSOR_ID_20611, SENSOR_DRVNAME_GC02K_MIPI_RAW_20611, GC02K_MIPI_RAW_20611_SensorInit},
#endif
#if defined(OV02B1B_MIPI_RAW_20611)
	{OV02B1B_SENSOR_ID_20611, SENSOR_DRVNAME_OV02B1B_MIPI_RAW_20611, OV02B1B_MIPI_RAW_20611_SensorInit},
#endif
    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

struct IMGSENSOR_HW_CFG imgsensor_custom_config_20075[] = {
    {
            IMGSENSOR_SENSOR_IDX_MAIN,
            IMGSENSOR_I2C_DEV_0,
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
            IMGSENSOR_I2C_DEV_3,
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

struct IMGSENSOR_HW_CFG imgsensor_custom_config_20151[] = {
    {
            IMGSENSOR_SENSOR_IDX_MAIN,
            IMGSENSOR_I2C_DEV_0,
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
            IMGSENSOR_SENSOR_IDX_SUB,
            IMGSENSOR_I2C_DEV_1,
            {
                    {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                    {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
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
            IMGSENSOR_I2C_DEV_3,
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
            IMGSENSOR_SENSOR_IDX_MAIN3,
            IMGSENSOR_I2C_DEV_4,
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
    {IMGSENSOR_SENSOR_IDX_NONE}
};

//Shengguang.Zhu@Cam.Drv, 20200518, add for 20630 camera drvier porting
struct IMGSENSOR_HW_CFG imgsensor_custom_config_20630[] = {
    {
            IMGSENSOR_SENSOR_IDX_MAIN,
            IMGSENSOR_I2C_DEV_0,
            {
                    {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                    {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    /* Shipei.Chen@Cam.Drv, 20200502,  sensor porting!*/
                    #ifdef OPLUS_FEATURE_CAMERA_COMMON
                    //{IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL, IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_AFVDD, IMGSENSOR_HW_ID_REGULATOR},
                    #endif
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
                    {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL, IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
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
                    {IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL, IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
            },
    },
    {
            IMGSENSOR_SENSOR_IDX_SUB2,
            IMGSENSOR_I2C_DEV_3,
            {
                    {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                    {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                    //{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
            },
    },
    {
            IMGSENSOR_SENSOR_IDX_MAIN3,
            IMGSENSOR_I2C_DEV_4,
            {
                    {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                    {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                    //{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
            },
    },

    {IMGSENSOR_SENSOR_IDX_NONE}
};

//Shengguang.Zhu@Cam.Drv, 20200518, add for 20630 camera drvier porting
struct IMGSENSOR_HW_CFG imgsensor_custom_config_20633[] = {
    {
            IMGSENSOR_SENSOR_IDX_MAIN,
            IMGSENSOR_I2C_DEV_0,
            {
                    {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                    {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    /* Shipei.Chen@Cam.Drv, 20200502,  sensor porting!*/
                    #ifdef OPLUS_FEATURE_CAMERA_COMMON
                    //{IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL, IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_AFVDD, IMGSENSOR_HW_ID_REGULATOR},
                    #endif
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
                    {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL, IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
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
                    {IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL, IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
            },
    },
    {
            IMGSENSOR_SENSOR_IDX_SUB2,
            IMGSENSOR_I2C_DEV_3,
            {
                    {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                    {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                    //{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
            },
    },
    {
            IMGSENSOR_SENSOR_IDX_MAIN3,
            IMGSENSOR_I2C_DEV_4,
            {
                    {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
                    {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
                    {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
                    //{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
                    //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
                    {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
            },
    },

    {IMGSENSOR_SENSOR_IDX_NONE}
};

struct IMGSENSOR_HW_CFG imgsensor_custom_config_20611[] = {
    {
        IMGSENSOR_SENSOR_IDX_MAIN,
        IMGSENSOR_I2C_DEV_0,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
            //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            /* Shipei.Chen@Cam.Drv, 20200502,  sensor porting!*/
            {IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL, IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_SUB,
        IMGSENSOR_I2C_DEV_1,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
            //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_MAIN2,
        IMGSENSOR_I2C_DEV_2,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_REGULATOR},
            //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_SUB2,
        IMGSENSOR_I2C_DEV_3,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            //{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
            //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_MAIN3,
        IMGSENSOR_I2C_DEV_4,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            //{IMGSENSOR_HW_PIN_DVDD,  IMGSENSOR_HW_ID_GPIO},
            //{IMGSENSOR_HW_PIN_PDN,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE, IMGSENSOR_HW_ID_NONE},
        },
    },

    {IMGSENSOR_SENSOR_IDX_NONE}

};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20075[] = {
#if defined(OV48B_MIPI_RAW)
    {
        SENSOR_DRVNAME_OV48B_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {SensorMCLK, Vol_High, 0},
            {DOVDD, Vol_1800, 0},
            {AVDD, Vol_2800, 0},
            {DVDD, Vol_1200, 5},
            // {AFVDD, Vol_2800, 2},
            {RST, Vol_High, 5},
        },
    },
#endif
#if defined(OV32A_MIPI_RAW)
    {
        SENSOR_DRVNAME_OV32A_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {SensorMCLK, Vol_High, 5},
            {AVDD, Vol_2800, 1},
            {DOVDD, Vol_1800, 1},
            {DVDD, Vol_1100, 1},
            {RST, Vol_High, 5}
        },
    },
#endif
#if defined(HI846_MIPI_RAW)
    {
        SENSOR_DRVNAME_HI846_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2800, 0},
            {DVDD, Vol_1200, 0},
            {DOVDD, Vol_1800, 1},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2}
        },
    },
#endif
#if defined(GC02K0_MIPI_RAW)
    {
        SENSOR_DRVNAME_GC02K0_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {DOVDD, Vol_1800, 5},
            {AVDD, Vol_2800, 5},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 1},
        },
    },
#endif

    /* add new sensor before this line */
    {NULL,},
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20151[] = {
#if defined(OV48B_MIPI_RAW)
    {
        SENSOR_DRVNAME_OV48B_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {DOVDD, Vol_1800, 0},
            {AVDD, Vol_2800, 0},
            {DVDD, Vol_1200, 5},
            // {AFVDD, Vol_2800, 2},
            {SensorMCLK, Vol_High, 0},
            {RST, Vol_High, 5},
        },
    },
#endif
#if defined(S5KGM1ST_MIPI_RAW)
    {
        SENSOR_DRVNAME_S5KGM1ST_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2800, 0},
            {DOVDD, Vol_1800, 0},
            {DVDD, Vol_1100, 1},
            {RST, Vol_High, 1},
            {SensorMCLK, Vol_High, 2},
        },
    },
#endif
#if defined(HI846_MIPI_RAW)
    {
        SENSOR_DRVNAME_HI846_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2800, 0},
            {DVDD, Vol_1200, 0},
            {DOVDD, Vol_1800, 1},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2}
        },
    },
#endif
#if defined(IMX471_MIPI_RAW)
    {
        SENSOR_DRVNAME_IMX471_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2800, 0},
            {DVDD, Vol_1100, 0},
            {DOVDD, Vol_1800, 1},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2},
        },
    },
#endif
#if defined(GC02K0_MIPI_RAW)
    {
        SENSOR_DRVNAME_GC02K0_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {DOVDD, Vol_1800, 5},
            {AVDD, Vol_2800, 5},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 1},
        },
    },
#endif
#if defined(OV02B10_MIPI_RAW)
    {
        SENSOR_DRVNAME_OV02B10_MIPI_RAW,
        {
            {IMGSENSOR_HW_PIN_MIPI_SWITCH_EN, Vol_Low, 10},
            {RST, Vol_Low, 1},
            {DOVDD, Vol_1800, 5},
            {AVDD, Vol_2800, 5},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2},
        },
    },
#endif
#if defined(OV02A1B_MIPI_MONO)
    {
        SENSOR_DRVNAME_OV02A1B_MIPI_MONO,
        {
            {IMGSENSOR_HW_PIN_MIPI_SWITCH_EN, Vol_High, 10},
            {RST, Vol_High, 1, Vol_Low, 1},
            {AVDD, Vol_2800, 0},
            {DOVDD, Vol_1800, 0},
            {SensorMCLK, Vol_High, 5},
            {RST, Vol_Low, 9},
        },
    },
#endif
#if defined(OV02B1B_MIPI_MONO)
    {
        SENSOR_DRVNAME_OV02B1B_MIPI_MONO,
        {
            {IMGSENSOR_HW_PIN_MIPI_SWITCH_EN, Vol_High, 10},
            {RST, Vol_Low, 1},
            {DOVDD, Vol_1800, 5},
            {AVDD, Vol_2800, 5},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2},
        },
    },
#endif

    /* add new sensor before this line */
    {NULL,},
};

struct IMGSENSOR_HW_POWER_SEQ platform_power_sequence_20630[] ={
    {
        PLATFORM_POWER_SEQ_NAME,
        {
            {
                IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL,
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
                IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL,
                IMGSENSOR_HW_PIN_STATE_LEVEL_0,
                0,
                IMGSENSOR_HW_PIN_STATE_LEVEL_0,
                0
            },
        },
        IMGSENSOR_SENSOR_IDX_MAIN2,
    },
};

struct IMGSENSOR_HW_POWER_SEQ platform_power_sequence_20633[] ={
    {
        PLATFORM_POWER_SEQ_NAME,
        {
            {
                IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL,
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
                IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL,
                IMGSENSOR_HW_PIN_STATE_LEVEL_0,
                0,
                IMGSENSOR_HW_PIN_STATE_LEVEL_0,
                0
            },
        },
        IMGSENSOR_SENSOR_IDX_MAIN2,
    },
};

struct IMGSENSOR_HW_POWER_SEQ platform_power_sequence_20611[] ={
	{
		PLATFORM_POWER_SEQ_NAME,
		{
			{
				IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL,
				IMGSENSOR_HW_PIN_STATE_LEVEL_0,
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
				IMGSENSOR_HW_PIN_MIPI_SWITCH_SEL,
				IMGSENSOR_HW_PIN_STATE_LEVEL_HIGH,
				0,
				IMGSENSOR_HW_PIN_STATE_LEVEL_0,
				0
			},
		},
		IMGSENSOR_SENSOR_IDX_MAIN2,
	},
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20630[] = {
#if defined(S5KGW3_MIPI_RAW_ATHENSB)
        {
            SENSOR_DRVNAME_S5KGW3_MIPI_RAW_ATHENSB,
            {
                {RST, Vol_Low, 1},
                {DOVDD, Vol_1800, 1},
                {DVDD, Vol_1100, 1},
                {AVDD, Vol_2800, 1},
                {AF_VDD, Vol_2800, 0},
                {RST, Vol_High, 2},
                {SensorMCLK, Vol_High, 5},
            },
        },
#endif
#if defined(OV32A1Q_MIPI_RAW_ATHENSB)
        {
            SENSOR_DRVNAME_OV32A1Q_MIPI_RAW_ATHENSB,
            {
                {RST, Vol_Low, 1},
                {AVDD, Vol_2800, 0},
                {DVDD, Vol_1100, 0},
                {DOVDD, Vol_1800, 1},
                {MIPISEL, Vol_High, 3},
                {SensorMCLK, Vol_High, 1},
                {RST, Vol_High, 2},
            },
        },
#endif
#if defined(HI846_MIPI_RAW_ATHENSB)
        {
            SENSOR_DRVNAME_HI846_MIPI_RAW_ATHENSB,
            {
                {RST, Vol_Low, 1},
                {AVDD, Vol_2800, 0},
                {DVDD, Vol_1200, 0},
                {DOVDD, Vol_1800, 1},
                {MIPISEL, Vol_Low, 3},
                {SensorMCLK, Vol_High, 1},
                {RST, Vol_High, 2},
            },
        },
#endif
#if defined(GC02K_MIPI_RAW_ATHENSB)
        {
            SENSOR_DRVNAME_GC02K_MIPI_RAW_ATHENSB,
            {
                {RST, Vol_Low, 1},
                //{DVDD, Vol_1200, 0},
                {DOVDD, Vol_1800, 1},
                {AVDD, Vol_2800, 1},
                {SensorMCLK, Vol_High, 1},
                {RST, Vol_High, 2}
            },
        },
#endif
#if defined(GC02M1B_MIPI_RAW_ATHENSB)
        {
            SENSOR_DRVNAME_GC02M1B_MIPI_RAW_ATHENSB,
            {
                {RST, Vol_Low, 1},
                //{DVDD, Vol_1200, 0},
                {DOVDD, Vol_1800, 1},
                {AVDD, Vol_2800, 0},
                {SensorMCLK, Vol_High, 1},
                {RST, Vol_High, 2}
            },
        },
#endif


    /* add new sensor before this line */
    {NULL,},
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20633[] = {
#if defined(S5KGM1ST_MIPI_RAW_ATHENSC)
        {
            SENSOR_DRVNAME_S5KGM1ST_MIPI_RAW_ATHENSC,
            {
                {RST, Vol_Low, 1},
                {AVDD, Vol_2800, 0},
                {DVDD, Vol_1500, 0},
                {DOVDD, Vol_1800, 1},
                {AF_VDD, Vol_2800, 0},
                {RST, Vol_High, 1},
                {SensorMCLK, Vol_High, 1},

            },
        },
#endif
#if defined(IMX471_MIPI_RAW_ATHENSC)
        {
            SENSOR_DRVNAME_IMX471_MIPI_RAW_ATHENSC,
            {
                {RST, Vol_Low, 1},
                {AVDD, Vol_2800, 0},
                {DVDD, Vol_1100, 0},
                {DOVDD, Vol_1800, 1},
                {MIPISEL, Vol_High, 3},
                {SensorMCLK, Vol_High, 1},
                {RST, Vol_High, 2},
            },
        },
#endif
#if defined(HI846_MIPI_RAW_ATHENSC)
        {
            SENSOR_DRVNAME_HI846_MIPI_RAW_ATHENSC,
            {
                {RST, Vol_Low, 1},
                {AVDD, Vol_2800, 0},
                {DVDD, Vol_1200, 0},
                {DOVDD, Vol_1800, 1},
                {MIPISEL, Vol_Low, 3},
                {SensorMCLK, Vol_High, 1},
                {RST, Vol_High, 2},
            },
        },
#endif

#if defined(GC02K_MIPI_RAW_ATHENSB)
    {
        SENSOR_DRVNAME_GC02K_MIPI_RAW_ATHENSB,
        {
            {RST, Vol_Low, 1},
            //{DVDD, Vol_1200, 0},
            {DOVDD, Vol_1800, 1},
            {AVDD, Vol_2800, 1},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2}
        },
    },
#endif
#if defined(GC02M1B_MIPI_RAW_ATHENSB)
    {
        SENSOR_DRVNAME_GC02M1B_MIPI_RAW_ATHENSB,
        {
            {RST, Vol_Low, 1},
            //{DVDD, Vol_1200, 0},
            {DOVDD, Vol_1800, 1},
            {AVDD, Vol_2800, 0},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2}
        },
    },
#endif


    /* add new sensor before this line */
    {NULL,},
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20611[] = {
/* Shipei.Chen@Cam.Drv, 20200502,  sensor porting!*/
#if defined(S5KGM1ST_MIPI_RAW_20611)
		{
			SENSOR_DRVNAME_S5KGM1ST_MIPI_RAW_20611,
			{
				{RST, Vol_Low, 1},
				{AVDD, Vol_2800, 0},
				{DVDD, Vol_1200, 0},
				{DOVDD, Vol_1800, 1},
				// /* Shipei.Chen@Cam.Drv, 20200502,  sensor porting!*/
				{MIPISEL,Vol_High,3},
				{RST, Vol_High, 2},
				{SensorMCLK, Vol_High, 1},
			},
		},
#endif
//#if defined(OV16A10_MIPI_RAW_20611)
//		{
//			SENSOR_DRVNAME_OV16A10_MIPI_RAW_20611,
//			{
//				{RST, Vol_Low, 1},
//				{AVDD, Vol_2800, 0},
//				{DVDD, Vol_1500, 0},
//				{DOVDD, Vol_1800, 1},
//				/* Shipei.Chen@Cam.Drv, 20200502,  sensor porting!*/
//				{MIPISEL, Vol_High, 3},
//				{SensorMCLK, Vol_High, 1},
//				{RST, Vol_High, 2},
//			},
//		},
//#endif
#if defined(S5K3P9SP_MIPI_RAW_20611)
	{
		SENSOR_DRVNAME_S5K3P9SP_MIPI_RAW_20611,
		{
			{RST, Vol_Low, 1},
			{AVDD, Vol_2800, 0},
			{DVDD, Vol_1100, 0},
			{DOVDD, Vol_1800, 1},
			{SensorMCLK, Vol_High, 1},
			{RST, Vol_High, 2},
		},
	},
#endif
#if defined(HI846_MIPI_RAW_20611)
		{
			SENSOR_DRVNAME_HI846_MIPI_RAW_20611,
			{
				{RST, Vol_Low, 1},
				{AVDD, Vol_2800, 0},
				{DVDD, Vol_1200, 0},
				{DOVDD, Vol_1800, 1},
				{SensorMCLK, Vol_High, 1},
				{RST, Vol_High, 2},
			},
		},
#endif
#if defined(GC02K_MIPI_RAW_20611)
	{
		SENSOR_DRVNAME_GC02K_MIPI_RAW_20611,
		{
			{RST, Vol_Low, 1},
			//{DVDD, Vol_1200, 0},
			{DOVDD, Vol_1800, 1},
			{AVDD, Vol_2800, 0},
			{SensorMCLK, Vol_High, 1},
			{RST, Vol_High, 2}
		},
	},
#endif
#if defined(OV02B1B_MIPI_RAW_20611)
	{
		SENSOR_DRVNAME_OV02B1B_MIPI_RAW_20611,
		{
			{RST, Vol_Low, 1},
			//{DVDD, Vol_1200, 0},
			{DOVDD, Vol_1800, 1},
			{AVDD, Vol_2800, 0},
			{SensorMCLK, Vol_High, 1},
			{RST, Vol_High, 2}
		},
	},
#endif
	/* add new sensor before this line */
	{NULL,},
};

struct CAMERA_DEVICE_INFO gImgEepromInfo_20075= {
    .i4SensorNum = 4,
    .pCamModuleInfo = {
        {OV48B_SENSOR_ID,  0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0x96,0x98,0x94}, "Cam_r0", "ov48b2q"},
        {OV32A_SENSOR_ID,  0xA8, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f",  "ov32a1q"},
        {HI846_SENSOR_ID,  0xA2, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "hi846"},
        {GC02K0_SENSOR_ID, 0xA4, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r2", "gc02k0"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {OV48B_STEREO_START_ADDR, HI846_STEREO_START_ADDR },
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};

struct CAMERA_DEVICE_INFO gImgEepromInfo_20630 = {
    .i4SensorNum = 5,
    .pCamModuleInfo = {
        {S5KGW3_SENSOR_ID,              0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "s5kgw3_20630"},
        {ATHENSB_OV32A1Q_SENSOR_ID,     0xA8, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f",  "athensb_ov32a1q_20630"},
        {ATHENSB_HI846_SENSOR_ID,       0xA2, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "athensb_hi846_20630"},
        {ATHENSB_GC02M1B_SENSOR_ID,     0xFF, {0xFF, 0xFF}, 0xFF, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r2", "athensb_gc02m1b_20630"},
        {ATHENSB_GC02K_SENSOR_ID,       0xA4, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r3", "athensb_gc02k_20630"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {S5KGW3_STEREO_START_ADDR_20630, HI846_STEREO_START_ADDR_20630},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};

struct CAMERA_DEVICE_INFO gImgEepromInfo_20633 = {
    .i4SensorNum = 5,
    .pCamModuleInfo = {
        {ATHENSC_S5KGM1ST_SENSOR_ID,    0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "athensc_s5kgm1st_20630"},
        {ATHENSC_IMX471_SENSOR_ID,      0xA8, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f",  "athensc_imx471_20630"},
        {ATHENSC_HI846_SENSOR_ID,       0xA2, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "athensc_hi846_20630"},
        {ATHENSB_GC02M1B_SENSOR_ID,     0xFF, {0xFF, 0xFF}, 0xFF, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r2", "athensb_gc02m1b_20630"},
        {ATHENSB_GC02K_SENSOR_ID,       0xA4, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r3", "athensb_gc02k_20630"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {S5KGM1ST_STEREO_START_ADDR_20633, HI846_STEREO_START_ADDR_20633},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};

struct CAMERA_DEVICE_INFO gImgEepromInfo_20611 = {
    .i4SensorNum = 5,
    .pCamModuleInfo = {
        {S5KGM1ST_SENSOR_ID_20611,    0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "s5kgm1st_mipi_raw_20611"},
        {S5K3P9SP_SENSOR_ID_20611,      0xA8, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f",  "s5k3p9sp_mipi_raw_20611"},
        {HI846_SENSOR_ID_20611,       0xA2, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "hi846_mipi_raw_20611"},
        {GC02K_SENSOR_ID_20611,     0xFF, {0xFF, 0xFF}, 0xFF, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r2", "gc02k_mipi_raw_20611"},
        {OV02B1B_SENSOR_ID_20611,       0xA4, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r3", "ov02b1b_mipi_raw_20611"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {S5KGM1ST_STEREO_START_ADDR_20611, HI846_STEREO_START_ADDR_20611},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};

void oplus_imgsensor_hwcfg()
{
    if (is_project(20075)|| is_project(20076)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20075;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20075;
         oplus_sensor_power_sequence = sensor_power_sequence_20075;
         gImgEepromInfo = gImgEepromInfo_20075;
    } else if (is_project(20630) ||is_project(20631) || is_project(20632) || is_project(0x206B4)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20630;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20630;
         oplus_sensor_power_sequence = sensor_power_sequence_20630;
         oplus_platform_power_sequence = platform_power_sequence_20630;
         gImgEepromInfo = gImgEepromInfo_20630;
    } else if (is_project(20633) ||is_project(20634) ||is_project(20635)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20633;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20633;
         oplus_sensor_power_sequence = sensor_power_sequence_20633;
         oplus_platform_power_sequence = platform_power_sequence_20633;
         gImgEepromInfo = gImgEepromInfo_20633;
    } else if (is_project(20610) || is_project(20611) || is_project(20613) || is_project(20680)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20611;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20611;
         oplus_sensor_power_sequence = sensor_power_sequence_20611;
         oplus_platform_power_sequence = platform_power_sequence_20611;
         gImgEepromInfo = gImgEepromInfo_20611;
    } else {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20075;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20075;
         oplus_sensor_power_sequence = sensor_power_sequence_20075;
         gImgEepromInfo = gImgEepromInfo_20075;
    }
}
#endif
