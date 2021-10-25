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
#ifndef __IMGSENSOR_HWCFG_20131_CTRL_H__
#define __IMGSENSOR_HWCFG_20131_CTRL_H__
#include "imgsensor_hwcfg_custom.h"


struct IMGSENSOR_SENSOR_LIST *oplus_gimgsensor_sensor_list = NULL;
struct IMGSENSOR_HW_CFG *oplus_imgsensor_custom_config = NULL;
struct IMGSENSOR_HW_POWER_SEQ *oplus_sensor_power_sequence = NULL;
struct CAMERA_DEVICE_INFO gImgEepromInfo;

struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_20131[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(OV64B_MIPI_RAW)
{OV64B_SENSOR_ID, SENSOR_DRVNAME_OV64B_MIPI_RAW, OV64B_MIPI_RAW_SensorInit},
#endif
#if defined(IMX616_MIPI_RAW)
{IMX616_SENSOR_ID, SENSOR_DRVNAME_IMX616_MIPI_RAW, IMX616_MIPI_RAW_SensorInit},
#endif
#if defined(IMX615_MIPI_RAW)
{IMX615_SENSOR_ID, SENSOR_DRVNAME_IMX615_MIPI_RAW, IMX615_MIPI_RAW_SensorInit},
#endif
#if defined(IMX319_MIPI_RAW)
{IMX319_SENSOR_ID, SENSOR_DRVNAME_IMX319_MIPI_RAW, IMX319_MIPI_RAW_SensorInit},
#endif
#if defined(OV02B10_MIPI_RAW)
{OV02B10_SENSOR_ID, SENSOR_DRVNAME_OV02B10_MIPI_RAW, OV02B10_MIPI_RAW_SensorInit},
#endif
#if defined(GC02M1B_MIPI_MONO)
{GC02M1B_SENSOR_ID, SENSOR_DRVNAME_GC02M1B_MIPI_MONO, GC02M1B_MIPI_MONO_SensorInit},
#endif

    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

struct IMGSENSOR_HW_CFG imgsensor_custom_config_20131[] = {
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
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_SUB2,
        IMGSENSOR_I2C_DEV_2,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_MAIN3,
        IMGSENSOR_I2C_DEV_1,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },

    {IMGSENSOR_SENSOR_IDX_NONE}
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20131[] = {
#if defined(OV64B_MIPI_RAW)
    {
        SENSOR_DRVNAME_OV64B_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {SensorMCLK, Vol_High, 1},
            {AVDD, Vol_2800, 2},
            {DOVDD, Vol_1800, 1},
            {DVDD, Vol_1100, 5},
            {RST, Vol_High, 5},
        },
    },
#endif
#if defined(IMX616_MIPI_RAW)
    {
        SENSOR_DRVNAME_IMX616_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2900, 0},
            {DVDD, Vol_1100, 0},
            {DOVDD, Vol_1800, 1},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2},
        },
    },
#endif
#if defined(IMX615_MIPI_RAW)
    {
        SENSOR_DRVNAME_IMX615_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2900, 0},
            {DVDD, Vol_1100, 0},
            {DOVDD, Vol_1800, 1},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2},
        },
    },
#endif
#if defined(IMX319_MIPI_RAW)
    {
        SENSOR_DRVNAME_IMX319_MIPI_RAW,
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
#if defined(OV02B10_MIPI_RAW)
    {
        SENSOR_DRVNAME_OV02B10_MIPI_RAW,
        {
            {RST, Vol_Low, 1},
            {DOVDD, Vol_1800, 1},
            {AVDD, Vol_2800, 4, Vol_Low, 8},
            {SensorMCLK, Vol_High, 5},
            {RST, Vol_High, 10}
        },
    },
#endif
#if defined(GC02M1B_MIPI_MONO)
    {
        SENSOR_DRVNAME_GC02M1B_MIPI_MONO,
        {
            {RST, Vol_Low, 1},
            //{DVDD, Vol_1200, 0},
            {DOVDD, Vol_1800, 1},
            {AVDD, Vol_2800, 0, Vol_Low, 8},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 2}
        },
    },
#endif
    /* add new sensor before this line */
    {NULL,},
};

struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_20601[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(IMX686_MIPI_RAW_20601)
    {IMX686_SENSOR_ID_20601, SENSOR_DRVNAME_IMX686_MIPI_RAW_20601, IMX686_MIPI_RAW_20601_SensorInit},
#endif
#if defined(OV32A1Q_MIPI_RAW_20601)
    {OV32A1Q_SENSOR_ID_20601, SENSOR_DRVNAME_OV32A1Q_MIPI_RAW_20601, OV32A1Q_MIPI_RAW_20601_SensorInit},
#endif
#if defined(HI846_MIPI_RAW_20601)
    {HI846_SENSOR_ID_20601, SENSOR_DRVNAME_HI846_MIPI_RAW_20601, HI846_MIPI_RAW_20601_SensorInit},
#endif
#if defined(GC02M1B_MIPI_RAW_20601)
    {GC02M1B_SENSOR_ID_20601, SENSOR_DRVNAME_GC02M1B_MIPI_RAW_20601, GC02M1B_MIPI_RAW_20601_SensorInit},
#endif
#if defined(GC2385_MIPI_RAW_20601)
    {GC2385_SENSOR_ID_20601, SENSOR_DRVNAME_GC2385_MIPI_RAW_20601, GC2385_MIPI_RAW_20601_SensorInit},
#endif

    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

struct IMGSENSOR_HW_CFG imgsensor_custom_config_20601[] = {
    {
        IMGSENSOR_SENSOR_IDX_MAIN,
        IMGSENSOR_I2C_DEV_0,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_AVDD_1,IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            /* Feiping.Li@Cam.Drv, 20200110, add for ois i2c error */
            {IMGSENSOR_HW_PIN_AFVDD, IMGSENSOR_HW_ID_REGULATOR},
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
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },

    {IMGSENSOR_SENSOR_IDX_NONE}
};

struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20601[] = {
#if defined(IMX686_MIPI_RAW_20601)
    {
        SENSOR_DRVNAME_IMX686_MIPI_RAW_20601,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2900, 0},
            {AVDD_1, Vol_1800, 0},
            {DVDD, Vol_1100, 0},
            {DOVDD, Vol_1800, 2},
            //{AFVDD, Vol_1800, 1},
            {SensorMCLK, Vol_High, 1},
            {RST, Vol_High, 3}
        },
    },
#endif
#if defined(HI846_MIPI_RAW_20601)
    {
        SENSOR_DRVNAME_HI846_MIPI_RAW_20601,
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
#if defined(GC02M1B_MIPI_RAW_20601)
    {
        SENSOR_DRVNAME_GC02M1B_MIPI_RAW_20601,
        {
            {RST, Vol_Low, 1},
            {DVDD, Vol_1800, 0},
            {DOVDD, Vol_1800, 1},
            {AVDD, Vol_2800, 1},
            {RST, Vol_High, 2},
            {SensorMCLK, Vol_High, 1}
        },
    },
#endif
#if defined(GC2385_MIPI_RAW_20601)
    {
        SENSOR_DRVNAME_GC2385_MIPI_RAW_20601,
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
#if defined(OV32A1Q_MIPI_RAW_20601)
    {
        SENSOR_DRVNAME_OV32A1Q_MIPI_RAW_20601,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2800, 0},
            {DVDD, Vol_1100, 0},
            {DOVDD, Vol_1800, 1},
            {RST, Vol_High, 2},
            {SensorMCLK, Vol_High, 1},
        },
    },
#endif

    /* add new sensor before this line */
    {NULL,},
};


struct CAMERA_DEVICE_INFO gImgEepromInfo_20131 = {
    .i4SensorNum = 5,
    .pCamModuleInfo = {
        {OV64B_SENSOR_ID,   0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "ov64b2q"},
        {IMX616_SENSOR_ID,  0xA8, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f",  "imx616"},
        {IMX319_SENSOR_ID,  0xA2, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "imx319"},
        {OV02B10_SENSOR_ID, 0xA4, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r2", "ov02b10"},
        {GC02M1B_SENSOR_ID, 0xFF, {0xFF, 0xFF}, 0xFF, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r3", "gc02m1b"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {OV64B_STEREO_START_ADDR, IMX319_STEREO_START_ADDR},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};

struct IMGSENSOR_SENSOR_LIST
    gimgsensor_sensor_list_20645[MAX_NUM_OF_SUPPORT_SENSOR] = {
#if defined(OV64B_MIPI_RAW_20645)
    {OV64B_SENSOR_ID_20645, SENSOR_DRVNAME_OV64B_MIPI_RAW_20645, OV64B_MIPI_RAW_20645_SensorInit},
#endif
#if defined(OV32A1Q_MIPI_RAW_20645)
    {OV32A1Q_SENSOR_ID_20645, SENSOR_DRVNAME_OV32A1Q_MIPI_RAW_20645, OV32A1Q_MIPI_RAW_20645_SensorInit},
#endif
#if defined(HI846_MIPI_RAW_20645)
    {HI846_SENSOR_ID_20645, SENSOR_DRVNAME_HI846_MIPI_RAW_20645, HI846_MIPI_RAW_20645_SensorInit},
#endif
#if defined(OV02B10_MIPI_RAW_20645)
    {OV02B10_SENSOR_ID_20645, SENSOR_DRVNAME_OV02B10_MIPI_RAW_20645, OV02B10_MIPI_RAW_20645_SensorInit},
#endif

    /*  ADD sensor driver before this line */
    {0, {0}, NULL}, /* end of list */
};

struct IMGSENSOR_HW_CFG imgsensor_custom_config_20645[] = {
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
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_SUB2,
        IMGSENSOR_I2C_DEV_2,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },
    {
        IMGSENSOR_SENSOR_IDX_MAIN3,
        IMGSENSOR_I2C_DEV_1,
        {
            {IMGSENSOR_HW_PIN_MCLK,  IMGSENSOR_HW_ID_MCLK},
            {IMGSENSOR_HW_PIN_AVDD,  IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_DOVDD, IMGSENSOR_HW_ID_REGULATOR},
            {IMGSENSOR_HW_PIN_RST,   IMGSENSOR_HW_ID_GPIO},
            {IMGSENSOR_HW_PIN_NONE,  IMGSENSOR_HW_ID_NONE},
        },
    },

    {IMGSENSOR_SENSOR_IDX_NONE}
};


struct IMGSENSOR_HW_POWER_SEQ sensor_power_sequence_20645[] = {
#if defined(OV64B_MIPI_RAW_20645)
    {
        SENSOR_DRVNAME_OV64B_MIPI_RAW_20645,
        {
            {RST, Vol_Low, 1},
            {SensorMCLK, Vol_High, 1},
            {AVDD, Vol_2800, 2},
            {DOVDD, Vol_1800, 1},
            {DVDD, Vol_1100, 5},
            {RST, Vol_High, 5},
        },
    },
#endif
#if defined(OV32A1Q_MIPI_RAW_20645)
    {
        SENSOR_DRVNAME_OV32A1Q_MIPI_RAW_20645,
        {
            {RST, Vol_Low, 1},
            {AVDD, Vol_2800, 0},
            {DVDD, Vol_1100, 0},
            {DOVDD, Vol_1800, 1},
            {RST, Vol_High, 2},
            {SensorMCLK, Vol_High, 1},
        },
    },
#endif
#if defined(HI846_MIPI_RAW_20645)
    {
        SENSOR_DRVNAME_HI846_MIPI_RAW_20645,
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
#if defined(OV02B10_MIPI_RAW_20645)
    {
        SENSOR_DRVNAME_OV02B10_MIPI_RAW_20645,
        {
            {RST, Vol_Low, 1},
            {DOVDD, Vol_1800, 1},
            {AVDD, Vol_2800, 4, Vol_Low, 8},
            {SensorMCLK, Vol_High, 5},
            {RST, Vol_High, 10}
        },
    },
#endif

    /* add new sensor before this line */
    {NULL,},


};

struct CAMERA_DEVICE_INFO gImgEepromInfo_20645 = {
    .i4SensorNum = 4,
    .pCamModuleInfo = {
        {OV64B_SENSOR_ID_20645,     0xA0, {0x00, 0x06}, 0xB0, 1, {0x92,0xFF,0xFF,0x94}, "Cam_r0", "ov64b_20645"},
        {OV32A1Q_SENSOR_ID_20645,   0xA8, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_f",  "ov32a1q_20645"},
        {HI846_SENSOR_ID_20645,     0xA2, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r1", "hi846_20645"},
        {OV02B10_SENSOR_ID_20645,   0xA4, {0x00, 0x06}, 0xB0, 0, {0xFF,0xFF,0xFF,0xFF}, "Cam_r2", "ov02b10_20645"},
    },
    .i4MWDataIdx = IMGSENSOR_SENSOR_IDX_MAIN2,
    .i4MTDataIdx = 0xFF,
    .i4FrontDataIdx = 0xFF,
    .i4NormDataLen = 40,
    .i4MWDataLen = 3102,
    .i4MWStereoAddr = {OV64B_STEREO_START_ADDR, HI846_STEREO_START_ADDR_20645},
    .i4MTStereoAddr = {0xFFFF, 0xFFFF},
    .i4FrontStereoAddr = {0xFFFF, 0xFFFF},
};


void oplus_imgsensor_hwcfg()
{
    if (is_project(20131) || is_project(20133)
          || is_project(20255) || is_project(20257)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20131;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20131;
         oplus_sensor_power_sequence = sensor_power_sequence_20131;
         gImgEepromInfo = gImgEepromInfo_20131;
    } else if (is_project(20601) || is_project(20602) || is_project(20660)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20601;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20601;
         oplus_sensor_power_sequence = sensor_power_sequence_20601;
         gImgEepromInfo = gImgEepromInfo_20131;
    } else if (is_project(20645) || is_project(20649)
          || is_project(0x2064A) || is_project(0x2068D)) {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20645;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20645;
         oplus_sensor_power_sequence = sensor_power_sequence_20645;
         gImgEepromInfo = gImgEepromInfo_20645;
    } else {
         oplus_gimgsensor_sensor_list = gimgsensor_sensor_list_20131;
         oplus_imgsensor_custom_config = imgsensor_custom_config_20131;
         oplus_sensor_power_sequence = sensor_power_sequence_20131;
         gImgEepromInfo = gImgEepromInfo_20131;
    }
}
#endif
