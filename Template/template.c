
#include "template.h"

int VoidCastInt(void *value) {return (*(int*)value);}
double VoidCastDouble(void *value) {return (*(double*)value);}
float VoidCastFloat(void *value) {return (*(float*)value);}
char VoidCastChar(void *value) {return (*(const char*)value);}
const char *VoidCastStr(void *value) {return ((const char*)value);}

void PrintT(Template T, void *value, const char *beginning, const char *end)
{
    switch(T)
    {
        case INT:
            printf("%s%d%s", beginning, VoidCastInt(value), end);
            break;
        case DOUBLE:
            printf("%s%.2f%s", beginning, VoidCastDouble(value), end);
            break;
        case FLOAT:
            printf("%s%.2f%s", beginning, VoidCastFloat(value), end);
            break;
        case CHAR:
            printf("%s\'%c\'%s", beginning, VoidCastChar(value), end);
            break;
        case STR:
            printf("%s\"%s\"%s", beginning, VoidCastStr(value), end);
            break;
        default:
            break;
    }
}

bool CheckLessEqualValue(Template T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (VoidCastInt(value1) <= VoidCastInt(value2));
        case DOUBLE:
            return (VoidCastDouble(value1) <= VoidCastDouble(value2));
        case FLOAT:
            return (VoidCastFloat(value1) <= VoidCastFloat(value2));
        case CHAR:
            return (VoidCastChar(value1) <= VoidCastChar(value2));
        case STR:
            return (strlen(VoidCastStr(value1)) <= strlen(VoidCastStr(value2)));
        default:
            return false;
    }
}

bool CheckGreaterValue(Template T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (VoidCastInt(value1) > VoidCastInt(value2));
        case DOUBLE:
            return (VoidCastDouble(value1) > VoidCastDouble(value2));
        case FLOAT:
            return (VoidCastFloat(value1) > VoidCastFloat(value2));
        case CHAR:
            return (VoidCastChar(value1) > VoidCastChar(value2));
        case STR:
            return (strlen(VoidCastStr(value1)) > strlen(VoidCastStr(value2)));
        default:
            return false;
    }
}

bool CheckEqualValue(Template T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (VoidCastInt(value1) == VoidCastInt(value2));
        case DOUBLE:
            return (VoidCastDouble(value1) == VoidCastDouble(value2));
        case FLOAT:
            return (VoidCastFloat(value1) == VoidCastFloat(value2));
        case CHAR:
            return (VoidCastChar(value1) == VoidCastChar(value2));
        case STR:
            return (strlen(VoidCastStr(value1)) == strlen(VoidCastStr(value2)));
        default:
            return false;
    }
}

int SizeOfType(Template T, void *value)
{
    int size = 0;

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
            size = sizeof(char) + 1;
            break;
        case STR:
            {
                const char *str_value = VoidCastStr(value);
                size = (int)(sizeof(char) * strlen(str_value) + 1);
            }
            break;
        default:
            break;
    }

    return size;
}