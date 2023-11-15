# asgn4
asgn4 creates Conway's Game of Life in a program that accepts starting options and can print the game to a window.

# building
to build all components for asgn4, run "make all"\
to clean files for asgn4, run "make clean"\
to format files for asgn4, run "make format"

# running
to run life.c, run, "./life" combined with the available options:
\
ex: "./life -i text.txt" to input a file into the game
\
OPTIONS:
-t: specify toroidal universe\
-s: silence ncurses\
-i: specify input file\
-o: speicfy output file\
-h: print program usage

# files
## universe.c
c file that contains implementation of universe functions

## universe.h
header file for universe.c

## life.c
c file that contains main function and life game implementation

## Makefile
file that builds asgn4

## README.md
read me file for asgn4

## DESIGN.pdf
pdf file that contains asgn4 design

## WRITEUP.pdf
pdf file that contains asgn4 writeup
