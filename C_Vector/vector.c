
#include <stdio.h>
#include <stdlib.h>

#define intA(...)               INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define doubleA(...)            DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define floatA(...)             FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define charA(...)              CHAR, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define strA(...)               STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

#define VectorInitInt(...)      VectorInit(intA(__VA_ARGS__))
#define VectorInitDouble(...)   VectorInit(doubleA(__VA_ARGS__))
#define VectorInitFloat(...)    VectorInit(floatA(__VA_ARGS__))
#define VectorInitChar(...)     VectorInit(charA(__VA_ARGS__))
#define VectorInitStr(...)      VectorInit(strA(__VA_ARGS__))

#define int(x)                  INT, (int[]){x}
#define double(x)               DOUBLE, (double[]){x}
#define float(x)                FLOAT, (float[]){x}
#define char(x)                 CHAR, (char[]){x}
#define str(x)                  STR, (char*)x

#define int_cast(x)             (*(int*)x)
#define double_cast(x)          (*(double*)x)
#define float_cast(x)           (*(float*)x)
#define char_cast(x)            (*(char*)x)
#define str_cast(x)             ((char*)x)

#define DEFAULT_CAPACITY_SIZE   100

int allocated_mem = 0;

typedef enum boolean {false, true} bool;
typedef enum T {INT, DOUBLE, FLOAT, CHAR, STR} Type;
typedef enum VectorWarningCodes {VEC_NULL, VEC_SIZE_ZERO, VEC_TYPE, VEC_SIZE_LGE, VEC_SIZE_LG, VEC_NEW_SIZE} WarningVector;

typedef struct Vector
{
    int size;
    int capacity;
    void **data;
    Type T;
} Vector_T;

bool CheckWarnings(Vector_T *vec_data, WarningVector code, const char *function_name, int check_value)
{
    switch (code)
    {
        case VEC_NULL:
            if (vec_data == NULL)
            {
                printf("Warning in %s: Vector is NULL.\n", function_name);
                return true;
            }
            break;
        case VEC_SIZE_ZERO:
            if (vec_data->size == 0)
            {
                printf("Warning in %s: Vector size is 0.\n", function_name);
                return true;
            }
            break;
        case VEC_TYPE:
            {
                Type T = check_value;
    
                if (vec_data->T != T)
                {
                    printf("Warning in %s: Vector type does not equal the type of the new value.\n", function_name);
                    return true;
                }
            }
            break;
        case VEC_SIZE_LGE:
            {
                int index = check_value;

                if (index < 0 || index >= vec_data->size)
                {
                    printf("Warning in %s: Index out of range.\n", function_name);
                    return true;
                }
            }
            break;
        case VEC_SIZE_LG:
            {
                int index = check_value;

                if (index < 0 || index > vec_data->size)
                {
                    printf("Warning in %s: Index out of range.\n", function_name);
                    return true;
                }
            }
            break;
        case VEC_NEW_SIZE:
            {
                int new_vector_size = check_value;

                if (new_vector_size <= vec_data->size)
                {
                    printf("Warning in %s: new_vector_size is less than the current size of the vector.\n", function_name);
                    return true; 
                }
            }
            break;
        default:
            break;
    }

    return false;
}

void PrintT(Type T, void *word, const char *beginning, const char *end)
{
    printf("%s", beginning);

    switch (T)
    {
        case INT:
            printf("%d", int_cast(word));
            break;
        case DOUBLE:
            printf("%.2f", double_cast(word));
            break;
        case FLOAT:
            printf("%.2f", float_cast(word));
            break;
        case CHAR:
            printf("%c", char_cast(word));
            break;
        case STR:
            printf("%s", str_cast(word));
            break;
        default:
            printf("Unidentified Type.");
            break;
    }

    printf("%s", end);
}

void PrintAllocatedMemory()
{
    printf("Bytes Allocated: %d\n", allocated_mem);
}

void PrintVectorSize(Vector_T *vec_data)
{   
    if (CheckWarnings(vec_data, VEC_NULL, "PrintVectorSize", -1))
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
            return (int_cast(value_1) == int_cast(value_2));
        case DOUBLE:
            return (double_cast(value_1) == double_cast(value_2));
        case FLOAT:
            return (float_cast(value_1) == float_cast(value_2));
        case CHAR:
            return (char_cast(value_1) == char_cast(value_2));
        case STR:
            return (StrLen(str_cast(value_1)) == StrLen(str_cast(value_2)));
        default:
            return false;
    }
}

bool CheckGreaterValue(Type T, void *value_1, void *value_2)
{
    switch (T)
    {
        case INT:
            return (int_cast(value_1) > int_cast(value_2));
        case DOUBLE:
            return (double_cast(value_1) > double_cast(value_2));
        case FLOAT:
            return (float_cast(value_1) > float_cast(value_2));
        case CHAR:
            return (char_cast(value_1) > char_cast(value_2));
        case STR:
            return (StrLen(str_cast(value_1)) > StrLen(str_cast(value_2)));
        default:
            return false;
    }
}

void InsertionSort(Type T, void **array, int array_size) 
{ 
    int j = 0;
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

void *ReallocVoid(Vector_T **vec_data, int new_vector_size)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "ReallocVoid", -1)
        || CheckWarnings(*vec_data, VEC_NEW_SIZE, "ReallocVoid", new_vector_size))
    {
        return NULL;
    }

    (*vec_data)->capacity = new_vector_size;
    allocated_mem += (sizeof(void *) * (new_vector_size - (*vec_data)->size));
    return realloc((*vec_data)->data, sizeof(void*) * (new_vector_size));
}

Vector_T *NewVector(Type T)
{
    Vector_T *temp = malloc(sizeof(Vector_T));
    temp->T = T;
    allocated_mem += sizeof(Vector_T);
    return temp;
}

void FreeVector(Vector_T **vec_data)
{
    allocated_mem -= (sizeof(Vector_T));

    free(*vec_data);
    *vec_data = NULL;
}

void *NewVoid(int size)
{
    allocated_mem += sizeof(void *) * size;
    return malloc(sizeof(void *) * size);
}

void FreeVoid(Vector_T **vec_data)
{
    allocated_mem -= ((*vec_data)->capacity * sizeof(void*));

    (*vec_data)->size = 0;
    (*vec_data)->capacity = 0;
    free((*vec_data)->data);
}

Vector_T *VectorInit(Type T, void *array, int size)
{
    Vector_T *new_vector = NewVector(T);
    new_vector->capacity = size > DEFAULT_CAPACITY_SIZE ? size + DEFAULT_CAPACITY_SIZE : DEFAULT_CAPACITY_SIZE;
    new_vector->data = NewVoid(new_vector->capacity);

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **str_array= (char**)((char*)array);

                for (int i = 0; i < size; i++)
                {
                    new_vector->data[i] = (void*)*str_array;
                    str_array++;
                }
                break;
            }
        default:
            break;
    }

    new_vector->size = size;
    return new_vector;
}

void VectorPush(Vector_T **vec_data, Type T, void *value)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorPush", -1) 
        || CheckWarnings(*vec_data, VEC_TYPE, "VectorPush", T))
    {
        return;
    }

    if ((*vec_data)->capacity == (*vec_data)->size)
    {
        if ((*vec_data)->capacity == 0 && (*vec_data)->size == 0)
        {
            (*vec_data)->capacity = DEFAULT_CAPACITY_SIZE;
            (*vec_data)->data = NewVoid((*vec_data)->capacity);
        }
        else
        {
            void **new_data = ReallocVoid(vec_data, (*vec_data)->capacity + DEFAULT_CAPACITY_SIZE);
            (*vec_data)->data = new_data;
        }
    }

    (*vec_data)->data[(*vec_data)->size] = value;
    (*vec_data)->size++;
}

void VectorInsert(Vector_T **vec_data, Type T, void *value, int index)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorInsert", -1)
        || CheckWarnings(*vec_data, VEC_TYPE, "VectorInsert", T)
        || CheckWarnings(*vec_data, VEC_SIZE_LG, "VectorInsert", index))
    {
        return;
    }

    if ((*vec_data)->capacity == 0 && (*vec_data)->size == 0)
    {
        (*vec_data)->capacity = DEFAULT_CAPACITY_SIZE;
        (*vec_data)->data = NewVoid((*vec_data)->capacity);
    }
    else if ((*vec_data)->capacity == (*vec_data)->size)
    {
        void **new_data = ReallocVoid(vec_data, (*vec_data)->capacity + DEFAULT_CAPACITY_SIZE);
        (*vec_data)->data = new_data;
    }

    for (int i = ((*vec_data)->size); i >= index; i--)
    {
        (*vec_data)->data[i] = (*vec_data)->data[i - 1];
    }

    (*vec_data)->size++;
    (*vec_data)->data[index] = value;
}

void VectorExtened(Vector_T **vec_data, Type T, void *array, int size_array)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorExtened", -1)
        || CheckWarnings(*vec_data, VEC_TYPE, "VectorExtened", T))
    {
        return;
    }

    if ((*vec_data)->capacity == 0 && (*vec_data)->size == 0)
    {
        (*vec_data)->capacity = DEFAULT_CAPACITY_SIZE;
        (*vec_data)->data = NewVoid((*vec_data)->capacity);
    }

    int total_size = (*vec_data)->size + size_array;

    switch (T)
    {
        case INT:
            {
                int *data_array = (int*)array;

                for (int i = (*vec_data)->size; i < total_size; i++)
                {
                    if (i == (*vec_data)->capacity)
                    {
                        void **new_data = ReallocVoid(vec_data, (*vec_data)->capacity + DEFAULT_CAPACITY_SIZE);
                        (*vec_data)->data = new_data;
                    }

                    (*vec_data)->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)array);

                for (int i = (*vec_data)->size; i < total_size; i++)
                {
                    if (i == (*vec_data)->capacity)
                    {
                        void **new_data = ReallocVoid(vec_data, (*vec_data)->capacity + DEFAULT_CAPACITY_SIZE);
                        (*vec_data)->data = new_data;
                    }

                    (*vec_data)->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)array);

                for (int i = (*vec_data)->size; i < total_size; i++)
                {
                    if (i == (*vec_data)->capacity)
                    {
                        void **new_data = ReallocVoid(vec_data, (*vec_data)->capacity + DEFAULT_CAPACITY_SIZE);
                        (*vec_data)->data = new_data;
                    }

                    (*vec_data)->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)array);

                for (int i = (*vec_data)->size; i < total_size; i++)
                {
                    if (i == (*vec_data)->capacity)
                    {
                        void **new_data = ReallocVoid(vec_data, (*vec_data)->capacity + DEFAULT_CAPACITY_SIZE);
                        (*vec_data)->data = new_data;
                    }

                    (*vec_data)->data[i] = (void*)data_array;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array= (char**)((char*)array);

                for (int i = (*vec_data)->size; i < total_size; i++)
                {
                    if (i == (*vec_data)->capacity)
                    {
                        void **new_data = ReallocVoid(vec_data, (*vec_data)->capacity + DEFAULT_CAPACITY_SIZE);
                        (*vec_data)->data = new_data;
                    }

                    (*vec_data)->data[i] = (void*)*data_array;
                    data_array++;
                }
            }
            break;
        default:
            return;
    }

    (*vec_data)->size = total_size;
}

void VectorPop(Vector_T **vec_data)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorPop", -1) 
        || CheckWarnings(*vec_data, VEC_SIZE_ZERO, "VectorPop", -1))
    {
        return;
    }

    (*vec_data)->size--;
    (*vec_data)->data[(*vec_data)->size] = NULL;
}

void VectorPopIndex(Vector_T **vec_data, int index)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorPopIndex", -1)
        || CheckWarnings(*vec_data, VEC_SIZE_ZERO, "VectorPopIndex", -1) 
        || CheckWarnings(*vec_data, VEC_SIZE_LGE, "VectorPopIndex", index))
    {
        return;
    }

    for (int i = index; i < ((*vec_data)->size)-1; i++)
    {
        (*vec_data)->data[i] = (*vec_data)->data[i + 1];
    }

    (*vec_data)->size--;
    (*vec_data)->data[(*vec_data)->size] = NULL;
}

void VectorRemoveValue(Vector_T **vec_data, Type T, void *value)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorRemoveValue", -1) 
        || CheckWarnings(*vec_data, VEC_TYPE, "VectorRemoveValue", T)
        || CheckWarnings(*vec_data, VEC_SIZE_ZERO, "VectorRemoveValue", -1))
    {
        return;
    }

    for (int i = 0; i < (*vec_data)->size; i++)
    {
        if (CheckEqualValue((*vec_data)->T, (*vec_data)->data[i], value))
        {
            VectorPopIndex(vec_data, i);
            return;
        }
    }

    printf("Warning in VectorRemoveValue: Value could not be found in the vector.\n");
}

void VectorReverse(Vector_T **vec_data)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorReverse", -1)
        || CheckWarnings(*vec_data, VEC_SIZE_ZERO, "VectorReverse", -1))
    {
        return;
    }

    int half_size = (*vec_data)->size / 2;

    for (int i = 0; i < half_size; i++)
    {
        void* temp = (*vec_data)->data[i];
        (*vec_data)->data[i] = (*vec_data)->data[((*vec_data)->size - 1) - i];
        (*vec_data)->data[((*vec_data)->size - 1) - i] = temp;
    }
}

void VectorSort(Vector_T **vec_data)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorSort", -1)
        || CheckWarnings(*vec_data, VEC_SIZE_ZERO, "VectorSort", -1))
    {
        return;
    }

    InsertionSort((*vec_data)->T, (*vec_data)->data, (*vec_data)->size);
}

void VectorClear(Vector_T **vec_data)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorClear", -1))
    {
        return;
    }

    FreeVoid(vec_data);
}

void VectorFree(Vector_T **vec_data)
{
    if (CheckWarnings(*vec_data, VEC_NULL, "VectorFree", -1))
    {
        return;
    }

    VectorClear(vec_data);
    FreeVector(vec_data);
}

void VectorPrint(Vector_T *vec_data)
{
    if (CheckWarnings(vec_data, VEC_NULL, "VectorPrint", -1) 
        || CheckWarnings(vec_data, VEC_SIZE_ZERO, "VectorPrint", -1))
    {
        return;
    }

    for (int i = 0; i < vec_data->size; i++)
    {
        PrintT(vec_data->T, vec_data->data[i], "", "\n");
    }
}