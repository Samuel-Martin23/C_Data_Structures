
#include "vector.h"

#define VEC_FREE                    0x00000001
#define VEC_SIZE_ZERO               0x00000002
#define VEC_TYPE                    0x00000004
#define VEC_SIZE_G                  0x00000008
#define VEC_SIZE_GE                 0x00000010
#define VEC_CAPACITY                0x00000020
#define TURN_OFF_WARNING            0x00000040

#define WARNING_PLACEHOLDER         -1

static bool check_warnings(vector_t *vec, int warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & VEC_FREE)
    {
        if (vec->data == NULL)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector data is NULL%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & VEC_SIZE_ZERO)
    {
        if (vec->size == 0)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector size is 0%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & VEC_TYPE)
    {
        template_t T = (template_t)check_value;

        if (vec->T != T)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector type does not equal the new data type%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & VEC_SIZE_G)
    {
        int index = check_value;

        if (index < 0 || index > vec->size)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & VEC_SIZE_GE)
    {
        int index = check_value;

        if (index < 0 || index >= vec->size)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & VEC_CAPACITY)
    {
        if (vec->capacity)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector already contains capacity%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    return false;
}

void insertion_sort(template_t T, void **array, int array_size)
{
    int j = 0;
    void *key = NULL;

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
    size_t number_of_bytes = 0;

    vec->capacity = vec->size > DEFAULT_CAPACITY_SIZE ? vec->size + DEFAULT_CAPACITY_SIZE : DEFAULT_CAPACITY_SIZE;
    number_of_bytes = sizeof(void*) * (size_t)vec->capacity;
    vec->allocated_mem += (int)(number_of_bytes);
    vec->data = malloc(number_of_bytes);
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
    else if (size == 0)
    {
        vector_free(vec);
    }
}

void new_index(vector_t *vec, int index, void *value)
{
    switch (vec->T)
    {
        case INT:
            {
                size_t number_of_bytes = (size_t)get_bytes(vec->T, value);
                int *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_int(value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[index] = allocated_value;
            }
            break;
        case DOUBLE:
            {
                size_t number_of_bytes = (size_t)get_bytes(vec->T, value);
                double *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_double(value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[index] = allocated_value;
            }
            break;
        case FLOAT:
            {
                size_t number_of_bytes = (size_t)get_bytes(vec->T, value);
                float *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_float(value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[index] = allocated_value;
            }
            break;
        case CHAR:
             {
                size_t number_of_bytes = (size_t)get_bytes(vec->T, value);
                char cast_value = void_cast_char(value);
                const char *allocated_value = malloc(number_of_bytes);

                vec->allocated_mem += (int)(number_of_bytes);
                memcpy((char*)allocated_value, &cast_value, 1);

                vec->data[index] = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *cast_value = void_cast_str(value);
                size_t number_of_bytes = (size_t)get_bytes(vec->T, (void*)cast_value);
                const char *allocated_value = malloc(number_of_bytes);
                
                vec->allocated_mem += (int)(number_of_bytes);
                number_of_bytes--;
                memcpy((char*)allocated_value, cast_value, number_of_bytes);
                *((char*)allocated_value + number_of_bytes) = '\0';

                vec->data[index] = (char*)allocated_value;
            }
            break;
         case BOOL:
            {
                size_t number_of_bytes = (size_t)get_bytes(vec->T, value);
                bool *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_bool(value);
                vec->allocated_mem += (int)number_of_bytes;
                vec->data[index] = allocated_value;
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

bool check_vector_null_init(vector_t *vec, template_t T, void *data, int size)
{
    if (vec->data == NULL)
    {
        *vec = vector_init(T, data, size);
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
    if (check_warnings(vec, VEC_FREE, "print_vector_size", WARNING_PLACEHOLDER))
    {
        return;
    }

    printf("Vector Size: %d\n", vec->size);
}

vector_t vector_init(template_t T, void *data, int size)
{
    vector_t new_vector;
    new_vector.size = size;
    new_vector.data = NULL;
    new_vector.T = T;
    new_vector.allocated_mem = 0;

    if (new_vector.size == 0)
    {
        new_vector.capacity = 0;
        return new_vector;
    }

    new_void_elements(&new_vector);
    
    switch (new_vector.T)
    {
        case INT:
        case DOUBLE:
        case FLOAT:
        case CHAR:
        case BOOL:
            for (int i = 0; i < new_vector.size; i++)
            {
                new_index(&new_vector, i, data);
                data += get_bytes(new_vector.T, data);
            }
            break;
        case STR:
            {
                char **str_array = ((char**)data);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_index(&new_vector, i, *str_array);
                    str_array++;
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

    convert_2d_str(T, &value);

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

    convert_2d_str(T, &value);

    new_index(vec, index, value);
}

void vector_extend(vector_t *vec, template_t T, void *data, int size)
{
    if (check_vector_null_init(vec, T, data, size))
    {
        return;
    }

    if (check_warnings(vec, VEC_TYPE, "vector_extend", (int)T))
    {
        return;
    }

    int total_size = vec->size + size;

    switch (T)
    {
        case INT:
        case DOUBLE:
        case FLOAT:
        case CHAR:
        case BOOL:
            for (int i = vec->size; i < total_size; i++)
            {
                capacity_reallocation(vec, i);
                new_index(vec, i, data);
                data += get_bytes(vec->T, data);
            }
            vec->size = total_size;
            break;
        case STR:
            {
                char **str_array = ((char**)data);

                for (int i = vec->size; i < total_size; i++)
                {
                    capacity_reallocation(vec, i);
                    new_index(vec, i, *str_array);
                    str_array++;
                }

                vec->size = total_size;
            }
            break;
    }
}

void vector_pop(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO,
        "vector_pop", WARNING_PLACEHOLDER))
    {
        return;
    }

    pop_last_index(vec);
}

void vector_pop_index(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE,
        "vector_pop_index", index))
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
    if (check_warnings(vec, VEC_FREE | VEC_TYPE | VEC_SIZE_ZERO,
        "vector_remove_value", (int)T))
    {
        return;
    }

    convert_2d_str(T, &value);

    int index = vector_check_value(vec, T, value);

    if (index >= 0)
    {
        vector_pop_index(vec, index);
        return;
    }
}

void *vector_at(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE,
        "vector_at", index))
    {
        return NULL;
    }

    return vec->data[index];
}

int vector_check_value(vector_t *vec, template_t T, void *value)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO | VEC_TYPE,
        "vector_check_value", (int)T))
    {
        return -1;
    }

    for (int i = 0; i < vec->size; i++)
    {
        if (check_equal_value(T, value, vec->data[i]))
        {
            return i;
        }
    }

    return -1;
}

void vector_reverse(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO,
        "vector_reverse", WARNING_PLACEHOLDER))
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
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO,
        "vector_sort", WARNING_PLACEHOLDER))
    {
        return;
    }

    insertion_sort(vec->T, vec->data, vec->size);
}

void vector_copy(vector_t *vec_dest, vector_t *vec_src)
{
    if (check_warnings(vec_src, VEC_FREE | VEC_SIZE_ZERO | VEC_TYPE,
        "vector_copy", (int)vec_dest->T) ||
        check_warnings(vec_dest, VEC_CAPACITY, "vector_copy", WARNING_PLACEHOLDER))
    {
        return;
    }

    vec_dest->size = vec_src->size;
    vec_dest->data = NULL;
    vec_dest->T = vec_src->T;
    vec_dest->allocated_mem = 0;

    if (vec_dest->size == 0)
    {
        vec_dest->capacity = 0;
        return;
    }

    new_void_elements(vec_dest);

    switch (vec_dest->T)
    {
        case INT:
        case DOUBLE:
        case FLOAT:
        case CHAR:
        case STR:
        case BOOL:
            for (int i = 0; i < vec_dest->size; i++)
            {
                new_index(vec_dest, i, vec_src->data[i]);
            }
            break;
    }
}

void vector_clear(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO,
        "vector_clear", WARNING_PLACEHOLDER))
    {
        return;
    }

    free_indices(vec);
}

void vector_free(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE, "vector_free", WARNING_PLACEHOLDER))
    {
        return;
    }

    free_indices(vec);
    vec->allocated_mem -= (vec->capacity * (int)sizeof(void*));
    vec->size = 0;
    vec->capacity = 0;
    free(vec->data);
    vec->data = NULL;
}

void vector_print(vector_t *vec)
{
    if (check_warnings(vec, VEC_FREE | VEC_SIZE_ZERO,
        "vector_print", WARNING_PLACEHOLDER))
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
