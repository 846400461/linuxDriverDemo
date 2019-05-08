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
#define MEM_CLEAR 0x1
#define VIRTUALMEM_SIZE 0x1000
#define VIRTUALMEM_MAJOR 0
static int Virtualmem_major = VIRTUALMEM_MAJOR;
struct Virtualmem_dev
{
	struct cdev cdev;
	unsigned char mem[VIRTUALMEM_SIZE];
};
struct Virtualmem_dev *Virtualmem_devp;
int Virtualmem_open(struct inode *inode, struct file *filp)
{
	struct Virtualmem_dev *dev;
	dev= container_of(inode->i_cdev,struct Virtualmem_dev,cdev);
	filp->private_data=dev;
	return 0;
}
	
int Virtualmem_release( struct inode *inode, struct file *filp)
{
	return 0;
}

static ssize_t Virtualmem_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret =0;
	struct Virtualmem_dev *dev= filp->private_data;
	if (p >= VIRTUALMEM_SIZE)
		return count ? - ENXIO: 0;
	if(count > VIRTUALMEM_SIZE-p)
		count= VIRTUALMEM_SIZE-p;
	if(copy_to_user(buf, (void*)(dev->mem + p),count))
	{
		ret= - EFAULT;
	}
	else
	{
		ret=count;
		printk(KERN_INFO"read %d bytes(s) from %d\n", count,(int)p);
	}

	return ret;	
}

static ssize_t Virualmem_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long p = *ppos;
	unsigned int count = size;
	int ret= 0;
	struct Virtualmem_dev *dev = filp->private_data;
	if(p >= VIRTUALMEM_SIZE)
		return count ? - ENXIO: 0;
	if(count > VIRTUALMEM_SIZE-p)
		count=VIRTUALMEM_SIZE-p;
	if(copy_from_user(dev->mem+p,buf,count))
		ret= -EFAULT;
	else
	{
		ret=count;
		printk(KERN_INFO"written %d bytes(s) from %d\n",count,(int)p);
	}
	return ret;
	
}

static long Virtualmem_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct Virtualmem_dev *devp = filp->private_data;
	switch(cmd)
	{
		case MEM_CLEAR:
			memset(devp->mem,arg,VIRTUALMEM_SIZE);
		default:
			return -EINVAL;
	}	

	return 0;
}

static const struct file_operations Virtualmem_fops =
{
	.owner=THIS_MODULE,
	.read=Virtualmem_read,
	.write=Virualmem_write,
	.open=Virtualmem_open,
	.release=Virtualmem_release,
	.unlocked_ioctl= Virtualmem_ioctl,
};

static void Virtualmem_setup_cdev(struct Virtualmem_dev *dev, int index)
{
	int err,devno = MKDEV(Virtualmem_major, index);
	cdev_init(&dev->cdev, &Virtualmem_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &Virtualmem_fops;
	err = cdev_add(&dev->cdev,devno,1);
	if(err)
		printk(KERN_NOTICE"Error %d adding Virtualmem %d",err,index);	
}

int Virtualmem_init(void)
{
	int result;
	dev_t devno = MKDEV(Virtualmem_major,0);
	if(Virtualmem_major)
		result=register_chrdev_region(devno,1,"Virtualmem");
	else
	{
		result=alloc_chrdev_region(&devno,0,1,"Virtualmem");
		Virtualmem_major=MAJOR(devno);
	}

	if(result < 0)
		return result;
	Virtualmem_devp = kmalloc(sizeof(struct Virtualmem_dev),GFP_KERNEL);
	if(!Virtualmem_devp)
	{
		result = - ENOMEM;
		goto fail_malloc;
	}

	memset(Virtualmem_devp,0,sizeof(struct Virtualmem_dev));
	Virtualmem_setup_cdev(Virtualmem_devp,0);
	printk(KERN_ALERT"finish\n");
	return 0;
fail_malloc: unregister_chrdev_region(devno,1);
	return result;
}

void Virtualmem_exit(void)
{
	cdev_del(&Virtualmem_devp->cdev);
	kfree(Virtualmem_devp);
	unregister_chrdev_region(MKDEV(Virtualmem_major,0),1);
}

MODULE_AUTHOR("Wednesday");
MODULE_LICENSE("GPL");
module_init(Virtualmem_init);
module_exit(Virtualmem_exit);