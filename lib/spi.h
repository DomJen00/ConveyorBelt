/*
File: spi.h
Author: HP
As of: 2919-10-17
*/

#ifndef SPI_H_
#define SPI_H_

#include "funcs.h"

#define PINMUX_PATH "/sys/devices/platform/ocp/ocp"

/*
Struct: spiPinmux
CSpin: pin assigned to SPI CS output. spi0: "P9_17_pinmux", spi1: "P9_28_pinmux" (C strings)
MISOpin: pin assigned to SPI MISO input. spi0: "P9_21_pinmux", spi1: "P9_29_pinmux" (C strings)
MOSIpin: pin assigned to SPI MOSI output. spi0: "P9_18_pinmux", spi1: "P9_30_pinmux" (C strings)
SCLKpin: pin assigned to SPI SCLK output. spi0: "P9_22_pinmux", spi1: "P9_31_pinmux" (C strings)
*/
typedef struct {
	char CSpin [MAX_FILENAME];
	char MISOpin [MAX_FILENAME];
	char MOSIpin [MAX_FILENAME];
	char SCLKpin [MAX_FILENAME];
} spiPinmux;

/*
Struct: spiDescriptor
spiNum: Number of spi to be used. MUST BE +1, e.g. for spi0: spiNum=1, for spi1: spiNum=2
        On this board spi0 is connected to the ADC, spi1 to the H-Bridge
bitsPerWord: is used to hold the bits per word size of SPI. Usually 8
mode: is used to hold the mode of SPI (clock edge and clock polarity)
      For spi0: mode=0, for spi1: mode=1
speed: is used to hold the speed of SPI (bit per second)
flags: mode of operation. In both caeses O_RDWR is needed.
*/
typedef struct {
	int fd;
	unsigned char spiNum; /* is used to hold the channel number of SPI, MUST BE +1, e.g. for spi0: 1, for spi1: 2 */
	unsigned char bitsPerWord; /* is used to hold the bits per word size of SPI */
	unsigned char mode; /* is used to hold the mode of SPI */
	unsigned int speed; /* is used to hold the speed of SPI */
	unsigned char flags;
	spiPinmux pinmux;
} spiDescriptor;

/*
Function: spiSetPinmux
Parameter: a struct of type spiDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Sets the pinmux of the corresponding four IO pins to spi.
The pinmux information has to be supplied as strings in the struct spiPinmux.
*/
int spiSetPinmux(spiDescriptor* spi);

/*
Function: spiOpen
Parameter: a struct of type spiDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Open the SPI file according to channel given in spiNum.
Sets parameters node, bitsPerWord, speed.
*/
int spiOpen(spiDescriptor* spi);

/*
Function: spiClose
Parameter: a struct of type spiDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Closes the SPI file again.
*/
int spiClose(spiDescriptor *spi);

/*
Function: spiTransfer
Parameter: a struct of type spiDescriptor, location of data to be transmitted, location of data to be received, size of data
Returns: 0 on succes, -1 on error, including error output
Does: Sends and receves data over SPI
*/
int spiTransfer(spiDescriptor* spi, unsigned char tx[], unsigned char rx[], unsigned int length);

/*
Function: spiXfer8Bits
Parameter: a struct of type spiDescriptor, value to be transmitted
Returns: data received over SPI
Does: Simplified function which calls spiTransfer to perform functionality
*/
unsigned char spiXfer8Bits(spiDescriptor* spi, unsigned char value);

/*
Function: spiXfer16Bits
Parameter: a struct of type spiDescriptor, value to be transmitted
Returns: data received over SPI
Does: Simplified function which calls spiTransfer to perform functionality
*/
unsigned short spiXfer16Bits(spiDescriptor* spi, unsigned short value);

#endif /* SPI_H_ */
