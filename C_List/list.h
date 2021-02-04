
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
    void *value;
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

void print_allocated_mem_list(list_t *list);
void print_list_size(list_t *list);

list_t list_init(template_t T, void *data, int size);
void list_append(list_t *list, template_t T, void *value);
void list_insert(list_t *list, template_t T, void *value, int index);
void list_extend(list_t *list, template_t T, void *data, int size);
void list_remove_index(list_t *list, int index);
void list_remove_value(list_t *list, template_t T, void *value);
void *list_get_value(list_t *list, int index);
bool list_check_value(list_t *list, template_t T, void *value);
void list_reverse(list_t *list);
void list_sort(list_t *list);
void list_copy(list_t *list_dest, list_t *list_src);
void list_free(list_t *list);
void list_print(list_t *list);


#endif /* LINKED_LIST_H */
