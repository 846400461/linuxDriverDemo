#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/types.h>
//test
struct test_dev
{
	struct cdev cdev;
	unsigned char mem[128];
};

dev_t devno=0;
static int hello_init(void)
{

	printk(KERN_ALERT"Hello World\n");

	devno=MKDEV(0,0);
	alloc_chrdev_region(&devno,0,1,"virhello");
#ifdef CONFIG_TEST
	printk(KERN_ALERT"Hello Kconfig\n");
#endif
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT"Bye Bye World\n");
	unregister_chrdev_region(devno,1);
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");

