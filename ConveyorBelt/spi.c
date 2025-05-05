/*
File: spi.c
Author: HP
As of: 2919-10-17
Command line sequence followed for pinmux:
•	sys/devices/platform/ocp/ocp:P9_17_pinmux# echo spi_cs > state
•	sys/devices/platform/ocp/ocp:P9_22_pinmux# echo spi_sclk > state
•	sys/devices/platform/ocp/ocp:P9_18_pinmux# echo spi > state
•	sys/devices/platform/ocp/ocp:P9_21_pinmux# echo spi > state
Then control of SPI features using the ioctrl function.
*/

#include "spi.h"
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include "spi.h"

/*
Function: spiSetPinmux
Parameter: a struct of type spiDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Sets the pinmux of the corresponding four IO pins to spi.
The pinmux information has to be supplied as strings in the struct spiPinmux.
*/
int spiSetPinmux(spiDescriptor* spi) {
	char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "%s:%s/state", PINMUX_PATH, spi->pinmux.CSpin);
	retVal = setParameter(filename, "spi_cs");
	if ( retVal < 0) {
		perror("In spiSetPinmux");
		return (-1);
	}
	sprintf(filename, "%s:%s/state", PINMUX_PATH, spi->pinmux.MISOpin);
	retVal = setParameter(filename, "spi");
	if ( retVal < 0) {
		perror("In spiSetPinmux");
		return (-1);
	}
	sprintf(filename, "%s:%s/state", PINMUX_PATH, spi->pinmux.MOSIpin);
	retVal = setParameter(filename, "spi");
	if ( retVal < 0) {
		perror("In spiSetPinmux");
		return (-1);
	}
	sprintf(filename, "%s:%s/state", PINMUX_PATH, spi->pinmux.SCLKpin);
	retVal = setParameter(filename, "spi_sclk");
	if ( retVal < 0) {
		perror("In spiSetPinmux");
		return (-1);
	}
	return 0;
}

/*
Function: spiOpen
Parameter: a struct of type spiDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Open the SPI file according to channel given in spiNum.
Sets parameters node, bitsPerWord, speed.
*/
int spiOpen(spiDescriptor* spi) {
    char filename[MAX_FILENAME];
	int retVal;
	sprintf(filename, "/dev/spidev%d.0", spi->spiNum);
    spi->fd = open(filename, spi->flags);
    if (spi->fd < 0) {
		perror("spiOpen: Can't open file");
		return -1;
    }
	retVal = ioctl(spi->fd, SPI_IOC_WR_MODE, &spi->mode);
	if (retVal < 0) {
       perror("spiOpen: Can't set SPI mode");
       return -1;
    }
	retVal = ioctl(spi->fd, SPI_IOC_WR_BITS_PER_WORD, &spi->bitsPerWord);
	if (retVal < 0) {
       perror("spiOpen: Can't set bits per word");
       return -1;
    }
	retVal = ioctl(spi->fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi->speed);
	if (retVal < 0) {
       perror("spiOpen: Can't set max speed");
       return -1;
    }
    return 0;
}

/*
Function: spiClose
Parameter: a struct of type spiDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Closes the SPI file again.
*/
int spiClose(spiDescriptor* spi) {
	int retVal;
    retVal= close(spi->fd);
	if (retVal < 0) {
		perror("spiClose");
		return -1;
	}
	return 0;
}

/*
Function: spiTransfer
Parameter: a struct of type spiDescriptor, location of data to be transmitted, location of data to be received, size of data
Returns: 0 on succes, -1 on error, including error output
Does: Sends and receves data over SPI
*/
int spiTransfer(spiDescriptor* spi, unsigned char tx[], unsigned char rx[], unsigned int length) {
	int retVal;
	struct spi_ioc_transfer transfer = {
		   .tx_buf = (unsigned long)tx,
		   .rx_buf = (unsigned long)rx,
		   .len = length,
		   .delay_usecs = 0,
		   .speed_hz = spi->speed,
		   .bits_per_word = spi->bitsPerWord,
   };
   // send the SPI message (all of the above fields, inc. buffers)
   retVal = ioctl(spi->fd, SPI_IOC_MESSAGE(1), &transfer);
   if (retVal < 0) {
      perror("SpiTransfer: SPI_IOC_MESSAGE Failed");
      return -1;
   }
   return 0;
}

/*
Function: spiXfer8Bits
Parameter: a struct of type spiDescriptor, value to be transmitted
Returns: data received over SPI
Does: Simplified function which calls spiTransfer to perform functionality
*/
unsigned char spiXfer8Bits(spiDescriptor* spi, unsigned char value) {
	int retVal;
	unsigned char sendData[1] = {};
	unsigned char receiveData[1] = {};
	unsigned char returnValue;

	sendData[0] = value;
	retVal = spiTransfer(spi, sendData, receiveData, sizeof(sendData));
	if (retVal < 0) {
		perror("Failed to update output.");
	}
	returnValue = receiveData[0];
	return returnValue;
}

/*
Function: spiXfer16Bits
Parameter: a struct of type spiDescriptor, value to be transmitted
Returns: data received over SPI
Does: Simplified function which calls spiTransfer to perform functionality
*/
unsigned short spiXfer16Bits(spiDescriptor* spi, unsigned short value) {
	int retVal;
	unsigned char sendData[2] = {};
	unsigned char receiveData[2] = {};
	unsigned short returnValue;

	sendData[0] = (unsigned char) ((value & 0xff00) >> 8);
	sendData[1] = (unsigned char) (value & 0x00ff);
	retVal = spiTransfer(spi, sendData, receiveData, sizeof(sendData));
	if (retVal < 0) {
		perror("Failed to update output.");
	}
	returnValue = (unsigned short) ((receiveData[0] << 8) + receiveData[1]);
	return returnValue;
}

