/*
File: funcs.h
Author: HP
As of: 2919-10-17
Functions used to acces files with ASCII text (write/read)
*/

#ifndef FUNCS_H_
#define FUNCS_H_

#define MAX_DATA_BUF 64
#define MAX_FILENAME 128

 /*
 Function: setParameter
 Parameter: a pointer to a string containing the filename and one to the string holding the parameter value
 Returns: 0 on succes, -1 on error, including error output
 Does: Opens the file given by filename and writes the parameter value given, then closes the file.
 */
int setParameter(char* filename, char* toWrite);

/*
Function: getParameter
Parameter: a pointer to a string containing the filename and one to the string to hold the parameter value
Returns: 0 on succes, -1 on error, including error output
Does: Opens the file given by filename and reads the parameter from the location given, then closes the file.
*/
int getParameter(char* filename, char* toRead);

#endif /* FUNCS_H_ */
