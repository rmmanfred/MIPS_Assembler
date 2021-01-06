//DataLoc.c
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
#include "DataLoc.h"

/**
 * Pre-Condition: name is valid c string, location is a non-negative 
 * integer
 * Post-Condition: A new DataLoc object is allocated and filled with
 * parameters, pointer to this object is returned
 * Constructor function creating new DataLocs with provided name and 
 * location, returns pointer to creation
 */
DataLoc* create_DataLoc(char* name, int location)
{
	DataLoc* obj = calloc(1, sizeof(DataLoc));
	obj->name = calloc(strlen(name)+1, sizeof(char));
	strcpy(obj->name, name);
	obj->loc = location;
	return obj;
}

/**
 * Pre-Condition: ptr is valid DataLoc pointer
 * Post-Condition: Allocated fields of ptr and ptr are freed
 * Clears allocated data from DataLoc pointer and then pointer itself
 */
void clear_DataLoc(DataLoc* ptr)
{
	free(ptr->name);
	free(ptr);
}
