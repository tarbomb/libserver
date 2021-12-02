#ifndef LIB_SOCKET_H
#define LIB_SOCKET_H

#include <stdlib.h>
#include <arpa/inet.h>

#define LIB_SOCKET_EXIT_FAILIURE EXIT_FAILURE

/*
 * A container for a socket and socket address
*/
struct LibsocketSocket {
    int fd;
    void *metadata;
    struct sockaddr_in address;
};

typedef void (*LibsocketAcceptCallback)(struct LibsocketSocket *socket_container, int connector);

/*
 * Initializes a new socket on an address and port. This can
 * be treated as both a server, or client.
 *
 * @param port: the port to register the socket to
 * @return: a socket file descriptor and address
*/
struct LibsocketSocket libsocket_socket_init(int port);

/*
 * Initializes a socket as a server socket that listens for a QUEUE
 * number of connects to queue up before revoking them.
 *
 * @param socket_container: the socket to bind
 * @param queue: the number of connections to queue up before blocking.
*/
void libsocket_socket_bind(struct LibsocketSocket *socket_container, int queue);

/*
 * Connects a socket to a specific address with the port bound to a socket.
 *
 * @param socket_container: the socket to connect
 * @param address: the address to connect to
*/
void libsocket_socket_connect(struct LibsocketSocket *socket_container, const char *address);

/*
 * Assign arbitrary data to the socket.
 *
 * @param socket_container: the socket to attach the data to
 * @param metadata: the data to attach
*/
void libsocket_socket_set_data(struct LibsocketSocket *socket_container, void *metadata);

/*
 * Polls a socket for any incoming connections, and invokes the accept
 * callback if one is detected.
 *
 * @param socket_container: the socket to poll
 * @param timeout: the timeout to poll for
 * @param callback: the callback invoked after an acception
 * @return: the descriptor of the new connection, or -1 if there was none
*/
int libsocket_socket_accept(struct LibsocketSocket *socket_container, int timeout, LibsocketAcceptCallback callback);

/*
 * Polls the socket file descriptor, and dertermines if
 * input is ready.
 *
 * @param descriptor: the socket file descriptor to poll
 * @param timeout: the timeout for polling
 * @return: whether or not input is ready to be read
*/
int libsocket_socket_has_input(int descriptor, int timeout);

#endif
