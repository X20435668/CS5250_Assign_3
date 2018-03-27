#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xu Yudong");

static char*who="XuYudong";
module_param(who,charp,0000);
MODULE_PARM_DESC(who,"who should I say hello to?");
	
static int hello_init(void)
{
	printk(KERN_ALERT "Hello, world: %s\n", who);
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
