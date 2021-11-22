/*
 * A function-based, ANSI C compliant dynamic array implementation. Also
 * has a setting for treating the array like a stack-only data structure.
 *
 * To use the setting described above, define the 'CT_ARRAY_STACK_ONLY'
 * macro.
*/

#ifndef CT_ARRAY_H
#define CT_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Overload-able default size */
#ifndef CT_ARRAY_PHYSICAL_SIZE
#define CT_ARRAY_PHYSICAL_SIZE 5
#endif

/* Overload-able equation for changing the size */
#ifndef CT_ARRAY_GROWTH_FACTOR
#define CT_ARRAY_GROWTH_FACTOR(current_size) \
    ((current_size) * 2)
#endif

#define ct_array_define_array(array_name, array_type) \
    struct array_name {                               \
        unsigned int logical_size;                    \
        unsigned int physical_size;                   \
        array_type *contents;                         \
    }

#ifndef CT_ARRAY_STACK_ONLY
#define ct_array_define_init(array_name, array_type, identifier)                                  \
    struct array_name *identifier##_array_init() {                                                \
        struct array_name *new_array = (struct array_name*) calloc(1, sizeof(struct array_name)); \
                                                                                                  \
        new_array->contents = (array_type*) calloc(CT_ARRAY_PHYSICAL_SIZE, sizeof(array_type));   \
        new_array->physical_size = CT_ARRAY_PHYSICAL_SIZE;                                        \
                                                                                                  \
        return new_array;                                                                         \
    }
#else
#define ct_array_define_init(array_name, array_type, identifier)                \
    struct array_name identifier##_array_init(size_t size, array_type *block) { \
        struct array_name new_array = {0, size, block};                         \
                                                                                \
        return new_array;                                                       \
    }
#endif

#ifndef CT_ARRAY_STACK_ONLY
#define ct_array_define_free(array_name, identifier, free_value) \
    void identifier##_array_free(struct array_name *array) {     \
        unsigned int index = 0;                                  \
                                                                 \
        for(index = 0; index < array->logical_size; index++) {   \
            free_value(array->contents[index]);                  \
        }                                                        \
                                                                 \
        free(array);                                             \
    }
#else
#define ct_array_define_free(array_name, identifier, free_value) \
void identifier##_array_free(struct array_name *array) {         \
        unsigned int index = 0;                                  \
                                                                 \
        for(index = 0; index < array->logical_size; index++) {   \
            free_value(array->contents[index]);                  \
        }                                                        \
}
#endif

#ifndef CT_ARRAY_STACK_ONLY
#define ct_array_define_insert(array_name, array_type, identifier)                                                                                      \
    struct array_name *identifier##_array_insert(struct array_name* array, array_type value, unsigned int index) {                                      \
        if(index < 0 || index > array->logical_size) {                                                                                                  \
            fprintf(stderr, #identifier "_array_insert: attempt to insert outside the bounds of array (array: %p, index: %i)\n", (void*) array, index); \
            exit(EXIT_FAILURE);                                                                                                                         \
        }                                                                                                                                               \
                                                                                                                                                        \
        if(array->logical_size == array->physical_size) {                                                                                               \
            array->physical_size = CT_ARRAY_GROWTH_FACTOR(array->physical_size);                                                                        \
            array->contents = (array_type*) realloc(array->contents, sizeof(array_type) * array->physical_size);                                        \
        }                                                                                                                                               \
                                                                                                                                                        \
        memmove(array->contents + (index + 1), array->contents + index, sizeof(array_type) * (array->logical_size - index));                            \
        array->contents[index] = value;                                                                                                                 \
        array->logical_size++;                                                                                                                          \
                                                                                                                                                        \
        return array;                                                                                                                                   \
    }
#else
#define ct_array_define_insert(array_name, array_type, identifier)                                                                                          \
    struct array_name *identifier##_array_insert(struct array_name* array, array_type value, unsigned int index) {                                          \
        if(index < 0 || index > array->logical_size) {                                                                                                      \
            fprintf(stderr, #identifier "_array_insert: attempt to insert outside the bounds of array (array: %p, index: %i)\n", (void*) array, index);     \
            exit(EXIT_FAILURE);                                                                                                                             \
        }                                                                                                                                                   \
                                                                                                                                                            \
        if(array->logical_size == array->physical_size) {                                                                                                   \
            fprintf(stderr, #identifier "_array_insert: attempt to insert into full array (array: %p, length: %u)\n", (void*) array, array->physical_size); \
            exit(EXIT_FAILURE);                                                                                                                             \
        }                                                                                                                                                   \
                                                                                                                                                            \
        memmove(array->contents + (index + 1), array->contents + index, sizeof(array_type) * (array->logical_size - index));                                \
        array->contents[index] = value;                                                                                                                     \
        array->logical_size++;                                                                                                                              \
                                                                                                                                                            \
        return array;                                                                                                                                       \
    }
#endif

#define ct_array_define_pop(array_name, array_type, identifier)                                                                                   \
    array_type identifier##_array_pop(struct array_name* array, unsigned int index) {                                                             \
        array_type temp = {0};                                                                                                                    \
                                                                                                                                                  \
        if(index < 0 || index >= array->logical_size) {                                                                                           \
            fprintf(stderr, #identifier "_array_pop: attempt to pop outside the bounds of array (array: %p, index: %i)\n", (void*) array, index); \
            exit(EXIT_FAILURE);                                                                                                                   \
        }                                                                                                                                         \
                                                                                                                                                  \
        temp = array->contents[index];                                                                                                            \
        memmove(array->contents + index, array->contents + index + 1, sizeof(array_type) * (array->logical_size - index));                        \
        array->logical_size--;                                                                                                                    \
                                                                                                                                                  \
        return temp;                                                                                                                              \
    }

#define ct_array_define_remove(array_name, array_type, identifier, compare, free_value)                                          \
    struct array_name *identifier##_array_remove(struct array_name* array, array_type value) {                                   \
        unsigned int index = 0;                                                                                                  \
                                                                                                                                 \
        for(index = 0; index < array->logical_size; index++) {                                                                   \
            if(compare(array->contents[index], value) == 0) {                                                                    \
                continue;                                                                                                        \
            }                                                                                                                    \
                                                                                                                                 \
            break;                                                                                                               \
        }                                                                                                                        \
                                                                                                                                 \
        if(index >= array->logical_size) {                                                                                       \
            fprintf(stderr, #identifier "_array_remove: attempt to remove value not inside array (array: %p)\n", (void*) array); \
            exit(EXIT_FAILURE);                                                                                                  \
        }                                                                                                                        \
                                                                                                                                 \
        free_value(array->contents[index]);                                                                                      \
        memmove(array->contents + index, array->contents + index + 1, sizeof(array_type) * (array->logical_size - index));       \
        array->logical_size--;                                                                                                   \
                                                                                                                                 \
        return array;                                                                                                            \
    }

#define ct_array_define_find(array_name, array_type, identifier, compare)       \
    int identifier##_array_find(struct array_name* array, array_type value) {   \
        int index = 0;                                                          \
                                                                                \
        for(index = 0; index < array->logical_size; index++) {                  \
            if(compare(array->contents[index], value) == 0) {                   \
                continue;                                                       \
            }                                                                   \
                                                                                \
            return index;                                                       \
        }                                                                       \
                                                                                \
        return -1;                                                              \
    }

#ifndef CT_ARRAY_STACK_ONLY
#define ct_array_define_append(array_name, array_type, identifier)                                               \
    struct array_name *identifier##_array_append(struct array_name* array, array_type value) {                   \
        if(array->logical_size == array->physical_size) {                                                        \
            array->physical_size = CT_ARRAY_GROWTH_FACTOR(array->physical_size);                                 \
            array->contents = (array_type*) realloc(array->contents, sizeof(array_type) * array->physical_size); \
        }                                                                                                        \
                                                                                                                 \
        array->contents[array->logical_size] = value;                                                            \
        array->logical_size++;                                                                                   \
                                                                                                                 \
        return array;                                                                                            \
    }
#else
#define ct_array_define_append(array_name, array_type, identifier)                                                                                          \
    struct array_name *identifier##_array_append(struct array_name* array, array_type value) {                                                              \
        if(array->logical_size == array->physical_size) {                                                                                                   \
            fprintf(stderr, #identifier "_array_append: attempt to insert into full array (array: %p, length: %u)\n", (void*) array, array->physical_size); \
            exit(EXIT_FAILURE);                                                                                                                             \
        }                                                                                                                                                   \
                                                                                                                                                            \
        array->contents[array->logical_size] = value;                                                                                                       \
        array->logical_size++;                                                                                                                              \
                                                                                                                                                            \
        return array;                                                                                                                                       \
    }
#endif

#endif
