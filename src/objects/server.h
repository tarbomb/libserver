#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include <stdlib.h>
#include <pthread.h>

#include "client.h"
#include "../libsocket/libsocket.h"

#define LIB_SERVER_MAXIMUM_CLIENTS  128

/*
 * The server structure. Contains an array of connected clients,
 * as well as the central socket for the server.
*/
struct LibserverServer {
    struct LibsocketSocket socket;
    struct LibserverClientArray clients;
    pthread_mutex_t *mutex;
};

struct LibserverCommand {
    const char *command;
    void (*callback)(struct LibserverServer *server, const char *arguments);
};

/*
 * Array of commands that are registered into a server.
*/
struct LibserverCommandArray {
    unsigned int logical_size;
    unsigned int physical_size;
    struct LibserverCommand *contents;
};

/*
 * Initialize a new server structure on the stack.
 *
 * @param port: the port of the server
 * @param length: maximum length of the array
 * @param clients: pointer to an array of clients
 * @return: a new server
*/
struct LibserverServer libserver_server_init(int port, size_t length, struct pollfd clients[]);

/*
 * Releases a server, and all of its clients from memory. Also removes
 * and detaches the shared memory mutex.
 *
 * @param server: the server to release
 * @param mutex: location of the server's mutex
*/
void libserver_server_free(struct LibserverServer *server, const char *mutex);

/*
 * Initializes a new shared-memory mutex into a server, using the file at
 * the path FILE. The file is created if it does not exist.
 *
 * @param server: the server to initialize the mutex into
 * @param mutex: location to store the server's mutex
 * @return: the new mutex
*/
pthread_mutex_t *libserver_server_init_mutex(struct LibserverServer *server, const char *mutex);

#endif
