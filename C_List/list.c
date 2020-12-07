
#include "list.h"

int VoidCastInt(void *value) {return (*(int*)value);}
double VoidCastDouble(void *value) {return (*(double*)value);}
float VoidCastFloat(void *value) {return (*(float*)value);}
char VoidCastChar(void *value) {return (*(const char*)value);}
const char *VoidCastStr(void *value) {return ((const char*)value);}

void PrintT(Node_T *curr, const char *beginning, const char *end)
{
    if (curr == NULL)
    {
        return;
    }

    printf("%s", beginning);

    switch (curr->T)
    {
        case INT:
            printf("%d", VoidCastInt(curr->data));
            break;
        case DOUBLE:
            printf("%.4f", VoidCastDouble(curr->data));
            break;
        case FLOAT:
            printf("%.4f", VoidCastFloat(curr->data));
            break;
        case CHAR:
            printf("%c", VoidCastChar(curr->data));
            break;
        case STR:
            printf("%s", VoidCastStr(curr->data));
            break;
        default:
            printf("Unidentified Type.");
            break;
    }

    printf("%s", end);
}

void PrintAllocatedMemory(Linked_List_T *list)
{
    printf("Bytes Allocated:%d\n", list->allocated_mem);
}

void PrintListSize(Linked_List_T *list)
{
    printf("List Size: %d\n", list->size);
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
            return (VoidCastInt(value1) <= VoidCastInt(value2));
        case DOUBLE:
            return (VoidCastDouble(value1) <= VoidCastDouble(value2));
        case FLOAT:
            return (VoidCastFloat(value1) <= VoidCastFloat(value2));
        case CHAR:
            return (VoidCastChar(value1) <= VoidCastChar(value2));
        case STR:
            return (StrLen(VoidCastStr(value1)) <= StrLen(VoidCastStr(value2)));
        default:
            return false;
    }
}

bool CheckGreaterValue(Type T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (VoidCastInt(value1) > VoidCastInt(value2));
        case DOUBLE:
            return (VoidCastDouble(value1) > VoidCastDouble(value2));
        case FLOAT:
            return (VoidCastFloat(value1) > VoidCastFloat(value2));
        case CHAR:
            return (VoidCastChar(value1) > VoidCastChar(value2));
        case STR:
            return (StrLen(VoidCastStr(value1)) > StrLen(VoidCastStr(value2)));
        default:
            return false;
    }
}

bool CheckEqualValue(Type T, void *value1, void *value2)
{
    switch (T)
    {
        case INT:
            return (VoidCastInt(value1) == VoidCastInt(value2));
        case DOUBLE:
            return (VoidCastDouble(value1) == VoidCastDouble(value2));
        case FLOAT:
            return (VoidCastFloat(value1) == VoidCastFloat(value2));
        case CHAR:
            return (VoidCastChar(value1) == VoidCastChar(value2));
        case STR:
            return (StrLen(VoidCastStr(value1)) == StrLen(VoidCastStr(value2)));
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

Node_T *NewNode(Linked_List_T *list, Type T, void *data)
{
    Node_T *curr = malloc(sizeof(Node_T));
    curr->data = data;
    curr->next = NULL;
    curr->T = T;
    list->allocated_mem += sizeof(Node_T);
    return curr;
}

void FreeNode(Linked_List_T *list, Node_T **curr)
{
    free(*curr);
    *curr = NULL;
    list->allocated_mem -= sizeof(Node_T);
}

Linked_List_T InitLinkedList(Type T, void *data, int size)
{
    Linked_List_T new_list;
    Node_T *top = NULL;
    Node_T *curr = NULL;

    new_list.size = size;

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)data);

                new_list.head = NewNode(&new_list, T, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array= ((double*)data);

                new_list.head = NewNode(&new_list, T, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array= ((float*)data);

                new_list.head = NewNode(&new_list, T, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);

                new_list.head = NewNode(&new_list, T, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array = (char**)((char*)data);

                new_list.head = NewNode(&new_list, T, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        default:
            exit(1);
            break;
    }

    new_list.tail = top;
    return new_list;
}

void LinkedListAppend(Linked_List_T *list, Type T, void *data)
{
    if (list->head == NULL)
    {
        list->head = NewNode(list, T, data);
        list->tail = list->head;
        return;
    }
    else if (list->tail == NULL)
    {
        return;
    }

    Node_T **tail = &(list->tail);
    Node_T *curr = NewNode(list, (*tail)->T, data);

    (*tail)->next = curr;
    *tail = (*tail)->next;
    list->size++;
}

void LinkedListInsert(Linked_List_T *list, int index, Type T, void *data)
{
    if (list->head == NULL)
    {
        return;
    }

    if (index < 0 || index >= list->size)
    {
        return;
    }

    Node_T **head = &(list->head);

    if (index == 0)
    {
        Node_T *temp = NewNode(list, (*head)->T, data);
        temp->next = *head;
        *head = temp;
        list->size++;
        return;
    }
    else if (index == list->size-1)
    {
        LinkedListAppend(list, T, data);
        return;
    }

    Node_T *prev = NULL;
    Node_T *curr = *head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    Node_T *temp = NewNode(list, curr->T, data);
    prev->next = temp;
    temp->next = curr;
    list->size++;
}

void LinkedListExtend(Linked_List_T *list, Type T, void *data, int size)
{
    if (list->head == NULL)
    {
        *list = InitLinkedList(T, data, size);
        return;
    }
    else if (list->tail == NULL)
    {
        return;
    }

    Node_T *curr = NULL;
    Node_T **tail = &(list->tail);

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = NewNode(list, (*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
            }
            break;
        case DOUBLE:
            {
                double *data_array= ((double*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = NewNode(list, (*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
            }
            break;
        case FLOAT:
            {
                float *data_array= ((float*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = NewNode(list, (*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = NewNode(list, (*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
            }
            break;
        case STR:
            {
                char **data_array = (char**)((char*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = NewNode(list, (*tail)->T, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
            }
            break;
        default:
            return;
    }

    list->size += size;
}

void LinkedListRemoveIndex(Linked_List_T *list, int index)
{
    if (list->head == NULL)
    {
        return;
    }

    if (index < 0 || index >= list->size)
    {
        return;
    }

    Node_T **head = &(list->head);

    if (index == 0) 
    {
        Node_T* next = (*head)->next;
        FreeNode(list, head);
        *head = next;
        list->size--;
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
    FreeNode(list, &curr);
    list->size--;
}

void LinkedListRemoveValue(Linked_List_T *list, Type T, void *value)
{
    if (list->tail == NULL)
    {
        return;
    }

    Node_T **head = &(list->head);

    if (CheckEqualValue(T, (*head)->data, value))
    {
        Node_T *next = (*head)->next;
        FreeNode(list, head);
        *head = next;
        list->size--;
        return;
    }

    Node_T *curr = *head;
    Node_T *prev = NULL;

    while (curr != NULL)
    {
        if (CheckEqualValue(T, curr->data, value))
        {
            if (curr->next == NULL)
            {
                list->tail = prev;
            }

            prev->next = curr->next;
            FreeNode(list, &curr);
            list->size--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

Node_T *LinkedListGetValue(Linked_List_T *list, int index)
{
    Node_T *top = NULL;
    
    if (list->head == NULL)
    {
        return top;
    }

    if (index < 0 || index >= list->size)
    {
        return top;
    }

    top = list->head;

    for (int i = 0; i < index; i++)
    {
        if (top == NULL)
        {
            return top;
        }

        top = top->next;
    }

    return top;
}

bool LinkedListCheckValue(Linked_List_T *list, Type T, void *data)
{
    if (list->head == NULL)
    {
        return false;
    }

    Node_T *top = list->head;

    while (top != NULL)
    {
        if (CheckEqualValue(T, top->data, data))
        {
            return true;
        }

        top = top->next;
    }

    return false;
}

void LinkedListReverse(Linked_List_T *list)
{
    Node_T *curr = list->head;
    Node_T *prev = NULL;
    Node_T *next = NULL;

    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    list->head = prev;
}

void LinkedListSort(Linked_List_T *list)
{
    SortDataValues(&(list->head));
    
    if (list->tail != NULL)
    {
        Node_T *tail = list->tail;

        while (tail->next != NULL)
        {
            tail = tail->next;
        }

        list->tail = tail;
    }
}

void LinkedListFree(Linked_List_T *list)
{
    if (list->tail == NULL)
    {
        return;
    }

    Node_T **head = &(list->head);
    Node_T *succeeding = (*head)->next;

    while (succeeding != NULL)
    {
        FreeNode(list, head);
        list->size--;
        *head = succeeding;
        succeeding = succeeding->next;
    }

    FreeNode(list, head);
    list->size--;
    list->tail = NULL;
}

void LinkedListPrint(Linked_List_T *data_set)
{
    if (data_set->head == NULL)
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