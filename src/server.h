/*
 * Prototypes and important structures for libserver.
*/

#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include <pthread.h>
#include "data-structures/client.h"

#define LIB_SERVER_DEFAULT_CLIENT_LENGTH 5

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
struct LibServerServer libserver_init(const char *mutex_file);

/*
 * Releases a server structure from memory.
 *
 * @param server: the server to release
 * @param mutex_file: the file the mutex is mapped to
*/
void libserver_free(struct LibServerServer server, const char *mutex_file);


#endif
