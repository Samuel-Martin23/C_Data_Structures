
#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LINKED_LIST_TYPE            0x00000001
#define LINKED_LIST_HEAD_NULL       0x00000002
#define LINKED_LIST_INDEX_LGE       0x00000004

#define IntCastVoid(value)          INT, &(int){value}
#define DoubleCastVoid(value)       DOUBLE, &(double){value}
#define FloatCastVoid(value)        FLOAT, &(float){value}
#define CharCastVoid(value)         CHAR, &(char){value}
#define StrCastVoid(value)          STR, (char*){value}

#define IntCastVoidArray(...)       INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define DoubleCastVoidArray(...)    DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define FloatCastVoidArray(...)     FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define CharCastVoidArray(...)      CHAR, (char[]){__VA_ARGS__}, sizeof((char[]){__VA_ARGS__})/sizeof((char[]){__VA_ARGS__}[0])
#define StrCastVoidArray(...)       STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

#define InitLinkedListInt(...)      InitLinkedList(IntCastVoidArray(__VA_ARGS__))
#define InitLinkedListDouble(...)   InitLinkedList(DoubleCastVoidArray(__VA_ARGS__))
#define InitLinkedListFloat(...)    InitLinkedList(FloatCastVoidArray(__VA_ARGS__))
#define InitLinkedListChar(...)     InitLinkedList(CharCastVoidArray(__VA_ARGS__))
#define InitLinkedListStr(...)      InitLinkedList(StrCastVoidArray(__VA_ARGS__))

typedef enum T {INT, DOUBLE, FLOAT, CHAR, STR} Type;

typedef struct Node
{
    void *data;
    struct Node *next;
} Node_T;

typedef struct Linked_List
{
    Node_T *head;
    Node_T *tail;
    int size;
    Type T;
    int allocated_mem;
} Linked_List_T;

int VoidCastInt(void *value);
double VoidCastDouble(void *value);
float VoidCastFloat(void *value);
char VoidCastChar(void *value);
const char *VoidCastStr(void *value);

void PrintT(Type T, Node_T *curr, const char *beginning, const char *end);
bool CheckWarnings(Linked_List_T *list, int warning_code, const char *function_name, int check_value);
void PrintAllocatedMemory(Linked_List_T *list);
void PrintListSize(Linked_List_T *list);
bool CheckLessEqualValue(Type T, void *value1, void *value2);
bool CheckGreaterValue(Type T, void *value1, void *value2);
bool CheckEqualValue(Type T, void *value1, void *value2);
Node_T *MergeLists(Node_T *left, Node_T *right, Type T);
void PartitionList(Node_T *head, Node_T **front, Node_T **back);
void SortDataValues(Node_T **head, Type T);
void *NewValue(Linked_List_T *list, void *value);
void FreeIndex(Linked_List_T *list, Node_T **curr);
Node_T *NewNode(Linked_List_T *list, void *data);
void FreeValue(Linked_List_T *list, Node_T **curr);

Linked_List_T InitLinkedList(Type T, void *data, int data_len);
void LinkedListAppend(Linked_List_T *list, Type T, void *data);
void LinkedListInsert(Linked_List_T *list, int index, Type T, void *data);
void LinkedListExtend(Linked_List_T *list, Type T, void *data, int data_len);
void LinkedListRemoveIndex(Linked_List_T *list, int index);
void LinkedListRemoveValue(Linked_List_T *list, Type T, void *value);
Node_T *LinkedListGetValue(Linked_List_T *list, int index);
bool LinkedListCheckValue(Linked_List_T *list, Type T, void *data);
void LinkedListReverse(Linked_List_T *list);
void LinkedListSort(Linked_List_T *list);
void LinkedListFree(Linked_List_T *list);
void LinkedListPrint(Linked_List_T *list);


#endif //LINKED_LIST_H