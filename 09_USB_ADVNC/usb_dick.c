#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/errno.h>	//For error handeling.

/* USB Protocol Specific header */
#include <linux/usb.h>

static int dick_probe(struct usb_interface *interface, const struct usb_device_id *id)
{
	printk(KERN_INFO "Dick inserted  :: (%04X:%04X)\n", id->idVendor, id->idProduct);
    return 0;    
}

static void dick_disconnect(struct usb_interface *interface) 
{
	printk(KERN_INFO "Dick removed.\n");
}

static struct usb_device_id dick_table[] = 
{
    { USB_DEVICE(0x090c, 0x1000) },
    {} /* Terminating struct */
};
MODULE_DEVICE_TABLE(usb, dick_table);

static struct usb_driver usb_dick = 
{
    .name = "usb_dick",
    .id_table = dick_table,
    .probe = dick_probe,
    .disconnect = dick_disconnect,
};

static int __init IN(void) {

	// printk(KERN_INFO "Registering inserted USB dick\n");
    return usb_register(&usb_dick);
}

static void __exit OUT(void) {

	// printk(KERN_INFO "De-registering inserted USB dick\n");
    usb_deregister(&usb_dick);
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("USB REGISTER");
