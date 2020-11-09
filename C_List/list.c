
#include <stdio.h>
#include <stdlib.h>

#define intA(...)                   INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define doubleA(...)                DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define floatA(...)                 FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define charA(...)                  CHAR, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define strA(...)                   STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

#define InitLinkedListInt(...)      InitLinkedList(intA(__VA_ARGS__))
#define InitLinkedListDouble(...)   InitLinkedList(doubleA(__VA_ARGS__))
#define InitLinkedListFloat(...)    InitLinkedList(floatA(__VA_ARGS__))
#define InitLinkedListChar(...)     InitLinkedList(charA(__VA_ARGS__))
#define InitLinkedListStr(...)      InitLinkedList(strA(__VA_ARGS__))

#define int(x)                      INT, (int[]){x}
#define double(x)                   DOUBLE, (double[]){x}
#define float(x)                    FLOAT, (float[]){x}
#define char(x)                     CHAR, (char[]){x}
#define str(x)                      STR, (char*)x

#define int_cast(x)                 (*(int*)x)
#define double_cast(x)              (*(double*)x)
#define float_cast(x)               (*(float*)x)
#define char_cast(x)                (*(char*)x)
#define str_cast(x)                 ((char*)x)

int allocated_mem = 0;

typedef enum boolean {false, true} bool;
typedef enum T {INT, DOUBLE, FLOAT, CHAR, STR} Type;

typedef struct Node
{
    void *data;
    struct Node *next;
    Type T;
} Node_T;

typedef struct Linked_List
{
    Node_T *head;
    Node_T *tail;
    int size;
} Linked_List_T;

bool CheckNodeNull(Node_T *curr)
{
    return (curr == NULL);
}

bool CheckListNull(Linked_List_T *data_set)
{
    return (data_set == NULL);
}

void PrintT(Node_T *curr, const char *beginning, const char *end)
{
    if (CheckNodeNull(curr))
    {
        return;
    }

    printf("%s", beginning);

    switch (curr->T)
    {
        case INT:
            printf("%d", int_cast(curr->data));
            break;
        case DOUBLE:
            printf("%.4f", double_cast(curr->data));
            break;
        case FLOAT:
            printf("%.4f", float_cast(curr->data));
            break;
        case CHAR:
            printf("%c", char_cast(curr->data));
            break;
        case STR:
            printf("%s", str_cast(curr->data));
            break;
        default:
            printf("Unidentified Type.");
            break;
    }

    printf("%s", end);
}

void PrintAllocatedMemory()
{
    printf("%d Bytes Allocated\n", allocated_mem);
}

void PrintListSize(Linked_List_T *data_set)
{
    if ((CheckListNull(data_set)))
    {
        return;
    }

    printf("List Size: %d\n", data_set->size);
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

bool CheckLessEqualValue(Type T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (int_cast(value1) <= int_cast(value2));
        case DOUBLE:
            return (double_cast(value1) <= double_cast(value2));
        case FLOAT:
            return (float_cast(value1) <= float_cast(value2));
        case CHAR:
            return (char_cast(value1) <= char_cast(value2));
        case STR:
            return (StrLen(str_cast(value1)) <= StrLen(str_cast(value2)));
        default:
            return false;
    }
}

bool CheckGreaterValue(Type T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (int_cast(value1) > int_cast(value2));
        case DOUBLE:
            return (double_cast(value1) > double_cast(value2));
        case FLOAT:
            return (float_cast(value1) > float_cast(value2));
        case CHAR:
            return (char_cast(value1) > char_cast(value2));
        case STR:
            return (StrLen(str_cast(value1)) > StrLen(str_cast(value2)));
        default:
            return false;
    }
}

bool CheckEqualValue(Type T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (int_cast(value1) == int_cast(value2));
        case DOUBLE:
            return (double_cast(value1) == double_cast(value2));
        case FLOAT:
            return (float_cast(value1) == float_cast(value2));
        case CHAR:
            return (char_cast(value1) == char_cast(value2));
        case STR:
            return (StrLen(str_cast(value1)) == StrLen(str_cast(value2)));
        default:
            return false;
    }
}

Node_T *MergeLists(Node_T *left, Node_T *right)
{
    if (left == NULL)
    {
        return right;
    }
    else if (right == NULL)
    {
        return left;
    }

    Node_T *merged_head = NULL;

    if (CheckLessEqualValue(right->T, left->data, right->data))
    {
        merged_head = left;
        merged_head->next = MergeLists(left->next, right);
    }
    else
    {
        merged_head = right;
        merged_head->next = MergeLists(left, right->next);
    }

    return merged_head;
}

void PartitionList(Node_T *head, Node_T **front, Node_T **back)
{
    Node_T *slow = NULL;
    Node_T *fast = NULL;

    if (head == NULL || head->next == NULL){

        *front = head;
        *back = NULL;
    }
    else
    {
        slow = head;
        fast = head->next;

        while (fast != NULL)
        {
            fast = fast->next;

            if (fast != NULL)
            {
                slow = slow->next;
                fast = fast->next;
            }
        }

        *front = head;
        *back = slow->next;
        slow->next = NULL;
    }
}

void SortDataValues(Node_T **head)
{
    Node_T *top = *head;
    Node_T *split_left = NULL;
    Node_T *split_right = NULL;

    if (top == NULL || top->next == NULL)
    {
        return;
    }

    PartitionList(top, &split_left, &split_right);

    SortDataValues(&split_left);
    SortDataValues(&split_right);

    *head = MergeLists(split_left, split_right);
}

Node_T *NewNode(Type T, void *data)
{
    Node_T *curr = malloc(sizeof(Node_T));
    curr->data = data;
    curr->next = NULL;
    curr->T = T;
    allocated_mem += sizeof(Node_T);
    return curr;
}

void FreeNode(Node_T **curr)
{
    free(*curr);
    *curr = NULL;

    if (allocated_mem <= 0)
    {
        return;
    }

    allocated_mem -= sizeof(Node_T);
}

Linked_List_T *NewLinkedList(Type T, void *data)
{
    Linked_List_T *curr = malloc(sizeof(Linked_List_T));
    curr->head = NewNode(T, data);
    curr->size++;
    allocated_mem += sizeof(Linked_List_T);
    return curr;
}

Linked_List_T *InitLinkedList(Type T, void *data, int data_len)
{
    Linked_List_T *new_list = NULL;
    Node_T *top = NULL;
    Node_T *curr = NULL;

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)data);
                new_list = NewLinkedList(T, data_array);
                top = new_list->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array= ((double*)data);
                new_list = NewLinkedList(T, data_array);
                top = new_list->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array= ((float*)data);
                new_list = NewLinkedList(T, data_array);
                top = new_list->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);
                new_list = NewLinkedList(T, data_array);
                top = new_list->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array = (char**)((char*)data);
                new_list = NewLinkedList(T, *data_array);
                top = new_list->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    curr = NewNode(T, *data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        default:
            break;
    }

    new_list->tail = top;
    new_list->size = data_len;
    return new_list;
}

void LinkedListAppend(Linked_List_T **list_data, Type T, void *data)
{
    if ( !(CheckListNull(*list_data)) && CheckNodeNull((*list_data)->head))
    {
        (*list_data)->head = NewNode(T, data);
        (*list_data)->tail = (*list_data)->head;
        return;
    }
    else if ((CheckListNull(*list_data)) || (CheckNodeNull((*list_data)->tail)))
    {
        return;
    }

    Node_T **tail = &((*list_data)->tail);
    Node_T *curr = NewNode((*tail)->T, data);

    (*tail)->next = curr;
    *tail = (*tail)->next;
    (*list_data)->size++;
}

void LinkedListInsert(Linked_List_T **data_set, int index, Type T, void *data)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->head)))
    {
        return;
    }

    if (index < 0 || index >= (*data_set)->size)
    {
        return;
    }

    Node_T **head = &((*data_set)->head);

    if (index == 0)
    {
        Node_T *temp = NewNode((*head)->T, data);
        temp->next = *head;
        *head = temp;
        (*data_set)->size++;
        return;
    }
    else if (index == (*data_set)->size-1)
    {
        LinkedListAppend(data_set, T, data);
        return;
    }

    Node_T *prev = NULL;
    Node_T *curr = *head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    Node_T *temp = NewNode(curr->T, data);
    prev->next = temp;
    temp->next = curr;
    (*data_set)->size++;
}

void LinkedListFree(Linked_List_T **curr);
void LinkedListExtend(Linked_List_T **data_set, Type T, void *data, int data_len)
{
    if (!(CheckListNull(*data_set)) && CheckNodeNull((*data_set)->head))
    {
        LinkedListFree(data_set);
        *data_set = InitLinkedList(T, data, data_len);
        return;
    }
    else if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->tail)))
    {
        return;
    }

    Node_T *curr = NULL;
    Node_T **tail = &((*data_set)->tail);

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)data);

                for (int i = 0; i < data_len; i++)
                {
                    curr = NewNode((*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                    (*data_set)->size++;
                }

                (*data_set)->tail = curr;
            }
            break;
        case DOUBLE:
            {
                double *data_array= ((double*)data);

                for (int i = 0; i < data_len; i++)
                {
                    curr = NewNode((*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                    (*data_set)->size++;
                }

                (*data_set)->tail = curr;
            }
            break;
        case FLOAT:
            {
                float *data_array= ((float*)data);

                for (int i = 0; i < data_len; i++)
                {
                    curr = NewNode((*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                    (*data_set)->size++;
                }

                (*data_set)->tail = curr;
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);

                for (int i = 0; i < data_len; i++)
                {
                    curr = NewNode((*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                    (*data_set)->size++;
                }

                (*data_set)->tail = curr;
            }
            break;
        case STR:
            {
                char **data_array = (char**)((char*)data);

                for (int i = 0; i < data_len; i++)
                {
                    curr = NewNode((*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                    (*data_set)->size++;
                }

                (*data_set)->tail = curr;
            }
            break;
        default:
            return;
    }

    (*data_set)->size += data_len;
}

void LinkedListRemoveIndex(Linked_List_T **data_set, int index)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->head)))
    {
        return;
    }

    if (index < 0 || index >= (*data_set)->size)
    {
        return;
    }

    Node_T **head = &((*data_set)->head);

    if (index == 0) 
    {
        Node_T* next = (*head)->next;
        FreeNode(head);
        *head = next;
        (*data_set)->size--;
        return;
    }

    Node_T *curr = *head;
    Node_T *prev = NULL;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    prev->next = curr->next;
    FreeNode(&curr);
    (*data_set)->size--;
}

void LinkedListRemoveValue(Linked_List_T **data_set, Type T, void *value)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->tail)))
    {
        return;
    }

    Node_T **head = &((*data_set)->head);

    if (CheckEqualValue(T, (*head)->data, value))
    {
        Node_T *next = (*head)->next;
        FreeNode(head);
        *head = next;
        (*data_set)->size--;
        return;
    }

    Node_T *curr = *head;
    Node_T *prev = NULL;

    while (!(CheckNodeNull(curr)))
    {
        if (CheckEqualValue(T, curr->data, value))
        {
            if (CheckNodeNull(curr->next))
            {
                (*data_set)->tail = prev;
            }

            prev->next = curr->next;
            FreeNode(&curr);
            (*data_set)->size--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

Node_T *GetValueFromIndex(Linked_List_T **data_set, int index)
{
    Node_T *top = NULL;
    
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->head)))
    {
        return top;
    }

    if (index < 0 || index >= (*data_set)->size)
    {
        return top;
    }

    top = ((*data_set)->head);

    for (int i = 0; i < index; i++)
    {
        if (CheckNodeNull(top))
        {
            return top;
        }

        top = top->next;
    }

    return top;
}

bool CheckValueLinkedList(Linked_List_T **data_set, Type T, void *data)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->head)))
    {
        return false;
    }

    Node_T *top = ((*data_set)->head);

    while (!(CheckNodeNull(top)))
    {
        if (CheckEqualValue(T, top->data, data))
        {
            return true;
        }

        top = top->next;
    }

    return false;
}

void LinkedListReverse(Linked_List_T **data_set)
{
    Node_T *curr = (*data_set)->head;
    Node_T *prev = NULL;
    Node_T *next = NULL;

    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    (*data_set)->head = prev;
}

void LinkedListSort(Linked_List_T **data_set)
{
    SortDataValues(&((*data_set)->head));
    
    if (!CheckNodeNull((*data_set)->tail))
    {
        Node_T *tail = (*data_set)->tail;

        while (!CheckNodeNull(tail->next))
        {
            tail = tail->next;
        }

        (*data_set)->tail = tail;
    }
}

void LinkedListClear(Linked_List_T **data_set)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->tail)))
    {
        return;
    }

    Node_T **head = &((*data_set)->head);
    Node_T *next = (*head)->next;

    while (!(CheckNodeNull(next)))
    {
        FreeNode(head);
        (*data_set)->size--;
        *head = next;
        next = next->next;
    }

    FreeNode(head);
    (*data_set)->size--;
    (*data_set)->tail = NULL;
}

void LinkedListFree(Linked_List_T **curr)
{
    LinkedListClear(curr);
    free(*curr);
    *curr = NULL;

    if (allocated_mem <= 0)
    {
        return;
    }

    allocated_mem -= sizeof(Linked_List_T);
}

void LinkedListPrint(Linked_List_T *data_set)
{
    if (data_set == NULL || data_set->head == NULL)
    {
        return;
    }

    Node_T *top = data_set->head;

    while (top != NULL)
    {
        PrintT(top, "", "");
        printf("-->");
        top = top->next;
    }

    printf("NULL\n");
}