obj-m += hello_world.o  # final object

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o modules.order Module.symvers hello_world.mod.c *.ko
