#include "dynamic_array_int.h"

static bool equal_ints(void *value_1, void *value_2)
{
    return (*(int*)value_1 == *(int*)value_2);
}

static void print_int(void *value)
{
    printf("%d", *(int*)value);
}

static void *alloc_int(void *value)
{
    int *alloc_value = malloc(sizeof(int));
    *alloc_value = *(int*)value;
    return alloc_value;
}

DynamicArray *dynamic_array_init_int(void)
{
    return dynamic_array_init(equal_ints, print_int, alloc_int, free);
}

DynamicArray *dynamic_array_init_int_args(size_t size, ...)
{
    DynamicArray *dyn_array = dynamic_array_init_int();

    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < size; i++)
    {
        dynamic_array_append_int(dyn_array, va_arg(args, int));
    }

    va_end(args);

    return dyn_array;
}

void dynamic_array_append_int(DynamicArray *dyn_array, int value)
{
    dynamic_array_append(dyn_array, &value);
}

void dynamic_array_insert_int(DynamicArray *dyn_array, size_t index, int value)
{
   dynamic_array_insert(dyn_array, index, &value);
}

void dynamic_array_remove_int(DynamicArray *dyn_array, int value)
{
    dynamic_array_remove(dyn_array, &value);
}

int dynamic_array_at_int(DynamicArray *dyn_array, size_t index)
{
    void *value = dynamic_array_at(dyn_array, index);

    if (value == NULL)
    {
        fprintf(stderr, "Bad index\n");
        exit(EXIT_FAILURE);
    }

    return *(int*)value;
}

int dynamic_array_sum_int(DynamicArray *dyn_array)
{
    int sum = 0;
    DynamicArrayIterator *iter = dynamic_array_iterator_init(dyn_array);

    for (int value = 0; dynamic_array_iterate_int(iter, &value);)
    {
        sum += value;
    }

    dynamic_array_iterator_free(iter);

    return sum;
}

void dynamic_array_extend_int_args(DynamicArray *dyn_array, size_t size, ...)
{
    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < size; i++)
    {
        dynamic_array_append_int(dyn_array, va_arg(args, int));
    }

    va_end(args);
}

void dynamic_array_extend_int_array(DynamicArray *dyn_array, int *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        dynamic_array_append_int(dyn_array, arr[i]);
    }
}

void dynamic_array_extend_int_dyn_array(DynamicArray *src, DynamicArray *ext)
{
    DynamicArrayIterator *iter = dynamic_array_iterator_init(ext);

    for (int value = 0; dynamic_array_iterate_int(iter, &value);)
    {
        dynamic_array_append_int(src, value);
    }

    dynamic_array_iterator_free(iter);
}

bool dynamic_array_iterate_int(DynamicArrayIterator *iter, int *value)
{
    void *iter_value = NULL;
    bool is_iterating = dynamic_array_iterator_iterate(iter, &iter_value);

    if (is_iterating)
    {
        *value = *(int*)iter_value;
    }
    else
    {
        *value = 0;
    }

    return is_iterating;
}
