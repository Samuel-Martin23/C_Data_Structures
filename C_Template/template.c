
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
    double epsilon = 0.0000001f;

    if (fabs(value_1 - value_2) < epsilon)
    {
        return true;
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

void convert_2d_str(template_t T, void **value)
{
    if (T == STR)
    {
        *value = *((char**)*value);
    }
}
