SHELL := /bin/sh
CC = clang -Wall
CFLAGS = -Wall -Wpedantic -Werror -Wextra -g -gdwarf-4 '-Wstrict-prototypes'

all:
	scan-build --use-cc=clang -no-failure-reports --status-bugs make encode decode

encode: encode.o trie.o word.o io.o
	$(CC) -o $@ $^ $(LFLAGS)

decode: decode.o trie.o word.o io.o
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	rm -f keygen encode decode *.o

format:
	clang-format -i -style=file *.[ch]

