/*
 * Implementations of server functions.
*/

#include <pthread.h>

#include "server.h"
#include "../libsocket/libsocket.h"
#include "../shm-tools/shm-tools.h"

struct LibserverServer libserver_server_init(int port, size_t length, struct pollfd clients[]) {
    struct LibserverServer new_server = {0};

    new_server.socket = libsocket_socket_init(port);
    new_server.clients = libserver_client_array_init(length, clients);

    return new_server;
}

pthread_mutex_t *libserver_server_init_mutex(struct LibserverServer *server, const char *file) {
    pthread_mutex_t *new_mutex = NULL;
    int mutex_key = shmtools_get_id_create(file, sizeof(pthread_mutex_t));
    
    new_mutex = (pthread_mutex_t *) shmtools_attach(mutex_key);
    server->mutex = new_mutex;

    return new_mutex;
}
