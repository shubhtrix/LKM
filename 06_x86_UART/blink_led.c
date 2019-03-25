/* Accessing the UART Tx line for controlling LED through 
 * UART 16550A or 16550D by configuring LCR (Line Control Register)*/
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/delay.h>
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

/* The corresponding APIs (prototyped in <linux/serial_reg.h>) for 
I/O port mapping API's.*/
#include <linux/serial_reg.h>

#define SERIAL_PORT_BASE 0x3f8

static int __init IN(void) {

    int i;
    u8 data;

    data = inb(SERIAL_PORT_BASE + UART_LCR);
    // UART_LCR under linux/serial_reg.h :: #define UART_LCR	3	 

    for (i=0;i<5;i++) {
        /* Pulling the Tx Line Low. */
        data |= UART_LCR_SBC;
        // #define UART_LCR_SBC		0x40
        outb(data, SERIAL_PORT_BASE + UART_LCR);
        msleep(500);
        data &= ~UART_LCR_SBC;
        outb(data, SERIAL_PORT_BASE + UART_LCR);
        msleep(500);
    }
	return 0;
}

static void __exit OUT(void) {
    printk(KERN_INFO "Exiting FROM BLINK MODULE!!!");
}

module_init(IN);
module_exit(OUT);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SHUBH");
MODULE_DESCRIPTION("TEST MODULE");
