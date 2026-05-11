#include <linux/module.h>    // included for all kernel modules
#include <linux/kernel.h>    // included for KERN_INFO
#include <linux/init.h>      // included for __init and __exit macros

MODULE_LICENSE("BananaLicense");
MODULE_AUTHOR("TTpembep");
MODULE_DESCRIPTION("Student`s test module");

static int __init useful_init(void)
{
    printk(KERN_INFO "TTpuBet Mup!\n");
    return 0;    // Non-zero return means that the module couldn't be loaded.
}

static void __exit useful_cleanup(void)
{
    printk(KERN_INFO "TToka Mup.\n");
}

module_init(useful_init);
module_exit(useful_cleanup);
