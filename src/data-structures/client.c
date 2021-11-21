/*
 * Implementations of the client array and client-related functions.
*/

#include "client.h"
#include "structures/array.h"

void libserver_client_free(struct LibServerClient client) {
    return;
}

/* Macro generated functions */
ct_array_define_init(LibServerClientArray, struct LibServerClient, libserver_client)

ct_array_define_free(LibServerClientArray, libserver_client, libserver_client_free)

ct_array_define_append(LibServerClientArray, struct LibServerClient, libserver_client)
