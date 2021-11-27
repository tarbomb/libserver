/*
 * Implementations of server functions.
*/

#include "server.h"
#include "../libsocket/libsocket.h"

struct LibserverServer libserver_server_init(int port, size_t length, struct pollfd clients[]) {
    struct LibserverServer new_server = {0};

    new_server.socket = libsocket_socket_init(port);
    new_server.clients = libserver_client_array_init(length, clients);

    return new_server;
}
