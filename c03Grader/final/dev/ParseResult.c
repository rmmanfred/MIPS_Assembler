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
