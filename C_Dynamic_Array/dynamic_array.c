#include "dynamic_array.h"

typedef struct dynamic_array
{
    size_t size;
    size_t capacity;
    void **data;

    size_t iter_index;

    dynamic_array_equal_values equal_values;
    dynamic_array_print_index print_index;
    dynamic_array_free_index free_index;
} dynamic_array_t;

static void realloc_void_elements(dynamic_array_t *dyn_array, size_t new_capacity)
{
    dyn_array->capacity = new_capacity;
    dyn_array->data = realloc(dyn_array->data, sizeof(void*) * dyn_array->capacity);
}

static void check_capacity_reallocation(dynamic_array_t *dyn_array)
{
    if (dyn_array->size == dyn_array->capacity)
    {
        realloc_void_elements(dyn_array, dyn_array->capacity + DEFAULT_CAPACITY_SIZE);
    }
    else if ((dyn_array->size % DEFAULT_CAPACITY_SIZE) == 0 && dyn_array->capacity != DEFAULT_CAPACITY_SIZE)
    {
        realloc_void_elements(dyn_array, dyn_array->size);
    }
}

size_t dynamic_array_get_size(dynamic_array_t *dyn_array)
{
    return dyn_array->size;
}

size_t dynamic_array_get_capacity(dynamic_array_t *dyn_array)
{
    return dyn_array->capacity;
}

// Beware this could be dangerous, so please don't
// use this unless you need to and you know what you are doing.
void **dynamic_array_get_data(dynamic_array_t *dyn_array)
{
    return dyn_array->data;
}

dynamic_array_t *dynamic_array_init_alloc(dynamic_array_equal_values equal_values, dynamic_array_print_index print_index,
                                            dynamic_array_free_index free_index)
{
    dynamic_array_t *dyn_array = malloc(sizeof(dynamic_array_t));

    dyn_array->size = 0;
    dyn_array->capacity = DEFAULT_CAPACITY_SIZE;
    dyn_array->data = calloc(dyn_array->capacity, sizeof(void*));

    dyn_array->iter_index = 0;

    dyn_array->equal_values = equal_values;
    dyn_array->print_index = print_index;
    dyn_array->free_index = free_index;

    return dyn_array;
}

void dynamic_array_push(dynamic_array_t *dyn_array, void *value)
{
    if (dyn_array == NULL)
    {
        return;
    }

    check_capacity_reallocation(dyn_array);

    dyn_array->data[dyn_array->size] = value;
    dyn_array->size++;
}

void dynamic_array_insert(dynamic_array_t *dyn_array, size_t index, void *value)
{
    if (dyn_array == NULL)
    {
        return;
    }

    if (index > dyn_array->size)
    {
        index = dyn_array->size;
    }

    check_capacity_reallocation(dyn_array);

    for (size_t i = dyn_array->size; i > index; i--)
    {
        dyn_array->data[i] = dyn_array->data[i - 1];
    }

    dyn_array->data[index] = value;

    dyn_array->size++;
}

void dynamic_array_pop(dynamic_array_t *dyn_array)
{
    if (dyn_array == NULL || dyn_array->size == 0)
    {
        return;
    }

    dyn_array->size--;
    dyn_array->free_index(dyn_array->data[dyn_array->size]);
    check_capacity_reallocation(dyn_array);
}

void dynamic_array_pop_index(dynamic_array_t *dyn_array, size_t index)
{
    if (dyn_array == NULL || dyn_array->size == 0 || index >= dyn_array->size)
    {
        return;
    }

    dyn_array->size--;
    dyn_array->free_index(dyn_array->data[index]);

    for (size_t i = index; i < dyn_array->size; i++)
    {
        dyn_array->data[i] = dyn_array->data[i + 1];
    }

    check_capacity_reallocation(dyn_array);
}

void dynamic_array_remove(dynamic_array_t *dyn_array, void *value)
{
    if (dyn_array == NULL || dyn_array->size == 0)
    {
        return;
    }

    size_t index = 0;
    bool is_found = dynamic_array_get_value_index(dyn_array, &index, value);

    if (!(is_found))
    {
        return;
    }

    dynamic_array_pop_index(dyn_array, index);
}

void *dynamic_array_at(dynamic_array_t *dyn_array, size_t index)
{
    if (dyn_array == NULL || index >= dyn_array->size)
    {
        return NULL;
    }

    return dyn_array->data[index];
}

bool dynamic_array_index(dynamic_array_t *dyn_array, size_t *index, void *value)
{
    if (dyn_array == NULL || dyn_array->size == 0 || index == NULL)
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

bool dynamic_array_contains(dynamic_array_t *dyn_array, void *value)
{
    if (dyn_array == NULL || dyn_array->size == 0)
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

void dynamic_array_reverse(dynamic_array_t *dyn_array)
{
    if (dyn_array == NULL || dyn_array->size == 0)
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

bool dynamic_array_iterate(dynamic_array_t *dyn_array, void **value)
{
    if (dyn_array == NULL || dyn_array->size == 0)
    {
        return false;
    }

    // We have reached the end.
    if (dyn_array->iter_index == dyn_array->size)
    {
        // Reset everything back to the beginning.
        *value = NULL;
        dyn_array->iter_index = 0;
        return false;
    }

    (*value) = dyn_array->data[dyn_array->iter_index];
    dyn_array->iter_index++;

    return true;
    
}

void dynamic_array_print(dynamic_array_t *dyn_array)
{
    if (dyn_array == NULL || dyn_array->size == 0)
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

void dynamic_array_free(dynamic_array_t *dyn_array)
{
    if (dyn_array == NULL)
    {
        return;
    }

    for (size_t i = 0; i < dyn_array->size; i++)
    {
        dyn_array->free_index(dyn_array->data[i]);
    }

    free(dyn_array->data);
    free(dyn_array);
}
