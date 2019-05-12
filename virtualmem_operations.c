#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include"virtualmem_type.h"
#include<linux/errno.h>

static int my_open(struct inode *inode,struct file *file)
{
        struct test_dev *my_data;
        my_data=container_of(inode->i_cdev,struct test_dev,cdev);
        file->private_data=my_data;
        return 0;
}

static ssize_t my_read(struct file *file,char __user *user_buffer,size_t size, loff_t *offset)
{
        struct test_dev *my_data;
        my_data=(struct test_dev *)file->private_data;
        if(copy_to_user(user_buffer,my_data->mem,size)){
               return -EFAULT;
        }
        printk(KERN_ALERT"read %d byte\n",size);
        return size;
} 

static ssize_t my_write(struct file *file,const char __user *user_buffer,size_t size, loff_t *offset)
{
        struct test_dev *my_data;
        my_data=(struct test_dev *)file->private_data;
        if(copy_from_user(my_data->mem,user_buffer,size)){
                return -EFAULT;
        }
               
        printk(KERN_ALERT"write %d byte\n",size);
        return size;
}
   

static long my_ioctl(struct file *file,unsigned int cmd, unsigned long arg)
{
        return 0;
} 

static int my_release(struct inode *inode, struct file *file)
{
        return 0;
}



const struct file_operations my_fops={
	.owner=THIS_MODULE,
	.open=my_open,
	.read=my_read,
	.write=my_write,
	.unlocked_ioctl=my_ioctl,
	.release=my_release
};
