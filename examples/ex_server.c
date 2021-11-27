#include <unistd.h>
#include "../src/objects/server.h"

void new_client(struct LibsocketSocket *socket, int connector) {

}

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverCommand commands[LIB_SERVER_MAXIMUM_COMMANDS] = {0};
    struct LibserverServer server = libserver_server_init(5036);

    libserver_server_init_clients(&server, LIB_SERVER_MAXIMUM_CLIENTS, clients);
    libserver_server_init_commands(&server, LIB_SERVER_MAXIMUM_COMMANDS, commands);
    libserver_server_init_mutex(&server, "./mutex");

    /* Handle requests */
    while(1) {
        libsocket_socket_accept(&server.socket, 500, new_client);
    }

    libserver_server_free(&server, "./mutex");

    return EXIT_SUCCESS;
}
