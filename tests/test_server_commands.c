#include <string.h>
#include "common.h"

void ping_command(struct LibserverServer *server, int descriptor, const char *command) {

}

void pong_command(struct LibserverServer *server, int descriptor, const char *command) {

}

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverCommand commands[LIB_SERVER_MAXIMUM_COMMANDS] = {0};
    struct LibserverServer new_server = libserver_server_init(TEST_PORT);


    libserver_server_init_mutex(&new_server, "./mutex");
    libserver_server_init_clients(&new_server, LIB_SERVER_MAXIMUM_CLIENTS, clients);
    libserver_server_init_commands(&new_server, LIB_SERVER_MAXIMUM_COMMANDS, commands);

    assert(new_server.commands.logical_size == 0);
    assert(new_server.commands.physical_size == LIB_SERVER_MAXIMUM_COMMANDS);
    assert(new_server.commands.contents == commands);

    libserver_server_add_command(&new_server, "ping", ping_command);
    assert(new_server.commands.logical_size == 1);
    assert(new_server.commands.contents[new_server.commands.logical_size - 1].callback == ping_command);
    assert(strcmp(new_server.commands.contents[new_server.commands.logical_size - 1].command, "ping") == 0);

    libserver_server_add_command(&new_server, "pong", pong_command);
    assert(new_server.commands.logical_size == 2);
    assert(new_server.commands.contents[new_server.commands.logical_size - 1].callback == pong_command);
    assert(strcmp(new_server.commands.contents[new_server.commands.logical_size - 1].command, "pong") == 0);

    /* Release the server and mutex */
    libserver_server_free(&new_server, "./mutex");

    return EXIT_SUCCESS;
}
