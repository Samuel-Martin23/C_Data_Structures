#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_CAPACITY_SIZE       100

typedef struct dynamic_array dynamic_array_t;

typedef bool (*dynamic_array_equal_values)(void *value_1, void *value_2);
typedef void (*dynamic_array_print_index)(void *value);
typedef void (*dynamic_array_free_value)(void *value);

size_t dynamic_array_get_size(dynamic_array_t *dyn_array);
size_t dynamic_array_get_capacity(dynamic_array_t *dyn_array);
void **dynamic_array_get_data(dynamic_array_t *dyn_array);

dynamic_array_t *dynamic_array_init_alloc(dynamic_array_equal_values equal_values, dynamic_array_print_index print_index,
                                            dynamic_array_free_value free_index);
void dynamic_array_push(dynamic_array_t *dyn_array, void *value);
void dynamic_array_insert(dynamic_array_t *dyn_array, size_t index, void *value);
void dynamic_array_pop(dynamic_array_t *dyn_array);
void dynamic_array_pop_index(dynamic_array_t *dyn_array, size_t index);
void dynamic_array_remove(dynamic_array_t *dyn_array, void *value);
void *dynamic_array_at(dynamic_array_t *dyn_array, size_t index);
bool dynamic_array_index(dynamic_array_t *dyn_array, size_t *index, void *value);
bool dynamic_array_contains(dynamic_array_t *dyn_array, void *value);
void dynamic_array_reverse(dynamic_array_t *dyn_array);
bool dynamic_array_iterate(dynamic_array_t *dyn_array, void **value);
void dynamic_array_print(dynamic_array_t *dyn_array);
void dynamic_array_free(dynamic_array_t *dyn_array);

#endif /* DYNAMIC_ARRAY_H */
