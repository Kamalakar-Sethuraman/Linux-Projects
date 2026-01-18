#include <linux/module.h>
#include <linux/init.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/timer.h>
#include <linux/spinlock.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("KAMAL");
MODULE_DESCRIPTION("DAY-15: SysFS Interface Driver");

static int sensor_value = 0;
static int interval = 1000; // ms

static struct kobject *sensor_kobj;
static struct timer_list sensor_timer;
static spinlock_t sensor_lock;

static void sensor_timer_handler(struct timer_list *t)
{
    unsigned long flags;
    spin_lock_irqsave(&sensor_lock,flags);
    sensor_value++;
    spin_unlock_irqrestore(&sensor_lock,flags);

    mod_timer(&sensor_timer,jiffies + msecs_to_jiffies(interval));
}

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf,"%d\n",sensor_value);
}

static ssize_t interval_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count)
{
    sscanf(buf,"%d",&interval);
    return count;
}

static struct kobj_attribute value_attr = __ATTR_RO(value);
static struct kobj_attribute interval_attr = __ATTR_WO(interval);

static struct attribute *sensor_attrs[] = { &value_attr.attr, &interval_attr.attr, NULL };
static struct attribute_group sensor_attr_group = { .attrs = sensor_attrs };

static int __init sensor_init(void)
{
    spin_lock_init(&sensor_lock);
    sensor_kobj = kobject_create_and_add("sensor_class",kernel_kobj);
    sysfs_create_group(sensor_kobj,&sensor_attr_group);

    timer_setup(&sensor_timer,sensor_timer_handler,0);
    mod_timer(&sensor_timer,jiffies + msecs_to_jiffies(interval));

    pr_info("DAY-15 MODULE LOADED\n");
    return 0;
}

static void __exit sensor_exit(void)
{
    del_timer_sync(&sensor_timer);
    kobject_put(sensor_kobj);
    pr_info("MODULE UNLOADED\n");
}

module_init(sensor_init);
module_exit(sensor_exit);
