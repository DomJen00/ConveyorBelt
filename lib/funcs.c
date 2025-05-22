/*
File: funcs.c
Author: HP
As of: 2919-10-17
Functions used to acces files with ASCII text (write/read)
*/

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include "funcs.h"

/*
Function: setParameter
Parameter: a pointer to a string containing the filename and one to the string holding the parameter value
Returns: 0 on succes, -1 on error, including error output
Does: Opens the file given by filename and writes the parameter value given, then closes the file.
*/
int setParameter(char* filename, char* toWrite) { 
	FILE* myFd;
	int retVal;
	char myString[MAX_DATA_BUF];
	strcpy(myString, filename);
	myFd = fopen(myString, "w");
	if (myFd == NULL) {
		perror("fopen");
		return -1;
	}
	strcpy(myString, toWrite);
	retVal = fputs(myString, myFd);
	if (retVal < 0) {
		perror("fputs");
		return -1;
	}
	retVal = fclose(myFd);
	if (retVal < 0) {
		perror("fclose");
		return -1;
	}
	return 0;
}

/*
Function: getParameter
Parameter: a pointer to a string containing the filename and one to the string to hold the parameter value
Returns: 0 on succes, -1 on error, including error output
Does: Opens the file given by filename and reads the parameter from the location given, then closes the file.
*/
int getParameter(char* filename, char* toRead) {
	FILE* myFd;
	int retVal;
	char* retString;
	myFd = fopen(filename, "r");
	if (myFd == NULL) {
		perror("fopen");
		return -1;
	}
	retString = fgets(toRead, MAX_DATA_BUF, myFd);
	if (retString == NULL) {
		perror("gpio/get-value: fgets");
		return -1;
	}
	retVal = fclose(myFd);
	if (retVal < 0) {
		perror("fclose");
		return -1;
	}
	return 0;
}
