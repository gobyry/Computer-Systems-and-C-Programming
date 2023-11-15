SHELL := /bin/sh
CC = clang -Wall
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -gdwarf-4 '-Wstrict-prototypes'

all: life

life: life.o universe.o
	$(CC) -lm -o life life.o universe.o -lncurses

clean:
	rm life -f e *.o
format:
	clang-format -i -style=file *.[ch]


