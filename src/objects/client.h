#ifndef LIB_SERVER_CLIENT_H
#define LIB_SERVER_CLIENT_H

#include <poll.h>

/*
 * Array of integers representing file descriptors.
*/
struct LibserverClientArray {
    unsigned int logical_size;
    unsigned int physical_size;
    struct pollfd *contents;
};

void libserver_client_free(struct pollfd value);

struct LibserverClientArray *libserver_client_array_append(struct LibserverClientArray *array, struct pollfd value);

void libserver_client_array_free(struct LibserverClientArray *array);

#endif
