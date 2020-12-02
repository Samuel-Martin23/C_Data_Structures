
#include "vector.h"

int VoidCastInt(void *value) {return (*(int*)value);}
double VoidCastDouble(void *value) {return (*(double*)value);}
float VoidCastFloat(void *value) {return (*(float*)value);}
char VoidCastChar(void *value) {return (*(const char*)value);}
const char *VoidCastStr(void *value) {return ((const char*)value);}

bool CheckWarnings(Vector_T *vec_data, int warning_code, const char *function_name, int check_value)
{
    if (warning_code & VEC_FREE)
    {
        if (vec_data->data == NULL)
        {
            printf("Warning in %s: Vector data is freed.\n", function_name);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_ZERO)
    {
        if (vec_data->size == 0)
        {
            printf("Warning in %s: Vector size is 0.\n", function_name);
            return true;
        }
    }

    if (warning_code & VEC_TYPE)
    {
        Type T = check_value;

        if (vec_data->T != T)
        {
            printf("Warning in %s: Vector type does not equal the type of the new value.\n", function_name);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_G)
    {
        int index = check_value;

        if (index < 0 || index > vec_data->size)
        {
            printf("Warning in %s: Index out of range.\n", function_name);
            return true;
        }
    }

    if (warning_code & VEC_SIZE_GE)
    {
        int index = check_value;

        if (index < 0 || index >= vec_data->size)
        {
            printf("Warning in %s: Index out of range.\n", function_name);
            return true;
        }
    }

    if (warning_code & VEC_NEW_SIZE)
    {
        int new_vector_size = check_value;

        if (new_vector_size <= vec_data->size)
        {
            printf("Warning in %s: new_vector_size is less than the current size of the vector.\n", function_name);
            return true;
        }
    }

    return false;
}

void PrintT(Type T, void *value, const char *beginning, const char *end)
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

void PrintAllocatedMemory(Vector_T *vec_data)
{
    printf("Bytes Allocated: %d\n", vec_data->allocated_mem);
}

void PrintVectorSize(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "PrintVectorSize", -1))
    {
        return;
    }

    printf("Vector Size: %d\n", vec_data->size);
}

int StrLen(const char *word)
{
    int length = 0;

    while (*word != '\0')
    {
        word++;
        length++;
    }

    return length;
}

bool CheckEqualValue(Type T, void *value_1, void *value_2)
{
    switch (T)
    {
        case INT:
            return (VoidCastInt(value_1) == VoidCastInt(value_2));
        case DOUBLE:
            return (VoidCastDouble(value_1) == VoidCastDouble(value_2));
        case FLOAT:
            return (VoidCastFloat(value_1) == VoidCastFloat(value_2));
        case CHAR:
            return (VoidCastChar(value_1) == VoidCastChar(value_2));
        case STR:
            return (StrLen(VoidCastStr(value_1)) == StrLen(VoidCastStr(value_2)));
        default:
            return false;
    }
}

bool CheckGreaterValue(Type T, void *value_1, void *value_2)
{
    switch (T)
    {
        case INT:
            return (VoidCastInt(value_1) > VoidCastInt(value_2));
        case DOUBLE:
            return (VoidCastDouble(value_1) > VoidCastDouble(value_2));
        case FLOAT:
            return (VoidCastFloat(value_1) > VoidCastFloat(value_2));
        case CHAR:
            return (VoidCastChar(value_1) > VoidCastChar(value_2));
        case STR:
            return (StrLen(VoidCastStr(value_1)) > StrLen(VoidCastStr(value_2)));
        default:
            return false;
    }
}

void InsertionSort(Type T, void **array, int array_size)
{
    int j;
    void *key;

    for (int i = 1; i < array_size; i++)
    {
        key = array[i];
        j = i - 1;

        while (j >= 0 && CheckGreaterValue(T, array[j], key))
        {
            array[j + 1] = array[j];
            j--;
        }

        array[j + 1] = key;
    }
}

void *NewVoidElements(Vector_T *vec_data)
{
    vec_data->allocated_mem += sizeof(void *) * vec_data->capacity;
    return malloc(sizeof(void *) * vec_data->capacity);
}

void *ReallocVoidElements(Vector_T *vec_data, int new_vector_size)
{
    if (CheckWarnings(vec_data, VEC_NEW_SIZE, "ReallocVoid", new_vector_size))
    {
        return NULL;
    }

    vec_data->capacity = new_vector_size;
    vec_data->allocated_mem += (int)(sizeof(void *) * (new_vector_size - vec_data->size));
    return realloc(vec_data->data, sizeof(void*) * (new_vector_size));
}

Vector_T VectorInit(Type T, void *array, int size)
{
    Vector_T new_vector;
    new_vector.capacity = size > DEFAULT_CAPACITY_SIZE ? size + DEFAULT_CAPACITY_SIZE : DEFAULT_CAPACITY_SIZE;
    new_vector.data = NewVoidElements(&new_vector);

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector.data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector.data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector.data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector.data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array= (char**)((char*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector.data[i] = (void*)*data_array;
                    data_array++;
                }
            }
            break;
        default:
            break;
    }

    new_vector.size = size;
    return new_vector;
}

void VectorPush(Vector_T *vec_data, Type T, void *value)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_TYPE, "VectorPush", T))
    {
        return;
    }

    if (vec_data->capacity == vec_data->size)
    {
        vec_data->data = ReallocVoidElements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
    }

    vec_data->data[vec_data->size] = value;
    vec_data->size++;
}

void VectorInsert(Vector_T *vec_data, Type T, void *value, int index)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_TYPE, "VectorInsert", T)
        || CheckWarnings(vec_data, VEC_SIZE_G, "VectorInsert", index))
    {
        return;
    }

    if (vec_data->capacity == vec_data->size)
    {
        vec_data->data = ReallocVoidElements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
    }

    for (int i = vec_data->size; i >= index; i--)
    {
        vec_data->data[i] = vec_data->data[i - 1];
    }

    vec_data->size++;
    vec_data->data[index] = value;
}

void VectorExtend(Vector_T *vec_data, Type T, void *array, int size_array)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_TYPE, "VectorExtened", T))
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
                        vec_data->data = ReallocVoidElements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = ReallocVoidElements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = ReallocVoidElements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = ReallocVoidElements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array= (char**)((char*)array);

                for (int i = vec_data->size; i < total_size; i++)
                {
                    if (i == vec_data->capacity)
                    {
                        vec_data->data = ReallocVoidElements(vec_data, vec_data->capacity + DEFAULT_CAPACITY_SIZE);
                    }

                    vec_data->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        default:
            return;
    }

    vec_data->size = total_size;
}

void VectorPop(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "VectorPop", -1))
    {
        return;
    }

    vec_data->size--;
    vec_data->data[vec_data->size] = NULL;
}

void VectorPopIndex(Vector_T *vec_data, int index)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE, "VectorPopIndex", index))
    {
        return;
    }

    for (int i = index; i < vec_data->size-1; i++)
    {
        vec_data->data[i] = vec_data->data[i + 1];
    }

    vec_data->size--;
    vec_data->data[vec_data->size] = NULL;
}

void VectorRemoveValue(Vector_T *vec_data, Type T, void *value)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_TYPE | VEC_SIZE_ZERO, "VectorRemoveValue", T))
    {
        return;
    }

    for (int i = 0; i < vec_data->size; i++)
    {
        if (CheckEqualValue(vec_data->T, vec_data->data[i], value))
        {
            VectorPopIndex(vec_data, i);
            return;
        }
    }

    printf("Warning in VectorRemoveValue: Value could not be found in the vector.\n");
}

void VectorReverse(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "VectorReverse", -1))
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

void VectorSort(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "VectorSort", -1))
    {
        return;
    }

    InsertionSort(vec_data->T, vec_data->data, vec_data->size);
}

void VectorClear(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "VectorClear", -1))
    {
        return;
    }

    for (int i = 0; i < vec_data->size; i++)
    {
        vec_data->data[i] = NULL;
    }

    vec_data->size = 0;
}

void VectorFree(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE, "VectorClear", -1))
    {
        return;
    }

    vec_data->allocated_mem -= (vec_data->capacity * sizeof(void*));
    vec_data->size = -1;
    vec_data->capacity = -1;
    free(vec_data->data);
    vec_data->data = NULL;
    vec_data->T = -1;
}

void VectorPrint(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_SIZE_ZERO, "VectorPrint", -1))
    {
        return;
    }

    int i;

    for (i = 0; i < vec_data->size-1; i++)
    {
        PrintT(vec_data->T, vec_data->data[i], "|", "| ");
    }

    PrintT(vec_data->T, vec_data->data[i], "|", "|\n");
}