SHELL := /bin/sh
CC = clang -Wall
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -gdwarf-4 '-Wstrict-prototypes' -lncurses

all: sorting

sorting: sorting.o shell.o batcher.o heap.o quick.o set.o stats.o
	$(CC) -lm -o sorting sorting.o shell.o batcher.o heap.o quick.o set.o stats.o

clean:
	rm sorting -f e *.o
format:
	clang-format -i -style=file *.[ch]



