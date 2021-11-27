#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include <stdlib.h>
#include <pthread.h>

#include "client.h"
#include "command.h"
#include "../libsocket/libsocket.h"

#define LIB_SERVER_SOCKET_QUEUE     10
#define LIB_SERVER_MAXIMUM_CLIENTS  128
#define LIB_SERVER_MAXIMUM_COMMANDS 64

/*
 * The server structure. Contains an array of connected clients,
 * as well as the central socket for the server.
*/
struct LibserverServer {
    int alive;
    struct LibsocketSocket socket;
    struct LibserverClientArray clients;
    struct LibserverCommandArray commands;
    pthread_mutex_t *mutex;
};

/*
 * Initialize a new server structure on the stack.
 *
 * @param port: the port of the server
 * @return: a new server
*/
struct LibserverServer libserver_server_init(int port);

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

/*
 * Initializes a new array of commands within the server
 * with a maximum length LENGTH, and with a stack array
 * COMMANDS.
 *
 * @param server: the server to initialize commands into
 * @param length: the maximum length of the array
 * @param commands: the stack array to store in
*/
void libserver_server_init_commands(struct LibserverServer *server, size_t length, struct LibserverCommand commands[]);

/*
 * Initializes a new array of clients within the server
 * with a maximum length LENGTH, and with a stack array
 * CLIENTS.
 *
 * @param server: the server to initialize clients into
 * @param length: the maximum length of the array
 * @param clients: the stack array to store in
*/
void libserver_server_init_clients(struct LibserverServer *server, size_t length, struct pollfd clients[]);

/*
 * Registers a new command into the server that will be invoked when
 * a socket sends a requested prefixed with the command. The type
 * signature of the callback should look as such:
 *
 * void callback(struct LibserverServer*, int, const char*);
 *
 * @param name: the name of the command
 * @param callback: the function to invoke
 * @return: a copy of the command
*/
struct LibserverCommand
libserver_server_add_command(struct LibserverServer *server, const char *name, LibseverCallback callback);

/*
 * Adds a new client to a server's array of clients.
 *
 * @param server: the server to add the client to
 * @param descriptor: the socket descriptor of the client
 * @return: a copy of the client
*/
struct pollfd libserver_server_add_client(struct LibserverServer *server, int descriptor);

/*
 * Process any incoming inputs on connected sockets, and dispatching
 * commands
*/
int libserver_server_process(struct LibserverServer *server);





#endif
