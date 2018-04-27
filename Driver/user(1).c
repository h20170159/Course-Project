#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>              /* open */
#include <unistd.h>             /* exit */
#include <sys/ioctl.h>          /* ioctl */
#include "ioctl.h"




int ioctl_set_channel(int file_desc, char *message)
{
    int ret_val;

    ret_val = ioctl(file_desc, IOCTL_SET_CHANNEL, message);

    if (ret_val < 0) {
        printf("ioctl_set_msg failed:%d\n", ret_val);
        exit(-1);
    }
    return 0;
}

int ioctl_set_align(int file_desc, char *message)
{
    int ret_val;

    ret_val = ioctl(file_desc, IOCTL_SET_ALIGN, message);

    if (ret_val < 0) {
        printf("ioctl_set_ali failed:%d\n", ret_val);
        exit(-1);
    }
    return 0;
}

int main()
{
    	int file_desc, ret_val;
    	int op;
	char buf[4];
    	file_desc = open(DEVICE_FILE_NAME, 0);
       	printf("DEVICE_FILE_NAME: %d\n", file_desc);
	ret_val = read(file_desc,buf, 1);
       	printf("ret_val: %d\n", ret_val);

    	if (file_desc < 0) {
        	printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
        	exit(-1);
    	}
	else{
        	printf("Openning file: %s\n", DEVICE_FILE_NAME);
    	}
read:
	if (ret_val < 0) {
        	printf("Can't read device file: %d\n", ret_val);
        	exit(-1);
    	}
	else{
        	printf("Reading file: %s\n", DEVICE_FILE_NAME);
		printf("value from adc is :  ||%d||\n" ,(int)*buf);
    	}
goto menu;

chan:	
	printf("2. Choose channel(0/1/2) \n");
	scanf("%d",&op);
	if(op==0)
		ioctl_set_channel(file_desc, "0");
	if(op==1)
		ioctl_set_channel(file_desc, "1");
	if(op==2)
		ioctl_set_channel(file_desc, "2");

menu:	printf("Choose: \n");
	printf("1. Read current channel \n");
	printf("2. Change channel(0/1/2) \n");
	printf("3. Exit\n");
	scanf("%d",&op);
	if(op==1)
		goto read;
	if(op==2)
		goto chan;
	
	//ioctl_set_align(file_desc, "m");
	

    	close(file_desc);
    	return 0;
}
