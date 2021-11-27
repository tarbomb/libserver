#include <poll.h>
#include "common.h"

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverServer new_server = libserver_server_init(5000, LIB_SERVER_MAXIMUM_CLIENTS, clients);


    return EXIT_SUCCESS;
}
