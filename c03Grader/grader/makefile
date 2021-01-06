SHELL=/bin/bash
CC=gcc
CFLAGS=-std=c11 -Wall -W -O0 -ggdb3
assembler: ASMParser.c ParseResult.c BinFunc.c RegNum.c c03driver.c DataLoc.c
	$(CC) $(CFLAGS) -o assemble ASMParser.c ParseResult.c BinFunc.c RegNum.c c03driver.c DataLoc.c
