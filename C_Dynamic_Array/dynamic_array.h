#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_CAPACITY_SIZE       100

typedef struct DynamicArray DynamicArray;
typedef struct DynamicArrayIterator DynamicArrayIterator;

typedef bool (*dynamic_array_equal_values)(void *value_1, void *value_2);
typedef void (*dynamic_array_print_index)(void *value);
typedef void (*dynamic_array_free_value)(void *value);

size_t dynamic_array_get_size(DynamicArray *dyn_array);
size_t dynamic_array_get_capacity(DynamicArray *dyn_array);
void **dynamic_array_get_data(DynamicArray *dyn_array);

DynamicArray *dynamic_array_init(dynamic_array_equal_values equal_values, dynamic_array_print_index print_index,
                                    dynamic_array_free_value free_value);
void dynamic_array_append(DynamicArray *dyn_array, void *value);
void dynamic_array_insert(DynamicArray *dyn_array, size_t index, void *value);
void dynamic_array_pop(DynamicArray *dyn_array);
void dynamic_array_pop_index(DynamicArray *dyn_array, size_t index);
void dynamic_array_pop_index_range(DynamicArray *dyn_array, size_t start_index, size_t end_index);
void dynamic_array_remove(DynamicArray *dyn_array, void *value);
void *dynamic_array_at(DynamicArray *dyn_array, size_t index);
bool dynamic_array_index(DynamicArray *dyn_array, size_t *index, void *value);
bool dynamic_array_contains(DynamicArray *dyn_array, void *value);
void dynamic_array_reverse(DynamicArray *dyn_array);
void dynamic_array_print(DynamicArray *dyn_array);
void dynamic_array_free(DynamicArray *dyn_array);

DynamicArrayIterator *dynamic_array_iterator_init(DynamicArray *dyn_array);
bool dynamic_array_iterator_iterate(DynamicArrayIterator *iter, void **value);
void dynamic_array_iterator_free(DynamicArrayIterator *iter);

#endif /* DYNAMIC_ARRAY_H */
