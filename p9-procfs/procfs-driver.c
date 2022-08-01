#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include <linux/procfs_fs.h>

#define LINUX_KERNEL_VERSION 510

#define WR_VALUE _IOW('a', 'a', int32_t*)
#define RD_VALUE _IOR('a', 'b', int32_t*)

int32_t value = 0;
char etx_array[20] = "try_proc_array\n";
static int len = 1;

dev_t dev = 0;
static struct class *dev_class;
static struct cdev * etx_cdev;
static struct proc_dir_entry *parent;

// Function prototypes
static int __init driver_init(void);
static void __exit driver_exit(void);

// Driver functions
static int etx_open(struct inode *inode, struct file *file);
static int etx_release(struct inode *inode, struct file *file);
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off);
static ssize_t etx_write(struct file *filp, const char *buf, size_t len, loff_t *off);
static long etx_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

// Procfs functions
static int open_proc(struct inode *inode, struct file *file);
static int release_proc(struct inode *inode, struct file *file);
static ssize_t read_proc(struct file *filp, char __user *buf, size_t len, loff_t *offset);
static ssize_t write_proc(struct file *filp, const char *buf, size_t len, loff_t *off);

// File operation structure
static struct file_operations fops =
{
    .owner = THIS_MODULE,
    .read = etx_read,
    .write = etx_write,
    .open = etx_open,
    .unlocked_ioctl = etx_ioctl,
    .release = etx_release,
};

#if (LINUX_KERNEL_VERSION > 505)

static struct proc_ops proc_fops =
{
    .proc_open = open_proc,
    .proc_read = read_proc,
    .proc_write = write_proc,
    .proc_release = release_proc,
};

#else 

static struct file_operations proc_fops =
{
    .open = open_proc,
    .read = read_proc,
    .write = write_proc,
    .release = release_proc,
};

#endif //LINUX_KERNEL_VERSION > 505

static int open_proc(struct inode *inode, struct file *file)
{
    pr_info("proc file opened\n");
    return 0;
}

static int release_proc(struct inode *inode, struct file *file)
{
    pr_info("proc file released\n");
    return 0;
}

static ssize_t read_proc(struct file *filp, char __user *buf, size_t leng, loff_t offset)
{
    pr_info("proc file read\n");
    if (len)
    {
        len = 0;
    }
    else
    {
        len = 1;
        return 0;
    }

    if (copy_to_user(buf, etx_array, 20))
    {
        pr_err("Data send error\n");
    }
    return len;
}

static ssize_t write_proc(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    pr_info("proc file wrote\n");

    if (copy_from_user(etx_array, buf, len))
    {
        pr_err("Data write error\n");
    }
    return len;
}

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

static int __init driver_init(void)
{
    if ((alloc_chrdev_region(&dev, 0, 1, "quannh41_device")) < 0)
    {
        pr_info("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(dev), MINOR(dev));

    cdev_init(&etx_cdev, &fops);

    if ((cdev_add(&etx_cdev, dev, 1)) < 0)
    {
        pr_info("Cannot add the device to the system\n");
        goto r_class;
    }

    if ((dev_class = class_create(THIS_MODULE, "quannh41_class")) == NULL)
    {
        pr_info("Cannot create the struct class\n");
        goto r_class;
    }

    if ((device_create(dev_class, NULL, dev, NULL, "quannh41_device")) == NULL)
    {
        pr_info("Cannot create the device\n");
        goto r_device;
    }

    parent = proc_mkdir("etx", NULL);

    if( parent == NULL)
    {
        pr_info("Error creating proc entry\n");
        goto r_device;
    }

    proc_create("etx_proc", 0666, parent, &proc_fops);

    pr_info("Device driver inserted done\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

static void __exit driver_exit(void)
{
    // remove single proc entry
    // remove_proc_entry("etx/etx_proc", parent);

    // remove complete /proc/etx
    proc_remove(parent);

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
MODULE_DESCRIPTION("Learn procfs");