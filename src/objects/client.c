/*
 * Implementations of the client array and methods.
*/

#include <stdlib.h>
#include <poll.h>
#include <unistd.h>

#include "client.h"

/* Client array operations */
void libserver_client_array_append(struct LibserverClientArray *array, struct pollfd value) {
    if(array->logical_size == array->physical_size) {
        array->physical_size = array->physical_size * 2;
        array->contents = (struct pollfd*) realloc(array->contents, sizeof(struct pollfd) * array->physical_size);
    }

    array->contents[array->logical_size] = value;
    array->logical_size++;
}

void libserver_client_array_free(struct LibserverClientArray *array) {
   unsigned int index = 0;

   for(index = 0; index < array->logical_size; index++) {
       libserver_client_free(array->contents[index]);
   }

   free(array);
}

void libserver_client_free(struct pollfd value) {
    close(value.fd);
}
