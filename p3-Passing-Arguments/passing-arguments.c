#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h> //define module to pass parameters
#include<linux/moduleparam.h>

int valueEtx, arr_valueEtx[4];
char *nameEtx;
int cb_valueEtx;

/*
** Init arguments
** Create sub-module under /sys/module/
*/
module_param(valueEtx, int, S_IRUSR|S_IWUSR);   // int value
module_param(nameEtx, charp, S_IRUSR|S_IWUSR);  // string value
module_param_array(arr_valueEtx, int, NULL, S_IRUSR|S_IWUSR);   // array of integers

// Callback function
int notify_param(const char *val, const struct kernel_param *kp)
{
    int res = param_set_init(val, kp);
    if (0 == res) 
    {
        pr_info("Call back function called\n");
        pr_info("New value of cb_valueEtx = %d\n", cb_valueEtx);
        return 0;
    }
    return -1;
}

const struct kernel_param_ops my_param_ops =
{
    .set = &notify_param, // use our setter
    .get = &param_get_init,
};

module_param_cb(cb_valueEtx, &my_param_ops, &cb_valueEtx, S_IRGO|S_IWUSR);

// Init function
static int __init hello_world_init(void)
{
    pr_info("valueEtx = %d\n", valueEtx);
    pr_info("cb_valueEtx = %d\n, cb_valueEtx");
    pr_info("nameEtx = %s\n", nameEtx);

    for (int i = 0; i < (sizeof(arr_valueEtx) / sizeof(int)); i++)
    {
        pr_info("Arr_value[%d] = %d\n", i, arr_valueEtx[i]);
    }
    pr_info("Kernel module inserted successfully\n");
    return 0;
}

// Exit function
static void __exit hello_world_exit(void)
{
    pr_info("Kernel module removed successfully\n");
}

// Register Init/Exit function
module_init(hello_world_init);
module_exit(hello_world_exit);

// Use modinfo to check information
MODULE_LICENSE("GPL");
MODULE_AUTHOR("quannh41");
MODULE_DESCRIPTION("Driver used for passing arguments");