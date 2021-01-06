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
//#include <stdbool.h>

/***  Add include directives for here as needed.  ***/

/*#include <inttypes.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "RegNum.h"
#include "BinFunc.h"*/

//immediate and offset are integers in the range -32768 to 32767

static void parseSpecial(ParseResult* init, int offset);
static void parseR(ParseResult* init, int offset);
static void parseI(ParseResult* init, int offset);
static void parsePseudo(ParseResult* init, int offset, char* giveback);
static void idFill(ParseResult* sub, BinFunc* elem);
static void fillField(ParseResult* elem, char* fill, 
int choice, int size);
static const int REGNAME = 6;
static const int IM = 17;
static const int FLAG = 257;
static const int MON = 8;
static const int REGBIT = 6;
static const int LONGNUM = 7;
//static const int LEAP = 27;

/** Breaks up given the MIPS32 assembly instruction and creates a proper
 * ParseResult object storing information about that instruction.
 *
 * Pre: pASM points to an array holding the representation of a
 * syntactically valid assembly instruction, giveback is NULL character
 * pointer
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
ParseResult* parseASM(const char* const pASM, char* giveback) {
	
	//Create ParseResult object
	ParseResult* obj = calloc(1, sizeof(ParseResult));
	(*obj).ASMInstruction = calloc(strlen(pASM)+1, sizeof(char));
	(*obj).Mnemonic = calloc(MON, sizeof(char));
	obj->rdName = NULL;
	obj->rsName = NULL;
	obj->rtName = NULL;
	obj->Opcode = NULL;
	obj->Funct = NULL;
	obj->RD = NULL;
	obj->RS = NULL;
	obj->RT = NULL;
	obj->IMM = NULL;
	obj->FLAGS = NULL;
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
	BinFunc* plem = calloc(1, sizeof(BinFunc));
	if (spec_Find((*obj).Mnemonic, spec))
	{
		idFill(obj, spec);
		free(spec);
		free(rlem);
		free(ilem);
		free(plem);
		parseSpecial(obj, n);
	}
	else if (r_Find((*obj).Mnemonic, rlem))
	{
		idFill(obj, rlem);
		free(rlem);
		free(spec);
		free(ilem);
		free(plem);
		parseR(obj, n);
	}
	else if(pseudo_Find(obj->Mnemonic, plem))
	{
		idFill(obj, plem);
		free(rlem);
		free(spec);
		free(ilem);
		free(plem);
		parsePseudo(obj, n, giveback);
	}
	else
	{
		i_Find((*obj).Mnemonic, ilem);
		idFill(obj, ilem);
		free(rlem);
		free(ilem);
		free(plem);
		free(spec);
		parseI(obj, n);
	}
	return obj;
}

//mult rs rt, syscall (20 bits), 
//sra/sll rd rt sa (pad rs with 0s, sa 5 bits)
/**
 * Pre-Condition: init is validly allocated ParseResult with instruction
 * and mnemonic, offset points to first content after mnemonic
 * Post-Condition: Proper ParseResult object is created for print
 * Parses instructions assigned to the "spec" lookup table
 * Instructions which require special circumstances/not fitting other
 * categories
 * Includes the following (tentative):
 * mult, syscall, la, beq, bne, sra, sll, blez, bgtz, j
 */
static void parseSpecial(ParseResult* init, int offset)
{
	//setup registers and fields
	//unused
	(*init).rd = (uint8_t)0;
	(*init).Imm = (uint16_t)0;
	
	//pull over full command
	char* st = calloc(strlen((*init).ASMInstruction) + 1, sizeof(char));
	strcpy(st, (*init).ASMInstruction);
	
	if (strcmp(init->Mnemonic, "mult") == 0)
	{
		char* temp = calloc(REGNAME, sizeof(char));
		int n = fillWord(st, temp, offset, REGNAME);
		
		//RS fill
		fillField(init, temp, 0, REGBIT);
		
		n = skipWhitespace(st, n);
		free(temp);
		temp = calloc(REGNAME, sizeof(char));
		n = fillWord(st, temp, n, REGNAME);
		
		//RT fill
		fillField(init, temp, 1, REGBIT);
		
		free(temp);
		
		//IMM special fill
		(*init).IMM = calloc(11, sizeof(char));
		strcpy(init->IMM, "0000000000");
	}
	//la
	else if(strcmp((*init).Mnemonic, "la")==0)
	{
		char* temp = calloc(REGNAME, sizeof(char));
		int n = fillWord(st, temp, offset, FLAG);
		
		//RT fill
		/*fillField(init, temp, 1, REGBIT);
		
		//RS special fill
		init->RS = calloc(REGBIT, sizeof(char));
		strcpy(init->RS, "00000");
		free(temp);
		
		temp = calloc(FLAG, sizeof(char));
		n = skipWhitespace(st, n);
		n = fillWord(st, temp, n, FLAG-1);
		temp = realloc(temp, (strlen(temp)+1)*sizeof(char));*/
		
		//FLAGS fill
		fillField(init, temp, 4, FLAG);
		
		free(temp);
	}
	//beq, bne, blez, bgtz
	else if(strcmp((*init).Mnemonic, "beq")==0 || 
	strcmp((*init).Mnemonic, "bne")==0 || 
	strcmp((*init).Mnemonic, "blez")==0 ||
	strcmp((*init).Mnemonic, "bgtz")==0)
	{
		char* temp = calloc(REGNAME, sizeof(char));
		int n = fillWord(st, temp, offset, REGNAME);
		
		//RS fill
		fillField(init, temp, 0, REGBIT);
		
		free(temp);
		if (strcmp((*init).Mnemonic, "blez")!=0 && 
		strcmp((*init).Mnemonic, "bgtz")!=0)
		{
			temp = calloc(REGNAME, sizeof(char));
			n = skipWhitespace(st, n);
			n = fillWord(st, temp, n, REGNAME);
			
			//RT fill
			fillField(init, temp, 1, REGBIT);
			
			free(temp);
		}
		else
		{
			//RT special fill
			init->RT = calloc(REGBIT, sizeof(char));
			strcpy(init->RT, "00000");
		}
		temp = calloc(FLAG, sizeof(char));
		n = skipWhitespace(st, n);
		n = fillWord(st, temp, n, FLAG);
		
		//JUMP fill
		fillField(init, temp, 5, FLAG);
		
		free(temp);
	}
	//j
	else if (strcmp((*init).Mnemonic, "j")==0 || 
	strcmp((*init).Mnemonic, "la")==0)
	{
		char* temp = calloc(FLAG, sizeof(char));
		int n = skipWhitespace(st, offset);
		n = fillWord(st, temp, n, FLAG);
		temp = realloc(temp, (1+strlen(temp))*sizeof(char));
		
		//j
		if(strcmp((*init).Mnemonic, "j")==0)
		{
			//JUMP fill
			fillField(init, temp, 5, FLAG);
		}
		//la
		else
		{
			//FLAGS fill
			fillField(init, temp, 4, FLAG);
		}
		
		free(temp);
	}
	//sll, sra
	else if (strcmp((*init).Mnemonic, "sll")==0 ||  
	strcmp((*init).Mnemonic, "sra")==0)
	{
		//RS special fill
		init->RS = calloc(REGBIT, sizeof(char));
		strcpy(init->RS, "00000");
		
		char* temp = calloc(REGNAME, sizeof(char));
		int n = skipWhitespace(st, offset);
		n = fillWord(st, temp, n, REGNAME);
		temp = realloc(temp, (1+strlen(temp))*sizeof(char));
		
		//RD fill
		fillField(init, temp, 2, REGBIT);
		
		free(temp);
		temp = calloc(REGNAME, sizeof(char));
		n = skipWhitespace(st, n);
		n = fillWord(st, temp, n, REGNAME);
		temp = realloc(temp, (1+strlen(temp))*sizeof(char));
		
		//RT fill
		fillField(init, temp, 1, REGBIT);
		
		free(temp);
		temp = calloc(LONGNUM, sizeof(char));
		n = skipWhitespace(st, n);
		n = fillWord(st, temp, n, LONGNUM);
		temp = realloc(temp, (1+strlen(temp))*sizeof(char));
		
		//IMM fill
		fillField(init, temp, 3, REGBIT-1);
		
		free(temp);
	}
	else //syscall
	{
		//IMM special fill
		(*init).IMM = calloc(21, sizeof(char));
		strcpy(init->IMM, "00000000000000000000");
	}
	free(st);
}

/**
 * Pre-Condition: init is validly allocated ParseResult with instruction
 * and mnemonic, offset points to first content after mnemonic
 * Post-Condition: Proper ParseResult object is created for print
 * Parses instructions assigned to the "r" lookup table
 * Instructions following r format, more like those that are of the form
 * "INSTRUCTION rd, rs, rt"
 * Includes the following (tentative):
 * mul, sub, nor, add, addu, slt, srav
 */
static void parseR(ParseResult* init, int offset)
{
	//pull over full command
	char* st = calloc(strlen((*init).ASMInstruction) + 1, sizeof(char));
	strcpy(st, (*init).ASMInstruction);
	
	char* temp = calloc(REGNAME, sizeof(char));
	int n = offset;
	
	//blt has custom RD fill
	if (strcmp(init->Mnemonic, "blt")!=0)
	{
		n = fillWord(st, temp, offset, REGNAME);
		//RD fill
		fillField(init, temp, 2, REGBIT);
	}
	if (strcmp(init->Mnemonic, "move")!=0)
	{
		n = skipWhitespace(st, n);
		free(temp);
		temp = calloc(REGNAME, sizeof(char));
		n = fillWord(st, temp, n, REGNAME);
		temp = realloc(temp, (1+strlen(temp))*sizeof(char));
		
		if (strcmp(init->Mnemonic, "srav")!=0)
		{
			//RS fill
			fillField(init, temp, 0, REGBIT);
		}
		else
		{		
			//RT fill
			fillField(init, temp, 1, REGBIT);
		}
	}
	else
	{
		fillField(init, "$zero", 0, REGBIT);
	}
	
	n = skipWhitespace(st, n);
	free(temp);
	temp = calloc(REGNAME, sizeof(char));
	n = fillWord(st, temp, n, REGNAME);
	
	if (strcmp(init->Mnemonic, "srav")!=0)
	{
		//RT fill
		fillField(init, temp, 1, REGBIT);
	}
	else
	{
		//RS fill
		fillField(init, temp, 0, REGBIT);
	}
	
	free(temp);
	free(st);
}

/**
 * Pre-Condition: init is validly allocated ParseResult with instruction
 * and mnemonic, offset points to first content after mnemonic
 * Post-Condition: Proper ParseResult object is created for print
 * Parses instructions assigned to the "i" lookup table
 * mostly instructions utilizing the 16 bit immediate field or other
 * immediate fields
 * Includes the following (tentative):
 * addi, lui, lw, slti, addiu, sw
 */
static void parseI(ParseResult* init, int offset)
{	
	//unused
	(*init).rd = (uint8_t)255;
	(*init).rs = (uint8_t)255;
	
	//pull over full command
	char* st = calloc(strlen((*init).ASMInstruction) + 1, sizeof(char));
	strcpy(st, (*init).ASMInstruction);
	
	//pull reg from command
	char* temp = calloc(REGNAME, sizeof(char));
	int n = fillWord(st, temp, offset, REGNAME);
	
	//RT fill
	fillField(init, temp, 1, REGBIT);
	
	free(temp);
	n = skipWhitespace(st, n);
	
	//addi, slti, addiu
	if (strcmp((*init).Mnemonic, "addi") == 0 || 
	strcmp((*init).Mnemonic, "slti") == 0 ||
	strcmp((*init).Mnemonic, "addiu") == 0)
	{
		temp = calloc(REGNAME, sizeof(char));
		n = fillWord(st, temp, n, REGNAME);
		
		//RS fill
		//WHAT THE FUCK AM I DOING?
		fillField(init, temp, 0, REGBIT);
		
		free(temp);
		n = skipWhitespace(st, n);
	}
	
	//IMM variables
	int16_t elem;
	char* imme;
	
	//lw, sw
	if (strcmp((*init).Mnemonic, "lw") == 0 || 
	strcmp((*init).Mnemonic, "sw") == 0)
	{
		_Bool symb = false;
		//Look for any more registers
		for (int x = n; x < strlen(st); x++)
		{
			if (st[x]=='$')
			{
				symb = true;
			}
		}
		if (symb)
		{
			temp = calloc(12, sizeof(char));
			n = fillWord(st, temp, n, 12);
			temp = realloc(temp, (strlen(temp)+1) * sizeof(char));
			
			char* im = calloc(7, sizeof(char));
			char* reg = calloc(REGNAME, sizeof(char));
			sscanf(temp, "%[^(](%[^)])", im, reg);
			
			//RS fill
			fillField(init, reg, 0, REGBIT);
			
			//IMM setup
			int i = atoi(im);
			elem = (int16_t) i;
			imme = toBin(i, IM-1);
			free(temp);
			free(im);
			free(reg);
		}
		else
		{
			elem = -1;
			imme = "\n";
			//RS fill
			fillField(init, "$zero", 0, REGBIT);
			
			temp = calloc(FLAG, sizeof(char));
			n = fillWord(st, temp, n, FLAG);
			temp = realloc(temp, (strlen(temp)+1) * sizeof(char));
			fillField(init, temp, 4, FLAG);
		}
	}
	//lui, addi
	else
	{
		if(strcmp((*init).Mnemonic, "lui") == 0 || 
		strcmp((*init).Mnemonic, "li") == 0)
		{
			//RS special fill
			fillField(init, "$zero", 0, REGBIT);
		}
		//account for large int
		temp = calloc(7, sizeof(char));
		n = fillWord(st, temp, n, 7);
		temp = realloc(temp, (strlen(temp)+1) * sizeof(char));
		
		//IMM setup
		int test = atoi(temp);
		imme = toBin(test, IM-1);
		elem = (int16_t)test;
		free(temp);
	}
	//IMM manual fill
	if((strcmp(imme, "\n")!=0))
	{
		(*init).IMM = calloc(IM, sizeof(char));
		(*init).Imm = elem;
		strcpy((*init).IMM, imme);
		free(imme);
		free(st);
	}
}

/**
 * Pre-Condition: init is validly allocated ParseResult with instruction
 * and mnemonic, offset points to first content after mnemonic
 * Post-Condition: Proper ParseResult object is created for print
 * Parses instructions assigned to the "pseudo" lookup table
 * Instructions not native to ASM, requiring conversion to common
 * formats or adjustments for special conditions
 * Includes the following (tentative):
 * move, blt, la, li
 */
static void parsePseudo(ParseResult* init, int offset, char* giveback)
{
	if (strcmp((*init).Mnemonic, "blt") == 0)
	{
		int n = offset;
		//pull over full command
		char* st = calloc(strlen((*init).ASMInstruction) + 1, 
		sizeof(char));
		strcpy(st, (*init).ASMInstruction);
		
		//process
		//RD fill
		fillField(init, "$at", 2, REGBIT);
		
		//Hand off to slt handler
		parseR(init, offset);
		
		//Move pointer to offset, expect 2 commas first
		int x = 0;
		while (x < 2)
		{
			if (st[n] == ',')
			{
				x++;
			}
			n++;
		}
		n = skipWhitespace(st, n);
		char temp[257] = "\0";
		n = fillWord(st, temp, n, FLAG);
		
		//setup new string
		char inst[257] = "\0";
		sprintf(inst, "bne $at, $zero, %s\n", temp);
		//giveback = calloc(257, sizeof(char));
		strcpy(giveback, inst);
		giveback = realloc(giveback, (1+strlen(giveback))*sizeof(char));
	}
	//la rt, label = addi rt, $zero, label
	else if (strcmp((*init).Mnemonic, "la") == 0)
	{
		//pull over full command
		int n = offset;
		char* st = calloc(strlen((*init).ASMInstruction) + 1, 
		sizeof(char));
		char* temp = calloc(REGNAME, sizeof(char));
		strcpy(st, (*init).ASMInstruction);
		
		//place reg
		n = fillWord(st, temp, n, REGNAME);
		n = skipWhitespace(st, n);
		//RT fill
		fillField(init, temp, 1, REGBIT);
		
		//RS fill
		fillField(init, "$zero", 0, REGBIT);
		
		free(temp);
		free(st);
		
		//Pass to special
		parseSpecial(init, n);
	}
}

/**
 * Pre-Condition: sub is validly allocated ParseResult, elem is valid
 * BinFunc from lookup table
 * Post-Condition: sub is properly updated with opcode and funct fields
 * Uses provided BinFunc to update ParseResult object with important
 * opcode and funct fields
 */
static void idFill(ParseResult* sub, BinFunc* elem)
{
	(*sub).Opcode = calloc(7, sizeof(char));
	(*sub).Funct = calloc(7, sizeof(char));
	strcpy((*sub).Opcode, (*elem).op);
	
	//Conditional funct update
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
 * Pre-Condition: elem is validly allocated ParseResult, fill is string
 * containing the information we wish to use, choice is between 0 and 5,
 * size indicates memory size for calloc
 * Post-Condition: elem contains desired info in desired field
 * Field filling manager handling allocations for important string
 * fields within ParseResult object. Choice mapping is as follows:
 * RS - 0 | RT - 1 | RD - 2 | IMM - 3 | FLAGS - 4 | JUMP - 5
 */
static void fillField(ParseResult* elem, char* fill, int choice, int size)
{
	//registers, works!
	if (choice <= 2)
	{
		RegNum* reg = Find(fill);
		char* name = calloc(REGNAME, sizeof(char));
		char* binary = calloc(size, sizeof(char));
		strcpy(name, reg->Name);
		name = realloc(name, (1+strlen(fill))*sizeof(char));
		strcpy(binary, reg->Bin);
		binary = realloc(binary, (1+strlen(binary))*sizeof(char));
		
		//RS
		if (choice == 0)
		{
			elem->rs = reg->reg;
			elem->rsName = name;
			elem->RS = binary;
		}
		
		//RT
		else if (choice == 1)
		{
			elem->rt = reg->reg;
			elem->rtName = name;
			elem->RT = binary;
		}
		
		//RD
		else if (choice == 2)
		{
			elem->rd = reg->reg;
			elem->rdName = name;
			elem->RD = binary;
		}
	}
	
	//IMM
	else if (choice == 3)
	{
		int num = atoi(fill);
		elem->Imm = num;
		elem->IMM = toBin(num, size);
	}
	
	//Labels
	else
	{
		//FLAGS
		if (choice == 4)
		{
			elem->FLAGS = calloc(size, sizeof(char));
			strcpy(elem->FLAGS, fill);
			elem->FLAGS = realloc(elem->FLAGS, 
			(1+strlen(elem->FLAGS))*sizeof(char));
		}
		
		//JUMP
		else
		{
			elem->JUMP = calloc(size, sizeof(char));
			strcpy(elem->JUMP, fill);
			elem->JUMP = realloc(elem->JUMP, 
			(1+strlen(elem->JUMP))*sizeof(char));
		}
	}
}
