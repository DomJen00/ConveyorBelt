/*
File: gpio.h
Author: HP
As of: 2023-05-12
*/


#ifndef GPIO_H_
#define GPIO_H_

#include "funcs.h"

#define GPIO_PATH "/sys/class/gpio"

/*
Struct: gpioDescriptor
gpioNum: Number to identify GPIO. Example: P8_12 (pin number on Beaglebone Black) 
         = GPIO1_12 (pin number of the processor): 32*1+12=44 (GPIO number of the software)
direction: "in" or "out" (C string)
*/
typedef struct {
	int gpioNum;
	char direction [4];
} gpioDescriptor;

/*
Function: gpioOpen
Parameter: a struct of type gpioDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Since the all the gpios used by me are already enabled,
only the direction is set accordingly.
*/
int gpioOpen(gpioDescriptor* gpio);

/*
Function: gpioSetValue
Parameter: a struct of type gpioDescriptor, and the value (0 or 1)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the output to the desired value
*/
int gpioSetValue(gpioDescriptor* gpio, int value);

/*
Function: gpioGetValue
Parameter: a struct of type gpioDescriptor
Returns: the value (0 or 1) or -1 on error, including error output
Does: Reads the input
*/
int gpioGetValue(gpioDescriptor* gpio);

#endif /* GPIO_H_ */
