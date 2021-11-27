#include "../src/objects/server.h"

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverCommand commands[LIB_SERVER_MAXIMUM_COMMANDS] = {0};
    struct LibserverServer server = libserver_server_init(5036);

    return EXIT_SUCCESS;
}
