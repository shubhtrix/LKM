# If called directly from the command line, invoke the kernel build system.
ifeq ($(KERNELRELEASE),)
 
#    KERNEL_SOURCE := /usr/src/linux
    KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build
    PWD := $(shell pwd)

default: module
 
module:
	$(MAKE) -C $(KERNEL_SOURCE) SUBDIRS=$(PWD) modules
 
clean:
	$(MAKE) -C $(KERNEL_SOURCE) SUBDIRS=$(PWD) clean
 
# Otherwise KERNELRELEASE is defined; we've been invoked from the
# kernel build system and can use its language.
else
 
    obj-m := usb_dick.o
 
endif
