
#include "vector.h"

int VoidCastInt(void *value) {return (*(int*)value);}
double VoidCastDouble(void *value) {return (*(double*)value);}
float VoidCastFloat(void *value) {return (*(float*)value);}
char VoidCastChar(void *value) {return (*(const char*)value);}
const char *VoidCastStr(void *value) {return ((const char*)value);}

int DataTypeSize(Type T, void *value)
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
            printf("Warning in %s: new_vector_size is less than or equal to the current size of the vector.\n", function_name);
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
            return (strlen(VoidCastStr(value_1)) == strlen(VoidCastStr(value_2)));
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
            return (strlen(VoidCastStr(value_1)) > strlen(VoidCastStr(value_2)));
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

void *NewIndex(Vector_T *vec_data, void *index_value)
{
    switch (vec_data->T)
    {
        case INT:
            {
                int *allocated_value = malloc(sizeof(int));
                *allocated_value = VoidCastInt(index_value);
                vec_data->allocated_mem += sizeof(int);
                index_value = allocated_value;
            }
            break;
        case DOUBLE:
            {
                double *allocated_value = malloc(sizeof(double));
                *allocated_value = VoidCastDouble(index_value);
                vec_data->allocated_mem += sizeof(double);
                index_value = allocated_value;
            }
            break;
        case FLOAT:
            {
                float *allocated_value = malloc(sizeof(float));
                *allocated_value = VoidCastFloat(index_value);
                vec_data->allocated_mem += sizeof(float);
                index_value = allocated_value;
            }
            break;
        case CHAR:
             {
                char key_data = VoidCastChar(index_value);
                const char *allocated_value = malloc(sizeof(char) + 1);

                vec_data->allocated_mem += (int)(sizeof(char) + 1);
                memcpy((char*)allocated_value, &key_data, 1);
                *((char*)allocated_value + 1) = '\0';

                index_value = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *str_value = VoidCastStr(index_value);
                int str_length = (int)strlen(str_value);
                const char *allocated_value = malloc(sizeof(char) * (str_length + 1));

                vec_data->allocated_mem += (int)(sizeof(char) * (str_length + 1));
                memcpy((char*)allocated_value, str_value, str_length);
                *((char*)allocated_value + str_length) = '\0';

                index_value = (char*)allocated_value;
            }
            break;
        default:
            break;
    }

    return index_value;
}

void FreeIndex(Vector_T *vec_data, int index)
{
    vec_data->allocated_mem -= DataTypeSize(vec_data->T, vec_data->data[index]);
    free(vec_data->data[index]);
}

void NullIndex(Vector_T *vec_data)
{
    vec_data->size--;
    vec_data->data[vec_data->size] = NULL;
}

Vector_T VectorInit(Type T, void *array, int size)
{
    Vector_T new_vector;
    new_vector.size = size;
    new_vector.capacity = new_vector.size > DEFAULT_CAPACITY_SIZE ? new_vector.size + DEFAULT_CAPACITY_SIZE : DEFAULT_CAPACITY_SIZE;
    new_vector.T = T;
    new_vector.allocated_mem = 0;
    new_vector.data = NewVoidElements(&new_vector);

    switch (new_vector.T)
    {
        case INT:
            {
                int *data_array = ((int*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = NewIndex(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = NewIndex(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = NewIndex(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = NewIndex(&new_vector, data_array);
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array= (char**)((char*)array);

                for (int i = 0; i < new_vector.size; i++)
                {
                    new_vector.data[i] = NewIndex(&new_vector, *data_array);
                    data_array++;
                }
            }
            break;
        default:
            exit(1);
            break;
    }

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

    vec_data->data[vec_data->size] = NewIndex(vec_data, value);
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
    vec_data->data[index] = NewIndex(vec_data, value);
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

                    vec_data->data[i] = NewIndex(vec_data, data_array);
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

                    vec_data->data[i] = NewIndex(vec_data, data_array);
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

                    vec_data->data[i] = NewIndex(vec_data, data_array);
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

                    vec_data->data[i] = NewIndex(vec_data, data_array);
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

                    vec_data->data[i] = NewIndex(vec_data, *data_array);
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

    FreeIndex(vec_data, vec_data->size-1);
    NullIndex(vec_data);
}

void VectorPopIndex(Vector_T *vec_data, int index)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE, "VectorPopIndex", index))
    {
        return;
    }

    if (index == vec_data->size-1)
    {
        FreeIndex(vec_data, index);
        NullIndex(vec_data);
        return;
    }

    FreeIndex(vec_data, index);

    for (int i = index; i < vec_data->size-1; i++)
    {
        vec_data->data[i] = vec_data->data[i + 1];
    }

    NullIndex(vec_data);
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
        FreeIndex(vec_data, i);
        vec_data->data[i] = NULL;
    }

    vec_data->size = 0;
}

void VectorFree(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE, "VectorFree", -1))
    {
        return;
    }

    VectorClear(vec_data);
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