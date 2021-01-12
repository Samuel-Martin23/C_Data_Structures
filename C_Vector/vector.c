
#include "vector.h"

#define VEC_FREE                    0x00000001
#define VEC_SIZE_ZERO               0x00000002
#define VEC_TYPE                    0x00000004
#define VEC_SIZE_G                  0x00000008
#define VEC_SIZE_GE                 0x00000010

static bool CheckWarnings(Vector_t *vec_data, int warning_code, const char *function_name, int check_value)
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
        Template T = check_value;

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

    return false;
}

void PrintAllocatedMemVector(Vector_t *vec_data)
{
    printf("Bytes Allocated: %d\n", vec_data->allocated_mem);
}

void PrintVectorSize(Vector_t *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "PrintVectorSize", -1))
    {
        return;
    }

    printf("Vector Size: %d\n", vec_data->size);
}

void InsertionSort(Template T, void **array, int array_size)
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

void *NewVoidElements(Vector_t *vec_data)
{
    vec_data->allocated_mem += sizeof(void *) * vec_data->capacity;
    return malloc(sizeof(void *) * vec_data->capacity);
}

void *ReallocVoidElements(Vector_t *vec_data, int new_vector_capacity)
{
    vec_data->allocated_mem += (int)(sizeof(void *) * (new_vector_capacity - vec_data->capacity));
    vec_data->capacity = new_vector_capacity;
    return realloc(vec_data->data, sizeof(void*) * (new_vector_capacity));
}

void CheckCapacityReallocation(Vector_t *vec_data)
{
    if (vec_data->size % DEFAULT_CAPACITY_SIZE == 0 && vec_data->capacity != DEFAULT_CAPACITY_SIZE)
    {
        vec_data->data = ReallocVoidElements(vec_data, vec_data->size);
    }
}

void *NewIndex(Vector_t *vec_data, void *index_value)
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

void FreeIndex(Vector_t *vec_data, int index)
{
    vec_data->allocated_mem -= SizeOfType(vec_data->T, vec_data->data[index]);
    free(vec_data->data[index]);
}

void NullIndex(Vector_t *vec_data)
{
    vec_data->size--;
    vec_data->data[vec_data->size] = NULL;
}

Vector_t VectorInit(Template T, void *array, int size)
{
    Vector_t new_vector;
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

void VectorPush(Vector_t *vec_data, Template T, void *value)
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

void VectorInsert(Vector_t *vec_data, Template T, void *value, int index)
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

void VectorExtend(Vector_t *vec_data, Template T, void *array, int size_array)
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

void VectorPop(Vector_t *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "VectorPop", -1))
    {
        return;
    }

    FreeIndex(vec_data, vec_data->size-1);
    NullIndex(vec_data);

    CheckCapacityReallocation(vec_data);
}

void VectorPopIndex(Vector_t *vec_data, int index)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO | VEC_SIZE_GE, "VectorPopIndex", index))
    {
        return;
    }

    if (index == vec_data->size-1)
    {
        VectorPop(vec_data);
        return;
    }

    FreeIndex(vec_data, index);

    for (int i = index; i < vec_data->size-1; i++)
    {
        vec_data->data[i] = vec_data->data[i + 1];
    }

    NullIndex(vec_data);

    CheckCapacityReallocation(vec_data);
}

void VectorRemoveValue(Vector_t *vec_data, Template T, void *value)
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

void VectorReverse(Vector_t *vec_data)
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

void VectorSort(Vector_t *vec_data)
{
    if (CheckWarnings(vec_data, VEC_FREE | VEC_SIZE_ZERO, "VectorSort", -1))
    {
        return;
    }

    InsertionSort(vec_data->T, vec_data->data, vec_data->size);
}

void VectorClear(Vector_t *vec_data)
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

void VectorFree(Vector_t *vec_data)
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

void VectorPrint(Vector_t *vec_data)
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