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
    libserver_command_array_free(&server->commands);

    shmtools_detach(server->mutex);
    shmtools_destroy(shmtools_get_id(mutex, sizeof(pthread_mutex_t)));

    unlink(mutex);
    close(server->socket.fd);
}

void libserver_server_init_commands(struct LibserverServer *server, size_t length, struct LibserverCommand commands[]) {
    server->commands = libserver_command_array_init(length, commands);
}

struct LibserverCommand libserver_server_add_command(struct LibserverServer *server, const char *name,
                                                     void (*callback)(struct LibserverServer *server, int descriptor, const char *arguments)) {
    struct LibserverCommand new_command = {0};

    new_command.callback = callback;
    new_command.command = name;

    libserver_command_array_append(&server->commands, new_command);

    return new_command;
}
