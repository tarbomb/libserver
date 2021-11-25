/*
 * Implementations of the client array and client-related functions.
*/

#include <unistd.h>

#include "client.h"
#include "structures/array.h"

void libserver_client_free(struct LibServerClient client) {
    close(client.from_client);

    return;
}

int libserver_client_compare(struct LibServerClient compare_a, struct LibServerClient compare_b) {
    return compare_a.process_id == compare_b.process_id;
}

/* Macro generated functions */
ct_array_define_init(LibServerClientArray, struct LibServerClient, libserver_client)

ct_array_define_free(LibServerClientArray, libserver_client, libserver_client_free)

ct_array_define_append(LibServerClientArray, struct LibServerClient, libserver_client)

ct_array_define_remove(LibServerClientArray, struct LibServerClient, libserver_client, libserver_client_compare, libserver_client_free)
