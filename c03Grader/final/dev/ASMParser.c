BinFunc.c                                                                                           0000644 0001750 0001750 00000004020 13650725155 012731  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //BinFunc.c
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
static const BinFunc spec[5] = {{"mult", "000000", "011000"}, 
{"syscall", "000000", "001100"}, {"la", "001000", NULL}, 
{"beq", "000100", NULL}, {"bne", "000101", NULL}};

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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                c03driver.c                                                                                         0000644 0001750 0001750 00000002221 13650645246 013211  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //c03driver.c
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
                                                                                                                                                                                                                                                                                                                                                                               DataLoc.c                                                                                           0000644 0001750 0001750 00000002154 13652071653 012721  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //DataLoc.c
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
 * 
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
 * 
 */
void clear_DataLoc(DataLoc* ptr)
{
	free(ptr->name);
	//free(ptr);
}
                                                                                                                                                                                                                                                                                                                                                                                                                    ParseResult.c                                                                                       0000644 0001750 0001750 00000025636 13652076200 013666  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //ParseResult.c
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
static void printData(FILE* Out, char* str);
static void fileAppend(FILE* Out, FILE* Temp);
static DataLoc* var_Find(char* name);
static DataLoc* jump_Find(char* name);
static void clearArrays();
DataLoc** var;
int vsize;
DataLoc** jump;
int jsize;
int dataPos;

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
   condFree(pPR->JUMP);
   condFree(pPR->FLAGS);
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
static void printASM(FILE* Out, const ParseResult* const pPR, int x)
{
	fprintf(Out, pPR->Opcode);
	condPrint(Out, pPR->RS);
	condPrint(Out, pPR->RT);
	condPrint(Out, pPR->RD);
	if (pPR->IMM != NULL)
	{
		condPrint(Out, pPR->IMM);
	}
	else if(pPR->FLAGS != NULL)
	{
		DataLoc* dt = var_Find(pPR->FLAGS);
		char* print = toBin(dt->loc, 16);
		fprintf(Out, "%s", print);
		free(print);
	}
	else if(pPR->JUMP != NULL)
	{
		DataLoc* dt = jump_Find(pPR->JUMP);
		char* print = toBin(((dt->loc) - x)-1, 16);
		fprintf(Out, "%s", print);
		free(print);
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
static void condPrint(FILE* Out, char* ptr)
{
	if (ptr != NULL)
	{
		fprintf(Out, ptr);
	}
}

/**
 * 
 */
void processFiles(char* in, char* out)
{
	FILE* In = fopen(in, "r");
	FILE* Out = fopen(out, "w");
	FILE* Tmp = tmpfile();
	var = calloc(1, sizeof(DataLoc*));
	vsize = 0;
	jsize = 0;
	jump = calloc(1, sizeof(DataLoc*));
	//FILE* Tmp = fopen("test.o", "w+");
	int lineLength = 256;
	char line[257] = "\0";
	char linex[257] = "\0";
	int n = 1;
	int offset = 8192;
	//need data section handling later
	int relev = 0;
    int text = 0;
	while (fgets(line, lineLength, In) != NULL)
	{
		char* key = calloc(lineLength + 1, sizeof(char));
		strcpy(key, line);
		key = realloc(key, (strlen(key)+1) * sizeof(char));
		if (relev==1)
		{
			if (strchr(key, ':') != NULL)
			{
				char* temp = calloc(257, sizeof(char));
				sscanf(key, "%[^:]:", temp);
				temp = realloc(temp, (strlen(temp)+1)*sizeof(char));
				if (jsize >= 1)
				{
					jump = realloc(jump, (jsize+1)*sizeof(DataLoc*));
				}
				jump[jsize] = create_DataLoc(temp, n);
				free(temp);
				jsize++;
			}
			else if (key[0] != '\n')
			{
				n++;
			}
		}
		else if(relev==2)
		{
			if (key[0] == '\n')
			{
				relev = 0;
			}
			else// if()
			{
				char* temp = calloc(257, sizeof(char));
				sscanf(key, "%[^:]:", temp);
				temp = realloc(temp, (strlen(temp)+1)*sizeof(char));
				if (vsize >= 1)
				{
					var = realloc(var, (vsize+1)*sizeof(DataLoc*));
				}
				var[vsize] = create_DataLoc(temp, offset + (dataPos*4));
				vsize++;
				char* type = strchr(key, '.');
				printData(Tmp, type);
				free(temp);
			}
		}
		else if(key[0] == '.')
		{
			if (strcmp(key, ".text\n")==0)
			{
				relev = 1;
				n = 1;
				offset = 0;
                text = (int)ftell(In);
			}
			else if (strcmp(key, ".data\n")==0)
			{
				relev = 2;
				dataPos = 0;
			}
		}
		free(key);
	}

    //Actually execute, return to text section
    fseek(In, text, SEEK_SET);
    int x = 1;
    while(fgets(line, lineLength, In) != NULL)
    {
        char* ky = calloc(lineLength + 1, sizeof(char));
		strcpy(ky, line);
		ky = realloc(ky, (strlen(ky)+1) * sizeof(char));
			if (strchr(ky, ':') == NULL && ky[0] != '.' && ky[0] != '\n')
			{
				filterComment(ky, linex); //adjust
				ky = realloc(ky, (1+strlen(ky)) * sizeof(char));
				ParseResult* elem = parseASM(ky, x);
				printASM(Out, elem, x);
				clearResult(elem);
				free(elem);
				x++;
			}
		free(ky);
    }
	fprintf(Out, "\n");
	fseek(Tmp, 0, SEEK_SET);
	fileAppend(Out, Tmp);
	fclose(In);
	fclose(Out);
	fclose(Tmp);
	clearArrays();
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

/**
 * 
 */
static void printData(FILE* Out, char* str)
{
	char id[257] = "\0";
	int i = 0;
	while (i < strlen(str))
	{
		if (str[i] == ' ')
		{
			break;
		}
		id[i] = str[i];
		i++;
	}
	id[i] = '\0';
	i++;
	char st[257] = "\0";
	if (strcmp(id, ".word")==0)
	{
		while (str[i] != '\n')
		{
			if ((str[i] == ' ') || (str[i] == ','))
			{
				i++;
			}
			else
			{
				//st = "\0";
				//int n = 0;
				/*while ((str[i] == ' ') || (str[i] == ',') || 
				(str[i] != '\n'))
				{
					st[n] = str[i];
					n++;
					i++;
				}*/
				i = fillWord(str, st, i, 256);
				int n = 1;
				char pr[257] = "\0";
				strcpy(pr, st);
				char* count = strchr(st, ':');
				if (count != NULL)
				{
					sscanf(count, ":%d", &n);
					sscanf(st, "%[^:]", pr);
				}
				//printf("%d, %s\n", n, pr);
				int bin = atoi(st);
				char* pt = toBin(bin, 32);
				while (n > 0)
				{
					fprintf(Out, "%s\n", pt);
					dataPos++;
					n--;
				}
				free(pt);
			}
		}
	}
	else
	{
		fprintf(Out, "-----------\n");
		printf("m2: this shouldn't execute\n");
	}
}

/**
 * 
 */
 //return alloc'd string
char* toBin(int value, int size)
{
	int num;
	if (value < 0)
	{
		num = -value;
	}
	else
	{
		num = value;
	}
	int* bin = calloc(size, sizeof(int));
	for (int i = (size-1); num != 0; i--)
	{
		bin[i] = abs(num % 2);
		num = num/2;
	}
	_Bool one = false;
	if (value < 0)
	{
		for (int n = size-1; n >= 0; n--)
		{
			if (one)
			{
				bin[n] = inv(bin[n]);
			}
			else if (bin[n] == 1)
			{
				one = true;
			}
		}
	}
	char* ret = calloc(size+1, sizeof(char));
	for (int i = 0; i < size; i++)
	{
		sprintf(&ret[i], "%d", bin[i]);
	}
	free(bin);
	return ret;
}

/**
 * 
 */
int inv(int val)
{
	return (int) val == 0;
}

/**
 * 
 */
int fillWord(char* line, char* dest, int offset, int buffer)
{
	int i = 0;
	while (i < (buffer-1) && (offset+i) < (int)strlen(line) && 
	(line[offset+i] != ' ' && line[offset+i] != ',' && 
	line[offset+i] != '\t' && line[offset+i] != '\n'))
	{
		dest[i] = line[offset+i];
		i++;
	}
	//dest[i] = '\0';
	//i++;
	return offset+i;
}

/**
 * 
 */
static void fileAppend(FILE* Out, FILE* Temp)
{
	char line[257] = "\0";
	while (fgets(line, 256, Temp) != NULL)
	{
		if(strlen(line) > 0)
		{
			fprintf(Out, "%s", line);
		}
	}
}

/**
 * 
 */
static DataLoc* var_Find(char* name)
{
	for (int i = 0; i < vsize; i++)
	{
		if (strcmp(name, var[i]->name)==0)
		{
			return var[i];
		}
	}
	return NULL;
	//return create_DataLoc("bs", -1);
}

/**
 * 
 */
static DataLoc* jump_Find(char* name)
{
	for (int i = 0; i < jsize; i++)
	{
		if (strcmp(name, jump[i]->name)==0)
		{
			return jump[i];
		}
	}
	return NULL;
	//return create_DataLoc("bs", -1);
}

/**
 * 
 */
static void clearArrays()
{
	for (int i = 0; i < jsize; i++)
	{
		clear_DataLoc(jump[i]);
		free(jump[i]);
	}
	free(jump);
	for (int i = 0; i < vsize; i++)
	{
		clear_DataLoc(var[i]);
		free(var[i]);
	}
	free(var);
}
                                                                                                  RegNum.c                                                                                            0000644 0001750 0001750 00000002626 13646450666 012623  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //RegNum.c
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
                                                                                                          ASMParser.h                                                                                         0000644 0001750 0001750 00000001226 13652057437 013217  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               #ifndef ASMPARSER_H
#define ASMPARSER_H
#include "ParseResult.h"

/**  Breaks up given MIPS32 assembly instruction and creates a proper 
 *   ParseResult object storing information about that instruction.
 * 
 *   Pre:  pASM points to an array holding the bits (as chars) of a
 *         syntactically valid assembly instruction, whose mnemonic is
 *         one of the following:
 *             add  addi  and  andi  lui  lw  or  ori  sub
 * 
 *   Returns:
 *         A pointer to a proper ParseResult object whose fields have been
 *         correctly initialized to correspond to the target of pASM.
 */
ParseResult* parseASM(const char* const pASM);

#endif
                                                                                                                                                                                                                                                                                                                                                                          BinFunc.h                                                                                           0000644 0001750 0001750 00000002202 13646074713 012740  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //BinFunc.h
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
 * 
 */
struct _BinFunc
{
	char* Name;
	char* op;
	char* func;
};
typedef struct _BinFunc BinFunc;
const bool r_Find(char* name, BinFunc* elem);
const bool i_Find(char* name, BinFunc* elem);
const bool spec_Find(char* name, BinFunc* elem);
#endif
                                                                                                                                                                                                                                                                                                                                                                                              DataLoc.h                                                                                           0000644 0001750 0001750 00000002072 13650667220 012724  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //DataLoc.h
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
                                                                                                                                                                                                                                                                                                                                                                                                                                                                      ParseResult.h                                                                                       0000644 0001750 0001750 00000006745 13650704467 013706  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               #ifndef PARSERESULT_H
#define PARSERESULT_H
#include <inttypes.h>
#include <stdio.h>
#include "RegNum.h"
#include "DataLoc.h"
#include <stdbool.h>

/**  Represents the possible field values for a MIPS32 machine instruction.
 * 
 *   A ParseResult object is said to be proper iff:
 * 
 *     - Each of the char* members is either NULL or points to a zero-
 *       terminated C-string.
 *     - If ASMInstruction is not NULL, the contents of the array represent
 *       a MIPS32 assembly instruction.
 *     - If ASMInstruction is not NULL, the other fields are set to properly
 *       represent the corresponding fields of the MIPS32 assembly instruction
 *       stored in ASMInstruction.
 *     - Each field that is not relevant to the MIPS32 assembly instruction
 *       is set as described in the comments below.
 */
struct _ParseResult {
   // Each char* member will be NULL or point to dynamically-allocated char array
   // holding a zero-terminated C string.

   // The assembly code portion
   char* ASMInstruction;    // the assembly instruction, as a C-string
   char* Mnemonic;          // the symbolic name of the instruction
   char* rdName;            // the symbolic names of the registers, as C-strings;
   char* rsName;            //    NULL if the register field is not specified
   char* rtName;            //    in the assembly instruction
   
   //   The following are integer values
   int16_t  Imm;            // the immediate field, as a signed integer;
                            //   0 if not relevant for the assembly instruction
   uint8_t  rd;             // the three register fields, as small unsigned integers;
   uint8_t  rs;             //   255 if not relevant for the assembly instruction
   uint8_t  rt;

   // The computed machine code portion
   //   These are malloc'd zero-terminated C-strings
   char* Opcode;            // the opcode field bits
   char* Funct;             // the funct field bits
                            //   NULL if not relevant for the assembly instruction
   char* RD;                // the bit representations of the register numbers; 
   char* RS;                //   NULL if not relevant for the assembly instruction
   char* RT;
   char* IMM;               // 2's complement bit representation of the immediate;
                            //   NULL if not relevant for the assembly instruction
   char* FLAGS;
   char* JUMP;
};

typedef struct _ParseResult ParseResult;

/**  Frees the dynamic content of a ParseResult object.
 * 
 *   Pre:  pPR points to a proper ParseResult object.
 *   Post: All of the dynamically-allocated arrays in *pPR have been
 *         deallocated.
 *         *pPR is proper.
 * 
 *   Comments:
 *     -  The function has no information about whether *pPR has been
 *        allocated dynamically, so it cannot risk attempting to 
 *        deallocate *pPR.
 *     -  The function is intended to provide the user with a simple
 *        way to free memory; the user may or may not reuse *pPR.  So,
 *        the function does set the pointers in *pPR to NULL.
 */
void clearResult(ParseResult* const pPR);

/**  Prints the contents of a ParseResult object.
 * 
 *   Pre:  Log is open on an output file.
 *         pPR points to a proper ParseResult object.
 */
void printResult(FILE* Log, const ParseResult* const pPR);

/**
 *
 */
void processFiles(char* in, char* out);

/**
 * 
 */
void processSymbols(char* in, char* out);

char* toBin(int value, int size);

int inv(int val);

int fillWord(char* line, char* dest, int offset, int buffer);
#endif
                           RegNum.h                                                                                            0000644 0001750 0001750 00000002022 13637542062 012606  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //RegNum.h
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
#ifndef REGNUM_H
#define REGNUM_H
#include <inttypes.h>
#include <stddef.h>
#include <string.h>
/**
 * 
 */
struct _RegNum
{
	char* Name;
	uint8_t reg;
	char* Bin;
};
typedef struct _RegNum RegNum;
const RegNum* Find(char* name);
#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              pledge.txt                                                                                          0000644 0001750 0001750 00000001432 13646476730 013255  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //  On my honor: 
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
                                                                                                                                                                                                                                      readme.txt                                                                                          0000644 0001750 0001750 00000000370 13650730566 013245  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               //readme.txt
This submission doesn't seem to correctly handle bne and beq 
instructions. Specifically addresses of label to branch to.
It also doesn't finish the line for some reason. This should be 
sorted up by the final
-Ross Manfred <rmm26079>
                                                                                                                                                                                                                                                                        makefile                                                                                            0000644 0001750 0001750 00000000351 13650667065 012751  0                                                                                                    ustar   rmanfred                        rmanfred                                                                                                                                                                                                               SHELL=/bin/bash
CC=gcc
CFLAGS=-std=c11 -Wall -W -O0 -ggdb3
assembler: ASMParser.c ParseResult.c BinFunc.c RegNum.c c03driver.c DataLoc.c
	$(CC) $(CFLAGS) -o assemble ASMParser.c ParseResult.c BinFunc.c RegNum.c c03driver.c DataLoc.c
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       