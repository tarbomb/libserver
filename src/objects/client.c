/*
 * Implementations of the client array and methods.
*/

#include <poll.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "client.h"


void libserver_client_array_append(struct LibserverClientArray *array, struct pollfd value) {
    if(array->logical_size == array->physical_size) {
        fprintf(stderr, "libserver_client_array_append: attempt to insert into full array (array: %p, length: %u)\n", (void *) array, array->physical_size);
        exit(EXIT_FAILURE);
    }

    array->contents[array->logical_size] = value;
    array->logical_size++;
}

struct LibserverClientArray libserver_client_array_init(unsigned int size, struct pollfd *block) {
   struct LibserverClientArray new_array = {0};

   new_array.logical_size = 0;
   new_array.physical_size = size;
   new_array.contents = block;

   return new_array;
}

void libserver_client_array_free(struct LibserverClientArray *array) {
   unsigned int index = 0;

   for(index = 0; index < array->logical_size; index++) {
       libserver_client_free(array->contents[index]);
   }
}

void libserver_client_free(struct pollfd value) {
    close(value.fd);
}
