#ifndef ADC_IOCTL_H
#define ADC_IOCTL_H

#define SET_CHANNEL 0x40
#define GET_CHANNEL 0xC0

#include <linux/ioctl.h>
 
typedef struct
{
    int channel, alignment;
} adc_arg_t;
 
#define ADC_GET_VARIABLES _IOR('a', 'b', adc_arg_t *)

#define ADC_SET_VARIABLES _IOW('a', 'a', adc_arg_t *)
 
#endif
