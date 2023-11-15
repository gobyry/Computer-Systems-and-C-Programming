## asgn5
Assignment 5 is an implementation of the Schmidt-Samoa with custom numtheory, keygen, encryption, and decryption program files.

# Building
To build all assignment components for asgn5, run “make all”.\
To clean files for asgn5, run “make clean”.\
To format files for asgn5, run “make format”.

# Running
To run keygen.c, run “./keygen “ combined with the available options:\
ex: “./keygen -h” which prints the program usage.\
[OPTIONS]\
-b: specifies minimum number of bits.\
-i: specifies the number of iterations.\
-n: specifies the public key file.\
-d: specifies the private key file.\
-s: specifies random seed.\
-v: enables verbose output.\
-h: displays program usage.\
\
To run encrypt.c, run “./encrypt  “ combined with the available options:\
ex: “./encrypt -h” which prints the program usage.\
[OPTIONS]\
-i: specifies the input file.\
-o: specifies the output file.\
-n: specifies the public key file.\
-v: enables verbose output.\
-h: displays program usage.\
\
To run decrypt.c, run “./decrypt  “ combined with the available options:\
ex: “./decrypt -h” which prints the program usage.\
[OPTIONS]\
-i: specifies the input file.\
-o: specifies the output file.\
-n: specifies the private key file.\
-v: enables verbose output.\
-h: displays program usage.

# Files
## decrypt.c
.c file that contains the main program and decrypt implementation.

## encrypt.c 
.c file that contains the main program and encrypt implementation.
## keygen.c
.c file that contains the main program and keygen implementation.

## numtheory.c
.c file that contains implementation of numtheory.

## numtheory.h
header file for numtheory.c

## randstate.c
.c file that contains implementation of randstate.

## randstate.h
header file for randstate.c

## ss.c
.c file that contains implementation of the ss algorithm functions.

## ss.h
header file for ss.c

## Makefile
file that builds asgn5.

## README.md
read me file for asgn5.

## DESIGN.pdf
.pdf file that contains final design for asgn5.

## WRITEUP.pdf
.pdf file that contains writeup for asgn5.


