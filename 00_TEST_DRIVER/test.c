#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>

static int __init IN(void)
{ 
	printk(KERN_INFO "THIS IS TEST MODULE!!!");
	return 0;
}

static void __exit OUT(void)
{
	printk(KERN_INFO "Exiting FROM TEST MODULE!!!");
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("TEST MODULE");
