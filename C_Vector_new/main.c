#include "vector.h"
#include <stdarg.h>

#define SIZE 5

bool vec_equal_int(void *value_1, void *value_2)
{
    return (*((int*)value_1) == *((int*)value_2));
}

void vec_print_int(void *value)
{
    printf("%d", *((int*)value));
}

void *alloc_int(int value)
{
    int *allocated_value = malloc(sizeof(int));
    *allocated_value = value;
    return allocated_value;
}

void vector_va_int(vector_t *vec, size_t size, ...)
{
    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < size; i++)
    {
        vector_push(vec, alloc_int(va_arg(args, int)));
    }

    va_end(args);
}

int main()
{
    vector_t *scores = vector_init_alloc(vec_equal_int, vec_print_int);
    vector_va_int(scores, SIZE, 345, 123, 8934, 34, 89);

    vector_print(scores);

    vector_free(scores);

    return 0;
}
