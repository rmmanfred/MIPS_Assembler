//ASMParser.c
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
#include "ASMParser.h"
#include <stdbool.h>

/***  Add include directives for here as needed.  ***/

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "RegNum.h"
#include "BinFunc.h"
//addi mul mult lui lw sub
//$t0, $t1, $t2, $t3, $s0, $s1, $s2, $s3
//immediate and offset are integers in the range -32768 to 32767

static int skipWhitespace(char* sub, int offset);
static int fillWord(char* line, char* dest, int offset, int buffer);
static void parseSpecial(ParseResult* init, int offset);
static void parseR(ParseResult* init, int offset);
static void parseI(ParseResult* init, int offset);
static void idFill(ParseResult* sub, BinFunc* elem);
static char* toBin(int value, int size);
static int inv(int val);

/**
 * R-format: mnemonic reg1, reg2, reg3 (mul, sub)
 * I-format: mnemonic reg1, reg2, immediate (addi)
 * mnemonic reg1, immediate (lui)
 * mnemonic reg1, offset(reg2) (lw)
 * SPECIAL2: mnemonic reg1, reg2 (mult)
 **/
/** Breaks up given the MIPS32 assembly instruction and creates a proper
 * ParseResult object storing information about that instruction.
 *
 * Pre: pASM points to an array holding the representation of a
 * syntactically valid assembly instruction, whose mnemonic is
 * one of the following:
 *
 * add addi and andi lui lw sub
 *
 * The instruction will be formatted as follows:
 *
 * <mnemonic><ws><operand1>,<ws><operand2>,<ws>...
 *
 * where <ws> is an arbitrary mixture of space and tab characters.
 *
 * Returns:
 * A pointer to a proper ParseResult object whose fields have been
 * correctly initialized to correspond to the target of pASM.
 */
ParseResult* parseASM(const char* const pASM) {
	/*
	 * Filled Fields: ASMInstruction, Mnemonic
	 * Setup Fields: Opcode, rt fields
	 * Alloc'd: st, temp
	 * Freed: st, temp
	 */
	//Create ParseResult object
	ParseResult* obj = calloc(1, sizeof(ParseResult));
	(*obj).ASMInstruction = calloc(strlen(pASM)+1, sizeof(char));
	(*obj).Mnemonic = calloc(5, sizeof(char));
	
	//Avoid const attachment
	char* st = (char *) calloc((strlen(pASM)+1), sizeof(char));
	strcpy(st, pASM);
	
	//position variable
	int n = 0;
	
	//expect instruct no longer than 4 char
	//should run into whitespace
	n = skipWhitespace(st, n);
	
	//highest mnemonic length
	char* temp = calloc(8, sizeof(char));
	n = fillWord(st, temp, n, 8);
	
	//adjust counters and temp
	temp = realloc(temp, (strlen(temp) + 1)*sizeof(char));
	
	//fill init fields
	strcpy((*obj).ASMInstruction, pASM);
	strcpy((*obj).Mnemonic, temp);
	
	//set position
	n = skipWhitespace(st, n);
	
	//Ditch helper variables
	free(st);
	free(temp);
	
	//Perform Switch to helper functions
	BinFunc* elem = calloc(1, sizeof(BinFunc));
	if (spec_Find((*obj).Mnemonic, elem))
	{
		idFill(obj, elem);
		free(elem);
		parseSpecial(obj, n);
	}
	else if (r_Find((*obj).Mnemonic, elem))
	{
		idFill(obj, elem);
		free(elem);
		parseR(obj, n);
	}
	else
	{
		i_Find((*obj).Mnemonic, elem);
		idFill(obj, elem);
		free(elem);
		parseI(obj, n);
	}
	return obj;
}

//mult rs rt
/**
 * 
 */
static void parseSpecial(ParseResult* init, int offset)
{
	/*
	 * Opcode, Funct, Mnemonic, ASMInstruction
	 * Filled Fields: all reg fields, immediates, Funct, 
	 * 					Opcode
	 * Alloc'd: st, temp (twice)
	 * Freed: st, temp (twice)
	 */
	//setup registers and fields
	(*init).rsName = calloc(4, sizeof(char));
	(*init).RS = calloc(6, sizeof(char));
	(*init).rtName = calloc(4, sizeof(char));
	(*init).RT = calloc(6, sizeof(char));
	//unused
	(*init).rdName = NULL;
	(*init).rd = (uint8_t)0;
	(*init).RD = NULL;
	(*init).Imm = (uint16_t)0;
	(*init).IMM = NULL;
	
	//pull over full command
	char* st = calloc(strlen((*init).ASMInstruction) + 1, sizeof(char));
	strcpy(st, (*init).ASMInstruction);
	if (strcmp(init->Mnemonic, "mult") == 0)
	{
		//expect registers as parameters, skip to content
		char* temp = calloc(4, sizeof(char));
		int n = fillWord(st, temp, offset, 4);
		strcpy((*init).rsName, temp);
		RegNum* search = Find((*init).rsName);
		(*init).rs = (*search).reg;
		char* num = toBin((int)(*search).reg, 5);
		strcpy((*init).RS, num);
		free(num);
		
		n = skipWhitespace(st, n);
		free(temp);
		temp = calloc(4, sizeof(char));
		n = fillWord(st, temp, n, 4);
		strcpy((*init).rtName, temp);
		search = Find((*init).rtName);
		(*init).rt = (*search).reg;
		num = toBin((int)(*search).reg, 5);
		strcpy((*init).RT, num);
		free(temp);
		free(num);
	}
	else //syscall, change later
	{
		free(init->RS);
		free(init->RT);
		init->RT = NULL;
		init->RS = NULL;
		(*init).IMM = calloc(21, sizeof(char));
		strcpy(init->IMM, "00000000000000000000");
	}
	free(st);
}

//mul rd rs rt, sub rd rs rt
/**
 * 
 */
static void parseR(ParseResult* init, int offset)
{
	
	/*
	 * Opcode, Funct, Mnemonic, ASMInstruction
	 * Filled Fields:
	 * Alloc'd:
	 * Freed:
	 */
	(*init).rdName = calloc(4, sizeof(char));
	(*init).RD = calloc(6, sizeof(char));
	(*init).rsName = calloc(4, sizeof(char));
	(*init).RS = calloc(6, sizeof(char));
	(*init).rtName = calloc(4, sizeof(char));
	(*init).RT = calloc(6, sizeof(char));
	
	//pull over full command
	char* st = calloc(strlen((*init).ASMInstruction) + 1, sizeof(char));
	strcpy(st, (*init).ASMInstruction);
	
	char* temp = calloc(4, sizeof(char));
	int n = fillWord(st, temp, offset, 4);
	strcpy((*init).rdName, temp);
	RegNum* search = Find((*init).rdName);
	(*init).rd = (*search).reg;
	char* num = toBin((int)(*search).reg, 5);
	strcpy((*init).RD, num);
	free(num);
	
	n = skipWhitespace(st, n);
	free(temp);
	temp = calloc(4, sizeof(char));
	n = fillWord(st, temp, n, 4);
	strcpy((*init).rsName, temp);
	search = Find((*init).rsName);
	(*init).rs = (*search).reg;
	num = toBin((int)(*search).reg, 5);
	strcpy((*init).RS, num);
	free(num);
	
	n = skipWhitespace(st, n);
	free(temp);
	temp = calloc(4, sizeof(char));
	n = fillWord(st, temp, n, 4);
	strcpy((*init).rtName, temp);
	search = Find((*init).rtName);
	(*init).rt = (*search).reg;
	num = toBin((int)(*search).reg, 5);
	strcpy((*init).RT, num);
	free(temp);
	free(st);
	free(num);
}

//addi rt rs imm, lui rt imm, lw rt off(base)
/**
 * 
 */
static void parseI(ParseResult* init, int offset)
{
	//Alloc'd: st, temp (4), num (2), im, reg, imme (2)
	//Freed: st, temp (4), num (2), im, reg, imme (outside loop)
	
	//Alloc strings
	(*init).rtName = calloc(4, sizeof(char));
	(*init).RT = calloc(6, sizeof(char));
	(*init).IMM = calloc(17, sizeof(char));
	
	//unused
	(*init).rdName = NULL;
	(*init).rd = (uint8_t)255;
	(*init).RD = NULL;
	(*init).rsName = NULL;
	(*init).rs = (uint8_t)255;
	(*init).RS = NULL;
	
	//pull over full command
	char* st = calloc(strlen((*init).ASMInstruction) + 1, sizeof(char));
	strcpy(st, (*init).ASMInstruction);
	
	//pull reg from command
	char* temp = calloc(4, sizeof(char));
	int n = fillWord(st, temp, offset, 4);
	strcpy((*init).rtName, temp);
	RegNum* search = Find((*init).rtName);
	(*init).rt = (*search).reg;
	char* num = toBin((int)(*search).reg, 5);
	strcpy((*init).RT, num);
	free(num);
	free(temp);
	n = skipWhitespace(st, n);
	
	//addi
	if (strcmp((*init).Mnemonic, "addi") == 0 || 
	strcmp((*init).Mnemonic, "slti") == 0)
	{
		(*init).rsName = calloc(4, sizeof(char));
		(*init).RS = calloc(6, sizeof(char));
		temp = calloc(4, sizeof(char));
		n = fillWord(st, temp, n, 4);
		strcpy((*init).rsName, temp);
		search = Find((*init).rsName);
		(*init).rs = (*search).reg;
		num = toBin((int)(*search).reg, 5);
		strcpy((*init).RS, num);
		free(num);
		free(temp);
		n = skipWhitespace(st, n);
	}
	int16_t elem;
	char* imme;
	//lw
	if (strcmp((*init).Mnemonic, "lw") == 0)
	{
		(*init).rsName = calloc(4, sizeof(char));
		(*init).RS = calloc(6, sizeof(char));
		temp = calloc(12, sizeof(char));
		n = fillWord(st, temp, n, 12);
		temp = realloc(temp, (strlen(temp)+1) * sizeof(char));
		char* im = calloc(7, sizeof(char));
		char* reg = calloc(4, sizeof(char));
		sscanf(temp, "%[^(](%[^)])", im, reg);
		int i = atoi(im);
		search = Find(reg);
		num = toBin((int)(*search).reg, 5);
		strcpy((*init).RS, num);
		strcpy((*init).rsName, reg);
		(*init).rs = (*search).reg;
		elem = (int16_t) i;
		imme = toBin(i, 16);
		free(temp);
		free(im);
		free(reg);
		free(num);
	}
	//lui, addi
	else
	{
		if(strcmp((*init).Mnemonic, "lui") == 0)
		{
			(*init).RS = calloc(6, sizeof(char));
			strcpy((*init).RS, "00000");
			(*init).rs = (uint8_t) 0;
		}
		//account for large int
		temp = calloc(7, sizeof(char));
		n = fillWord(st, temp, n, 7);
		temp = realloc(temp, (strlen(temp)+1) * sizeof(char));
		int test = atoi(temp);
		imme = toBin(test, 16);
		elem = (int16_t)test;
		free(temp);
	}
	(*init).Imm = elem;
	strcpy((*init).IMM, imme);
	free(imme);
	free(st);
}

/**
 * 
 */
static int skipWhitespace(char* sub, int offset)
{
	while (sub[offset] == ' ' || sub[offset] == ',' || 
		sub[offset] == '\t')
	{
		offset++;
	}
	return offset;
}

/**
 * 
 */
static int fillWord(char* line, char* dest, int offset, int buffer)
{
	int i = 0;
	while (i < (buffer-1) && (offset+i) < (int)strlen(line) && 
	(line[offset+i] != ' ' && line[offset+i] != ',' && 
	line[offset+i] != '\t' && line[offset+i] != '\n'))
	{
		dest[i] = line[offset+i];
		i++;
	}
	return offset+i;
}

/**
 * 
 */
static void idFill(ParseResult* sub, BinFunc* elem)
{
	(*sub).Opcode = calloc(7, sizeof(char));
	(*sub).Funct = calloc(7, sizeof(char));
	strcpy((*sub).Opcode, (*elem).op);
	if ((*elem).func != NULL)
	{
		strcpy((*sub).Funct, (*elem).func);
	}
	else
	{
		free((*sub).Funct);
		(*sub).Funct = NULL;
	}
}

/**
 * 
 */
 //return alloc'd string
static char* toBin(int value, int size)
{
	int num = value;
	if (num < 0)
	{
		num = -num;
	}
	int* bin = calloc(size, sizeof(int));
	for (int i = (size-1); num >= 1; i--)
	{
		bin[i] = num % 2;
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
static int inv(int val)
{
	return (int) val == 0;
}
