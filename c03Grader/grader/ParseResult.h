#ifndef PARSERESULT_H
#define PARSERESULT_H
#include <inttypes.h>
#include <stdio.h>
#include "RegNum.h"
#include "DataLoc.h"
#include "BinFunc.h"
#include <stdbool.h>
#include <math.h>

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
 * Pre-Condition: in and out are proper c strings representing filenames
 * Post-Condition: data from input file is assembled and printed to 
 * output file
 * Main powerhouse program managing the processing of input and printing
 * to output as binary
 */
void processFiles(char* in, char* out);

/**
 * Pre-Condition: in and out are proper c strings representing filenames
 * Post-Condition: data from input file is assembled and printed to 
 * output file
 * Function handling -symbols switch logging location of symbolic labels
 * and variables to output file based on input file's instructions
 */
void processSymbols(char* in, char* out);

/**
 * Pre-Condition: value is integer in mandated range, size is positive
 * Post-Condition: a binary representation of the value in the indicated
 * size is returned
 * Converts provided value to binary string of length size
 */
char* toBin(int value, int size);

/**
 * Pre-Condition: val is single digit 1 or 0
 * Post-Condition: single digit returned which is inversion of provided
 * digit in binary
 * Performs singular conversion from integer to it's single digit binary
 * inverse
 */
int inv(int val);

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
int fillWord(char* line, char* dest, int offset, int buffer);

/**
 * Pre-Condition: sub is a valid c string, offset is non negative
 * Post-Condition: integer returned indicating index of next pertinent
 * character in sub
 * Handy function skipping over whitespace and other ignorable 
 * characters and returning an index where useful data can be found
 */
int skipWhitespace(char* sub, int offset);
#endif
