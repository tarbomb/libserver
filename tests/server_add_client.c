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
    assert(server.clients.logical_size == 1);
    assert(server.clients.physical_size == LIB_SERVER_DEFAULT_CLIENT_LENGTH);
    assert(server.clients.contents[0].process_id == 1);
    assert(libserver_file_exists("./" LIB_SERVER_CLIENT_DIRECTORY "/1.from") == 1);

    libserver_server_add_client(&server, 2);
    assert(server.clients.logical_size == 2);
    assert(server.clients.physical_size == LIB_SERVER_DEFAULT_CLIENT_LENGTH);
    assert(server.clients.contents[1].process_id == 2);
    assert(libserver_file_exists("./" LIB_SERVER_CLIENT_DIRECTORY "/2.from") == 1);

    libserver_server_free(server);
    libserver_server_cleanup(server);

    return EXIT_SUCCESS;
}
