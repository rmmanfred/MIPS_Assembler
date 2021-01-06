# VT Assembler Project

## Purpose
- Create an assembler converting .asm assembly files into a binary object (.o) file
- Support for following instructions as well as .word and .asciiz used in the .data section:
  - load and store instructions lw, sw, lui
  - arithmetic instructions add, addi, addu, addiu, mul, mult, sub
  - logic instructions nop, nor, sll, slt, slti, sra, srav
  - control-of-flow instructions beq, blez, bgtz, bne, j, syscall
  - pseudo code instructions:
    - move rd, rs: copy register rs to rd
    - blt rs, rt, offset: conditional branch if rs < rt
    - la rt, label: load address label to register rt
    - li rt, imm16: load 16 bit immediate value to register rt
    - lw rt, label: load word at address label to register rt
- more on the details of this assignment can be seen [here](MIPSAssembler.pdf)

## Execution
- Used static lookup tables to provide conversions from instruction to binary representation as well as symbolic register name to binary and integer representation
- Parsing instructions from the .asm files create ParseResult objects that store information as well binary data we use to print
- ...

## Limitations and Errors
- Limited to instructions and data specified above
- Known errors when encountering .asciiz data values (hope to fix in the future)

## Usage and Testing
- Demo to come
- "assemble <input .asm file> <output file>" or "assemble <input> <output> -symbols" to produce symbol table
- provided "compare" executable can be used to compare output file from expected values
- Results of testing basic files can be found in grader folder
