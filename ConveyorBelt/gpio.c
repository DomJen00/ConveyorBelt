/*
File: gpio.c
Author: HP
As of: 2023-05-12
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gpio.h"

 /*
 Function: gpioOpen
 Parameter: a struct of type gpioDescriptor
 Returns: 0 on succes, -1 on error, including error output
 Does: Since the all the gpios used by me are already enabled,
 only the direction is set accordingly.
 */
int gpioOpen(gpioDescriptor* gpio) {
	char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "%s/gpio%d/direction", GPIO_PATH, gpio->gpioNum);
	retVal = setParameter(filename, gpio->direction);
	if ( retVal < 0) {
		perror("In gpioOpen");
		return (-1);
	}
	return 0;
}

/*
Function: gpioSetValue
Parameter: a struct of type gpioDescriptor, and the value (0 or 1)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the output to the desired value
*/
int gpioSetValue(gpioDescriptor* gpio, int value) {
	char filename[MAX_FILENAME];
	char myBuf[2];
	int retVal;
	sprintf(myBuf, "%d", value);
	sprintf(filename, "%s/gpio%d/value", GPIO_PATH, gpio->gpioNum);
	retVal = setParameter(filename, myBuf);
	if ( retVal < 0) {
		perror("In gpioSetValue");
		return (-1);
	}
	return 0;
}

/*
Function: gpioGetValue
Parameter: a struct of type gpioDescriptor
Returns: the value (0 or 1) or -1 on error, including error output
Does: Reads the input
*/
int gpioGetValue(gpioDescriptor* gpio) {
	char filename[MAX_FILENAME];
	char myBuf[2];
	int myVal;
	int retVal;
	sprintf(filename, "%s/gpio%d/value", GPIO_PATH, gpio->gpioNum);
	retVal = getParameter(filename, myBuf);
	if ( retVal < 0) {
		perror("In gpioGetValue");
		return (-1);
	}
	if (myBuf[0] == '0') {
		myVal = 0;
	}
	else if (myBuf[0] == '1') {
		myVal = 1;
	}
	else {
		perror("gpioGetValue: wrong value");
		return -1;
	}
	return myVal;
}
