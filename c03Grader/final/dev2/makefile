SHELL=/bin/bash
CC=gcc
CFLAGS=-std=c11 -Wall -W -O0 -ggdb3
CG = tar
CFLAG=-cvf
assembler: ASMParser.c ParseResult.c BinFunc.c RegNum.c c03driver.c DataLoc.c
	$(CC) $(CFLAGS) -o assemble ASMParser.c ParseResult.c BinFunc.c RegNum.c c03driver.c DataLoc.c

package:
	$(CG) $(CFLAG) rmm26079.tar *.c *.h makefile *.txt
clear:
	rm rmm26079.tar
