
#ifndef VECTOR_H
#define VECTOR_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../C_Template/template.h"

#define DEFAULT_CAPACITY_SIZE       100

#define vector_init_int(...)        vector_init(int_array_cast_void(__VA_ARGS__))
#define vector_init_double(...)     vector_init(double_array_cast_void(__VA_ARGS__))
#define vector_init_float(...)      vector_init(float_array_cast_void(__VA_ARGS__))
#define vector_init_char(...)       vector_init(char_array_cast_void(__VA_ARGS__))
#define vector_init_str(...)        vector_init(str_array_cast_void(__VA_ARGS__))
#define vector_init_bool(...)       vector_init(bool_array_cast_void(__VA_ARGS__))

typedef struct vector
{
    int size;
    int capacity;
    void **data;
    template_t T;
    int allocated_mem;
} vector_t;

void print_allocated_mem_vector(vector_t *vec);
void print_vector_size(vector_t *vec);

vector_t vector_init(template_t T, void *data, int size);
void vector_push(vector_t *vec, template_t T, void *value);
void vector_insert(vector_t *vec, template_t T, void *value, int index);
void vector_extend(vector_t *vec, template_t T, void *data, int size);
void vector_pop(vector_t *vec);
void vector_pop_index(vector_t *vec, int index);
void vector_remove_value(vector_t *vec, template_t T, void *value);
void *vector_at(vector_t *vec, int index);
int vector_check_value(vector_t *vec, template_t T, void *value);
void vector_reverse(vector_t *vec);
void vector_sort(vector_t *vec);
void vector_copy(vector_t *vec_dest, vector_t *vec_src);
void vector_clear(vector_t *vec);
void vector_free(vector_t *vec);
void vector_print(vector_t *vec);


#endif /* VECTOR_H */
