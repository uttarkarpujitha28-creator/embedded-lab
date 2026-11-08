#include<linux/kernel.h>
#include<linux/module.h>

static int __init my_init(void)
{
    pr_info("inside the hello code of the module\n");
    return 0;
}

static void __exit my_exit(void)
{
    pr_info("at the exit\n");

}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("PUJITHA");
MODULE_VERSION("V0.1");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("HELLO CODE");
    


