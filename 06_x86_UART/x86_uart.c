/* Accessing the video RAM of DOS days. */
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

/* The corresponding APIs (prototyped in <asm/io.h>) for 
mapping and unmapping the device bus addresses to virtual addresses
*/
#include <asm/io.h>

#define VRAM_BASE 0x000A0000
#define VRAM_SIZE 0x00020000

/* Need to have some kind of a user buffer. */
static void __iomem *vram;

static dev_t first;		    //One vaiable containing Majaor & Minor Nos.
static struct cdev c_dev;	//Global variable for character device structure 
static struct class *cl;	//Global variable for device class 

static int my_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "MY_NULL DEVICE_OPEN()\n");
	return 0;
}
static int my_close(struct inode *i, struct file *f) {
	printk(KERN_INFO "MY_NULL DEVICE_CLOSE()\n");
	return 0;
}
static ssize_t my_read(struct file *f, char __user *buf,
			size_t len, loff_t *off) {

    int i;
    u8 byte;
    printk(KERN_INFO "MY_NULL DEVICE_READ()\n");

    if (*off >= VRAM_SIZE) {
        return 0;
    }
    if (*off + len > VRAM_SIZE) {
        len = VRAM_SIZE - *off;
    }

    for (i=0;i<len;i++) {
    
        byte = ioread8((u8 *)vram + *off + i);
        if (copy_to_user (buf + i, &byte, 1)) {
            return -EFAULT;
        }
    }
    *off += len;

    return len;
}

static ssize_t my_write(struct file *f, const char __user *buf,
			size_t len, loff_t *off) {
    int i;
    u8 byte;
    printk(KERN_INFO "MY_NULL DEVICE_WRITE()\n");

    if (*off >= VRAM_SIZE) {
        return 0;
    }
    if (*off + len > VRAM_SIZE) {
        len = VRAM_SIZE - *off;
    }

    for (i=0;i<len;i++) {
    
        if (copy_from_user (&byte, buf + i, 1)) {
            return -EFAULT;
        }
        iowrite8(byte, (u8 *)vram + *off + i);
    }
    *off += len;

    return len;
}

// FILE OPERATIONS SRUCTURE FROM "linux/fs.h"
static struct file_operations my_ops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = my_read,
	.write = my_write,
};

static int __init IN(void) {

    if ((vram = ioremap(VRAM_BASE, VRAM_SIZE)) == NULL)
    {
        printk(KERN_ERR "Mapping video RAM failed\n");
        return -1;
    }    
    if(alloc_chrdev_region( &first, 0, 2, "vram") < 0)
		return -1;

	printk(KERN_INFO "Major no.: %d\tMinor no.: %d\n", MAJOR(first), MINOR(first));

	//if((cl = class_create(THIS_MODULE, "char_driver")) == NULL) {
	if((cl = class_create(THIS_MODULE, "nursery")) == NULL) {
		unregister_chrdev_region(first, 2);
		return -1;
	}
	
	if (device_create(cl, NULL, first, NULL, "vram") == NULL) {
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
    iounmap(vram);
	printk(KERN_INFO "Exiting FROM TEST MODULE!!!");
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("TEST MODULE");
