#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/uaccess.h>

#define DRIVER_NAME   "sensor"
#define CLASS_NAME    "sensor_class"
#define SENSOR_COUNT  2

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KAMAL");
MODULE_DESCRIPTION("Upgrade-1 Day-3: Per-device sysfs control");

/* ================= Per-device structure ================= */
struct sensor_dev {
    int id;
    int sensor_value;
    int interval_ms;

    char *buffer;
    struct timer_list timer;

    dev_t devt;
    struct cdev cdev;
    struct device *device;
};

/* ================= Globals ================= */
static struct class *sensor_class;
static struct sensor_dev sensors[SENSOR_COUNT];

/* ================= Timer ================= */
static void sensor_timer_handler(struct timer_list *t)
{
    struct sensor_dev *dev =
        container_of(t, struct sensor_dev, timer);

    dev->sensor_value++;

    snprintf(dev->buffer, PAGE_SIZE,
             "Sensor%d Value = %d\n",
             dev->id, dev->sensor_value);

    mod_timer(&dev->timer,
              jiffies + msecs_to_jiffies(dev->interval_ms));
}

/* ================= Sysfs handlers ================= */
static ssize_t interval_show(struct device *dev,
                             struct device_attribute *attr,
                             char *buf)
{
    struct sensor_dev *sdev = dev_get_drvdata(dev);
    return sprintf(buf, "%d\n", sdev->interval_ms);
}

static ssize_t interval_store(struct device *dev,
                              struct device_attribute *attr,
                              const char *buf,
                              size_t count)
{
    struct sensor_dev *sdev = dev_get_drvdata(dev);
    int val;

    if (kstrtoint(buf, 10, &val))
        return -EINVAL;

    if (val < 100)
        return -EINVAL;

    sdev->interval_ms = val;

    mod_timer(&sdev->timer,
              jiffies + msecs_to_jiffies(sdev->interval_ms));

    return count;
}

static DEVICE_ATTR_RW(interval);

/* ================= File ops ================= */
static int sensor_open(struct inode *inode, struct file *file)
{
    struct sensor_dev *dev =
        container_of(inode->i_cdev,
                     struct sensor_dev,
                     cdev);
    file->private_data = dev;
    return 0;
}

static ssize_t sensor_read(struct file *file,
                           char __user *buf,
                           size_t count,
                           loff_t *ppos)
{
    struct sensor_dev *dev = file->private_data;

    return simple_read_from_buffer(buf, count,
                                   ppos,
                                   dev->buffer,
                                   strlen(dev->buffer));
}

static struct file_operations sensor_fops = {
    .owner  = THIS_MODULE,
    .open   = sensor_open,
    .read   = sensor_read,
    .llseek = noop_llseek,
};

/* ================= Init ================= */
static int __init sensor_init(void)
{
    int i;
    dev_t base_dev;

    alloc_chrdev_region(&base_dev, 0,
                        SENSOR_COUNT, DRIVER_NAME);

    sensor_class = class_create(CLASS_NAME);

    for (i = 0; i < SENSOR_COUNT; i++) {

        sensors[i].id = i;
        sensors[i].sensor_value = 0;
        sensors[i].interval_ms = 2000;
        sensors[i].devt = base_dev + i;

        sensors[i].buffer = kmalloc(PAGE_SIZE, GFP_KERNEL);
        snprintf(sensors[i].buffer, PAGE_SIZE,
                 "Sensor%d Value = 0\n", i);

        cdev_init(&sensors[i].cdev, &sensor_fops);
        cdev_add(&sensors[i].cdev, sensors[i].devt, 1);

        sensors[i].device =
            device_create(sensor_class, NULL,
                          sensors[i].devt,
                          &sensors[i],
                          "sensor%d", i);

        device_create_file(sensors[i].device,
                           &dev_attr_interval);

        timer_setup(&sensors[i].timer,
                    sensor_timer_handler, 0);

        mod_timer(&sensors[i].timer,
                  jiffies + msecs_to_jiffies(
                      sensors[i].interval_ms));
    }

    pr_info("Upgrade-1 Day-3 driver loaded\n");
    return 0;
}

/* ================= Exit ================= */
static void __exit sensor_exit(void)
{
    int i;

    for (i = 0; i < SENSOR_COUNT; i++) {
        del_timer_sync(&sensors[i].timer);
        device_remove_file(sensors[i].device,
                           &dev_attr_interval);
        device_destroy(sensor_class, sensors[i].devt);
        kfree(sensors[i].buffer);
        cdev_del(&sensors[i].cdev);
    }

    class_destroy(sensor_class);
    unregister_chrdev_region(sensors[0].devt,
                             SENSOR_COUNT);

    pr_info("Driver unloaded\n");
}

module_init(sensor_init);
module_exit(sensor_exit);
