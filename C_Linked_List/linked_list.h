
#ifndef LINKED_LIST_H
#define LINKED_LIST_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../Template/template.h"

#define LinkedListInitInt(...)      InitLinkedList(IntCastVoidArray(__VA_ARGS__))
#define LinkedListInitDouble(...)   InitLinkedList(DoubleCastVoidArray(__VA_ARGS__))
#define LinkedListInitFloat(...)    InitLinkedList(FloatCastVoidArray(__VA_ARGS__))
#define LinkedListInitChar(...)     InitLinkedList(CharCastVoidArray(__VA_ARGS__))
#define LinkedListInitStr(...)      InitLinkedList(StrCastVoidArray(__VA_ARGS__))

typedef struct Node
{
    void *data;
    struct Node *next;
} Node_t;

typedef struct Linked_List
{
    Node_t *head;
    Node_t *tail;
    int size;
    Template T;
    int allocated_mem;
} Linked_List_t;

void PrintAllocatedMemList(Linked_List_t *list);
void PrintListSize(Linked_List_t *list);

Linked_List_t InitLinkedList(Template T, void *data, int data_len);
void LinkedListAppend(Linked_List_t *list, Template T, void *data);
void LinkedListInsert(Linked_List_t *list, int index, Template T, void *data);
void LinkedListExtend(Linked_List_t *list, Template T, void *data, int data_len);
void LinkedListRemoveIndex(Linked_List_t *list, int index);
void LinkedListRemoveValue(Linked_List_t *list, Template T, void *value);
Node_t *LinkedListGetValue(Linked_List_t *list, int index);
bool LinkedListCheckValue(Linked_List_t *list, Template T, void *data);
void LinkedListReverse(Linked_List_t *list);
void LinkedListSort(Linked_List_t *list);
void LinkedListFree(Linked_List_t *list);
void LinkedListPrint(Linked_List_t *list);


#endif //LINKED_LIST_H