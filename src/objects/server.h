/*
 * Prototypes and important structures for the server object.
*/

#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include <pthread.h>

#include "client.h"

#define LIB_SERVER_MUTEX_NAME               "libserver_mutex"
#define LIB_SERVER_DEFAULT_CLIENT_LENGTH    5

/*
 * A structure containing the current state of the server,
 * like any events that need to be processed, and the connected
 * clients.
*/
struct LibServerServer {
    pthread_mutex_t *mutex;
    struct LibServerClientArray clients;
};

/*
 * Initializes a new server on the stack, with a shared-memory
 * mutex that is mapped to a file. If the file does not exist,
 * it is created.
 *
 * @param mutex_file: the file to map the mutex to
 * @return: the new server
*/
struct LibServerServer libserver_server_init(const char *mutex_file);

/*
 * Releases a server structure from memory.
 *
 * @param server: the server to release
 * @param mutex_file: the file the mutex is mapped to
*/
void libserver_server_free(struct LibServerServer server, const char *mutex_file);

/*
 * Initializes a new client and adds it to the server's
 * array of clients.
 *
 * @param process_id: the process id of the client to mark down
*/
void libserver_server_add_client(struct LibServerServer *server, int process_id);

/*
 * Initializes a mutex into a server, based off a directory location.
 *
 * @param server: the server to load the mutex into
 * @param directory: the directory that the mutex should be created in
*/
void libserver_server_init_mutex(struct LibServerServer *server, const char *directory);


#endif
