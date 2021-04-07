#include "vector.h"

#define VEC_NULL                    0x00000001u
#define VEC_SIZE_ZERO               0x00000002u
#define VEC_TYPE_NONE               0x00000004u
#define VEC_SIZE_G                  0x00000008u
#define VEC_SIZE_GE                 0x00000010u
#define VEC_CAPACITY                0x00000020u
#define VEC_TYPE                    0x00000040u
#define VEC_STEP                    0x00000080u
#define TURN_OFF_WARNING            0x00000100u

#define WARNING_PLACEHOLDER         -1

typedef struct vector
{
    int size;
    int capacity;
    void **data;
    template_t T;
} vector_t;

static bool check_warnings(vector_t *vec, u_int16_t warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & VEC_NULL)
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

    if (warning_code & VEC_TYPE_NONE)
    {
        if (vec->T == NONE)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector type equals NONE%s\n", function_name, purple, white, reset);
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

        if (index < 0)
        {
            if ((vec->size + index) < 0)
            {
                if (!(warning_code & TURN_OFF_WARNING))
                {
                    printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
                }

                return true;
            }
        }
        else if (index >= vec->size)
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

    if (warning_code & VEC_TYPE)
    {
        template_t T = (template_t)check_value;

        if (vec->T != T)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector dest type does not equal vector src type%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & VEC_STEP)
    {
        int step = check_value;

        if (step <= 0)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s slice step cannot be less than or equal to zero%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    return false;
}

static void insertion_sort(template_t T, void **array, int size)
{
    int j = 0;
    void *key = NULL;

    for (int i = 1; i < size; i++)
    {
        key = array[i];
        j = i - 1;

        while (j >= 0 && check_greater_value(T, array[j], key, false))
        {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

static int ceil_int(int x, int y) 
{
    return (int)ceil(x / (double)y);
}

static int round_nearest_capacity(int value)
{
    return (int)(ceil(value / (double)DEFAULT_CAPACITY_SIZE) * DEFAULT_CAPACITY_SIZE);
}

static void new_void_elements(vector_t *vec)
{
    size_t number_of_bytes = 0;

    vec->capacity = round_nearest_capacity(vec->size);
    number_of_bytes = sizeof(void*) * (size_t)vec->capacity;
    vec->data = malloc(number_of_bytes);

    mem_usage.allocated += (u_int32_t)(number_of_bytes);
}

static void realloc_void_elements(vector_t *vec, int new_capacity)
{
    size_t number_of_bytes = sizeof(void*);

    vec->data = realloc(vec->data, number_of_bytes * (size_t)(new_capacity));
    mem_usage.allocated += (u_int32_t)(number_of_bytes * (size_t)(new_capacity - vec->capacity));
    vec->capacity = new_capacity;
}

static void capacity_reallocation(vector_t *vec, int size)
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

static void null_index(vector_t *vec)
{
    vec->size--;
    vec->data[vec->size] = NULL;
}

static void pop_last_index(vector_t *vec)
{
    free_T_value(vec->T, vec->data[vec->size-1]);
    null_index(vec);
    capacity_reallocation(vec, vec->size);
}

static int check_T_value(vector_t *vec, void *value)
{
    switch (vec->T)
    {
        case INT:
            return vector_check_value(vec, void_cast_int(value));
        case DOUBLE:
            return vector_check_value(vec, void_cast_double(value));
        case FLOAT:
            return vector_check_value(vec, void_cast_float(value));
        case CHAR:
            return vector_check_value(vec, void_cast_char(value));
        case STR:
            return vector_check_value(vec, void_cast_str(value));
        case BOOL:
            return vector_check_value(vec, void_cast_bool(value));
        case NONE:
            break;
    }

    return -1;
}

int get_vector_size(vector_t *vec)
{
    return vec->size;
}

int get_vector_capacity(vector_t *vec)
{
    return vec->capacity;
}

template_t get_vector_template(vector_t *vec)
{
    return vec->T;
}

vector_t *vector_init(template_t T, int size, ...)
{
    size_t number_of_bytes = sizeof(vector_t);

    vector_t *new_vector = malloc(number_of_bytes);
    new_vector->size = size;
    new_vector->data = NULL;
    new_vector->T = T;

    mem_usage.allocated += (u_int32_t)(number_of_bytes);

    if (new_vector->size <= 0)
    {
        new_vector->size = 0;
        new_vector->capacity = 0;
        return new_vector;
    }

    new_void_elements(new_vector);

    va_list args;
    va_start(args, size);

    for (int i = 0; i < new_vector->size; i++)
    {
        new_vector->data[i] = new_arg_T_value(new_vector->T, args);
    }

    va_end(args);

    return new_vector;
}

void vector_push(vector_t *vec, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE,
        "vector_push", WARNING_PLACEHOLDER))
    {
        return;
    }

    va_list args;
    va_start(args, vec);

    void *value = new_arg_T_value(vec->T, args);

    va_end(args);

    capacity_reallocation(vec, vec->size);
    vec->data[vec->size] = value;
    vec->size++;
}

void vector_insert(vector_t *vec, int index, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_G,
        "vector_insert", index))
    {
        return;
    }

    capacity_reallocation(vec, vec->size);

    for (int i = vec->size; i > index; i--)
    {
        vec->data[i] = vec->data[i - 1];
    }

    va_list args;
    va_start(args, index);

    vec->data[index] = new_arg_T_value(vec->T, args);

    va_end(args);

    vec->size++;
}

void vector_extend(vector_t *vec, int size, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE,
        "vector_extend", WARNING_PLACEHOLDER))
    {
        return;
    }

    int original_size = vec->size;
    vec->size += size;

    va_list args;
    va_start(args, size);

    for (int i = original_size; i < vec->size; i++)
    {
        capacity_reallocation(vec, i);
        vec->data[i] = new_arg_T_value(vec->T, args);
    }

    va_end(args);
}

void vector_pop(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO,
        "vector_pop", WARNING_PLACEHOLDER))
    {
        return;
    }

    pop_last_index(vec);
}

void vector_pop_index(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO | VEC_SIZE_GE,
        "vector_pop_index", index))
    {
        return;
    }

    int last_index = vec->size - 1;

    if (index == last_index)
    {
        pop_last_index(vec);
        return;
    }

    free_T_value(vec->T, vec->data[index]);

    for (int i = index; i < last_index; i++)
    {
        vec->data[i] = vec->data[i + 1];
    }

    null_index(vec);

    capacity_reallocation(vec, vec->size);
}

void vector_remove_value(vector_t *vec, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO,
        "vector_remove_value", WARNING_PLACEHOLDER))
    {
        return;
    }

    va_list args;
    va_start(args, vec);

    void *value = new_arg_T_value(vec->T, args);

    va_end(args);

    int index = check_T_value(vec, value);

    if (index >= 0)
    {
        vector_pop_index(vec, index);
    }

    free_T_value(vec->T, value);
}

void *vector_at(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO | VEC_SIZE_GE,
        "vector_at", index))
    {
        return NULL;
    }

    return index < 0 ? vec->data[vec->size + index] : vec->data[index];
}

void vector_at_range(vector_t *vec_dest, vector_t *vec_src, int start, int end, int step)
{
    if (check_warnings(vec_src, VEC_NULL | VEC_SIZE_ZERO,
        "vector_at_range", WARNING_PLACEHOLDER) ||
        check_warnings(vec_dest, VEC_CAPACITY | VEC_STEP,
        "vector_at_range", step))
    {
        return;
    }

    if (start < 0)
    {
        start = vec_src->size + start;
    }

    if (end == 0)
    {
        end = vec_src->size;
    }
    else if (end < 0)
    {
        end = vec_src->size + end;
    }

    if (start < 0 || end > vec_src->size || (end - start) <= 0)
    {
        printf("vector_at_range: \033[1;95mwarning:\033[1;97m indices out of range\033[0m\n");
        return;
    }

    int step_counter = 0;
    vec_dest->size = ceil_int((end - start), step);
    vec_dest->T = vec_src->T;
    step--;

    new_void_elements(vec_dest);

    for (int i = 0; i < vec_dest->size; i++)
    {
        vec_dest->data[i] = new_T_value(vec_dest->T, vec_src->data[i + start + step_counter]);
        step_counter += step;
    }
}

int vector_check_value(vector_t *vec, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO,
        "vector_check_value", WARNING_PLACEHOLDER))
    {
        return -1;
    }

    va_list args;
    va_start(args, vec);

    void *value = new_arg_T_value(vec->T, args);

    va_end(args);

    for (int i = 0; i < vec->size; i++)
    {
        if (check_equal_value(vec->T, value, vec->data[i], false))
        {
            free_T_value(vec->T, value);
            return i;
        }
    }

    free_T_value(vec->T, value);
    return -1;
}

void vector_reverse(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO,
        "vector_reverse", WARNING_PLACEHOLDER))
    {
        return;
    }

    void *temp = NULL;
    int half_size = vec->size / 2;
    int last_index = vec->size - 1;

    for (int i = 0; i < half_size; i++)
    {
        temp = vec->data[i];
        vec->data[i] = vec->data[last_index - i];
        vec->data[last_index - i] = temp;
    }
}

void vector_sort(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO,
        "vector_sort", WARNING_PLACEHOLDER))
    {
        return;
    }

    insertion_sort(vec->T, vec->data, vec->size);
}

void vector_copy(vector_t *vec_dest, vector_t *vec_src)
{
    if (check_warnings(vec_src, VEC_NULL | VEC_SIZE_ZERO,
        "vector_copy", WARNING_PLACEHOLDER) ||
        check_warnings(vec_dest, VEC_CAPACITY, "vector_copy", WARNING_PLACEHOLDER))
    {
        return;
    }

    vec_dest->size = vec_src->size;
    vec_dest->data = NULL;

    if (vec_dest->size == 0)
    {
        vec_dest->capacity = 0;
        return;
    }

    vec_dest->T = vec_src->T;

    new_void_elements(vec_dest);

    for (int i = 0; i < vec_dest->size; i++)
    {
        vec_dest->data[i] = new_T_value(vec_dest->T, vec_src->data[i]);
    }
}

void vector_free(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL, "vector_free", WARNING_PLACEHOLDER))
    {
        return;
    }

    for (int i = 0; i < vec->size; i++)
    {
        free_T_value(vec->T, vec->data[i]);
        vec->data[i] = NULL;
    }

    mem_usage.freed += (u_int32_t)(vec->capacity * (int)sizeof(void*));
    mem_usage.freed += (u_int32_t)(sizeof(vector_t));

    vec->size = 0;
    vec->capacity = 0;
    vec->T = NONE;
    free(vec->data);
    vec->data = NULL;
    free(vec);
}

void vector_print(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_SIZE_ZERO,
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
