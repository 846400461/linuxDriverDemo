obj-$(CONFIG_HELLO)		+=test.o
#hello-$(CONFIG_TEST)		:=test.o
#hostprogs-y	:=hello		
KDIR	:=/home/caibin/linuxProject/kernel/linux-stable
#PWD		:=$(shell pwd)				#caibinss
ARCH           := arm                                  #add for caibin
CROSS_COMPILE  :=arm-none-linux-gnueabi-               #add for caibin

default:
	$(MAKE) CROSS_COMPILE=$(CROSS_COMPILE) ARCH=$(ARCH) -C $(KDIR) M=$$PWD modules
clean:
	rm -rf *.o* *~ core .depend .*.cmd *.ko *.mod.c
	rm -rf .tmp_versions/
	rm -rf *.symvers
