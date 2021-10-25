ifeq ($(CONFIG_MEDIATEK_SOLUTION),y)
MTK_PLATFORM := $(subst ",,$(CONFIG_MTK_PLATFORM))

ifdef CONFIG_OPLUS_FEATURE_MMCPLUS
MMC_BUILD_FOLDER :=ommc
else
MMC_BUILD_FOLDER :=mmc
endif

ifdef CONFIG_OPLUS_FEATURE_UFSPLUS
UFS_BUILD_FOLDER :=oufs
else
UFS_BUILD_FOLDER :=ufs
endif

ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/core
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/base/power/include
ccflags-y += -I$(srctree)/drivers/devfreq
ccflags-y += -I$(srctree)/drivers/scsi/$(UFS_BUILD_FOLDER)

ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/ComboA
ifeq ($(CONFIG_MTK_PLATFORM), "mt6761")
ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/ComboA/mt6765
else
ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/ComboA/$(MTK_PLATFORM)
endif
ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/$(MTK_PLATFORM)/$(MTK_PLATFORM)

ifeq ($(CONFIG_TRUSTONIC_TEE_SUPPORT),y)
include $(srctree)/drivers/misc/mediatek/gud/Makefile.include
ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/rpmb/drrpmb/public
ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/rpmb/drrpmb_gp/public
ccflags-y += -I$(srctree)/drivers/$(MMC_BUILD_FOLDER)/host/mediatek/rpmb/tlrpmb/public
endif

#
# Attention:
# Enabling/using CFG_RPMB_KEY_PROGRAMED_IN_KERNEL means accepting the
# risk of revealing key in
# Linux Kernel. Mediatek won't take the responsibility for loss incurred
# by the key revealing.
#
#ccflags-y += -DCFG_RPMB_KEY_PROGRAMED_IN_KERNEL

ifeq ($(CONFIG_MTK_HW_FDE_AES),y)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/fde_aes/$(MTK_PLATFORM)
endif

obj-$(CONFIG_RPMB) += rpmb-mtk.o
endif

obj-$(CONFIG_RPMB) += rpmb.o
rpmb-objs += core.o
rpmb-$(CONFIG_RPMB_INTF_DEV) += cdev.o
obj-$(CONFIG_RPMB_SIM) += rpmb_sim.o

ccflags-y += -D__CHECK_ENDIAN__
