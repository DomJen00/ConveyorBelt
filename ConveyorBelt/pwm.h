/*
File: pwm.h
Author: HP
As of: 2919-10-17
*/

#ifndef PWM_H_
#define PWM_H_

#include "funcs.h"

#define PWM_PATH "/sys/class/pwm/pwmchip"
#define PINMUX_PATH "/sys/devices/platform/ocp/ocp"

/* 
Connection information:
H-bridge pin IN1 is tied to pwm7 pin A (P8_19). Alternatively it can be used as GPIO0_22 = 22 (GPIO SW-number)
H-bridge pin IN2 is tied to pwm7 pin B (P8_13). Alternatively it can be used as GPIO0_23 = 23 (GPIO SW-number)
H-bridge pin DIS is tied to GPIO2_12 (P8_39) = 76 (GPIO SW-number).
H-bridge pin ENBL is tied to GPIO2_15 (P8_38) = 79 (GPIO SW-number).
*/

/*
Struct: pwmDescriptor
pwmNum: Number of pwm to be used. On this board pwm is no 7
pinNameA: pin assigned to output A of pwm. For pwm7 it is "P8_19_pinmux" (C string)
pinNameB: pin assigned to output B of pwm. For pwm7 it is "P8_13_pinmux" (C string)
*/
typedef struct {
	unsigned char pwmNum;
	char pinNameA [MAX_FILENAME];
	char pinNameB [MAX_FILENAME];
} pwmDescriptor;

/*
Function: pwmSetPinmux_A or _B
Parameter: a struct of type pwmDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Sets the pinmux of the corresponding IO pin (A or B) to pwm.
The pinmux information has to be supplied as string in the struct pwm.
*/
int pwmSetPinmux_A(pwmDescriptor* pwm);
int pwmSetPinmux_B(pwmDescriptor* pwm);

/*
Function: pwmOpen_A or _B
Parameter: a struct of type pwmDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Initializes one of the pwm channels (A or B) of this pwm.
This shall not be done, if the channels have been initialized before!
Hence have to check, if the directory ...pwm-7:0 or ...pwm-7:1 exists
The sleep(1) is needed in order to allow the device to initialize
*/
int pwmOpen_A(pwmDescriptor* pwm);
int pwmOpen_B(pwmDescriptor* pwm);

/*
Function: pwmSetPeriod_A or _B
Parameter: a struct of type pwmDescriptor, and period (in ns)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the period of one of the pwm channels (A or B), value is in ns.
*/
int pwmSetPeriod_A(pwmDescriptor *pwm, int period);
int pwmSetPeriod_B(pwmDescriptor* pwm, int period);

/*
Function: pwmSetDuty_A or _B
Parameter: a struct of type pwmDescriptor, and duty cycle (in ns)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the duty cycle of one of the pwm channels (A or B), value is in ns.
*/
int pwmSetDuty_A(pwmDescriptor *pwm, int duty);
int pwmSetDuty_B(pwmDescriptor* pwm, int duty);

/*
Function: pwmSetPolarity_A or _B
Parameter: a struct of type pwmDescriptor, and polarity (0 or 1)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the polarity of one of the pwm channels (A or B), 0 = normal, 1 = inversed
Polarity must be written after the period is initialized, otherwise an error occurs!
*/
int pwmSetPolarity_A(pwmDescriptor* pwm, int polarity);
int pwmSetPolarity_B(pwmDescriptor* pwm, int polarity);

/*
Function: pwmSetEnable_A or _B
Parameter: a struct of type pwmDescriptor, and enable (0 or 1)
Returns: 0 on succes, -1 on error, including error output
Does: Enables (if 1) or disables (if 0) one of the pwm channels (A or B) of this pwm.
*/
int pwmSetEnable_A(pwmDescriptor* pwm, unsigned char enable);
int pwmSetEnable_B(pwmDescriptor* pwm, unsigned char enable);

#endif /* PWM_H_ */
