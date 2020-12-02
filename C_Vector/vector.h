
#ifndef VECTOR_H
#define VECTOR_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define IntCastVoid(value)          INT, &(int){value}
#define DoubleCastVoid(value)       DOUBLE, &(double){value}
#define FloatCastVoid(value)        FLOAT, &(float){value}
#define CharCastVoid(value)         CHAR, &(char){value}
#define StrCastVoid(value)          STR, (char*){value}

#define IntCastVoidArray(...)       INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define DoubleCastVoidArray(...)    DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define FloatCastVoidArray(...)     FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define CharCastVoidArray(...)      CHAR, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define StrCastVoidArray(...)       STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

#define VectorInitInt(...)          VectorInit(IntCastVoidArray(__VA_ARGS__))
#define VectorInitDouble(...)       VectorInit(DoubleCastVoidArray(__VA_ARGS__))
#define VectorInitFloat(...)        VectorInit(FloatCastVoidArray(__VA_ARGS__))
#define VectorInitChar(...)         VectorInit(CharCastVoidArray(__VA_ARGS__))
#define VectorInitStr(...)          VectorInit(StrCastVoidArray(__VA_ARGS__))

#define VEC_FREE                    0x00000001
#define VEC_SIZE_ZERO               0x00000002
#define VEC_TYPE                    0x00000004
#define VEC_SIZE_G                  0x00000008
#define VEC_SIZE_GE                 0x00000010
#define VEC_NEW_SIZE                0x00000020

#define DEFAULT_CAPACITY_SIZE       100

typedef enum T {INT, DOUBLE, FLOAT, CHAR, STR} Type;

typedef struct Vector
{
    int size;
    int capacity;
    void **data;
    Type T;
    int allocated_mem;
} Vector_T;

int VoidCastInt(void *value);
double VoidCastDouble(void *value);
float VoidCastFloat(void *value);
char VoidCastChar(void *value);
const char *VoidCastStr(void *value);

bool CheckWarnings(Vector_T *vec_data, int warning_code, const char *function_name, int check_value);
void PrintT(Type T, void *value, const char *beginning, const char *end);
void PrintAllocatedMemory(Vector_T *vec_data);
void PrintVectorSize(Vector_T *vec_data);
int StrLen(const char *word);
bool CheckEqualValue(Type T, void *value_1, void *value_2);
bool CheckGreaterValue(Type T, void *value_1, void *value_2);
void InsertionSort(Type T, void **array, int array_size);
void *NewVoidElements(Vector_T *vec_data);
void *ReallocVoidElements(Vector_T *vec_data, int new_vector_size);

Vector_T VectorInit(Type T, void *array, int size);
void VectorPush(Vector_T *vec_data, Type T, void *value);
void VectorInsert(Vector_T *vec_data, Type T, void *value, int index);
void VectorExtend(Vector_T *vec_data, Type T, void *array, int size_array);
void VectorPop(Vector_T *vec_data);
void VectorPopIndex(Vector_T *vec_data, int index);
void VectorRemoveValue(Vector_T *vec_data, Type T, void *value);
void VectorReverse(Vector_T *vec_data);
void VectorSort(Vector_T *vec_data);
void VectorClear(Vector_T *vec_data);
void VectorFree(Vector_T *vec_data);
void VectorPrint(Vector_T *vec_data);


#endif //VECTOR_H