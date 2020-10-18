#include <stdio.h>
#include <stdlib.h>

#define intA(...)    INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define doubleA(...) DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define floatA(...)  FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define charA(...)   CHAR, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define strA(...)    STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

#define int(x)      INT, (int[]){x}
#define double(x)   (double[]){x}
#define float(x)    (float[]){x}
#define char(x)     int(x)
#define str(x)      (char*)x

#define int_cast(x)     (*(int*)x)
#define double_cast(x)  (*(double*)x)
#define float_cast(x)   (*(float*)x)
#define char_cast(x)    (*(char*)x)
#define str_cast(x)     ((char*)x)

int allocated_mem = 0;
typedef enum boolean {false, true} bool;
typedef enum T {INT, DOUBLE, FLOAT, CHAR, STR} Type;

typedef struct Data_Container
{
    void *data;
    struct Data_Container *next;
    Type T;
} Node;

typedef struct Linked_List
{
    Node *head;
    Node *tail;
    int size;
} List;

Node *tail = NULL;
int list_size = 0;

void PrintAllocatedMemory()
{
    printf("%d Bytes Allocated\n", allocated_mem);
}

void PrintListSize(List *data_set)
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

bool CheckNodeNull(Node *curr)
{
    if (curr == NULL)
    {
        return true;
    }

    return false;
}

bool CheckListNull(List *data_set)
{
    if (data_set == NULL)
    {
        return true;
    }

    return false;
}

void PrintT(Node *curr, const char *beginning, const char *end)
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

Node *MergeLists(Node *left, Node *right)
{
    Node *merged_head = NULL;

    if (left == NULL)
    {
        return right;
    }
    else if (right == NULL)
    {
        return left;
    }

    if (left->T == INT || right->T == INT)
    {
        if (int_cast(left->data) <= int_cast(right->data))
        {
            merged_head = left;
            merged_head->next = MergeLists(left->next, right);
        }
        else
        {
            merged_head = right;
            merged_head->next = MergeLists(left, right->next);
        }
    }
    else if (left->T == DOUBLE || right->T == DOUBLE)
    {
        if (double_cast(left->data) <= double_cast(right->data))
        {
            merged_head = left;
            merged_head->next = MergeLists(left->next, right);
        }
        else
        {
            merged_head = right;
            merged_head->next = MergeLists(left, right->next);
        }
    }
    else if (left->T == FLOAT || right->T == FLOAT)
    {
        if (float_cast(left->data) <= float_cast(right->data))
        {
            merged_head = left;
            merged_head->next = MergeLists(left->next, right);
        }
        else
        {
            merged_head = right;
            merged_head->next = MergeLists(left, right->next);
        }
    }
    else if (left->T == CHAR || right->T == CHAR)
    {
        if (char_cast(left->data) <= char_cast(right->data))
        {
            merged_head = left;
            merged_head->next = MergeLists(left->next, right);
        }
        else
        {
            merged_head = right;
            merged_head->next = MergeLists(left, right->next);
        }
    }
    else if (left->T == STR || right->T == STR)
    {
        if (StrLen(str_cast(left->data)) <= StrLen(str_cast(right->data)))
        {
            merged_head = left;
            merged_head->next = MergeLists(left->next, right);
        }
        else
        {
            merged_head = right;
            merged_head->next = MergeLists(left, right->next);
        }
    }

    return merged_head;
}

void PartitionList(Node *head, Node **front, Node **back)
{
    Node *slow = NULL;
    Node *fast = NULL;

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

Node *NewNode(Type T, void *data)
{
    Node *curr = malloc(sizeof(Node));
    curr->data = data;
    curr->next = NULL;
    curr->T = T;
    allocated_mem += sizeof(Node);
    return curr;
}

void FreeNode(Node **curr)
{
    free(*curr);
    *curr = NULL;

    if (allocated_mem <= 0)
    {
        return;
    }

    allocated_mem -= sizeof(Node);
}

List *NewLinkedList(Type T, void *data)
{
    List *curr = malloc(sizeof(List));
    curr->head = NewNode(T, data);
    curr->size++;
    allocated_mem += sizeof(List);
    return curr;
}

void ClearList(List **data_set);
void FreeLinkedList(List **curr)
{
    ClearList(curr);
    free(*curr);
    *curr = NULL;

    if (allocated_mem <= 0)
    {
        return;
    }

    allocated_mem -= sizeof(List);
}

List *InitList(Type T, void *data, int data_len)
{
    List *data_set = NULL;

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)data);
                data_set = NewLinkedList(T, data_array);
                Node *top = data_set->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    Node *curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                    data_set->size++;
                }

                data_set->tail = top;
            }
            break;
        case DOUBLE:
            {
                double *data_array= ((double*)data);
                data_set = NewLinkedList(T, data_array);
                Node *top = data_set->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    Node *curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                    data_set->size++;
                }

                data_set->tail = top;
            }
            break;
        case FLOAT:
            {
                float *data_array= ((float*)data);
                data_set = NewLinkedList(T, data_array);
                Node *top = data_set->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    Node *curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                    data_set->size++;
                }

                data_set->tail = top;
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);
                data_set = NewLinkedList(T, data_array);
                Node *top = data_set->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    Node *curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                    data_set->size++;
                }

                data_set->tail = top;
            }
            break;
        case STR:
            {
                char **data_array = (char**)((char*)data);
                data_set = NewLinkedList(T, data_array);
                Node *top = data_set->head;
                data_array++;

                for (int i = 1; i < data_len; i++)
                {
                    Node *curr = NewNode(T, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                    data_set->size++;
                }

                data_set->tail = top;
            }
            break;
        default:
            break;
    }

    return data_set;
}

void AppendList(List **data_set, Type T, void *data)
{
    if ( !(CheckListNull(*data_set)) && CheckNodeNull((*data_set)->head))
    {
        (*data_set)->head = NewNode(T, data);
        (*data_set)->tail = (*data_set)->head;
        return;
    }
    else if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->tail)))
    {
        return;
    }

    Node **head = &((*data_set)->head);
    Node **tail = &((*data_set)->tail);

    Node *curr = NewNode((*tail)->T, data);
    (*tail)->next = curr;
    *tail = (*tail)->next;
    (*data_set)->size++;
}

void RemoveValueList(List **data_set, Type T, void *value)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->tail)))
    {
        return;
    }

    Node **head = &((*data_set)->head);

    if (CheckEqualValue(T, (*head)->data, value))
    {
        Node *next = (*head)->next;
        FreeNode(head);
        *head = next;
        (*data_set)->size--;
        return;
    }

    Node *curr = *head;
    Node *prev = NULL;

    while (!(CheckNodeNull(*head)))
    {
        if (CheckEqualValue(T, curr->data, value))
        {
            prev->next = curr->next;
            FreeNode(&curr);
            (*data_set)->size--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

void ClearList(List **data_set)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->tail)))
    {
        return;
    }

    Node **head = &((*data_set)->head);
    Node *next = (*head)->next;

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

void ExtendList(List **data_set, Type T, void *data, int data_len)
{
    if ( !(CheckListNull(*data_set)) && CheckNodeNull((*data_set)->head))
    {
        FreeLinkedList(data_set);
        *data_set = InitList(T, data, data_len);
        return;
    }
    else if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->tail)))
    {
        return;
    }

    Node *curr = NULL;
    Node **tail = &((*data_set)->tail);

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
}

Node *GetValueFromIndex(List **data_set, int index)
{
    Node *top = NULL;
    
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

bool CheckValueList(List **data_set, Type T, void *data)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->head)))
    {
        return false;
    }

    Node *top = ((*data_set)->head);

    for (int i = 0; i < (*data_set)->size; i++)
    {
        if (CheckEqualValue(T, top->data, data))
        {
            return true;
        }

        top = top->next;
    }

    return false;
}

void InsertList(List **data_set, int index, Type T, void *data)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->head)))
    {
        return;
    }

    if (index < 0 || index >= (*data_set)->size)
    {
        return;
    }

    Node **head = &((*data_set)->head);

    if (index == 0)
    {
        Node *temp = NewNode((*head)->T, data);
        temp->next = *head;
        *head = temp;
        (*data_set)->size++;
        return;
    }
    else if (index == (*data_set)->size-1)
    {
        AppendList(data_set, T, data);
        return;
    }

    Node *prev = NULL;
    Node *curr = *head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    Node *temp = NewNode(curr->T, data);
    prev->next = temp;
    temp->next = curr;
    (*data_set)->size++;
}

void PopListIndex(List **data_set, int index)
{
    if ((CheckListNull(*data_set)) || (CheckNodeNull((*data_set)->head)))
    {
        return;
    }

    if (index < 0 || index >= (*data_set)->size)
    {
        return;
    }

    Node **head = &((*data_set)->head);

    if (index == 0) 
    {
        Node* next = (*head)->next;
        FreeNode(head);
        *head = next;
        (*data_set)->size--;
        return;
    }

    Node *curr = *head;
    Node *prev = NULL;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    prev->next = curr->next;
    FreeNode(&curr);
    (*data_set)->size--;
}

void ReverseList(List **data_set)
{
    Node *curr = (*data_set)->head;
    Node *prev = NULL;
    Node *next = NULL;

    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    (*data_set)->head = prev;
}

void SortData(Node **head)
{
    Node *top = *head;
    Node *split_left = NULL;
    Node *split_right = NULL;

    if (top == NULL || top->next == NULL)
    {
        return;
    }

    PartitionList(top, &split_left, &split_right);

    SortData(&split_left);
    SortData(&split_right);

    *head = MergeLists(split_left, split_right);
}

void SortList(List **data_set)
{
    SortData(&((*data_set)->head));
}

void PrintList(List *data_set)
{
    if (data_set == NULL || data_set->head == NULL)
    {
        return;
    }

    Node *top = data_set->head;

    while (top != NULL)
    {
        PrintT(top, "", "");
        printf("-->");
        top = top->next;
    }

    printf("NULL\n");
}