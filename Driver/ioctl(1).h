#ifndef IOCTL_H
#define IOCTL_H

#include <linux/ioctl.h>

#define IOCTL_SET_CHANNEL _IOW('a', 0, char *)
#define IOCTL_SET_ALIGN _IOW('a', 1, char *)

#define DEVICE_FILE_NAME "/dev/chardevice"

#define SUCCESS 0
#define DEVICE_NAME "chardevice"

int MYMAJOR;
int MYMINOR;
int ret_val;

#endif
