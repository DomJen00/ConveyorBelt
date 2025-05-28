#include "ConveyorMotor.h"

// Constructor
ConveyorMotor::ConveyorMotor() {
	m_targetRPM = 0;
	m_actualRPM = 0;
	m_motorRunning = false;

	setupHBridge();
	setupEncoder();	
	piRegler_initialize();
}

ConveyorMotor::~ConveyorMotor() {
	stopMotor();
}

void ConveyorMotor::setupEncoder() {
	// QEP setup (Encoder)
	strcpy(m_qep.qepNum, "48302000.epwmss/48302180.eqep");
	strcpy(m_qep.pinNameA, "P8_35_pinmux");
	strcpy(m_qep.pinNameB, "P8_33_pinmux");
	strcpy(m_qep.pinNameIndex, "P8_31_pinmux");

	if (qepSetPinmux(&m_qep) < 0) { 
		cerr << "qepSetPinmux failed" << endl;
	}

	if (qepSetMode(&m_qep, QEP_MODE_RELATIVE) < 0) {
		cerr << "qepSetMode failed" << endl;
	}
		
	if (qepSetPeriod(&m_qep, QEP_PERIOD_NS) < 0) {
		cerr << "qepSetPeriod failed" << endl;
	}

	cout << "Encoder initialised" << endl;
}

void ConveyorMotor::setupHBridge() {
	// GPIO setup (DIS, ENBL, IN1)
	m_gpioDIS.gpioNum = GPIO_NUM_DIS;
	strcpy(m_gpioDIS.direction, "out");

	m_gpioENBL.gpioNum = GPIO_NUM_ENBL;
	strcpy(m_gpioENBL.direction, "out");

	m_gpioIN1.gpioNum = GPIO_NUM_IN1;
	strcpy(m_gpioIN1.direction, "out");

	if (gpioOpen(&m_gpioDIS) < 0 || gpioOpen(&m_gpioENBL) < 0 || gpioOpen(&m_gpioIN1) < 0) {
		cerr << "Failed to open H-Bridge GPIOs" << endl;
	}
	if (gpioSetValue(&m_gpioDIS, 0) < 0) {
		cerr << "Set GPIO_DIS level failed" << endl;
	}
	if (gpioSetValue(&m_gpioENBL, 1) < 0) {
		cerr << "Set GPIO_ENBL level failed" << endl;
	}	
	if (gpioSetValue(&m_gpioIN1, CW) < 0) {
		cerr << "Set GPIO_IN1 failed" << endl;
	}

	// PWM setup (IN2)
	m_pwm.pwmNum = 7;
	strcpy(m_pwm.pinNameB, "P8_13_pinmux");

	if (pwmOpen_B(&m_pwm) < 0) {
		cerr << "pwmOpen failed" << endl;
	}
	if (pwmSetPinmux_B(&m_pwm) < 0) {
		cerr << "pwmSetPinmux failed" << endl;
	}
	if (pwmSetPeriod_B(&m_pwm, PWM_PERIOD_NS) < 0) {
		cerr << "pwmSetPeriod failed" << endl;
	}
	if (pwmSetDuty_B(&m_pwm, 0) < 0) {
		cerr << "pwmSetPeriod failed" << endl;
	}
	if (pwmSetPolarity_B(&m_pwm, PWM_POLARITY_NORMAL) < 0) {
		cerr << "pwmSetPolarity failed" << endl;
	}
	if (pwmSetEnable_B(&m_pwm, PWM_ENABLE) < 0) {
		cerr << "pwmSetEnable failed" << endl;
	}

	// SPI setup (H-Bridge)
	strcpy(m_spi.pinmux.CSpin, "P9_28_pinmux");
	strcpy(m_spi.pinmux.MISOpin, "P9_29_pinmux");
	strcpy(m_spi.pinmux.MOSIpin, "P9_30_pinmux");
	strcpy(m_spi.pinmux.SCLKpin, "P9_31_pinmux");
	m_spi.spiNum = SPI_NUM_1;
	m_spi.bitsPerWord = SPI_BITS_PER_WORD;
	m_spi.mode = SPI_MODE_1;
	m_spi.speed = SPI_SPEED;
	m_spi.flags = O_RDWR;

	if (spiSetPinmux(&m_spi) < 0) {
		cerr << "spiSetPinmux failed" << endl;
	}
	if (spiOpen(&m_spi) < 0) {
		cerr << "spiOpen failed" << endl;
	}
	
	spiXfer16Bits(&m_spi, WRITE_CONFIG_WORD);

	cout << "H-bridge initialised" << endl;
}

void ConveyorMotor::controlLoop() {
	while (m_motorRunning) {
		int rpm = getSpeedRPM();

		if (rpm < 0) {
			rtU.istWert = (-1) * rpm;
		}
		else {
			rtU.istWert = rpm;
		}

		rtU.sollWert = m_targetRPM;

		piRegler_step();

		double duty = rtY.y * PWM_SCALING_FACTOR;

		if (duty >= PWM_PERIOD_NS) {
			duty = PWM_PERIOD_NS;
		}

		// Setting speed of the motor
		if (pwmSetDuty_B(&m_pwm, static_cast<int>(duty)) < 0) {
			cerr << "pwmSetDuty_B in moveMotor failed" << endl;
		}

		usleep(50000);
	}
}

void* ConveyorMotor::controlLoopThread(void* arg) {
	static_cast<ConveyorMotor*>(arg)->controlLoop();
	return nullptr;
}

void ConveyorMotor::moveMotor(int rpm) {
	int direction = CW;

	if (rpm < 0) {
		direction = CCW;
		m_targetRPM = (- 1) * rpm;
	}

	m_targetRPM = rpm;

	// Setting direction of the motor
	if (gpioSetValue(&m_gpioIN1, direction) < 0) {
		cerr << "gpioSetValue in moveMotor failed" << endl;
	}

	if (!m_motorRunning) {
		gpioSetValue(&m_gpioDIS, 0); // Enable motor

		if (pthread_create(&m_controlThread, nullptr, controlLoopThread, this) != 0) {
			perror("Starting control thread failed.");
		}
		pthread_detach(m_controlThread);
		m_motorRunning = true;
	}
}

void ConveyorMotor::stopMotor() {
	if (m_motorRunning) {
		pwmSetDuty_B(&m_pwm, 0);
		gpioSetValue(&m_gpioDIS, 1);			
		m_motorRunning = false;

		//pthread_join(controlThread, NULL);
	}	
}

bool ConveyorMotor::isRunning() {
	return m_motorRunning;
}

int ConveyorMotor::getSpeedRPM() {
	long encoderSteps = qepGetPosition(&m_qep);
	double stepsPerSecond = static_cast<double>(encoderSteps) / (QEP_PERIOD_NS * 1e-9);
	double rpm = (stepsPerSecond / QEP_RESOLUTION) * 60.0;
	m_actualRPM = static_cast<int>(rpm);

	return m_actualRPM;
}