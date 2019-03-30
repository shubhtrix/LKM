#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/types.h>	
#include <linux/kdev_t.h>	//For Major and minor numbers
#include <linux/fs.h>		//Function alloc_chardev_register()
#include <linux/device.h>	//For class create, device create & device destroy.
#include <linux/cdev.h>		//For cdev_init & cdev_add.
#include <linux/errno.h>	//For error handeling.
#include <asm/uaccess.h>
#include <linux/uaccess.h>

#include "query_io.h"

#define FIRST_MINOR 0
#define MINOR_CNT 0

static dev_t dev;		    //One vaiable containing Majaor & Minor Nos.
static struct cdev c_dev;	//Global variable for character device structure 
static struct class *cl;	//Global variable for device class 
static int status = 1, dignity = 3, ego = 5; 

static int my_open(struct inode *i, struct file *f) {
	printk(KERN_INFO "MY_NULL DEVICE_OPEN()\n");
	return 0;
}
static int my_close(struct inode *i, struct file *f) {
	printk(KERN_INFO "MY_NULL DEVICE_CLOSE()\n");
	return 0;
}
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    query_arg_t q;

    switch (cmd) {
        case QUERY_GET_VARIABLES:
            q.status = status;
            q.dignity = dignity;
            q.ego = ego;
            if (copy_to_user((query_arg_t *)arg, &q, sizeof(query_arg_t)))
            {
                return -EACCES;
            }
            break;
        case QUERY_CLR_VARIABLES:
            q.status = 0;
            q.dignity = 0;
            q.ego = 0;
            if (copy_to_user((query_arg_t *)arg, &q, sizeof(query_arg_t)))
            {
                return -EACCES;
            }
            break;
        case QUERY_SET_VARIABLES:
            if (copy_from_user(&q, (query_arg_t *)arg, sizeof(query_arg_t)))
            {
                return -EACCES;
            }
            status = q.status;
            dignity = q.dignity;
            ego = q.ego;
            break;
        default:
            return -EINVAL;
    }

    return 0;
}

// FILE OPERATIONS SRUCTURE FROM "linux/fs.h"
static struct file_operations my_query_ops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
};

static int __init IN(void) {

    int ret;
    struct device *dev_ret;

    if((ret = alloc_chrdev_region( &dev, FIRST_MINOR, MINOR_CNT, "query_io")) < 0) {
		return ret;
    }

	printk(KERN_INFO "Major no.: %d\tMinor no.: %d\n", MAJOR(dev), MINOR(dev));

	cdev_init(&c_dev, &my_query_ops);
	if ((ret = cdev_add(&c_dev, dev, 1)) == -1) {
		return ret;
	}

	if(IS_ERR(cl = class_create(THIS_MODULE, "char"))) {
        cdev_del(&c_dev);
		unregister_chrdev_region(dev, 2);
		return PTR_ERR(cl);
	}
	
	if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "query"))) {
		class_destroy(cl);
        cdev_del(&c_dev);
		unregister_chrdev_region(dev, 2);
		return PTR_ERR(dev_ret);
	}

	return 0;
}

static void __exit OUT(void) {

	device_destroy(cl, dev);
	class_destroy(cl);
    cdev_del(&c_dev);
	unregister_chrdev_region(dev, MINOR_CNT);
	printk(KERN_INFO "Exiting FROM TEST MODULE!!!");
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("QUERY IOCTL");
