#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_CAPACITY_SIZE       100

typedef struct vector vector_t;

typedef bool (*vector_equal_values)(void *value_1, void *value_2);
typedef void (*vector_print_index)(void *value);
typedef void (*vector_free_index)(void *value);

size_t vector_get_size(vector_t *vec);
size_t vector_get_capacity(vector_t *vec);
void **vector_get_data(vector_t *vec);

vector_t *vector_init_alloc(vector_equal_values equal_values, vector_print_index print_index,
                            vector_free_index free_index);
void vector_push(vector_t *vec, void *alloc_value);
void vector_insert(vector_t *vec, size_t index, void *alloc_value);
void vector_pop(vector_t *vec);
void vector_pop_index(vector_t *vec, size_t index);
void vector_remove_value(vector_t *vec, void *value);
size_t vector_get_value_index(vector_t *vec, void *value);
bool vector_contains_value(vector_t *vec, void *value);
void vector_reverse(vector_t *vec);
void vector_print(vector_t *vec);
void vector_free(vector_t *vec);

#endif /* VECTOR_H */
