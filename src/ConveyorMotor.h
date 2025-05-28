/*
File: ConveyorMotor.h
Author: DomJen00
As of: 2025-05-05
*/

#pragma once

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
extern "C" {
#include "../lib/spi.h"
#include "../lib/qep.h"
#include "../lib/gpio.h"
#include "../lib/pwm.h"
#include "../lib/piRegler.h"
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
#define QEP_PERIOD_NS			10000000.0 
#define QEP_RESOLUTION			2000.0

#define PWM_PERIOD_NS			50000.0
#define PWM_POLARITY_NORMAL		0
#define PWM_ENABLE				1
#define PWM_DISABLE				0
#define PWM_SCALING_FACTOR		PWM_PERIOD_NS / 7.0		// PWM_PERIOD_NS / rtb_Saturation of PI control 

#define SPI_NUM_1				2
#define SPI_BITS_PER_WORD		16
#define SPI_MODE_1				1
#define SPI_SPEED				1000000
#define WRITE_CONFIG_WORD		0b1110000000011000 
#define READ_WORD				0b0110001111111000

class ConveyorMotor {
private:
	int m_targetRPM;
	int m_actualRPM;
	bool m_motorRunning;

	gpioDescriptor m_gpioDIS;
	gpioDescriptor m_gpioENBL;
	gpioDescriptor m_gpioIN1;
	pwmDescriptor m_pwm;
	spiDescriptor m_spi;
	qepDescriptor m_qep;

	pthread_t m_controlThread;

	void setupEncoder();
	void setupHBridge();
	void controlLoop();	

	static void* controlLoopThread(void* arg);

public:
	ConveyorMotor();
	~ConveyorMotor();
	void moveMotor(int rpm);
	void stopMotor();	
	bool isRunning();
	int getSpeedRPM();
};