CC ?= cc
PREFIX ?=/usr/local/lib
CFLAGS ?= -ansi -Wall -Wextra -pedantic -Wshadow -Wdouble-promotion -g
OBJS=out/libsocket.o out/shm-tools.o out/server.o out/client.o out/command.o
TESTS=$(patsubst %.c,%.out,$(wildcard tests/*.c))
DEBUGGER ?=

.PHONY: clean check

build: $(OBJS)

out/libsocket.o: src/libsocket/*
	mkdir -p out/
	$(CC) -c src/libsocket/libsocket.c -o out/libsocket.o $(CFLAGS)

out/shm-tools.o: src/shm-tools/*
	mkdir -p out/
	$(CC) -c src/shm-tools/shm-tools.c -o out/shm-tools.o $(CFLAGS)

out/server.o: src/objects/server.*
	mkdir -p out/
	$(CC) -c src/objects/server.c -o out/server.o $(CFLAGS)

out/client.o: src/objects/client.*
	mkdir -p out/
	$(CC) -c src/objects/client.c -o out/client.o $(CFLAGS)

out/command.o: src/objects/command.*
	mkdir -p out/
	$(CC) -c src/objects/command.c -o out/command.o $(CFLAGS)

%.out: %.c $(OBJS) 
	$(CC) $< out/*.o -o $@

check: $(TESTS)
	@for test_file in tests/*.out; do $(DEBUGGER) ./$$test_file; printf "Test '%s' completed.\n" $$test_file; done

clean:
	$(RM) -r out/
	$(RM) tests/*.out
