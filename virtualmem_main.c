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
#define MY_MAJOR	0
#define MY_MAX_MINORS	5

struct test_dev my_cdev;
extern const struct file_operations my_fops;

int virtualmem_init(void)
{
	int err;
	dev_t dev_id=MKDEV(MY_MAJOR,0);
	err=alloc_chrdev_region(&dev_id,0,1,"virtualmem");
	if(err){
		printk(KERN_ALERT"failed to get dev id\n");
		return err;
	}
	printk(KERN_ALERT"dev id = %d\n",dev_id);

	cdev_init(&my_cdev.cdev, &my_fops);
	err=cdev_add(&my_cdev.cdev,dev_id,1);
	if(err){
		printk(KERN_ALERT"failed to add cdev\n");
		return err;
	}
	printk(KERN_ALERT"added cdev successfully\n");
	return 0;

}

void virtualmem_exit(void)
{
	cdev_del(&(my_cdev.cdev));
	unregister_chrdev_region(MKDEV(MY_MAJOR,0),1);
}

module_init(virtualmem_init);
module_exit(virtualmem_exit);
MODULE_LICENSE("GPL");