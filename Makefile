obj-m += hello_world.o  # final object
obj-m += char_driver.o
obj-m += debugfs/debugfs_demo.o
obj-m += print_jiffies.o
obj-m += debugfs_jiffies.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

modules:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o modules.order Module.symvers hello_world.mod.c *.ko
