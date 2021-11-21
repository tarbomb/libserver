#ifndef LIB_SERVER_CLIENT_H
#define LIB_SERVER_CLIENT_H

#include "structures/array.h"

/*
 * A structure representing a client that is connected to
 * the server.
*/
struct LibServerClient {
    int process_id;
};

/*
 * Release a client from memory.
 *
 * @param client: the client to release
*/
void libserver_client_free(struct LibServerClient client);

/*
 * An array of clients for the server to manage.
*/
ct_array_define_array(LibServerClientArray, struct LibServerClient);

/*
 * Initialize a new client array on the heap.
 *
 * @parreturn: the new client array
*/
struct LibServerClientArray *libserver_client_array_init();

/*
 * Release a client array and its contents from memory.
 *
 * @param array: the array to release
*/
void libserver_client_array_free(struct LibServerClientArray *array);

/*
 * Appends a new client onto an array.
 *
 * @param array: the array to append to
 * @param value: the client to append
 * @return: the array appended to
*/
struct LibServerClientArray *libserver_client_array_append(struct LibServerClientArray *array, struct LibServerClient value);

#endif
