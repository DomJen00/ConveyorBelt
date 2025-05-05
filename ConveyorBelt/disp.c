/*
File: disp.c
Author: HP
As of: 2919-10-17
*/

#include "disp.h"
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

/*
Function: openDisplay
Parameter: a struct of type dispDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Opens a file with the specified name in the home directory
of the user debian (/home/debian)
*/
int openDisplay(dispDescriptor* disp) {
	char myFilename[MAX_FILENAME];
	sprintf(myFilename, "../../../../../%s", disp->fileName);
	disp->myFd = fopen(myFilename, "w");
	if (disp->myFd == NULL) {
		perror("fopen");
		return -1;
	}
	return 0;
}

/*
Function: clearDisplay
Parameter: a struct of type dispDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Clears the display for displaying new info
*/
int clearDisplay(dispDescriptor* disp) {
	int retVal;
	retVal = fputs("\f", disp->myFd);
	if (retVal < 0) {
		perror("fputs");
		return -1;
	}
	retVal = fflush(disp->myFd);
	if (retVal < 0) {
		perror("fputs");
		return -1;
	}
	return 0;
}

/*
Function: writeLineToDisplay
Parameter: a struct of type dispDescriptor and a pointer to the string to be written
Returns: 0 on succes, -1 on error, including error output
Does: Writes the given string and moves to the next line
The maximum lenght is given by MAX_CONS_LEN
*/
int writeLineToDisplay(dispDescriptor* disp, char * myString) {
	int retVal;
	char myBuf[MAX_CONS_LEN];
	sprintf(myBuf, "%s\r\n", myString);
	retVal = fputs(myBuf, disp->myFd);
	if (retVal < 0) {
		perror("fputs");
		return -1;
	}
	retVal = fflush(disp->myFd);
	if (retVal < 0) {
		perror("fputs");
		return -1;
	}
	return 0;
}

/*
Function: closeDisplay
Parameter: a struct of type dispDescriptor
Returns: 0 on succes, -1 on error, including error output
Does: Closes the file "dispsoleFile.txt"
*/
int closeDisplay(dispDescriptor* disp) {
	int retVal;
	retVal = fclose(disp->myFd);
	if (retVal < 0) {
		perror("fclose");
		return -1;
	}
	return 0;
}
