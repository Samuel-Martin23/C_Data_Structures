#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../C_Template/template.h"
#include "../C_Allocation_Metrics/allocation_metrics.h"

#define DEFAULT_CAPACITY_SIZE       100

typedef struct vector
{
    int size;
    int capacity;
    void **data;
    template_t T;
} vector_t;

void print_vector_size(vector_t *vec);

vector_t vector_init(template_t T, int size, ...);
void vector_push(vector_t *vec, ...);
void vector_insert(vector_t *vec, int index, ...);
void vector_extend(vector_t *vec, int size, ...);
void vector_pop(vector_t *vec);
void vector_pop_index(vector_t *vec, int index);
void vector_remove_value(vector_t *vec, ...);
void *vector_at(vector_t *vec, int index);
int vector_check_value(vector_t *vec, ...);
void vector_reverse(vector_t *vec);
void vector_sort(vector_t *vec);
void vector_copy(vector_t *vec_dest, vector_t *vec_src);
void vector_free(vector_t *vec);
void vector_print(vector_t *vec);

#endif /* VECTOR_H */
