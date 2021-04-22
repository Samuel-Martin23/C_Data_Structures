#include "vector.h"

#define VEC_NULL                    0x00000001u
#define VEC_TYPE_NONE               0x00000002u
#define VEC_SIZE_ZERO               0x00000004u
#define VEC_SIZE_G                  0x00000008u
#define VEC_SIZE_GE                 0x00000010u
#define VEC_ALLOC                   0x00000020u
#define VEC_TYPE                    0x00000040u
#define VEC_STEP                    0x00000080u
#define TURN_OFF_WARNING            0x00000100u

#define WARNING_PLACEHOLDER         -1

typedef struct vector
{
    int size;
    int capacity;
    template_t T;
    union
    {
        int *data_int;
        double *data_double;
        float *data_float;
        char *data_char;
        char **data_str;
        bool *data_bool;
    };

} vector_t;

static bool check_warnings(vector_t *vec, u_int16_t warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & VEC_NULL)
    {
        if (vec == NULL)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector is NULL%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & VEC_ALLOC)
    {
        if (vec != NULL)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s vector is not NULL%s\n", function_name, purple, white, reset);
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

static void insertion_sort_int(vector_t *vec)
{
    int j = 0;
    int key = 0;

    for (int i = 1; i < vec->size; i++)
    {
        key = vec->data_int[i];
        j = i - 1;

        while (j >= 0 && vec->data_int[j] > key)
        {
            vec->data_int[j + 1] = vec->data_int[j];
            j--;
        }

        vec->data_int[j + 1] = key;
    }
}

static void insertion_sort_double(vector_t *vec)
{
    int j = 0;
    double key = 0.0;

    for (int i = 1; i < vec->size; i++)
    {
        key = vec->data_double[i];
        j = i - 1;

        while (j >= 0 && vec->data_double[j] > key)
        {
            vec->data_double[j + 1] = vec->data_double[j];
            j--;
        }

        vec->data_double[j + 1] = key;
    }
}

static void insertion_sort_float(vector_t *vec)
{
    int j = 0;
    float key = 0.0f;

    for (int i = 1; i < vec->size; i++)
    {
        key = vec->data_float[i];
        j = i - 1;

        while (j >= 0 && vec->data_float[j] > key)
        {
            vec->data_float[j + 1] = vec->data_float[j];
            j--;
        }

        vec->data_float[j + 1] = key;
    }
}

static void insertion_sort_char(vector_t *vec)
{
    int j = 0;
    char key = 0;

    for (int i = 1; i < vec->size; i++)
    {
        key = vec->data_char[i];
        j = i - 1;

        while (j >= 0 && vec->data_char[j] > key)
        {
            vec->data_char[j + 1] = vec->data_char[j];
            j--;
        }

        vec->data_char[j + 1] = key;
    }
}

static void insertion_sort_str(vector_t *vec)
{
    int j = 0;
    char *key = "";

    for (int i = 1; i < vec->size; i++)
    {
        key = vec->data_str[i];
        j = i - 1;

        while (j >= 0 && get_ascii_size(vec->data_str[j]) > get_ascii_size(key))
        {
            vec->data_str[j + 1] = vec->data_str[j];
            j--;
        }

        vec->data_str[j + 1] = key;
    }
}

static void insertion_sort_bool(vector_t *vec)
{
    int j = 0;
    bool key = false;

    for (int i = 1; i < vec->size; i++)
    {
        key = vec->data_bool[i];
        j = i - 1;

        while (j >= 0 && vec->data_bool[j] > key)
        {
            vec->data_bool[j + 1] = vec->data_bool[j];
            j--;
        }

        vec->data_bool[j + 1] = key;
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

static void new_elements(vector_t *vec)
{
    vec->capacity = round_nearest_capacity(vec->size);

    switch (vec->T)
    {
        case INT:
            vec->data_int = alloc_mem(sizeof(int) * (size_t)vec->capacity);
            break;
        case DOUBLE:
            vec->data_double = alloc_mem(sizeof(double) * (size_t)vec->capacity);
            break;
        case FLOAT:
            vec->data_float = alloc_mem(sizeof(float) * (size_t)vec->capacity);
            break;
        case CHAR:
            vec->data_char = alloc_mem(sizeof(char) * (size_t)vec->capacity);
            break;
        case STR:
            vec->data_str = alloc_mem(sizeof(char*) * (size_t)vec->capacity);
            break;
        case BOOL:
            vec->data_bool = alloc_mem(sizeof(bool) * (size_t)vec->capacity);
            break;
        case NONE: // default:
            break;
    }
}

static void realloc_void_elements(vector_t *vec, int new_capacity)
{
    size_t number_of_bytes = 0;

    switch (vec->T)
    {
        case INT:
            number_of_bytes = sizeof(int);
            vec->data_int = realloc(vec->data_int, number_of_bytes * (size_t)(new_capacity));
            break;
        case DOUBLE:
            number_of_bytes = sizeof(double);
            vec->data_double = realloc(vec->data_double, number_of_bytes * (size_t)(new_capacity));
            break;
        case FLOAT:
            number_of_bytes = sizeof(float);
            vec->data_float = realloc(vec->data_float, number_of_bytes * (size_t)(new_capacity));
            break;
        case CHAR:
            number_of_bytes = sizeof(char);
            vec->data_char = realloc(vec->data_char, number_of_bytes * (size_t)(new_capacity));
            break;
        case STR:
            number_of_bytes = sizeof(char*);
            vec->data_str = realloc(vec->data_str, number_of_bytes * (size_t)(new_capacity));
            break;
        case BOOL:
            number_of_bytes = sizeof(bool);
            vec->data_bool = realloc(vec->data_bool, number_of_bytes * (size_t)(new_capacity));
            break;
        case NONE: // default:
            break;
    }

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

static void pop_last_index(vector_t *vec)
{
    vec->size--;
    capacity_reallocation(vec, vec->size);
}

static void arg_at_vec_index(vector_t *vec, int index, va_list args)
{
    switch (vec->T)
    {
        case INT:
            {
                int value = va_arg(args, int);
                vec->data_int[index] = value;
            }
            break;
        case DOUBLE:
            {
                double value = va_arg(args, double);
                vec->data_double[index] = value;
            }
            break;
        case FLOAT:
            {
                float value = (float)va_arg(args, double);
                vec->data_float[index] = value;
            }
            break;
        case CHAR:
            {
                char value = (char)va_arg(args, int);
                vec->data_char[index] = value;
            }
            break;
        case STR:
            {
                char *value = va_arg(args, char*);
                vec->data_str[index] = value;
            }
            break;
        case BOOL:
             {
                bool value = (bool)va_arg(args, int);
                vec->data_bool[index] = value;
            }
            break;
        case NONE: // default:
            break;
    }
}

void print_vec_index(vector_t *vec, int index, const char *beginning, const char *end)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO | VEC_SIZE_GE,
        __func__, index))
    {
        return;
    }

    switch (vec->T)
    {
        case INT:
            printf("%s%d%s", beginning, vec->data_int[index], end);
            break;
        case DOUBLE:
            printf("%s%.2f%s", beginning, vec->data_double[index], end);
            break;
        case FLOAT:
            printf("%s%.2f%s", beginning, vec->data_float[index], end);
            break;
        case CHAR:
            printf("%s%c%s", beginning, vec->data_char[index], end);
            break;
        case STR:
            printf("%s%s%s", beginning, vec->data_str[index], end);
            break;
        case BOOL:
            printf("%s%d%s", beginning, vec->data_bool[index], end);
            break;
        case NONE: // default:
            break;
    }
}

int get_vector_size(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL, __func__, WARNING_PLACEHOLDER))
    {
        return -1;
    }

    return vec->size;
}

int get_vector_capacity(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL, __func__, WARNING_PLACEHOLDER))
    {
        return -1;
    }

    return vec->capacity;
}

template_t get_vector_template(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL, __func__, WARNING_PLACEHOLDER))
    {
        return NONE;
    }

    return vec->T;
}

vector_t *alloc_vector(template_t T, int size, ...)
{
    vector_t *vec = alloc_mem(sizeof(vector_t));
    vec->size = size;
    vec->T = T;

    if (vec->size <= 0 || vec->T == NONE)
    {
        vec->size = 0;
        vec->capacity = 0;
        return vec;
    }

    new_elements(vec);

    va_list args;
    va_start(args, size);

    for (int i = 0; i < vec->size; i++)
    {
        arg_at_vec_index(vec, i, args);
    }

    va_end(args);

    return vec;
}

void vector_push(vector_t *vec, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE,
        __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    capacity_reallocation(vec, vec->size);

    va_list args;
    va_start(args, vec);

    arg_at_vec_index(vec, vec->size, args);

    va_end(args);
    vec->size++;
}

void vector_insert(vector_t *vec, int index, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_G,
        __func__, index))
    {
        return;
    }

    capacity_reallocation(vec, vec->size);

    va_list args;
    va_start(args, index);

    switch (vec->T)
    {
        case INT:
            for (int i = vec->size; i > index; i--)
            {
                vec->data_int[i] = vec->data_int[i - 1];
            }
            break;
        case DOUBLE:
            for (int i = vec->size; i > index; i--)
            {
                vec->data_double[i] = vec->data_double[i - 1];
            }
            break;
        case FLOAT:
            for (int i = vec->size; i > index; i--)
            {
                vec->data_float[i] = vec->data_float[i - 1];
            }
            break;
        case CHAR:
            for (int i = vec->size; i > index; i--)
            {
                vec->data_char[i] = vec->data_char[i - 1];
            }
            break;
        case STR:
            for (int i = vec->size; i > index; i--)
            {
                vec->data_str[i] = vec->data_str[i - 1];
            }
            break;
        case BOOL:
            for (int i = vec->size; i > index; i--)
            {
                vec->data_bool[i] = vec->data_bool[i - 1];
            }
            break;
        case NONE: // default:
            break;
    }

    arg_at_vec_index(vec, index, args);
    va_end(args);

    vec->size++;
}

void vector_extend(vector_t *vec, int size, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE,
        __func__, WARNING_PLACEHOLDER))
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
        arg_at_vec_index(vec, i, args);
    }

    va_end(args);
}

void vector_pop(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    pop_last_index(vec);
}

void vector_pop_index(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO | VEC_SIZE_GE,
        __func__, index))
    {
        return;
    }

    int last_index = vec->size - 1;

    if (index == last_index)
    {
        pop_last_index(vec);
        return;
    }

    switch (vec->T)
    {
        case INT:
            for (int i = index; i < last_index; i++)
            {
                vec->data_int[i] = vec->data_int[i + 1];
            }
            break;
        case DOUBLE:
            for (int i = index; i < last_index; i++)
            {
                vec->data_double[i] = vec->data_double[i + 1];
            }
            break;
        case FLOAT:
            for (int i = index; i < last_index; i++)
            {
                vec->data_float[i] = vec->data_float[i + 1];
            }
            break;
        case CHAR:
            for (int i = index; i < last_index; i++)
            {
                vec->data_char[i] = vec->data_char[i + 1];
            }
            break;
        case STR:
            for (int i = index; i < last_index; i++)
            {
                vec->data_str[i] = vec->data_str[i + 1];
            }
            break;
        case BOOL:
            for (int i = index; i < last_index; i++)
            {
                vec->data_bool[i] = vec->data_bool[i + 1];
            }
            break;
        case NONE: // default:
            break;
    }

    vec->size--;

    capacity_reallocation(vec, vec->size);
}

void vector_remove_value(vector_t *vec, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    int index = -1;

    va_list args;
    va_start(args, vec);

    switch (vec->T)
    {
        case INT:
            index = vector_get_index(vec, va_arg(args, int));
            break;
        case DOUBLE:
            index = vector_get_index(vec, va_arg(args, double));
            break;
        case FLOAT:
            index = vector_get_index(vec, (float)va_arg(args, double));
            break;
        case CHAR:
            index = vector_get_index(vec, (char)va_arg(args, int));
            break;
        case STR:
            index = vector_get_index(vec, va_arg(args, char*));
            break;
        case BOOL:
            index = vector_get_index(vec, (bool)va_arg(args, int));
            break;
        case NONE: // default:
            break;
    }

    if (index >= 0)
    {
        vector_pop_index(vec, index);
    }

    va_end(args);
}

void *vector_at(vector_t *vec, int index)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO | VEC_SIZE_GE,
        __func__, index))
    {
        return NULL;
    }

    switch (vec->T)
    {
        case INT:
            return index < 0 ? &vec->data_int[vec->size + index] : &vec->data_int[index];
        case DOUBLE:
            return index < 0 ? &vec->data_double[vec->size + index] : &vec->data_double[index];
        case FLOAT:
            return index < 0 ? &vec->data_float[vec->size + index] : &vec->data_float[index];
        case CHAR:
            return index < 0 ? &vec->data_char[vec->size + index] : &vec->data_char[index];
        case STR:
            return index < 0 ? &vec->data_str[vec->size + index] : &vec->data_str[index];
        case BOOL:
            return index < 0 ? &vec->data_bool[vec->size + index] : &vec->data_bool[index];
        case NONE: // default:
            break;
    }

    return NULL;
}

void vector_at_range(vector_t **vec_dest, vector_t *vec_src, int start, int end, int step)
{
    if (check_warnings(vec_src, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER) ||
        check_warnings(*vec_dest, VEC_ALLOC | VEC_STEP,
        __func__, step))
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
        printf("%s: \033[1;95mwarning:\033[1;97m indices out of range\033[0m\n", __func__);
        return;
    }

    *vec_dest = alloc_mem(sizeof(vector_t));

    int step_counter = 0;
    (*vec_dest)->size = ceil_int((end - start), step);
    (*vec_dest)->T = vec_src->T;
    step--;

    new_elements(*vec_dest);

    switch ((*vec_dest)->T)
    {
        case INT:
            for (int i = 0; i < (*vec_dest)->size; i++)
            {
                (*vec_dest)->data_int[i] = vec_src->data_int[i + start + step_counter];
                step_counter += step;
            }
            break;
        case DOUBLE:
            for (int i = 0; i < (*vec_dest)->size; i++)
            {
                (*vec_dest)->data_double[i] = vec_src->data_double[i + start + step_counter];
                step_counter += step;
            }
            break;
        case FLOAT:
            for (int i = 0; i < (*vec_dest)->size; i++)
            {
                (*vec_dest)->data_float[i] = vec_src->data_float[i + start + step_counter];
                step_counter += step;
            }
            break;
        case CHAR:
            for (int i = 0; i < (*vec_dest)->size; i++)
            {
                (*vec_dest)->data_char[i] = vec_src->data_char[i + start + step_counter];
                step_counter += step;
            }
            break;
        case STR:
            for (int i = 0; i < (*vec_dest)->size; i++)
            {
                (*vec_dest)->data_str[i] = vec_src->data_str[i + start + step_counter];
                step_counter += step;
            }
            break;
        case BOOL:
            for (int i = 0; i < (*vec_dest)->size; i++)
            {
                (*vec_dest)->data_bool[i] = vec_src->data_bool[i + start + step_counter];
                step_counter += step;
            }
            break;
        case NONE: // default:
            break;
    }
}

int vector_get_index(vector_t *vec, ...)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER))
    {
        return -1;
    }

    va_list args;
    va_start(args, vec);

    switch (vec->T)
    {
        case INT:
            {
                int value = va_arg(args, int);

                for (int i = 0; i < vec->size; i++)
                {
                    if (value == vec->data_int[i])
                    {
                        va_end(args);
                        return i;
                    }
                }
            }          
            break;
        case DOUBLE:
            {
                double value = va_arg(args, double);

                for (int i = 0; i < vec->size; i++)
                {
                    if (check_double_equal(value, vec->data_double[i]))
                    {
                        va_end(args);
                        return i;
                    }
                }
            }      
            break;
        case FLOAT:
            {
                float value = (float)va_arg(args, double);

                for (int i = 0; i < vec->size; i++)
                {
                    if (check_float_equal(value, vec->data_float[i]))
                    {
                        va_end(args);
                        return i;
                    }
                }
            }   
            break;
        case CHAR:
            {
                char value = (char)va_arg(args, int);

                for (int i = 0; i < vec->size; i++)
                {
                    if (value == vec->data_char[i])
                    {
                        va_end(args);
                        return i;
                    }
                }
            }
            break;
        case STR:
            {
                char *value = va_arg(args, char*);
                int value_len = get_ascii_size(value);

                for (int i = 0; i < vec->size; i++)
                {
                    if (value_len == get_ascii_size(vec->data_str[i]))
                    {
                        va_end(args);
                        return i;
                    }
                }
            }
            break;
        case BOOL:
            {
                bool value = (bool)va_arg(args, int);

                for (int i = 0; i < vec->size; i++)
                {
                    if (value == vec->data_bool[i])
                    {
                        va_end(args);
                        return i;
                    }
                }
            }
            break;
        case NONE: // default:
            break;
    }

    va_end(args);
    return -1;
}

void vector_reverse(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    int half_size = vec->size / 2;
    int last_index = vec->size - 1;

    switch (vec->T)
    {
        case INT:
            {
                int temp = 0;

                for (int i = 0; i < half_size; i++)
                {
                    temp = vec->data_int[i];
                    vec->data_int[i] = vec->data_int[last_index - i];
                    vec->data_int[last_index - i] = temp;
                }
            }
            break;
        case DOUBLE:
            {
                double temp = 0.0;

                for (int i = 0; i < half_size; i++)
                {
                    temp = vec->data_double[i];
                    vec->data_double[i] = vec->data_double[last_index - i];
                    vec->data_double[last_index - i] = temp;
                }
            }
            break;
        case FLOAT:
            {
                float temp = 0.0f;

                for (int i = 0; i < half_size; i++)
                {
                    temp = vec->data_float[i];
                    vec->data_float[i] = vec->data_float[last_index - i];
                    vec->data_float[last_index - i] = temp;
                }
            }
            break;
        case CHAR:
             {
                char temp = 0;

                for (int i = 0; i < half_size; i++)
                {
                    temp = vec->data_char[i];
                    vec->data_char[i] = vec->data_char[last_index - i];
                    vec->data_char[last_index - i] = temp;
                }
            }
            break;
        case STR:
            {
                char *temp = "";

                for (int i = 0; i < half_size; i++)
                {
                    temp = vec->data_str[i];
                    vec->data_str[i] = vec->data_str[last_index - i];
                    vec->data_str[last_index - i] = temp;
                }
            }
            break;
        case BOOL:
            {
                bool temp = false;

                for (int i = 0; i < half_size; i++)
                {
                    temp = vec->data_bool[i];
                    vec->data_bool[i] = vec->data_bool[last_index - i];
                    vec->data_bool[last_index - i] = temp;
                }
            }
            break;
        case NONE: // default:
            break;
    }
}

void vector_sort(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    switch (vec->T)
    {
        case INT:
            insertion_sort_int(vec);
            break;
        case DOUBLE:
            insertion_sort_double(vec);
            break;
        case FLOAT:
            insertion_sort_float(vec);
            break;
        case CHAR:
            insertion_sort_char(vec);
            break;
        case STR:
            insertion_sort_str(vec);
            break;
        case BOOL:
            insertion_sort_bool(vec);
            break;
        case NONE: // default:
            break;
    }
}

void vector_copy(vector_t **vec_dest, vector_t *vec_src)
{
    if (check_warnings(vec_src, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER) ||
        check_warnings(*vec_dest, VEC_ALLOC, 
        __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    vector_at_range(vec_dest, vec_src, 0, 0, 1);
}

void vector_free(vector_t **vec)
{
    if (check_warnings(*vec, VEC_NULL, __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    switch ((*vec)->T)
    {
        case INT:
            free_mem((*vec)->data_int, (sizeof(int) * (size_t)(*vec)->capacity));
            break;
        case DOUBLE:
            free_mem((*vec)->data_double, (sizeof(double) * (size_t)(*vec)->capacity));
            break;
        case FLOAT:
            free_mem((*vec)->data_float, (sizeof(float) * (size_t)(*vec)->capacity));
            break;
        case CHAR:
            free_mem((*vec)->data_char, (sizeof(char) * (size_t)(*vec)->capacity));
            break;
        case STR:
            free_mem((*vec)->data_str, (sizeof(char*) * (size_t)(*vec)->capacity));
            break;
        case BOOL:
            free_mem((*vec)->data_bool, (sizeof(bool) * (size_t)(*vec)->capacity));
            break;
        case NONE: // default:
            break;
    }

    free_mem(*vec, sizeof(vector_t));
    *vec = NULL;
}

void vector_print(vector_t *vec)
{
    if (check_warnings(vec, VEC_NULL | VEC_TYPE_NONE | VEC_SIZE_ZERO,
        __func__, WARNING_PLACEHOLDER))
    {
        return;
    }

    int i;

    for (i = 0; i < vec->size-1; i++)
    {
        print_vec_index(vec, i, "|", "| ");
    }

    print_vec_index(vec, i, "|", "|\n");
}
