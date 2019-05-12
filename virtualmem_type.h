#ifndef VIRTUAL_TYPE_H
#define VIRTUAL_TYPE_H
#include<linux/cdev.h>
#include<linux/types.h>

struct test_dev
{
	struct cdev cdev;
	unsigned char mem[4096];
	int mode;
};

#endif