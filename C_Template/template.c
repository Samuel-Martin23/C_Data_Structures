
#include "template.h"

int void_cast_int(void *value) {return (*(int*)value);}
double void_cast_double(void *value) {return (*(double*)value);}
float void_cast_float(void *value) {return (*(float*)value);}
char void_cast_char(void *value) {return (*(const char*)value);}
const char *void_cast_str(void *value) {return ((const char*)value);}
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
    }
}

bool check_float_equal(float value1, float value2)
{
    float epsilon = 0.01f;

    if (fabs(value1 - value2) < epsilon)
    {
        return true;
    }

    return false;
}

bool check_double_equal(double value1, double value2)
{
    double epsilon = 0.0000001f;

    if (fabs(value1 - value2) < epsilon)
    {
        return true;
    }

    return false;
}

bool check_less_equal_value(template_t T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (void_cast_int(value1) <= void_cast_int(value2));
        case DOUBLE:
            return (void_cast_double(value1) <= void_cast_double(value2));
        case FLOAT:
            return (void_cast_float(value1) <= void_cast_float(value2));
        case CHAR:
            return (void_cast_char(value1) <= void_cast_char(value2));
        case STR:
            return (strlen(void_cast_str(value1)) <= strlen(void_cast_str(value2)));
        case BOOL:
            return (void_cast_bool(value1) <= void_cast_bool(value2));
    }

    return false;
}

bool check_greater_value(template_t T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (void_cast_int(value1) > void_cast_int(value2));
        case DOUBLE:
            return (void_cast_double(value1) > void_cast_double(value2));
        case FLOAT:
            return (void_cast_float(value1) > void_cast_float(value2));
        case CHAR:
            return (void_cast_char(value1) > void_cast_char(value2));
        case STR:
            return (strlen(void_cast_str(value1)) > strlen(void_cast_str(value2)));
        case BOOL:
            return (void_cast_bool(value1) > void_cast_bool(value2));
    }

    return false;
}

bool check_equal_value(template_t T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (void_cast_int(value1) == void_cast_int(value2));
        case DOUBLE:
            return (check_double_equal(void_cast_double(value1), void_cast_double(value2)));
        case FLOAT:
            return (check_float_equal(void_cast_float(value1), void_cast_float(value2)));
        case CHAR:
            return (void_cast_char(value1) == void_cast_char(value2));
        case STR:
            return (strlen(void_cast_str(value1)) == strlen(void_cast_str(value2)));
        case BOOL:
            return (void_cast_bool(value1) == void_cast_bool(value2));
    }

    return false;
}

int get_bytes(template_t T, void *value)
{
    int size = 0;

    switch (T)
    {
        case INT:
            size = (int)sizeof(int);
            break;
        case DOUBLE:
            size = (int)sizeof(double);
            break;
        case FLOAT:
            size = (int)sizeof(float);
            break;
        case CHAR:
            size = (int)sizeof(char) + 1;
            break;
        case STR:
            {
                const char *str_value = void_cast_str(value);
                size = (int)(sizeof(char) * strlen(str_value) + 1);
            }
            break;
        case BOOL:
            size = (int)sizeof(bool);
            break;
    }

    return size;
}
