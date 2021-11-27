#ifndef LIB_SERVER_H
#define LIB_SERVER_H

#include "client.h"

/*
 * The server structure. Contains an array of connected clients,
 * as well as the central socket for the server.
*/
struct LibserverServer {
    int socket;
    struct LibserverClientArray clients;
};

/*
 * Initialize a new server structure on the stack.
 *
 * @param port: the port of the server
 * @return: a new server
*/
struct LibserverServer libserver_server_init(int port);

#endif
