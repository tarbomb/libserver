#include <poll.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "common.h"

int main(void) {
    struct pollfd clients[LIB_SERVER_MAXIMUM_CLIENTS] = {0};
    struct LibserverServer new_server = libserver_server_init(TEST_PORT);

    libserver_server_init_clients(&new_server, LIB_SERVER_MAXIMUM_CLIENTS, clients);

    /* Baisc properties */
    assert(new_server.clients.contents == clients);
    assert(new_server.clients.physical_size == LIB_SERVER_MAXIMUM_CLIENTS);
    assert(new_server.socket.metadata == NULL);
    assert(new_server.socket.fd != -1);
    assert(new_server.mutex == NULL);

    /* Initialize mutex */
    assert(libserver_server_init_mutex(&new_server, "./mutex") != NULL);
    assert(new_server.mutex != NULL);

    /* Can the mutex be successfully held and unheld? */
    assert(pthread_mutex_lock(new_server.mutex) == 0);
    assert(pthread_mutex_unlock(new_server.mutex) == 0);

    /* Release the server and mutex */
    libserver_server_free(&new_server, "./mutex");

    return EXIT_SUCCESS;
}
