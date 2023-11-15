SHELL := /bin/sh
CC = clang -Wall
CFLAGS = -Wall -Wpedantic -Werror -Wextra 

all: mathlib-test 

mathlib-test: mathlib.h mathlib-test.o e.o madhava.o euler.o bbp.o viete.o newton.o
	$(CC) -lm -o mathlib-test mathlib-test.o e.o madhava.o euler.o bbp.o viete.o newton.o
clean:
	rm mathlib-test -f e *.o
format:
	clang-format -i -style=file *.[ch]


