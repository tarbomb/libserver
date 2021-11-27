/*
 * Implementations of server functions.
*/

#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/poll.h>
#include <sys/socket.h>

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
    if(mutex != NULL) {
        shmtools_detach(server->mutex);
        shmtools_destroy(shmtools_get_id(mutex, sizeof(pthread_mutex_t)));
        unlink(mutex);
    }

    shutdown(server->socket.fd, SHUT_RDWR);
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

    libserver_client_array_append(&server->clients, new_client);

    return new_client;
}

size_t libserver_server_extract_command(const char *command, size_t length, char buffer[]) {
    size_t written = 0;
    size_t index = 0;

    for(index = 0; index < length && command[index] != '\0'; index++) {
        if(command[index] == LIB_SERVER_COMMAND_DELIMITER) {
            break;
        }        

        buffer[index] = command[index];
        written++;
    }

    buffer[index] = '\0';

    return written;
}

int libserver_server_dispatch(struct LibserverServer *server, int sender, const char *message, const char *command) {
    struct LibserverCommand target = {0};
    int index = -1;

    target.command = command;
    index = libserver_command_array_find(&server->commands, target);

    if(index >= 0) {
        server->commands.contents[index].callback(server, sender, message);

        return 1;
    }

    return 0;
}

int libserver_server_process(struct LibserverServer *server) {
    int processed = 0;
    unsigned int index = 0;
    char client_message[LIB_SERVER_READ_BUFFER] = {0};
    char client_command[LIB_SERVER_COMMAND_BUFFER] = {0};
    
    /* No events */
    if(poll(server->clients.contents, server->clients.logical_size, LIB_SERVER_POLL_TIMEOUT) <= 0) {
        return 0;
    }

    /* Process input if it exists */
    for(index = 0; index < server->clients.logical_size; index++) {
        struct pollfd client = server->clients.contents[index];

        /* Not the event we are looking for */
        if((client.revents & POLLRDNORM) == 0) {
            continue;
        }

        /* Read message and extract command to perform dispatchment */
        read(client.fd, client_message, LIB_SERVER_READ_BUFFER);
        libserver_server_extract_command(client_message, LIB_SERVER_COMMAND_BUFFER, client_command);

        /* Execute the command, or handle an error */
        if(libserver_server_dispatch(server, client.fd, client_message, client_command) == 0) {
            return -1;
        }

        processed++;
    }

    return processed;
}
