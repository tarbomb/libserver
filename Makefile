CC=cc
PREFIX=/usr/local/lib
HEADER_PREFIX = /usr/local/include
CFLAGS=-ansi -Wall -Wextra -lpthread
TESTS=$(patsubst %.c,%.out,$(wildcard tests/*.c))

.PHONY: tests

build: out/server.o out/client.o

all: out/server.o out/client.o $(PREFIX)/libserver.so

install: $(PREFIX)/libserver.so

$(PREFIX)/libserver.so: out/*.o
	$(CC) -shared -o $(PREFIX)/libserver.so out/*.o

out/server.o: src/server.c src/server.h
	$(CC) -c src/server.c -o out/server.o -fpic $(CFLAGS)

out/client.o: src/data-structures/client.c src/data-structures/client.h
	$(CC) -c src/data-structures/client.c -o out/client.o -fpic $(CFLAGS)

tests/%.out: tests/%.c
	$(CC) $< -o $@ out/server.o out/client.o

tests: $(TESTS) out/server.o out/client.o
	for test_file in tests/*.out; do \
		 ./$$test_file;			 	 \
	done
