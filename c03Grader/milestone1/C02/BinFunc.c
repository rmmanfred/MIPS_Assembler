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
 * 
 */
static const BinFunc r[4] = {{"mul", "011100", "000010"},
{"sub", "000000", "100010"}, {"nor", "000000", "100111"}, {"add", "000000", "100000"}};

/**
 * 
 */
static const BinFunc i[4] = {{"addi", "001000", NULL}, 
{"lui", "001111", NULL}, {"lw", "100011", NULL}, 
{"slti", "001010", NULL}};

/**
 * 
 */
static const BinFunc spec[2] = {{"mult", "000000", "011000"}, 
{"syscall", "000000", "001100"}};

/**
 * 
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
 * 
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
 * 
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
