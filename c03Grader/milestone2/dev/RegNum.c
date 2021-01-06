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
 * 
 */
static const RegNum reg[15] ={{"$zero", 0, "00000"}, {"$t0", 8, "01000"}, 
	{"$t1", 9, "01001"},	{"$t2", 10, "01010"}, {"$t3", 11, "01011"}, 
	{"$s0", 16, "10000"}, {"$s1", 17, "10001"}, {"$s2", 18, "10010"}, 
	{"$s3", 19, "10011"}, {"$s4", 20, "10100"}, {"$s5", 21, "10101"}, 
	{"$s6", 22, "10110"}, {"$s7", 23, "10111"}, {"$v0", 2, "00010"}, 
	{"$v1", 3, "00011"}};

/**
 * 
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
