/*
 * Implementations of the commands object and array for the server.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "command.h"

struct LibserverCommand libserver_command_free(struct LibserverCommand value) {
    return value;
}

int libserver_command_compare(struct LibserverCommand compare_a, struct LibserverCommand compare_b) {
    return strcmp(compare_a.command, compare_b.command) == 0;
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
   struct LibserverCommandArray new_array = {0};

   new_array.logical_size = 0;
   new_array.physical_size = size;
   new_array.contents = block;

   return new_array;
}

void libserver_command_array_free(struct LibserverCommandArray *array) {
   unsigned int index = 0;

   for(index = 0; index < array->logical_size; index++) {
       libserver_command_free(array->contents[index]);
   }
}

int libserver_command_array_find(struct LibserverCommandArray *array, struct LibserverCommand value) {
    unsigned int index = 0;

    for(index = 0; index < array->logical_size; index++) {
        if(libserver_command_compare(array->contents[index], value) == 0) {
            continue;
        }

        return index;
    }

    return -1;
}
