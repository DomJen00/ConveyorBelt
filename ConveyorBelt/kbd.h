/*
File: kbd.h
Author: HP
As of: 2023-05-12
*/

#ifndef KBD_H_
#define KBD_H_

/*
Struct: kbdDescriptor
contains descriptors of kbdOut0, kbdOut1, kbdOut2, kbdOut3 kbdIn0, kbdIn1, kbdIn2, kbdIn3;
*/
typedef struct {
	unsigned char version;
	gpioDescriptor kbdOut0;
	gpioDescriptor kbdOut1; 
	gpioDescriptor kbdOut2; 
	gpioDescriptor kbdOut3; 
	gpioDescriptor kbdIn0;
	gpioDescriptor kbdIn1;
	gpioDescriptor kbdIn2;
	gpioDescriptor kbdIn3;
} kbdDescriptor;

/*
Function: initKeyboard
Parameter: a descriptor of type kbdDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Initializes the 4 inputs and the 4 outputs needed to scan the keyboard
Fills the struct kbdDescriptor with appropriate values
*/
int initKeyboard(kbdDescriptor* kbd);

/*
Function: getKey
Parameter: a descriptor of type kbdDescriptor
Returns: ASCII of key pressed, or 0x00 if no key was pressed
Does: Scans the keyboard, by setting the putputs to 0 one after the other and reading the inputs
*/
char getKey(kbdDescriptor* kbd);

#endif /* KBD_H_ */
