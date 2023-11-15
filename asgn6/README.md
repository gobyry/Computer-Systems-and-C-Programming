# Assignment 6
Assignment 6 implements the LZ78 compression algorithm using an encode and decode program. The assignment is able to compress and decompress any file, text, or binary while performing read and writes in efficient blocks of 4KB.

# Building
To build all assignment components for asgn6, run “make all”.\
To clean files for asgn6, run “make clean”.\
To format files for asgn6, run “make format”.

# Running
To run encode.c, run “./encode “ combined with the available options:\
ex: “./encode -h” which prints the program usage.\
[OPTIONS]\
-v: enables verbose output.\
-i: specifies input file.\
-o: specifies output file.\
-h: displays program usage.\
\
To run decode.c, run “./decode  “ combined with the available options:\
ex: “./decode -h” which prints the program usage.\
[OPTIONS]\
-v: enables verbose output.\
-i: specifies input file.\
-o: specifies output file.\
-h: displays program usage.

# Files
## encode.c
.c file that implements the encode program.

## decode.c
.c file that implements the decode program.

## trie.c
.c file that implements Trie ADT.

## trie.h
Header file for trie.c.

## word.c
.c file that implements Word ADT.

## word.h
Header file for word.c.

## io.c
.c file that implements I/O module.

## io.h
Header file for io.c.

## endian.h
Header file for endianness module.

## code.h
Header file for code macros.

## README.md
Read me file for assignment 6.

## DESIGN.pdf
.pdf design file for assignment 6.

## WRITEUP.pdf
.pdf writeup file for assignment 6.

## Makefile
Make file that builds assignment 6.



