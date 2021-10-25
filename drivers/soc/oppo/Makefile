#ifdef VENDOR_EDIT
# Bin.Li@EXP.BSP.bootloader.bootflow, 2017/05/15, Add for oppo device info and oppo project
#obj-$(CONFIG_OPPO_COMMON_SOFT)  +=  oppo_project/
obj-$(CONFIG_OPPO_DEVICE_IFNO)  +=  oppo_devinfo/

obj-y += oppo_custom/
#obj-$(CONFIG_OPPO_HYPNUS)       +=  oppo_hypnus/

#Liang.Zhang@TECH.Storage.Stability.OPPO_SHUTDOWN_DETECT, 2019/04/28, Add for shutdown detect
#obj-$(CONFIG_OPPO_SHUTDOWN_DETECT)  +=  oppo_shutdown_detect/
#endif /* VENDOR_EDIT */
#ifdef VENDOR_EDIT
#Weican.Zhao@PSW.NW.EM, 2019/02/12, Add for OPPO Critical log
#obj-$(CONFIG_OPPO_CRITICALLOG)  +=  oppo_criticallog/
obj-y  +=  oppo_criticallog/

#Kun.Hu@TECH.BSP.Stability.PHOENIX_PROJECT 2019/06/15, Add for PHOENIX_PROJECT
#obj-$(CONFIG_OPPO_PHOENIX)     +=  oppo_phoenix/
#endif /* VENDOR_EDIT */

#ifdef VENDOR_EDIT
#* Bin.Xu@BSP.Kernel.Stability, 2019/12/2, Add for oppo force dump */
#obj-$(CONFIG_OPPO_KEY_HANDLE)    +=  oppo_key_handle/
#endif /* VENDOR_EDIT */

#ifdef OPLUS_FEATURE_NFC_CONSOFT
#/* WangYukun@CONNECTIVITY.NFC.BASIC, 2020/03/24, Add for oppo nfc chipset */
obj-$(CONFIG_OPPO_NFC)     +=  oppo_nfc/
#endif /* OPLUS_FEATURE_NFC_CONSOFT */

#ifdef OPLUS_FEATURE_SENSOR_SMEM
obj-$(CONFIG_OPPO_ALS_CALI)   +=  oppo_als_cali_value/
#endif /*OPLUS_FEATURE_SENSOR_SMEM */

#ifdef OPLUS_FEATURE_MEMLEAK_DETECT
#Hailong.Liu@BSP.MM 2019/03/18, Add for svelte
obj-$(CONFIG_OPPO_SVELTE)     +=  oppo_svelte/
#endifi /* OPLUS_FEATURE_MEMLEAK_DETECT */

#ifdef VENDOR_EDIT
#//WuWeiZhong@BSP.Power.Basic, 2020/06/12, add for wakelock profiler
obj-$(CONFIG_OPPO_WAKELOCK_PROFILER)     +=  oppo_wakelock_profiler/
#endif /* VENDOR_EDIT */

#ifdef OPLUS_FEATURE_MEMLEAK_DETECT
#Hailong.Liu@BSP.MM 2019/03/18, Add for svelte
obj-$(CONFIG_OPPO_SVELTE)     +=  oppo_svelte/
#endifi /* OPLUS_FEATURE_MEMLEAK_DETECT */
