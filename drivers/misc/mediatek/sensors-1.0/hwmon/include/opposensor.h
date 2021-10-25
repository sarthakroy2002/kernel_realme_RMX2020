/*
 * Copyright (C) 2016 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

/*
 * Copyright(C)2014 MediaTek Inc.
 * Modification based on code covered by the below mentioned copyright
 * and/or permission notice(S).
 */

#ifndef __OPPOSENSOR_H__
#define __OPPOSENSOR_H__

/*xiebaixue@PSW.BSP.Sensor, 2020/04/17, Add for oppo sensor algo driver*/
#define SENSOR_TYPE_CAMERA_PROTECT                     73
#define SENSOR_TYPE_FREE_FALL                          74
#define SENSOR_TYPE_PICKUP_DETECT                      75
#define SENSOR_TYPE_FP_DISPLAY                         76
#define SENSOR_TYPE_LUX_AOD                            78
#define SENSOR_TYPE_PEDO_MINUTE                        79
//#ifdef OPLUS_FEATURE_ACTIVITY_RECOGNITION
//pengqingqing@ANDROID.AITECH,2020/07/01, add for oppo_activity_recognition
#define SENSOR_TYPE_OPPO_ACTIVITY_RECOGNITION          81
//#endif //OPLUS_FEATURE_ACTIVITY_RECOGNITION
#define SENSOR_TYPE_TP_GESTURE                         82
//pengqingqing@ANDROID.AITECH,2020/07/29, add for elevator_detect
#define SENSOR_TYPE_ELEVATOR_DETECT                    83
/* end sensor type */
#define SENSOR_TYPE_MAX_NUM                            SENSOR_TYPE_ELEVATOR_DETECT


#define ID_OPPO_BASE 			(0)
#define ID_CAMERA_PROTECT       (ID_OPPO_BASE + SENSOR_TYPE_CAMERA_PROTECT - 1)
#define ID_FREE_FALL            (ID_OPPO_BASE + SENSOR_TYPE_FREE_FALL - 1)
#define ID_PICKUP_DETECT        (ID_OPPO_BASE + SENSOR_TYPE_PICKUP_DETECT- 1)
#define ID_FP_DISPLAY           (ID_OPPO_BASE + SENSOR_TYPE_FP_DISPLAY - 1)
#define ID_LUX_AOD              (ID_OPPO_BASE + SENSOR_TYPE_LUX_AOD - 1)
#define ID_PEDO_MINUTE          (ID_OPPO_BASE + SENSOR_TYPE_PEDO_MINUTE - 1)
//#ifdef OPLUS_FEATURE_ACTIVITY_RECOGNITION
//pengqingqing@ANDROID.AITECH,2020/07/01, add for oppo_activity_recognition
#define ID_OPPO_ACTIVITY_RECOGNITION (ID_OPPO_BASE + SENSOR_TYPE_OPPO_ACTIVITY_RECOGNITION - 1)
//#endif //OPLUS_FEATURE_ACTIVITY_RECOGNITION
#define ID_TP_GESTURE           (ID_OPPO_BASE + SENSOR_TYPE_TP_GESTURE - 1)
//pengqingqing@ANDROID.AITECH,2020/07/29, add for elevator_detect
#define ID_ELEVATOR_DETECT      (ID_OPPO_BASE + SENSOR_TYPE_ELEVATOR_DETECT - 1)
/* end sensor ID */
#define ID_SENSOR_MAX_HANDLE    (ID_ELEVATOR_DETECT)
/*QZL@PSW.BSP.Sensor, 2018/12/24, Add for oppo algo*/
typedef struct {
	uint32_t value;
	uint16_t report_count;
}camera_protect_event_t;

typedef struct {
	uint32_t free_fall_time;
	uint32_t angle;
	uint16_t report_count;
}free_fall_event_t;

typedef struct {
	uint32_t value;
	uint16_t report_count;
}pickup_detect_event_t;

typedef struct {
	uint32_t value;
	uint16_t report_count;
}fp_display_event_t;

typedef struct {
	int32_t state;
}sar_modem_event_t;

typedef struct {
	int16_t state;
    uint16_t report_count;
}lux_aod_event_t;

typedef struct {
        int16_t value;
        uint16_t report_count;
}tp_gesture_event_t;

typedef struct {
	uint32_t step_count;
	uint16_t report_count;
	uint32_t move_status;
	uint16_t time_gap;
	uint32_t step_run_count;
	uint32_t step_walk_count;
}pedo_minute_event_t;

//#ifdef OPLUS_FEATURE_ACTIVITY_RECOGNITION
//pengqingqing@ANDROID.AITECH,2020/07/01, add for oppo_activity_recognition
typedef struct {
    uint16_t motion_count;
    uint16_t motion_ith;
    uint16_t incar_state;
    uint16_t activity_mode;
    uint32_t delta_time;
}oppo_activity_recognition_event_t;
//#endif //OPLUS_FEATURE_ACTIVITY_RECOGNITION

//pengqingqing@ANDROID.AITECH,2020/07/29, add for elevator_detect
typedef struct {
    uint16_t value;
    uint16_t report_count;
} elevator_detect_event_t;

union oppo_data_unit_t {
/*QZL@PSW.BSP.Sensor, 2018/12/24, Add for oppo algo*/
    camera_protect_event_t camera_protect_data_t;
    free_fall_event_t free_fall_data_t;
    pickup_detect_event_t pickup_detect_data_t;
    fp_display_event_t fp_display_data_t;
    sar_modem_event_t sar_modem_event;
    lux_aod_event_t lux_aod_event;
    tp_gesture_event_t tp_gesture_data_t;
    pedo_minute_event_t pedo_minute_event;
    //#ifdef OPLUS_FEATURE_ACTIVITY_RECOGNITION
    //pengqingqing@ANDROID.AITECH,2020/07/01, add for oppo_activity_recognition
    oppo_activity_recognition_event_t oppo_activity_recognition_event;
    //#endif //OPLUS_FEATURE_ACTIVITY_RECOGNITION
    //pengqingqing@ANDROID.AITECH,2020/07/29, add for elevator_detect
    elevator_detect_event_t elevator_detect_event;
};

#endif /*__OPPOSENSOR_H*/