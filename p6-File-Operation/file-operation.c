#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>

dev_t dev = 0; //used for major and minor number
static struct class *dev_class;
static struct cdev etx_cdev;

// Function prototype
static int __init driver_init(void);
static void __exit driver_exit(void);
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);

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

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .read = etx_read,
    .write = etx_write,
    .open = etx_open,
    .release = etx_release,
};

static int __init driver_init(void)
{
    // Allocate major number
    if ((alloc_chrdev_region(&dev, 0, 1, "etx_Dev")) < 0)
    {
        pr_err("Cannot allocate major number\n")
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    // Create cdev structure
    cdev_init(&etx_cdev, &fops);

    // Add character device to the system
    if ((cdev_add(&etx_cdev, dev, 1)) < 0)
    {
        pr_err("Cannot add device to the system\n");
        goto r_class;
    }

    // Create struct class
    if ((dev_class = class_create(THIS_MODULE, "etx_class")) == NULL)
    {
        pr_err("Cannot create the struct class\n");
        goto r_class;
    }

    // Create device
    if ((device_create(dev_class, NULL, dev, NULL, "etx_device")) == NULL)
    {
        pr_err("Cannot create the device\n");
        goto r_device;
    }

    pr_info("Device Driver Insert\n");
    return 0;
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit driver_exit(void)
{
    device_destroy(dev_class, class, dev);
    class_destroy(dev_class);
    cdev_del(&wrz_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device driver removed\n");
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHUR("quannh41");
MODULE_DESCRIPTION("Linux device driver (File operations");