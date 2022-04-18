#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct queue_list queue_list_t;

typedef void (*queue_node_print_value)(void *value);
typedef void (*queue_node_free_value)(void *value);

size_t queue_list_get_size(queue_list_t *ql);
queue_list_t *queue_list_init(queue_node_print_value print_value, queue_node_free_value free_value);
void queue_list_enqueue(queue_list_t *ql, void *value);
void *queue_list_get_front(queue_list_t *ql);
void queue_list_dequeue(queue_list_t *ql);
bool queue_list_iterate(queue_list_t *ql, void **value);
void queue_list_print(queue_list_t *ql);
void queue_list_free(queue_list_t *ql);

#endif /* QUEUE_H */
