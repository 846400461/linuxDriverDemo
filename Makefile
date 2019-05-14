CONFIG_HELLO			?=m
obj-$(CONFIG_HELLO)		+=virtualmem.o
virtualmem-objs			:=virtualmem_main.o virtualmem_operations.o
KDIR				:=$$PWD"/../../"
ARCH				:= arm                                  #add for caibinddd
CROSS_COMPILE			:=arm-none-linux-gnueabi-               #add for caibin

default:
	$(MAKE) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) -C $(KDIR) M=$$PWD modules
clean:
	rm -rf *.o* *~ core .depend .*.cmd *.ko *.mod.c
	rm -rf .tmp_versions/
	rm -rf *.symvers
