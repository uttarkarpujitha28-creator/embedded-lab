#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>


int value,arr_val[4];
char *name;
int cb_val=0;

module_param(value,int,S_IWUSR|S_IRUSR);
module_param(name,charp,S_IWUSR|S_IRUSR);
module_param_array(arr_val,int,NULL,S_IWUSR|S_IRUSR);

int notify_cb(const char *val,const struct kernel_param *kp)
{
    int res= param_set_int(val,kp);
    if(res==0)
    {
        pr_info("callback notified\n");
        pr_info("value of the cb update is cb =%d\n",cb_val);
        return 0;
    }
    return -1;
}

const struct kernel_param_ops my_kp_ops = 
{

    .get = &param_get_int,
    .set = &notify_cb,
};

module_param_cb(cb_val,&my_kp_ops,&cb_val,S_IWUSR|S_IRUSR);

static int __init my_init(void)
{
    int i;
    pr_info("value = %d\n",value);
    pr_info("name = %s\n",name);
    pr_info("cb_val = %d\n",cb_val);
    for(i=0;i<((sizeof(arr_val))/sizeof(int));i++)
    {
        pr_info("arr_val=%d\n",arr_val[i]); 
    }
    return 0;
}

static void __exit my_exit(void)
{
    pr_info("at the args exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_AUTHOR("PUJITHA");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("USER ARGS CODE");
MODULE_VERSION("V0.1");