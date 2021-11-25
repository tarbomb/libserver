/*
 * Tests for removing clients from the server. This does not test
 * the functionality through IPC, but rather simulates it.
*/

#include <stdlib.h>


#include "common.h"
#include "../src/objects/server.h"

int main(void) {
    struct LibServerServer server = libserver_server_init("./");

    /* Make the appropriate directory */
    mkdir("./" LIB_SERVER_CLIENT_DIRECTORY, 0744);

    libserver_server_add_client(&server, 1);

    libserver_server_free(server);
    libserver_server_cleanup(server);

    return EXIT_SUCCESS;
}
