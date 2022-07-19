#include "dynamic_array.h"

typedef struct DynamicArray
{
    size_t size;
    size_t capacity;
    void **data;

    dynamic_array_equal_values equal_values;
    dynamic_array_print_index print_index;
    dynamic_array_free_value free_value;
} DynamicArray;

typedef struct DynamicArrayIterator
{
    size_t index;
    size_t size;
    void **data;
} DynamicArrayIterator;

static bool realloc_void_elements(DynamicArray *dyn_array, size_t new_capacity)
{
    void **data = realloc(dyn_array->data, sizeof(void*) * new_capacity);

    if (data == NULL)
    {
        return false;
    }

    dyn_array->capacity = new_capacity;
    dyn_array->data = data;

    return true;
}

static bool check_capacity_reallocation(DynamicArray *dyn_array)
{
    if (dyn_array->size == dyn_array->capacity)
    {
        return realloc_void_elements(dyn_array, dyn_array->capacity + DEFAULT_CAPACITY_SIZE);
    }
    else if ((dyn_array->size % DEFAULT_CAPACITY_SIZE) == 0 && dyn_array->capacity != DEFAULT_CAPACITY_SIZE)
    {
        return realloc_void_elements(dyn_array, dyn_array->size);
    }

    return true;
}

size_t dynamic_array_get_size(DynamicArray *dyn_array)
{
    return dyn_array->size;
}

size_t dynamic_array_get_capacity(DynamicArray *dyn_array)
{
    return dyn_array->capacity;
}

// Beware this could be dangerous, so please don't
// use this unless you need to and you know what you are doing.
void **dynamic_array_get_data(DynamicArray *dyn_array)
{
    return dyn_array->data;
}

DynamicArray *dynamic_array_init(dynamic_array_equal_values equal_values, dynamic_array_print_index print_index,
                                 dynamic_array_free_value free_value)
{
    DynamicArray *dyn_array = malloc(sizeof(DynamicArray));

    dyn_array->size = 0;
    dyn_array->capacity = DEFAULT_CAPACITY_SIZE;
    dyn_array->data = calloc(dyn_array->capacity, sizeof(void*));

    dyn_array->equal_values = equal_values;
    dyn_array->print_index = print_index;
    dyn_array->free_value = free_value;

    return dyn_array;
}

void dynamic_array_push(DynamicArray *dyn_array, void *value)
{
    if (dyn_array == NULL || dyn_array->data == NULL)
    {
        dyn_array->free_value(value);
        return;
    }

    if (!(check_capacity_reallocation(dyn_array)))
    {
        dyn_array->free_value(value);
        return;
    }

    dyn_array->data[dyn_array->size] = value;
    dyn_array->size++;
}

void dynamic_array_insert(DynamicArray *dyn_array, size_t index, void *value)
{
    if (dyn_array == NULL || dyn_array->data == NULL)
    {
        dyn_array->free_value(value);
        return;
    }

    if (index > dyn_array->size)
    {
        index = dyn_array->size;
    }

    if (!(check_capacity_reallocation(dyn_array)))
    {
        dyn_array->free_value(value);
        return;
    }

    for (size_t i = dyn_array->size; i > index; i--)
    {
        dyn_array->data[i] = dyn_array->data[i - 1];
    }

    dyn_array->data[index] = value;

    dyn_array->size++;
}

void dynamic_array_pop(DynamicArray *dyn_array)
{
    if (dyn_array == NULL || dyn_array->data == NULL || dyn_array->size == 0)
    {
        return;
    }

    dyn_array->size--;
    dyn_array->free_value(dyn_array->data[dyn_array->size]);
    check_capacity_reallocation(dyn_array);
}

void dynamic_array_pop_index(DynamicArray *dyn_array, size_t index)
{
    if (dyn_array == NULL || dyn_array->data == NULL
        || dyn_array->size == 0 || index >= dyn_array->size)
    {
        return;
    }

    dyn_array->size--;
    dyn_array->free_value(dyn_array->data[index]);

    for (size_t i = index; i < dyn_array->size; i++)
    {
        dyn_array->data[i] = dyn_array->data[i + 1];
    }

    check_capacity_reallocation(dyn_array);
}

void dynamic_array_remove(DynamicArray *dyn_array, void *value)
{
    if (dyn_array == NULL || dyn_array->data == NULL || dyn_array->size == 0)
    {
        return;
    }

    size_t index = 0;

    // Returns false if the value is not in the array.
    if (!(dynamic_array_index(dyn_array, &index, value)))
    {
        return;
    }

    dynamic_array_pop_index(dyn_array, index);
}

void *dynamic_array_at(DynamicArray *dyn_array, size_t index)
{
    if (dyn_array == NULL || dyn_array->data == NULL || index >= dyn_array->size)
    {
        return NULL;
    }

    return dyn_array->data[index];
}

bool dynamic_array_index(DynamicArray *dyn_array, size_t *index, void *value)
{
    if (dyn_array == NULL || dyn_array->data == NULL
        || dyn_array->size == 0 || index == NULL)
    {
        return false;
    }

    for (size_t i = 0; i < dyn_array->size; i++)
    {
        if (dyn_array->equal_values(value, dyn_array->data[i]))
        {
            *index = i;
            return true;
        }
    }

    return false;
}

bool dynamic_array_contains(DynamicArray *dyn_array, void *value)
{
    if (dyn_array == NULL || dyn_array->data == NULL 
        || dyn_array->size == 0)
    {
        return false;
    }

    for (size_t i = 0; i < dyn_array->size; i++)
    {
        if (dyn_array->equal_values(value, dyn_array->data[i]))
        {
            return true;
        }
    }

    return false;
}

void dynamic_array_reverse(DynamicArray *dyn_array)
{
    if (dyn_array == NULL || dyn_array->data == NULL || dyn_array->size == 0)
    {
        return;
    }

    size_t half_size = dyn_array->size / 2;
    size_t last_index = dyn_array->size - 1;
    void *temp = NULL;

    for (size_t i = 0; i < half_size; i++)
    {
        temp = dyn_array->data[i];
        dyn_array->data[i] = dyn_array->data[last_index - i];
        dyn_array->data[last_index - i] = temp;
    }
}

void dynamic_array_print(DynamicArray *dyn_array)
{
    if (dyn_array == NULL || dyn_array->data == NULL || dyn_array->size == 0)
    {
        printf("{}\n");
        return;
    }

    size_t i;

    printf("{");

    for (i = 0; i < dyn_array->size-1; i++)
    {
        dyn_array->print_index(dyn_array->data[i]);
        printf(", ");
    }

    dyn_array->print_index(dyn_array->data[i]);
    printf("}\n");
}

void dynamic_array_free(DynamicArray *dyn_array)
{
    if (dyn_array == NULL)
    {
        return;
    }

    if (dyn_array->data == NULL)
    {
        free(dyn_array);
        return;
    }

    for (size_t i = 0; i < dyn_array->size; i++)
    {
        dyn_array->free_value(dyn_array->data[i]);
    }

    free(dyn_array->data);
    free(dyn_array);
}

DynamicArrayIterator *dynamic_array_iterator_init(DynamicArray *dyn_array)
{
    DynamicArrayIterator *iter = malloc(sizeof(DynamicArrayIterator));

    iter->index = 0;
    iter->size = dyn_array->size;
    iter->data = dyn_array->data;

    return iter;
}

bool dynamic_array_iterator_iterate(DynamicArrayIterator *iter, void **value)
{
    if (iter == NULL || iter->data == NULL || iter->size == 0)
    {
        return false;
    }

    // We have reached the end.
    if (iter->index == iter->size)
    {
        // Reset everything back to the beginning.
        *value = NULL;
        iter->index = 0;
        return false;
    }

    (*value) = iter->data[iter->index];
    iter->index++;

    return true;
}

void dynamic_array_iterator_free(DynamicArrayIterator *iter)
{
    free(iter);
}

