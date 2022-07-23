#ifndef DYNAMIC_ARRAY_INT
#define DYNAMIC_ARRAY_INT

#include <stdarg.h>

#include "dynamic_array.h"

DynamicArray *dynamic_array_init_int(void);
DynamicArray *dynamic_array_init_int_args(size_t size, ...);
void dynamic_array_append_int(DynamicArray *dyn_array, int value);
void dynamic_array_insert_int(DynamicArray *dyn_array, size_t index, int value);
void dynamic_array_remove_int(DynamicArray *dyn_array, int value);
int dynamic_array_at_int(DynamicArray *dyn_array, size_t index);
int dynamic_array_sum_int(DynamicArray *dyn_array);
void dynamic_array_extend_int_args(DynamicArray *dyn_array, size_t size, ...);
void dynamic_array_extend_int_array(DynamicArray *dyn_array, int *arr, size_t size);
size_t dynamic_array_count_int(DynamicArray *dyn_array, int value);
bool dynamic_array_iterate_int(DynamicArrayIterator *iter, int *value);

#endif /* DYNAMIC_ARRAY_INT */
