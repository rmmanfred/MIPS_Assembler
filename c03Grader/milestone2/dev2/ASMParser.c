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
static void parseSpecial(ParseResult* init, int offset);
static void parseR(ParseResult* init, int offset);
static void parseI(ParseResult* init, int offset);
static void idFill(ParseResult* sub, BinFunc* elem);
static const int REGNAME = 6;
static const int IM = 17;
static const int FLAG = 257;
static const int MON = 8;
static const int REGBIT = 6;

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
	(*obj).Mnemonic = calloc(MON, sizeof(char));
	obj->JUMP = NULL;
	
	//Avoid const attachment
	char* st = (char *) calloc((strlen(pASM)+1), sizeof(char));
	strcpy(st, pASM);
	
	//position variable
	int n = 0;
	
	//expect instruct no longer than 4 char
	//should run into whitespace
	n = skipWhitespace(st, n);
	
	//highest mnemonic length
	char* temp = calloc(MON, sizeof(char));
	n = fillWord(st, temp, n, MON);
	
	//adjust counters and temp
	temp = realloc(temp, (strlen(temp) + 1)*sizeof(char));
	
	//fill init fields
	strcpy((*obj).ASMInstruction, pASM);
	strcpy((*obj).Mnemonic, temp);
	obj->Mnemonic = realloc(obj->Mnemonic, 
	(strlen(temp)+1)*sizeof(char));
	
	//set position
	n = skipWhitespace(st, n);
	
	//Ditch helper variables
	free(st);
	free(temp);
	
	//Perform Switch to helper functions
	BinFunc* spec = calloc(1, sizeof(BinFunc));
	BinFunc* rlem = calloc(1, sizeof(BinFunc));
	BinFunc* ilem = calloc(1, sizeof(BinFunc));
	if (spec_Find((*obj).Mnemonic, spec))
	{
		idFill(obj, spec);
		free(spec);
		free(rlem);
		free(ilem);
		parseSpecial(obj, n);
	}
	else if (r_Find((*obj).Mnemonic, rlem))
	{
		idFill(obj, rlem);
		free(rlem);
		free(spec);
		free(ilem);
		parseR(obj, n);
	}
	else
	{
		i_Find((*obj).Mnemonic, ilem);
		idFill(obj, ilem);
		free(rlem);
		free(ilem);
		free(spec);
		parseI(obj, n);
	}
	return obj;
}

//mult rs rt, syscall (20 bits)
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
	//(*init).rsName = calloc(4, sizeof(char));
	//(*init).RS = calloc(6, sizeof(char));
	//(*init).rtName = calloc(4, sizeof(char));
	//(*init).RT = calloc(6, sizeof(char));
	//unused
	init->RT = NULL;
	init->RS = NULL;
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
		(*init).rsName = calloc(REGNAME, sizeof(char));
		(*init).RS = calloc(REGBIT, sizeof(char));
		(*init).rtName = calloc(REGNAME, sizeof(char));
		(*init).RT = calloc(REGBIT, sizeof(char));
		
		char* temp = calloc(REGNAME, sizeof(char));
		int n = fillWord(st, temp, offset, REGNAME);
		strcpy((*init).rsName, temp);
		init->rsName = realloc(init->rsName, 
		(strlen(init->rsName)+1)*sizeof(char));
		RegNum* search = Find((*init).rsName);
		(*init).rs = (*search).reg;
		char* num = toBin((int)(*search).reg, REGBIT-1);
		strcpy((*init).RS, num);
		free(num);
		
		n = skipWhitespace(st, n);
		free(temp);
		temp = calloc(REGNAME, sizeof(char));
		n = fillWord(st, temp, n, REGNAME);
		strcpy((*init).rtName, temp);
		init->rtName = realloc(init->rtName, 
		(strlen(init->rtName)+1)*sizeof(char));
		search = Find((*init).rtName);
		(*init).rt = (*search).reg;
		num = toBin((int)(*search).reg, REGBIT-1);
		strcpy((*init).RT, num);
		free(temp);
		free(num);
		(*init).IMM = calloc(11, sizeof(char));
		strcpy(init->IMM, "0000000000");
	}
	//la
	else if(strcmp((*init).Mnemonic, "la")==0)
	{
		char* temp = calloc(REGNAME, sizeof(char));
		int n = fillWord(st, temp, offset, REGNAME);
		init->rtName = calloc(REGNAME, sizeof(char));
		strcpy(init->rtName, temp);
		init->rtName = realloc(init->rtName, 
		(strlen(init->rtName)+1)*sizeof(char));
		init->RT = calloc(REGBIT, sizeof(char));
		RegNum* elem = Find(init->rtName);
		strcpy(init->RT, elem->Bin);
		init->rt = elem->reg;
		init->RS = calloc(REGBIT, sizeof(char));
		strcpy(init->RS, "00000");
		free(temp);
		temp = calloc(FLAG, sizeof(char));
		n = skipWhitespace(st, n);
		n = fillWord(st, temp, n, FLAG-1);
		temp = realloc(temp, (strlen(temp)+1)*sizeof(char));
		init->FLAGS = calloc(FLAG, sizeof(char));
		strcpy(init->FLAGS, temp);
		init->FLAGS = realloc(init->FLAGS, 
		(strlen(init->FLAGS)+1)*sizeof(char));
		free(temp);
	}
	else if(strcmp((*init).Mnemonic, "beq")==0 || 
	strcmp((*init).Mnemonic, "bne")==0)
	{
		init->rtName = calloc(REGNAME, sizeof(char));
		init->rsName = calloc(REGNAME, sizeof(char));
		init->JUMP = calloc(IM, sizeof(char));
		char* temp = calloc(REGNAME, sizeof(char));
		int n = fillWord(st, temp, offset, REGNAME);
		strcpy(init->rsName, temp);
		free(temp);
		temp = calloc(REGNAME, sizeof(char));
		n = skipWhitespace(st, n);
		n = fillWord(st, temp, n, REGNAME);
		strcpy(init->rtName, temp);
		free(temp);
		temp = calloc(FLAG, sizeof(char));
		n = skipWhitespace(st, n);
		n = fillWord(st, temp, n, FLAG);
		strcpy(init->JUMP, temp);
		init->JUMP = realloc(init->JUMP, 
		(strlen(init->JUMP)+1)*sizeof(char));
		free(temp);
		init->RT = calloc(REGBIT, sizeof(char));
		RegNum* elem = Find(init->rtName);
		strcpy(init->RT, elem->Bin);
		init->RS = calloc(REGBIT, sizeof(char));
		elem = Find(init->rsName);
		strcpy(init->RS, elem->Bin);
	}
	else //syscall, change later
	{
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
	(*init).rdName = calloc(REGNAME, sizeof(char));
	(*init).RD = calloc(REGBIT, sizeof(char));
	(*init).rsName = calloc(REGNAME, sizeof(char));
	(*init).RS = calloc(REGBIT, sizeof(char));
	(*init).rtName = calloc(REGNAME, sizeof(char));
	(*init).RT = calloc(REGBIT, sizeof(char));
	init->FLAGS = NULL;
	
	//pull over full command
	char* st = calloc(strlen((*init).ASMInstruction) + 1, sizeof(char));
	strcpy(st, (*init).ASMInstruction);
	
	char* temp = calloc(REGNAME, sizeof(char));
	int n = fillWord(st, temp, offset, REGNAME);
	strcpy((*init).rdName, temp);
	RegNum* search = Find((*init).rdName);
	(*init).rd = (*search).reg;
	char* num = toBin((int)(*search).reg, REGBIT-1);
	strcpy((*init).RD, num);
	free(num);
	
	n = skipWhitespace(st, n);
	free(temp);
	temp = calloc(REGNAME, sizeof(char));
	n = fillWord(st, temp, n, REGNAME);
	strcpy((*init).rsName, temp);
	search = Find((*init).rsName);
	(*init).rs = (*search).reg;
	num = toBin((int)(*search).reg, REGBIT-1);
	strcpy((*init).RS, num);
	free(num);
	
	n = skipWhitespace(st, n);
	free(temp);
	temp = calloc(REGNAME, sizeof(char));
	n = fillWord(st, temp, n, REGNAME);
	strcpy((*init).rtName, temp);
	search = Find((*init).rtName);
	(*init).rt = (*search).reg;
	num = toBin((int)(*search).reg, REGBIT-1);
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
	(*init).rtName = calloc(REGNAME, sizeof(char));
	(*init).RT = calloc(REGBIT, sizeof(char));
	(*init).IMM = calloc(IM, sizeof(char));
	
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
	char* temp = calloc(REGNAME, sizeof(char));
	int n = fillWord(st, temp, offset, REGNAME);
	strcpy((*init).rtName, temp);
	RegNum* search = Find((*init).rtName);
	(*init).rt = (*search).reg;
	char* num = toBin((int)(*search).reg, REGBIT-1);
	strcpy((*init).RT, num);
	free(num);
	free(temp);
	n = skipWhitespace(st, n);
	
	//addi
	if (strcmp((*init).Mnemonic, "addi") == 0 || 
	strcmp((*init).Mnemonic, "slti") == 0)
	{
		(*init).rsName = calloc(REGNAME, sizeof(char));
		(*init).RS = calloc(REGBIT, sizeof(char));
		temp = calloc(REGNAME, sizeof(char));
		n = fillWord(st, temp, n, REGNAME);
		strcpy((*init).rsName, temp);
		search = Find((*init).rsName);
		(*init).rs = (*search).reg;
		num = toBin((int)(*search).reg, REGBIT-1);
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
		(*init).rsName = calloc(REGNAME, sizeof(char));
		(*init).RS = calloc(REGBIT, sizeof(char));
		temp = calloc(12, sizeof(char));
		n = fillWord(st, temp, n, 12);
		temp = realloc(temp, (strlen(temp)+1) * sizeof(char));
		char* im = calloc(7, sizeof(char));
		char* reg = calloc(REGNAME, sizeof(char));
		sscanf(temp, "%[^(](%[^)])", im, reg);
		int i = atoi(im);
		search = Find(reg);
		num = toBin((int)(*search).reg, REGBIT-1);
		strcpy((*init).RS, num);
		strcpy((*init).rsName, reg);
		(*init).rs = (*search).reg;
		elem = (int16_t) i;
		imme = toBin(i, IM-1);
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
			(*init).RS = calloc(REGBIT, sizeof(char));
			strcpy((*init).RS, "00000");
			(*init).rs = (uint8_t) 0;
		}
		//account for large int
		temp = calloc(7, sizeof(char));
		n = fillWord(st, temp, n, 7);
		temp = realloc(temp, (strlen(temp)+1) * sizeof(char));
		int test = atoi(temp);
		imme = toBin(test, IM-1);
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
