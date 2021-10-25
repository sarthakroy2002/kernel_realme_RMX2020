# In case the platform does NOT support this type of sensors

ifneq ($(CONFIG_MTK_SENSOR_SUPPORT), n)
obj-y += core/

ifeq ($(CONFIG_CUSTOM_KERNEL_SENSORHUB),y)
obj-$(CONFIG_NANOHUB) += mtk_nanohub/
endif
obj-$(CONFIG_MTK_HF_LSM6DSM_SECONDARY) += lsm6dsm_secondary/
obj-$(CONFIG_MTK_HF_TEST_CASE) += test/
#ifdef OPLUS_FEATURE_SENSOR
#/*Chao.Zeng@PSW.BSP.Sensor, 2020/08/06, Add for oppo sensor type to send some info to scp*/
obj-y += oppo_sensor_devinfo/
#ifneq ($(OPLUS_FEATURE_SENSOR_ALGORITHM),)
obj-y += oppo_virtual_sensor/
#endif
#endif //OPLUS_FEATURE_SENSOR
endif