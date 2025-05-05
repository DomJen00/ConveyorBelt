/*
File: kbd.c
Author: HP
As of: 2023-05-12
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gpio.h"
#include "kbd.h"

char kbdTable[3][16] = { {'1','2','3','F','4','5','6','E','7','8','9','D','A','0','B','C'},
						 {'C','B','0','A','D','9','8','7','E','6','5','4','F','3','2','1'},
						 {'1','4','7',' ','2','5','8','0','3','6','9','.','A','B','C','D'} };

/*
Function: initKeyboard
Parameter: a descriptor of type kbdDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Initializes the 4 inputs and the 4 outputs needed to scan the keyboard
Fills the struct kbdDescriptor with appropriate values
*/
int initKeyboard(kbdDescriptor* kbd) {
	int retVal;
	char kbdVer[3];
	kbd->kbdOut0.gpioNum = 66;
	// P8_7 = GPIO2_2: 32*2+2=66 - KBD-OUT-0
	strcpy(kbd->kbdOut0.direction, "out");
	retVal = gpioOpen(&(kbd->kbdOut0));
	if (retVal < 0) return (-1);
	gpioSetValue(&(kbd->kbdOut0), 1);
	kbd->kbdOut1.gpioNum = 67;
	// P8_8 = GPIO2_3: 32*2+3=67 - KBD-OUT-1
	strcpy(kbd->kbdOut1.direction, "out");
	retVal = gpioOpen(&(kbd->kbdOut1));
	if (retVal < 0) return (-1);
	gpioSetValue(&(kbd->kbdOut1), 1);
	kbd->kbdOut2.gpioNum = 69;
	// P8_9 = GPIO2_5: 32*2+5=69 - KBD-OUT-2
	strcpy(kbd->kbdOut2.direction, "out");
	retVal = gpioOpen(&(kbd->kbdOut2));
	if (retVal < 0) return (-1);
	gpioSetValue(&(kbd->kbdOut2), 1);
	kbd->kbdOut3.gpioNum = 68;
	// P8_10 = GPIO2_4: 32*2+2=68 - KBD-OUT-3
	strcpy(kbd->kbdOut3.direction, "out");
	retVal = gpioOpen(&(kbd->kbdOut3));
	if (retVal < 0) return (-1);
	gpioSetValue(&(kbd->kbdOut3), 1);
	kbd->kbdIn0.gpioNum = 45;
	// P8_11 = GPIO1_13: 32*1+13=45 - KBD-IN-0
	strcpy(kbd->kbdIn0.direction, "in");
	retVal = gpioOpen(&(kbd->kbdIn0));
	if (retVal < 0) return (-1);
	kbd->kbdIn1.gpioNum = 44;
	// P8_12 = GPIO1_12: 32*1+12=44 - KBD-IN-1
	strcpy(kbd->kbdIn1.direction, "in");
	retVal = gpioOpen(&(kbd->kbdIn1));
	if (retVal < 0) return (-1);
	kbd->kbdIn2.gpioNum = 47;
	// P8_15 = GPIO1_15: 32*1+15=47 - KBD-IN-2
	strcpy(kbd->kbdIn2.direction, "in");
	retVal = gpioOpen(&(kbd->kbdIn2));
	if (retVal < 0) return (-1);
	kbd->kbdIn3.gpioNum = 46;
	// P8_16 = GPIO1_14: 32*1+14=46 - KBD-IN-3
	strcpy(kbd->kbdIn3.direction, "in");
	retVal = gpioOpen(&(kbd->kbdIn3));
	if (retVal < 0) return (-1);
	getParameter("../../../../../kbdVer.cfg", kbdVer);
	kbd->version = kbdVer[0];
	return 0;
}

/*
Function: getKey
Parameter: a descriptor of type kbdDescriptor
Returns: ASCII of key pressed, or 0x00 if no key was pressed
Does: Scans the keyboard, by setting the putputs to 0 one after the other and reading the inputs
*/
char getKey(kbdDescriptor* kbd) {
	char key = 0;
	unsigned char i = 0;
	unsigned char j = kbd->version - '0';
	// Now set the pins to '0': KBD-OUT-0
	gpioSetValue(&(kbd->kbdOut0), 0);
	// Now read the inputs and decide which key was pressed
	if (gpioGetValue(&(kbd->kbdIn0)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn1)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn2)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn3)) == 0) key = kbdTable[j][i];
	i++;
	// Now set the pins to '0': KBD-OUT-1
	gpioSetValue(&(kbd->kbdOut0), 1);
	gpioSetValue(&(kbd->kbdOut1), 0);
	// Now read the inputs and decide which key was pressed
	if (gpioGetValue(&(kbd->kbdIn0)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn1)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn2)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn3)) == 0) key = kbdTable[j][i];
	i++;
	// Now set the pins to '0': KBD-OUT-2
	gpioSetValue(&(kbd->kbdOut1), 1);
	gpioSetValue(&(kbd->kbdOut2), 0);
	// Now read the inputs and decide which key was pressed
	if (gpioGetValue(&(kbd->kbdIn0)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn1)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn2)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn3)) == 0) key = kbdTable[j][i];
	i++;
	// Now set the pins to '0': KBD-OUT-3
	gpioSetValue(&(kbd->kbdOut2), 1);
	gpioSetValue(&(kbd->kbdOut3), 0);
	// Now read the inputs and decide which key was pressed
	if (gpioGetValue(&(kbd->kbdIn0)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn1)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn2)) == 0) key = kbdTable[j][i];
	i++;
	if (gpioGetValue(&(kbd->kbdIn3)) == 0) key = kbdTable[j][i];
	gpioSetValue(&(kbd->kbdOut3), 1);
	return key;
}
