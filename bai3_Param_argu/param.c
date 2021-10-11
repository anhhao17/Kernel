#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include<linux/moduleparam.h>


//module_param(name,type,permission) 
//module_param_array()
//module_param_cb() notification when change value and need to register to file operating structure


//all macros should be placed outside function

//this will create in a sysfs entry (/sys/module/argument/parameters/..)


/*
permissions
S_IRUSR  can read
S_IWUSR  can write
S_IXUSR  can execute(thuc thi)
*/
int value,arr_value[4];
char *name;
int cb_value=0;
module_param(value, int, S_IRUSR|S_IWUSR);                      //integer value
module_param(name, charp, S_IRUSR|S_IWUSR);                     //String
module_param_array(arr_value, int, NULL, S_IRUSR|S_IWUSR); 

int notify_param(const char *val, const struct kernel_param *kp)
{
        int res = param_set_int(val, kp); // Use helper for write variable
        if(res==0) {
                pr_info("Call back function called...\n");
                pr_info("New value of cb_valueETX = %d\n", cb_value);
                return 0;
        }
        return -1;
}
 
const struct kernel_param_ops my_param_ops = 
{
        .set = notify_param, // Use our setter ...
        .get = param_get_int, // .. and standard getter
};
 
module_param_cb(cb_value, &my_param_ops, &cb_value, S_IRUGO|S_IWUSR );


static int __init hello_world_init(void)
{
        int i;
        pr_info("ValueETX = %d  \n", value);
        pr_info("cb_valueETX = %d  \n", cb_value);
        pr_info("NameETX = %s \n", name);
        for (i = 0; i < (sizeof arr_value / sizeof (int)); i++) {
                pr_info("Arr_value[%d] = %d\n", i, arr_value[i]);
        }
        pr_info("Kernel Module Inserted Successfully...\n");
    return 0;
}

/*
** Module Exit function
*/
static void __exit hello_world_exit(void)
{
    pr_info("Kernel Module Removed Successfully...\n");
}
 
module_init(hello_world_init);
module_exit(hello_world_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EmbeTronicX <embetronicx@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("1.0");