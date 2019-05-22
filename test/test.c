#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include <unistd.h>
int main()
{
	int fd,i=0;
	fd=open("/dev/testVirMem",O_RDWR);
	if(fd<0){
	perror("open");
	}
	while(1){
	for(i=0;i<4;i++)
		ioctl(fd,0,i);
	sleep(1);
	for(i=0;i<4;i++)
		ioctl(fd,1,i);
	sleep(1);
	
	}
}
