/***************************************************************
** Copyright (C),  2018,  OPPO Mobile Comm Corp.,  Ltd
** VENDOR_EDIT
** File : oppo_display_private_api.h
** Description : oppo display private api implement
** Version : 1.0
** Date : 2018/03/20
** Author : Jie.Hu@PSW.MM.Display.Stability
**
** ------------------------------- Revision History: -----------
**  <author>        <data>        <version >        <desc>
**   Hu.Jie          2018/03/20        1.0           Build this moudle
**   Guo.Ling        2018/10/11        1.1           Modify for SDM660
**   Guo.Ling        2018/11/27        1.2           Modify for mt6779
**   Lin.Hao         2019/11/01        1.3           Modify for MT6779_Q
******************************************************************/
#include "oplus_display_private_api.h"
#include "disp_drv_log.h"
#include <linux/oppo_mm_kevent_fb.h>
#include <linux/fb.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/of_fdt.h>
#include <linux/of.h>
#include <linux/platform_device.h>
/* Zhijun.ye@PSW.MM.Display.LCD.Stability, 2019/11/22,
 * add for enable dc by default on special version */
#include <soc/oppo/oppo_project.h>

/*
 * we will create a sysfs which called /sys/kernel/oppo_display,
 * In that directory, oppo display private api can be called
 */
#define LCM_ID_READ_LEN 1
#define PANEL_SERIAL_NUM_REG 0xA1
#define PANEL_SERIAL_NUM_REG_CONTINUE 0xA8
#define PANEL_REG_READ_LEN   16

#if 0
unsigned long oppo_display_brightness = 0;
unsigned int oppo_set_brightness = 0;
unsigned int aod_light_mode = 0;
bool oppo_display_hbm_support;
bool oppo_display_ffl_support;
bool oppo_display_panelid_support;
bool oppo_display_sau_support;
bool oppo_display_fppress_support;
#endif
bool oplus_flag_lcd_off = false;
unsigned long oplus_silence_mode = 0;
unsigned int oplus_fp_silence_mode = 0;
bool oplus_display_cabc_support; /* if cabc is supported(cmd sent without cmdq handle) */
bool oplus_display_cabc_cmdq_support; /* if cabc is supported(cmd sent with cmdq handle) */
bool oplus_display_mipi_before_init; /* if frame data must be sent before lcd init(ic:TM TD4330) */
bool oplus_display_lcm_id_check_support; /* whether lcm id is needed to be checked or not(only himax ic of 18311&18011 for now) */
bool oplus_display_aod_support; /* if aod feature is supported */
bool oplus_display_panelnum_support;
bool oplus_display_panelnum_continue_support;
#if 0
bool oppo_display_aod_legacy_support; /* if aod feature is supported(for old project like 17197 which aodlight mode and fppress is not supported on) */
bool oppo_display_aod_ramless_support; /* if ramless aod feature is supported */
bool oppo_display_aodlight_support; /* if aod light mode( high-light or low-light) option is supported */
#endif
#ifdef OPLUS_FEATURE_MULTIBITS_BL
/* Zhijun.Ye@MM.Display.LCD.Machine, 2020/09/23, add for multibits backlight */
bool oplus_display_tenbits_support;
bool oplus_display_elevenbits_support;
bool oplus_display_twelvebits_support;
#endif /* OPLUS_FEATURE_MULTIBITS_BL */
bool oplus_display_backlight_ic_support; /* if backlight ic like lm3697 or ktd3136 is supported */
bool oplus_display_bl_set_on_lcd_esd_recovery;
/* recover backlight after esd recovery (cmd sent without a cmdq handle) */
bool oplus_display_bl_set_cmdq_on_lcd_esd_recovery;
/* recover backlight after esd recovery (cmd sent with a cmdq handle) */
u32 oplus_display_esd_try_count = 0; /* customize esd consecutive recover max time until success (only 18531&18561&18161 for now)*/
#if 0
bool oppo_display_esd_check_dsi_rx_data1; /* if need to read DSI_RX_DATA1 reg on esd checking(only himax ic of 18311&18011 for now)*/
/* tp fw must be reloaded after lcd esd recovery if value is 1(18311&18011&18611) */
u32 oppo_display_tp_fw_reload_on_lcd_esd_recovery = 0;
/* need recover backlight after esd recovery if value is 1(cmd sent withous cmdq handle) */
u32 oppo_display_dyn_data_rate = 0;
u32 oppo_display_dyn_dsi_hbp = 0;
/*Zhijun.Ye@PSW.MM.Display.LCD.Feature, 2020/01/09, add for special lcd resolution */
u32 oppo_display_lcd_height = 0; /* for projects which share the same lcd driver file but have different resolution height */

extern int primary_display_aod_backlight(int level);
extern int primary_display_setbacklight(unsigned int level);
extern void _primary_path_switch_dst_lock(void);
extern void _primary_path_switch_dst_unlock(void);
extern void _primary_path_lock(const char *caller);
extern void _primary_path_unlock(const char *caller);
extern bool primary_display_get_fp_hbm_state(void);
extern int primary_display_set_hbm_mode(unsigned int level);
extern void ffl_set_enable(unsigned int enable);
extern int primary_display_read_lcm_id(char cmd, uint32_t *buf, int num);
extern int panel_serial_number_read(char cmd, uint64_t *buf, int num);
#endif
extern int __attribute((weak)) primary_display_read_serial(char addr, uint64_t *buf, int lenth) { return 1;};
extern int __attribute((weak)) primary_display_set_cabc_mode(unsigned int level) { return 0;};

/* Zhijun.Ye@MM.Display.LCD.Machine, 2020/09/23, add for lcd */
int oplus_mtkfb_custom_data_init(struct platform_device *pdev)
{
	int of_ret = 0;

	if (!pdev) {
		pr_err("%s, pdev is null\n", __func__);
		return -1;
	}

	#ifdef OPLUS_FEATURE_MULTIBITS_BL
	/* Zhijun.Ye@MM.Display.LCD.Machine, 2020/09/23, add for multibits backlight */
	oplus_display_tenbits_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_tenbits_support");
	oplus_display_elevenbits_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_elevenbits_support");
	oplus_display_twelvebits_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_twelvebits_support");
	#endif /* OPLUS_FEATURE_MULTIBITS_BL */
	oplus_display_backlight_ic_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_backlight_ic_support");
	oplus_display_cabc_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_cabc_support");
	oplus_display_cabc_cmdq_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_cabc_cmdq_support");
	oplus_display_bl_set_on_lcd_esd_recovery = of_property_read_bool(pdev->dev.of_node, "oplus_display_bl_set_on_lcd_esd_recovery");
	oplus_display_bl_set_cmdq_on_lcd_esd_recovery = of_property_read_bool(pdev->dev.of_node, "oplus_display_bl_set_cmdq_on_lcd_esd_recovery");
	oplus_display_mipi_before_init = of_property_read_bool(pdev->dev.of_node, "oplus_display_mipi_before_init");
	oplus_display_lcm_id_check_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_lcm_id_check_support");
	oplus_display_aod_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_aod_support");
	oplus_display_panelnum_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_panelnum_support");
	oplus_display_panelnum_continue_support = of_property_read_bool(pdev->dev.of_node, "oplus_display_panelnum_continue_support");

	of_ret = of_property_read_u32(pdev->dev.of_node, "oplus_display_esd_try_count", &oplus_display_esd_try_count);
	if (!of_ret)
		dev_err(&pdev->dev, "read property oplus_display_esd_try_count failed.");
	else
		DISPMSG("%s:oplus_display_esd_try_count=%u\n", __func__, oplus_display_esd_try_count);

	return of_ret;
}

#if 0
static ssize_t oppo_display_get_brightness(struct device *dev,
                                struct device_attribute *attr, char *buf)
{
	if (oppo_display_brightness > LED_FULL || oppo_display_brightness < LED_OFF) {
		oppo_display_brightness = LED_OFF;
	}
	//printk(KERN_INFO "oppo_display_get_brightness = %ld\n",oppo_display_brightness);
	return sprintf(buf, "%ld\n", oppo_display_brightness);
}

static ssize_t oppo_display_set_brightness(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t num)
{
	int ret;

	ret = kstrtouint(buf, 10, &oppo_set_brightness);

	printk("%s %d\n", __func__, oppo_set_brightness);

	if (oppo_set_brightness > LED_FULL || oppo_set_brightness < LED_OFF) {
		return num;
	}

	_primary_path_switch_dst_lock();
	_primary_path_lock(__func__);
	primary_display_setbacklight(oppo_set_brightness);
	_primary_path_unlock(__func__);
	_primary_path_switch_dst_unlock();

	return num;
}

static ssize_t oppo_display_get_max_brightness(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	//printk(KERN_INFO "oppo_display_get_max_brightness = %d\n",LED_FULL);
	return sprintf(buf, "%u\n", LED_FULL);
}

static ssize_t oppo_get_aod_light_mode(struct device *dev,
		struct device_attribute *attr, char *buf) {

	printk(KERN_INFO "oppo_get_aod_light_mode = %d\n",aod_light_mode);

	return sprintf(buf, "%d\n", aod_light_mode);
}

static ssize_t oppo_set_aod_light_mode(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count) {
	unsigned int temp_save = 0;
	int ret = 0;

	if (oppo_display_aodlight_support) {
		ret = kstrtouint(buf, 10, &temp_save);
		if(oppo_display_aod_ramless_support) {
			printk(KERN_INFO "oppo_set_aod_light_mode = %d return on hbm\n",temp_save);
			return count;
		}

		if (primary_display_get_fp_hbm_state()) {
			printk(KERN_INFO "oppo_set_aod_light_mode = %d return on hbm\n",temp_save);
			return count;
		}
		aod_light_mode = temp_save;
		ret = primary_display_aod_backlight(aod_light_mode);
		printk(KERN_INFO "oppo_set_aod_light_mode = %d\n",temp_save);
        }

	return count;
}

int oppo_panel_alpha = 0;
int oppo_underbrightness_alpha = 0;
int alpha_save = 0;
struct ba {
	u32 brightness;
	u32 alpha;
};

struct ba brightness_alpha_lut[] = {
	{0, 0xff},
	{1, 0xee},
	{2, 0xe8},
	{3, 0xe6},
	{4, 0xe5},
	{6, 0xe4},
	{10, 0xe0},
	{20, 0xd5},
	{30, 0xce},
	{45, 0xc6},
	{70, 0xb7},
	{100, 0xad},
	{150, 0xa0},
	{227, 0x8a},
	{300, 0x80},
	{400, 0x6e},
	{500, 0x5b},
	{600, 0x50},
	{800, 0x38},
	{1023, 0x18},
};

static int interpolate(int x, int xa, int xb, int ya, int yb)
{
	int bf, factor, plus;
	int sub = 0;

	bf = 2 * (yb - ya) * (x - xa) / (xb - xa);
	factor = bf / 2;
	plus = bf % 2;
	if ((xa - xb) && (yb - ya))
		sub = 2 * (x - xa) * (x - xb) / (yb - ya) / (xa - xb);

	return ya + factor + plus + sub;
}

int bl_to_alpha(int brightness)
{
	int level = ARRAY_SIZE(brightness_alpha_lut);
	int i = 0;
	int alpha;

	for (i = 0; i < ARRAY_SIZE(brightness_alpha_lut); i++){
		if (brightness_alpha_lut[i].brightness >= brightness)
			break;
	}

	if (i == 0)
		alpha = brightness_alpha_lut[0].alpha;
	else if (i == level)
		alpha = brightness_alpha_lut[level - 1].alpha;
	else
		alpha = interpolate(brightness,
			brightness_alpha_lut[i-1].brightness,
			brightness_alpha_lut[i].brightness,
			brightness_alpha_lut[i-1].alpha,
			brightness_alpha_lut[i].alpha);
	return alpha;
}

int brightness_to_alpha(int brightness)
{
	int alpha;

	if (brightness <= 3)
		return alpha_save;

	alpha = bl_to_alpha(brightness);

	alpha_save = alpha;

	return alpha;
}

int oppo_get_panel_brightness_to_alpha(void)
{
	if (oppo_panel_alpha)
		return oppo_panel_alpha;

	return brightness_to_alpha(oppo_display_brightness);
}

static ssize_t oppo_display_get_dim_alpha(struct device *dev,
                                struct device_attribute *attr, char *buf)
{
	if (!primary_display_get_fp_hbm_state())
		return sprintf(buf, "%d\n", 0);

	oppo_underbrightness_alpha = oppo_get_panel_brightness_to_alpha();

	return sprintf(buf, "%d\n", oppo_underbrightness_alpha);
}

static ssize_t oppo_display_set_dim_alpha(struct device *dev,
                               struct device_attribute *attr,
                               const char *buf, size_t count)
{
	sscanf(buf, "%x", &oppo_panel_alpha);
	return count;
}

int oppo_dc_alpha = 0;
int oppo_dc_enable = 0;

static ssize_t oppo_display_get_dc_enable(struct device *dev,
                                struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", oppo_dc_enable);
}

static ssize_t oppo_display_set_dc_enable(struct device *dev,
                               struct device_attribute *attr,
                               const char *buf, size_t count)
{
	sscanf(buf, "%x", &oppo_dc_enable);
	return count;
}

static ssize_t oppo_display_get_dim_dc_alpha(struct device *dev,
                                struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", oppo_dc_alpha);
}

static ssize_t oppo_display_set_dim_dc_alpha(struct device *dev,
                               struct device_attribute *attr,
                               const char *buf, size_t count)
{
	sscanf(buf, "%x", &oppo_dc_alpha);
	return count;
}

unsigned long HBM_mode = 0;
unsigned long HBM_pre_mode = 0;
struct timespec hbm_time_on;
struct timespec hbm_time_off;
long hbm_on_start = 0;
extern bool primary_is_aod_supported(void);

static ssize_t LCM_HBM_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	printk("%s HBM_mode=%ld\n", __func__, HBM_mode);
	return sprintf(buf, "%ld\n", HBM_mode);
}

static ssize_t LCM_HBM_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t num)
{
	int ret;
	unsigned char payload[100] = "";
	printk("oppo_display_hbm_support = %d, primary_is_aod_supported() = %d\n", oppo_display_hbm_support, primary_is_aod_supported());
	if (oppo_display_hbm_support || primary_is_aod_supported()) {
		HBM_pre_mode = HBM_mode;
		ret = kstrtoul(buf, 10, &HBM_mode);
		printk("%s HBM_mode=%ld\n", __func__, HBM_mode);
		ret = primary_display_set_hbm_mode((unsigned int)HBM_mode);
		if (HBM_mode == 8) {
			get_monotonic_boottime(&hbm_time_on);
			hbm_on_start = hbm_time_on.tv_sec;
		} else if (HBM_pre_mode == 8 && HBM_mode != 8) {
			get_monotonic_boottime(&hbm_time_off);
			scnprintf(payload, sizeof(payload), "EventID@@%d$$hbm@@hbm state on time = %ld sec$$ReportLevel@@%d",
				OPPO_MM_DIRVER_FB_EVENT_ID_HBM,(hbm_time_off.tv_sec - hbm_on_start),OPPO_MM_DIRVER_FB_EVENT_REPORTLEVEL_LOW);
			upload_mm_kevent_fb_data(OPPO_MM_DIRVER_FB_EVENT_MODULE_DISPLAY,payload);
		}
	}
	return num;
}

unsigned int ffl_set_mode = 0;
unsigned int ffl_backlight_on = 0;
extern bool ffl_trigger_finish;

static ssize_t FFL_SET_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	printk("%s ffl_set_mode=%d\n", __func__, ffl_set_mode);
	return sprintf(buf, "%d\n", ffl_set_mode);
}

static ssize_t FFL_SET_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t num)
{
	int ret;
	printk("oppo_display_ffl_support = %d\n", oppo_display_ffl_support);
	if (oppo_display_ffl_support) {
		ret = kstrtouint(buf, 10, &ffl_set_mode);
		printk("%s ffl_set_mode=%d\n", __func__, ffl_set_mode);
		if (ffl_trigger_finish && (ffl_backlight_on == 1) && (ffl_set_mode == 1)) {
			ffl_set_enable(1);
		}
	}
	return num;
}

unsigned char lcm_id_addr = 0;
static uint32_t lcm_id_info = 0x0;

static ssize_t lcm_id_info_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int ret = 0;
	if ((oppo_display_panelid_support) && (lcm_id_addr != 0)) {
		ret = primary_display_read_lcm_id(lcm_id_addr, &lcm_id_info, LCM_ID_READ_LEN);
		ret = scnprintf(buf, PAGE_SIZE, "LCM ID[%x]: 0x%x 0x%x\n", lcm_id_addr, lcm_id_info, 0);
	} else {
		ret = scnprintf(buf, PAGE_SIZE, "LCM ID[00]: 0x00 0x00\n");
	}
	lcm_id_addr = 0;
	return ret;
}

static ssize_t lcm_id_info_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t num)
{
	int ret;
	ret = kstrtou8(buf, 0, &lcm_id_addr);
	printk("%s lcm_id_addr = 0x%x\n", __func__, lcm_id_addr);
	return num;
}
#endif
static uint64_t serial_number = 0x0;
#if 0
int lcm_first_get_serial(void)
{
	int ret = 0;
	if (oppo_display_panelnum_support) {
		pr_err("lcm_first_get_serial\n");
		ret = panel_serial_number_read(PANEL_SERIAL_NUM_REG, &serial_number,
				PANEL_REG_READ_LEN);
	}

	return ret;
}

int lcm_first_get_serial_change(void)
{
	int ret = 0;

	pr_err("lcm_first_get_serial\n");
	ret = panel_serial_number_read(PANEL_SERIAL_NUM_REG, &serial_number,
			PANEL_REG_READ_LEN);

	return ret;
}
#endif
static ssize_t mdss_get_panel_serial_number(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int ret = 0;

	if (oplus_display_panelnum_support) {
		if (serial_number == 0) {
			ret = primary_display_read_serial(PANEL_SERIAL_NUM_REG, &serial_number,
				PANEL_REG_READ_LEN);
		}
		if (ret <= 0 && serial_number == 0)
			ret = scnprintf(buf, PAGE_SIZE, "Get serial number failed: %d\n",ret);
		else
			ret = scnprintf(buf, PAGE_SIZE, "Get panel serial number: %llx\n",serial_number);
	} else if (oplus_display_panelnum_continue_support) {
		if (serial_number == 0) {
			ret = primary_display_read_serial(PANEL_SERIAL_NUM_REG_CONTINUE, &serial_number,
				PANEL_REG_READ_LEN);
		}
		if (ret <= 0 && serial_number == 0)
			ret = scnprintf(buf, PAGE_SIZE, "Get serial number failed: %d\n",ret);
		else
			ret = scnprintf(buf, PAGE_SIZE, "Get panel serial number: %llx\n",serial_number);
	} else {
		ret = scnprintf(buf, PAGE_SIZE, "Unsupported panel!!\n");
	}
	return ret;
}

static ssize_t panel_serial_store(struct device *dev,
		struct device_attribute *attr,
		const char *buf, size_t count)
{

    DISPMSG("[soso] Lcm read 0xA1 reg = 0x%llx\n", serial_number);

	return count;
}

static ssize_t silence_show(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	printk("%s oplus_silence_mode=%ld\n", __func__, oplus_silence_mode);
	return sprintf(buf, "%ld\n", oplus_silence_mode);
}

static ssize_t silence_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t num)
{
	int ret;
	ret = kstrtoul(buf, 10, &oplus_silence_mode);
	printk("%s oplus_silence_mode=%ld\n", __func__, oplus_silence_mode);
	return num;
}

#if 0
extern bool flag_lcd_off;
bool oppo_fp_notify_down_delay = false;
bool oppo_fp_notify_up_delay = false;
bool ds_rec_fpd;
bool doze_rec_fpd;

void fingerprint_send_notify(struct fb_info *fbi, uint8_t fingerprint_op_mode)
{
	struct fb_event event;

	event.info  = fbi;
	event.data = &fingerprint_op_mode;
	fb_notifier_call_chain(MTK_ONSCREENFINGERPRINT_EVENT, &event);
	pr_info("%s send uiready : %d\n", __func__, fingerprint_op_mode);
}

static ssize_t fingerprint_notify_trigger(struct device *dev,
                               struct device_attribute *attr,
                               const char *buf, size_t num)
{
	uint8_t fingerprint_op_mode = 0x0;

	if (oppo_display_fppress_support) {
		/* will ignoring event during panel off situation. */
		if (flag_lcd_off)
		{
			pr_err("%s panel in off state, ignoring event.\n", __func__);
			return num;
		}
		if (kstrtou8(buf, 0, &fingerprint_op_mode))
		{
			pr_err("%s kstrtouu8 buf error!\n", __func__);
			return num;
		}
		if (fingerprint_op_mode == 1) {
			oppo_fp_notify_down_delay = true;
		} else {
			oppo_fp_notify_up_delay = true;
			ds_rec_fpd = false;
			doze_rec_fpd = false;
		}
		pr_info("%s receive uiready %d\n", __func__,fingerprint_op_mode);
	}
	return num;
}
#endif

unsigned long CABC_mode = 2;

unsigned long cabc_old_mode = 1;
unsigned long cabc_true_mode = 1;
unsigned long cabc_sun_flag = 0;
unsigned long cabc_back_flag = 1;

enum{
    CABC_LEVEL_0,
    CABC_LEVEL_1,
    CABC_LEVEL_3 = 3,
    CABC_EXIT_SPECIAL = 8,
    CABC_ENTER_SPECIAL = 9,
};

#if defined(CONFIG_MACH_MT6768) || defined(CONFIG_MACH_MT6785)
//Jiantao.Liu@ODM_WT.MM.Display.Lcd, 2020/07/30, Modify  DRE&CABC for backlight hight light mode
bool backlight_high_light_dre_cabc = true;
#else
bool backlight_high_light_dre_cabc = false;
#endif
/*
* add dre only use for camera
*/
extern void disp_aal_set_dre_en(int enable);

static ssize_t LCM_CABC_show(struct device *dev,
                struct device_attribute *attr, char *buf)
{
    if (backlight_high_light_dre_cabc) {
        pr_err("%s CABC_mode=%ld\n", __func__, cabc_true_mode);
        return sprintf(buf, "%ld\n", cabc_true_mode);
    } else {
        printk("%s CABC_mode=%ld\n", __func__, CABC_mode);
        return sprintf(buf, "%ld\n", CABC_mode);
    }
}

static ssize_t LCM_CABC_store(struct device *dev,
        struct device_attribute *attr, const char *buf, size_t num)
{
    int ret = 0;
    if (backlight_high_light_dre_cabc) {
        ret = kstrtoul(buf, 10, &cabc_old_mode);
        cabc_true_mode = cabc_old_mode;
        if(cabc_old_mode < 4) {
            cabc_back_flag = cabc_old_mode;
        }
        pr_err("%s CABC_mode=%ld cabc_back_flag = %d\n", __func__, cabc_old_mode, cabc_back_flag);
        if(CABC_ENTER_SPECIAL == cabc_old_mode) {
            cabc_sun_flag = 1;
            cabc_true_mode = 0;
        } else if (CABC_EXIT_SPECIAL == cabc_old_mode) {
            cabc_sun_flag = 0;
            cabc_true_mode = cabc_back_flag;
        } else if (1 == cabc_sun_flag) {
            if(CABC_LEVEL_0 == cabc_back_flag) {
                disp_aal_set_dre_en(1);
                pr_err("%s enable dre1\n", __func__);
            } else {
                disp_aal_set_dre_en(1);
                pr_err("%s disable dre1\n", __func__);
            }
            return num;
        }

        if(cabc_true_mode == CABC_LEVEL_0 && cabc_back_flag == CABC_LEVEL_0) {
            disp_aal_set_dre_en(1);
            pr_err("%s enable dre2\n", __func__);
        } else {
            disp_aal_set_dre_en(1);
            pr_err("%s enable dre 2\n", __func__);
        }

        pr_err("%s  cabc_true_mode = %d\n", __func__,  cabc_true_mode);
        ret = primary_display_set_cabc_mode((unsigned int)cabc_true_mode);

        if(cabc_true_mode != cabc_back_flag) {
            cabc_true_mode = cabc_back_flag;
        }
    } else {
        ret = kstrtoul(buf, 10, &CABC_mode);
        if( CABC_mode > 3 ){
            CABC_mode = 3;
        }
        printk("%s CABC_mode=%ld\n", __func__, CABC_mode);

        if (CABC_mode == 0) {
            disp_aal_set_dre_en(1);
            printk("%s enable dre\n", __func__);
        } else {
            disp_aal_set_dre_en(0);
            printk("%s disable dre\n", __func__);
        }

        /*
        * modify for oled not need set cabc
        */
        if (oplus_display_cabc_support || oplus_display_cabc_cmdq_support) {
            ret = primary_display_set_cabc_mode((unsigned int)CABC_mode);
        }
    }

    return num;
}

static struct kobject *oplus_display_kobj;

/*static DEVICE_ATTR(oppo_brightness, S_IRUGO|S_IWUSR, oppo_display_get_brightness, oppo_display_set_brightness);
static DEVICE_ATTR(oppo_max_brightness, S_IRUGO|S_IWUSR, oppo_display_get_max_brightness, NULL);

static DEVICE_ATTR(aod_light_mode_set, S_IRUGO|S_IWUSR, oppo_get_aod_light_mode, oppo_set_aod_light_mode);
static DEVICE_ATTR(dim_alpha, S_IRUGO|S_IWUSR, oppo_display_get_dim_alpha, oppo_display_set_dim_alpha);
static DEVICE_ATTR(dimlayer_bl_en, S_IRUGO|S_IWUSR, oppo_display_get_dc_enable, oppo_display_set_dc_enable);
static DEVICE_ATTR(dim_dc_alpha, S_IRUGO|S_IWUSR, oppo_display_get_dim_dc_alpha, oppo_display_set_dim_dc_alpha);
static DEVICE_ATTR(hbm, S_IRUGO|S_IWUSR, LCM_HBM_show, LCM_HBM_store);
static DEVICE_ATTR(ffl_set, S_IRUGO|S_IWUSR, FFL_SET_show, FFL_SET_store);
static DEVICE_ATTR(panel_id, S_IRUGO|S_IWUSR, lcm_id_info_show, lcm_id_info_store);
static DEVICE_ATTR(oppo_notify_fppress, S_IRUGO|S_IWUSR, NULL, fingerprint_notify_trigger);*/
static DEVICE_ATTR(panel_serial_number, S_IRUGO|S_IWUSR, mdss_get_panel_serial_number, panel_serial_store);
static DEVICE_ATTR(sau_closebl_node, S_IRUGO|S_IWUSR, silence_show, silence_store);
static DEVICE_ATTR(LCM_CABC, S_IRUGO|S_IWUSR, LCM_CABC_show, LCM_CABC_store);

/*
 * Create a group of attributes so that we can create and destroy them all
 * at once.
 */
static struct attribute *oplus_display_attrs[] = {
	/*&dev_attr_oppo_brightness.attr,
	&dev_attr_oppo_max_brightness.attr,
	&dev_attr_aod_light_mode_set.attr,
	&dev_attr_dim_alpha.attr,
	&dev_attr_dimlayer_bl_en.attr,
	&dev_attr_dim_dc_alpha.attr,
	&dev_attr_hbm.attr,
	&dev_attr_ffl_set.attr,
	&dev_attr_panel_id.attr,
	&dev_attr_oppo_notify_fppress.attr,*/
	&dev_attr_panel_serial_number.attr,
	&dev_attr_sau_closebl_node.attr,
	&dev_attr_LCM_CABC.attr,
	NULL,	/* need to NULL terminate the list of attributes */
};

static struct attribute_group oplus_display_attr_group = {
	.attrs = oplus_display_attrs,
};

static int __init oplus_display_private_api_init(void)
{
	int retval;

	/* Zhijun.ye@PSW.MM.Display.LCD.Stability, 2019/11/22,
	 * add for enable dc by default on special version */
	/*if (get_eng_version() == 1)
		oppo_dc_enable = 1;*/

	oplus_display_kobj = kobject_create_and_add("oppo_display", kernel_kobj);
	if (!oplus_display_kobj)
		return -ENOMEM;

	/* Create the files associated with this kobject */
	retval = sysfs_create_group(oplus_display_kobj, &oplus_display_attr_group);
	if (retval)
		kobject_put(oplus_display_kobj);

	return retval;
}

static void __exit oplus_display_private_api_exit(void)
{
	kobject_put(oplus_display_kobj);
}

module_init(oplus_display_private_api_init);
module_exit(oplus_display_private_api_exit);
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Hujie <>");
