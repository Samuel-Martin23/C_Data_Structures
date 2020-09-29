#include <stdlib.h>
#include <stdio.h>

typedef enum boolean {false, true} bool;
typedef enum T {INT, DOUBLE, FLOAT, CHAR, STR} Type;

#define intA(...)    INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define doubleA(...) DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define floatA(...)  FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define charA(...)   CHAR, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define strA(...)    STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

#define int(x)      INT, (int[]){x}
#define double(x)   DOUBLE, (double[]){x}
#define float(x)    FLOAT, (float[]){x}
#define char(x)     CHAR, (char[]){x}
#define str(x)      STR, (char*)x

#define int_cast(x)     (*(int*)x)
#define double_cast(x)  (*(double*)x)
#define float_cast(x)   (*(float*)x)
#define char_cast(x)    (*(char*)x)
#define str_cast(x)     ((char*)x)

int allocated_mem = 0;

typedef struct Vctr
{
    int size;
    int capacity;
    void **data;
    Type T;
} Vctr_T;

typedef Vctr_T* Vector_T;

void print(Type T, void *word)
{
    switch (T)
    {
        case INT:
            printf("%d\n", int_cast(word));
            break;
        case DOUBLE:
            printf("%.2f\n", double_cast(word));
            break;
        case FLOAT:
            printf("%.2f\n", float_cast(word));
            break;
        case CHAR:
            printf("%c\n", char_cast(word));
            break;
        case STR:
            printf("%s\n", str_cast(word));
            break;
        default:
            printf("Unidentified Type.\n");
            break;
    }
}

int StrLen(const char *word)
{
    int len = 0;

    while (*word != '\0')
    {
        word++;
        len++;
    }

    return len;
}

bool CheckEqualValue(Type T, void *index, void *value)
{
    switch (T)
    {
        case INT:
            return (int_cast(index) == int_cast(value));
        case DOUBLE:
            return (double_cast(index) == double_cast(value));
        case FLOAT:
            return (float_cast(index) == float_cast(value));
        case CHAR:
            return (char_cast(index) == char_cast(value));
        case STR:
            return (str_cast(index) == str_cast(value));
        default:
            return false;
    }
}

bool CheckGreaterValue(Type T, void *index, void *key)
{
    switch (T)
    {
        case INT:
            return (int_cast(index) > int_cast(key));
        case DOUBLE:
            return (double_cast(index) > double_cast(key));
        case FLOAT:
            return (float_cast(index) > float_cast(key));
        case CHAR:
            return (char_cast(index) > char_cast(key));
        case STR:
            return (StrLen(str_cast(index)) > StrLen(str_cast(key)));
        default:
            return false;
    }
}

void VectorPrint(Vctr_T **v)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorPrint: vector is NULL.\n");
        return;
    }

    switch ((*v)->T)
    {
        case INT:
            for (int i = 0; i < (*v)->size; i++)
            {
                printf("|%d| ", int_cast((*v)->data[i]));
            }
            break;
        case DOUBLE:
            for (int i = 0; i < (*v)->size; i++)
            {
                printf("|%.2f| ", double_cast((*v)->data[i]));
            }
            break;
        case FLOAT:
            for (int i = 0; i < (*v)->size; i++)
            {
                printf("|%.2f| ", float_cast((*v)->data[i]));
            }
            break;
        case CHAR:
            for (int i = 0; i < (*v)->size; i++)
            {
                printf("|%c| ", char_cast((*v)->data[i]));
            }
            break;
        case STR:
            for (int i = 0; i < (*v)->size; i++)
            {
                printf("|%s| ", str_cast((*v)->data[i]));
            }
            break;
        default:
            printf("Unidentified Type.\n");
            break;
    }

    printf("\n");
}

void PrintAllocatedMemory()
{
    printf("Bytes Allocated: %d\n", allocated_mem);
}

void *RealocMem(Vctr_T **v, int new_array_size)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in RealocMem: vector is NULL.\n");
        return NULL;
    }
    else if (new_array_size <= (*v)->size)
    {
        printf("Warning encountered in RealocMem: new_array_size is less than the current size of the vector.\n");
        return NULL; 
    }

    (*v)->capacity = new_array_size;
    allocated_mem += (sizeof(void *) * (new_array_size - (*v)->size));
    return realloc((*v)->data, sizeof(void*) * (new_array_size));
}

Vctr_T *NewMemVec(Type T)
{
    Vctr_T *temp = malloc(sizeof(Vctr_T));
    temp->T = T;
    allocated_mem += sizeof(Vctr_T);
    return temp;
}

void *NewMemVoid(int size)
{
    allocated_mem += sizeof(void *) * size;
    return malloc(sizeof(void *) * size);
}

Vctr_T *VectorInit(Type T, void *array, int size)
{
    Vctr_T *new_vector = NewMemVec(T);
    new_vector->capacity = size > 50 ? size + 50 : 50;
    new_vector->data = NewMemVoid(new_vector->capacity);

    if (T == STR)
    {
        char **str_array= (char**)((char*)array);

        for (int i = 0; i < size; i++)
        {
            new_vector->data[i] = (void*)*str_array;
            new_vector->size++;
            str_array++;
        }
    }
    else if (T == DOUBLE)
    {
        double *double_array = ((double*)array);

        for (int i = 0; i < size; i++)
        {
            new_vector->data[i] = (void*)double_array;
            new_vector->size++;
            double_array++;
        }
    }
    else
    {
        int* int_array = ((int*)array);

        for (int i = 0; i < size; i++)
        {
            new_vector->data[i] = (void*)int_array;
            new_vector->size++;
            int_array++;
        }
    }

    return new_vector;
}

void VectorPush(Vctr_T **v, Type T, void *value)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorPush: vector is NULL.\n");
        return;
    }
    else if ((*v)->T != T)
    {
        printf("Warning encountered in VectorPush: vector type does not equal the type of the new value.\n");
        return;
    }

    if ((*v)->capacity == (*v)->size)
    {
        void **new_data = RealocMem(v, (*v)->size + 50);
        (*v)->data = new_data;
    }

    (*v)->data[(*v)->size] = value;
    (*v)->size++;
}

void VectorPop(Vctr_T **v)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorPop: vector is NULL.\n");
        return;
    }
    else if ((*v)->size <= 0)
    {
        printf("Warning encountered in VectorPop: vector size is 0.\n");
        return;
    }

    (*v)->size--;
    (*v)->data[(*v)->size] = NULL;
}

void VectorPopIndex(Vctr_T **v, int index)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorPopIndex: vector is NULL.\n");
        return;
    }
    else if (index < 0 || index >= (*v)->size)
    {
        printf("Warning encountered in VectorPopIndex: index out of range.\n");
        return;
    }

    for (int i = index; i < ((*v)->size)-1; i++)
    {
        (*v)->data[i] = (*v)->data[i + 1];
    }

    (*v)->size--;
    (*v)->data[(*v)->size] = NULL;
}

void VectorRemoveValue(Vctr_T **v, Type T, void *value)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorRemoveValue: vector is NULL.\n");
        return;
    }
    else if ((*v)->T != T)
    {
        printf("Warning encountered in VectorRemoveValue: vector type does not equal the type of the new value.\n");
        return;
    }

    for (int i = 0; i < (*v)->size; i++)
    {
        if (CheckEqualValue((*v)->T, (*v)->data[i], value))
        {
            VectorPopIndex(v, i);
            return;
        }
    }

    printf("Value could not be found in the vector.\n");
}

void VectorClear(Vctr_T **v)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorClear: vector is NULL.\n");
        return;
    }

    allocated_mem -= (sizeof(Vctr_T) + (*v)->capacity * sizeof(void*));

    free((*v)->data);
    free(*v);
    *v = NULL;
}

void VectorExtened(Vctr_T **v, Type T, void *array, int size_array)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorExtened: vector is NULL.\n");
        return;
    }
    else if ((*v)->T != T)
    {
        printf("Warning encountered in VectorExtened: vector type does not equal the type of the new value.\n");
        return;
    }

    int original_size = (*v)->size;
    int total_size = original_size + size_array;

    if (T == STR)
    {
        char **str_array= (char**)((char*)array);

        for (int i = original_size; i < total_size; i++)
        {
            if ((*v)->capacity == i)
            {
                void **new_data = RealocMem(v, (*v)->size + 50);
                (*v)->data = new_data;
            }

            (*v)->data[i] = (void*)*str_array;
            (*v)->size++;
            str_array++;
        }
    }
    else if (T == DOUBLE)
    {
        double *double_array = ((double*)array);

        for (int i = original_size; i < total_size; i++)
        {
            if ((*v)->capacity == i)
            {
                void **new_data = RealocMem(v, (*v)->size + 50);
                (*v)->data = new_data;
            }

            (*v)->data[i] = (void*)double_array;
            (*v)->size++;
            double_array++;
        }
    }
    else
    {
        int* int_array = (int*)array;

        for (int i = original_size; i < total_size; i++)
        {
            if ((*v)->capacity == i)
            {
                void **new_data = RealocMem(v, (*v)->size + 50);
                (*v)->data = new_data;
            }

            (*v)->data[i] = (void*)int_array;
            (*v)->size++;
            int_array++;
        }
    }
}

void VectorInsert(Vctr_T **v, Type T, void *value, int index)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorInsert: vector is NULL.\n");
        return;
    }
    else if ((*v)->T != T)
    {
        printf("Warning encountered in VectorInsert: vector type does not equal the type of the new value.\n");
        return;
    }
    else if (index < 0 || index > (*v)->size)
    {
        printf("Warning encountered in VectorInsert: index out of range.\n");
        return;
    }

    if ((*v)->capacity == (*v)->size)
    {
        void **new_data = RealocMem(v, (*v)->size + 50);
        (*v)->data = new_data;
    }

    (*v)->size++;

    for (int i = ((*v)->size)-1; i > index; i--)
    {
        (*v)->data[i] = (*v)->data[i - 1];
    }

    (*v)->data[index] = value;
}

void VectorReverse(Vctr_T **v)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorReverse: vector is NULL.\n");
        return;
    }

    int half_size = (*v)->size / 2;

    for (int i = 0; i < half_size; i++)
    {
        void* temp = (*v)->data[i];
        (*v)->data[i] = (*v)->data[((*v)->size - 1) - i];
        (*v)->data[((*v)->size - 1) - i] = temp;
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

void VectorSort(Vctr_T **v)
{
    if ((*v) == NULL)
    {
        printf("Warning encountered in VectorSort: vector is NULL.\n");
        return;
    }

    InsertionSort((*v)->T, (*v)->data, (*v)->size);
}