CC ?= cc
PREFIX ?=/usr/local/lib
CFLAGS ?=
OBJS=out/libsocket.o out/libfs.o out/shm-tools.o out/server.o out/client.o

.PHONY: clean

build: $(OBJS)

out/libsocket.o: src/libsocket/*
	mkdir -p out/
	$(CC) -c src/libsocket/libsocket.c -o out/libsocket.o $(CFLAGS)

out/libfs.o: src/libfs/*
	mkdir -p out/
	$(CC) -c src/libfs/libfs.c -o out/libfs.o $(CFLAGS)

out/shm-tools.o: src/shm-tools/*
	mkdir -p out/
	$(CC) -c src/shm-tools/shm-tools.c -o out/shm-tools.o $(CFLAGS)

out/server.o: src/objects/server.*
	mkdir -p out/
	$(CC) -c src/objects/server.c -o out/server.o $(CFLAGS)

out/client.o: src/objects/client.*
	mkdir -p out/
	$(CC) -c src/objects/client.c -o out/client.o $(CFLAGS)



clean:
	rm -f `find . -type f -name '*.o'`
