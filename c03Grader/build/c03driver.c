//c03driver.c
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
#include "ParseResult.h"
/**
 * Main function executed on command line
 * Processes input file for ASM commands and prints
 * to output files
 */
int main(int argc, char** argv)
{
	if (argc == 3)
	{
		processFiles(argv[1], argv[2]);
	}
	else if (argc == 4)
	{
		processSymbols(argv[1], argv[2]);
	}
	else
	{
		printf("Wrong numer of arguments: %d", argc);
	}
}
