/*
File: pwm.c
Author: HP
As of: 2919-10-17
Command line sequence which is followed:
•	sys/devices/platform/ocp/ocp:P8_19_pinmux# echo pwm > state
•	sys/devices/platform/ocp/ocp:P8_13_pinmux# echo pwm > state
•	sys/class/pwm/pwmchip7# echo 0 > export
•	sys/class/pwm/pwmchip7/pwm-7:0# echo 2000000 > period
•	sys/class/pwm/pwmchip7/pwm-7:0# echo 1000000 > duty_cycle
•	sys/class/pwm/pwmchip7/pwm-7:0# echo 1 > enable
•	sys/class/pwm/pwmchip7# echo 1 > export
•	sys/class/pwm/pwmchip7/pwm-7:1# echo 2000000 > period
•	sys/class/pwm/pwmchip7/pwm-7:1# echo 500000 > duty_cycle
•	sys/class/pwm/pwmchip7/pwm-7:1# echo inversed > polarity
•	sys/class/pwm/pwmchip7/pwm-7:1# echo 1 > enable
The values for period and duty_cycle are given in ns.
*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include "pwm.h"

/*
Function: pwmSetPinmux_A or _B
Parameter: a struct of type pwmDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Sets the pinmux of the corresponding IO pin (A or B) to pwm.
The pinmux information has to be supplied as string in the struct pwm.
*/
int pwmSetPinmux_A(pwmDescriptor* pwm) {
	char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "%s:%s/state", PINMUX_PATH, pwm->pinNameA);
	retVal = setParameter(filename, "pwm");
	if ( retVal < 0) {
		perror("In pwmSetPinmux");
		return (-1);
	}
	return 0;
}
int pwmSetPinmux_B(pwmDescriptor* pwm) {
	char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "%s:%s/state", PINMUX_PATH, pwm->pinNameB);
	retVal = setParameter(filename, "pwm");
	if (retVal < 0) {
		perror("In pwmSetPinmux");
		return (-1);
	}
	return 0;
}


/*
Function: pwmOpen_A or _B
Parameter: a struct of type pwmDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Initializes one of the pwm channels (A or B) of this pwm.
This shall not be done, if the channels have been initialized before!
Hence have to check, if the directory ...pwm-7:0 or ...pwm-7:1 exists
The sleep(1) is needed in order to allow the device to initialize
*/
int pwmOpen_A(pwmDescriptor *pwm) {
	char filename[MAX_FILENAME];
	int retVal;
	DIR* dir;

	sprintf(filename, "%s%d/pwm-%d:0", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	dir = opendir(filename);
	if (dir) {
		/* Directory exists. */
		closedir(dir);
	}
	else {
		/* Directory does not exist, have to initialize channel */
		sprintf(filename, "%s%d/export", PWM_PATH, pwm->pwmNum);
		retVal = setParameter(filename, "0");
		if (retVal < 0) {
			perror("In pwmOpen");
			return (-1);
		}
		sleep(1);
	}
	return 0;
}
int pwmOpen_B(pwmDescriptor* pwm) {
	char filename[MAX_FILENAME];
	int retVal;
	DIR* dir;

	sprintf(filename, "%s%d/pwm-%d:1", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	dir = opendir(filename);
	if (dir) {
		/* Directory exists. */
		closedir(dir);
	}
	else {
		/* Directory does not exist, have to initialize channel */
		sprintf(filename, "%s%d/export", PWM_PATH, pwm->pwmNum);
		retVal = setParameter(filename, "1");
		if (retVal < 0) {
			perror("In pwmOpen");
			return (-1);
		}
		sleep(1);
	}
	return 0;
}


/*
Function: pwmSetPeriod_A or _B
Parameter: a struct of type pwmDescriptor, and period (in ns)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the period of one of the pwm channels (A or B), value is in ns.
*/
int pwmSetPeriod_A(pwmDescriptor *pwm, int period) {
	char filename[MAX_FILENAME];
	char myBuf[13];
	int retVal;
	sprintf(myBuf, "%d", period);
	sprintf(filename, "%s%d/pwm-%d:0/period", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	retVal = setParameter(filename, myBuf);
	if ( retVal < 0) {
		perror("In pwmSetPeriod");
		return (-1);
	}
	return 0;
}
int pwmSetPeriod_B(pwmDescriptor* pwm, int period) {
	char filename[MAX_FILENAME];
	char myBuf[13];
	int retVal;
	sprintf(myBuf, "%d", period);
	sprintf(filename, "%s%d/pwm-%d:1/period", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	retVal = setParameter(filename, myBuf);
	if ( retVal < 0) {
		perror("In pwmSetPeriod");
		return (-1);
	}
	return 0;
}

/*
Function: pwmSetDuty_A or _B
Parameter: a struct of type pwmDescriptor, and duty cycle (in ns)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the duty cycle of one of the pwm channels (A or B), value is in ns.
*/
int pwmSetDuty_A(pwmDescriptor *pwm, int duty) {
	char filename[MAX_FILENAME];
	char myBuf[13];
	int retVal;
	sprintf(myBuf, "%d", duty);
	sprintf(filename, "%s%d/pwm-%d:0/duty_cycle", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	retVal = setParameter(filename, myBuf);
	if (retVal < 0) {
		perror("In pwmSetDuty");
		return (-1);
	}
	return 0;
}
int pwmSetDuty_B(pwmDescriptor* pwm, int duty) {
	char filename[MAX_FILENAME];
	char myBuf[13];
	int retVal;
	sprintf(myBuf, "%d", duty);
	sprintf(filename, "%s%d/pwm-%d:1/duty_cycle", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	retVal = setParameter(filename, myBuf);
	if (retVal < 0) {
		perror("In pwmSetDuty");
		return (-1);
	}
	return 0;
}


/*
Function: pwmSetPolarity_A or _B
Parameter: a struct of type pwmDescriptor, and polarity (0 or 1)
Returns: 0 on succes, -1 on error, including error output
Does: Sets the polarity of one of the pwm channels (A or B), 0 = normal, 1 = inversed
Polarity must be written after the period is initialized, otherwise an error occurs!
*/
int pwmSetPolarity_A(pwmDescriptor* pwm, int polarity) {
	char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "%s%d/pwm-%d:0/polarity", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	if (polarity == 1) {
		retVal = setParameter(filename, "inversed");
		if (retVal < 0) {
			perror("In pwmOpen");
			return (-1);
		}
	}
	else {
		retVal = setParameter(filename, "normal");
		if (retVal < 0) {
			perror("In pwmOpen");
			return (-1);
		}
	}
	return 0;
}
int pwmSetPolarity_B(pwmDescriptor* pwm, int polarity) {
	char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "%s%d/pwm-%d:1/polarity", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	if (polarity == 1) {
		retVal = setParameter(filename, "inversed");
		if (retVal < 0) {
			perror("In pwmOpen");
			return (-1);
		}
	}
	else {
		retVal = setParameter(filename, "normal");
		if (retVal < 0) {
			perror("In pwmOpen");
			return (-1);
		}
	}
	return 0;
}


/*
Function: pwmSetEnable_A or _B
Parameter: a struct of type pwmDescriptor, and enable (0 or 1)
Returns: 0 on succes, -1 on error, including error output
Does: Enables (if 1) or disables (if 0) one of the pwm channels (A or B) of this pwm.
*/
int pwmSetEnable_A(pwmDescriptor* pwm, unsigned char enable) {
	char filename[MAX_FILENAME];
	char myBuf[2];
	int retVal;
	sprintf(myBuf, "%d", enable);
	sprintf(filename, "%s%d/pwm-%d:0/enable", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	retVal = setParameter(filename, myBuf);
	if (retVal < 0) {
		perror("In pwmSetEnable");
		return (-1);
	}
	return 0;
}
int pwmSetEnable_B(pwmDescriptor* pwm, unsigned char enable) {
	char filename[MAX_FILENAME];
	char myBuf[2];
	int retVal;
	sprintf(myBuf, "%d", enable);
	sprintf(filename, "%s%d/pwm-%d:1/enable", PWM_PATH, pwm->pwmNum, pwm->pwmNum);
	retVal = setParameter(filename, myBuf);
	if (retVal < 0) {
		perror("In pwmSetEnable");
		return (-1);
	}
	return 0;
}


