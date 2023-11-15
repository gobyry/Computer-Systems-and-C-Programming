# asgn 2
asgn2 uses custom e, pi, and squre root approximation functions to make a custom math library and compares it to math.h. 

## building
to build all components for asgn2, run "make all"
to clean files for asgn2, run "make clean"
to format files for asgn2, run "make format"

## running
to run mathlib-test.c program, run "./mathlib-test" with the avaiable options:

ex. "./mathlib-test -a" to run all tests

-a: all tests\
-e: e test\
-b: bailey borwein plouffe test\
-m: madhava test\
-r: euler test\
-v: viete test\
-n: newton test\
-s: enable statistics\
-h: program usage

# files
## bbp.c 
c file that contains the implementation of the bbp pi approximation method and term tracker functions

## e.c 
c file that contains the implementation of the eulers number approximation and term tracker functions

## madhava.c
c file that contains the implementation of the madhava pi approximation method and term tracker functions

## euler.c 
c file that contains the implementation of the euler pi approximation method and term tracker functions

## viete.c 
c file that contains the implementation of the viete pi approximation method and term tracker functions

## newton.c 
c file that contains the implementation of the newton square root and term tracker functions

## mathlib-test.c 
c file that contains the main function and test harness for all functions in math-test.h and prints output

## mathlib.h 
header file for all the functions for asgn2

## DESIGN.pdf 
pdf file that contains asgn2 final design

## WRITEUP.pdf 
pdf file that contains asgn2 writeup

## Makefile 
file that makes all components of asgn2

## README.md 
read me file for asgn2




