/*
File: disp.h
Author: HP
As of: 2919-10-17
*/

#ifndef CONS_H_
#define CONS_H_

#include "funcs.h"
#include <stdio.h>

#define MAX_CONS_LEN 80
#define MAX_CONS_HIGH 23

/* 
Operation information:
By using these functions the information is written into a file.
By default the file is written into the start directory of a remote shell, which is /home/debian
This file can be displayed by using the shell command "tail -f fileName"
*/

/*
Struct: dispDescriptor
myFd: here the file descriptor is stored, after the file is opened.
fileName: name of the file. Has to be supplied before opening the file.
*/
typedef struct {
	FILE* myFd;
	char fileName[MAX_FILENAME];
} dispDescriptor;

/*
Function: openDisplay
Parameter: a struct of type dispDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Opens a file named "..." (name given in the dispDescriptor) in the project directory
*/
int openDisplay(dispDescriptor* disp);

/*
Function: clearDisplay
Parameter: a struct of type dispDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Clears the display for displaying new info
*/
int clearDisplay(dispDescriptor* disp);

/*
Function: writeLineToDisplay
Parameter: a struct of type dispDescriptor and a pointer to the string to be written
Returns: 0 on succes, -1 on error, including error output
Does: Writes the given string and moves to the next line
The maximum lenght is given by MAX_CONS_LEN
*/
int writeLineToDisplay (dispDescriptor* disp, char * myString);

/*
Function: closeDisplay
Parameter: a struct of type dispDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Closes the file "dispsoleFile.txt"
*/
int closeDisplay(dispDescriptor* disp);

#endif /* CONS_H_ */
