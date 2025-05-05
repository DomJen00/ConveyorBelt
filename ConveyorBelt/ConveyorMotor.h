/*
File: ConveyorMotor.h
Author: DomJen00
As of: 2025-05-05
*/

class ConveyorMotor {
private:
	int _targetRPM;
	int _actualRPM;
	bool _motorRunning;

	gpioDescriptor _gpioDIS;
	gpioDescriptor _gpioENBL;
	pwmDescriptor _pwm;
	spiDescriptor _spi;
	qepDescriptor _qep;
		
	bool setupEncoder();
	bool setupHBridge();

public:
	ConveyorMotor();

	void moveProfile(int rpm);
	void slowMove(int durationMS);
	void startMotor();
	void stopMotor();
	bool isRunning();
	int getSpeed();
};