#
# Makefile for kernel Silead Fingerprint drivers.
#

ifdef CONFIG_ARCH_MSM
	 ccflags-y += -DBSP_SIL_PLAT_QCOM
endif

ifdef CONFIG_ARCH_QCOM
	 ccflags-y += -DBSP_SIL_PLAT_QCOM
endif

ifdef CONFIG_ARCH_SPRD
	 ccflags-y += -DBSP_SIL_PLAT_SPRD
endif

ifeq ($(CONFIG_OPPO_FINGERPRINT_QCOM),y)
obj-y	+= silead_fp_platform.o
else
obj-y += silead_fp.o
endif

ifeq ($(CONFIG_OPPO_FINGERPRINT_MTK),y)
   #include $(srctree)/drivers/misc/mediatek/Makefile.custom
   ccflags-y += -DBSP_SIL_PLAT_MTK
   ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
   subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
   subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
   subdir-ccflags-y += -I$(srctree)/drivers/spi/trustzone/spi/Tlspi/inc
   include $(srctree)/drivers/misc/mediatek/gud/Makefile.include

endif

