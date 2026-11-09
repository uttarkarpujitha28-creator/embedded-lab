#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>

dev_t dev = 0;

static int __init my_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev, 1, 1, "sys_dev");

    if (ret < 0)
    {
        pr_err("Cannot allocate character device region\n");
        return ret;
    }

    pr_info("major = %d minor = %d\n", MAJOR(dev), MINOR(dev));
    pr_info("module inserted\n");

    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev_region(dev, 1);
    pr_info("Removed the char device region\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("PUJITHA");
MODULE_DESCRIPTION("DYNAMIC MAJOR MINOR NUMBER ALLOCATION");
MODULE_LICENSE("GPL");
MODULE_VERSION("V0.1");