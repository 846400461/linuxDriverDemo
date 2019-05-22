#include<linux/init.h>
#include<linux/module.h>
#include<linux/types.h>
#include<linux/fs.h>
#include<linux/errno.h>
#include<linux/mm.h>
#include<linux/cdev.h>
#include<linux/kernel.h>
#include<linux/slab.h>
#include <linux/uaccess.h>
#include"virtualmem_type.h"
#include<linux/device.h>
#include<linux/io.h>
#define MY_MAJOR	0
#define MY_MAX_MINORS	5

struct test_dev my_cdev;
extern const struct file_operations my_fops;
struct class *my_class=NULL;
dev_t dev_id;
extern void __iomem* GPJ2CON;
extern void __iomem* GPJ2DAT;

int virtualmem_init(void)
{
	int err;
	dev_id=MKDEV(MY_MAJOR,0);
	err=alloc_chrdev_region(&dev_id,0,1,"virtualmem");
	if(err){
		printk(KERN_ALERT"failed to get dev id\n");
		return err;
	}

	cdev_init(&my_cdev.cdev, &my_fops);
	err=cdev_add(&my_cdev.cdev,dev_id,1);
	if(err){
		printk(KERN_ALERT"failed to add cdev\n");
		return err;
	}

	my_class=class_create(THIS_MODULE,"testVirtualmem");
	if(IS_ERR(my_class)){
		printk("Err: failed in creating class.\n");
		return 0;
	}
	device_create(my_class,NULL,dev_id,NULL,"testVirMem");

	printk(KERN_ALERT"added cdev successfully\n");

	if(request_mem_region(0xE0200280,8,"GPIO_LED")==NULL)
	{
		printk("gpio erro!\n");
		return -1;
	}

	GPJ2CON=ioremap(0xE0200280,8);
	GPJ2DAT=GPJ2CON+4;
	if(!GPJ2CON){
		printk("gpio erro!\n");
		return -1;
	}
	*(unsigned int *)GPJ2CON=0x11111111;

	return 0;

}

void virtualmem_exit(void)
{
	release_mem_region(0xE0200280,8);
	iounmap(GPJ2CON);
	cdev_del(&(my_cdev.cdev));
	unregister_chrdev_region(dev_id,1);
	device_destroy(my_class,dev_id);
	class_destroy(my_class);
}

module_init(virtualmem_init);
module_exit(virtualmem_exit);
MODULE_LICENSE("GPL");
