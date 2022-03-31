#include "vector.h"

typedef struct vector
{
    size_t size;
    size_t capacity;
    void **data;

    vector_equal_values equal_values;
    vector_print_index print_index;
    vector_free_index free_index;
} vector_t;

static void realloc_void_elements(vector_t *vec, size_t new_capacity)
{
    vec->capacity = new_capacity;
    vec->data = realloc(vec->data, sizeof(void*) * vec->capacity);
}

static void check_capacity_reallocation(vector_t *vec)
{
    if (vec->size == vec->capacity)
    {
        realloc_void_elements(vec, vec->capacity + DEFAULT_CAPACITY_SIZE);
    }
    else if ((vec->size % DEFAULT_CAPACITY_SIZE) == 0 && vec->capacity != DEFAULT_CAPACITY_SIZE)
    {
        realloc_void_elements(vec, vec->size);
    }
}

size_t vector_get_size(vector_t *vec)
{
    return vec->size;
}

size_t vector_get_capacity(vector_t *vec)
{
    return vec->capacity;
}

// Beware this could be dangerous, so please don't
// use this unless you need to and you know what you are doing.
void **vector_get_data(vector_t *vec)
{
    return vec->data;
}

vector_t *vector_init_alloc(vector_equal_values equal_values, vector_print_index print_index,
                            vector_free_index free_index)
{
    vector_t *vec = malloc(sizeof(vector_t));
    vec->size = 0;
    vec->capacity = DEFAULT_CAPACITY_SIZE;
    vec->data = malloc(sizeof(void*) * vec->capacity);
    memset(vec->data, 0, sizeof(*vec->data) * vec->capacity);

    vec->equal_values = equal_values;
    vec->print_index = print_index;
    vec->free_index = free_index;

    return vec;
}

void vector_push(vector_t *vec, void *alloc_value)
{
    check_capacity_reallocation(vec);

    vec->data[vec->size] = alloc_value;
    vec->size++;
}

void vector_insert(vector_t *vec, size_t index, void *alloc_value)
{
    check_capacity_reallocation(vec);

    for (size_t i = vec->size; i > index; i--)
    {
        vec->data[i] = vec->data[i - 1];
    }

    vec->data[index] = alloc_value;

    vec->size++;
}

void vector_pop(vector_t *vec)
{
    vec->size--;
    vec->free_index(vec->data[vec->size]);
    check_capacity_reallocation(vec);
}

void vector_pop_index(vector_t *vec, size_t index)
{
    vec->size--;
    vec->free_index(vec->data[index]);

    for (size_t i = index; i < vec->size; i++)
    {
        vec->data[i] = vec->data[i + 1];
    }

    check_capacity_reallocation(vec);
}

void vector_remove_value(vector_t *vec, void *value)
{
    vector_pop_index(vec, vector_get_value_index(vec, value));
}

size_t vector_get_value_index(vector_t *vec, void *value)
{
    for (size_t i = 0; i < vec->size; i++)
    {
        if (vec->equal_values(value, vec->data[i]))
        {
            return i;
        }
    }

    printf("BAD INDEX!!!\n");
    exit(1);
}

bool vector_contains_value(vector_t *vec, void *value)
{
    for (size_t i = 0; i < vec->size; i++)
    {
        if (vec->equal_values(value, vec->data[i]))
        {
            return true;
        }
    }

    return false;
}

void vector_reverse(vector_t *vec)
{
    size_t half_size = vec->size / 2;
    size_t last_index = vec->size - 1;
    void *temp = NULL;

    for (size_t i = 0; i < half_size; i++)
    {
        temp = vec->data[i];
        vec->data[i] = vec->data[last_index - i];
        vec->data[last_index - i] = temp;
    }
}

void vector_print(vector_t *vec)
{
    size_t i;

    printf("{");

    for (i = 0; i < vec->size-1; i++)
    {
        vec->print_index(vec->data[i]);
        printf(", ");
    }

    vec->print_index(vec->data[i]);
    printf("}\n");
}

void vector_free(vector_t *vec)
{
    for (size_t i = 0; i < vec->size; i++)
    {
        vec->free_index(vec->data[i]);
    }

    free(vec->data);
    free(vec);
}
