/*
 * Tests for adding clients to the server. This does not test
 * the functionality through IPC, but rather simulates it.
*/

#include <sys/stat.h>

#include "common.h"
#include "../src/objects/server.h"

int main(void) {
    struct LibServerServer server = libserver_server_init("./");

    /* Make the appropriate directory */
    mkdir("./" LIB_SERVER_CLIENT_DIRECTORY, 0744);

    libserver_server_add_client(&server, 1);

    libserver_server_free(server);

    return EXIT_SUCCESS;
}
