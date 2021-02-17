#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../C_Template/template.h"
#include "../C_Allocation_Metrics/allocation_metrics.h"

typedef struct node_list
{
    void *value;
    struct node_list *next;
} node_list_t;

typedef struct list
{
    node_list_t *head;
    node_list_t *tail;
    int size;
    template_t T;
} list_t;

void print_list_size(list_t *list);

list_t list_init(template_t T, int size, ...);
void list_append(list_t *list, ...);
void list_insert(list_t *list, int index, ...);
void list_extend(list_t *list, int size, ...);
void list_remove_index(list_t *list, int index);
void list_remove_value(list_t *list, ...);
void *list_get_value(list_t *list, int index);
bool list_check_value(list_t *list, ...);
void list_reverse(list_t *list);
void list_sort(list_t *list);
void list_copy(list_t *list_dest, list_t *list_src);
void list_free(list_t *list);
void list_print(list_t *list);

#endif /* LINKED_LIST_H */
