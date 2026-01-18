#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "sensor_char"
#define CLASS_NAME  "sensor_class"
#define IOCTL_SET_VALUE _IOW('s', 1, int)
#define IOCTL_GET_VALUE _IOR('s', 2, int)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KAMAL");
MODULE_DESCRIPTION("DAY-12: Char Device with IOCTL");

static int sensor_value = 0;

static dev_t dev;
static struct cdev sensor_cdev;
static struct class *sensor_class;

static int sensor_open(struct inode *inode, struct file *file){ return 0; }
static int sensor_release(struct inode *inode, struct file *file){ return 0; }

static long sensor_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case IOCTL_SET_VALUE:
            if(copy_from_user(&sensor_value, (int __user *)arg, sizeof(sensor_value)))
                return -EFAULT;
            pr_info("IOCTL: Value set to %d\n", sensor_value);
            break;
        case IOCTL_GET_VALUE:
            if(copy_to_user((int __user *)arg, &sensor_value, sizeof(sensor_value)))
                return -EFAULT;
            pr_info("IOCTL: Value returned %d\n", sensor_value);
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

static struct file_operations sensor_fops = {
    .owner = THIS_MODULE,
    .open  = sensor_open,
    .release = sensor_release,
    .unlocked_ioctl = sensor_ioctl,
};

static int __init sensor_init(void)
{
    alloc_chrdev_region(&dev,0,1,DEVICE_NAME);
    cdev_init(&sensor_cdev,&sensor_fops);
    cdev_add(&sensor_cdev,dev,1);

    sensor_class = class_create(CLASS_NAME);
    device_create(sensor_class,NULL,dev,NULL,DEVICE_NAME);

    pr_info("DAY-12 MODULE LOADED\n");
    return 0;
}

static void __exit sensor_exit(void)
{
    device_destroy(sensor_class,dev);
    class_destroy(sensor_class);
    cdev_del(&sensor_cdev);
    unregister_chrdev_region(dev,1);
    pr_info("MODULE UNLOADED\n");
}

module_init(sensor_init);
module_exit(sensor_exit);
