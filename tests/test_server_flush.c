#include <stdio.h>
#include "common.h"

int main(void) {
    struct pollfd clients[10] = {0};
    struct LibserverClientArray client_array = libserver_client_array_init(10, clients);
    struct LibserverServer new_server = libserver_server_init(5031);

    libserver_server_init_clients(&new_server, 10, clients);

    libserver_server_add_client(&new_server, 4);
    libserver_server_add_client(&new_server, 5);
    libserver_server_add_client(&new_server, 6);
    libserver_server_add_client(&new_server, 7);
    libserver_server_add_client(&new_server, 8);
    libserver_server_add_client(&new_server, 9);

    client_array.contents[0].fd = -1;
    client_array.contents[3].fd = -1;
    client_array.contents[2].fd = -1;

    libserver_server_flush(&new_server);
    assert(new_server.clients.logical_size == 3);
    assert(new_server.clients.contents[0].fd == 5);
    assert(new_server.clients.contents[1].fd == 8);
    assert(new_server.clients.contents[2].fd == 9);

    libserver_server_free(&new_server, NULL);

    return EXIT_SUCCESS;
}
