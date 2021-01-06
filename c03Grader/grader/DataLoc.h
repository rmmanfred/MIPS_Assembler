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
 * Represents relevant data scanned in by ParseResult but not valid
 * ASM instructions, namely variables declared in data section of asm
 * file and logical location labels used by branches and jumps in text
 * section
 * name: stores name of label
 * loc: global or local location of relevent data (line number or 
 * literal position)
 */
struct _DataLoc
{
	char* name;
	int loc;
};
typedef struct _DataLoc DataLoc;

/**
 * Pre-Condition: name is valid c string, location is a non-negative 
 * integer
 * Post-Condition: A new DataLoc object is allocated and filled with
 * parameters, pointer to this object is returned
 * Constructor function creating new DataLocs with provided name and 
 * location, returns pointer to creation
 */
DataLoc* create_DataLoc(char* name, int location);

/**
 * Pre-Condition: ptr is valid DataLoc pointer
 * Post-Condition: Allocated fields of ptr and ptr are freed
 * Clears allocated data from DataLoc pointer and then pointer itself
 */
void clear_DataLoc(DataLoc* ptr);
#endif
