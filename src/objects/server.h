#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include <stdlib.h>

#include "client.h"
#include "../libsocket/libsocket.h"

/*
 * The server structure. Contains an array of connected clients,
 * as well as the central socket for the server.
*/
struct LibserverServer {
    struct LibsocketSocket socket;
    struct LibserverClientArray clients;
};

/*
 * Initialize a new server structure on the stack.
 *
 * @param port: the port of the server
 * @param length: maximum length of the array
 * @param clients: pointer to an array of clients
 * @return: a new server
*/
struct LibserverServer libserver_server_init(int port, size_t length, struct pollfd clients[]);

#endif
