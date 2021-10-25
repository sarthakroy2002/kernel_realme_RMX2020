/***************************************************
 * File:touch.c
 * VENDOR_EDIT
 * Copyright (c)  2008- 2030  Oppo Mobile communication Corp.ltd.
 * Description:
 *             tp dev
 * Version:1.0:
 * Date created:2016/09/02
 * Author: hao.wang@Bsp.Driver
 * TAG: BSP.TP.Init
*/

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/serio.h>
#include "oppo_touchscreen/tp_devices.h"
#include "oppo_touchscreen/touchpanel_common.h"
/* Pan.Chen@BSP.TP.Function, 2020/09/11, bringup add for touchscreen mould. */
#include <soc/oplus/system/oplus_project.h>
#include <soc/oppo/device_info.h>
#include "touch.h"

#define MAX_LIMIT_DATA_LENGTH         100
extern char *saved_command_line;
int g_tp_dev_vendor = TP_UNKNOWN;
int g_tp_prj_id = 0;
struct hw_resource *g_hw_res;
static bool is_tp_type_got_in_match = false;    /*indicate whether the tp type is specified in the dts*/
int tp_type = 0;
/*if can not compile success, please update vendor/oppo_touchsreen*/
struct tp_dev_name tp_dev_names[] = {
	{TP_OFILM, "OFILM"},
	{TP_BIEL, "BIEL"},
	{TP_TRULY, "TRULY"},
	{TP_BOE, "BOE"},
	{TP_G2Y, "G2Y"},
	{TP_TPK, "TPK"},
	{TP_JDI, "JDI"},
	{TP_TIANMA, "TIANMA"},
	{TP_SAMSUNG, "SAMSUNG"},
	{TP_DSJM, "DSJM"},
	{TP_BOE_B8, "BOEB8"},
	{TP_INNOLUX, "INNOLUX"},
	{TP_HIMAX_DPT, "DPT"},
	{TP_AUO, "AUO"},
	{TP_DEPUTE, "DEPUTE"},
	{TP_HUAXING, "HUAXING"},
	{TP_HLT, "HLT"},
	{TP_DJN, "DJN"},
	{TP_TXD, "TXD"},
	{TP_UNKNOWN, "UNKNOWN"},
};

typedef enum {
	TP_INDEX_NULL,
	himax_83112a,
	himax_83112f,
	ili9881_auo,
	ili9881_tm,
	nt36525b_boe,
	nt36525b_hlt,
	nt36672c,
	ili9881_inx,
	ili9881_txd,
	ili9881_truly,
	goodix_gt9886,
	focal_ft3518,
	td4330,
	himax_83112b,
	ili9882n_txd,
} TP_USED_INDEX;
TP_USED_INDEX tp_used_index  = TP_INDEX_NULL;

#define GET_TP_DEV_NAME(tp_type) ((tp_dev_names[tp_type].type == (tp_type))?tp_dev_names[tp_type].name:"UNMATCH")

#ifndef CONFIG_MTK_FB
void primary_display_esd_check_enable(int enable)
{
	return;
}
EXPORT_SYMBOL(primary_display_esd_check_enable);
#endif /*CONFIG_MTK_FB*/

bool __init tp_judge_ic_match(char *tp_ic_name)
{
	pr_err("[TP] tp_ic_name = %s \n", tp_ic_name);
	switch(get_project()) {
	case 132780:
		pr_info("[TP] tp judge ic forward for 206AC\n");
		if (strstr(tp_ic_name, "nf_nt36525b") && strstr(saved_command_line, "nt36525b_hlt")) {
		return true;
		}
		if (strstr(tp_ic_name, "ili9882n") && strstr(saved_command_line, "ilt9882n")) {
		return true;
		}
		pr_err("[TP] ERROR! ic is not match driver\n");
		return false;
	case 19741:
	case 19747:
		pr_info("tp judge ic forward for 19741\n");
		if (strstr(tp_ic_name, "nf_nt36525b") && strstr(boot_command_line, "nt36525b_hlt")) {
			return true;
		}
		if (strstr(tp_ic_name, "ili9881h") && strstr(boot_command_line, "ilt9881h")) {
			return true;
		}
		return false;
	case 19131:
	case 19132:
	case 19133:
	case 19420:
	case 19421:
		pr_info("tp judge ic forward for 19131\n");
		if (strstr(tp_ic_name, "nf_nt36672c") && strstr(saved_command_line, "nt36672c_fhdp_dsi_vdo")) {
			return true;
		}
		if (strstr(tp_ic_name, "hx83112f_nf") && strstr(saved_command_line, "hx83112f_fhdp_dsi_vdo")) {
			return true;
		}
		return false;
	case 20613:
    	case 20611:
    	case 20610:
    	case 20680: 
	case 20686:
		pr_info("tp judge ic forward for 20611\n");
		if (strstr(tp_ic_name, "nf_nt36672c") && strstr(boot_command_line, "nt36672c")) {
			return true;
		}
		if (strstr(tp_ic_name, "hx83112f_nf") && strstr(boot_command_line, "hx83112f")) {
			return true;
		}
		return false;
	

	case 20630:
	case 20631:
	case 20632:
	case 20633:
	case 20634:
	case 20635:
	case 0x206B4:
		pr_info("[TP] case 20630\n");
		is_tp_type_got_in_match = true;
		if (strstr(tp_ic_name, "Goodix-gt9886")&&strstr(boot_command_line, "hx83112f_fhdp_dsi_vdo_dphy_tianma_lcm_drv")) {
			pr_info("[TP] Goodix-gt9886\n");
			tp_type = 1;
            g_tp_dev_vendor = TP_SAMSUNG;
            return true;
        }
		if (strstr(tp_ic_name, "focaltech,fts")&&strstr(boot_command_line, "s68fc01")) {
			pr_info("[TP] focaltech 3518\n");
			tp_type = 0;
            g_tp_dev_vendor = TP_SAMSUNG;
            return true;
        }
		break;		
	case 18531:
	case 18561:
	case 18161:
		pr_info("[TP] tp judge ic forward for 18531\n");
		if (strstr(tp_ic_name, "hx83112a_nf") && strstr(saved_command_line, "_himax83112a")) {
			return true;
		} else if (strstr(tp_ic_name, "hx83112b_nf") && strstr(saved_command_line, "_himax83112b")) {
			return true;
		} else if (strstr(tp_ic_name, "td4320_nf") && strstr(saved_command_line, "_td4330")) {
			return true;
		} else if (strstr(tp_ic_name, "nt36672") && strstr(saved_command_line, "tm_nt36670a")) {
			return true;
		}
		pr_err("[TP] ERROR! ic is not match driver\n");
		return false;
	default:
		break;
	}
	return true;
}

bool  tp_judge_ic_match_commandline(struct panel_info *panel_data)
{
	int prj_id = 0;
	int i = 0;
	bool ic_matched = false;
	prj_id = get_project();
	pr_err("[TP] get_project() = %d \n", prj_id);
	pr_err("[TP] boot_command_line = %s \n", saved_command_line);

	for (i = 0; i < panel_data->project_num; i++) {
		if (prj_id == panel_data->platform_support_project[i]) {
			g_tp_prj_id = panel_data->platform_support_project_dir[i];

			if (strstr(saved_command_line, panel_data->platform_support_commandline[i])
					|| strstr("default_commandline", panel_data->platform_support_commandline[i])) {
				pr_err("[TP] Driver match the project\n");
				ic_matched = true;
			}
		}
	}

	if (!ic_matched) {
		pr_err("[TP] Driver does not match the project\n");
		pr_err("Lcd module not found\n");
		return false;
	}

	switch (prj_id) {
	case 19741:
	case 19747:
		pr_info("[TP] case 19741\n");
		is_tp_type_got_in_match = true;

		if (strstr(saved_command_line, "nt36525b_hlt")) {
			pr_err("[TP] touch ic = nt36525b_hlt \n");
			tp_used_index = nt36525b_hlt;
			g_tp_dev_vendor = TP_HLT;
		}

		if (strstr(saved_command_line, "ilt9881h_txd")) {
			pr_err("[TP] touch ic = ilt9881h_txd \n");
			tp_used_index = ili9881_txd;
			g_tp_dev_vendor = TP_TXD;
		}
		if (strstr(saved_command_line, "ilt9881h_truly")) {
			pr_err("[TP] touch ic = ilt9881h_truly \n");
			tp_used_index = ili9881_truly;
			g_tp_dev_vendor = TP_TRULY;
		}
		break;
	case 19661:
	case 19662:
	case 19663:
	case 19665:
	case 19666:
	case 19668:
		pr_info("[TP] case 19661\n");
		is_tp_type_got_in_match = true;

		if (strstr(saved_command_line, "nt36672c_tianma")) {
			pr_err("[TP] touch ic = nt36672c_tianma \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_TIANMA;
		}

		if (strstr(saved_command_line, "nt36672c_jdi")) {
			pr_err("[TP] touch ic = nt36672c_jdi \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_JDI;
		}
		break;
	case 19131:
	case 19132:
	case 19133:
	case 19420:
	case 19421:

#ifdef CONFIG_MACH_MT6873
		pr_info("[TP] case 19131\n");
		is_tp_type_got_in_match = true;

		if (strstr(saved_command_line, "nt36672c_fhdp_dsi_vdo_auo_cphy_90hz_tianma")) {
			pr_err("[TP] touch ic = nt36672c_tianma \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_TIANMA;
		}

		if (strstr(saved_command_line, "nt36672c_fhdp_dsi_vdo_auo_cphy_120hz_tianma")) {
			pr_err("[TP] touch ic = nt36672c_tianma \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_TIANMA;
		}

		if (strstr(saved_command_line, "nt36672c_fhdp_dsi_vdo_auo_cphy_90hz_jdi")) {
			pr_err("[TP] touch ic = nt36672c_jdi \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_JDI;
		}

		if (strstr(saved_command_line, "hx83112f_fhdp_dsi_vdo_auo_cphy_90hz_jdi")) {
			pr_err("[TP] touch ic = hx83112f_jdi \n");
			tp_used_index = himax_83112f;
			g_tp_dev_vendor = TP_JDI;
		}

#endif
		break;

	case 20001:
	case 20002:
	case 20003:
	case 20200:
		pr_info("[TP] case 20001\n");
		is_tp_type_got_in_match = true;

		if (strstr(saved_command_line, "nt36672c")) {
			pr_err("[TP] touch ic = nt36672c_jdi \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_JDI;
		}

		if (strstr(saved_command_line, "hx83112f")) {
			pr_err("[TP] touch ic = hx83112f_tianma \n");
			tp_used_index = himax_83112f;
			g_tp_dev_vendor = TP_TIANMA;
		}
		break;

	case 20613:
	case 20611:
	case 20610:
	case 20680:
	case 20686:
		pr_info("[TP] case 20611\n");
		is_tp_type_got_in_match = true;
		if (strstr(saved_command_line, "nt36672c")) {
			pr_err("[TP] touch ic = nt36672c_jdi \n");
			tp_used_index = nt36672c;
			g_tp_dev_vendor = TP_JDI;
		}

		if (strstr(saved_command_line, "hx83112f")) {
			pr_err("[TP] touch ic = hx83112f_tianma \n");
			tp_used_index = himax_83112f;
			g_tp_dev_vendor = TP_TIANMA;
		}
        break;
	/*Xiaofei.Gong@BSP.TP.Function, 2020/10/17, add TP for pascal touchscreen.*/
	case 132780:
		pr_info("[TP] case 206AC\n");
		is_tp_type_got_in_match = true;

		if (strstr(saved_command_line, "nt36525b_hlt_psc_ac_boe_vdo")) {
			tp_used_index = nt36525b_hlt;
			g_tp_dev_vendor = TP_HLT;
			pr_err("[TP] 206AC tp vendor hlt_b3\n");
		}
	/*TP_JDI is TP_HLT_B8*/
		if (strstr(saved_command_line, "nt36525b_hlt_psc_ac_vdo")) {
			g_tp_dev_vendor = TP_JDI;
			tp_used_index = nt36525b_hlt;
			pr_err("[TP] 206AC tp vendor hlt_b8\n");
		}
		if (strstr(saved_command_line, "ilt9882n_txd")) {
			g_tp_dev_vendor = TP_TXD;
			tp_used_index = ili9882n_txd;
			pr_err("[TP] 206AC tp vendor ili9882n_txd\n");
		}
		break;
	/*Xiaofei.Gong@BSP.TP.Function, 2020/09/15, bringup add for sala touchscreen.*/
	case 20682:
		pr_info("[TP] case 20682\n");
		is_tp_type_got_in_match = true;
		tp_used_index = nt36672c;

		if (strstr(saved_command_line, "nt36672c_tianma")) {
			g_tp_dev_vendor = TP_TIANMA;

		} else if (strstr(saved_command_line, "nt36672c_jdi")) {
			g_tp_dev_vendor = TP_JDI;

		} else if (strstr(saved_command_line, "nt36672c_boe")) {
			g_tp_dev_vendor = TP_BOE;
		} else {
			g_tp_dev_vendor = TP_UNKNOWN;
		}

		pr_err("[TP] g_tp_dev_vendor: %s\n", tp_dev_names[g_tp_dev_vendor].name);
        break;
	/*Xiaofei.Gong@BSP.TP.Function, 2020/09/15, bringup add for sala touchscreen.*/
	case 18531:
	case 18561:
	case 18161:
		is_tp_type_got_in_match = true;

		if (strstr(saved_command_line, "dsjm_jdi_himax83112b")) {
			tp_used_index = himax_83112b;
			g_tp_dev_vendor = TP_JDI;
		} else if (strstr(saved_command_line, "djn_jdi_himax83112b")) {
			tp_used_index = himax_83112b;
			g_tp_dev_vendor = TP_DJN;
		} else if (strstr(saved_command_line, "tm_himax83112")) {
			tp_used_index = himax_83112a;
			g_tp_dev_vendor = TP_TIANMA;
		} else if (strstr(saved_command_line, "jdi_himax83112a")) {
			tp_used_index = himax_83112a;
			g_tp_dev_vendor = TP_JDI;
		} else if (strstr(saved_command_line, "tianma_himax83112a")) {
			tp_used_index = himax_83112a;
			g_tp_dev_vendor = TP_TIANMA;
		} else if (strstr(saved_command_line, "dsjm_himax83112")) {
			tp_used_index = himax_83112a;
			g_tp_dev_vendor = TP_DSJM;
		} else if (strstr(saved_command_line, "dsjm_jdi_td4330")) {
			tp_used_index = td4330;
			g_tp_dev_vendor = TP_DSJM;
		} else if (strstr(saved_command_line, "dpt_jdi_td4330")) {
			tp_used_index = td4330;
			g_tp_dev_vendor = TP_HIMAX_DPT;
		} else if (strstr(saved_command_line, "tianma_td4330")) {
			tp_used_index = td4330;
			g_tp_dev_vendor = TP_TIANMA;
		} else if (strstr(saved_command_line, "tm_nt36670a")) {
			pr_info("chip_name is nt36672a_nf, vendor is tianma\n");
		}

		pr_err("[TP] g_tp_dev_vendor: %s\n", tp_dev_names[g_tp_dev_vendor].name);
		break;

	default:
		pr_info("other project, no need process here!\n");
		break;
	}

	pr_info("[TP]ic:%d, vendor:%d\n", tp_used_index, g_tp_dev_vendor);
	return true;
}

int tp_util_get_vendor(struct hw_resource *hw_res,
		       struct panel_info *panel_data)
{
	char *vendor;
	int prj_id = 0;
	prj_id = get_project();
	g_hw_res = hw_res;

	panel_data->test_limit_name = kzalloc(MAX_LIMIT_DATA_LENGTH, GFP_KERNEL);

	if (panel_data->test_limit_name == NULL) {
		pr_err("[TP]panel_data.test_limit_name kzalloc error\n");
	}

	panel_data->extra = kzalloc(MAX_LIMIT_DATA_LENGTH, GFP_KERNEL);

	if (panel_data->extra == NULL) {
		pr_err("[TP]panel_data.extra kzalloc error\n");
	}

	if (is_tp_type_got_in_match) {
		panel_data->tp_type = g_tp_dev_vendor;
	}
if ( prj_id == 20630 || prj_id == 20631 || prj_id == 20632 || prj_id == 20633 || prj_id == 20634 || prj_id == 20635 || prj_id == 0x206B4 ) {
			panel_data->tp_type = TP_SAMSUNG;
			if(tp_type){
					memcpy(panel_data->manufacture_info.version, "goodix_", 7);
			}else{
					memcpy(panel_data->manufacture_info.version, "focaltech_", 10);
			}
}
	if (panel_data->tp_type == TP_UNKNOWN) {
		pr_err("[TP]%s type is unknown\n", __func__);
		return 0;
	}

	if (prj_id == 19165 || prj_id == 19166) {
	    memcpy(panel_data->manufacture_info.version, "0xBD3100000", 11);
	}
	if (prj_id == 20075 || prj_id == 20076) {
	    memcpy(panel_data->manufacture_info.version, "0xRA5230000", 11);
	}

	vendor = GET_TP_DEV_NAME(panel_data->tp_type);

	/*Xuhang.Li@BSP.TP.Function, 2020/09/18, add tp path for chaka.*/
	if (prj_id == 20131 || prj_id == 20133 || prj_id == 20255 || prj_id == 20257
		|| prj_id == 20644 || prj_id == 20645 || prj_id == 20649 || prj_id == 0x2064A || prj_id == 0x2068D) {
		memcpy(panel_data->manufacture_info.version, "0xbd3520000", 11);
	}

	if (strstr(saved_command_line, "oppo20131_tianma_nt37701")) {
		hw_res->TX_NUM = 18;
		hw_res->RX_NUM = 40;
		vendor = "TIANMA";
	}

	/*Xuhang.Li@BSP.TP.Function, 2020/09/18, add tp path for chaka.*/


        if (strstr(saved_command_line, "nt36672c_boe_sala")) {
                hw_res->TX_NUM = 18;
                hw_res->RX_NUM = 36;
        }

	strcpy(panel_data->manufacture_info.manufacture, vendor);

	switch (prj_id) {
	case 19741:
	case 19747:
		pr_info("[TP] enter case 19741\n");

		if ((tp_used_index == nt36525b_hlt) && (g_tp_dev_vendor == TP_HLT)) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/19741/FW_%s_%s.bin",
				 "NF_NT36525B", "HLT");

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/19741/LIMIT_%s_%s.img",
					 "NF_NT36525B", "HLT");
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			pr_info("[TP]: firmware_headfile = FW_19741_NT36525B_HLT\n");
			strcpy(panel_data->manufacture_info.manufacture, "HLT");
			memcpy(panel_data->manufacture_info.version, "NT36525B_HLT", 12);
			panel_data->firmware_headfile.firmware_data = FW_19741_NT36525B_HLT;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_19741_NT36525B_HLT);
		}
		if ((tp_used_index == ili9881_txd) && (g_tp_dev_vendor == TP_TXD)) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/19741/FW_%s_%s.bin",
				 "NF_ILT9881H", vendor);

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/19741/LIMIT_%s_%s.ini",
					 "NF_ILT9881H", vendor);
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			pr_info("[TP]: firmware_headfile = FW_19741_ILI9881H_TXD\n");
			memcpy(panel_data->manufacture_info.version, "TXD_ILI9881H", 12);
			panel_data->firmware_headfile.firmware_data = FW_19741_ILI9881H_TXD;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_19741_ILI9881H_TXD);
		}
		if ((tp_used_index == ili9881_truly) && (g_tp_dev_vendor == TP_TRULY)) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/19741/FW_%s_%s.bin",
				 "NF_ILT9881H", vendor);

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/19741/LIMIT_%s_%s.ini",
					 "NF_ILT9881H", vendor);
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			pr_info("[TP]: firmware_headfile = FW_19741_ILI9881H_TRULY\n");
			memcpy(panel_data->manufacture_info.version, "XL_ILI9881H_", 12);
			panel_data->firmware_headfile.firmware_data = FW_19741_ILI9881H_TRULY;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_19741_ILI9881H_TRULY);
		}
		break;
	case 19661:
	case 19662:
	case 19663:
	case 19665:
	case 19666:
	case 19668:
		pr_info("[TP] enter case OPPO_19661\n");

		if (tp_used_index == nt36672c) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/19661/FW_%s_%s.bin",
				 "NT36672C_NF", vendor);

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/19661/LIMIT_%s_%s.img",
					 "NT36672C_NF", vendor);
			}

			if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					 "tp/19661/BOOT_FW_%s_%s.ihex",
					 "NT36672C_NF", vendor);
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;

			if ((tp_used_index == nt36672c) && (g_tp_dev_vendor == TP_JDI)) {
				pr_info("[TP]: firmware_headfile = FW_19661_NT36672C_JDI\n");
				memcpy(panel_data->manufacture_info.version, "NT72C_JDI", 9);
				panel_data->firmware_headfile.firmware_data = FW_19661_NT36672C_JDI;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_19661_NT36672C_JDI);
			}

			if ((tp_used_index == nt36672c) && (g_tp_dev_vendor == TP_TIANMA)) {
				pr_info("[TP]: firmware_headfile = FW_19661_NT36672C_TIANMA\n");
				memcpy(panel_data->manufacture_info.version, "NT72C_TM_", 9);
				panel_data->firmware_headfile.firmware_data = FW_19661_NT36672C_TIANMA;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_19661_NT36672C_TIANMA);
			}
		}
		break;
	case 19131:
	case 19132:
	case 19133:
	case 19420:
	case 19421:
#ifdef CONFIG_MACH_MT6873
		pr_info("[TP] enter case OPPO_19131\n");

		if (tp_used_index == nt36672c) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/19131/FW_%s_%s.img",
				 "NF_NT36672C", vendor);

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/19131/LIMIT_%s_%s.img",
					 "NF_NT36672C", vendor);
			}

			if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					 "tp/19131/BOOT_FW_%s_%s.ihex",
					 "NF_NT36672C", vendor);
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;

			if ((tp_used_index == nt36672c) && (g_tp_dev_vendor == TP_JDI)) {
				pr_info("[TP]: firmware_headfile = FW_19131_NF_NT36672C_JDI_fae_jdi\n");
				memcpy(panel_data->manufacture_info.version, "0xDD300JN200", 12);
				/*panel_data->firmware_headfile.firmware_data = FW_19131_NF_NT36672C_JDI;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_19131_NF_NT36672C_JDI);*/
				panel_data->firmware_headfile.firmware_data = FW_19131_NF_NT36672C_JDI_fae_jdi;
                panel_data->firmware_headfile.firmware_size = sizeof(FW_19131_NF_NT36672C_JDI_fae_jdi);
			}

			if ((tp_used_index == nt36672c) && (g_tp_dev_vendor == TP_TIANMA)) {
				pr_info("[TP]: firmware_headfile = FW_19131_NF_NT36672C_TIANMA_fae_tianma\n");
				memcpy(panel_data->manufacture_info.version, "0xDD300TN000", 12);
				/*panel_data->firmware_headfile.firmware_data = FW_19131_NF_NT36672C_TIANMA_realme;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_19131_NF_NT36672C_TIANMA_realme);*/
                panel_data->firmware_headfile.firmware_data = FW_19131_NF_NT36672C_TIANMA_fae_tianma;
                panel_data->firmware_headfile.firmware_size = sizeof(FW_19131_NF_NT36672C_TIANMA_fae_tianma);
			}
		}

		if (tp_used_index == himax_83112f) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/19131/FW_%s_%s.img",
				 "NF_HX83112F", vendor);

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/19131/LIMIT_%s_%s.img",
					 "NF_HX83112F", vendor);
			}

			if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					 "tp/19131/BOOT_FW_%s_%s.ihex",
					 "NF_HX83112F", vendor);
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;

			if (tp_used_index == himax_83112f) {
				pr_info("[TP]: firmware_headfile = FW_19131_NF_HX83112F_JDI\n");
				memcpy(panel_data->manufacture_info.version, "0xDD300JH000", 12);
				panel_data->firmware_headfile.firmware_data = FW_19131_NF_HX83112F_JDI;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_19131_NF_HX83112F_JDI);
			}
		}

#endif
		break;

	case 20051:
		pr_info("[TP] enter case OPPO_20051\n");

		if (tp_used_index == nt36672c) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/20051/FW_%s_%s.img",
				 "NF_NT36672C", "JDI");

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/20051/LIMIT_%s_%s.img",
					 "NF_NT36672C", "JDI");
			}

			if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					 "tp/20051/BOOT_FW_%s_%s.ihex",
					 "NF_NT36672C", "JDI");
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;

			if ((tp_used_index == nt36672c) && (g_tp_dev_vendor == TP_JDI)) {
				pr_info("[TP]: firmware_headfile = FW_20051_NF_NT36672C_JDI_fae_jdi\n");
				memcpy(panel_data->manufacture_info.version, "0xBD358JN200", 12);
				panel_data->firmware_headfile.firmware_data = FW_20051_NF_NT36672C_JDI_fae_jdi;
                panel_data->firmware_headfile.firmware_size = sizeof(FW_20051_NF_NT36672C_JDI_fae_jdi);
			}
		}

		break;

	case 20613:
	case 20611:
	case 20610:
	case 20680:
	case 20686:
		pr_info("[TP] enter case OPPO_20611\n");

		if (tp_used_index == nt36672c) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/20611/FW_%s_%s.img",
				 "NF_NT36672C", "JDI");

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/20611/LIMIT_%s_%s.img",
					 "NF_NT36672C", "JDI");
			}

			if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					 "tp/20611/BOOT_FW_%s_%s.ihex",
					 "NF_NT36672C", "JDI");
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;

			if ((tp_used_index == nt36672c) && (g_tp_dev_vendor == TP_JDI)) {
				pr_info("[TP]: firmware_headfile = FW_20001_NF_NT36672C_JDI\n");
				memcpy(panel_data->manufacture_info.version, "0xFA356DN", 9);
				panel_data->firmware_headfile.firmware_data = FW_20001_NF_NT36672C_JDI;
                panel_data->firmware_headfile.firmware_size = sizeof(FW_20001_NF_NT36672C_JDI);
			}
		}
		if (tp_used_index == himax_83112f) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
                     		"tp/20611/FW_%s_%s.img",
                     		"NF_HX83112F", vendor);

            		if (panel_data->test_limit_name) {
                	snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
                         		"tp/20611/LIMIT_%s_%s.img",
                         		"NF_HX83112F", vendor);
            		}

            		if (panel_data->extra) {
                	snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
                         	"tp/20611/BOOT_FW_%s_%s.ihex",
                         	"NF_HX83112F", vendor);
            		}
            	panel_data->manufacture_info.fw_path = panel_data->fw_name;
            		if (tp_used_index == himax_83112f) {
                	pr_info("[TP]: firmware_headfile = FW_20001_NF_HX83112F_TIANMA\n");
                	memcpy(panel_data->manufacture_info.version, "0xFA219TH", 9);
                	panel_data->firmware_headfile.firmware_data = FW_20001_NF_HX83112F_TIANMA;
                	panel_data->firmware_headfile.firmware_size = sizeof(FW_20001_NF_HX83112F_TIANMA);
            		}
        	}

		break;
	case 20001:
	case 20002:
	case 20003:
	case 20200:
		pr_info("[TP] enter case OPPO_20001\n");

		if (tp_used_index == nt36672c) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/20001/FW_%s_%s.img",
				 "NF_NT36672C", vendor);

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/20001/LIMIT_%s_%s.img",
					 "NF_NT36672C", vendor);
			}

			if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					 "tp/20001/BOOT_FW_%s_%s.ihex",
					 "NF_NT36672C", vendor);
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;

			if (tp_used_index == nt36672c) {
				pr_info("[TP]: firmware_headfile = FW_20001_NF_NT36672C_JDI_fae_jdi\n");
				memcpy(panel_data->manufacture_info.version, "0xFA219DN", 9);
				panel_data->firmware_headfile.firmware_data = FW_20001_NF_NT36672C_JDI;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_20001_NF_NT36672C_JDI);
			}
		}

		if (tp_used_index == himax_83112f) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				 "tp/20001/FW_%s_%s.img",
				 "NF_HX83112F", vendor);

			if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					 "tp/20001/LIMIT_%s_%s.img",
					 "NF_HX83112F", vendor);
			}

			if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					 "tp/20001/BOOT_FW_%s_%s.ihex",
					 "NF_HX83112F", vendor);
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;

			if (tp_used_index == himax_83112f) {
				pr_info("[TP]: firmware_headfile = FW_20001_NF_HX83112F_TIANMA\n");
				memcpy(panel_data->manufacture_info.version, "0xFA219TH", 9);
				panel_data->firmware_headfile.firmware_data = FW_20001_NF_HX83112F_TIANMA;
                panel_data->firmware_headfile.firmware_size = sizeof(FW_20001_NF_HX83112F_TIANMA);
			}
		}

		break;

	/* Pan.Chen@BSP.TP.Function, 2020/09/11, bringup add for touchscreen. */
	case 132780:
		pr_info("[TP] enter case 206AC\n");

		if ((tp_used_index == nt36525b_hlt) && (g_tp_dev_vendor == TP_HLT)) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				"tp/206AC/FW_%s_%s.bin",
				"NF_NT36525B", vendor);

			if (panel_data->test_limit_name) {
			snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				 "tp/206AC/LIMIT_%s_%s.img",
				 "NF_NT36525B", vendor);
			}

			if (panel_data->extra) {
			snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
				 "tp/206AC/BOOT_FW_%s_%s.ihex",
				 "NF_NT36525B", vendor);
			}
			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			memcpy(panel_data->manufacture_info.version, "HLT_B3_NT25_", 12);
			panel_data->firmware_headfile.firmware_data = FW_206AC_NF_NT36525B_HLT;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_206AC_NF_NT36525B_HLT);
		}
	/*TP_JDI is TP_HLT_B8*/
		if ((tp_used_index == nt36525b_hlt) && (g_tp_dev_vendor == TP_JDI)) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				"tp/206AC/FW_%s_%s.bin",
				"NF_NT36525B", "HLTB8");

			if (panel_data->test_limit_name) {
			snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				"tp/206AC/LIMIT_%s_%s.img",
				"NF_NT36525B", "HLTB8");
			}

			if (panel_data->extra) {
			snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
				"tp/206AC/BOOT_FW_%s_%s.ihex",
				"NF_NT36525B", "HLTB8");
			}
			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			strcpy(panel_data->manufacture_info.manufacture, "HLT");
			memcpy(panel_data->manufacture_info.version, "HLT_B8_NT25_", 12);
			panel_data->firmware_headfile.firmware_data = FW_206AC_NF_NT36525B_HLT_B8;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_206AC_NF_NT36525B_HLT_B8);
		}
		if ((tp_used_index == ili9882n_txd) && (g_tp_dev_vendor == TP_TXD)) {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
				"tp/206AC/FW_%s_%s.bin",
				"NF_ILT9882N", "TXD");

			if (panel_data->test_limit_name) {
			snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				"tp/206AC/LIMIT_%s_%s.ini",
				"NF_ILT9882N", "TXD");
			}

			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			strcpy(panel_data->manufacture_info.manufacture, "TXD");
			memcpy(panel_data->manufacture_info.version, "TXD_ILI882N_", 12);
			panel_data->firmware_headfile.firmware_data = FW_206AC_NF_ILT9882N_TXD;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_206AC_NF_ILT9882N_TXD);
		}
		break;

	/*Xiaofei.Gong@BSP.TP.Function, 2020/09/15, bringup add for sala touchscreen.*/
	case 20682:
		pr_info("[TP] enter case 20682\n");
		if (tp_used_index == nt36672c) {
			if (g_tp_dev_vendor == TP_TIANMA) {
				pr_info("[TP] 20682 tp vendor tianma\n");
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
					"tp/20682/FW_%s_%s.bin",
					"NT36672C_NF", vendor);
				if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					"tp/20682/LIMIT_%s_%s.img",
					"NT36672C_NF", vendor);
				}
				if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					"tp/20682/BOOT_FW_%s_%s.ihex",
					"NT36672C_NF", vendor);
				}
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "NT72C_TM_", 9);
				panel_data->firmware_headfile.firmware_data = FW_20682_NT36672C_TIANMA;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_20682_NT36672C_TIANMA);
			} else if (g_tp_dev_vendor == TP_JDI) {
				pr_info("[TP] 20682 tp vendor jdi\n");
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
					"tp/20682/FW_%s_%s.bin",
					"NT36672C_NF", vendor);

				if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					"tp/20682/LIMIT_%s_%s.img",
					"NT36672C_NF", vendor);
				}

				if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					"tp/20682/BOOT_FW_%s_%s.ihex",
					"NT36672C_NF", vendor);
				}
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "NT72C_JDI", 9);
				panel_data->firmware_headfile.firmware_data = FW_20682_NT36672C_JDI;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_20682_NT36672C_JDI);
			} else if (g_tp_dev_vendor == TP_BOE) {
				pr_info("[TP] 20682 tp vendor boe\n");
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
					"tp/20682/FW_%s_%s.bin",
					"NT36672C_NF", vendor);

				if (panel_data->test_limit_name) {
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
					"tp/20682/LIMIT_%s_%s.img",
					"NT36672C_NF", vendor);
				}

				if (panel_data->extra) {
				snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
					"tp/20682/BOOT_FW_%s_%s.ihex",
					"NT36672C_NF", vendor);
				}
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "NT72C_BOE", 9);
				panel_data->firmware_headfile.firmware_data = FW_20682_NT36672C_BOE;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_20682_NT36672C_BOE);
			} else {
				pr_info("[TP] 20682 tp ic not found\n");
			}
		}
		break;

	/*Xiaofei.Gong@BSP.TP.Function, 2020/09/15, bringup add for sala touchscreen.*/
	case 18531:
		pr_info("[TP] project:%d\n", g_tp_prj_id);

		if (tp_used_index == himax_83112a) {
			if (g_tp_dev_vendor == TP_DSJM) {
                snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112A_NF_DSJM.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xBD1203", 8);
				panel_data->firmware_headfile.firmware_data = FW_18311_HX83112A_NF_DSJM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18311_HX83112A_NF_DSJM);

			} else if (g_tp_dev_vendor == TP_JDI) {
                snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112A_NF_JDI.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD0750", 8);
				panel_data->firmware_headfile.firmware_data = FW_18531_HX83112A_NF_JDI;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_HX83112A_NF_JDI);

			} else if (g_tp_dev_vendor == TP_TIANMA) {
                snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112A_NF_TIANMA.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD0751", 8);
				panel_data->firmware_headfile.firmware_data = FW_18531_HX83112A_NF_TM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_HX83112A_NF_TM);
			}
		} else if (tp_used_index == himax_83112b) {
            snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112B_NF_DSJM.img", g_tp_prj_id);
			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			memcpy(panel_data->manufacture_info.version, "0xDD0755", 8);
			panel_data->firmware_headfile.firmware_data = FW_18531_HX83112B_NF_DSJM;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_HX83112B_NF_DSJM);

		} else if (tp_used_index == td4330) {
			if (g_tp_dev_vendor == TP_DSJM) {
                snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_TD4330_NF_DSJM.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD075E", 8);
				panel_data->firmware_headfile.firmware_data = FW_18531_TD4330_NF_DSJM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_TD4330_NF_DSJM);

			} else if (g_tp_dev_vendor == TP_HIMAX_DPT) {
                snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_TD4330_NF_DPT.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD075D", 8);
				panel_data->firmware_headfile.firmware_data = FW_18531_TD4330_NF_DPT;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_TD4330_NF_DPT);

			} else if (g_tp_dev_vendor == TP_TIANMA) {
                snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_TD4330_NF_TIANMA.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD075A", 8);
				panel_data->firmware_headfile.firmware_data = FW_18531_TD4330_NF_TM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_TD4330_NF_TM);
			}
			if (panel_data->test_limit_name)
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				"tp/%d/LIMIT_TD4330_NF_%s.img",
				g_tp_prj_id, vendor);
		} else {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_NT36672A_NF_TIANMA.bin", g_tp_prj_id);
			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			memcpy(panel_data->manufacture_info.version, "0xDD0752", 8);
			panel_data->firmware_headfile.firmware_data = FW_18531_NT36672A_NF_TM;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_NT36672A_NF_TM);
			if (panel_data->test_limit_name)
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				"tp/%d/LIMIT_NT36672A_NF_%s.img",
				g_tp_prj_id, vendor);
		}
		break;
	case 18561:
	case 18161:
		pr_info("[TP] project:%d\n", g_tp_prj_id);

		if (tp_used_index == himax_83112a) {
			if (g_tp_dev_vendor == TP_DSJM) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112A_NF_DSJM.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xBD1203", 8);
				panel_data->firmware_headfile.firmware_data = FW_18311_HX83112A_NF_DSJM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18311_HX83112A_NF_DSJM);
			} else if (g_tp_dev_vendor == TP_JDI) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112A_NF_JDI.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD0750", 8);
				panel_data->firmware_headfile.firmware_data = FW_18561_HX83112A_NF_JDI;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18561_HX83112A_NF_JDI);
			} else if (g_tp_dev_vendor == TP_TIANMA) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112A_NF_TIANMA.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD0751", 8);
				panel_data->firmware_headfile.firmware_data = FW_18561_HX83112A_NF_TM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18561_HX83112A_NF_TM);
			}
			if (panel_data->test_limit_name)
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				"tp/%d/LIMIT_HX83112A_NF_%s.img",
				g_tp_prj_id, vendor);
		} else if (tp_used_index == himax_83112b) {
			if (g_tp_dev_vendor == TP_DSJM) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112B_NF_DSJM.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD0755", 8);
				panel_data->firmware_headfile.firmware_data = FW_18531_HX83112B_NF_DSJM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_HX83112B_NF_DSJM);
			} else if (g_tp_dev_vendor == TP_DJN) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_HX83112B_NF_DJN.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xCC1250", 8);
				panel_data->firmware_headfile.firmware_data = FW_18561_HX83112B_NF_DJN;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18561_HX83112B_NF_DJN);
			}
		} else if (tp_used_index == td4330) {
			if (g_tp_dev_vendor == TP_DSJM) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_TD4330_NF_DSJM.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD075E", 8);
				panel_data->firmware_headfile.firmware_data = FW_18561_TD4330_NF_DSJM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18561_TD4330_NF_DSJM);
			} else if (g_tp_dev_vendor == TP_HIMAX_DPT) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_TD4330_NF_DPT.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD075D", 8);
				panel_data->firmware_headfile.firmware_data = FW_18561_TD4330_NF_DPT;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18561_TD4330_NF_DPT);
			} else if (g_tp_dev_vendor == TP_TIANMA) {
				snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_TD4330_NF_TIANMA.img", g_tp_prj_id);
				panel_data->manufacture_info.fw_path = panel_data->fw_name;
				memcpy(panel_data->manufacture_info.version, "0xDD075A", 8);
				panel_data->firmware_headfile.firmware_data = FW_18561_TD4330_NF_TM;
				panel_data->firmware_headfile.firmware_size = sizeof(FW_18561_TD4330_NF_TM);
			}
			if (panel_data->test_limit_name)
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				"tp/%d/LIMIT_TD4330_NF_%s.img",
				g_tp_prj_id, vendor);
		} else {
			snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH, "tp/%d/FW_NT36672A_NF_TIANMA.bin", g_tp_prj_id);
			panel_data->manufacture_info.fw_path = panel_data->fw_name;
			memcpy(panel_data->manufacture_info.version, "0xDD0752", 8);
			panel_data->firmware_headfile.firmware_data = FW_18531_NT36672A_NF_TM;
			panel_data->firmware_headfile.firmware_size = sizeof(FW_18531_NT36672A_NF_TM);
			if (panel_data->test_limit_name)
				snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				"tp/%d/LIMIT_NT36672A_NF_%s.img",
				g_tp_prj_id, vendor);
		}
		break;
	/*Xuhang.Li@BSP.TP.Function, 2020/09/24, add tp path for chaka.*/
	case 20131:
	case 20133:
	case 20255:
	case 20257:
	case 20644:
	case 20645:
        case 20649:
	case 0x2064A:
	case 0x2068D:
		snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
			 "tp/20131/FW_%s_%s.%s",
			  panel_data->chip_name, vendor, !strcmp(vendor,
					"SAMSUNG") ? "bin" : "img");

		if (panel_data->test_limit_name) {
			snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				 "tp/20131/LIMIT_%s_%s.img",
				 panel_data->chip_name, vendor);
		}
		panel_data->manufacture_info.fw_path = panel_data->fw_name;
		break;

	/*Xuhang.Li@BSP.TP.Function, 2020/09/24, add end*/
	case 20630:
	case 20631:
	case 20632:
	case 20633:
	case 20634:
	case 20635:
	case 0x206B4:
		snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
                 "tp/20630/FW_%s_%s.img",
                 panel_data->chip_name, vendor);

        if (panel_data->test_limit_name) {
            snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
                     "tp/20630/LIMIT_%s_%s.img",
                     panel_data->chip_name, vendor);
        }

        if (panel_data->extra) {
            snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
                     "tp/20630/BOOT_FW_%s_%s.ihex",
                     panel_data->chip_name, vendor);
        }
        panel_data->manufacture_info.fw_path = panel_data->fw_name;
        break;
	default:
		snprintf(panel_data->fw_name, MAX_FW_NAME_LENGTH,
			 "tp/%d/FW_%s_%s.img",
			 g_tp_prj_id, panel_data->chip_name, vendor);

		if (panel_data->test_limit_name) {
			snprintf(panel_data->test_limit_name, MAX_LIMIT_DATA_LENGTH,
				 "tp/%d/LIMIT_%s_%s.img",
				 g_tp_prj_id, panel_data->chip_name, vendor);
		}

		if (panel_data->extra) {
			snprintf(panel_data->extra, MAX_LIMIT_DATA_LENGTH,
				 "tp/%d/BOOT_FW_%s_%s.ihex",
				 prj_id, panel_data->chip_name, vendor);
		}

		panel_data->manufacture_info.fw_path = panel_data->fw_name;
		break;
	}

	pr_info("[TP]vendor:%s fw:%s limit:%s\n",
		vendor,
		panel_data->fw_name,
		panel_data->test_limit_name == NULL ? "NO Limit" : panel_data->test_limit_name);

	return 0;
}

/**
 * Description:
 * pulldown spi7 cs to avoid current leakage
 * because of current sourcing from cs (pullup state) flowing into display module
 **/
void switch_spi7cs_state(bool normal)
{
    if(normal){
        if( !IS_ERR_OR_NULL(g_hw_res->pin_set_high) ) {
            pr_info("%s: going to set spi7 cs to spi mode .\n", __func__);
            pinctrl_select_state(g_hw_res->pinctrl, g_hw_res->pin_set_high);
        }else{
            pr_info("%s: cannot to set spi7 cs to spi mode .\n", __func__);
        }
    } else {
        if( !IS_ERR_OR_NULL(g_hw_res->pin_set_low) ) {
            pr_info("%s: going to set spi7 cs to pulldown .\n", __func__);
            pinctrl_select_state(g_hw_res->pinctrl, g_hw_res->pin_set_low);
        }else{
            pr_info("%s: cannot to set spi7 cs to pulldown .\n", __func__);
        }
    }
}
EXPORT_SYMBOL(switch_spi7cs_state);



