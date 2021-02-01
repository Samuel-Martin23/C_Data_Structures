
#include "vector.h"

#define VEC_FREE                    0x00000001
#define VEC_SIZE_ZERO               0x00000002
#define VEC_TYPE                    0x00000004
#define VEC_SIZE_G                  0x00000008
#define VEC_SIZE_GE                 0x00000010

static bool check_warnings(vector_t *vec, int warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & VEC_FREE)
    {
        if (vec->data == NULL)
        {
            printf("%s: %swarning:%s vector data is freed%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_ZERO)
    {
        if (vec->size == 0)
        {
            printf("%s: %swarning:%s vector size is 0%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_TYPE)
    {
        template_t T = (template_t)check_value;

        if (vec->T != T)
        {
            printf("%s: %swarning:%s vector type does not equal the type of the new value%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_G)
    {
        int index = check_value;

        if (index < 0 || index > vec->size)
        {
            printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_GE)
    {
        int index = check_value;

        if (index < 0 || index >= vec->size)
        {
            printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    return false;
}

void insertion_sort(template_t T, void **array, int array_size)
{
    int j;
    void *key;

    for (int i = 1; i < array_size; i++)
    {
        key = array[i];
        j = i - 1;

        while (j >= 0 && check_greater_value(T, array[j], key))
        {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

void new_void_elements(vector_t *vec)
{
    size_t number_of_bytes = sizeof(void*);

    vec->allocated_mem += (int)(number_of_bytes * (size_t)vec->capacity);
    vec->data = malloc(number_of_bytes * (size_t)vec->capacity);
}

void realloc_void_elements(vector_t *vec, int new_capacity)
{
    size_t number_of_bytes = sizeof(void*);

    vec->allocated_mem += (int)(number_of_bytes * (size_t)(new_capacity - vec->capacity));
    vec->capacity = new_capacity;
    vec->data = realloc(vec->data, number_of_bytes * (size_t)(new_capacity));
}

void capacity_reallocation(vector_t *vec, int size)
{
    if (vec->capacity == size)
    {
        realloc_void_elements(vec, vec->capacity + DEFAULT_CAPACITY_SIZE);
    }
    else if (size % DEFAULT_CAPACITY_SIZE == 0 && vec->capacity != DEFAULT_CAPACITY_SIZE)
    {
        realloc_void_elements(vec, size);
    }
}

void new_index(vector_t *vec, int i, void *index_value)
{
    size_t number_of_bytes = 0;

    switch (vec->T)
    {
        case INT:
            {
                number_of_bytes = sizeof(int);
                int *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_int(index_value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[i] = allocated_value;
            }
            break;
        case DOUBLE:
            {
                number_of_bytes = sizeof(double);
                double *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_double(index_value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[i] = allocated_value;
            }
            break;
        case FLOAT:
            {
                number_of_bytes = sizeof(float);
                float *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_float(index_value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[i] = allocated_value;
            }
            break;
        case CHAR:
             {
                number_of_bytes = sizeof(char) + 1;
                char key_data = void_cast_char(index_value);
                const char *allocated_value = malloc(number_of_bytes);

                vec->allocated_mem += (int)(number_of_bytes);
                memcpy((char*)allocated_value, &key_data, 1);
                *((char*)allocated_value + 1) = '\0';

                vec->data[i] = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *str_value = void_cast_str(index_value);
                size_t str_length = strlen(str_value);
                number_of_bytes = sizeof(char) * (str_length + 1);
                const char *allocated_value = malloc(number_of_bytes);

                vec->allocated_mem += (int)(number_of_bytes);
                memcpy((char*)allocated_value, str_value, str_length);
                *((char*)allocated_value + str_length) = '\0';

                vec->data[i] = (char*)allocated_value;
            }
            break;
         case BOOL:
            {
                number_of_bytes = sizeof(bool);
                bool *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_bool(index_value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[i] = allocated_value;
            }
            break;
    }
}

void free_index(vector_t *vec, int index)
{
    vec->allocated_mem -= get_bytes(vec->T, vec->data[index]);
    free(vec->data[index]);
}

void free_indices(vector_t *vec)
{
    for (int i = 0; i < vec->size; i++)
    {
        free_index(vec, i);
        vec->data[i] = NULL;
    }

    vec->size = 0;
}

void null_index(vector_t *vec)
{
    vec->size--;
    vec->data[vec->size] = NULL;
}

bool check_vector_null_init(vector_t *vec, template_t T, void *value, int size)
{
    if (vec->data == NULL)
    {
        *vec = vector_init(T, value, size);
        return true;
    }

    return false;
}

void pop_last_index(vector_t *vec)
{
    free_index(vec, vec->size-1);
    null_index(vec);
    capacity_reallocation(vec, vec->size);
}

void print_allocated_mem_vector(vector_t *vec)
{
    printf("Bytes Allocated: %d\n", vec->allocated_mem);
}

void print_vector_size(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE, "print_vector_size", -1))
    {
        return;
    }

    printf("Vector Size: %d\n", vec->size);
}

vector_t vector_init(template_t T, void *array, int size)
{
    vector_t new_vector;
    new_vector.size = size;
    new_vector.capacity = new_vector.size > DEFAULT_CAPACITY_SIZE ? new_vector.size + DEFAULT_CAPACITY_SIZE : DEFAULT_CAPACITY_SIZE;
    new_vector.T = T;
    new_vector.allocated_mem = 0;

    if (new_vector.size == 0)
    {
        return new_vector;
    }

    new_void_elements(&new_vector);

    switch (new_vector.T)
    {
        case INT:
            {
                int *data_array = ((int*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_index(&new_vector, i, data_array);
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_index(&new_vector, i, data_array);
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_index(&new_vector, i, data_array);
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_index(&new_vector, i, data_array);
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array = ((char**)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_index(&new_vector, i, *data_array);
                    data_array++;
                }
            }
            break;
        case BOOL:
            {
                bool *data_array = ((bool*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_index(&new_vector, i, data_array);
                    data_array++;
                }
            }
            break;
    }

    return new_vector;
}

void vector_push(vector_t *vec, template_t T, void *value)
{
    if (check_vector_null_init(vec, T, value, 1))
    {
        return;
    }

    if (check_warnings(vec, VEC_TYPE, "vector_push", (int)T))
    {
        return;
    }

    capacity_reallocation(vec, vec->size);
    new_index(vec, vec->size, value);
    vec->size++;
}

void vector_insert(vector_t *vec, template_t T, void *value, int index)
{
    if (index == 0)
    {
        if (check_vector_null_init(vec, T, value, 1))
        {
            return;
        }
    }

    if (check_warnings(vec, VEC_TYPE, "vector_insert", (int)T)
        || check_warnings(vec, VEC_SIZE_G, "vector_insert", index))
    {
        return;
    }

    capacity_reallocation(vec, vec->size);

    for (int i = vec->size; i > index; i--)
    {
        vec->data[i] = vec->data[i - 1];
    }

    vec->size++;
    new_index(vec, index, value);
}

void vector_extend(vector_t *vec, template_t T, void *array, int size_array)
{
    if (check_vector_null_init(vec, T, array, size_array))
    {
        return;
    }

    if (check_warnings(vec, VEC_TYPE, "vector_extend", (int)T))
    {
        return;
    }

    int total_size = vec->size + size_array;

    switch (T)
    {
        case INT:
            {
                int *data_array = (int*)array;

                for (int i = vec->size; i < total_size; i++)
                {
                    capacity_reallocation(vec, i);
                    new_index(vec, i, data_array);
                    data_array++;
                }

                vec->size = total_size;
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = vec->size; i < total_size; i++)
                {
                    capacity_reallocation(vec, i);
                    new_index(vec, i, data_array);
                    data_array++;
                }

                vec->size = total_size;
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = vec->size; i < total_size; i++)
                {
                    capacity_reallocation(vec, i);
                    new_index(vec, i, data_array);
                    data_array++;
                }

                vec->size = total_size;
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = vec->size; i < total_size; i++)
                {
                    capacity_reallocation(vec, i);
                    new_index(vec, i, data_array);
                    data_array++;
                }

                vec->size = total_size;
            }
            break;
        case STR:
            {
                char **data_array= (char**)((char*)array);

                for (int i = vec->size; i < total_size; i++)
                {
                    capacity_reallocation(vec, i);
                    new_index(vec, i, *data_array);
                    data_array++;
                }

                vec->size = total_size;
            }
            break;
        case BOOL:
            {
                bool *data_array = (bool*)array;

                for (int i = vec->size; i < total_size; i++)
                {
                    capacity_reallocation(vec, i);
                    new_index(vec, i, data_array);
                    data_array++;
                }

                vec->size = total_size;
            }
            break;
    }
}

void vector_pop(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO, "vector_pop", -1))
    {
        return;
    }

    pop_last_index(vec);
}

void vector_pop_index(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE, "vector_pop_index", index))
    {
        return;
    }

    if (index == vec->size-1)
    {
        pop_last_index(vec);
        return;
    }

    free_index(vec, index);

    for (int i = index; i < vec->size-1; i++)
    {
        vec->data[i] = vec->data[i + 1];
    }

    null_index(vec);

    capacity_reallocation(vec, vec->size);
}

void vector_remove_value(vector_t *vec, template_t T, void *value)
{
    if (check_warnings(vec, VEC_FREE | VEC_TYPE | VEC_SIZE_ZERO, "vector_remove_value", (int)T))
    {
        return;
    }

    for (int i = 0; i < vec->size; i++)
    {
        if (check_equal_value(vec->T, vec->data[i], value))
        {
            vector_pop_index(vec, i);
            return;
        }
    }
}

void *vector_at(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE, "vector_at", index))
    {
        return NULL;
    }

    return vec->data[index];
}

bool vector_check_value(vector_t *vec, template_t T, void *value)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO | VEC_TYPE, "vector_check_value", (int)T))
    {
        return false;
    }

    for (int i = 0; i < vec->size; i++)
    {
        if (check_equal_value(T, value, vec->data[i]))
        {
            return true;
        }
    }

    return false;
}

void vector_reverse(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO, "vector_reverse", -1))
    {
        return;
    }

    void *temp = NULL;
    int half_size = vec->size / 2;

    for (int i = 0; i < half_size; i++)
    {
        temp = vec->data[i];
        vec->data[i] = vec->data[(vec->size - 1) - i];
        vec->data[(vec->size - 1) - i] = temp;
    }
}

void vector_sort(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO, "vector_sort", -1))
    {
        return;
    }

    insertion_sort(vec->T, vec->data, vec->size);
}

void vector_clear(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO, "vector_clear", -1))
    {
        return;
    }

    free_indices(vec);
}

void vector_free(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE, "vector_free", -1))
    {
        return;
    }

    free_indices(vec);
    vec->allocated_mem -= (vec->capacity * (int)sizeof(void*));
    vec->size = -1;
    vec->capacity = -1;
    free(vec->data);
    vec->data = NULL;
}

void vector_print(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO, "vector_print", -1))
    {
        return;
    }

    int i;

    for (i = 0; i < vec->size-1; i++)
    {
        print_t(vec->T, vec->data[i], "|", "| ");
    }

    print_t(vec->T, vec->data[i], "|", "|\n");
}
