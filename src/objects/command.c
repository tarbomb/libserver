/*
 * Implementations of the commands object and array for the server.
*/

#include <stdio.h>
#include <stdlib.h>
#include "command.h"

void libserver_command_free(struct LibserverCommand value) {
    return;
}

void libserver_command_array_append(struct LibserverCommandArray *array, struct LibserverCommand value) {
    if(array->logical_size == array->physical_size) {
        fprintf(stderr, "libserver_command_array_append: attempt to insert into full array (array: %p, length: %u)\n", (void *) array, array->physical_size);
        exit(EXIT_FAILURE);
    }

    array->contents[array->logical_size] = value;
    array->logical_size++;
}

struct LibserverCommandArray libserver_command_array_init(unsigned int size, struct LibserverCommand *block) {
   struct LibserverCommandArray new_array = {0, size, block};

   return new_array;
}

void libserver_command_array_free(struct LibserverCommandArray *array) {
   unsigned int index = 0;

   for(index = 0; index < array->logical_size; index++) {
       libserver_command_free(array->contents[index]);
   }
}
