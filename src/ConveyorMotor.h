/*
File: ConveyorMotor.h
Author: DomJen00
As of: 2025-05-05
*/

#pragma once

#include <iostream>
#include <fcntl.h>
#include <cstring>
extern "C" {
#include "../lib/spi.h"
#include "../lib/qep.h"
#include "../lib/gpio.h"
#include "../lib/pwm.h"
}

using namespace std;

enum Direction {
	CW = 0,
	CCW
};

#define GPIO_NUM_DIS			76
#define GPIO_NUM_ENBL			79
#define GPIO_NUM_IN1			22

#define QEP_MODE_RELATIVE		1
#define QEP_MODE_ABSOLUTE		0
#define QEP_PERIOD_NS			100000000	
#define QEP_RESOLUTION			500*4

#define PWM_PERIOD_NS			50000
#define PWM_POLARITY_NORMAL		0
#define PWM_ENABLE				1
#define PWM_DISABLE				0
#define RPM_MAX					3130
#define RPM_MIN					1000

#define SPI_NUM_1				2
#define SPI_BITS_PER_WORD		16
#define SPI_MODE_1				1
#define SPI_SPEED				1000000
#define WRITE_CONFIG_WORD		0b1110000000011000 
#define READ_WORD				0b0110001111111000

class ConveyorMotor {
private:
	int _targetRPM;
	int _actualRPM;
	bool _motorRunning;

	gpioDescriptor _gpioDIS;
	gpioDescriptor _gpioENBL;
	gpioDescriptor _gpioIN1;
	pwmDescriptor _pwm;
	spiDescriptor _spi;
	qepDescriptor _qep;
		
	void setupEncoder();
	void setupHBridge();
	void enableMotor();
	int rpmToPWM(int rpm);

public:
	ConveyorMotor();
	~ConveyorMotor();
	void moveMotor(int speed);
	void moveProfile(int rpm);
	void slowMove(int durationMS);
	void stopMotor();
	bool isRunning();	
	int getSpeedRPM();
};