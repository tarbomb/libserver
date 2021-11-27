#include "common.h"

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverCommand commands[LIB_SERVER_MAXIMUM_COMMANDS] = {0};
    struct LibserverServer new_server = libserver_server_init(TEST_PORT, LIB_SERVER_MAXIMUM_CLIENTS, clients);

    libserver_server_init_mutex(&new_server, "./mutex");
    libserver_server_init_commands(&new_server, LIB_SERVER_MAXIMUM_COMMANDS, commands);

    assert(new_server.commands.logical_size == 0);
    assert(new_server.commands.physical_size == LIB_SERVER_MAXIMUM_COMMANDS);
    assert(new_server.commands.contents == commands);

    /* Release the server and mutex */
    libserver_server_free(&new_server, "./mutex");

    return EXIT_SUCCESS;
}
