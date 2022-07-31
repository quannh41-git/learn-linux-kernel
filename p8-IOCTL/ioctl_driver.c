#include <linux/kenel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define WR_VALUE _IOW('a', 'a', int32_t*)
#define RD_VALUE _IOR('a', 'b', int32_t*)

int32_t value = 0;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;

// Function prototype
static int __init driver_init(void);
static void __exit driver_exit(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff *off);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// File operation structure
static struct file_operation fops =
{
    .owner = THIS_MODULE,
    .read = etx_read,
    .write = etx_write,
    .open = etx_open,
    .release = etx_release,
    .unlocked_ioctl = etx_ioctl,
};

static int etx_open(struct inode *inode, struct file *file)
{
    pr_info("Driver open function is called\n");
    return 0;
}

static int etx_release(struct inode *inode, struct file *file)
{
    pr_info("Driver release function is called\n");
    return 0;
}

static ssize_t etx_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    pr_info("Driver read function\n");
    return 0;
}

static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("Driver write function\n");
    return len;
}

// This function will be called when we write IOCTL on the device file
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case WR_VALUE:
            if (copy_from_user(&value, (int32_t*)arg, sizeof(value)))
            {
                pr_err("Data write err\n");
            }
            pr_info("Value = %d\n", value);
            break;
        case RD_VALUE:
            if (copy_to_user((int32_t*)arg, &value, sizeof(value)))
            {
                pr_err("Data read err\n");
            }
            break;
        default: 
            pr_info("Default\n");
            break;
    }
    return 0;
}

// Init function
static int __init driver_init(void)
{
    if (alloc_chrdev_region(&dev, 0, 1, "quannh41_device") < 0)
    {
        pr_err("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    // Create cdev structure
    cdev_init(&etx_cdev, &fops);

    // Add character device to the system
    if ((cdev_add(&etx_cdev, dev, 1)) < 0)
    {
        pr_err("Cannot add the device to the system\n");
        goto r_class;
    }

    // Create struct class
    if ((dev_class = class_create(THIS_MODULE, "quannh41_class")) == NULL)
    {
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }

    // Create device
    if ((device_create(dev_class, NULL, dev, NULL, "quannh41_device")) == NULL)
    {
        pr_err("Cannot create the device\n");
        goto r_device;
    }
    pr_info("Device driver inserted done\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

// Exit function
static void __exit driver_exit(void)
{
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device driver removed done\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("quannh41");
MODULE_DESCRIPTION("Learn IOCTL");