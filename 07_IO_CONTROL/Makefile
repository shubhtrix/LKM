obj-m:=query_io.o

KERNELDIR=/lib/modules/$(shell uname -r)/build 

all: 
	make -C $(KERNELDIR) M=$(shell pwd) modules

clean:
	make -C $(KERNELDIR) M=$(shell pwd) clean
