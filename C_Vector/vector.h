
#ifndef VECTOR_H
#define VECTOR_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../Template/template.h"

#define DEFAULT_CAPACITY_SIZE       100

#define VectorInitInt(...)          VectorInit(IntCastVoidArray(__VA_ARGS__))
#define VectorInitDouble(...)       VectorInit(DoubleCastVoidArray(__VA_ARGS__))
#define VectorInitFloat(...)        VectorInit(FloatCastVoidArray(__VA_ARGS__))
#define VectorInitChar(...)         VectorInit(CharCastVoidArray(__VA_ARGS__))
#define VectorInitStr(...)          VectorInit(StrCastVoidArray(__VA_ARGS__))

typedef struct Vector
{
    int size;
    int capacity;
    void **data;
    Template T;
    int allocated_mem;
} Vector_t;

void PrintAllocatedMemVector(Vector_t *vec_data);
void PrintVectorSize(Vector_t *vec_data);

Vector_t VectorInit(Template T, void *array, int size);
void VectorPush(Vector_t *vec_data, Template T, void *value);
void VectorInsert(Vector_t *vec_data, Template T, void *value, int index);
void VectorExtend(Vector_t *vec_data, Template T, void *array, int size_array);
void VectorPop(Vector_t *vec_data);
void VectorPopIndex(Vector_t *vec_data, int index);
void VectorRemoveValue(Vector_t *vec_data, Template T, void *value);
void VectorReverse(Vector_t *vec_data);
void VectorSort(Vector_t *vec_data);
void VectorClear(Vector_t *vec_data);
void VectorFree(Vector_t *vec_data);
void VectorPrint(Vector_t *vec_data);


#endif //VECTOR_H