/*
 * Copyright (c) 2015 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/backlight.h>
#include <drm/drmP.h>
#include <drm/drm_mipi_dsi.h>
#include <drm/drm_panel.h>

#include <linux/gpio/consumer.h>
#include <linux/regulator/consumer.h>

#include <video/mipi_display.h>
#include <video/of_videomode.h>
#include <video/videomode.h>
#include <linux/of_graph.h>
#include <linux/fb.h>
#include <linux/module.h>
#include <linux/of_platform.h>
#include <linux/platform_device.h>
#include <soc/oppo/device_info.h>

#define CONFIG_MTK_PANEL_EXT
#if defined(CONFIG_MTK_PANEL_EXT)
#include "../mediatek/mtk_panel_ext.h"
#include "../mediatek/mtk_log.h"
#include "../mediatek/mtk_drm_graphics_base.h"
#endif

#ifdef VENDOR_EDIT
/*shifan@bsp.tp 2020.0304, pulldown spi7 cs when doing ftm in case of current leakage*/
#include <mt-plat/mtk_boot_common.h>
#endif /*VENDOR_EDIT*/
#ifdef CONFIG_MTK_ROUND_CORNER_SUPPORT
#include "../mediatek/mtk_corner_pattern/mtk_data_hw_roundedpattern.h"
#endif

/* enable this to check panel self -bist pattern */
/* #define PANEL_BIST_PATTERN */
/****************TPS65132***********/
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
//#include "lcm_i2c.h"
#include <mt-plat/mtk_boot_common.h>

#define AVDD_REG 0x00
#define AVDD_REG 0x01

/* i2c control start */
#define LCM_I2C_ID_NAME "I2C_LCD_BIAS"
static struct i2c_client *_lcm_i2c_client;
static char bl_tb0[] = { 0x51, 0xff };

/*****************************************************************************
 * Function Prototype
 *****************************************************************************/
static int _lcm_i2c_probe(struct i2c_client *client,
			  const struct i2c_device_id *id);
static int _lcm_i2c_remove(struct i2c_client *client);

#ifdef VENDOR_EDIT
/* shifan@PSW.BSP.TP.Function, 2019/04/26, Add for TP common code */
extern int tp_gesture_enable_flag(void);
extern void lcd_queue_load_tp_fw(void);
void __attribute__((weak)) switch_spi7cs_state(bool normal) {return;}
#endif /* VENDOR_EDIT */
static bool cabc_dimming_on = false;
static int frame_count = 0;
extern unsigned long esd_flag;
static int esd_brightness;
extern unsigned long oplus_max_normal_brightness;
extern void disp_aal_set_dre_en(int enable);
#ifdef VENDOR_EDIT
/* shiyaqiang@RM.MM.LCD.Driver 20180621 add for keep cabc mode after resume*/
static int cabc_lastlevel = 0;
static int last_brightness = 0;
static void cabc_switch(void *dsi, dcs_write_gce cb,
                void *handle, unsigned int cabc_mode);
#endif
/*****************************************************************************
 * Data Structure
 *****************************************************************************/
struct _lcm_i2c_dev {
	struct i2c_client *client;
};

static const struct of_device_id _lcm_i2c_of_match[] = {
	{
	    .compatible = "mediatek,I2C_LCD_BIAS",
	},
	{},
};

static const struct i2c_device_id _lcm_i2c_id[] = { { LCM_I2C_ID_NAME, 0 },
						    {} };

static struct i2c_driver _lcm_i2c_driver = {
	.id_table = _lcm_i2c_id,
	.probe = _lcm_i2c_probe,
	.remove = _lcm_i2c_remove,
	/* .detect		   = _lcm_i2c_detect, */
	.driver = {
		.owner = THIS_MODULE,
		.name = LCM_I2C_ID_NAME,
		.of_match_table = _lcm_i2c_of_match,
	},
};

/*****************************************************************************
 * Function
 *****************************************************************************/
static int _lcm_i2c_probe(struct i2c_client *client,
			  const struct i2c_device_id *id)
{
	pr_debug("[LCM][I2C] %s\n", __func__);
	pr_debug("[LCM][I2C] NT: info==>name=%s addr=0x%x\n", client->name,
		 client->addr);
	_lcm_i2c_client = client;
	return 0;
}

static int _lcm_i2c_remove(struct i2c_client *client)
{
	pr_debug("[LCM][I2C] %s\n", __func__);
	_lcm_i2c_client = NULL;
	i2c_unregister_device(client);
	return 0;
}

static int _lcm_i2c_write_bytes(unsigned char addr, unsigned char value)
{
	int ret = 0;
	struct i2c_client *client = _lcm_i2c_client;
	char write_data[2] = { 0 };

	if (client == NULL) {
		pr_debug("ERROR!! _lcm_i2c_client is null\n");
		return 0;
	}

	write_data[0] = addr;
	write_data[1] = value;
	ret = i2c_master_send(client, write_data, 2);
	if (ret < 0)
		pr_info("[LCM][ERROR] _lcm_i2c write data fail !!\n");

	return ret;
}

/*
 * module load/unload record keeping
 */
static int __init _lcm_i2c_init(void)
{
	pr_debug("[LCM][I2C] %s\n", __func__);
	i2c_add_driver(&_lcm_i2c_driver);
	pr_debug("[LCM][I2C] %s success\n", __func__);
	return 0;
}

static void __exit _lcm_i2c_exit(void)
{
	pr_debug("[LCM][I2C] %s\n", __func__);
	i2c_del_driver(&_lcm_i2c_driver);
}

module_init(_lcm_i2c_init);
module_exit(_lcm_i2c_exit);
/***********************************/

struct lcm {
	struct device *dev;
	struct drm_panel panel;
	struct backlight_device *backlight;
	struct gpio_desc *reset_gpio;
//	struct gpio_desc *lcd_1p8_gpio;
	struct gpio_desc *bias_pos, *bias_neg;

//	struct gpio_desc *bias_en;
	bool prepared;
	bool enabled;

	int error;

    bool is_normal_mode;
};

#define lcm_dcs_write_seq(ctx, seq...) \
({\
	const u8 d[] = { seq };\
	BUILD_BUG_ON_MSG(ARRAY_SIZE(d) > 64, "DCS sequence too big for stack");\
	lcm_dcs_write(ctx, d, ARRAY_SIZE(d));\
})

#define lcm_dcs_write_seq_static(ctx, seq...) \
({\
	static const u8 d[] = { seq };\
	lcm_dcs_write(ctx, d, ARRAY_SIZE(d));\
})

static inline struct lcm *panel_to_lcm(struct drm_panel *panel)
{
	return container_of(panel, struct lcm, panel);
}

static void lcm_dcs_write(struct lcm *ctx, const void *data, size_t len)
{
	struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
	ssize_t ret;
	char *addr;

	if (ctx->error < 0)
		return;

	addr = (char *)data;
	if ((int)*addr < 0xB0)
		ret = mipi_dsi_dcs_write_buffer(dsi, data, len);
	else
		ret = mipi_dsi_generic_write(dsi, data, len);
	if (ret < 0) {
		dev_err(ctx->dev, "error %zd writing seq: %ph\n", ret, data);
		ctx->error = ret;
	}
}

#ifdef PANEL_SUPPORT_READBACK
static int lcm_dcs_read(struct lcm *ctx, u8 cmd, void *data, size_t len)
{
	struct mipi_dsi_device *dsi = to_mipi_dsi_device(ctx->dev);
	ssize_t ret;

	if (ctx->error < 0)
		return 0;

	ret = mipi_dsi_dcs_read(dsi, cmd, data, len);
	if (ret < 0) {
		dev_err(ctx->dev, "error %d reading dcs seq:(%#x)\n", ret, cmd);
		ctx->error = ret;
	}

	return ret;
}

static void lcm_panel_get_data(struct lcm *ctx)
{
	u8 buffer[3] = {0};
	static int ret;

	if (ret == 0) {
		ret = lcm_dcs_read(ctx,  0x0A, buffer, 1);
		dev_info(ctx->dev, "return %d data(0x%08x) to dsi engine\n",
			 ret, buffer[0] | (buffer[1] << 8));
	}
}
#endif

#if defined(CONFIG_RT5081_PMU_DSV) || defined(CONFIG_MT6370_PMU_DSV)
static struct regulator *disp_bias_pos;
static struct regulator *disp_bias_neg;


static int lcm_panel_bias_regulator_init(void)
{
	static int regulator_inited;
	int ret = 0;

	if (regulator_inited)
		return ret;

	/* please only get regulator once in a driver */
	disp_bias_pos = regulator_get(NULL, "dsv_pos");
	if (IS_ERR(disp_bias_pos)) { /* handle return value */
		ret = PTR_ERR(disp_bias_pos);
		pr_err("get dsv_pos fail, error: %d\n", ret);
		return ret;
	}

	disp_bias_neg = regulator_get(NULL, "dsv_neg");
	if (IS_ERR(disp_bias_neg)) { /* handle return value */
		ret = PTR_ERR(disp_bias_neg);
		pr_err("get dsv_neg fail, error: %d\n", ret);
		return ret;
	}

	regulator_inited = 1;
	return ret; /* must be 0 */

}

static int lcm_panel_bias_enable(void)
{
	int ret = 0;
	int retval = 0;

	lcm_panel_bias_regulator_init();

	/* set voltage with min & max*/
	ret = regulator_set_voltage(disp_bias_pos, 5400000, 5400000);
	if (ret < 0)
		pr_err("set voltage disp_bias_pos fail, ret = %d\n", ret);
	retval |= ret;

	ret = regulator_set_voltage(disp_bias_neg, 5400000, 5400000);
	if (ret < 0)
		pr_err("set voltage disp_bias_neg fail, ret = %d\n", ret);
	retval |= ret;

	/* enable regulator */
	ret = regulator_enable(disp_bias_pos);
	if (ret < 0)
		pr_err("enable regulator disp_bias_pos fail, ret = %d\n", ret);
	retval |= ret;

	ret = regulator_enable(disp_bias_neg);
	if (ret < 0)
		pr_err("enable regulator disp_bias_neg fail, ret = %d\n", ret);
	retval |= ret;

	return retval;
}

static int lcm_panel_bias_disable(void)
{
	int ret = 0;
	int retval = 0;

	lcm_panel_bias_regulator_init();

	ret = regulator_disable(disp_bias_neg);
	if (ret < 0)
		pr_err("disable regulator disp_bias_neg fail, ret = %d\n", ret);
	retval |= ret;

	ret = regulator_disable(disp_bias_pos);
	if (ret < 0)
		pr_err("disable regulator disp_bias_pos fail, ret = %d\n", ret);
	retval |= ret;

	return retval;
}
#endif

static void lcm_panel_init(struct lcm *ctx)
{
	pr_info("SYQ %s+\n", __func__);
	lcm_dcs_write_seq_static(ctx, 0xFF, 0x10);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0xB0, 0x00);
	lcm_dcs_write_seq_static(ctx, 0xC2, 0x1B, 0xA0);
////updata xh 20200611 TP 180HZ //////////////////
	lcm_dcs_write_seq_static(ctx, 0XFF, 0X20);
	lcm_dcs_write_seq_static(ctx, 0XFB, 0X01);
	lcm_dcs_write_seq_static(ctx, 0X2F, 0X83);
	lcm_dcs_write_seq_static(ctx, 0XFF, 0X25);
	lcm_dcs_write_seq_static(ctx, 0XFB, 0X01);
	lcm_dcs_write_seq_static(ctx, 0X18, 0X20);
	lcm_dcs_write_seq_static(ctx, 0XFF, 0X26);
	lcm_dcs_write_seq_static(ctx, 0XFB, 0X01);
	lcm_dcs_write_seq_static(ctx, 0X15, 0X04);
	lcm_dcs_write_seq_static(ctx, 0X83, 0X02);
	lcm_dcs_write_seq_static(ctx, 0X80, 0X05);
	lcm_dcs_write_seq_static(ctx, 0X88, 0X00);
	lcm_dcs_write_seq_static(ctx, 0XFF, 0X27);
	lcm_dcs_write_seq_static(ctx, 0XFB, 0X01);
	lcm_dcs_write_seq_static(ctx, 0X21, 0XE7);
	lcm_dcs_write_seq_static(ctx, 0XE4, 0XDA);
	lcm_dcs_write_seq_static(ctx, 0XE6, 0X6D);
	lcm_dcs_write_seq_static(ctx, 0XFF, 0X2A);
	lcm_dcs_write_seq_static(ctx, 0XFB, 0X01);
	lcm_dcs_write_seq_static(ctx, 0X0A, 0X70);
	lcm_dcs_write_seq_static(ctx, 0X0C, 0X09);
	lcm_dcs_write_seq_static(ctx, 0X0F, 0X00);
	lcm_dcs_write_seq_static(ctx, 0X11, 0XF0);
	lcm_dcs_write_seq_static(ctx, 0X15, 0X0E);
	lcm_dcs_write_seq_static(ctx, 0X16, 0XB6);
	lcm_dcs_write_seq_static(ctx, 0X19, 0X0E);
	lcm_dcs_write_seq_static(ctx, 0X1A, 0X8A);
	lcm_dcs_write_seq_static(ctx, 0X1E, 0X4F);
	lcm_dcs_write_seq_static(ctx, 0X1F, 0X51);
	lcm_dcs_write_seq_static(ctx, 0X20, 0X4F);
	lcm_dcs_write_seq_static(ctx, 0X28, 0XEC);
	lcm_dcs_write_seq_static(ctx, 0X29, 0X0C);
	lcm_dcs_write_seq_static(ctx, 0X2A, 0X05);
	lcm_dcs_write_seq_static(ctx, 0X2B, 0X00);
	lcm_dcs_write_seq_static(ctx, 0X2D, 0X06);
	lcm_dcs_write_seq_static(ctx, 0X2F, 0X02);
	lcm_dcs_write_seq_static(ctx, 0X30, 0X4A);
	lcm_dcs_write_seq_static(ctx, 0X31, 0X40);
	lcm_dcs_write_seq_static(ctx, 0X33, 0X0E);
	lcm_dcs_write_seq_static(ctx, 0X34, 0XEE);
	lcm_dcs_write_seq_static(ctx, 0X35, 0X30);
	lcm_dcs_write_seq_static(ctx, 0X36, 0X06);
	lcm_dcs_write_seq_static(ctx, 0X37, 0XE9);
	lcm_dcs_write_seq_static(ctx, 0X38, 0X34);
	lcm_dcs_write_seq_static(ctx, 0X39, 0X02);
	lcm_dcs_write_seq_static(ctx, 0X3A, 0X4A);
	lcm_dcs_write_seq_static(ctx, 0X4E, 0X0E);
	lcm_dcs_write_seq_static(ctx, 0X4F, 0X9B);
	lcm_dcs_write_seq_static(ctx, 0X52, 0X0E);
	lcm_dcs_write_seq_static(ctx, 0X53, 0X6F);
	lcm_dcs_write_seq_static(ctx, 0X61, 0XC7);
	lcm_dcs_write_seq_static(ctx, 0X63, 0XF3);
	lcm_dcs_write_seq_static(ctx, 0X68, 0X8A);
	lcm_dcs_write_seq_static(ctx, 0X6A, 0X0F);
	lcm_dcs_write_seq_static(ctx, 0X6B, 0XC9);
	lcm_dcs_write_seq_static(ctx, 0X6C, 0X20);
	lcm_dcs_write_seq_static(ctx, 0X6D, 0XE3);
	lcm_dcs_write_seq_static(ctx, 0X6E, 0XC6);
	lcm_dcs_write_seq_static(ctx, 0X6F, 0X22);
	lcm_dcs_write_seq_static(ctx, 0X70, 0XE1);
	lcm_dcs_write_seq_static(ctx, 0X7A, 0X07);
	lcm_dcs_write_seq_static(ctx, 0X7C, 0X02);
	lcm_dcs_write_seq_static(ctx, 0X7F, 0X2C);
	lcm_dcs_write_seq_static(ctx, 0X83, 0X0F);
	lcm_dcs_write_seq_static(ctx, 0X84, 0X12);
	lcm_dcs_write_seq_static(ctx, 0X87, 0X0E);
	lcm_dcs_write_seq_static(ctx, 0X88, 0XE6);
	lcm_dcs_write_seq_static(ctx, 0X8C, 0X3A);
	lcm_dcs_write_seq_static(ctx, 0X8D, 0X3A);
	lcm_dcs_write_seq_static(ctx, 0X8E, 0X3A);
	lcm_dcs_write_seq_static(ctx, 0X97, 0X14);
	lcm_dcs_write_seq_static(ctx, 0X98, 0X32);
	lcm_dcs_write_seq_static(ctx, 0X99, 0X01);
	lcm_dcs_write_seq_static(ctx, 0X9A, 0X08);
	lcm_dcs_write_seq_static(ctx, 0X9C, 0X4C);
	lcm_dcs_write_seq_static(ctx, 0X9D, 0XB1);
	lcm_dcs_write_seq_static(ctx, 0X9F, 0X75);
	lcm_dcs_write_seq_static(ctx, 0XA2, 0X42);
	lcm_dcs_write_seq_static(ctx, 0XA3, 0X6F);
	lcm_dcs_write_seq_static(ctx, 0XA5, 0X47);
	lcm_dcs_write_seq_static(ctx, 0XA6, 0X6A);
	lcm_dcs_write_seq_static(ctx, 0XA7, 0X4C);

	lcm_dcs_write_seq_static(ctx, 0XFF,0X2C);
	lcm_dcs_write_seq_static(ctx, 0XFB,0X01);
	lcm_dcs_write_seq_static(ctx, 0X56,0X0E);
	lcm_dcs_write_seq_static(ctx, 0X58,0X0E);
	lcm_dcs_write_seq_static(ctx, 0X59,0X0E);
	lcm_dcs_write_seq_static(ctx, 0X6A,0X14);
	lcm_dcs_write_seq_static(ctx, 0X6B,0X34);
	lcm_dcs_write_seq_static(ctx, 0X6C,0X34);
	lcm_dcs_write_seq_static(ctx, 0X6D,0X34);
	lcm_dcs_write_seq_static(ctx, 0X9D,0X0F);
	lcm_dcs_write_seq_static(ctx, 0X9F,0X00);	
	lcm_dcs_write_seq_static(ctx, 0XFF,0X2B);
	lcm_dcs_write_seq_static(ctx, 0XFB,0X01);
	lcm_dcs_write_seq_static(ctx, 0XB7,0X06);
	lcm_dcs_write_seq_static(ctx, 0XB8,0X03);
	lcm_dcs_write_seq_static(ctx, 0XC0,0X03);	
///*******CABC**********////
	lcm_dcs_write_seq_static(ctx, 0xFF, 0xF0);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0xD2, 0x52);
	lcm_dcs_write_seq_static(ctx, 0xFF, 0x23);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x00, 0x80);
	/*lcm_dcs_write_seq_static(ctx, 0x04, 0x05);
	lcm_dcs_write_seq_static(ctx, 0x05, 0x2d);
	lcm_dcs_write_seq_static(ctx, 0x06, 0x01);*/
	lcm_dcs_write_seq_static(ctx, 0x07, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x08, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x09, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x11, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x12, 0x95);
	lcm_dcs_write_seq_static(ctx, 0x15, 0x68);
	lcm_dcs_write_seq_static(ctx, 0x16, 0x0B);
	lcm_dcs_write_seq_static(ctx, 0x0A, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x0B, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x0C, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x0D, 0x00);
	lcm_dcs_write_seq_static(ctx, 0xA0, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x30, 0xFA);
	lcm_dcs_write_seq_static(ctx, 0x31, 0xF9);
	lcm_dcs_write_seq_static(ctx, 0x32, 0xF7);
	lcm_dcs_write_seq_static(ctx, 0x33, 0xF5);
	lcm_dcs_write_seq_static(ctx, 0x34, 0xF3);
	lcm_dcs_write_seq_static(ctx, 0x35, 0xEC);
	lcm_dcs_write_seq_static(ctx, 0x36, 0xEA);
	lcm_dcs_write_seq_static(ctx, 0x37, 0xE8);
	lcm_dcs_write_seq_static(ctx, 0x38, 0xDD);
	lcm_dcs_write_seq_static(ctx, 0x39, 0xDA);
	lcm_dcs_write_seq_static(ctx, 0x3A, 0xD8);
	lcm_dcs_write_seq_static(ctx, 0x3B, 0xD6);
	lcm_dcs_write_seq_static(ctx, 0x3D, 0xD4);
	lcm_dcs_write_seq_static(ctx, 0x3F, 0xD2);
	lcm_dcs_write_seq_static(ctx, 0x40, 0xCF);
	lcm_dcs_write_seq_static(ctx, 0x41, 0xCC);

	lcm_dcs_write_seq_static(ctx, 0x45, 0xE2);
	lcm_dcs_write_seq_static(ctx, 0x46, 0xDC);
	lcm_dcs_write_seq_static(ctx, 0x47, 0xCD);
	lcm_dcs_write_seq_static(ctx, 0x48, 0xC5);
	lcm_dcs_write_seq_static(ctx, 0x49, 0xBB);
	lcm_dcs_write_seq_static(ctx, 0x4A, 0xB3);
	lcm_dcs_write_seq_static(ctx, 0x4B, 0xB1);
	lcm_dcs_write_seq_static(ctx, 0x4C, 0xB0);
	lcm_dcs_write_seq_static(ctx, 0x4D, 0xAB);
	lcm_dcs_write_seq_static(ctx, 0x4E, 0xAA);
	lcm_dcs_write_seq_static(ctx, 0x4F, 0xA7);
	lcm_dcs_write_seq_static(ctx, 0x50, 0xA1);
	lcm_dcs_write_seq_static(ctx, 0x51, 0x9B);
	lcm_dcs_write_seq_static(ctx, 0x52, 0x97);
	lcm_dcs_write_seq_static(ctx, 0x53, 0x92);
	lcm_dcs_write_seq_static(ctx, 0x54, 0x89);

	lcm_dcs_write_seq_static(ctx, 0x58, 0xDF);
	lcm_dcs_write_seq_static(ctx, 0x59, 0xDA);
	lcm_dcs_write_seq_static(ctx, 0x5A, 0xCA);
	lcm_dcs_write_seq_static(ctx, 0x5B, 0xC2);
	lcm_dcs_write_seq_static(ctx, 0x5C, 0xB4);
	lcm_dcs_write_seq_static(ctx, 0x5D, 0xAB);
	lcm_dcs_write_seq_static(ctx, 0x5E, 0xA9);
	lcm_dcs_write_seq_static(ctx, 0x5F, 0xA8);
	lcm_dcs_write_seq_static(ctx, 0x60, 0xA7);
	lcm_dcs_write_seq_static(ctx, 0x61, 0xA6);
	lcm_dcs_write_seq_static(ctx, 0x62, 0xA0);
	lcm_dcs_write_seq_static(ctx, 0x63, 0x98);
	lcm_dcs_write_seq_static(ctx, 0x64, 0x90);
	lcm_dcs_write_seq_static(ctx, 0x65, 0x8A);
	lcm_dcs_write_seq_static(ctx, 0x66, 0x84);
	lcm_dcs_write_seq_static(ctx, 0x67, 0x77);
	lcm_dcs_write_seq_static(ctx, 0x6F, 0x00);

	lcm_dcs_write_seq_static(ctx, 0xFF, 0xE0);		 
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x35, 0x82);
	lcm_dcs_write_seq_static(ctx, 0xFF, 0xF0);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x5A, 0x00);	
	lcm_dcs_write_seq_static(ctx, 0xFF, 0xD0);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x53, 0x22);
	lcm_dcs_write_seq_static(ctx, 0x54, 0x02);
	lcm_dcs_write_seq_static(ctx, 0xFF, 0xC0);
	lcm_dcs_write_seq_static(ctx, 0xFE, 0xC0); 
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x9C, 0x11);
	lcm_dcs_write_seq_static(ctx, 0x9D, 0x11);
	lcm_dcs_write_seq_static(ctx, 0xFF, 0x20);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x01);
	lcm_dcs_write_seq_static(ctx, 0x20, 0xF0);
	lcm_dcs_write_seq_static(ctx, 0xFF, 0x10);
	lcm_dcs_write_seq_static(ctx, 0xFB, 0x10);
	lcm_dcs_write_seq_static(ctx, 0x53, 0x24);
	lcm_dcs_write_seq_static(ctx, 0x35, 0x00);
	lcm_dcs_write_seq_static(ctx, 0x11);
	msleep(10);
	lcm_dcs_write_seq_static(ctx, 0x29);
	msleep(70);
	pr_info("%s-\n", __func__);
}

static int lcm_disable(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	int blank_mode = FB_BLANK_POWERDOWN;
	struct fb_event event;

	if (!ctx->enabled)
		return 0;

	event.info  = NULL;
	event.data = &blank_mode;
	pr_info("%s for gesture\n", __func__);
	fb_notifier_call_chain(FB_EARLY_EVENT_BLANK, &event);

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_POWERDOWN;
		backlight_update_status(ctx->backlight);
	}

	ctx->enabled = false;

	return 0;
}

static int lcm_unprepare(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	if (!ctx->prepared)
		return 0;

	lcm_dcs_write_seq_static(ctx, 0x28);
	lcm_dcs_write_seq_static(ctx, 0x10);
	usleep_range(60 * 1000, 60 * 1000);

	ctx->error = 0;
	ctx->prepared = false;

#if defined(CONFIG_RT5081_PMU_DSV) || defined(CONFIG_MT6370_PMU_DSV)
	lcm_panel_bias_disable();
#else
#ifdef VENDOR_EDIT
/* shifan@PSW.BSP.TP.Function, 2020/02/20, Add for TP common code */
    pr_info("%s: tp_gesture_enable_flag = %d \n", __func__, tp_gesture_enable_flag());
    if ((0 == tp_gesture_enable_flag())||(esd_flag == 1)) {
#endif /*VENDOR_EDIT*/
	/*ctx->reset_gpio =
		devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(ctx->dev, "%s: cannot get reset_gpio %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	gpiod_set_value(ctx->reset_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);
	*/

	ctx->bias_neg = devm_gpiod_get_index(ctx->dev,
		"bias", 1, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_neg)) {
		dev_err(ctx->dev, "%s: cannot get bias_neg %ld\n",
			__func__, PTR_ERR(ctx->bias_neg));
		return PTR_ERR(ctx->bias_neg);
	}
	gpiod_set_value(ctx->bias_neg, 0);
	devm_gpiod_put(ctx->dev, ctx->bias_neg);

	udelay(1000);

	ctx->bias_pos = devm_gpiod_get_index(ctx->dev,
		"bias", 0, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_pos)) {
		dev_err(ctx->dev, "%s: cannot get bias_pos %ld\n",
			__func__, PTR_ERR(ctx->bias_pos));
		return PTR_ERR(ctx->bias_pos);
	}
	gpiod_set_value(ctx->bias_pos, 0);
	devm_gpiod_put(ctx->dev, ctx->bias_pos);
	msleep(1);
/*
	ctx->lcd_1p8_gpio = devm_gpiod_get(ctx->dev, "lcd-1p8", GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->lcd_1p8_gpio, 0);
	devm_gpiod_put(ctx->dev, ctx->lcd_1p8_gpio);*/
/*        usleep_range(2 * 1000, 2 * 1000);
        ctx->bias_en = devm_gpiod_get(ctx->dev, "ldo", GPIOD_OUT_HIGH);
        gpiod_set_value(ctx->bias_en, 0);
        devm_gpiod_put(ctx->dev, ctx->bias_en);*/
#ifdef VENDOR_EDIT
/*shifan@bsp.tp, 2020.0303, add for pulldown spi7 cs when doing sleep*/
        if( ctx->is_normal_mode ){
        //    switch_spi7cs_state(false);
        }
    }
#endif /*VENDOR_EDIT*/
#endif
	return 0;
}

static int lcm_panel_poweron(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);
	int ret;

	pr_info("%s+\n", __func__);
     if ((0 == tp_gesture_enable_flag())||(esd_flag == 1)) {
	/*usleep_range(20 * 1000, 20 * 1000);
	ctx->lcd_1p8_gpio = devm_gpiod_get(ctx->dev, "lcd-1p8", GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->lcd_1p8_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->lcd_1p8_gpio);
	msleep(1);*/
		/*liuchao@rm.bsp.tp.basic, 2020.0630, add for restoring spi7 cs when doing wakeup*/	
	if(ctx->is_normal_mode) {
	//	switch_spi7cs_state(true);
	}

	ctx->bias_pos = devm_gpiod_get_index(ctx->dev,
		"bias", 0, GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->bias_pos, 1);
	devm_gpiod_put(ctx->dev, ctx->bias_pos);

	udelay(2000);
	ctx->bias_neg = devm_gpiod_get_index(ctx->dev,
		"bias", 1, GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->bias_neg, 1);
	devm_gpiod_put(ctx->dev, ctx->bias_neg);
	_lcm_i2c_write_bytes(0x0, 0xf);
	_lcm_i2c_write_bytes(0x1, 0xf);
	
	usleep_range(10 * 1000, 10 * 1000);

	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
        usleep_range(5 * 1000, 5 * 1000);
	gpiod_set_value(ctx->reset_gpio, 1);
	usleep_range(5 * 1000, 5 * 1000);
	gpiod_set_value(ctx->reset_gpio, 0);
	usleep_range(5 * 1000, 5 * 1000);
        devm_gpiod_put(ctx->dev, ctx->reset_gpio);
     }
	pr_info("%s-\n", __func__);
	return 0;
}

static int lcm_panel_poweroff(struct drm_panel *panel)
{
	return 0;
}

static int lcm_prepare(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);
	int ret;

	pr_info("%s\n", __func__);
	if (ctx->prepared)
		return 0;

#if defined(CONFIG_RT5081_PMU_DSV) || defined(CONFIG_MT6370_PMU_DSV)
	lcm_panel_bias_enable();
#else
	pr_info("%s-\n", __func__);
/*	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	usleep_range(5 * 1000, 5 * 1000);
	gpiod_set_value(ctx->reset_gpio, 0);
	usleep_range(5 * 1000, 5 * 1000);*/
/*	ctx->bias_pos = devm_gpiod_get_index(ctx->dev,
		"bias", 0, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_pos)) {
		dev_err(ctx->dev, "%s: cannot get bias_pos %ld\n",
			__func__, PTR_ERR(ctx->bias_pos));
		return PTR_ERR(ctx->bias_pos);
	}
	gpiod_set_value(ctx->bias_pos, 1);
	devm_gpiod_put(ctx->dev, ctx->bias_pos);

	udelay(2000);

	ctx->bias_neg = devm_gpiod_get_index(ctx->dev,
		"bias", 1, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_neg)) {
		dev_err(ctx->dev, "%s: cannot get bias_neg %ld\n",
			__func__, PTR_ERR(ctx->bias_neg));
		return PTR_ERR(ctx->bias_neg);
	}
	gpiod_set_value(ctx->bias_neg, 1);
	devm_gpiod_put(ctx->dev, ctx->bias_neg);
	_lcm_i2c_write_bytes(0x0, 0xf);
	_lcm_i2c_write_bytes(0x1, 0xf);
    msleep(10);*/
#endif

	ctx->reset_gpio = devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->reset_gpio, 1);
	usleep_range(5 * 1000, 5 * 1000);
	gpiod_set_value(ctx->reset_gpio, 0);
	usleep_range(5 * 1000, 5 * 1000);
	gpiod_set_value(ctx->reset_gpio, 1);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);
	usleep_range(10 * 1000, 10 * 1000);
	#ifdef VENDOR_EDIT
/*shifan@bsp.tp, 2020.0303, add for restoring spi7 cs when doing wakeup*/
	if(ctx->is_normal_mode) {
	//	switch_spi7cs_state(true);
		lcd_queue_load_tp_fw();
	}
#endif /* VENDOR_EDIT */
	lcm_panel_init(ctx);

	ret = ctx->error;
	if (ret < 0)
		lcm_unprepare(panel);

	ctx->prepared = true;

#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_rst(panel);
#endif
#ifdef PANEL_SUPPORT_READBACK
	lcm_panel_get_data(ctx);
#endif
#ifdef VENDOR_EDIT
/*shifan@bsp.tp, 2020.0303, add for restoring spi7 cs when doing wakeup*/
//	if(ctx->is_normal_mode) {
//		switch_spi7cs_state(true);
//		lcd_queue_load_tp_fw();
//	}
#endif /* VENDOR_EDIT */
	return ret;
}

static int lcm_enable(struct drm_panel *panel)
{
	struct lcm *ctx = panel_to_lcm(panel);

	if (ctx->enabled)
		return 0;

	if (ctx->backlight) {
		ctx->backlight->props.power = FB_BLANK_UNBLANK;
		backlight_update_status(ctx->backlight);
	}

	ctx->enabled = true;

	return 0;
}

#define VAC (2400)
#define HAC (1080)

static struct drm_display_mode default_mode = {
	.clock = 326420,
	.hdisplay = HAC,
	.hsync_start = HAC + 356,//HFP
	.hsync_end = HAC + 356 + 22,//HSA
	.htotal = HAC + 356 + 22 + 22,//HBP1289
	.vdisplay = 2400,
	.vsync_start = VAC + 1291,//VFP
	.vsync_end = VAC + 1291 + 10,//VSA
	.vtotal = VAC + 1291 + 10 + 10,//VBP4948
	.vrefresh = 60,
};

static struct drm_display_mode performance_mode = {
	.clock = 326420,
	.hdisplay = HAC,
	.hsync_start = HAC + 356,//HFP
	.hsync_end = HAC + 356 + 22,//HSA
	.htotal = HAC + 356 + 22 + 22,//HBP
	.vdisplay = VAC,
	.vsync_start = VAC + 54,//VFP
	.vsync_end = VAC + 54 + 10,//VSA
	.vtotal = VAC + 54 + 10 + 10,//VBP3299
	.vrefresh = 90,
};

#if defined(CONFIG_MTK_PANEL_EXT)
static struct mtk_panel_params ext_params = {
	.pll_clk = 553,
	.vfp_low_power = 2528,//45hz
	.cust_esd_check = 0,
	.esd_check_enable = 1,
	.lcm_esd_check_table[0] = {
		.cmd = 0x0A, .count = 1, .para_list[0] = 0x9c, .mask_list[0] = 0x9c,
	},
	.output_mode = MTK_PANEL_DSC_SINGLE_PORT,
	.dsc_params = {
		.enable = 1,
		.ver = 17,
		.slice_mode = 1,
		.rgb_swap = 0,
		.dsc_cfg = 34,
		.rct_on = 1,
		.bit_per_channel = 8,
		.dsc_line_buf_depth = 9,
		.bp_enable = 1,
		.bit_per_pixel = 128,
		.pic_height = 2400,
		.pic_width = 1080,
		.slice_height = 8,
		.slice_width = 540,
		.chunk_size = 540,
		.xmit_delay = 170,
		.dec_delay = 526,
		.scale_value = 32,
		.increment_interval = 43,
		.decrement_interval = 7,
		.line_bpg_offset = 12,
		.nfl_bpg_offset = 3511,
		.slice_bpg_offset = 3255,
		.initial_offset = 6144,
		.final_offset = 7072,
		.flatness_minqp = 3,
		.flatness_maxqp = 12,
		.rc_model_size = 8192,
		.rc_edge_factor = 6,
		.rc_quant_incr_limit0 = 11,
		.rc_quant_incr_limit1 = 11,
		.rc_tgt_offset_hi = 3,
		.rc_tgt_offset_lo = 3,
		},
	.data_rate = 1107,
	.dyn_fps = {
		.switch_en = 1, .vact_timing_fps = 90,
	},
};

static struct mtk_panel_params ext_params_90hz = {
	.pll_clk = 553,
	.vfp_low_power = 1291,//60hz
	.cust_esd_check = 0,
	.esd_check_enable = 1,
	.lcm_esd_check_table[0] = {
		.cmd = 0x0A, .count = 1, .para_list[0] = 0x9c, .mask_list[0] = 0x9c,
	},
	.output_mode = MTK_PANEL_DSC_SINGLE_PORT,
	.dsc_params = {
		.enable = 1,
		.ver = 17,
		.slice_mode = 1,
		.rgb_swap = 0,
		.dsc_cfg = 34,
		.rct_on = 1,
		.bit_per_channel = 8,
		.dsc_line_buf_depth = 9,
		.bp_enable = 1,
		.bit_per_pixel = 128,
		.pic_height = 2400,
		.pic_width = 1080,
		.slice_height = 8,
		.slice_width = 540,
		.chunk_size = 540,
		.xmit_delay = 170,
		.dec_delay = 526,
		.scale_value = 32,
		.increment_interval = 43,
		.decrement_interval = 7,
		.line_bpg_offset = 12,
		.nfl_bpg_offset = 3511,
		.slice_bpg_offset = 3255,
		.initial_offset = 6144,
		.final_offset = 7072,
		.flatness_minqp = 3,
		.flatness_maxqp = 12,
		.rc_model_size = 8192,
		.rc_edge_factor = 6,
		.rc_quant_incr_limit0 = 11,
		.rc_quant_incr_limit1 = 11,
		.rc_tgt_offset_hi = 3,
		.rc_tgt_offset_lo = 3,
		},
	.data_rate = 1107,
	.dyn_fps = {
		.switch_en = 1, .vact_timing_fps = 90,
	},
};

static int lcm_setbacklight_cmdq(void *dsi, dcs_write_gce cb,
		void *handle, unsigned int level)
{
	char bl_tb3[] = {0x53, 0x24};
	char bl_tb1[] = {0xFF, 0x10};
	char bl_tb2[] = {0xFB, 0x01};
	char bl_tb0[] = {0x51, 0xFF, 0xFF};
	pr_err("%s backlight = %d\n", __func__, level);
	if (level > 4095)
		level = 4095;
	if (get_boot_mode() == KERNEL_POWER_OFF_CHARGING_BOOT && level > 0) {
		level = 2047;
	}
	bl_tb0[1] = level >> 8;
	bl_tb0[2] = level & 0xFF;
	esd_brightness = level;
	if (!cb)
		return -1;

	pr_err("%s SYQ bl_tb0[1]=%x, bl_tb0[2]=%x\n", __func__, bl_tb0[1], bl_tb0[2]);
	cb(dsi, handle, bl_tb1, ARRAY_SIZE(bl_tb1));
	cb(dsi, handle, bl_tb2, ARRAY_SIZE(bl_tb2));
	if (cabc_dimming_on)
	{
		if (frame_count > 6)
		{
			cabc_dimming_on = false;
			frame_count = 0;
			cb(dsi, handle, bl_tb3, ARRAY_SIZE(bl_tb3));
		}else{
			frame_count += 1;
		}
	}
	cb(dsi, handle, bl_tb0, ARRAY_SIZE(bl_tb0));
#ifdef VENDOR_EDIT
/* shiyaqiang@RM.MM.LCD.Driver 20180621 add for keep cabc mode after resume*/
	if ((last_brightness == 0) && (cabc_lastlevel != 0)) {
		msleep(5);
		cabc_switch(dsi,cb,handle,cabc_lastlevel);
	}
	last_brightness = level;
#endif
	return 0;
}

static int oppo_esd_backlight_check(void *dsi, dcs_write_gce cb,
		void *handle)
{
	char bl_tb0[] = {0x51, 0x07, 0xff};

	pr_err("%s esd_backlight = %d\n", __func__, esd_brightness);
	bl_tb0[1] = esd_brightness >> 8;
	bl_tb0[2] = esd_brightness & 0xFF;
	if (!cb)
		return -1;
	pr_err("%s bl_tb0[1]=%x, bl_tb0[2]=%x\n", __func__, bl_tb0[1], bl_tb0[2]);
	cb(dsi, handle, bl_tb0, ARRAY_SIZE(bl_tb0));

	return 1;
}

static int mtk_panel_ext_param_set(struct drm_panel *panel,
			 unsigned int mode)
{
	struct mtk_panel_ext *ext = find_panel_ext(panel);
	int ret = 0;
	pr_err("%s ,%d\n", __func__,mode);
	if (mode == 0)
		ext->params = &ext_params;
	else if (mode == 2)
		ext->params = &ext_params_90hz;
	else
		ret = 1;

	return ret;
}

static int mtk_panel_ext_param_get(struct mtk_panel_params *ext_para,
			 unsigned int mode)
{
	int ret = 0;
	pr_err("%s ,%d\n", __func__,mode);
	if (mode == 0)
		ext_para = &ext_params;
	else if (mode == 2)
		ext_para = &ext_params_90hz;
	else
		ret = 1;

	return ret;

}

static void cabc_switch(void *dsi, dcs_write_gce cb,
		void *handle, unsigned int cabc_mode)
{
	char bl_tb0[] = {0x55, 0x00};
	char bl_tb1[] = {0xFF, 0x10};
	char bl_tb2[] = {0xFB, 0x01};
	char bl_tb3[] = {0x53, 0x2C};
	char bl_tb4[] = {0x03, 0xC0};
	char bl_tb5[] = {0x03, 0x00};
	char bl_tb6[] = {0xFF, 0x23};
/*	if(cabc_mode == 2)
		return;
	if(cabc_mode == 3)
		cabc_mode = 2;
*/
	pr_err("%s cabc = %d\n", __func__, cabc_mode);
	bl_tb0[1] = (u8)cabc_mode;

	cb(dsi, handle, bl_tb6, ARRAY_SIZE(bl_tb6));//FF  23
	cb(dsi, handle, bl_tb2, ARRAY_SIZE(bl_tb2));//FB  01
	if(cabc_mode == 0)
		cb(dsi, handle, bl_tb4, ARRAY_SIZE(bl_tb4));//03  C0
	else
		cb(dsi, handle, bl_tb5, ARRAY_SIZE(bl_tb5));//03  00
    msleep(5);
	cb(dsi, handle, bl_tb1, ARRAY_SIZE(bl_tb1));//FF 10
	cb(dsi, handle, bl_tb2, ARRAY_SIZE(bl_tb2));//FB 01
	cabc_dimming_on = true;
	frame_count = 0;
	if (last_brightness != 0)
		cb(dsi, handle, bl_tb3, ARRAY_SIZE(bl_tb3));//FB 01
	if(cabc_mode != 0){
		cb(dsi, handle, bl_tb0, ARRAY_SIZE(bl_tb0));//55 0X
	}
#ifdef VENDOR_EDIT
/* shiyaqiang@RM.MM.LCD.Driver 20180621 add for keep cabc mode after resume*/
	cabc_lastlevel = cabc_mode;
#endif
}

static int panel_ext_reset(struct drm_panel *panel, int on)
{
	struct lcm *ctx = panel_to_lcm(panel);

	ctx->reset_gpio =
		devm_gpiod_get(ctx->dev, "reset", GPIOD_OUT_HIGH);
	gpiod_set_value(ctx->reset_gpio, on);
	devm_gpiod_put(ctx->dev, ctx->reset_gpio);

	return 0;
}

static struct mtk_panel_funcs ext_funcs = {
	.reset = panel_ext_reset,
	.set_backlight_cmdq = lcm_setbacklight_cmdq,
	.esd_backlight_check = oppo_esd_backlight_check,
	.ext_param_set = mtk_panel_ext_param_set,
	.ext_param_get = mtk_panel_ext_param_get,
	.panel_poweron = lcm_panel_poweron,
	.panel_poweroff = lcm_panel_poweroff,
	//.mode_switch = mode_switch,
	.cabc_switch = cabc_switch,
};
#endif

struct panel_desc {
	const struct drm_display_mode *modes;
	unsigned int num_modes;

	unsigned int bpc;

	struct {
		unsigned int width;
		unsigned int height;
	} size;

	struct {
		unsigned int prepare;
		unsigned int enable;
		unsigned int disable;
		unsigned int unprepare;
	} delay;
};

static int lcm_get_modes(struct drm_panel *panel)
{
	struct drm_display_mode *mode;
	struct drm_display_mode *mode2;

	mode = drm_mode_duplicate(panel->drm, &default_mode);
	if (!mode) {
		dev_err(panel->drm->dev, "failed to add mode %ux%ux@%u\n",
			default_mode.hdisplay, default_mode.vdisplay,
			default_mode.vrefresh);
		return -ENOMEM;
	}

	drm_mode_set_name(mode);
	mode->type = DRM_MODE_TYPE_DRIVER | DRM_MODE_TYPE_PREFERRED;
	drm_mode_probed_add(panel->connector, mode);

	mode2 = drm_mode_duplicate(panel->drm, &performance_mode);
	if (!mode2) {
		dev_err(panel->drm->dev, "failed to add mode %ux%ux@%u\n",
			performance_mode.hdisplay,
			performance_mode.vdisplay,
			performance_mode.vrefresh);
		return -ENOMEM;
	}

	drm_mode_set_name(mode2);
	mode2->type = DRM_MODE_TYPE_DRIVER;
	drm_mode_probed_add(panel->connector, mode2);

	panel->connector->display_info.width_mm = 69;
	panel->connector->display_info.height_mm = 150;

	return 1;
}

static const struct drm_panel_funcs lcm_drm_funcs = {
	.disable = lcm_disable,
	.unprepare = lcm_unprepare,
	.prepare = lcm_prepare,
	.enable = lcm_enable,
	.get_modes = lcm_get_modes,
};

static int lcm_probe(struct mipi_dsi_device *dsi)
{
	struct device *dev = &dsi->dev;
	struct lcm *ctx;
	struct device_node *backlight;
	int ret;
	struct device_node *dsi_node, *remote_node = NULL, *endpoint = NULL;

	dsi_node = of_get_parent(dev->of_node);
	if (dsi_node) {
		endpoint = of_graph_get_next_endpoint(dsi_node, NULL);
		if (endpoint) {
			remote_node = of_graph_get_remote_port_parent(endpoint);
			pr_info("device_node name:%s\n", remote_node->name);
                   }
	}
	if (remote_node != dev->of_node) {
		pr_info("%s+ skip probe due to not current lcm.\n", __func__);
		return 0;
	}	

	pr_info("%s+\n", __func__);
	ctx = devm_kzalloc(dev, sizeof(struct lcm), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	mipi_dsi_set_drvdata(dsi, ctx);

	ctx->dev = dev;
	dsi->lanes = 4;
	dsi->format = MIPI_DSI_FMT_RGB888;
	dsi->mode_flags = MIPI_DSI_MODE_VIDEO
			 | MIPI_DSI_MODE_LPM | MIPI_DSI_MODE_EOT_PACKET
			 | MIPI_DSI_CLOCK_NON_CONTINUOUS;

	backlight = of_parse_phandle(dev->of_node, "backlight", 0);
	if (backlight) {
		ctx->backlight = of_find_backlight_by_node(backlight);
		of_node_put(backlight);

		if (!ctx->backlight)
			return -EPROBE_DEFER;
	}
/*
	ctx->lcd_1p8_gpio = devm_gpiod_get(dev, "lcd-1p8", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->lcd_1p8_gpio)) {
		dev_err(dev, "cannot get lcd-1p8-gpios %ld\n",
			PTR_ERR(ctx->lcd_1p8_gpio));
	//	return PTR_ERR(ctx->lcd_1p8_gpio);
	}
	devm_gpiod_put(dev, ctx->lcd_1p8_gpio);*/
	ctx->reset_gpio = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->reset_gpio)) {
		dev_err(dev, "%s: cannot get reset-gpios %ld\n",
			__func__, PTR_ERR(ctx->reset_gpio));
		return PTR_ERR(ctx->reset_gpio);
	}
	devm_gpiod_put(dev, ctx->reset_gpio);

	ctx->bias_pos = devm_gpiod_get_index(dev, "bias", 0, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_pos)) {
		dev_err(dev, "%s: cannot get bias-pos 0 %ld\n",
			__func__, PTR_ERR(ctx->bias_pos));
		return PTR_ERR(ctx->bias_pos);
	}
	devm_gpiod_put(dev, ctx->bias_pos);

	ctx->bias_neg = devm_gpiod_get_index(dev, "bias", 1, GPIOD_OUT_HIGH);
	if (IS_ERR(ctx->bias_neg)) {
		dev_err(dev, "%s: cannot get bias-neg 1 %ld\n",
			__func__, PTR_ERR(ctx->bias_neg));
		return PTR_ERR(ctx->bias_neg);
	}
	devm_gpiod_put(dev, ctx->bias_neg);

	ctx->prepared = true;
	ctx->enabled = true;

	drm_panel_init(&ctx->panel);
	ctx->panel.dev = dev;
	ctx->panel.funcs = &lcm_drm_funcs;

	ret = drm_panel_add(&ctx->panel);
	if (ret < 0)
		return ret;

	ret = mipi_dsi_attach(dsi);
	if (ret < 0)
		drm_panel_remove(&ctx->panel);

#if defined(CONFIG_MTK_PANEL_EXT)
	mtk_panel_tch_handle_reg(&ctx->panel);
	ret = mtk_panel_ext_create(dev, &ext_params, &ext_funcs, &ctx->panel);
	if (ret < 0)
		return ret;
#endif
#ifdef VENDOR_EDIT
/*shifan@bsp.tp 2020.0310 add for ftm */
    ctx->is_normal_mode = true;
    if( META_BOOT == get_boot_mode() || FACTORY_BOOT == get_boot_mode() )
        ctx->is_normal_mode = false;
    pr_info("%s: is_normal_mode = %d \n", __func__, ctx->is_normal_mode);
#endif /*VENDOR_EDIT*/
	pr_info("%s-\n", __func__);
	oplus_max_normal_brightness = 3067;
	disp_aal_set_dre_en(1);
	register_device_proc("lcd", "NT36672C_JDI_ZOLA", "nt_jdi_4096");
	return ret;
}

static int lcm_remove(struct mipi_dsi_device *dsi)
{
	struct lcm *ctx = mipi_dsi_get_drvdata(dsi);

	mipi_dsi_detach(dsi);
	drm_panel_remove(&ctx->panel);

	return 0;
}

static const struct of_device_id lcm_of_match[] = {
	{ .compatible = "oppo20611,nt36672c,tm,fhdp,dsi,vdo,dphy", },
	{ }
};

MODULE_DEVICE_TABLE(of, lcm_of_match);

static struct mipi_dsi_driver lcm_driver = {
	.probe = lcm_probe,
	.remove = lcm_remove,
	.driver = {
		.name = "oppo20611_nt36672c_tm_fhdp_dsi_vdo_dphy",
		.owner = THIS_MODULE,
		.of_match_table = lcm_of_match,
	},
};

module_mipi_dsi_driver(lcm_driver);

MODULE_AUTHOR("xxx<>");
MODULE_DESCRIPTION("nt36672c tm dphy vdo lcm_drv");
MODULE_LICENSE("GPL v2");


