CC=cc
PREFIX=/usr/local/lib
HEADER_PREFIX = /usr/local/include
CFLAGS=-Wall -Wextra -lpthread -Wno-unused-function -g
TESTS=$(patsubst %.c,%.out,$(wildcard tests/*.c))
MEM_CHECKER=

.PHONY: tests clean

build: out/server.o out/client.o out/shm-tools.o $(TESTS)

all: out/server.o out/client.o $(PREFIX)/libserver.so

install: $(PREFIX)/libserver.so

$(PREFIX)/libserver.so: out/*.o
	$(CC) -shared -o $(PREFIX)/libserver.so out/*.o

out/server.o: src/server.c src/server.h
	$(CC) -c src/server.c -o out/server.o -fpic $(CFLAGS)

out/client.o: src/data-structures/client.c src/data-structures/client.h
	$(CC) -c src/data-structures/client.c -o out/client.o -fpic $(CFLAGS)

out/shm-tools.o: src/shm-tools/shm-tools.c src/shm-tools/shm-tools.h
	$(CC) -c src/shm-tools/shm-tools.c -o out/shm-tools.o -fpic $(CFLAGS)

tests/%.out: tests/%.c out/server.o out/client.o
	$(CC) $< out/shm-tools.o out/server.o out/client.o -o $@ $(CFLAGS)

tests: $(TESTS)
	for test_file in tests/*.out; do \
		 $(MEM_CHECKER) ./$$test_file;			 	 \
	done

clean:
	rm -f tests/*.out
