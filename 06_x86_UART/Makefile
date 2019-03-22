obj-m:=x86_uart.o

KERNELDIR=/lib/modules/$(shell uname -r)/build 

all: 
	make -C $(KERNELDIR) M=$(shell pwd) modules

clean:
	make -C $(KERNELDIR) M=$(shell pwd) clean
