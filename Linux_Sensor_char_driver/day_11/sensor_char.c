#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/spinlock.h>
#include <linux/timer.h>

#define DEVICE_NAME "sensor_char"
#define CLASS_NAME  "sensor_class"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KAMAL");
MODULE_DESCRIPTION("DAY-11: Char Device + Blocking Read + Poll");

// Shared data
static int sensor_value = 0;
static int data_ready = 0;

// Sync
static spinlock_t sensor_lock;
static DECLARE_WAIT_QUEUE_HEAD(sensor_wq);

// Device
static dev_t dev;
static struct cdev sensor_cdev;
static struct class *sensor_class;

// Timer
static struct timer_list sensor_timer;

// Timer handler
static void sensor_timer_handler(struct timer_list *t)
{
    unsigned long flags;

    spin_lock_irqsave(&sensor_lock, flags);
    sensor_value++;
    data_ready = 1;
    spin_unlock_irqrestore(&sensor_lock, flags);

    wake_up_interruptible(&sensor_wq);

    mod_timer(&sensor_timer, jiffies + msecs_to_jiffies(2000));
}

// File ops
static ssize_t sensor_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char buffer[32];
    int len;
    unsigned long flags;

    if(wait_event_interruptible(sensor_wq, data_ready))
        return -ERESTARTSYS;

    spin_lock_irqsave(&sensor_lock, flags);
    len = snprintf(buffer, sizeof(buffer), "%d\n", sensor_value);
    data_ready = 0;
    spin_unlock_irqrestore(&sensor_lock, flags);

    return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static unsigned int sensor_poll(struct file *file, poll_table *wait)
{
    unsigned int mask = 0;
    unsigned long flags;

    poll_wait(file, &sensor_wq, wait);

    spin_lock_irqsave(&sensor_lock, flags);
    if(data_ready) mask |= POLLIN | POLLRDNORM;
    spin_unlock_irqrestore(&sensor_lock, flags);

    return mask;
}

static int sensor_open(struct inode *inode, struct file *file) { return 0; }
static int sensor_release(struct inode *inode, struct file *file) { return 0; }

static struct file_operations sensor_fops = {
    .owner = THIS_MODULE,
    .read  = sensor_read,
    .poll  = sensor_poll,
    .open  = sensor_open,
    .release = sensor_release,
};

static int __init sensor_init(void)
{
    alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME);
    cdev_init(&sensor_cdev, &sensor_fops);
    cdev_add(&sensor_cdev, dev, 1);

    sensor_class = class_create(CLASS_NAME);
    device_create(sensor_class, NULL, dev, NULL, DEVICE_NAME);

    spin_lock_init(&sensor_lock);

    timer_setup(&sensor_timer, sensor_timer_handler, 0);
    mod_timer(&sensor_timer, jiffies + msecs_to_jiffies(2000));

    pr_info("DAY-11 MODULE LOADED\n");
    return 0;
}

static void __exit sensor_exit(void)
{
    del_timer_sync(&sensor_timer);

    device_destroy(sensor_class, dev);
    class_destroy(sensor_class);
    cdev_del(&sensor_cdev);
    unregister_chrdev_region(dev, 1);

    pr_info("MODULE UNLOADED\n");
}

module_init(sensor_init);
module_exit(sensor_exit);
