#include <stdio.h>
#include <unistd.h>
#include "../src/objects/server.h"

void new_client(struct LibsocketSocket *socket, int connector) {
    struct LibserverServer *server = (struct LibserverServer *) socket->metadata;

    libserver_server_add_client(server, connector);
}

void kill_server(struct LibserverServer *server, int sender, const char *command) {
    printf("%s", "Killing server.\n");
    server->alive = 0;
}

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverCommand commands[LIB_SERVER_MAXIMUM_COMMANDS] = {0};
    struct LibserverServer server = libserver_server_init(5036);

    libsocket_socket_set_data(&server.socket, &server);
    libserver_server_init_clients(&server, LIB_SERVER_MAXIMUM_CLIENTS, clients);
    libserver_server_init_commands(&server, LIB_SERVER_MAXIMUM_COMMANDS, commands);
    libserver_server_init_mutex(&server, "./mutex");

    libserver_server_add_command(&server, "kill", kill_server);

    /* Handle requests */
    while(server.alive == 1) {
        libsocket_socket_accept(&server.socket, 500, new_client);
    }

    libserver_server_free(&server, "./mutex");

    return EXIT_SUCCESS;
}
