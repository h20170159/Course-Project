#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>

#include<linux/usb.h>
#include<linux/slab.h>
#include<linux/types.h>

#include "adc_ioctl.h"

#define VID  0x16C0
#define PID  0x05DC
 
#define FIRST_MINOR 2
#define MINOR_CNT 3
 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
static int channel = 1;
 
static int my_open(struct inode *i, struct file *f)
{ printk(KERN_INFO "Device File Opened...!!!\n");
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{printk(KERN_INFO "Device File Closed...!!!\n");
    return 0;
}

 
static ssize_t my_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Write function\n");
    return 0;
}

static void usbdev_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO " Device Removed\n");
	return;
}

static struct usb_device_id usbdev_table [] = {
	{USB_DEVICE(VID,PID)},
	{} /*terminating entry*/	
};
MODULE_DEVICE_TABLE(usb, usbdev_table);


static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
    printk(KERN_INFO "Read Function\n");
    return 0;
}


/////////////////////////usb probe function/////////////////////

static int usbdev_probe(struct usb_interface *interface, const struct usb_device_id *id)
{

printk(KERN_ALERT "Inside the %s function",__FUNCTION__);

	struct usb_device *udev = interface_to_usbdev(interface);

unsigned char       buffer[8];	// Used to receive data from firmware
  int               nBytes; 
int msg=1;


	//void *buffer=kmalloc(16,GFP_KERNEL);
	//unsigned char buffer=kmalloc(16,GFP_KERNEL);
      
      
	if(id->idProduct == PID)
	{
		printk(KERN_INFO "device is available\n");
	}


	   // spin_lock(&lock);
	
	  
            nBytes = usb_control_msg(udev,usb_sndctrlpipe(udev, 0) ,0, SET_CHANNEL, 0, 0,(char *)buffer, sizeof(buffer), 5000);// This control message is to send data to firmware


       
	nBytes = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0) ,0, GET_CHANNEL, msg, 0,(char *)buffer, sizeof(buffer), 5000);// This control message is to receive data from firmware
  
  
if (nBytes < 2)
     		{
        		printk(KERN_ALERT "nBytes = %d, func= %s Failed to get correct reply", nBytes, __func__);
        		return -EREMOTEIO;
     		}
         int  buf0 = buffer[0]|buffer[1]<<9;
        
        printk(KERN_INFO "Actual adc value from data buffer is %d\n",buf0);
	printk(KERN_INFO "adc value from message is %d\n",msg);
	printk(KERN_INFO "adc value from data buffer low is %d\n",buffer[0]);
	 

	return 0;
}
	
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    adc_arg_t q;
 
    switch (cmd)
    {
        case ADC_GET_VARIABLES:
            q.channel = channel;
        
            if (copy_to_user((adc_arg_t *)arg, &q, sizeof(adc_arg_t)))
            {
                return -EACCES;
            }
            break;
        case ADC_SET_VARIABLES:
            if (copy_from_user(&q, (adc_arg_t *)arg, sizeof(adc_arg_t)))
            {
                return -EACCES;
            }
 printk(KERN_INFO "q= %d\n", q);
            channel = q.channel;
           
            break;
        default:
            return -EINVAL;
    }
 
    return 0;
}
 
static struct file_operations fops =
{   
    .owner = THIS_MODULE,
    .read  = my_read,
    .write = my_write,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = my_ioctl

};
 
/////////////////////usb operation structure///////////

/*Operations structure*/
static struct usb_driver usbdev_driver = {
	name: "usbdev",  //name of the device
	probe: usbdev_probe, // Whenever Device is plugged in
	disconnect: usbdev_disconnect, // When we remove a device
	id_table: usbdev_table, //  List of devices served by this driver
};

static int __init adc_ioctl_init(void)
{
 if ((alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "adc_ioctl")) < 0)
    {
 
        return -1;
    }
  printk(KERN_INFO "Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 usb_register(&usbdev_driver);//usb register
cdev_init(&c_dev, &fops);
c_dev.owner = THIS_MODULE;
        c_dev.ops = &fops;
 
    if ((cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        printk(KERN_INFO "Cannot add the device to the system\n");
            goto r_class;
    }
     
    if ((cl = class_create(THIS_MODULE, "char"))==NULL)
    { printk(KERN_INFO "Cannot create the struct class\n");
            goto r_class;
}
     //   cdev_del(&c_dev);
       // unregister_chrdev_region(dev, MINOR_CNT);
        //return PTR_ERR(cl);
    //}
    if ((device_create(cl, NULL, dev, NULL, "adc0"))==NULL)
    {printk(KERN_INFO "Cannot create the Device 1\n");
            goto r_device;
    }
 
    printk(KERN_INFO "Device Driver Insert...Done!!!\n");
    return 0;

r_device:
        class_destroy(cl);
r_class:
        unregister_chrdev_region(dev, MINOR_CNT);
        return -1;
}
 
static void __exit adc_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
usb_deregister(&usbdev_driver);
printk(KERN_INFO "Device Driver Remove...Done!!!\n");
}
 
module_init(adc_ioctl_init);
module_exit(adc_ioctl_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("MONIKA VIJAY");
MODULE_DESCRIPTION("ADC Over USB Implementation with Char Driver");
