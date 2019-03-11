#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>	
#include <linux/kdev_t.h>	//For Major and minor numbers
#include <linux/fs.h>		//Function alloc_chardev_register()

static dev_t first;

static int __init IN(void) {
	printk(KERN_INFO " MODULE!!!");
	if(alloc_chrdev_region( &first, 0, 2, "SHUBH") < 0)
		return -1;
	
	printk(KERN_INFO "Major no.: %d\tMinor no.: %d\n", MAJOR(first), MINOR(first));
	return 0;
}

static void __exit OUT(void) {
	unregister_chrdev_region( first, 2);
	printk(KERN_INFO "Exiting FROM TEST MODULE!!!");
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("TEST MODULE");
