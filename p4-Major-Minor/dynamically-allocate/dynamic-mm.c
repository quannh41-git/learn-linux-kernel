#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>

dev_t dev = 0;

static int __init driver_init(void)
{
    if ((alloc_chrdev_region(&dev, 0, 1, "quannh41_device")) < 0)
    {
        pr_info("Cannot allocate major for device\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));
    pr_info("Kernel module inserted successfully\n");

    return 0;
}

static void __exit driver_exit(void)
{
    unregister_chrdev_region(dev, 1);
    pr_info("Kernel module removed successfully\n");
}

module_init(driver_init);
module_init(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("quannh41");
MODULE_DESCRIPTION("Linux driver - dynamically allocate Major and Minor number");