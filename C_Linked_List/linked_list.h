
#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../C_Template/template.h"

#define linked_list_init_int(...)       linked_list_init(int_array_cast_void(__VA_ARGS__))
#define linked_list_init_double(...)    linked_list_init(double_array_cast_void(__VA_ARGS__))
#define linked_list_init_float(...)     linked_list_init(float_array_cast_void(__VA_ARGS__))
#define linked_list_init_char(...)      linked_list_init(char_array_cast_void(__VA_ARGS__))
#define linked_list_init_str(...)       linked_list_init(str_array_cast_void(__VA_ARGS__))

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

typedef struct linked_list
{
    node_t *head;
    node_t *tail;
    int size;
    template_t T;
    int allocated_mem;
} linked_list_t;

void print_allocated_mem_list(linked_list_t *list);
void print_list_size(linked_list_t *list);

linked_list_t linked_list_init(template_t T, void *data, int data_len);
void linked_list_append(linked_list_t *list, template_t T, void *data);
void linked_list_insert(linked_list_t *list, int index, template_t T, void *data);
void linked_list_extend(linked_list_t *list, template_t T, void *data, int data_len);
void linked_list_remove_index(linked_list_t *list, int index);
void linked_list_remove_value(linked_list_t *list, template_t T, void *value);
node_t *linked_list_get_value(linked_list_t *list, int index);
bool linked_list_check_value(linked_list_t *list, template_t T, void *data);
void linked_list_reverse(linked_list_t *list);
void linked_list_sort(linked_list_t *list);
void linked_list_free(linked_list_t *list);
void linked_list_print(linked_list_t *list);


#endif /* LINKED_LIST_H */
