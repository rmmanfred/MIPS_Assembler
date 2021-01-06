//RegNum.c
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
#include "RegNum.h"
/**
 * Lookup table represents registers used by this project with their 
 * integer and binary equivalent
 */
static const RegNum reg[28] ={{"$zero", 0, "00000"}, {"$t0", 8, "01000"}, 
	{"$t1", 9, "01001"}, {"$t2", 10, "01010"}, {"$t3", 11, "01011"},  
	{"$t4", 12, "01100"}, {"$t5", 13, "01101"}, {"$t6", 14, "01110"}, 
	{"$t7", 15, "01111"}, {"$t8", 24, "11000"}, {"$t9", 25, "11001"}, 
	{"$s0", 16, "10000"}, {"$s1", 17, "10001"}, {"$s2", 18, "10010"}, 
	{"$s3", 19, "10011"}, {"$s4", 20, "10100"}, {"$s5", 21, "10101"}, 
	{"$s6", 22, "10110"}, {"$s7", 23, "10111"}, {"$v0", 2, "00010"}, 
	{"$v1", 3, "00011"}, {"$k0", 26, "11010"}, {"$k1", 27, "11011"}, 
	{"$at", 1, "00001"}, {"$a0", 4, "00100"}, {"$a1", 5, "00101"}, 
	{"$a2", 6, "00110"}, {"$a3", 7, "00111"}};

/**
 * Pre-Condition: name is proper c string
 * Post-Condition: RegNum indicated by name is returned if it exists
 * Finder function which searches lookup table for name and gives
 * associated RegNum
 */
const RegNum* Find(char* name)
{
	for (int i = 0; i < (sizeof(reg)/sizeof(RegNum)); i++)
	{
		if (strcmp(reg[i].Name, name) == 0)
		{
			return &reg[i];
		}
	}
	RegNum null = {NULL, NULL, NULL};
	return &null;
}
