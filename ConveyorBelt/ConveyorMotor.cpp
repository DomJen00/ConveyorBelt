#include <iostream>
#include <fcntl.h>
#include <cstring>
#include "spi.h"
#include "qep.h"
#include "gpio.h"
#include "pwm.h"
#include "ConveyorMotor.h"

using namespace std;

// Constructor
ConveyorMotor::ConveyorMotor() {
	_targetRPM = 0;
	_actualRPM = 0;
	_motorRunning = false;

	// GPIO setup (DIS, ENBL)
	_gpioDIS.gpioNum = 76;
	strcpy(_gpioDIS.direction, "out");
	_gpioENBL.gpioNum = 79;
	strcpy(_gpioENBL.direction, "out");

	// PWM setup (IN1, IN2)
	_pwm.pwmNum = 7;
	strcpy(_pwm.pinNameA, "P8_19_pinmux");
	strcpy(_pwm.pinNameB, "P8_13_pinmux");

	// SPI setup (H-Bridge)
	_spi.spiNum = 2;
	_spi.bitsPerWord = 8;
	_spi.mode = 1;
	_spi.speed = 500000;
	_spi.flags = O_RDWR;
	strcpy(_spi.pinmux.CSpin, "P9_28_pinmux");
	strcpy(_spi.pinmux.MISOpin, "P9_29_pinmux");
	strcpy(_spi.pinmux.MOSIpin, "P9_30_pinmux");
	strcpy(_spi.pinmux.SCLKpin, "P9_31_pinmux");

	// QEP setup (Encoder)
	strcpy(_qep.qepNum, "48302000.epwmss/48302180.eqep");
	strcpy(_qep.pinNameA, "P8_35_pinmux");
	strcpy(_qep.pinNameB, "P8_33_pinmux");
	strcpy(_qep.pinNameIndex, "P8_31_pinmux");
}

bool ConveyorMotor::setupEncoder() {

}

bool ConveyorMotor::setupHBridge() {

}

void ConveyorMotor::moveProfile(int rpm) {

	int SpeedOffs = rpm / 50;
	if (rpm < 0) {
		// Direction is left
		SpeedOffs = SpeedOffs * (-1);
		//...Put Code in here...//
	}
	else {
		// Direction is right
		//...Put Code in here...//
	}

	startMotor();
	// Acceleration
	for (int i = 0; i < 50; i++) {
		_targetRPM = _targetRPM + SpeedOffs;
		//Sleep(20); ... Sleep for 20ms
		//...Put Code in here...//
	}

	// Move with constant speed for 8s
	//Sleep(8000)

	//Decceleration
	for (int i = 0; i < 50; i++) {
		_targetRPM = _targetRPM - SpeedOffs;
		//Sleep(20); 
		//...Put Code in here...//
	}
	_targetRPM = 0;
	stopMotor();
}

void ConveyorMotor::slowMove(int durationMS) {

	if (durationMS < 0) {
		printf("Duration has to be positive\n");
	}
	else {
		startMotor();
		//Sleep(durationMS); // Sleep for certain amount of time
		stopMotor();
	}
}

void ConveyorMotor::startMotor() {
	//...Put Code in here...//
	_motorRunning = true;
}

void ConveyorMotor::stopMotor() {
	//...Put Code in here...//
	_motorRunning = false;
}

int ConveyorMotor::getSpeed() {
	return _actualRPM;
}

bool ConveyorMotor::isRunning() {
	return _motorRunning;
}