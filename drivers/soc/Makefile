# SPDX-License-Identifier: GPL-2.0
#
# Makefile for the Linux Kernel SOC specific device drivers.
#

obj-$(CONFIG_ARCH_ACTIONS)	+= actions/
obj-$(CONFIG_ARCH_AT91)		+= atmel/
obj-y				+= bcm/
obj-$(CONFIG_ARCH_DOVE)		+= dove/
obj-$(CONFIG_MACH_DOVE)		+= dove/
obj-y				+= fsl/
obj-y				+= mediatek/
obj-$(CONFIG_ARCH_MXC)		+= imx/
obj-$(CONFIG_SOC_XWAY)		+= lantiq/
obj-y						+= mediatek/
obj-$(CONFIG_ARCH_MESON)	+= amlogic/
obj-$(CONFIG_ARCH_QCOM)		+= qcom/
obj-y				+= renesas/
obj-$(CONFIG_ARCH_ROCKCHIP)	+= rockchip/
obj-$(CONFIG_SOC_SAMSUNG)	+= samsung/
obj-$(CONFIG_ARCH_SUNXI)	+= sunxi/
obj-$(CONFIG_ARCH_TEGRA)	+= tegra/
obj-$(CONFIG_SOC_TI)		+= ti/
obj-$(CONFIG_ARCH_U8500)	+= ux500/
obj-$(CONFIG_PLAT_VERSATILE)	+= versatile/
obj-$(CONFIG_ARCH_ZX)		+= zte/
#ifdef OPLUS_FEATURE_TP_BASIC
# Bin.Li@EXP.BSP.bootloader.bootflow, 2017/05/15, Add for oppo device info and oppo project
obj-$(CONFIG_OPPO_ARCH_FILE)	+= oppo/
#endif /*OPLUS_FEATURE_TP_BASIC*/

#ifdef OPLUS_FEATURE_TP_BASIC
# Bin.Li@EXP.BSP.bootloader.bootflow, 2017/05/15, Add for oppo device info and oppo project
obj-$(CONFIG_OPPO_ARCH_FILE)    += oppo/
#endif /*OPLUS_FEATURE_TP_BASIC*/

#ifdef OPLUS_FEATURE_IOMONITOR
obj-$(CONFIG_IOMONITOR)    +=  oplus/iomonitor/
#endif /*OPLUS_FEATURE_IOMONITOR*/

#ifdef OPLUS_FEATURE_TP_BASIC
#wen.luo@BSP.Kernel.Stability 2020/03/25 add for oplus kernel system
obj-$(CONFIG_OPLUS_SYSTEM_KERNEL)	+= oplus/system/
#endif /* OPLUS_FEATURE_TP_BASIC */

#ifdef OPLUS_FEATURE_HEALTHINFO
#//wenbin.liu@PSW.Kernel.MM, 2018/05/23, Add for Healthinfo monitor
obj-y     +=  oplus/oppo_healthinfo/
#endif /* OPLUS_FEATURE_HEALTHINFO */

#ifdef OPLUS_FEATURE_MIDAS
obj-$(CONFIG_OPLUS_FEATURE_MIDAS) 	+= oplus/oplus_midas/
#endif /* OPLUS_FEATURE_MIDAS */

#ifdef OPLUS_FEATURE_LOWMEM_DBG
obj-$(CONFIG_OPLUS_FEATURE_LOWMEM_DBG)	+=  oplus/lowmem_dbg/
#endif /* OPLUS_FEATURE_LOWMEM_DBG */
