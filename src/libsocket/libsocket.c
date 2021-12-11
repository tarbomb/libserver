/*
 * Functions used for initializing and handling servers.
*/

#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "libsocket.h"

struct LibsocketSocket libsocket_socket_init(int port) {
    struct LibsocketSocket new_socket = {0};

    new_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
    new_socket.address.sin_family = AF_INET;
    new_socket.address.sin_port = htons(port);

    if(new_socket.fd == -1) {
        fprintf(stderr, "libsocket_socket_init: could not initialize socket (%s)\n", strerror(errno));
        exit(LIB_SOCKET_EXIT_FAILIURE);
    }

    return new_socket;
}

#if LIB_SOCKET_NO_SERVER == 0
void libsocket_socket_bind(struct LibsocketSocket *socket_container, int queue) {
    socket_container->address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socket_container->fd, (struct sockaddr *) &socket_container->address,
            sizeof(socket_container->address)) == -1) {
        fprintf(stderr, "libsocket_server_bind: could not bind to socket (%s)\n", strerror(errno));
        exit(LIB_SOCKET_EXIT_FAILIURE);
    }

    listen(socket_container->fd, queue);
}

int libsocket_socket_accept(struct LibsocketSocket *socket_container, int timeout, LibsocketAcceptCallback callback) {
    int connection_fd = 0;
    struct pollfd descriptor = {0};

    descriptor.events = POLLIN;
    descriptor.fd = socket_container->fd;

    /* No events */
    if(poll(&descriptor, 1, timeout) <= 0) {
        return -1;
    }

    /* Incorrect event */
    if((descriptor.revents & POLLIN) == 0) {
        return -1;
    }


    
    connection_fd = accept(socket_container->fd, NULL, NULL);

    if(callback != NULL) {
        callback(socket_container, connection_fd);
    }

    return connection_fd;
}
#endif

#if LIB_SOCKET_NO_CLIENT == 0
void libsocket_socket_connect(struct LibsocketSocket *socket_container, const char *address) {
    int status = 0;

    if(inet_pton(AF_INET, address, &socket_container->address.sin_addr) <= 0) {
        fprintf(stderr, "libsocket_socket_connect: failed to generate address '%s' (%s)\n", address, strerror(errno));
        exit(LIB_SOCKET_EXIT_FAILIURE);
    }

    status = connect(socket_container->fd, (struct sockaddr *) &socket_container->address, sizeof(socket_container->address));

    if(status < 0) {
        fprintf(stderr, "libsocket_socket_connect: failed to connect to address '%s' (%s)\n", address, strerror(errno));
        exit(LIB_SOCKET_EXIT_FAILIURE);
    }
}
#endif

int libsocket_socket_has_input(int descriptor, int timeout) {
    struct pollfd target = {0};
    target.events = POLLIN;
    target.fd = descriptor;

    /* No events detected */
    if(poll(&target, 1, timeout) <= 0) {
        return 0;
    }

    /* Incorrect event */
    if((target.revents & POLLIN) == 0) {
        return 0;
    }

    return 1;
}

void libsocket_socket_set_data(struct LibsocketSocket *socket_container, void *metadata) {
    socket_container->metadata = metadata;
}
