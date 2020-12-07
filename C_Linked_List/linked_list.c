
#include "linked_list.h"

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

bool CheckWarnings(Linked_List_T *list, int warning_code, const char *function_name, int check_value)
{
    if (warning_code & LINKED_LIST_TYPE)
    {
        Type T = check_value;

        if (list->T != T)
        {
            printf("Warning in %s: Linked List type does not equal the new value type.\n", function_name);
            return true;
        }
    }

    if (warning_code & LINKED_LIST_HEAD_NULL)
    {
        if (list->head == NULL)
        {
            printf("Warning in %s: Linked List head is NULL.\n", function_name);
            return true;
        }
    }

    if (warning_code & LINKED_LIST_INDEX_LGE)
    {
        int index = check_value;

        if (index < 0 || index >= list->size)
        {
            printf("Warning in %s: Index out of range.\n", function_name);
            return true;
        }
    }

    return false;
}

void PrintT(Type T, Node_T *curr, const char *beginning, const char *end)
{
    if (curr == NULL)
    {
        return;
    }

    printf("%s", beginning);

    switch (T)
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
    printf("Bytes Allocated: %d\n", list->allocated_mem);
}

void PrintListSize(Linked_List_T *list)
{
    if (CheckWarnings(list, LINKED_LIST_HEAD_NULL, "PrintListSize", -1))
    {
        return;
    }

    printf("List Size: %d\n", list->size);
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
            return (strlen(VoidCastStr(value1)) <= strlen(VoidCastStr(value2)));
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
            return (strlen(VoidCastStr(value1)) > strlen(VoidCastStr(value2)));
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
            return (strlen(VoidCastStr(value1)) == strlen(VoidCastStr(value2)));
        default:
            return false;
    }
}

Node_T *MergeLists(Node_T *left, Node_T *right, Type T)
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

    if (CheckLessEqualValue(T, left->data, right->data))
    {
        merged_head = left;
        merged_head->next = MergeLists(left->next, right, T);
    }
    else
    {
        merged_head = right;
        merged_head->next = MergeLists(left, right->next, T);
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

void SortDataValues(Node_T **head, Type T)
{
    Node_T *top = *head;
    Node_T *split_left = NULL;
    Node_T *split_right = NULL;

    if (top == NULL || top->next == NULL)
    {
        return;
    }

    PartitionList(top, &split_left, &split_right);

    SortDataValues(&split_left, T);
    SortDataValues(&split_right, T);

    *head = MergeLists(split_left, split_right, T);
}

void *NewValue(Linked_List_T *list, void *value)
{
    switch (list->T)
    {
        case INT:
            {
                int *allocated_value = malloc(sizeof(int));
                *allocated_value = VoidCastInt(value);
                list->allocated_mem += sizeof(int);
                value = allocated_value;
            }
            break;
        case DOUBLE:
            {
                double *allocated_value = malloc(sizeof(double));
                *allocated_value = VoidCastDouble(value);
                list->allocated_mem += sizeof(double);
                value = allocated_value;
            }
            break;
        case FLOAT:
            {
                float *allocated_value = malloc(sizeof(float));
                *allocated_value = VoidCastFloat(value);
                list->allocated_mem += sizeof(float);
                value = allocated_value;
            }
            break;
        case CHAR:
             {
                char key_data = VoidCastChar(value);
                const char *allocated_value = malloc(sizeof(char) + 1);

                list->allocated_mem += (int)(sizeof(char) + 1);
                memcpy((char*)allocated_value, &key_data, 1);
                *((char*)allocated_value + 1) = '\0';

                value = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *str_value = VoidCastStr(value);
                int str_length = (int)strlen(str_value);
                const char *allocated_value = malloc(sizeof(char) * (str_length + 1));

                list->allocated_mem += (int)(sizeof(char) * (str_length + 1));
                memcpy((char*)allocated_value, str_value, str_length);
                *((char*)allocated_value + str_length) = '\0';

                value = (char*)allocated_value;
            }
            break;
        default:
            break;
    }

    return value;
}

void FreeValue(Linked_List_T *list, Node_T **curr)
{
    list->allocated_mem -= DataTypeSize(list->T, (*curr)->data);
    free((*curr)->data);
}

Node_T *NewNode(Linked_List_T *list, void *data)
{
    Node_T *curr = malloc(sizeof(Node_T));
    curr->data = NewValue(list, data);
    curr->next = NULL;
    list->allocated_mem += sizeof(Node_T);
    return curr;
}

void FreeNode(Linked_List_T *list, Node_T **curr)
{
    FreeValue(list, curr);
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
    new_list.T = T;
    new_list.allocated_mem = 0;

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)data);

                new_list.head = NewNode(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array= ((double*)data);

                new_list.head = NewNode(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array= ((float*)data);

                new_list.head = NewNode(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);

                new_list.head = NewNode(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array = (char**)((char*)data);

                new_list.head = NewNode(&new_list, *data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = NewNode(&new_list, *data_array);
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
        *list = InitLinkedList(T, data, 1);
        return;
    }

    if (CheckWarnings(list, LINKED_LIST_TYPE, "LinkedListAppend", T))
    {
        return;
    }

    Node_T **tail = &(list->tail);
    Node_T *curr = NewNode(list, data);

    (*tail)->next = curr;
    *tail = (*tail)->next;
    list->size++;
}

void LinkedListInsert(Linked_List_T *list, int index, Type T, void *data)
{
    if (list->head == NULL && index == 0)
    {
        *list = InitLinkedList(T, data, 1);
        return;
    }

    if (CheckWarnings(list, LINKED_LIST_TYPE | LINKED_LIST_HEAD_NULL, "LinkedListInsert", T)
        || CheckWarnings(list, LINKED_LIST_INDEX_LGE, "LinkedListInsert", index))
    {
        return;
    }

    Node_T **head = &(list->head);

    if (index == 0)
    {
        Node_T *temp = NewNode(list, data);
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

    Node_T *temp = NewNode(list, data);
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

    if (CheckWarnings(list, LINKED_LIST_TYPE, "LinkedListExtend", T))
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
                    curr = NewNode(list, data_array);
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
                    curr = NewNode(list, data_array);
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
                    curr = NewNode(list, data_array);
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
                    curr = NewNode(list, data_array);
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
                    curr = NewNode(list, data_array);
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
    if (CheckWarnings(list, LINKED_LIST_HEAD_NULL | LINKED_LIST_INDEX_LGE, "LinkedListRemoveIndex", index))
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
    if (CheckWarnings(list, LINKED_LIST_TYPE | LINKED_LIST_HEAD_NULL, "LinkedListRemoveValue", T))
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
    if (CheckWarnings(list, LINKED_LIST_HEAD_NULL | LINKED_LIST_INDEX_LGE, "LinkedListGetValue", index))
    {
        return NULL;
    }

    Node_T *top = list->head;

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
    if (CheckWarnings(list, LINKED_LIST_TYPE | LINKED_LIST_HEAD_NULL, "LinkedListCheckValue", T))
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
    if (CheckWarnings(list, LINKED_LIST_HEAD_NULL, "LinkedListReverse", -1))
    {
        return;
    }
  
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
    if (CheckWarnings(list, LINKED_LIST_HEAD_NULL, "LinkedListSort", -1))
    {
        return;
    }

    SortDataValues(&(list->head), list->T);
    
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
    if (CheckWarnings(list, LINKED_LIST_HEAD_NULL, "LinkedListFree", -1))
    {
        return;
    }

    Node_T **head = &(list->head);
    Node_T *succeeding = (*head)->next;

    while (succeeding != NULL)
    {
        FreeNode(list, head);
        *head = succeeding;
        succeeding = succeeding->next;
    }

    FreeNode(list, head);
    list->tail = NULL;
    list->size = -1;
    list->T = -1;
}

void LinkedListPrint(Linked_List_T *list)
{
    if (CheckWarnings(list, LINKED_LIST_HEAD_NULL, "LinkedListPrint", -1))
    {
        return;
    }

    Node_T *top = list->head;

    while (top != NULL)
    {
        PrintT(list->T, top, "", "");
        printf("-->");
        top = top->next;
    }

    printf("NULL\n");
}