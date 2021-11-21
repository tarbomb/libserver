/*
 * The main file for controlling the server. The main loop of the server goes roughly
 * like this:
 *
 * 1) Poll for any clients requesting to connect to the server on the server's input pipe
 * 2) Poll for any requests from the connected clients
 * 3) Distribute any events
 * 4) Repeat
*/

#include <stdlib.h>
#include <sys/shm.h>
#include <sys/mman.h>

#include "server.h"
#include "data-structures/client.h"

struct LibServerServer libserver_init(const char *mutex) {
    struct LibServerServer new_server = {0};

    new_server.clients.contents = malloc(LIB_SERVER_DEFAULT_CLIENT_LENGTH * sizeof(struct LibServerClient));
    new_server.clients.physical_size = LIB_SERVER_DEFAULT_CLIENT_LENGTH;

    return new_server;
}
