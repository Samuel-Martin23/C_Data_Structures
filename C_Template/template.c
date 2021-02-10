
#include "template.h"

int void_cast_int(void *value) {return (*(int*)value);}
double void_cast_double(void *value) {return (*(double*)value);}
float void_cast_float(void *value) {return (*(float*)value);}
char void_cast_char(void *value) {return (*(char*)value);}
char *void_cast_str(void *value) {return ((char*)value);}
bool void_cast_bool(void *value) {return (*(bool*)value);}

void print_t(template_t T, void *value, const char *beginning, const char *end)
{
    switch (T)
    {
        case INT:
            printf("%s%d%s", beginning, void_cast_int(value), end);
            break;
        case DOUBLE:
            printf("%s%.2f%s", beginning, void_cast_double(value), end);
            break;
        case FLOAT:
            printf("%s%.2f%s", beginning, void_cast_float(value), end);
            break;
        case CHAR:
            printf("%s\'%c\'%s", beginning, void_cast_char(value), end);
            break;
        case STR:
            printf("%s\"%s\"%s", beginning, void_cast_str(value), end);
            break;
        case BOOL:
            {
                bool condition = void_cast_bool(value);
                const char *state = condition ? "true" : "false";
                printf("%s%s%s", beginning, state, end);
            }
            break;
        case NONE: // default:
            break;
    }
}

bool check_float_equal(float value_1, float value_2)
{
    float epsilon = 0.01f;

    if (fabs(value_1 - value_2) < epsilon)
    {
        return true;
    }

    return false;
}

bool check_double_equal(double value_1, double value_2)
{
    double epsilon = 0.0000001;

    if (fabs(value_1 - value_2) < epsilon)
    {
        return true;
    }

    return false;
}

bool check_less_value(template_t T, void *value_1, void *value_2)
{
    return check_greater_value(T, value_2, value_1);
}

bool check_greater_value(template_t T, void *value_1, void *value_2)
{
    switch (T)
    {
        case INT:
            return (void_cast_int(value_1) > void_cast_int(value_2));
        case DOUBLE:
            return (void_cast_double(value_1) > void_cast_double(value_2));
        case FLOAT:
            return (void_cast_float(value_1) > void_cast_float(value_2));
        case CHAR:
            return (void_cast_char(value_1) > void_cast_char(value_2));
        case STR:
            return (strlen(void_cast_str(value_1)) > strlen(void_cast_str(value_2)));
        case BOOL:
            return (void_cast_bool(value_1) > void_cast_bool(value_2));
        case NONE: // default:
            break;
    }

    return false;
}

bool check_less_equal_value(template_t T, void *value_1, void *value_2)
{
    switch (T)
    {
        case INT:
            return (void_cast_int(value_1) <= void_cast_int(value_2));
        case DOUBLE:
            return (void_cast_double(value_1) <= void_cast_double(value_2));
        case FLOAT:
            return (void_cast_float(value_1) <= void_cast_float(value_2));
        case CHAR:
            return (void_cast_char(value_1) <= void_cast_char(value_2));
        case STR:
            return (strlen(void_cast_str(value_1)) <= strlen(void_cast_str(value_2)));
        case BOOL:
            return (void_cast_bool(value_1) <= void_cast_bool(value_2));
        case NONE: // default:
            break;
    }

    return false;
}

bool check_greater_equal_value(template_t T, void *value_1, void *value_2)
{
    return check_less_equal_value(T, value_2, value_1);
}

bool check_equal_value(template_t T, void *value_1, void *value_2)
{
    switch (T)
    {
        case INT:
            return (void_cast_int(value_1) == void_cast_int(value_2));
        case DOUBLE:
            return (check_double_equal(void_cast_double(value_1), void_cast_double(value_2)));
        case FLOAT:
            return (check_float_equal(void_cast_float(value_1), void_cast_float(value_2)));
        case CHAR:
            return (void_cast_char(value_1) == void_cast_char(value_2));
        case STR:
            return (!(strcmp(void_cast_str(value_1), void_cast_str(value_2))));
        case BOOL:
            return (void_cast_bool(value_1) == void_cast_bool(value_2));
        case NONE: // default:
            break;
    }

    return false;
}

bool check_not_equal_value(template_t T, void *value_1, void *value_2)
{
    return !(check_equal_value(T, value_1, value_2));
}

size_t get_bytes(template_t T, void *value)
{
    size_t size = 0;

    switch (T)
    {
        case INT:
            size = sizeof(int);
            break;
        case DOUBLE:
            size = sizeof(double);
            break;
        case FLOAT:
            size = sizeof(float);
            break;
        case CHAR:
            size = sizeof(char);
            break;
        case STR:
            {
                char *str_value = void_cast_str(value);
                size = (sizeof(char) * (strlen(str_value) + 1));
            }
            break;
        case BOOL:
            size = sizeof(bool);
            break;
        case NONE: // default:
            break;
    }

    return size;
}

void *new_T_value(template_t T, void *value)
{
    switch (T)
    {
        case INT:
            {
                size_t number_of_bytes = get_bytes(T, value);
                int *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_int(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case DOUBLE:
            {
                size_t number_of_bytes = get_bytes(T, value);
                double *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_double(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case FLOAT:
            {
                size_t number_of_bytes = get_bytes(T, value);
                float *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_float(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case CHAR:
             {
                size_t number_of_bytes = get_bytes(T, value);
                char cast_value = void_cast_char(value);
                char *allocated_value = malloc(number_of_bytes);

                memcpy(allocated_value, &cast_value, 1);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case STR:
            {
                char *cast_value = void_cast_str(value);
                size_t number_of_bytes = get_bytes(T, cast_value);
                char *allocated_value = malloc(number_of_bytes);

                mem_usage.allocated += (u_int32_t)number_of_bytes;

                number_of_bytes--;
                memcpy(allocated_value, cast_value, number_of_bytes);
                allocated_value[number_of_bytes] = '\0';

                value = allocated_value;
            }
            break;
        case BOOL:
            {
                size_t number_of_bytes = get_bytes(T, value);
                bool *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_bool(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case NONE: // default:
            break;
    }

    return value;
}

void free_T_value(template_t T, void *value)
{
    mem_usage.freed += (u_int32_t)get_bytes(T, value);
    free(value);
}

void convert_2d_str(template_t T, void **value)
{
    if (T == STR)
    {
        *value = *((char**)*value);
    }
}
