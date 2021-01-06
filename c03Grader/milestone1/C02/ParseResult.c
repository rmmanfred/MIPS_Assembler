//ParseResult.c
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
#include <stdlib.h>

/***  Add include directives for here as needed.  ***/

static void condFree(void * ptr);
static void condPrint(FILE* Out, char* ptr);
static void filterComment(char* src, char* dest);
static char hexNum(int num);
static char* hexPos(int pos);
void processSymbols(char* in, char* out);

/**  Frees the dynamic content of a ParseResult object.
 * 
 *   Pre:  pPR points to a proper ParseResult object.
 *   Post: All of the dynamically-allocated arrays in *pPR have been
 *         deallocated; pointers are NULL, static fields are reset to
 *         default values.
 * 
 *   Comments:
 *     -  The function has no information about whether *pPR has been
 *        allocated dynamically, so it cannot risk attempting to 
 *        deallocate *pPR.
 *     -  The function is intended to provide the user with a simple
 *        way to free memory; the user may or may not reuse *pPR.  So,
 *        the function does set the pointers in *pPR to NULL.
 */
void clearResult(ParseResult* const pPR) {
   
   condFree(pPR->ASMInstruction);
   condFree(pPR->Mnemonic);
   condFree(pPR->rdName);
   condFree(pPR->rsName);
   condFree(pPR->rtName);
   condFree(pPR->Opcode);
   condFree(pPR->Funct);
   condFree(pPR->RD);
   condFree(pPR->RS);
   condFree(pPR->RT);
   condFree(pPR->IMM);

}

/**  Prints the contents of a ParseResult object.
 * 
 *   Pre:  Log is open on an output file.
 *         pPR points to a proper ParseResult object.
 */
void printResult(FILE* Log, const ParseResult* const pPR) {
   
      fprintf(Log, "%s\n", pPR->ASMInstruction);
      fprintf(Log, "   %s   %s\n", pPR->Opcode, pPR->Mnemonic);
      fprintf(Log, "   %2"PRIu8"   %s", pPR->rd, pPR->rdName);
      if ( pPR->RD != NULL ) {
			fprintf(Log, "   %s", pPR->RD);
		}
		fprintf(Log, "\n");
      fprintf(Log, "   %2"PRIu8"   %s", pPR->rs, pPR->rsName);
      if ( pPR->RS != NULL ) {
			fprintf(Log, "   %s", pPR->RS);
		}
		fprintf(Log, "\n");
      fprintf(Log, "   %2"PRIu8"   %s", pPR->rt, pPR->rtName);
      if ( pPR->RT != NULL ) {
			fprintf(Log, "   %s", pPR->RT);
		}
		fprintf(Log, "\n");
      fprintf(Log, "   %s\n", pPR->Funct);
      fprintf(Log, "   %"PRId16"\n", pPR->Imm);
      if ( pPR->IMM != NULL ) {
			fprintf(Log, "   %s", pPR->IMM);
		}
		fprintf(Log, "\n");
		fprintf(Log, "\n");
}

/**
 * Frees ptr if not Null
 */
static void condFree(void* ptr)
{
	if (ptr != NULL)
	{
		free(ptr);
	}
	ptr = NULL;
}

/**
 * 
 */
void printASM(FILE* Out, const ParseResult* const pPR)
{
	fprintf(Out, pPR->Opcode);
	condPrint(Out, pPR->RS);
	condPrint(Out, pPR->RT);
	condPrint(Out, pPR->RD);
	if (pPR->IMM != NULL)
	{
		condPrint(Out, pPR->IMM);
	}
	else
	{
		fprintf(Out, "00000");
	}
	condPrint(Out, pPR->Funct);
	fprintf(Out, "\n");
}

/**
 * 
 */
void condPrint(FILE* Out, char* ptr)
{
	if (ptr != NULL)
	{
		fprintf(Out, ptr);
	}
}
 
/**
 * 
 */
//Line 256 bytes max
void processFiles(char* in, char* out)
{
	FILE* In = fopen(in, "r");
	FILE* Out = fopen(out, "w");
	int lineLength = 256;
	char line[257] = "\0";
	char linex[257] = "\0";
	//need data section handling later
	bool relev = false;
	while (fgets(line, lineLength, In) != NULL)
	{
		char* key = calloc(lineLength + 1, sizeof(char));
		strcpy(key, line);
		key = realloc(key, (strlen(key)+1) * sizeof(char));
		if (relev && key[0] != '.' && key[0] != '\n' && 
		strchr(key, ':') == NULL)
		{
			filterComment(key, linex); //adjust
			key = realloc(key, (1+strlen(key)) * sizeof(char));
			ParseResult* elem = parseASM(key);
			printASM(Out, elem);
			clearResult(elem);
		}
		else if(strcmp(key, ".text\n")==0)
		{
			relev = true;
		}
		free(key);
	}
	fcloseall();
}

/**
 * 
 */
void processSymbols(char* in, char* out)
{
	FILE* In = fopen(in, "r");
	FILE* Out = fopen(out, "w");
	char line[257] = "\0";
	int pos = 0;
	bool giveup = true;
	while (fgets(line, 256, In) != NULL && !giveup)
	{
		//milestone 1, only has .text
		char* key = calloc(257, sizeof(char));
		realloc(key, (strlen(line)+1)*sizeof(char));
		if (strchr(key, ':') != NULL)
		{
			char* label = calloc(33, sizeof(char));
			sscanf(key, "%[^:\n]", label);
			label = realloc(label, (strlen(label)+1)*sizeof(char));
			char* add = hexPos(pos);
			fprintf(Out, "%s %s\n", add, label);
			free(add);
		}
		else if (key[0] != '.' && key[0] != '\n' && 
		(strchr(key, ':') == NULL || strchr(key, '#') != NULL))
		{
			pos += 4;
		}
	}
	if (giveup)
	{
		fprintf(Out, "----------------------\n");
	}
	fcloseall();
}

/**
 * 
 */
static void filterComment(char* src, char* dest)
{
	int n = -1;
	for (int i = 0; i < strlen(src); i++)
	{
		if (src[i] == '#')
		{
			n = i;
			break;
		}
	}
	if (n > -1)
	{
		strncpy(src, src, n+1);
		dest[n+1] = '\0';
	}
	else
	{
		strcpy(src, src);
	}
}

/**
 * 
 */
static char* hexPos(int pos)
{
	int* hex = calloc(8, sizeof(int));
	char* addr = calloc(10, sizeof(char));
	addr[0] = '0';
	addr[1] = 'x';
	for (int i = 7; pos >= 1; i--)
	{
		hex[i] = pos % 16;
		pos = pos/16;
	}
	for (int i = 0; i < 8; i++)
	{
		addr[i + 2] = hexNum(hex[i]);
	}
	free(hex);
	return addr;
}

/**
 * 
 */
static char hexNum(int num)
{
	if (num > 9)
	{
		return (char) 55 + num;
	}
	else
	{
		return (char) 48 + num;
	}
}
