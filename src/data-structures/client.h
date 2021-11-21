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
 * An array of clients for the server to manage.
*/
ct_array_define_array(LibServerClientArray, struct LibServerClient);


/*
 * Appends a new client onto an array.
 *
 * @param array: the array to append to
 * @param value: the client to append
*/
void libserver_client_array_append(struct LibServerClientArray *array, struct LibServerClient value);

#endif
