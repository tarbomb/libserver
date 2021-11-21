/*
 * A function-based, ANSI C compliant queue implementation.
*/

#ifndef CT_QUEUE_H
#define CT_QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define ct_queue_is_empty(queue) \
    ((queue)->length == 0)

#define ct_queue_length(queue) \
    ((queue)->length)

#define ct_queue_peek(queue) \
    ((queue)->front)

#define ct_queue_define_node(node_name, node_type) \
    struct node_name {                             \
        node_type value;                           \
        struct node_name *next;                    \
    }

#define ct_queue_define_queue(queue_name, node_name) \
    struct queue_name {                              \
        unsigned int length;                         \
        struct node_name *front;                     \
        struct node_name *back;                      \
    }

#define ct_queue_define_init(queue_name, node_name, identifier)           \
    struct queue_name *identifier##_queue_init() {                        \
        return (struct queue_name*) calloc(1, sizeof(struct queue_name)); \
    }

#define ct_queue_define_free(queue_name, node_name, identifier, free_value) \
    void identifier##_queue_free(struct queue_name *queue) {                \
        struct node_name *node = queue->front;                              \
                                                                            \
        while(node != NULL) {                                               \
            struct node_name *next = node->next;                            \
                                                                            \
            free_value(node->value);                                        \
            free(node);                                                     \
            node = next;                                                    \
        }                                                                   \
                                                                            \
        free(queue);                                                        \
    }

#define ct_queue_define_enqueue(queue_name, node_name, value_type, identifier)                 \
    struct node_name *identifier##_queue_enqueue(struct queue_name *queue, value_type value) { \
        struct node_name *new_node = (struct node_name*) calloc(1, sizeof(struct node_name));  \
                                                                                               \
        if(queue->front == NULL) {                                                             \
            queue->front = new_node;                                                           \
            queue->back = new_node;                                                            \
        } else {                                                                               \
            queue->back->next = new_node;                                                      \
            queue->back = new_node;                                                            \
        }                                                                                      \
                                                                                               \
        new_node->value = value;                                                               \
        queue->length++;                                                                       \
                                                                                               \
        return new_node;                                                                       \
    }

#define ct_queue_define_dequeue(queue_name, node_name, value_type, identifier)                                               \
    value_type identifier##_queue_dequeue(struct queue_name *queue) {                                                        \
        value_type temp = {0};                                                                                               \
        struct node_name *front = queue->front;                                                                              \
                                                                                                                             \
        if(queue->front == NULL) {                                                                                           \
            fprintf(stderr, #identifier "_queue_dequeue: attempt to dequeue from empty queue (queue: %p)\n", (void*) queue); \
            exit(EXIT_FAILURE);                                                                                              \
        }                                                                                                                    \
                                                                                                                             \
        temp = queue->front->value;                                                                                          \
        queue->length--;                                                                                                     \
        queue->front = front->next;                                                                                          \
                                                                                                                             \
        if(queue->front == NULL) {                                                                                           \
            queue->back = NULL;                                                                                              \
        }                                                                                                                    \
                                                                                                                             \
        free(front);                                                                                                         \
                                                                                                                             \
        return temp;                                                                                                         \
    }

#endif
