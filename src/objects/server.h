#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include <stdlib.h>
#include <pthread.h>

#include "client.h"
#include "config.h"
#include "command.h"
#include "../libsocket/libsocket.h"

/*
 * The server structure. Contains an array of connected clients,
 * as well as the central socket for the server.
*/
struct LibserverServer {
    int alive;
    struct LibsocketSocket socket;
    struct LibserverClientArray clients;
    struct LibserverCommandArray commands;
#if LIB_SERVER_ENABLE_MUTEX == 1
    pthread_mutex_t *mutex;
#endif
};

/*
 * Initialize a new server structure on the stack.
 *
 * @param port: the port of the server
 * @return: a new server
*/
struct LibserverServer libserver_server_init(int port);

#if LIB_SERVER_ENABLE_MUTEX == 1
/*
 * Releases a server, and all of its clients from memory. Also removes
 * and detaches the shared memory mutex.
 *
 * @param server: the server to release
 * @param mutex: location of the server's mutex
*/
void libserver_server_free(struct LibserverServer *server, const char *mutex);
#else
/*
 * Releases a server, and all of its clients from memory.
 *
 * @param server: the server to release
*/
void libserver_server_free(struct LibserverServer *server);
#endif

#if LIB_SERVER_ENABLE_MUTEX == 1
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
struct LibserverCommand libserver_server_add_command(struct LibserverServer *server, const char *name, LibseverCallback callback);

/*
 * Adds a new client to a server's array of clients.
 *
 * @param server: the server to add the client to
 * @param descriptor: the socket descriptor of the client
 * @return: a copy of the client
*/
struct pollfd libserver_server_add_client(struct LibserverServer *server, int descriptor);

/*
 * Extracts the command from the start of the message, and writes
 * it to a buffer BUFFER of length LENGTH. The delimiter for the
 * command is ' ' by default.
 *
 * @param command: the command to extract from
 * @param length: the length of the buffer
 * @param buffer: the buffer to write the command to
 * @return: number of bytes written
*/
size_t libserver_server_extract_command(const char *command, size_t length, char buffer[]);

/*
 * Searches through a server's command array for a command with
 * the name COMMAND, and executes it.
 *
 * @param server: the server to search through
 * @param sender: the file descriptor of the socket that sent the command
 * @param message: the full message sent by the client
 * @param command: the name of the command
 * @return: 1 if a command was found, 0 if it was not
*/
int libserver_server_dispatch(struct LibserverServer *server, int sender, const char *message, const char *command);

/*
 * Process any incoming inputs on connected sockets, and invoke
 * any commands that the sockets request. Marks any sockets that
 * have been disconnected, which should be cleared by using the
 * libserver_server_flush function.
 *
 * @param server: the server to process requests for
 * @return: the number of clients processed, or -1 if an invalid command was detected
*/
int libserver_server_process(struct LibserverServer *server);

/*
 * Writes formatted data onto a socket. Maximum message size of
 * LIB_SERVER_RESPONSE_BUFFER.
 *
 * @param descriptor: the file descriptor of the socket to write to
 * @param format: the format string
 * @return: number of bytes written
*/
size_t libserver_server_respond(int descriptor, const char *format, ...);

/*
 * Flushes any disconnected clients from the server's client array.
 *
 * @param server: the server to flush
 * @return: number of clients flushed
*/
unsigned int libserver_server_flush(struct LibserverServer *server);

/*
 * Attempts to accept any new clients that are trying to connect to
 * the server, with a callback being invoked if there is a successful
 * connection.
 *
 * @param server: the server to accept clients for
 * @param timeout: the timeout to wait for accepting
 * @param on_accept: the callback to invoke on a successful connection
 * @return: the file descriptor associated with the new client, or -1 if there was none
*/
int libserver_server_accept(struct LibserverServer *server, int timeout, LibsocketAcceptCallback on_accept);

#endif
