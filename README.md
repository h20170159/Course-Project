# Course-Project
          ADC IMPLEMENTATION OVER USB

This repository contains adc implementation over usb.
In this project a selfmade hardware is used.

what will you get inside this

In this project internal ADC of atmega8 controller is used , when user requests data from a specific channel ,then data from adc is transferred to user over USB . The transfer is done using usb_control_msg().

This repository contains

1. Documentation
   This contains theoritical portion , adc registers, usb control message structure and uses and  reference links also.

1. Hardware
  which includes components list
  all circuit diagram, layout of pcb (in case if you wanted to make it on copper pcb)

2. Driver
   This has kernel side code which is implemented through char driver

3. Firmware
   This contains firwareside code 
   Firware side code is the code which implements actual functionality of adc ,all adc related functioning done in this code.
   
   
   
   How to build this ?
   
   install avrdude
   
    clone the complete repository
    
    $ cd Course-project/firmware
    
    $ make all
    
    $ avrdude -c usbasp -p m8 -U flash:w:main.hex
    
    $ cd Course-project/Driver
    
    $ make                      this will generate executable file (i.e driver.ko will be generated)
    
    $ sudo insmod driver.ko     it will insert the module
    
    $ gcc -o adc_user adc_user.c
    
    $ ./adc_user                (here choose menu accordingly)
    
                   (if permission denied then change permission by following command
                    $ chmod 777 /dev/adc0)
                    
    $ cd /dev
    
    $ ls -l         here you can see a char driver has been created named as adc0
    
    $ sudo rmmod driver.ko              it will remove the module
    
    
    
    

   


