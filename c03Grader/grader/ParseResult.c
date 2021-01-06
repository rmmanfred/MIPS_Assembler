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
static void printData(FILE* Out, char* str);
static void fileAppend(FILE* Out, FILE* Temp);
static DataLoc* var_Find(char* name);
static DataLoc* jump_Find(char* name);
static int asciiPrint(FILE* Out, int n, char c);
static void clearArrays();
static void dataPosition(char* str);
static int high_int(double num);
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
 * Post-Condition: ptr is freed if it points to something
 * Manages ParseResult fields that may or may not have been allocated
 * by freeing them if they are not NULL
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
 * Pre-Condition: Out is proper created output file, pPR is validly 
 * allocated ParseResult, x is line number of instruction being printed
 * Post-Condition: Out contains printed binary binary code of
 * instruction
 * Takes finished ParseResult object and prints it's contents to the 
 * output file
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
		char* print;
		if (strcmp("j", pPR->Mnemonic)==0)
		{
			print = toBin((dt->loc)-1, 26);
		}
		else
		{
			print = toBin(((dt->loc) - x)-1, 16);
		}
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
 * Pre-Condition: Out is valid file pointer, ptr is c string
 * Post-Condition: If ptr is not null, prints to Out, otherwise nothing
 * Conditional print manager printing string only if it has contents
 */
static void condPrint(FILE* Out, char* ptr)
{
	if (ptr != NULL)
	{
		fprintf(Out, ptr);
	}
}

/**
 * Pre-Condition: in and out are proper c strings representing filenames
 * Post-Condition: data from input file is assembled and printed to 
 * output file
 * Main powerhouse program managing the processing of input and printing
 * to output as binary
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
	
	int lineLength = 256;
	char line[257] = "\0";
	char linex[257] = "\0";
	int n = 1;
	int offset = 8192;
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
			else if (key[skipWhitespace(key, 0)] != '\n' && 
			key[skipWhitespace(key, 0)] != '#')
			{
				n++;
				filterComment(key, linex);
				if (strchr(key, "blt") != NULL)
				{
					//Pseudo Instruction gives extra instruction
					n++;
				}
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
		//giveback string
		char* next = calloc((lineLength)+1, sizeof(char));
		strcpy(next, "\n");
		
        char* ky = calloc(lineLength + 1, sizeof(char));
		strcpy(ky, line);
		ky = realloc(ky, (strlen(ky)+1) * sizeof(char));
			if (strchr(ky, ':') == NULL && ky[0] != '.' && 
			ky[skipWhitespace(ky, 0)] != '\n' && 
			ky[skipWhitespace(ky, 0)] != '#')
			{
				filterComment(ky, linex); //adjust
				ky = realloc(ky, (1+strlen(ky)) * sizeof(char));
				if (strstr(ky, "nop")==NULL)
				{
					ParseResult* elem = parseASM(ky, next);
					printASM(Out, elem, x);
					clearResult(elem);
					free(elem);
				}
				else
				{
					fprintf(Out, "00000000000000000000000000000000\n");
				}
				x++;
			}
		free(ky);
		//Parser gives back second command
		if (strcmp(next, "\n")!=0)
		{
			char no[257] = "\0";
			ParseResult* extra = parseASM(next, no);
			//rep previous line
			printASM(Out, extra, x);
			clearResult(extra);
			free(extra);
			x++;
		}
		free(next);
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
 * Pre-Condition: in and out are proper c strings representing filenames
 * Post-Condition: data from input file is assembled and printed to 
 * output file
 * Function handling -symbols switch logging location of symbolic labels
 * and variables to output file based on input file's instructions
 *
void processSymbols(char* in, char* out)
{
	FILE* In = fopen(in, "r");
	FILE* Out = fopen(out, "w");
	char line[257] = "\0";
	int pos = 0;
	bool giveup = true;
	while (fgets(line, 256, In) != NULL && !giveup)
	{
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
}*/

/**
 * 
 */
void processSymbols(char* in, char* out)
{
	FILE* In = fopen(in, "r");
	FILE* Out = fopen(out, "w");
	vsize = 0;
	jsize = 0;

	int lineLength = 256;
	char line[257] = "\0";
	char linex[257] = "\0";
	int n = 0;
	int offset = 8192;
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
				//jump[jsize] = create_DataLoc(temp, n);
				char* hex = hexPos(4*n);
				fprintf(Out, "%s %s\n", hex, temp);
				free(hex);
				free(temp);
				jsize++;
			}
			else if (key[skipWhitespace(key, 0)] != '\n' && 
			key[skipWhitespace(key, 0)] != '#')
			{
				n++;
				filterComment(key, linex);
				if (strchr(key, "blt") != NULL)
				{
					//Pseudo Instruction gives extra instruction
					n++;
				}
			}
		}
		else if(relev==2)
		{
			if (key[0] == '\n')
			{
				relev = 0;
			}
			else
			{
				char* temp = calloc(257, sizeof(char));
				sscanf(key, "%[^:]:", temp);
				temp = realloc(temp, (strlen(temp)+1)*sizeof(char));
				//var[vsize] = create_DataLoc(temp, offset + (dataPos*4));
				char* he = hexPos(offset + (dataPos*4));
				fprintf(Out, "%s %s\n", he, temp);
				free(he);
				vsize++;
				dataPosition(key);
				free(temp);
			}
		}
		else if(key[0] == '.')
		{
			if (strcmp(key, ".text\n")==0)
			{
				relev = 1;
				text = (int)ftell(In);
			}
			else if (strcmp(key, ".data\n")==0)
			{
				relev = 2;
				offset = 8192;
				dataPos = 0;
			}
		}
		free(key);
	}
	fclose(In);
	fclose(Out);
}

//NEEDS TO BE ADJUSTED
/**
 * Pre-Condition: 
 * Post-Condition: 
 * Adjusts src string to exclude commenting
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
 * Pre-Condition: pos is valid integer in mandated range
 * Post-Condition: string representation of hexidecimal equivalent of 
 * pos is returned
 * Conversion program of integer to hexadecimal string
 * To be used by -symbols switch
 */
static char* hexPos(int pos)
{
	int* hex = calloc(8, sizeof(int));
	char* addr = calloc(11, sizeof(char));
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
 * Pre-Condition: num is proper integer in range 0 - 15
 * Post-Condition: hexadecimal character representing conversion is
 * returned
 * Performs singular conversion from integer to it's single hexadecimal
 * digit equivalent
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
 * Pre-Condition: Out is proper file pointer and str is valid c string
 * Post-Condition: Contents of str are converted and properly printed
 * to Out
 * Provides proper print handling of data section of input file
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
		int n = 1;
		int x = 1;
		char* sub = strchr(str, '"');
		//add asciiz handling
		while (sub[x] != '"')
		{
			n = asciiPrint(Out, n, (char)sub[x]);
			x++;
		}
		n = asciiPrint(Out, n, 0);
		while (n != 1)
		{
			n = asciiPrint(Out, n, 0);
		}
	}
}

char* toBin(int value, int size)
{
	int num = value;
	char* ret = calloc(size+1, sizeof(char));
    if (value < 0)
	{
        _Bool one = false;
        int i = size-1;
        while (i >= 0)
        {
            int tmp = abs(num % 2);
            if (one)
            {
                tmp = inv(tmp);
            }
            else if (tmp == 1)
            {
                one = true;
            }
            ret[i] = (char) 48 + tmp;
            num = num/2;
            i--;
        }
	}
    else
    {
        for (int x = (size-1); x >= 0; x--)
        {
            int tmp = abs(num % 2);
            ret[x] = (char) 48 + tmp;
            num = num/2;
        }
    }
	return ret;
}
/**
 * Pre-Condition: value is integer in mandated range, size is positive
 * Post-Condition: a binary representation of the value in the indicated
 * size is returned
 * Converts provided value to binary string of length size
 *
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
	//sprintf(ret, "%s", bin);
	free(bin);
	return ret;
}

/**
 * Pre-Condition: val is single digit 1 or 0
 * Post-Condition: single digit returned which is inversion of provided
 * digit in binary
 * Performs singular conversion from integer to it's single digit binary
 * inverse
 */
int inv(int val)
{
	return (int) val == 0;
}

/**
 * Pre-Condition: line is proper c string, dest is allocated c string
 * with enough space to handle string of length buffer, offset points to
 * relevant characters in line, buffer is non negative
 * Post-Condition: dest will contain relevant word from line as pointed
 * to by offset, length is not longer than buffer, integer returned
 * indicates index after word taken.
 * Very useful substring function putting characters from line into dest
 * until buffer is reached or any whitespace/punctuation/end of string
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
	return offset+i;
}

/**
 * Pre-Condition: File pointers are both valid, open, and not NULL
 * Post-Condition: Contents of Temp are appended to the end of Out
 * Appends lines of Temp to the end of Out
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
 * Pre-Condition: name is a valid c string
 * Post-Condition: DataLoc pointer of given name is returned if it 
 * exists
 * Searches DataLoc array var for entry of given name
 * var array contains locations of associated variables declared by
 * data section of instructions
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
}

/**
 * Pre-Condition: name is a valid c string
 * Post-Condition: DataLoc pointer of given name is returned if it 
 * exists
 * Searches DataLoc array jump for entry of given name
 * jump array contains locations of labels to be used by branch and jump
 * instructions
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
}

/**
 * Post-Condition: Contents of var and jump arrays are deallocated as 
 * well as the arrays themselves
 * Clears companions arrays containing pertinent info for jump labels
 * and data locations
 */
static void clearArrays()
{
	for (int i = 0; i < jsize; i++)
	{
		clear_DataLoc(jump[i]);
	}
	free(jump);
	for (int i = 0; i < vsize; i++)
	{
		clear_DataLoc(var[i]);
	}
	free(var);
}

/**
 * Pre-Condition: sub is a valid c string, offset is non negative
 * Post-Condition: integer returned indicating index of next pertinent
 * character in sub
 * Handy function skipping over whitespace and other ignorable 
 * characters and returning an index where useful data can be found
 */
int skipWhitespace(char* sub, int offset)
{
	while (sub[offset] == ' ' || sub[offset] == ',' || 
		sub[offset] == '\t')
	{
		offset++;
	}
	return offset;
}

/**
 * Pre-Condition: 
 * Post-Condition: 
 * 
 */
static int asciiPrint(FILE* Out, int n, char c)
{
	//get binary of character
	char* print = toBin((int) c, 8);
	if (n == 4)
	{
		fprintf(Out, "%s\n", print);
		dataPos++;
		n = 1;
	}
	else
	{
		fprintf(Out, "%s", print);
		n++;
	}
	free(print);
	return n;
}

/**
 * 
 */
static void dataPosition(char* line)
{
	char* str = strchr(line, '.');
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
	if (strcmp(id, ".word")==0)
	{
		int n = 1;
		while (str[i] != '\n')
		{
			if (str[i] == ',')
			{
				n++;
			}
			if (str[i] == ':')
			{
				char* num = calloc(257, sizeof(char));
				fillWord(str, num, i+1, 257);
				num = realloc(num, (1+strlen(num))*sizeof(char));
				n += (atoi(num) - 1);
				free(num);
			}
			i++;
		}
		dataPos += n;
	}
	//asciiz
	else
	{
		char* letters = strchr(str, '"');
		int x = 1;
		int total = 0;
		while (letters[x] != '\n')
		{
			if(letters[x] != '"')
			{
				total++;
			}
			x++;
		}
		dataPos += high_int((double)(total+1)/4.00);
	}
}

/**
 * 
 */
static int high_int(double num)
{
	int tmp = (int) num;
	if ((double)tmp < num)
	{
		return tmp+1;
	}
	else
	{
		return tmp;
	}
}
