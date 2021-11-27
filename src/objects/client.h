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

/*
 * Release a client from memory.
 *
 * @param value: the poll target to release
*/
void libserver_client_free(struct pollfd value);

/*
 * Append a new client in the form of a file descriptor for polling
 * into a client array.
 *
 * @param array: the array to append to
 * @param value: the poll target to use
*/
void libserver_client_array_append(struct LibserverClientArray *array, struct pollfd value);

/*
 * Initializes a new client array on the stack with a stack
 * array of a specific size.
 *
 * @param size: the maximum length of the array
 * @param block: the array of poll targets
*/
struct LibserverClientArray libserver_client_array_init(unsigned int size, struct pollfd *block);

/*
 * Release a client array from memory.
 *
 * @param array: the array to release
*/
void libserver_client_array_free(struct LibserverClientArray *array);

#endif
