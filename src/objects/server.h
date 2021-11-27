#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include <stdlib.h>
#include <pthread.h>

#include "client.h"
#include "../libsocket/libsocket.h"

#define LIB_SERVER_MAXIMUM_CLIENTS  128
#define LIB_SERVER_MAXIMUM_COMMANDS 64

struct LibserverServer;

/*
 * Represents a command that the server can execute.
*/
struct LibserverCommand {
    const char *command;
    void (*callback)(struct LibserverServer *server, int descriptor, const char *arguments);
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
 * The server structure. Contains an array of connected clients,
 * as well as the central socket for the server.
*/
struct LibserverServer {
    struct LibsocketSocket socket;
    struct LibserverClientArray clients;
    struct LibserverCommandArray commands;
    pthread_mutex_t *mutex;
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
 * Registers a new command into the server that will be invoked when
 * a socket sends a requested prefixed with the command.
 *
 * @param name: the name of the command
 * @param callback: the function to invoke
 * @return: a copy of the command
*/
struct LibserverCommand libserver_server_add_command(struct LibserverServer *server, const char *name,
                                                     void (*callback)(struct LibserverServer *server, int descriptor, const char *arguments));

/*
 * Releases a libserver command from memory.
 *
 * @param value: the value to release
*/
void libserver_command_free(struct LibserverCommand value);

/*
 * Appends a libserver command onto the array of commands
 * that the server has.
 *
 * @param array: the array to append to
 * @param value: the value to append
*/
void libserver_command_array_append(struct LibserverCommandArray *array, struct LibserverCommand value);

/*
 * Initialize a new command array with a maximum size SIZE, and
 * a stack array.
 *
 * @param size: the maximum length of the array
 * @param block: the stack array to use
*/
struct LibserverCommandArray libserver_command_array_init(unsigned int size, struct LibserverCommand *block);

/*
 * Releases a command array from memory.
 *
 * @param array: the array to release
*/
void libserver_command_array_free(struct LibserverCommandArray *array);









#endif
