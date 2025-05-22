/*
File: qep.c
Author: HP
As of: 2919-10-17
Command line sequence which is followed:
•	sys/devices/platform/ocp/ocp:P8_31_pinmux# echo qep > state
•	sys/devices/platform/ocp/ocp:P8_33_pinmux# echo qep > state
•	sys/devices/platform/ocp/ocp:P8_35_pinmux# echo qep > state
•	sys/devices/platform/ocp/48302000.epwmss/48302180.eqep echo 1 > enabled
>>> This is not needed, since it is enabled by default! <<<
•	sys/devices/platform/ocp/48302000.epwmss/48302180.eqep echo 1 > mode
•	sys/devices/platform/ocp/48302000.epwmss/48302180.eqep cat position
>>> This call shall return the position value! <<<
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include "qep.h"


/*
Function: qepSetPinmux
Parameter: a struct of type qepDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Sets the pinmux of the corresponding three IO pins to qep.
The pinmux information has to be supplied as string in the struct qep.
*/
int qepSetPinmux(qepDescriptor* qep) {
	char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "%s:%s/state", PINMUX_PATH, qep->pinNameA);
	retVal = setParameter(filename, "qep");
	if (retVal < 0) {
		perror("In qepSetPinmux");
		return (-1);
	}
	sprintf(filename, "%s:%s/state", PINMUX_PATH, qep->pinNameB);
	retVal = setParameter(filename, "qep");
	if (retVal < 0) {
		perror("In qepSetPinmux");
		return (-1);
	}
	sprintf(filename, "%s:%s/state", PINMUX_PATH, qep->pinNameIndex);
	retVal = setParameter(filename, "qep");
	if (retVal < 0) {
		perror("In qepSetPinmux");
		return (-1);
	}
	return 0;
}

/*
Function: qepGetPosition
Parameter: a struct of type qepDescriptor
Returns: the position value (long) or -1 on error, including error output
Does: reads the position determined by the qep encoder
*/
long qepGetPosition(qepDescriptor* qep) {
	char filename[MAX_FILENAME];
	char myBuf[MAX_DATA_BUF];
	long myVal;
	int retVal;
	sprintf(filename, "%s/%s/position", QEP_PATH, qep->qepNum);
	retVal = getParameter(filename, myBuf);
	if (retVal < 0) {
		perror("In qepGetPosition");
		return (-1);
	}
	myVal = atol(myBuf);
	return myVal;
}

/*
Function: qepSetPeriod
Parameter: a struct of type qepDescriptor, and the period value in ns
Returns: 0 on succes, -1 on error, including error output
Does: Sets the period of the relative mode (the number read is the number of pulses in a period)
*/
int qepSetPeriod(qepDescriptor* qep, long period) {
	char filename[MAX_FILENAME];
	char myBuf[MAX_DATA_BUF];
	int retVal;
	sprintf(filename, "%s/%s/period", QEP_PATH, qep->qepNum);
	sprintf(myBuf, "%d", period);
	retVal = setParameter(filename, myBuf);
	if (retVal < 0) {
		perror("In qepSetPeriod");
		return (-1);
	}
	return 0;
}

/*
Function: qepSetMode
Parameter: a struct of type qepDescriptor, and the mode value 
Returns: 0 on succes, -1 on error, including error output
Does: Sets the mode to the given value (0 = absolute, 1 = relative)
*/
int qepSetMode(qepDescriptor* qep, unsigned char mode) {
	char filename[MAX_FILENAME];
	char myBuf[MAX_DATA_BUF];
	int retVal;
	sprintf(filename, "%s/%s/mode", QEP_PATH, qep->qepNum);
	sprintf(myBuf, "%d", mode);
	retVal = setParameter(filename, myBuf);
	if (retVal < 0) {
		perror("In qepSetPeriod");
		return (-1);
	}
	return 0;
}
