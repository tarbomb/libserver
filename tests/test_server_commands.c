#include "common.h"

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverServer new_server = libserver_server_init(TEST_PORT, LIB_SERVER_MAXIMUM_CLIENTS, clients);

    libserver_server_init_mutex(&new_server, "./mutex");



    /* Release the server and mutex */
    libserver_server_free(&new_server, "./mutex");

    return EXIT_SUCCESS;
}
