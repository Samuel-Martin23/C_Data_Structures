#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../C_Template/template.h"
#include "../../C_Allocation_Metrics/allocation_metrics.h"

#define DEFAULT_CAPACITY_SIZE       100

typedef struct vector vector_t;

void print_vec_index(vector_t *vec, int index, const char *beginning, const char *end);
size_t get_sizeof_vector();
int get_vector_size(vector_t *vec);
int get_vector_capacity(vector_t *vec);
template_t get_vector_template(vector_t *vec);

vector_t *vector_alloc(template_t T, int size, ...);
void vector_push(vector_t *vec, ...);
void vector_insert(vector_t *vec, int index, ...);
void vector_extend(vector_t *vec, int size, ...);
void vector_pop(vector_t *vec);
void vector_pop_index(vector_t *vec, int index);
void vector_remove_value(vector_t *vec, ...);
void *vector_at(vector_t *vec, int index);
vector_t *vector_range_alloc(vector_t *vec_src, int *start_opt, int *end_opt, int *step_opt);
int vector_get_index(vector_t *vec, ...);
void vector_reverse(vector_t *vec);
void vector_sort(vector_t *vec);
vector_t *vector_copy_alloc(vector_t *vec);
void vector_free(vector_t **vec);
void vector_print(vector_t *vec);

#endif /* VECTOR_H */
