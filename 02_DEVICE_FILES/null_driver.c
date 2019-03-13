#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>	
#include <linux/kdev_t.h>	//For Major and minor numbers
#include <linux/fs.h>		//Function alloc_chardev_register()
#include <linux/device.h>	//For class create, device create & device destroy.
#include <linux/cdev.h>		//For cdev_init & cdev_add.

static dev_t first;		    //One vaiable containing Majaor & Minor Nos.
static struct cdev c_dev;	//Global variable for character device structure 
static struct class *cl;	//Global variable for device class 

static int my_null_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "MY_NULL DEVICE_OPEN()\n");
	return 0;
}
static int my_null_close(struct inode *i, struct file *f) {
	printk(KERN_INFO "MY_NULL DEVICE_CLOSE()\n");
	return 0;
}
static ssize_t my_null_read(struct file *f, char __user *buf,
			size_t len, loff_t *off) {
	printk(KERN_INFO "MY_NULL DEVICE_READ()\n");
	return 0;
}
static ssize_t my_null_write(struct file *f, const char __user *buf,
			size_t len, loff_t *off) {
	printk(KERN_INFO "MY_NULL DEVICE_WRITE()\n");
	printk(KERN_INFO "MY_NULL DEVICE_WRITE LENGTH: %ld\n",len);
	return len;
}

// FILE OPERATIONS SRUCTURE FROM "linux/fs.h"
static struct file_operations my_ops = {
	.owner = THIS_MODULE,
//	.owner = SHUBH_FOPS_OWNER,
	.open = my_null_open,
	.release = my_null_close,
	.read = my_null_read,
	.write = my_null_write,	
};

static int __init IN(void) {
	printk(KERN_INFO " MODULE!!!");
	if(alloc_chrdev_region( &first, 0, 2, "SHUBH") < 0)
		return -1;

	printk(KERN_INFO "Major no.: %d\tMinor no.: %d\n", MAJOR(first), MINOR(first));

	if((cl = class_create(THIS_MODULE, "char_driver")) == NULL) {
		unregister_chrdev_region(first, 1);
		return -1;
	}
	
	if (device_create(cl, NULL, first, NULL, "shubh_null_device") == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}

	cdev_init(&c_dev, &my_ops);
	
	if (cdev_add(&c_dev, first, 1) == -1) {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		return -1;
	}
		
	return 0;
}

static void __exit OUT(void) {
	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "Exiting FROM TEST MODULE!!!");
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("TEST MODULE");
