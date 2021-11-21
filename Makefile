CC=cc
PREFIX=/usr/local/lib
HEADER_PREFIX = /usr/local/include
CFLAGS=-ansi -Wall -Wextra -lpthread

all: out/server.o out/client.o $(PREFIX)/libserver.so

install: $(PREFIX)/libserver.so

$(PREFIX)/libserver.so: out/*.o
	$(CC) -shared -o $(PREFIX)/libserver.so out/*.o

out/server.o: src/server.c src/server.h
	$(CC) -c src/server.c -o out/server.o -fpic $(CFLAGS)

out/client.o: src/data-structures/client.c src/data-structures/client.h
	$(CC) -c src/data-structures/client.c -o out/client.o -fpic $(CFLAGS)

# Build tests

.PHONY: tests
tests: tests/server_init.out
	for test_file in tests/*.out; do \
		 ./$$test_file;			 	 \
	done

tests/server_init.out: tests/server_init.c tests/common.h src/* src/data-structures/* src/data-structures/structures/*
	$(CC) tests/server_init.c src/server.c -o tests/server_init.out $(CFLAGS)
