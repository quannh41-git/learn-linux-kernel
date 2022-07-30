#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>

dev_t dev = MKDEV(235, 0);

static int __init driver_init(void)
{
    register_chrdev_region(dev, 1, "quannh41_device");
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
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("quannh41");
MODULE_DESCRIPTION("Linux driver - statically allocating the Major and Minor number");