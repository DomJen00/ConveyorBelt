/*
File: qep.h
Author: HP
As of: 2919-10-17
*/

#ifndef QEP_H_
#define QEP_H_

#include "funcs.h"

#define QEP_PATH "/sys/devices/platform/ocp"
#define PINMUX_PATH "/sys/devices/platform/ocp/ocp"

/* 
Connection information:
Encoder pin A is tied to qep2 pin A (P8_35).
Encoder pin B is tied to qep2 pin B (P8_33).
Encoder pin Index is tied to qep2 pin Index (P8_31).
*/

/*
Struct: qepDescriptor
qepNum: Number of qep to be used. On this board qep2 is "48302000.epwmss/48302180.eqep" (C string)
pinNameA: pin assigned to input A of qep. For qep2 it is "P8_35_pinmux" (C string)
pinNameB: pin assigned to input B of qep. For qep2 it is "P8_33_pinmux" (C string)
pinNameIndex: pin assigned to Index input of qep. For qep2 it is "P8_31_pinmux" (C string)
*/
typedef struct {
	char qepNum[MAX_FILENAME];
	char pinNameA[MAX_FILENAME];
	char pinNameB[MAX_FILENAME];
	char pinNameIndex[MAX_FILENAME];
} qepDescriptor;

/*
Function: qepSetPinmux
Parameter: a struct of type qepDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Sets the pinmux of the corresponding three IO pins to qep.
The pinmux information has to be supplied as string in the struct qep.
*/
int qepSetPinmux(qepDescriptor* qep);

/*
Function: qepGetPosition
Parameter: a struct of type qepDescriptor
Returns: the position value (long) or -1 on error, including error output
Does: reads the position determined by the qep encoder
*/
long qepGetPosition(qepDescriptor* qep);

/*
Function: qepSetPeriod
Parameter: a struct of type qepDescriptor, and the period value in ns
Returns: 0 on succes, -1 on error, including error output
Does: Sets the period of the relative mode (the number read is the number of pulses in a period)
*/
int qepSetPeriod(qepDescriptor* qep, long period);

/*
Function: qepSetMode
Parameter: a struct of type qepDescriptor, and the mode value
Returns: 0 on succes, -1 on error, including error output
Does: Sets the mode to the given value (0 = absolute, 1 = relative)
*/
int qepSetMode(qepDescriptor* qep, unsigned char mode);

#endif /* QEP_H_ */