/******************************************************************
** Copyright (C), 2004-2017, OPPO Mobile Comm Corp., Ltd.
** VENDOR_EDIT
** File: - oppo_sensor_devinfo.c
** Description: Source file for sensor device infomation registered in scp.
** Version: 1.0
** Date : 2017/12/12
** Author: Fei.Mo@PSW.BSP.Sensor
**
** --------------------------- Revision History: ---------------------
* <version> <date>      <author>                    <desc>
* Revision 1.0      2017/12/12        Fei.Mo@PSW.BSP.Sensor     Created,need to sync acp info
*******************************************************************/
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/time.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/libfdt.h>
#include <linux/suspend.h>
#include <soc/oppo/oppo_project.h>
#include "sensor_devinfo.h"
#include <linux/gpio.h>
#ifdef CONFIG_OPPO_SENSOR_MTK6885
#include "hf_sensor_type.h"
#include "mtk_nanohub.h"
#include "scp_helper.h"
#include "scp_excep.h"
#else
#include <SCP_sensorHub.h>
#include <hwmsensor.h>
#include "SCP_power_monitor.h"
#endif

#ifdef CONFIG_OPPO_SENSOR_MTK6885
extern int mtk_nanohub_set_cmd_to_hub(uint8_t sensor_id, enum CUST_ACTION action, void *data);
extern int mtk_nanohub_req_send(union SCP_SENSOR_HUB_DATA *data);
extern int mtk_nanohub_cfg_to_hub(uint8_t sensor_id, uint8_t *data, uint8_t count);
#endif

#define DEV_TAG                     "[sensor_devinfo] "
#define DEVINFO_LOG(fmt, args...)   pr_err(DEV_TAG"%s %d : "fmt, __func__, __LINE__, ##args)

#define UINT2Ptr(n)     (uint32_t *)(n)
#define Ptr2UINT32(p)   (uint32_t)(p)

#define IS_SUPPROT_HWCALI           (0x01)
#define IS_IN_FACTORY_MODE          (0x02)
#define IS_SUPPORT_NEW_ARCH         (0x03)
#define GYRO_CALI_VERSION           (0x04)
#define ACC_CALI_RANGE              (0x05)
#define DO_MAG_SELFTEST             (0x06)
#define IS_SUPPORT_MTK_CALI         (0x07)

#define RED_MAX_LUX                 (0x01)
#define GREEN_MAX_LUX               (0x02)
#define BLUE_MAX_LUX                (0x03)
#define WHITE_MAX_LUX               (0x04)
#define CALI_COE                    (0x05)
#define ROW_COE                     (0x06)

#define OPPOCUSTOM_FILE "/dev/block/by-name/oppo_custom"

struct delayed_work parameter_work;
struct delayed_work utc_work;

struct cali_data* g_cali_data = NULL;
static bool is_parameter_updated = false;

static char para_buf[3][128] = {"", "", ""};
static bool is_support_new_arch = false;
static bool is_support_mtk_cali = false;
static struct oppo_als_cali_data *gdata = NULL;
static struct proc_dir_entry *sensor_proc_dir = NULL;
static int gyro_cali_version = 1;
static char acc_cali_range[11] = {0};
atomic_t utc_suspend;

struct sensorlist_info_t {
    char name[16];
};

bool is_support_new_arch_func(void)
{
    return is_support_new_arch;
}

bool is_support_mtk_cali_func(void)
{
    return is_support_mtk_cali;
}

enum {
    accel,
    gyro,
    mag,
    als,
    ps,
    baro,
    maxhandle,
};

static struct sensorlist_info_t sensorlist_info[maxhandle];

typedef struct {
    unsigned int        nMagicNum1;
    unsigned int        nMagicNum2;
    unsigned int        nOther1[2];
    unsigned char       nOther2[64];
    unsigned char       Sensor[256];
} sensor_config_info_t;

const static struct fdt_property* oplus_get_dts_feature(int handle, char* node_name, char* feature_name)
{
    int index = 0;
    int offset = 0;
    int len = 0;
    char node[16];
    char *device_name = NULL;
    const struct fdt_property *prop = NULL;
    void *fdt = initial_boot_params;

    if (handle >= maxhandle) {
        DEVINFO_LOG("ivalid sensor handle %d\n", handle);
        return NULL;
    }

    for (index = 1; index <= 3; index++) {
        sprintf(node, "%s_%d", node_name, index);
        offset = fdt_path_offset(fdt, node);
        if (offset < 0) {
            DEVINFO_LOG("get %s fail", node);
            return NULL;
        }

        device_name = (char *) fdt_getprop(fdt, offset, "device_name", &len);
        if (device_name == NULL) {
            DEVINFO_LOG("get device_name prop fail \n");
            return NULL;
        } else {
            DEVINFO_LOG("device_name = %s", device_name);
        }

        if (!strcmp(device_name, sensorlist_info[handle].name)) {
            prop = fdt_get_property(fdt, offset, feature_name, &len);
            if (!prop) {
                DEVINFO_LOG("get prop fail \n");
                return NULL;
            }
            DEVINFO_LOG("feature_addr %d, len = %d\n", (uint32_t *)prop->data, fdt32_to_cpu(prop->len));
            break;
        }
    }
    return prop;
}

static void get_new_arch_info(void)
{
    void *fdt = initial_boot_params;
    int offset = 0;
    int is_support = 0;
    int len = 0;
    char *data = NULL;

    offset = fdt_path_offset(fdt, "/odm/cali_arch");
    if (offset < 0) {
        DEVINFO_LOG("get cali_arch fail");
        return;
    }

    data = (char *)fdt_getprop(fdt, offset, "new-arch-supported", &len);
    if (NULL == data) {
        DEVINFO_LOG("get new-arch-supported fail");
        return;
    } else {
        is_support = fdt32_to_cpu(*(int *)data);
        DEVINFO_LOG("get new-arch-supported = %d", is_support);
    }

    if (1 == is_support) {
        is_support_new_arch = true;
    } else {
        is_support_new_arch = false;
    }
}

static void get_mtk_cali_info(void)
{
    void *fdt = initial_boot_params;
    int offset = 0;
    int mtk_cali_is_support = 0;
    int len = 0;
    char *data = NULL;

    offset = fdt_path_offset(fdt, "/odm/mtk_cali");
    if (offset < 0) {
        DEVINFO_LOG("get cali_arch fail");
        return;
    }

    data = (char *)fdt_getprop(fdt, offset, "mtk-cali-supported", &len);
    if (NULL == data) {
        DEVINFO_LOG("get mtk-cali-supported fail");
        return;
    } else {
        mtk_cali_is_support = fdt32_to_cpu(*(int *)data);
        DEVINFO_LOG("get mtk-cali-supported = %d", mtk_cali_is_support);
    }

    if (1 == mtk_cali_is_support) {
        is_support_mtk_cali = true;
    } else {
        is_support_mtk_cali = false;
    }
}

static inline int handle_to_sensor(int handle)
{
    int sensor = -1;

    switch (handle) {
    case accel:
        sensor = ID_ACCELEROMETER;
        break;
    case gyro:
        sensor = ID_GYROSCOPE;
        break;
#ifdef CONFIG_OPPO_SENSOR_MTK6885
    case mag:
        sensor = ID_MAGNETIC_FIELD;
        break;
#else
    case mag:
        sensor = ID_MAGNETIC;
        break;
#endif
    case als:
        sensor = ID_LIGHT;
        break;
    case ps:
        sensor = ID_PROXIMITY;
        break;
    case baro:
        sensor = ID_PRESSURE;
        break;
    }
    return sensor;
}

static int read_oppo_custom(void *data)
{
    int ret = -1;
    loff_t pos = 0;
    mm_segment_t fs;
    struct file* pfile = NULL;
    sensor_config_info_t config_info;
    sensor_cali_file_v1_t *data_v1 = NULL;
    sensor_cali_file_v2_t *data_v2 = NULL;

    fs = get_fs();
    set_fs(KERNEL_DS);

    pfile = filp_open(OPPOCUSTOM_FILE, O_RDONLY | O_TRUNC, 0);
    if (IS_ERR(pfile)) {
        set_fs(fs);
        DEVINFO_LOG("failed to open file %s %p %d\n", OPPOCUSTOM_FILE, pfile, ret);
        return ret;
    }

    if (!is_support_new_arch) {
        data_v1 = (sensor_cali_file_v1_t *)data;
        if (!data_v1) {
            DEVINFO_LOG("data_v1 NULL\n");
            filp_close(pfile, NULL);
            set_fs(fs);
            return ret;
        }

        ret = vfs_read(pfile, (void *)(&config_info), sizeof(config_info), &pos);
        if (ret != sizeof(sensor_config_info_t)) {
            DEVINFO_LOG("failed to read file %s %p\n", OPPOCUSTOM_FILE, pfile);
            filp_close(pfile, NULL);
            set_fs(fs);
            return ret;
        }
        filp_close(pfile, NULL);
        set_fs(fs);

        memcpy(data_v1, config_info.Sensor, 256);
    } else {
        data_v2 = (sensor_cali_file_v2_t *)data;
        if (!data_v2) {
            DEVINFO_LOG("data_v2 NULL\n");
            filp_close(pfile, NULL);
            set_fs(fs);
            return ret;
        }

        ret = vfs_read(pfile, (void *)(&config_info), sizeof(config_info), &pos);
        if (ret != sizeof(sensor_config_info_t)) {
            DEVINFO_LOG("failed to read file %s %p\n", OPPOCUSTOM_FILE, pfile);
            filp_close(pfile, NULL);
            set_fs(fs);
            return ret;
        }
        filp_close(pfile, NULL);
        set_fs(fs);

        memcpy(data_v2, config_info.Sensor, 256);
    }
    DEVINFO_LOG("read success = %d\n", ret);

    return 0;
}

static int sensor_read_oppo_custom(struct cali_data *data)
{
    int ret = 0;
    int index = 0;
    sensor_cali_file_v1_t data_v1;
    sensor_cali_file_v2_t data_v2;

    if (!is_support_new_arch) {
        ret = read_oppo_custom(&data_v1);
        if (ret) {
            DEVINFO_LOG("read_oppo_custom error = %d\n", ret);
            return -EINVAL;
        }
        for (index = 0; index < 3; index++) {
            data->acc_data[index] = data_v1.GsensorData[index];
            data->gyro_data[index] = data_v1.GyroData[index];
        }
        for (index = 0; index < 6; index++) {
            data->ps_cali_data[index] = data_v1.ps_data[index];
        }
        data->als_factor = data_v1.gain_als;
    } else {
        ret = read_oppo_custom(&data_v2);
        if (ret) {
            DEVINFO_LOG("read_oppo_custom error = %d\n", ret);
            return -EINVAL;
        }
        for (index = 0; index < 3; index++) {
            data->acc_data[index] = data_v2.gsensor_data[index];
            data->gyro_data[index] = data_v2.gyro_data[index];
        }
        for (index = 0; index < 6; index++) {
            data->ps_cali_data[index] = data_v2.ps_data[index];
        }
        data->als_factor = data_v2.als_gain;
    }
    DEVINFO_LOG("acc[%d,%d,%d],gyro[%d,%d,%d],ps[%d,%d,%d,%d,%d,%d],als[%d]\n",
        data->acc_data[0], data->acc_data[1], data->acc_data[2],
        data->gyro_data[0], data->gyro_data[1], data->gyro_data[2],
        data->ps_cali_data[0], data->ps_cali_data[1], data->ps_cali_data[2],
        data->ps_cali_data[3], data->ps_cali_data[4], data->ps_cali_data[5],
        data->als_factor);
    return 0;
}

static void sync_utc2scp_work(struct work_struct *dwork)
{
    int err = 0;
    unsigned int len = 0;
    struct timex txc;
    struct rtc_time tm;
    union SCP_SENSOR_HUB_DATA rtc_req;
    uint32_t utc_data[4] = {0};

    if (atomic_read(&utc_suspend) == 1) {
        DEVINFO_LOG("Will suspend, stop sync utc \n");
        return;
    }

    do_gettimeofday(&(txc.time));
    rtc_time_to_tm(txc.time.tv_sec, &tm);

    utc_data[0] = (uint32_t)tm.tm_mday;
    utc_data[1] = (uint32_t)tm.tm_hour;
    utc_data[2] = (uint32_t)tm.tm_min;
    utc_data[3] = (uint32_t)tm.tm_sec;

    rtc_req.req.sensorType = 0;
    rtc_req.req.action = OPPO_ACTION_SCP_SYNC_UTC;
    rtc_req.req.data[0] = utc_data[0];
    rtc_req.req.data[1] = utc_data[1];
    rtc_req.req.data[2] = utc_data[2];
    rtc_req.req.data[3] = utc_data[3];

    len = sizeof(rtc_req.req);
#ifdef CONFIG_OPPO_SENSOR_MTK6885
    err = mtk_nanohub_req_send(&rtc_req);
#else
    err = scp_sensorHub_req_send(&rtc_req, &len, 1);
#endif
    if (err < 0 || rtc_req.rsp.action != OPPO_ACTION_SCP_SYNC_UTC) {
        DEVINFO_LOG("fail! err %d\n", err);
        return;
    }

    schedule_delayed_work(&utc_work, msecs_to_jiffies(2000));
}

int oppo_send_factory_mode_cmd_to_hub(int sensorType, int mode, void *result)
{
    int err = 0;
    unsigned int len = 0;
    union SCP_SENSOR_HUB_DATA fac_req;

    switch (sensorType) {
    case ID_ACCELEROMETER:
        DEVINFO_LOG("ID_ACCELEROMETER : send_factory_mode_cmd_to_hub");
        fac_req.req.sensorType = OPPO_ACCEL;
        fac_req.req.action = OPPO_ACTION_SET_FACTORY_MODE;
        fac_req.req.data[0] = mode;
        len = sizeof(fac_req.req);
#ifdef CONFIG_OPPO_SENSOR_MTK6885
        err = mtk_nanohub_req_send(&fac_req);
#else
        err = scp_sensorHub_req_send(&fac_req, &len, 1);
#endif
        if (err < 0 || fac_req.rsp.action != OPPO_ACTION_SET_FACTORY_MODE) {
            DEVINFO_LOG("fail! err %d\n", err);
            return -1;
        } else {
            *((uint8_t *) result) = fac_req.rsp.reserve[0];
        }
        break;
    case ID_PROXIMITY:
        DEVINFO_LOG("ID_PROXIMITY : send_factory_mode_cmd_to_hub");
        fac_req.req.sensorType = OPPO_PROXIMITY;
        fac_req.req.action = OPPO_ACTION_SET_FACTORY_MODE;
        fac_req.req.data[0] = mode;
        len = sizeof(fac_req.req);
#ifdef CONFIG_OPPO_SENSOR_MTK6885
        err = mtk_nanohub_req_send(&fac_req);
#else
        err = scp_sensorHub_req_send(&fac_req, &len, 1);
#endif
        if (err < 0 || fac_req.rsp.action != OPPO_ACTION_SET_FACTORY_MODE) {
            DEVINFO_LOG("fail! err %d\n", err);
            return -1;
        } else {
            *((uint8_t *) result) = fac_req.rsp.reserve[0];
        }
        break;
    default:
        DEVINFO_LOG("invalid sensortype %d\n", err);
    }
    return 1;
}

int oppo_send_selftest_cmd_to_hub(int sensorType, void *testresult)
{
    int err = 0;
    unsigned int len = 0;
    union SCP_SENSOR_HUB_DATA selftest_req;

    switch (sensorType) {
#ifdef CONFIG_OPPO_SENSOR_MTK6885
    case ID_MAGNETIC_FIELD:
#else
    case ID_MAGNETIC:
#endif
        DEVINFO_LOG("ID_MAGNETIC : oppo_send_selftest_cmd_to_hub");
        selftest_req.req.sensorType = OPPO_MAG;
        selftest_req.req.action = OPPO_ACTION_SELF_TEST;
        len = sizeof(selftest_req.req);
#ifdef CONFIG_OPPO_SENSOR_MTK6885
        err = mtk_nanohub_req_send(&selftest_req);
#else
        err = scp_sensorHub_req_send(&selftest_req, &len, 1);
#endif

        if (err < 0 || selftest_req.rsp.action != OPPO_ACTION_SELF_TEST) {
            DEVINFO_LOG("fail! err %d\n", err);
            return -1;
        } else {
            *((uint8_t *) testresult) = selftest_req.rsp.reserve[0];
        }
        break;
    default:
        DEVINFO_LOG("invalid sensortype %d\n", err);
    }
    return 1;
}

int get_sensor_parameter(struct cali_data *data)
{
    if (!g_cali_data) {
        DEVINFO_LOG("g_cali_data NULL! \n");
        return -EINVAL;
    }

    if (is_parameter_updated) {
        is_parameter_updated = false;
        sensor_read_oppo_custom(g_cali_data);
    }

    if (data) {
        memcpy(data, g_cali_data, sizeof(struct cali_data));
    } else {
        DEVINFO_LOG("data NULL! \n");
        return -EINVAL;
    }

    return 0;
}

void update_sensor_parameter(void)
{
    is_parameter_updated = true;
}

static int init_sensorlist_info(void)
{
    int err = 0;
    int handle = -1;
    int sensor = -1;
    int ret = -1;
    struct sensorInfo_t devinfo;

    for (handle = accel; handle < maxhandle; ++handle) {
        sensor = handle_to_sensor(handle);
        if (sensor < 0) {
            continue;
        }
#ifdef CONFIG_OPPO_SENSOR_MTK6885
        err = mtk_nanohub_set_cmd_to_hub(sensor,
                CUST_ACTION_GET_SENSOR_INFO, &devinfo);
#else
        err = sensor_set_cmd_to_hub(sensor,
                CUST_ACTION_GET_SENSOR_INFO, &devinfo);
#endif
        if (err < 0) {
            DEVINFO_LOG("sensor(%d) not register\n", sensor);
            strlcpy(sensorlist_info[handle].name,
                "NULL",
                sizeof(sensorlist_info[handle].name));
            continue;
        }

        DEVINFO_LOG("sensor(%s) register\n", devinfo.name);
        strlcpy(sensorlist_info[handle].name,
            devinfo.name,
            sizeof(sensorlist_info[handle].name));
        ret = 0;
    }
    return ret;
}

bool is_sensor_available(char *name)
{
    bool find = false;
    int handle = -1;

    for (handle = accel; handle < maxhandle; ++handle) {
        if (name && (!strcmp(sensorlist_info[handle].name, name))) {
            find = true;
            break;
        }
    }

    return find;
}

int get_light_sensor_type(void)
{
    int type = 1;
    const struct fdt_property *light_type_prop = NULL;

    light_type_prop = oplus_get_dts_feature(als, "/odm/light", "als_type");
    if (light_type_prop == NULL) {
        return NORMAL_LIGHT_TYPE;
    } else {
        type = fdt32_to_cpu(*((uint32_t *)light_type_prop->data));
    }

    DEVINFO_LOG("get_light_sensor_type = %d", type);
    return type;
}

int oppo_sensor_cfg_to_hub(uint8_t sensor_id, uint8_t *data, uint8_t count)
{
#ifdef CONFIG_OPPO_SENSOR_MTK6885
    return mtk_nanohub_cfg_to_hub(sensor_id, data, count);
#else
    return sensor_cfg_to_hub(sensor_id, data, count);
#endif
}

static int als_cali_read_func(struct seq_file *s, void *v)
{
    void *p = s->private;

    DEVINFO_LOG("Ptr2UINT32(p) = %d \n", Ptr2UINT32(p));
    switch (Ptr2UINT32(p)) {
    case RED_MAX_LUX:
        seq_printf(s, "%d", gdata->red_max_lux);
        break;
    case GREEN_MAX_LUX:
        seq_printf(s, "%d", gdata->green_max_lux);
        break;
    case BLUE_MAX_LUX:
        seq_printf(s, "%d", gdata->blue_max_lux);
        break;
    case WHITE_MAX_LUX:
        seq_printf(s, "%d", gdata->white_max_lux);
        break;
    case CALI_COE:
        seq_printf(s, "%d", gdata->cali_coe);
        break;
    case ROW_COE:
        seq_printf(s, "%d", gdata->row_coe);
        break;
    default:
        seq_printf(s, "not support\n");
    }
    return 0;
}

static ssize_t als_cali_write(struct file *filp, const char *ubuf, size_t cnt, loff_t *data)
{
    char buf[64] = {0};
    void* buf_str = buf;
    long val = 0;
    int ret = 0;
    struct seq_file *s = filp->private_data;
    void *p = s->private;
    int node = Ptr2UINT32(p);

    if (cnt >= sizeof(buf)) {
        return -EINVAL;
    } else {
        if (copy_from_user(buf_str, ubuf, cnt)) {
            return -EFAULT;
        }
    }

    ret = kstrtoul(buf, 0, (unsigned long *)&val);
    DEVINFO_LOG("node1 = %d,val = %d\n", node, val);

    switch (node) {
    case RED_MAX_LUX:
        gdata->red_max_lux = val;
        break;
    case GREEN_MAX_LUX:
        gdata->green_max_lux = val;
        break;
    case BLUE_MAX_LUX:
        gdata->blue_max_lux = val;
        break;
    case WHITE_MAX_LUX:
        gdata->white_max_lux = val;
        break;
    case CALI_COE:
        gdata->cali_coe = val;
        break;
    case ROW_COE:
        gdata->row_coe = val;
        break;
    default:
        DEVINFO_LOG("ivalid node type\n");
    }
    return cnt;
}

static int als_cali_open(struct inode *inode, struct file *file)
{
    return single_open(file, als_cali_read_func, PDE_DATA(inode));
}

static const struct file_operations als_cali_para_fops = {
    .owner = THIS_MODULE,
    .open  = als_cali_open,
    .write = als_cali_write,
    .read  = seq_read,
    .release = single_release,
};

static void get_accgyro_cali_version(void)
{
    const struct fdt_property *accgyro_prop = NULL;
    uint32_t acc_thrd_x;
    uint32_t acc_thrd_y;
    uint32_t acc_thrd_z;

    accgyro_prop = oplus_get_dts_feature(accel, "/odm/gsensor", "gyro_cali_version");
    if (accgyro_prop == NULL) {
        gyro_cali_version = 1;
    } else {
        gyro_cali_version = fdt32_to_cpu(*(uint32_t *)accgyro_prop->data);
    }
    DEVINFO_LOG("gyro_cali_version = %d", gyro_cali_version);

    accgyro_prop = oplus_get_dts_feature(accel, "/odm/gsensor", "acc_cali_range");
    if (accgyro_prop == NULL) {
        return;
    } else {
        acc_thrd_x = fdt32_to_cpu(*((uint32_t *)accgyro_prop->data));
        acc_thrd_y = fdt32_to_cpu(*((uint32_t *)accgyro_prop->data + 1));
        acc_thrd_z = fdt32_to_cpu(*((uint32_t *)accgyro_prop->data + 2));
        DEVINFO_LOG("acc range x y z [%d, %d, %d]", acc_thrd_x, acc_thrd_y, acc_thrd_z);
        sprintf(acc_cali_range, "%d %d %d", acc_thrd_x, acc_thrd_y, acc_thrd_z);
    }
}

static void oplus_als_cali_data_init(void)
{
    struct proc_dir_entry *pentry;
    struct oppo_als_cali_data *data = NULL;
    const struct fdt_property *row_coe_prop = NULL;

    DEVINFO_LOG("call \n");
    if (gdata) {
        DEVINFO_LOG("has been inited\n");
        return;
    }

    data = kzalloc(sizeof(struct oppo_als_cali_data), GFP_KERNEL);
    if (data == NULL) {
        DEVINFO_LOG("kzalloc fail\n");
        return;
    }
    gdata = data;

    row_coe_prop = oplus_get_dts_feature(als, "/odm/light", "als_ratio");
    if (row_coe_prop == NULL) {
        gdata->row_coe = 1000;
    } else {
        gdata->row_coe = fdt32_to_cpu(*(int *)row_coe_prop->data);
    }
    DEVINFO_LOG("row_coe = %d\n", gdata->row_coe);

    if (gdata->proc_oppo_als) {
        DEVINFO_LOG("proc_oppo_als has alread inited\n");
        return;
    }

    sensor_proc_dir = proc_mkdir("sensor", NULL);
    if (!sensor_proc_dir) {
        DEVINFO_LOG("can't create proc_sensor proc\n");
        return;
    }

    gdata->proc_oppo_als =  proc_mkdir("als_cali", sensor_proc_dir);
    if (!gdata->proc_oppo_als) {
        DEVINFO_LOG("can't create proc_oppo_als proc\n");
        return;
    }

    pentry = proc_create_data("red_max_lux", S_IRUGO | S_IWUGO, gdata->proc_oppo_als,
        &als_cali_para_fops, UINT2Ptr(RED_MAX_LUX));
    if (!pentry) {
        DEVINFO_LOG("create red_max_lux proc failed.\n");
        return;
    }

    pentry = proc_create_data("green_max_lux", S_IRUGO | S_IWUGO, gdata->proc_oppo_als,
        &als_cali_para_fops, UINT2Ptr(GREEN_MAX_LUX));
    if (!pentry) {
        DEVINFO_LOG("create red_max_lux proc failed.\n");
        return;
    }

    pentry = proc_create_data("blue_max_lux", S_IRUGO | S_IWUGO, gdata->proc_oppo_als,
        &als_cali_para_fops, UINT2Ptr(BLUE_MAX_LUX));
    if (!pentry) {
        DEVINFO_LOG("create red_max_lux proc failed.\n");
        return;
    }

    pentry = proc_create_data("white_max_lux", S_IRUGO | S_IWUGO, gdata->proc_oppo_als,
        &als_cali_para_fops, UINT2Ptr(WHITE_MAX_LUX));
    if (!pentry) {
        DEVINFO_LOG("create red_max_lux proc failed.\n");
        return;
    }

    pentry = proc_create_data("cali_coe", S_IRUGO | S_IWUGO, gdata->proc_oppo_als,
        &als_cali_para_fops, UINT2Ptr(CALI_COE));
    if (!pentry) {
        DEVINFO_LOG("create red_max_lux proc failed.\n");
        return;
    }

    pentry = proc_create_data("row_coe", S_IRUGO, gdata->proc_oppo_als,
        &als_cali_para_fops, UINT2Ptr(ROW_COE));
    if (!pentry) {
        DEVINFO_LOG("create red_max_lux proc failed.\n");
        return;
    }

    return;
}

static void sensor_devinfo_work(struct work_struct *dwork)
{
    int ret = 0;
    int count = 10;
    int index = 0;
    int cfg_data[12] = {0};

    do {
        ret = sensor_read_oppo_custom(g_cali_data);
        if (ret) {
            DEVINFO_LOG("try %d\n", count);
            count--;
            msleep(1000);
        }
    } while (ret && count > 0);

    if (ret) {
        DEVINFO_LOG("fail!\n");
        return;
    }

    /*to make sure scp is up*/
    count = 5;
    do {
        ret = init_sensorlist_info();
        if (ret < 0) {
            DEVINFO_LOG("scp access err , try %d\n", count);
            count--;
            msleep(1000);
        }
    } while (ret < 0 && count > 0);

    /* send cfg to scp*/
    //dynamic bias: cfg_data[0] ~ cfg_data[2]
    //static bias: cfg_data[3] ~ cfg_data[5],now cfg static bias
    for (index = 3; index < 6; index++) {
        cfg_data[index] = g_cali_data->acc_data[index-3];
    }
    if (!is_support_mtk_cali) {
        ret = oppo_sensor_cfg_to_hub(ID_ACCELEROMETER, (uint8_t *)cfg_data, sizeof(cfg_data));

        if (ret < 0) {
            DEVINFO_LOG("send acc config fail\n");
        }
    }
    memset(cfg_data, 0, sizeof(int) * 12);

    /*gyro*/
    //dynamic bias: cfg_data[0] ~ cfg_data[2]
    //static bias: cfg_data[3] ~ cfg_data[5],now cfg static bias
    for (index = 3; index < 6; index++) {
        cfg_data[index] = g_cali_data->gyro_data[index-3];
    }
    if (!is_support_mtk_cali) {
        ret = oppo_sensor_cfg_to_hub(ID_GYROSCOPE, (uint8_t *)cfg_data, sizeof(cfg_data));
        if (ret < 0) {
            DEVINFO_LOG("send gyro config fail\n");
        }
    }
    memset(cfg_data, 0, sizeof(int) * 12);

    /*ps*/
    for (index = 0; index < 6; index++) {
        cfg_data[index] = g_cali_data->ps_cali_data[index];
    }
    if (!is_support_mtk_cali) {
        ret = oppo_sensor_cfg_to_hub(ID_PROXIMITY, (uint8_t *)cfg_data, sizeof(cfg_data));
        if (ret < 0) {
            DEVINFO_LOG("send ps config fail\n");
        }
    }
    memset(cfg_data, 0, sizeof(int) * 12);

    /*light*/
    cfg_data[0] = g_cali_data->als_factor;
    if (!is_support_mtk_cali) {
        ret = oppo_sensor_cfg_to_hub(ID_LIGHT, (uint8_t *)cfg_data, sizeof(cfg_data));
        if (ret < 0) {
            DEVINFO_LOG("send light config fail\n");
        }
    }
    memset(cfg_data, 0, sizeof(int) * 12);

    oplus_als_cali_data_init();
    get_accgyro_cali_version();

    DEVINFO_LOG("success \n");
}

static ssize_t parameter_proc_read(struct file *file, char __user *buf,
    size_t count, loff_t *off)
{
    char page[512];
    int len = 0;

    len = sprintf(page, "{%s,\n %s,\n %s}", para_buf[0], para_buf[1], para_buf[2]);

    if ((para_buf[2][0] == '\0') && (para_buf[1][0] == '\0')) {
        page[len - 4] = ' ';
        page[len - 6] = ' ';
        page[len - 7] = ' ';
    } else if (para_buf[2][0] == '\0') {
        page[len - 4] = ' ';
    }

    if (len > *off) {
        len -= *off;
    } else {
        len = 0;
    }

    if (copy_to_user(buf, page, (len < count ?  len : count))) {
        return -EFAULT;
    }
    *off +=  len < count ?  len : count;
    return ( len < count ?  len : count);
}

static const struct file_operations parameter_proc_fops = {
    .read = parameter_proc_read,
    .write = NULL,
};

int get_msensor_parameter(int num)
{
    int offset = 0;
    int len = 0;
    int para_len = 0;
    int index = 0;
    int float_num = 0;
    int para_num = 0;
    uint32_t *data_addr = NULL;
    uint32_t temp_data;
    void *fdt = initial_boot_params;
    char *libname = NULL;
    char *match_project = NULL;
    char temp_buf[128] = {0}, msensor[16], float_buf[10], mag_para[30];
    char project[10] = {0};
    const struct fdt_property *prop = NULL;

    sprintf(msensor, "/odm/msensor_%d", num + 1);
    offset = fdt_path_offset(fdt, msensor);
    if (offset < 0) {
        DEVINFO_LOG("[oem] get %s offset fail", msensor);
        return -1;
    }

    libname = (char *) fdt_getprop(fdt, offset, "libname", &len);
    if (libname == NULL) {
        DEVINFO_LOG("get libname prop fail");
        return -1;
    }
    DEVINFO_LOG(" %s libname is %s\n", msensor, libname);

    prop = fdt_get_property(fdt, offset, "para_num", &len);
    if (prop == NULL) {
        DEVINFO_LOG("para num is null, no need to match project");
        prop = fdt_get_property(fdt, offset, "soft-mag-parameter", &len);
        if (prop == NULL) {
            DEVINFO_LOG("get soft-mag-parameter prop fail");
            return -1;
        }
        para_len = fdt32_to_cpu(prop->len); /*bytes*/
        data_addr = (uint32_t *)prop->data;
    } else {
        data_addr = (uint32_t *)prop->data;
        para_num = fdt32_to_cpu(*data_addr);
        DEVINFO_LOG(" %s match project start, para_num = %d\n", msensor, para_num);

        sprintf(project, "%d", get_project());
        DEVINFO_LOG("project %s\n", project);

        for (index = 0; index < para_num; index++) {
            sprintf(mag_para, "/odm/msensor_%d/mag_para_%d", num + 1, index + 1);
            offset = fdt_path_offset(fdt, mag_para);
            if (offset < 0) {
                DEVINFO_LOG("[oem] get %s offset fail", mag_para);
                return -1;
            }

            match_project = (char *) fdt_getprop(fdt, offset, "match_projects", &len);
            if (match_project == NULL) {
                DEVINFO_LOG("get match_project prop fail");
                return -1;
            }
            DEVINFO_LOG("soft_magpara_%d match project %s\n", index, match_project);

            if (strstr(match_project, project) != NULL) {
                prop = fdt_get_property(fdt, offset, "soft-mag-parameter", &len);
                if (prop == NULL) {
                    DEVINFO_LOG("get soft-mag-parameter prop fail");
                    return -1;
                }
                para_len = fdt32_to_cpu(prop->len); /*bytes*/
                data_addr = (uint32_t *)prop->data;

                DEVINFO_LOG("match project success");
                break;
            }
        }
    }

    if (!strcmp(libname, "mmc") || !strcmp(libname, "mxg")) { /*Memsic parameter need analyze*/
        float_num = fdt32_to_cpu(*data_addr);
        for (index = 0; index < 9; index++) {
            temp_data = fdt32_to_cpu(*(data_addr + (2 * index)));
            sprintf(float_buf, "%c%d.%d%d%d%d", fdt32_to_cpu(*(data_addr + 2 * index + 1)) ? '-' : ' ',
                temp_data / 10000, temp_data % 10000 / 1000, temp_data % 1000 / 100, temp_data % 100 / 10,
                temp_data % 10);
            sprintf(para_buf[num], "%s,%s", temp_buf, float_buf);
            strcpy(temp_buf, para_buf[num]);
        }
        temp_buf[0] = ' ';
        sprintf(para_buf[num], "\"%s\":[%s]", libname, temp_buf);
    } else if (!strcmp(libname, "akm")) {
        for (index = 1; index < para_len / 4; index++) {
            sprintf(para_buf[num], "%s,%d", temp_buf, fdt32_to_cpu(*(data_addr + index)));
            strcpy(temp_buf, para_buf[num]);
        }
        sprintf(para_buf[num], "\"%s\":[%d%s]", libname, fdt32_to_cpu(*data_addr), temp_buf);
    }
    return 0;
}

void  mag_soft_parameter_init()
{
    int ret = -1;
    int index = 0;

    for (index = 0; index < 3; index++) {
        ret = get_msensor_parameter(index);
        if (ret == -1) {
            para_buf[index][0] = '\0';
        } else {
            proc_create("mag_soft_parameter.json", 0666, NULL, &parameter_proc_fops);
        }
    }
}

static int sensor_feature_read_func(struct seq_file *s, void *v)
{
    void *p = s->private;
    int ret = 0;
    int selftest_result = 0;

    DEVINFO_LOG("Ptr2UINT32(p) = %d \n", Ptr2UINT32(p));
    switch (Ptr2UINT32(p)) {
    case IS_SUPPROT_HWCALI:
        if (!strcmp(sensorlist_info[ps].name, "tcs3701")) {
            seq_printf(s, "%d", 1);
        } else {
            seq_printf(s, "%d", 0);
        }
        break;
    case IS_IN_FACTORY_MODE:
        seq_printf(s, "%d", 1);
        break;
    case IS_SUPPORT_NEW_ARCH:
        seq_printf(s, "%d", is_support_new_arch);
        break;
    case IS_SUPPORT_MTK_CALI:
        seq_printf(s, "%d", is_support_mtk_cali);
        break;
    case GYRO_CALI_VERSION:
        seq_printf(s, "%d", gyro_cali_version);
        break;
    case ACC_CALI_RANGE:
        seq_printf(s, "%s", acc_cali_range);
        break;
    case DO_MAG_SELFTEST:
#ifdef CONFIG_OPPO_SENSOR_MTK6885
        ret = oppo_send_selftest_cmd_to_hub(ID_MAGNETIC_FIELD, &selftest_result);
#else
        ret = oppo_send_selftest_cmd_to_hub(ID_MAGNETIC, &selftest_result);
#endif
        if (ret < 0 || selftest_result < 0) {
            seq_printf(s, "%d", -1);
        } else {
            seq_printf(s, "%d", 0);
        }
        break;
    default:
        seq_printf(s, "not support chendai\n");
    }
    return 0;
}

static ssize_t sensor_feature_write(struct file *filp, const char *ubuf, size_t cnt, loff_t *data)
{
    char buf[64];
    void* buf_str = buf;
    long val = 0;
    int ret = 0;
    int result = 0;
    struct seq_file *s = filp->private_data;
    void *p = s->private;
    int node = Ptr2UINT32(p);

    if (cnt >= sizeof(buf)) {
        return -EINVAL;
    } else {
        if (copy_from_user(buf_str, ubuf, cnt)) {
            return -EFAULT;
        }
    }

    ret = kstrtoul(buf, 0, (unsigned long *)&val);
    DEVINFO_LOG("node1 = %d,val = %d\n", node, val);

    switch (node) {
    case IS_SUPPROT_HWCALI:
        break;
    case IS_IN_FACTORY_MODE:
        switch (val) {
        case PS_FACTORY_MODE:
            ret = oppo_send_factory_mode_cmd_to_hub(ID_PROXIMITY, 1, &result);
            break;
        case PS_NORMAL_MODE:
            ret = oppo_send_factory_mode_cmd_to_hub(ID_PROXIMITY, 0, &result);
            break;
        case GSENSOR_FACTORY_MODE:
            ret = oppo_send_factory_mode_cmd_to_hub(ID_ACCELEROMETER, 1, &result);
            break;
        case GSENSOR_NORMAL_MODE:
            ret = oppo_send_factory_mode_cmd_to_hub(ID_ACCELEROMETER, 0, &result);
            break;
        default:
            DEVINFO_LOG("ivalid sensor mode\n");
        }
        if (ret < 0 || result != 1) {
            DEVINFO_LOG("set_factory_mode fail\n");
        }
        break;
    default:
        DEVINFO_LOG("ivalid node type\n");
    }
    return cnt;
}

static int sensor_feature_open(struct inode *inode, struct file *file)
{
    return single_open(file, sensor_feature_read_func, PDE_DATA(inode));
}

static const struct file_operations Sensor_info_fops = {
    .owner = THIS_MODULE,
    .open  = sensor_feature_open,
    .write = sensor_feature_write,
    .read  = seq_read,
    .release = single_release,
};

static int oplus_sensor_feature_init()
{
    struct proc_dir_entry *p_entry;
    static struct proc_dir_entry *oplus_sensor = NULL;

    oplus_sensor = proc_mkdir("oplusSensorFeature", NULL);
    if (!oplus_sensor) {
        DEVINFO_LOG("proc_mkdir err\n ");
        return -ENOMEM;
    }

    p_entry = proc_create_data("is_support_hwcali", S_IRUGO, oplus_sensor, &Sensor_info_fops,
            UINT2Ptr(IS_SUPPROT_HWCALI));
    if (!p_entry) {
        DEVINFO_LOG("is_support_hwcali err\n ");
        return -ENOMEM;
    }

    p_entry = proc_create_data("is_support_new_arch", S_IRUGO, oplus_sensor, &Sensor_info_fops,
            UINT2Ptr(IS_SUPPORT_NEW_ARCH));
    if (!p_entry) {
        DEVINFO_LOG("is_support_new_arch err\n ");
        return -ENOMEM;
    }

    p_entry = proc_create_data("is_support_mtk_cali", S_IRUGO, oplus_sensor, &Sensor_info_fops,
            UINT2Ptr(IS_SUPPORT_MTK_CALI));
    if (!p_entry) {
        DEVINFO_LOG("is_support_mtk_cali err\n ");
        return -ENOMEM;
    }

    p_entry = proc_create_data("is_in_factory_mode", S_IRUGO | S_IWUGO, oplus_sensor, &Sensor_info_fops,
            UINT2Ptr(IS_IN_FACTORY_MODE));
    if (!p_entry) {
        DEVINFO_LOG("is_in_factory_mode err\n ");
        return -ENOMEM;
    }

    p_entry = proc_create_data("gyro_cali_version", S_IRUGO, oplus_sensor, &Sensor_info_fops,
            UINT2Ptr(GYRO_CALI_VERSION));
    if (!p_entry) {
        DEVINFO_LOG("gyro_cali_version err\n ");
        return -ENOMEM;
    }

    p_entry = proc_create_data("acc_cali_range", S_IRUGO, oplus_sensor, &Sensor_info_fops,
            UINT2Ptr(ACC_CALI_RANGE));
    if (!p_entry) {
        DEVINFO_LOG("acc_cali_range err\n ");
        return -ENOMEM;
    }

    p_entry = proc_create_data("do_mag_selftest", S_IRUGO, oplus_sensor, &Sensor_info_fops,
            UINT2Ptr(DO_MAG_SELFTEST));
    if (!p_entry) {
        DEVINFO_LOG("do_mag_selftest err\n ");
        return -ENOMEM;
    }
    return 0;
}
#ifdef CONFIG_OPPO_SENSOR_MTK6885
void __attribute__((weak)) scp_A_register_notify(struct notifier_block *nb)
{

}

void __attribute__((weak)) scp_A_unregister_notify(struct notifier_block *nb)
{

}

static int scp_ready_event(struct notifier_block *this,
    unsigned long event, void *ptr)
{

    if (event == SCP_EVENT_READY) {
        DEVINFO_LOG(" receiver SCP_EVENT_READY event send cfg data\n ");
        schedule_delayed_work(&parameter_work, msecs_to_jiffies(100));
    }

    return NOTIFY_DONE;
}

static struct notifier_block scp_ready_notifier = {
    .notifier_call = scp_ready_event,
};
#endif
static int scp_utc_sync_pm_event(struct notifier_block *notifier,
    unsigned long pm_event, void *unused)
{
    switch (pm_event) {
    case PM_SUSPEND_PREPARE:
    case PM_HIBERNATION_PREPARE:
        atomic_set(&utc_suspend, 1);
        break;
    case PM_POST_RESTORE:
    case PM_POST_SUSPEND:
    case PM_POST_HIBERNATION:
        atomic_set(&utc_suspend, 0);
        schedule_delayed_work(&utc_work, msecs_to_jiffies(100));
        break;
    default:
        break;
    }

    return NOTIFY_DONE;
}

static struct notifier_block scp_utc_sync_notifier = {
    .notifier_call = scp_utc_sync_pm_event,
    .priority = 0,
};
static int __init sensor_devinfo_init(void)
{
    int ret = 0;

    mag_soft_parameter_init();
    get_new_arch_info();
    get_mtk_cali_info();

    ret = oplus_sensor_feature_init();
    if (ret != 0) {
        DEVINFO_LOG("oplus_sensor_feature_init err\n ");
        return -ENOMEM;
    }

    g_cali_data = kzalloc(sizeof(struct cali_data), GFP_KERNEL);
    if (!g_cali_data) {
        DEVINFO_LOG("kzalloc err\n ");
        return -ENOMEM;
    }

    atomic_set(&utc_suspend, 0);
    INIT_DELAYED_WORK(&utc_work, sync_utc2scp_work);
    schedule_delayed_work(&utc_work, msecs_to_jiffies(2000));
    register_pm_notifier(&scp_utc_sync_notifier);

    /*init parameter*/
    INIT_DELAYED_WORK(&parameter_work, sensor_devinfo_work);
    schedule_delayed_work(&parameter_work, msecs_to_jiffies(100));
    #ifdef CONFIG_OPPO_SENSOR_MTK6885
    scp_A_register_notify(&scp_ready_notifier);
    #endif

    return 0;
}

late_initcall(sensor_devinfo_init);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("sensor devinfo");
MODULE_AUTHOR("Murhpy");
