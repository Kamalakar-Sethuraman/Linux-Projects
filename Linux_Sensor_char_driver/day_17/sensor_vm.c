#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/timer.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KAMAL");
MODULE_DESCRIPTION("DAY-17: Virtual Memory mmap Driver");

#define DEVICE_NAME "sensor_char"
#define BUF_SIZE PAGE_SIZE

static int sensor_value = 0;
static char *sensor_buf;

static dev_t dev;
static struct cdev sensor_cdev;
static struct class *sensor_class;
static struct timer_list sensor_timer;

static void sensor_timer_handler(struct timer_list *t)
{
    sensor_value++;
    snprintf(sensor_buf,BUF_SIZE,"Sensor Value = %d\n",sensor_value);
    mod_timer(&sensor_timer,jiffies+msecs_to_jiffies(2000));
}

static int sensor_mmap(struct file *filep,struct vm_area_struct *vma)
{
    unsigned long size = vma->vm_end - vma->vm_start;
    unsigned long pfn = virt_to_phys(sensor_buf)>>PAGE_SHIFT;

    if(size>BUF_SIZE) return -EINVAL;

    return remap_pfn_range(vma,vma->vm_start,pfn,size,vma->vm_page_prot);
}

static loff_t sensor_llseek(struct file *file,loff_t off,int whence)
{
    return -ESPIPE;
}

static struct file_operations sensor_fops={
    .owner=THIS_MODULE,
    .mmap=sensor_mmap,
    .llseek=sensor_llseek,
};

static int __init sensor_init(void)
{
    alloc_chrdev_region(&dev,0,1,DEVICE_NAME);
    cdev_init(&sensor_cdev,&sensor_fops);
    cdev_add(&sensor_cdev,dev,1);

    sensor_class = class_create(CLASS_NAME);
    device_create(sensor_class,NULL,dev,NULL,DEVICE_NAME);

    sensor_buf = kmalloc(BUF_SIZE,GFP_KERNEL);
    snprintf(sensor_buf,BUF_SIZE,"Sensor Value = 0\n");

    timer_setup(&sensor_timer,sensor_timer_handler,0);
    mod_timer(&sensor_timer,jiffies+msecs_to_jiffies(2000));

    pr_info("DAY-17 MODULE LOADED\n");
    return 0;
}

static void __exit sensor_exit(void)
{
    del_timer_sync(&sensor_timer);
    kfree(sensor_buf);

    device_destroy(sensor_class,dev);
    class_destroy(sensor_class);
    cdev_del(&sensor_cdev);
    unregister_chrdev_region(dev,1);

    pr_info("MODULE UNLOADED\n");
}

module_init(sensor_init);
module_exit(sensor_exit);
