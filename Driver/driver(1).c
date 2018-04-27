
#include <linux/module.h> 
#include <linux/kernel.h>
#include <linux/init.h> // for printk()
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/uaccess.h>
#include <linux/time.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/list.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/proc_fs.h>
#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb.h>
#include<linux/slab.h>
#include<linux/types.h>

#define VID  0x16C0
#define PID  0x05DC

#define SET_CHANNEL 0x40
#define GET_CHANNEL 0xC0

#include "ioctl.h"


#define MAJOR_NO 200
#define MINOR_NO 30


char channel[] = {'1'};
struct cdev * mycdev;
dev_t mychardevice;
struct class * myclass;

unsigned char buffer[8];
int ret_val;


    // int  errorCode = USB_ERROR_NOTFOUND;
     //spinlock_t lock;
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

static int usbdev_probe(struct usb_interface *interface, const struct usb_device_id *id)
{

	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);

	struct usb_device *udev = interface_to_usbdev(interface);

	


	//void *buffer=kmalloc(16,GFP_KERNEL);
	//unsigned char buffer=kmalloc(16,GFP_KERNEL);
        int                 nBytes;
        int buf0;
	int msg=1;

	if(id->idProduct == PID)
	{
		printk(KERN_INFO "device available in\n");
	}


//if(strcmp(argv[1], "adc") == 0){
	   // spin_lock(&lock);

            nBytes = usb_control_msg(udev,usb_sndctrlpipe(udev, 0) ,0, SET_CHANNEL, 0, 0,(char *)buffer, sizeof(buffer), 5000);


if (nBytes != 0)
     		{	
        		printk(KERN_ALERT "Failed to send data to device\n");
     		}
     
       
	nBytes = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0) ,0, GET_CHANNEL, msg, 0,(char *)buffer, sizeof(buffer), 5000);
  
  
if (nBytes < 2)
     		{
        		printk(KERN_ALERT "nBytes = %d, func= %s Failed to get correct reply", nBytes, __func__);
        		return -EREMOTEIO;
     		}
           buf0 = buffer[0]|buffer[1]<<9;
 
            printk(KERN_INFO "adc value from msg field is %d\n",msg);
	 printk(KERN_INFO "adc value from data buffer is %d\n",buf0);
  printk(KERN_INFO "adc value LOWER byte is %d\n",buffer[0]);
 printk(KERN_INFO "adc value HIGHER byte is %d\n",buffer[1]);

	return 0;
}

static char *Message_Ptr;

/*Operations structure*/
static struct usb_driver usbdev_driver = {
	name: "usbdev",  //name of the device
	probe: usbdev_probe, // Whenever Device is plugged in
	disconnect: usbdev_disconnect, // When we remove a device
	id_table: usbdev_table, //  List of devices served by this driver
};



//-------------------------------------------------------------------------------------------------
static int device_open(struct inode *inode, struct file *file)
{
printk(KERN_INFO "device_open\n");
    //try_module_get(THIS_MODULE);
    return SUCCESS;
}
//-------------------------------------------------------------------------------------------------
static int device_release(struct inode *inode, struct file *file)
{
printk(KERN_INFO "device_release\n");
    //module_put(THIS_MODULE);
    return SUCCESS;
}
//-------------------------------------------------------------------------------------------------
static ssize_t device_read(struct file *file, char __user * buffer, size_t len, loff_t * off)
{
	int bytes_read=0;
	if(*Message_Ptr==0)
		return 0;
	while(len && *Message_Ptr)
	{
		if(copy_to_user((ret_val *)ioctl_param, &buffer, sizeof(ret_val)))
			return -1;
		len--;
		bytes_read++;
	}

printk(KERN_INFO "Read %d bytes, %d left \n",bytes_read, len);

return bytes_read;
}
	/*static char *d;
	int c=1;
	int i=0;
	char y='0';
	char x[]="0000";
	printk(KERN_INFO "device_read\n");
	
	//Here read from ADC

	if(d<0)
		d=0;
	printk(KERN_INFO "device_read %d\n", d);
     	put_user(d,buffer++);
	return SUCCESS;

}*/
//-------------------------------------------------------------------------------------------------
long device_ioctl(struct file *file,	/* ditto */
                  unsigned int ioctl_num,        	/* number and param for ioctl */
                  unsigned long ioctl_param)
{
	char *temp1;
	char d;
	printk(KERN_INFO "device_ioctl\n");

    switch (ioctl_num) {
    case IOCTL_SET_CHANNEL:
        temp1 = (char *)ioctl_param;
	// Ask user to set ADC channel
	 if(copy_from_user(&buffer,(ret_val *)ioctl_param,sizeof(ret_val)))
		return -1;
	printk(KERN_INFO "channel number selected: %c\n",temp1[0]);
	break;

    case IOCTL_SET_ALIGN:
	printk(KERN_INFO "allignment: do nothing %c\n",'p');
	break;
    }

    return 5;
}
//-------------------------------------------------------------------------------------------------------
static struct file_operations Fops = {
	.owner = THIS_MODULE,
        .read = device_read,
        .unlocked_ioctl = device_ioctl,
        .open = device_open,
        .release = device_release      /* a.k.a. close */
};
//-------------------------------------------------------------------------------------------------------
int  mod_init(void){

printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	usb_register(&usbdev_driver);
	

int y,result;
   	mychardevice=MKDEV(MAJOR_NO,MINOR_NO);

	printk(KERN_INFO "Created device with <MAJOR,MINOR>: <%d,%d>\n",MAJOR(mychardevice),MINOR(mychardevice));	
	

	

	mycdev = cdev_alloc();

    	if(!mycdev) {
		printk(KERN_ERR "failed to alloc cdev\n");
    		return -1;
    	}
    	
	if ((myclass = class_create(THIS_MODULE, "myclass")) == NULL)
  	{
    		unregister_chrdev_region(mychardevice, 1);
    		return -1;
  	}
    	if (device_create(myclass, NULL, mychardevice, NULL,"mydevice") == NULL)
  	{
   	 	class_destroy(myclass);
    		unregister_chrdev_region(mychardevice, 1);
    		return -1;
  	}
	
	cdev_init(mycdev, &Fops);
    	result = cdev_add(mycdev, mychardevice, 1);
    	if(result < 0) {
		printk(KERN_ERR "failed to add cdev\n");
	}
	
	y=cdev_add(mycdev,mychardevice,1);
        printk(KERN_ALERT "--%d--\n",y);
	return 0;
}
//-------------------------------------------------------------------------------------------------------
void mod_exit(void){
	printk(KERN_ALERT "Inside the %s function\n", __FUNCTION__);
	usb_deregister(&usbdev_driver);
	printk(KERN_NOTICE "Leaving Kernel\n");
	printk(KERN_INFO "In exit module.\n");
	if(MAJOR(mychardevice)>0){
		cdev_del(mycdev);
		unregister_chrdev_region(mychardevice,1);
  		device_destroy(myclass, mychardevice);
  		class_destroy(myclass);
		printk(KERN_INFO "Device unregistered!\n");
	}
	else{
		printk(KERN_INFO "No device to unregister!\n");	
	}
}



//-------------------------------------------------------------------------------------------------------
module_init(mod_init);
module_exit(mod_exit);
MODULE_LICENSE("GPL");
//-------------------------------------------------------------------------------------------------------



