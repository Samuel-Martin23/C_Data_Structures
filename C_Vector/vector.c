
#include "vector.h"

#define VEC_FREE                    0x00000001
#define VEC_SIZE_ZERO               0x00000002
#define VEC_TYPE                    0x00000004
#define VEC_SIZE_G                  0x00000008
#define VEC_SIZE_GE                 0x00000010

static bool check_warnings(vector_t *vec_data, int warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & VEC_FREE)
    {
        if (vec_data->data == NULL)
        {
            printf("%s: %swarning:%s vector data is freed%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_ZERO)
    {
        if (vec_data->size == 0)
        {
            printf("%s: %swarning:%s vector size is 0%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_TYPE)
    {
        template_t T = (template_t)check_value;

        if (vec_data->T != T)
        {
            printf("%s: %swarning:%s vector type does not equal the type of the new value%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_G)
    {
        int index = check_value;

        if (index < 0 || index > vec_data->size)
        {
            printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_GE)
    {
        int index = check_value;

        if (index < 0 || index >= vec_data->size)
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

void *new_void_elements(vector_t *vec_data)
{
    vec_data->allocated_mem += (int)(sizeof(void *) * (size_t)vec_data->capacity);
    return malloc(sizeof(void *) * (size_t)vec_data->capacity);
}

void *realloc_void_elements(vector_t *vec_data, int new_vector_capacity)
{
    vec_data->allocated_mem += (int)(sizeof(void *) * (size_t)(new_vector_capacity - vec_data->capacity));
    vec_data->capacity = new_vector_capacity;
    return realloc(vec_data->data, sizeof(void*) * (size_t)(new_vector_capacity));
}

void check_capacity_reallocation(vector_t *vec_data)
{
    if (vec_data->size % DEFAULT_CAPACITY_SIZE == 0 && vec_data->capacity != DEFAULT_CAPACITY_SIZE)
    {
        vec_data->data = realloc_void_elements(vec_data, vec_data->size);
    }
}

void *new_index(vector_t *vec_data, void *index_value)
{
    switch (vec_data->T)
    {
        case INT:
            {
                int *allocated_value = malloc(sizeof(int));
                *allocated_value = void_cast_int(index_value);
                vec_data->allocated_mem += (int)sizeof(int);
                index_value = allocated_value;
            }
            break;
        case DOUBLE:
            {
                double *allocated_value = malloc(sizeof(double));
                *allocated_value = void_cast_double(index_value);
                vec_data->allocated_mem += (int)sizeof(double);
                index_value = allocated_value;
            }
            break;
        case FLOAT:
            {
                float *allocated_value = malloc(sizeof(float));
                *allocated_value = void_cast_float(index_value);
                vec_data->allocated_mem += (int)sizeof(float);
                index_value = allocated_value;
            }
            break;
        case CHAR:
             {
                char key_data = void_cast_char(index_value);
                const char *allocated_value = malloc(sizeof(char) + 1);

                vec_data->allocated_mem += (int)(sizeof(char) + 1);
                memcpy((char*)allocated_value, &key_data, 1);
                *((char*)allocated_value + 1) = '\0';

                index_value = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *str_value = void_cast_str(index_value);
                size_t str_length = strlen(str_value);
                const char *allocated_value = malloc(sizeof(char) * (str_length + 1));

                vec_data->allocated_mem += (int)(sizeof(char) * (str_length + 1));
                memcpy((char*)allocated_value, str_value, str_length);
                *((char*)allocated_value + str_length) = '\0';

                index_value = (char*)allocated_value;
            }
            break;
    }

    return index_value;
}

void free_index(vector_t *vec_data, int index)
{
    vec_data->allocated_mem -= get_bytes(vec_data->T, vec_data->data[index]);
    free(vec_data->data[index]);
}

void null_index(vector_t *vec_data)
{
    vec_data->size--;
    vec_data->data[vec_data->size] = NULL;
}

void print_allocated_mem_vector(vector_t *vec_data)
{
    printf("Bytes Allocated: %d\n", vec_data->allocated_mem);
}

void print_vector_size(vector_t *vec_data)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "print_vector_size", -1))
    {
        return;
    }

    printf("Vector Size: %d\n", vec_data->size);
}

vector_t vector_init(template_t T, void *array, int size)
{
    vector_t new_vector;
    new_vector.size = size;
    new_vector.capacity = new_vector.size > DEFAULT_CAPACITY_SIZE ? new_vector.size + DEFAULT_CAPACITY_SIZE : DEFAULT_CAPACITY_SIZE;
    new_vector.T = T;
    new_vector.allocated_mem = 0;
    new_vector.data = new_void_elements(&new_vector);

    switch (new_vector.T)
    {
        case INT:
            {
                int *data_array = ((int*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = new_index(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = new_index(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = new_index(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = new_index(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array= (char**)((char*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = new_index(&new_vector, *data_array);
                    data_array++;
                }
            }
            break;
    }

    return new_vector;
}

void vector_push(vector_t *vec_data, template_t T, void *value)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_TYPE, "vector_push", (int)T))
    {
        return;
    }

    if (vec_data->capacity == vec_data->size)
    {
        vec_data->data = realloc_void_elements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
    }

    vec_data->data[vec_data->size] = new_index(vec_data, value);
    vec_data->size++;
}

void vector_insert(vector_t *vec_data, template_t T, void *value, int index)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_TYPE, "vector_insert", (int)T)
        || check_warnings(vec_data, VEC_SIZE_G, "vector_insert", index))
    {
        return;
    }

    if (vec_data->capacity == vec_data->size)
    {
        vec_data->data = realloc_void_elements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
    }

    for (int i = vec_data->size; i >= index; i--)
    {
        vec_data->data[i] = vec_data->data[i - 1];
    }

    vec_data->size++;
    vec_data->data[index] = new_index(vec_data, value);
}

void vector_extend(vector_t *vec_data, template_t T, void *array, int size_array)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_TYPE, "vector_extend", (int)T))
    {
        return;
    }

    int total_size = vec_data->size + size_array;

    switch (T)
    {
        case INT:
            {
                int *data_array = (int*)array;

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = realloc_void_elements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = new_index(vec_data, data_array);
                    data_array++;
                }

                vec_data->size = total_size;
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = realloc_void_elements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = new_index(vec_data, data_array);
                    data_array++;
                }

                vec_data->size = total_size;
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = realloc_void_elements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = new_index(vec_data, data_array);
                    data_array++;
                }

                vec_data->size = total_size;
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = realloc_void_elements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = new_index(vec_data, data_array);
                    data_array++;
                }

                vec_data->size = total_size;
            }
            break;
        case STR:
            {
                char **data_array= (char**)((char*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = realloc_void_elements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = new_index(vec_data, *data_array);
                    data_array++;
                }

                vec_data->size = total_size;
            }
            break;
    }
}

void vector_pop(vector_t *vec_data)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "vector_pop", -1))
    {
        return;
    }

    free_index(vec_data, vec_data->size-1);
    null_index(vec_data);

    check_capacity_reallocation(vec_data);
}

void vector_pop_index(vector_t *vec_data, int index)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE, "vector_pop_index", index))
    {
        return;
    }

    if (index == vec_data->size-1)
    {
        vector_pop(vec_data);
        return;
    }

    free_index(vec_data, index);

    for (int i = index; i < vec_data->size-1; i++)
    {
        vec_data->data[i] = vec_data->data[i + 1];
    }

    null_index(vec_data);

    check_capacity_reallocation(vec_data);
}

void vector_remove_value(vector_t *vec_data, template_t T, void *value)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_TYPE | VEC_SIZE_ZERO, "vector_remove_value", (int)T))
    {
        return;
    }

    for (int i = 0; i < vec_data->size; i++)
    {
        if (check_equal_value(vec_data->T, vec_data->data[i], value))
        {
            vector_pop_index(vec_data, i);
            return;
        }
    }

    printf("Warning in vector_remove_value: Value could not be found in the vector.\n");
}

void vector_reverse(vector_t *vec_data)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "vector_reverse", -1))
    {
        return;
    }

    int half_size = vec_data->size / 2;

    for (int i = 0; i < half_size; i++)
    {
        void* temp = vec_data->data[i];
        vec_data->data[i] = vec_data->data[(vec_data->size - 1) - i];
        vec_data->data[(vec_data->size - 1) - i] = temp;
    }
}

void vector_sort(vector_t *vec_data)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "vector_sort", -1))
    {
        return;
    }

    insertion_sort(vec_data->T, vec_data->data, vec_data->size);
}

void vector_clear(vector_t *vec_data)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "vector_clear", -1))
    {
        return;
    }

    for (int i = 0; i < vec_data->size; i++)
    {
        free_index(vec_data, i);
        vec_data->data[i] = NULL;
    }

    vec_data->size = 0;
}

void vector_free(vector_t *vec_data)
{
    if (check_warnings(vec_data, VEC_FREE, "vector_free", -1))
    {
        return;
    }

    vector_clear(vec_data);
    vec_data->allocated_mem -= (int)((size_t)vec_data->capacity * sizeof(void*));
    vec_data->size = -1;
    vec_data->capacity = -1;
    free(vec_data->data);
    vec_data->data = NULL;
}

void vector_print(vector_t *vec_data)
{
    if (check_warnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "vector_print", -1))
    {
        return;
    }

    int i;

    for (i = 0; i < vec_data->size-1; i++)
    {
        print_t(vec_data->T, vec_data->data[i], "|", "| ");
    }

    print_t(vec_data->T, vec_data->data[i], "|", "|\n");
}
