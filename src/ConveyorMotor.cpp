#include "ConveyorMotor.h"

// Constructor
ConveyorMotor::ConveyorMotor() {
	_targetRPM = 0;
	_actualRPM = 0;
	_motorRunning = false;

	setupHBridge();
	setupEncoder();	
}

ConveyorMotor::~ConveyorMotor() {
	stopMotor();
}

void ConveyorMotor::setupEncoder() {
	// QEP setup (Encoder)
	strcpy(_qep.qepNum, "48302000.epwmss/48302180.eqep");
	strcpy(_qep.pinNameA, "P8_35_pinmux");
	strcpy(_qep.pinNameB, "P8_33_pinmux");
	strcpy(_qep.pinNameIndex, "P8_31_pinmux");

	if (qepSetPinmux(&_qep) < 0) { 
		cerr << "qepSetPinmux failed" << endl;
	}

	if (qepSetMode(&_qep, QEP_MODE_RELATIVE) < 0) {
		cerr << "qepSetMode failed" << endl;
	}
		
	if (qepSetPeriod(&_qep, QEP_PERIOD_NS) < 0) {
		cerr << "qepSetPeriod failed" << endl;
	}

	cout << "Encoder initialised" << endl;
}

void ConveyorMotor::setupHBridge() {
	// GPIO setup (DIS, ENBL, IN1)
	_gpioDIS.gpioNum = GPIO_NUM_DIS;
	strcpy(_gpioDIS.direction, "out");

	_gpioENBL.gpioNum = GPIO_NUM_ENBL;
	strcpy(_gpioENBL.direction, "out");

	_gpioIN1.gpioNum = GPIO_NUM_IN1;
	strcpy(_gpioIN1.direction, "out");

	if (gpioOpen(&_gpioDIS) < 0 || gpioOpen(&_gpioENBL) < 0 || gpioOpen(&_gpioIN1) < 0) {
		cerr << "Failed to open H-Bridge GPIOs" << endl;
	}
	if (gpioSetValue(&_gpioDIS, 0) < 0) {
		cerr << "Set GPIO_DIS level failed" << endl;
	}
	if (gpioSetValue(&_gpioENBL, 1) < 0) {
		cerr << "Set GPIO_ENBL level failed" << endl;
	}	
	if (gpioSetValue(&_gpioIN1, CW) < 0) {
		cerr << "Set GPIO_IN1 failed" << endl;
	}

	// PWM setup (IN2)
	_pwm.pwmNum = 7;
	strcpy(_pwm.pinNameB, "P8_13_pinmux");

	if (pwmOpen_B(&_pwm) < 0) {
		cerr << "pwmOpen failed" << endl;
	}
	if (pwmSetPinmux_B(&_pwm) < 0) {
		cerr << "pwmSetPinmux failed" << endl;
	}
	if (pwmSetPeriod_B(&_pwm, PWM_PERIOD_NS) < 0) {
		cerr << "pwmSetPeriod failed" << endl;
	}
	if (pwmSetDuty_B(&_pwm, 0) < 0) {
		cerr << "pwmSetPeriod failed" << endl;
	}
	if (pwmSetPolarity_B(&_pwm, PWM_POLARITY_NORMAL) < 0) {
		cerr << "pwmSetPolarity failed" << endl;
	}
	if (pwmSetEnable_B(&_pwm, PWM_ENABLE) < 0) {
		cerr << "pwmSetEnable failed" << endl;
	}

	// SPI setup (H-Bridge)
	strcpy(_spi.pinmux.CSpin, "P9_28_pinmux");
	strcpy(_spi.pinmux.MISOpin, "P9_29_pinmux");
	strcpy(_spi.pinmux.MOSIpin, "P9_30_pinmux");
	strcpy(_spi.pinmux.SCLKpin, "P9_31_pinmux");
	_spi.spiNum = SPI_NUM_1;
	_spi.bitsPerWord = SPI_BITS_PER_WORD;
	_spi.mode = SPI_MODE_1;
	_spi.speed = SPI_SPEED;
	_spi.flags = O_RDWR;

	if (spiSetPinmux(&_spi) < 0) {
		cerr << "spiSetPinmux failed" << endl;
	}
	if (spiOpen(&_spi) < 0) {
		cerr << "spiOpen failed" << endl;
	}
	
	spiXfer16Bits(&_spi, WRITE_CONFIG_WORD);

	cout << "H-bridge initialised" << endl;
}

void ConveyorMotor::enableMotor() {
	if (!_motorRunning) {
		gpioSetValue(&_gpioDIS, 0);
		_motorRunning = true;
	}	
}

int ConveyorMotor::rpmToPWM(int rpm) {
	const int dutyMin_ns = 21137;

	if (rpm < RPM_MIN) {
		return 0;
	}
	if (rpm >= RPM_MAX) {
		return PWM_PERIOD_NS;
	}

	// Linear Interpolation
	int duty = dutyMin_ns +  ((rpm - RPM_MIN) * (PWM_PERIOD_NS - dutyMin_ns)) / (RPM_MAX - RPM_MIN);
	return duty;
}

// Function for debugging
void ConveyorMotor::moveMotor(int speed) {
	// IN1 controls direction, IN2 controls PWM (Parallel Mode)

	int direction = CW;

	if (speed < 0) {
		direction = CCW;
		speed = speed * (-1);
	}

	// Setting direction of the motor
	if (gpioSetValue(&_gpioIN1, direction) < 0) {
		cerr << "gpioSetValue in moveMotor failed" << endl;
	}
	// Setting speed of the motor
	if (pwmSetDuty_B(&_pwm, rpmToPWM(speed)) < 0) {
		cerr << "pwmSetDuty_B in moveMotor failed" << endl;
	}

	enableMotor();
}

/*
* Thread machen
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

	enableMotor();
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
		enableMotor();
		//Sleep(durationMS); // Sleep for certain amount of time
		stopMotor();
	}
}
*/

void ConveyorMotor::stopMotor() {
	if (_motorRunning) {
		pwmSetDuty_B(&_pwm, 0);
		gpioSetValue(&_gpioDIS, 1);
		_motorRunning = false;
	}	
}

int ConveyorMotor::getSpeedRPM() {
	long encoderSteps = qepGetPosition(&_qep);
	double stepsPerSeconds = static_cast<double>(encoderSteps) / (static_cast<double>(QEP_RESOLUTION) * QEP_PERIOD_NS * 1e-9);
	_actualRPM = static_cast<int>(stepsPerSeconds * 60.0);
	return _actualRPM;
}

bool ConveyorMotor::isRunning() {
	return _motorRunning;
}