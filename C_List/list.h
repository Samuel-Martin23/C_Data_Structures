
#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../C_Template/template.h"

#define list_init_int(...)       list_init(int_array_cast_void(__VA_ARGS__))
#define list_init_double(...)    list_init(double_array_cast_void(__VA_ARGS__))
#define list_init_float(...)     list_init(float_array_cast_void(__VA_ARGS__))
#define list_init_char(...)      list_init(char_array_cast_void(__VA_ARGS__))
#define list_init_str(...)       list_init(str_array_cast_void(__VA_ARGS__))
#define list_init_bool(...)      list_init(bool_array_cast_void(__VA_ARGS__))

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

typedef struct list
{
    node_t *head;
    node_t *tail;
    int size;
    template_t T;
    int allocated_mem;
} list_t;

void print_allocated_mem_list(list_t *data_set);
void print_list_size(list_t *data_set);

list_t list_init(template_t T, void *data, int data_len);
void list_append(list_t *data_set, template_t T, void *data);
void list_insert(list_t *data_set, template_t T, void *data, int index);
void list_extend(list_t *data_set, template_t T, void *data, int data_len);
void list_remove_index(list_t *data_set, int index);
void list_remove_value(list_t *data_set, template_t T, void *value);
node_t *list_get_value(list_t *data_set, int index);
bool list_check_value(list_t *data_set, template_t T, void *data);
void list_reverse(list_t *data_set);
void list_sort(list_t *data_set);
void list_free(list_t *data_set);
void list_print(list_t *data_set);


#endif /* LINKED_LIST_H */
