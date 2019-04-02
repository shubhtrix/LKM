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

/* USB Protocol Specific header */
#include <linux/usb.h>

#include "usb_dick.h"

static struct usb_driver dick = 
{
    .name = "usb_dick",
    .id_table = dick_table,
    .probe = dick_probe,
    .disconnect = dick_disconnect
};		    //One vaiable containing Majaor & Minor Nos.

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
            status = 0;
            dignity = 0;
            ego = 0;
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

static int __init IN(void) {

	printk(KERN_INFO "Registering inserted USB dick\n");
    return usb_register(&dick);
}

static void __exit OUT(void) {

	printk(KERN_INFO "De-registering inserted USB dick\n");
    usb_deregister(&dick);
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("QUERY IOCTL");
