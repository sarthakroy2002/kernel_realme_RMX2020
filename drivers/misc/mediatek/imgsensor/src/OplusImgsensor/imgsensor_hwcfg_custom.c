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
#include "imgsensor_hwcfg_custom.h"
#ifdef SENSOR_PLATFORM_5G_H
int camera_status = 0;

int is_check_camera_status(void)
{
    pr_debug("For checking camera_status: %d \n", camera_status);
    return camera_status;
}
EXPORT_SYMBOL(is_check_camera_status);
#endif

struct IMGSENSOR_SENSOR_LIST *Oplusimgsensor_Sensorlist(void)
{
    struct IMGSENSOR_SENSOR_LIST *pOplusImglist = gimgsensor_sensor_list;
    pr_debug("Oplusimgsensor_Sensorlist enter:\n");
    /*Henry.Chang@Cam.Drv, 20200725, add for project CHAKA 20131,20133*/
    pOplusImglist = oplus_gimgsensor_sensor_list;
    pr_info("oplus_gimgsensor_sensor_list Selected\n");

    return pOplusImglist;
}

struct IMGSENSOR_HW_CFG *Oplusimgsensor_Custom_Config(void)
{
    struct IMGSENSOR_HW_CFG *pOplusImgHWCfg = imgsensor_custom_config;

    pOplusImgHWCfg = oplus_imgsensor_custom_config;
    pr_info("oplus_imgsensor_custom_config Selected\n");

    return pOplusImgHWCfg;
}

enum IMGSENSOR_RETURN Oplusimgsensor_i2c_init(
        struct IMGSENSOR_SENSOR_INST *psensor_inst)
{
    enum IMGSENSOR_RETURN ret = IMGSENSOR_RETURN_SUCCESS;
    struct IMGSENSOR_HW_CFG *pOplusImgHWCfg = imgsensor_custom_config;

    if (psensor_inst == NULL) {
        pr_info("Oplusimgsensor_i2c_init psensor_inst is NULL\n");
        return IMGSENSOR_RETURN_ERROR;
    }

    pOplusImgHWCfg = Oplusimgsensor_Custom_Config();
    ret = imgsensor_i2c_init(&psensor_inst->i2c_cfg,
                             pOplusImgHWCfg[psensor_inst->sensor_idx].i2c_dev);
    pr_debug("[%s] sensor_idx:%d name:%s ret: %d\n",
        __func__,
        psensor_inst->sensor_idx,
        psensor_inst->psensor_list->name,
        ret);

    return ret;
}

struct IMGSENSOR_HW_POWER_SEQ *Oplusimgsensor_matchhwcfg_power(
        enum  IMGSENSOR_POWER_ACTION_INDEX  pwr_actidx)
{
    struct IMGSENSOR_HW_POWER_SEQ *ppwr_seq = NULL;
    pr_debug("[%s] pwr_actidx:%d\n", __func__, pwr_actidx);
    if ((pwr_actidx != IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX)
        && (pwr_actidx != IMGSENSOR_POWER_MATCHSENSOR_HWCFG_INDEX)) {
        return NULL;
    }

    if (pwr_actidx == IMGSENSOR_POWER_MATCHSENSOR_HWCFG_INDEX) {
        ppwr_seq = oplus_sensor_power_sequence;
        pr_info("[%s] match oplus_sensor_power_sequence\n", __func__);
#ifdef SENSOR_PLATFORM_5G_B
    } else if (pwr_actidx == IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX){
            ppwr_seq = oplus_platform_power_sequence;
            pr_info("[%s] enter for 20075 IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX \n", __func__);
            return NULL;
#endif

#ifdef SENSOR_PLATFORM_4G_20682
    } else if (pwr_actidx == IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX){
            if (is_project(20682) || is_project(19661)) {
                ppwr_seq = oplus_platform_power_sequence;
                pr_info("[%s] match 20682 19661 IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX \n", __func__);
            }
#endif

#ifdef SENSOR_PLATFORM_5G_A
    } else if (pwr_actidx == IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX){
            ppwr_seq = oplus_platform_power_sequence;
            pr_info("[%s] match 19131 19420 IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX \n", __func__);
#endif

#ifdef SENSOR_PLATFORM_MT6771
    } else if (pwr_actidx == IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX){
            if (is_project(19531)) {
                ppwr_seq = oplus_platform_power_sequence;
                pr_info("[%s] MT6771 19531 IMGSENSOR_POWER_MATCHMIPI_HWCFG_INDEX \n", __func__);
            }
#endif
    } else {
        pr_info("[%s] NOT Support MIPISWITCH\n", __func__);
    }

    return ppwr_seq;
}

enum IMGSENSOR_RETURN Oplusimgsensor_ldoenable_power(
        struct IMGSENSOR_HW             *phw,
        enum   IMGSENSOR_SENSOR_IDX      sensor_idx,
        enum   IMGSENSOR_HW_POWER_STATUS pwr_status)
{
#ifdef SENSOR_PLATFORM_5G_H
    struct IMGSENSOR_HW_DEVICE *pdev = phw->pdev[IMGSENSOR_HW_ID_GPIO];
    pr_debug("[%s] sensor_idx:%d pdev->set is ERROR\n", __func__, sensor_idx );
    if ((pwr_status == IMGSENSOR_HW_POWER_STATUS_ON)
        && (is_project(20131) || is_project(20133)
              || is_project(20255) || is_project(20257))) {
        if (pdev->set != NULL) {
            pr_debug("set GPIO29 to enable pmic");
            pdev->set(pdev->pinstance, sensor_idx, IMGSENSOR_HW_PIN_PMIC_ENABLE, Vol_High);
        } else {
            pr_debug("[%s] sensor_idx:%d pdev->set is ERROR\n", __func__, sensor_idx );
        }
    } else if ((pwr_status == IMGSENSOR_HW_POWER_STATUS_ON)
        && (is_project(20601) || is_project(20602)|| is_project(20660))) {
        if (is_check_lcm_status() || camera_status == 1) {
            pr_debug("camera enable: the gpiod_set_value is 1, last_camera_status%d, lcm_status%d\n", camera_status, is_check_lcm_status());
        } else {
            pdev = phw->pdev[IMGSENSOR_HW_ID_GPIO];
            if (pdev->set != NULL) {
                pr_debug("camera enable: the gpiod_set_value is 1, last_camera_status%d, lcm_status%d\n", camera_status, is_check_lcm_status());
                pdev->set(pdev->pinstance, IMGSENSOR_SENSOR_IDX_MAIN, IMGSENSOR_HW_PIN_PMIC_ENABLE, Vol_High);
            }
        }
        camera_status = 1;
    } else if ((pwr_status == IMGSENSOR_HW_POWER_STATUS_OFF)
        && (is_project(20601) || is_project(20602)|| is_project(20660))) {
                if(is_check_lcm_status()) {
            pr_debug("camera disable: the gpiod_set_value is 1, last_camera_status%d, lcm_status%d\n", camera_status, is_check_lcm_status());
        } else if(camera_status == 0) {
            pr_debug("camera disable: the gpiod_set_value is 0, last_camera_status%d, lcm_status%d\n", camera_status, is_check_lcm_status());
        } else {
            pdev = phw->pdev[IMGSENSOR_HW_ID_GPIO];
            if (pdev->set != NULL) {
                pr_debug("camera disable: the gpiod_set_value is 0, camera_status%d, lcm_status%d\n", camera_status, is_check_lcm_status());
                pdev->set(pdev->pinstance, IMGSENSOR_SENSOR_IDX_MAIN, IMGSENSOR_HW_PIN_PMIC_ENABLE, Vol_Low);
            }
        }
        camera_status = 0;
    }
#endif
    return IMGSENSOR_RETURN_SUCCESS;
}

enum IMGSENSOR_RETURN Oplusimgsensor_power_pmic_20601(
        enum   IMGSENSOR_SENSOR_IDX      sensor_idx,
        enum   IMGSENSOR_HW_PIN          pin,
        enum   IMGSENSOR_HW_POWER_STATUS pwr_status)
{
    int pmic_avdd_20601[5][2] = {{3,2900},{4,2800},{4,2800},{4,2800},{4,2800}};
    int pmic_avdd1_20601[2] = {5,1800};
    int pmic_dvdd_20601[2] = {2,1200};
    int avddIdx = sensor_idx;
    pr_debug("[%s] is_pmic:%d pwr_status:%d avddIdx:%d", __func__, pmic_ldo_get_type(), pwr_status, avddIdx);

    if (pwr_status == IMGSENSOR_HW_POWER_STATUS_ON) {
        if (pin == IMGSENSOR_HW_PIN_AVDD) {
            pmic_ldo_set_voltage_mv(pmic_avdd_20601[avddIdx][0], pmic_avdd_20601[avddIdx][1]);
        } else if (pin == IMGSENSOR_HW_PIN_AVDD_1){
            pmic_ldo_set_voltage_mv(pmic_avdd1_20601[0],pmic_avdd1_20601[1]);
        } else if (pin == IMGSENSOR_HW_PIN_DVDD && ((sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN2) || (sensor_idx == IMGSENSOR_SENSOR_IDX_SUB2)) ){
            pmic_ldo_set_voltage_mv(pmic_dvdd_20601[0],pmic_dvdd_20601[1]);
        } else {
            return IMGSENSOR_RETURN_ERROR;
        }
    } else {
        if (pin == IMGSENSOR_HW_PIN_AVDD) {
            pmic_ldo_set_disable(pmic_avdd_20601[avddIdx][0]);
        }else if(pin == IMGSENSOR_HW_PIN_AVDD_1){
            pmic_ldo_set_disable(pmic_avdd1_20601[0]);
        }else if (pin == IMGSENSOR_HW_PIN_DVDD && ((sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN2) || (sensor_idx == IMGSENSOR_SENSOR_IDX_SUB2)) ){
            pmic_ldo_set_disable(pmic_dvdd_20601[0]);
        } else {
            return IMGSENSOR_RETURN_ERROR;
        }
    }
    return IMGSENSOR_RETURN_SUCCESS;

}

enum IMGSENSOR_RETURN Oplusimgsensor_power_fan53870_20630(
        enum   IMGSENSOR_SENSOR_IDX      sensor_idx,
        enum   IMGSENSOR_HW_PIN          pin,
        enum   IMGSENSOR_HW_POWER_STATUS pwr_status)
{
    int fan53870_avdd_20630[5][2] = {{4,2800},{6,2800},{3,2800},{3,2800},{3,2800}};
    int fan53870_dvdd_20630[2][2] = {{1,1100}, {2,1200}};
    int fan53870_afvdd_20630[2] = {5, 2800};
    int avddIdx = sensor_idx;
    pr_debug("[%s] is_fan53870_pmic:%d pwr_status:%d avddIdx:%d", __func__, is_fan53870_pmic(), pwr_status, avddIdx);

    if (pwr_status == IMGSENSOR_HW_POWER_STATUS_ON) {
        if (pin == IMGSENSOR_HW_PIN_AVDD) {
            fan53870_cam_ldo_set_voltage(fan53870_avdd_20630[avddIdx][0], fan53870_avdd_20630[avddIdx][1]);
        } else if (pin == IMGSENSOR_HW_PIN_DVDD){
            if((sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN)||(sensor_idx == IMGSENSOR_SENSOR_IDX_SUB)){
                fan53870_cam_ldo_set_voltage(fan53870_dvdd_20630[0][0],fan53870_dvdd_20630[0][1]);
            }else if(sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN2){
                fan53870_cam_ldo_set_voltage(fan53870_dvdd_20630[1][0],fan53870_dvdd_20630[1][1]);
            }
        } else if (pin == IMGSENSOR_HW_PIN_AFVDD) {
            if (sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN) {
                fan53870_cam_ldo_set_voltage(fan53870_afvdd_20630[0],fan53870_afvdd_20630[1]);
            }
        } else {
            return IMGSENSOR_RETURN_ERROR;
        }
    } else {
        if (pin == IMGSENSOR_HW_PIN_AVDD) {
            fan53870_cam_ldo_disable(fan53870_avdd_20630[avddIdx][0]);
        } else if (pin == IMGSENSOR_HW_PIN_DVDD){
            if((sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN)||(sensor_idx == IMGSENSOR_SENSOR_IDX_SUB)){
                fan53870_cam_ldo_disable(fan53870_dvdd_20630[0][0]);
            } else if(sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN2){
                fan53870_cam_ldo_disable(fan53870_dvdd_20630[1][0]);
            }
        } else if (pin == IMGSENSOR_HW_PIN_AFVDD) {
            if (sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN) {
                fan53870_cam_ldo_disable(fan53870_afvdd_20630[0]);
            }
        } else {
            return IMGSENSOR_RETURN_ERROR;
        }
    }
    return IMGSENSOR_RETURN_SUCCESS;

}


enum IMGSENSOR_RETURN Oplusimgsensor_ldo_powerset(
        enum   IMGSENSOR_SENSOR_IDX      sensor_idx,
        enum   IMGSENSOR_HW_PIN          pin,
        enum   IMGSENSOR_HW_POWER_STATUS pwr_status)
{
    int fan53870_avdd[3][2] = {{3, 2800}, {4, 2900}, {6, 2800}};
    int avddIdx = sensor_idx > IMGSENSOR_SENSOR_IDX_SUB ?
                    IMGSENSOR_SENSOR_IDX_MAIN2 : sensor_idx;

    pr_debug("[%s] is_pmic:%d pwr_status:%d avddIdx:%d",
                    __func__, pmic_ldo_get_type(), pwr_status, avddIdx);
    if(is_project(20601) || is_project(20602) || is_project(20660))
        return Oplusimgsensor_power_pmic_20601(sensor_idx,pin,pwr_status);

    if(is_project(20630) || is_project(20633) || is_project(20631) || is_project(20632) || is_project(0x206B4) \
        || is_project(20634) || is_project(20635))
        return Oplusimgsensor_power_fan53870_20630(sensor_idx,pin,pwr_status);

    /*pankaj.kumar@Cam.Drv, 20200928, add for project 18531  19531*/
    if(is_project(OPPO_18531) || is_project(OPPO_18561) || is_project(19531))
       return IMGSENSOR_RETURN_ERROR;

    //Niepengfei@Cam.Drv, 20201009, add for project 19165
    if (is_project(19165))
         return IMGSENSOR_RETURN_ERROR;

    //Niepengfei@Cam.Drv, 20201127, add for project 20611 20613
    if((is_project(20610)) || (is_project(20611)) || (is_project(20680)) || (is_project(20613)))
        return IMGSENSOR_RETURN_ERROR;
#ifdef SENSOR_PLATFORM_5G_B
    if (is_project(20075) || is_project(20076))
        return IMGSENSOR_RETURN_ERROR;
#endif

#ifdef SENSOR_PLATFORM_5G_A
    if ((is_project(19131)) || (is_project(19132)) || (is_project(19420)))
        return IMGSENSOR_RETURN_ERROR;
#endif

#ifdef SENSOR_PLATFORM_4G_20682
    if (is_project(20682) || is_project(20683)|| is_project(19661))
        return IMGSENSOR_RETURN_ERROR;
#endif

    if (pwr_status == IMGSENSOR_HW_POWER_STATUS_ON) {
        if (pin == IMGSENSOR_HW_PIN_AVDD) {
            if((is_project(20645) || is_project(20649)
                || is_project(0x2064A) || is_project(0x2068D))
                && sensor_idx == IMGSENSOR_SENSOR_IDX_SUB){
                fan53870_cam_ldo_set_voltage(4,2800);
           }else{
                fan53870_cam_ldo_set_voltage(
                        fan53870_avdd[avddIdx][0], fan53870_avdd[avddIdx][1]);
            }
        } else if (pin == IMGSENSOR_HW_PIN_DVDD
                && sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN2){
                if(is_project(20645) || is_project(20649)
                    || is_project(0x2064A) || is_project(0x2068D)) {
                    fan53870_cam_ldo_set_voltage(1, 1200);
                }else{
                    fan53870_cam_ldo_set_voltage(1, 1050);
                }
        } else {
            return IMGSENSOR_RETURN_ERROR;
        }
    } else {
        if (pin == IMGSENSOR_HW_PIN_AVDD) {
            fan53870_cam_ldo_disable(fan53870_avdd[avddIdx][0]);
        } else if (pin == IMGSENSOR_HW_PIN_DVDD
                && sensor_idx == IMGSENSOR_SENSOR_IDX_MAIN2){
            fan53870_cam_ldo_disable(1);
        } else {
            return IMGSENSOR_RETURN_ERROR;
        }
    }

    return IMGSENSOR_RETURN_SUCCESS;
}

void Oplusimgsensor_Registdeviceinfo(char *name, char *version, kal_uint8 module_id)
{
    char *manufacture;
    if (name == NULL || version == NULL)
    {
        pr_info("name or version is NULL");
        return;
    }
    switch (module_id)
    {
        case IMGSENSOR_MODULE_ID_SUNNY:  /* Sunny */
            manufacture = DEVICE_MANUFACUTRE_SUNNY;
            break;
        case IMGSENSOR_MODULE_ID_TRULY:  /* Truly */
            manufacture = DEVICE_MANUFACUTRE_TRULY;
            break;
        case IMGSENSOR_MODULE_ID_SEMCO:  /* Semco */
            manufacture = DEVICE_MANUFACUTRE_SEMCO;
            break;
        case IMGSENSOR_MODULE_ID_LITEON:  /* Lite-ON */
            manufacture = DEVICE_MANUFACUTRE_LITEON;
            break;
        case IMGSENSOR_MODULE_ID_QTECH:  /* Q-Tech */
            manufacture = DEVICE_MANUFACUTRE_QTECH;
            break;
        case IMGSENSOR_MODULE_ID_OFILM:  /* O-Film */
            manufacture = DEVICE_MANUFACUTRE_OFILM;
            break;
        case IMGSENSOR_MODULE_ID_SHINE:  /* Shine */
            manufacture = DEVICE_MANUFACUTRE_SHINE;
            break;
#ifdef OPLUS_FEATURE_CAMERA_COMMON
/*GaoWang@Cam.Drv add for 20682 camera BringUp 20200918*/
        case IMGSENSOR_MODULE_ID_HOLITECH:  /* Holitech */
            manufacture = DEVICE_MANUFACUTRE_HOLITECH;
            break;
#endif
        default:
            manufacture = DEVICE_MANUFACUTRE_NA;
    }
    register_device_proc(name, version, manufacture);
}

void Oplusimgsensor_powerstate_notify(bool val)
{
    if (is_project(20131) || is_project(20133)
          || is_project(20255) || is_project(20257)) {
        pr_info("[%s] val:%d", __func__, val);
        oplus_chg_set_camera_status(val);
        oplus_chg_set_camera_on(val);
    }
}
