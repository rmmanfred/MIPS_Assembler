//BinFunc.h
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
#ifndef BINFUNC_H
#define BINFUNC_H
#include <stddef.h>
#include <string.h>
#include <stdbool.h>
/**
 * Represents relevant binary data associated with an ASM instruction
 * Mnemonic. Namely that instruction's associated binary opcode and 
 * funct field
 * Name: instruction mnemonic
 * op: binary opcode
 * func: binary function field
 */
struct _BinFunc
{
	char* Name;
	char* op;
	char* func;
};
typedef struct _BinFunc BinFunc;

/**
 * Pre-Condition: name is valid string, elem is proper BinFunc pointer
 * Post-Condition: elem will be filled with object corresponding to name
 * Searches table for provided name and returns true if found, filling 
 * pointer with value
 */
const bool r_Find(char* name, BinFunc* elem);

/**
 * Pre-Condition: name is valid string, elem is proper BinFunc pointer
 * Post-Condition: elem will be filled with object corresponding to name
 * Searches table for provided name and returns true if found, filling 
 * pointer with value
 */
const bool i_Find(char* name, BinFunc* elem);

/**
 * Pre-Condition: name is valid string, elem is proper BinFunc pointer
 * Post-Condition: elem will be filled with object corresponding to name
 * Searches table for provided name and returns true if found, filling 
 * pointer with value
 */
const bool spec_Find(char* name, BinFunc* elem);
#endif
