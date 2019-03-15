#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>	
#include <linux/kdev_t.h>	//For Major and minor numbers
#include <linux/fs.h>		//Function alloc_chardev_register()
#include <linux/device.h>	//For class create, device create & device destroy.
#include <linux/cdev.h>		//For cdev_init & cdev_add.

/* To check user-buffer limits, pease use APIs
 * copy_to_user and copy_from_user          */
#include <linux/uaccess.h>

/* Need to have some kind of a user buffer. */
char usr_bfr[512];
static int count=0;

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
    // Need to write len bytes to buffer buf
    // buf[0] = usr_bfr[0];        // Good Practice to use copy_to_user instead
    if (copy_to_user (buf, usr_bfr, 1) != 0) {
        return -EFAULT;
    } else {
        printk(KERN_INFO "READ Data from user buffer:: %c\n", buf[0]);
        if (count) {
            return 0;
        } else {
            ++count;
            return 1;
        }
        ++off;
    }
}
static ssize_t my_null_write(struct file *f, const char __user *buf,
			size_t len, loff_t *off) {
	printk(KERN_INFO "MY_NULL DEVICE_WRITE()\n");
    //usr_bfr[0] = buf[0];        // Good Practice to use copy_from_user instead
    
    if (copy_from_user (usr_bfr,buf,1) !=0) {
        return -EFAULT;
    } else {
        printk(KERN_INFO "Buffer data provided by user :: %c", usr_bfr[0]);
	    printk(KERN_INFO "MY_NULL DEVICE_WRITE LENGTH: %ld",len);
        return len;
    }
}

// FILE OPERATIONS SRUCTURE FROM "linux/fs.h"
static struct file_operations my_ops = {
	.owner = THIS_MODULE,
	.open = my_null_open,
	.release = my_null_close,
	.read = my_null_read,
	.write = my_null_write,
};

static int __init IN(void) {
	
    if(alloc_chrdev_region( &first, 0, 2, "SHUBH") < 0)
		return -1;

	printk(KERN_INFO "Major no.: %d\tMinor no.: %d\n", MAJOR(first), MINOR(first));

	//if((cl = class_create(THIS_MODULE, "char_driver")) == NULL) {
	if((cl = class_create(THIS_MODULE, "nursery")) == NULL) {
		unregister_chrdev_region(first, 2);
		return -1;
	}
	
	if (device_create(cl, NULL, first, NULL, "gnric_hw_accss") == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(first, 2);
		return -1;
	}

	cdev_init(&c_dev, &my_ops);
	
	if (cdev_add(&c_dev, first, 1) == -1) {
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 2);
		return -1;
	}
		
	return 0;
}

static void __exit OUT(void) {

	cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 2);
	printk(KERN_INFO "Exiting FROM TEST MODULE!!!");
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("TEST MODULE");
