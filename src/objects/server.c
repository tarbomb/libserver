/*
 * Implementations of server functions.
*/

#include <poll.h>
#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>
#include <pthread.h>

#include "client.h"
#include "server.h"
#include "command.h"
#include "../libsocket/libsocket.h"
#include "../shm-tools/shm-tools.h"

struct LibserverServer libserver_server_init(int port) {
    struct LibserverServer new_server = {0};

    new_server.alive = 1;
    new_server.socket = libsocket_socket_init(port);
    libsocket_socket_bind(&new_server.socket, LIB_SERVER_SOCKET_QUEUE);

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
    libserver_command_array_free(&server->commands);

    shmtools_detach(server->mutex);
    shmtools_destroy(shmtools_get_id(mutex, sizeof(pthread_mutex_t)));

    unlink(mutex);
    close(server->socket.fd);
}

void libserver_server_init_commands(struct LibserverServer *server, size_t length, struct LibserverCommand commands[]) {
    server->commands = libserver_command_array_init(length, commands);
}

void libserver_server_init_clients(struct LibserverServer *server, size_t length, struct pollfd clients[]) {
    server->clients = libserver_client_array_init(length, clients);
}

struct LibserverCommand
libserver_server_add_command(struct LibserverServer *server, const char *name, LibseverCallback callback) {
    struct LibserverCommand new_command = {0};

    new_command.callback = callback;
    new_command.command = name;

    libserver_command_array_append(&server->commands, new_command);

    return new_command;
}

struct pollfd libserver_server_add_client(struct LibserverServer *server, int descriptor) {
    struct pollfd new_client = {0};

    new_client.fd = descriptor;
    new_client.events = POLLRDNORM;

    return new_client;
}

int libserver_server_process(struct LibserverServer *server) {
    int processed = 0;
    unsigned int index = 0;
    
    /* No events */
    if(poll(server->clients.contents, server->clients.logical_size, LIB_SERVER_POLL_TIMEOUT) <= 0) {
        return 0;
    }

    /* Process input if it exists */
    for(index = 0; index < server->clients.logical_size; index++) {
        struct pollfd client = server->clients.contents[index];

        if((client.revents & POLLRDNORM) == 0) {
            continue;
        }

        processed++;
    }


    return processed;
}










