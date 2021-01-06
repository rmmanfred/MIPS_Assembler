//BinFunc.c
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
#include "BinFunc.h"
/**
 * Static table of r format and similar instructions
 */
static const BinFunc r[8] = {{"mul", "011100", "000010"},
{"sub", "000000", "100010"}, {"nor", "000000", "100111"}, 
{"add", "000000", "100000"}, {"addu", "000000", "100001"}, 
{"slt", "000000", "101010"}, {"srav", "000000", "000111"}, 
{"move", "000000", "100001"}};

/**
 * Static table of pseudo instructions
 * must be converted properly later
 * MAY REMOVE "MOVE"
 */
static const BinFunc pseudo[2] = {{"blt", "000000", "101010"}, 
{"la", "001000", NULL}};

/**
 * Static table of instructions using immediates
 */
static const BinFunc i[7] = {{"addi", "001000", NULL}, 
{"lui", "001111", NULL}, {"lw", "100011", NULL}, 
{"slti", "001010", NULL}, {"addiu", "001001", NULL}, 
{"sw", "101011", NULL}, {"li", "001001", NULL}};

/**
 * Static table of special instructions not easily handled
 */
static const BinFunc spec[9] = {{"mult", "000000", "011000"}, 
{"syscall", "000000", "001100"}, {"beq", "000100", NULL}, 
{"bne", "000101", NULL}, {"sra", "000000", "000011"}, 
{"sll", "000000", "000000"}, {"blez", "000110", NULL}, 
{"bgtz", "000111", NULL}, {"j", "000010", NULL}};

/**
 * Pre-Condition: name is valid string, elem is proper BinFunc pointer
 * Post-Condition: elem will be filled with object corresponding to name
 * Searches table for provided name and returns true if found, filling 
 * pointer with value
 */
const bool r_Find(char* name, BinFunc* elem)
{
	for (int i = 0; i < (sizeof(r)/sizeof(BinFunc)); i++)
	{
		if (strcmp(r[i].Name, name) == 0)
		{
			memcpy(elem, &r[i], sizeof(BinFunc));
			return true;
		}
	}
	return false;
}

/**
 * Pre-Condition: name is valid string, elem is proper BinFunc pointer
 * Post-Condition: elem will be filled with object corresponding to name
 * Searches table for provided name and returns true if found, filling 
 * pointer with value
 */
const bool i_Find(char* name, BinFunc* elem)
{
	for (int n = 0; n < (sizeof(i)/sizeof(BinFunc)); n++)
	{
		if (strcmp(i[n].Name, name) == 0)
		{
			memcpy(elem, &i[n], sizeof(BinFunc));
			//elem = &i[n];
			return true;
		}
	}
	return false;
}

/**
 * Pre-Condition: name is valid string, elem is proper BinFunc pointer
 * Post-Condition: elem will be filled with object corresponding to name
 * Searches table for provided name and returns true if found, filling 
 * pointer with value
 */
const bool spec_Find(char* name, BinFunc* elem)
{
	for (int i = 0; i < (sizeof(spec)/sizeof(BinFunc)); i++)
	{
		if (strcmp(spec[i].Name, name) == 0)
		{
			memcpy(elem, &spec[i], sizeof(BinFunc));
			return true;
		}
	}
	return false;
}

/**
 * Pre-Condition: name is valid string, elem is proper BinFunc pointer
 * Post-Condition: elem will be filled with object corresponding to name
 * Searches table for provided name and returns true if found, filling 
 * pointer with value
 */
const bool pseudo_Find(char* name, BinFunc* elem)
{
	for (int i = 0; i < (sizeof(pseudo)/sizeof(BinFunc)); i++)
	{
		if (strcmp(pseudo[i].Name, name) == 0)
		{
			memcpy(elem, &pseudo[i], sizeof(BinFunc));
			return true;
		}
	}
	return false;
}
