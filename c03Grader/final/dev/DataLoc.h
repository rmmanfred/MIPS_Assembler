//DataLoc.h
//  On my honor: 
//  
//  - I have not discussed the C language code in my program with 
//    anyone other than my instructor or the teaching assistants  
//    assigned to this course. 
//  
//  - I have not used C language code obtained from another student,  
//    the Internet, or any other unauthorized source, either modified 
//    or unmodified.   
//  
//  - If any C language code or documentation used in my program  
//    was obtained from an authorized source, such as a text book or 
//    course notes, that has been clearly noted with a proper citation 
//    in the comments of my program. 
//  
//  - I have not designed this program in such a way as to defeat or 
//    interfere with the normal operation of the grading code. 
// 
//    <Ross Manfred> 
//    <Rmm26079>
#ifndef DATALOC_H
#define DATALOC_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/**
 * 
 */
struct _DataLoc
{
	char* name;
	int loc;
};
typedef struct _DataLoc DataLoc;
DataLoc* create_DataLoc(char* name, int location);
void clear_DataLoc(DataLoc* ptr);
#endif
