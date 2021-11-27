/*
 * Implementations of server functions.
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "client.h"
#include "server.h"
#include "../libsocket/libsocket.h"
#include "../shm-tools/shm-tools.h"

struct LibserverServer libserver_server_init(int port, size_t length, struct pollfd clients[]) {
    struct LibserverServer new_server = {0};

    new_server.socket = libsocket_socket_init(port);
    new_server.clients = libserver_client_array_init(length, clients);

    return new_server;
}

pthread_mutex_t *libserver_server_init_mutex(struct LibserverServer *server, const char *mutex) {
    pthread_mutex_t *new_mutex = NULL;
    int mutex_key = shmtools_get_id_create(mutex, sizeof(pthread_mutex_t));
    
    new_mutex = (pthread_mutex_t *) shmtools_attach(mutex_key);
    server->mutex = new_mutex;

    return new_mutex;
}

void libserver_server_free(struct LibserverServer *server, const char *mutex) {
    libserver_client_array_free(&server->clients);

    shmtools_detach(server->mutex);
    shmtools_destroy(shmtools_get_id(mutex, sizeof(pthread_mutex_t)));

    unlink(mutex);
    close(server->socket.fd);
}

/* Command array operations */
void libserver_command_free(struct LibserverCommand value) {
    return;
}

void libserver_command_array_append(struct LibserverCommandArray *array, struct LibserverCommand value) {
    if(array->logical_size == array->physical_size) {
        fprintf(stderr, "libserver_command_array_append: attempt to insert into full array (array: %p, length: %u)\n", (void *) array, array->physical_size);
        exit(EXIT_FAILURE);
    }

    array->contents[array->logical_size] = value;
    array->logical_size++;
}

struct LibserverCommandArray libserver_command_array_init(unsigned int size, struct LibserverCommand *block) {
   struct LibserverCommandArray new_array = {0, size, block};

   return new_array;
}

void libserver_command_array_free(struct LibserverCommandArray *array) {
   unsigned int index = 0;

   for(index = 0; index < array->logical_size; index++) {
       libserver_command_free(array->contents[index]);
   }
}
