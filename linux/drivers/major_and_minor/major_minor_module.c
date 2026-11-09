#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/init.h>

dev_t dev=MKDEV(256,0);

static int __init my_init(void)
{
    register_chrdev_region(dev,1,"sample_num");
    pr_info("major num =%d minor num =%d\n",MAJOR(dev),MINOR(dev));
    pr_info("module inserted\n");
    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev_region(dev,1);
    pr_info("unregistered the dev\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("PUJITHA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MAJOR AND MINOR NUMBER");
MODULE_VERSION("V0.1");