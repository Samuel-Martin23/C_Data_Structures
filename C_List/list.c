#include <stdio.h>
#include <stdlib.h>

typedef enum T {INT, DOUBLE, FLOAT, CHAR, STR} Type;

#define intA(...)    INT, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define doubleA(...) DOUBLE, (double[]){__VA_ARGS__}, sizeof((double[]){__VA_ARGS__})/sizeof((double[]){__VA_ARGS__}[0])
#define floatA(...)  FLOAT, (float[]){__VA_ARGS__}, sizeof((float[]){__VA_ARGS__})/sizeof((float[]){__VA_ARGS__}[0])
#define charA(...)   CHAR, (int[]){__VA_ARGS__}, sizeof((int[]){__VA_ARGS__})/sizeof((int[]){__VA_ARGS__}[0])
#define strA(...)    STR, (char*[]){__VA_ARGS__}, sizeof((char*[]){__VA_ARGS__})/sizeof((char*[]){__VA_ARGS__}[0])

#define int(x)      (int[]){x}
#define double(x)   (double[]){x}
#define float(x)    (float[]){x}
#define char(x)     int(x)
#define str(x)      (char*)x

#define int_cast(x)     (*(int*)x)
#define double_cast(x)  (*(double*)x)
#define float_cast(x)   (*(float*)x)
#define char_cast(x)    (*(char*)x)
#define str_cast(x)     ((char*)x)

typedef enum boolean {false, true} bool;
typedef struct Node
{
    void *data;
    struct Node *next;
    Type T;
    
} Node_T;

bool CheckValueType(Node_T *curr, void *value)
{
    switch (curr->T)
    {
        case INT:
            return (int_cast(curr->data) == int_cast(value));
        case DOUBLE:
            return (double_cast(curr->data) == double_cast(value));
        case FLOAT:
            return (float_cast(curr->data) == float_cast(value));
        case CHAR:
            return (char_cast(curr->data) == char_cast(value));
        case STR:
            return (str_cast(curr->data) == str_cast(value));
        default:
            return false;
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

Node_T *MergeLists(Node_T *left, Node_T *right)
{
    Node_T *merged_head = NULL;

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

void printT(Node_T *top)
{
    switch (top->T)
    {
        case INT:
            printf("%d", int_cast(top->data));
            break;
        case DOUBLE:
            printf("%.2f", double_cast(top->data));
            break;
        case FLOAT:
            printf("%.2f", float_cast(top->data));
            break;
        case CHAR:
            printf("%c", char_cast(top->data));
            break;
        case STR:
            printf("%s", str_cast(top->data));
            break;
        default:
            printf("ERROR\n");
            break;
    }
}

Node_T *tail = NULL;
int list_size = 0;
int allocated_mem = 0;
typedef Node_T* List_t;

void FreeMem(Node_T *curr)
{
    free(curr);

    if (allocated_mem <= 0)
    {
        return;
    }

    list_size--;
    allocated_mem -= sizeof(Node_T);
}

Node_T *NewMem()
{
    Node_T *temp = malloc(sizeof(Node_T));
    allocated_mem += sizeof(Node_T);
    return temp;
}

Node_T *NewNode(Type T, void *data)
{
    Node_T *temp = NewMem();
    temp->data = data;

    switch (T)
    {
        case INT:
            temp->T = INT;
            break;
        case DOUBLE:
            temp->T = DOUBLE;
            break;
        case FLOAT:
            temp->T = FLOAT;
            break;
        case CHAR:
            temp->T = CHAR;
            break;
        case STR:
            temp->T = STR;
            break;
    }

    temp->next = NULL;
    list_size++;
    return temp;
}

Node_T *InitList(Type T, void *data, int data_len)
{
    if (T == STR)
    {
        char **ptr_index = (char**)((char*)data);
        Node_T *head = NewNode(T, *ptr_index);
        Node_T *top = head;
        ptr_index++;

        for (int i = 1; i < data_len; i++)
        {
            Node_T *temp = NewNode(T, *ptr_index);
            top->next = temp;
            top = top->next;
            ptr_index++;
        }

        tail = top;
        return head;
    }
    else if (T == DOUBLE)
    {
        double *ptr_index = ((double*)data);
        Node_T *head = NewNode(T, ptr_index);
        Node_T *top = head;
        ptr_index++;

        for (int i = 1; i < data_len; i++)
        {
            Node_T *temp = NewNode(T, ptr_index);
            top->next = temp;
            top = top->next;
            ptr_index++;
        }

        tail = top;
        return head;
    }
    else
    {
        int *ptr_index = ((int*)data);
        Node_T *head = NewNode(T, ptr_index);
        Node_T *top = head;
        ptr_index++;

        for (int i = 1; i < data_len; i++)
        {
            Node_T *temp = NewNode(T, ptr_index);
            top->next = temp;
            top = top->next;
            ptr_index++;
        }

        tail = top;
        return head;
    }
}

void AppendList(void *data)
{
    if (tail == NULL)
    {
        return;
    }

    Node_T *temp = NewNode(tail->T, data);
    tail->next = temp;
    tail = tail->next;
}

void RemoveValueList(Node_T **head, void *value)
{
    if (CheckValueType(*head, value))
    {
        Node_T *next = (*head)->next;
        FreeMem(*head);
        *head = next;
        return;  
    }

    Node_T *curr = *head;
    Node_T *prev = NULL;

    while (curr != NULL)
    {
        if (CheckValueType(curr, value))
        {
            prev->next = curr->next;
            FreeMem(curr);
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

void ClearList(Node_T **head)
{
    Node_T *top = (*head)->next;

    while (top != NULL)
    {
        FreeMem(*head);
        *head = top;
        top = top->next;
    }

    FreeMem(*head);
    *head = NULL;
}

void ExtendList(Type T, void *data, int data_len)
{
    if (tail == NULL)
    {
        return;
    }

    if (T == STR)
    {
        char **ptr_index = (char**)((char*)data);

        for (int i = 0; i < data_len; i++)
        {
            Node_T *temp = NewNode(tail->T, *ptr_index);
            tail->next = temp;
            tail = tail->next;
            ptr_index++;
        }
    }
    else if (T == DOUBLE)
    {
        double *ptr_index = ((double*)data);

        for (int i = 0; i < data_len; i++)
        {
            Node_T *temp = NewNode(tail->T, ptr_index);
            tail->next = temp;
            tail = tail->next;
            ptr_index++;
        }
    }
    else
    {
        int *ptr_index = ((int*)data);

        for (int i = 0; i < data_len; i++)
        {
            Node_T *temp = NewNode(tail->T, ptr_index);
            tail->next = temp;
            tail = tail->next;
            ptr_index++;
        }
    }
}

void *GetIndexList(Node_T **head, int index)
{
    Node_T *top = *head;

    if (index < 0 || index > list_size)
    {
        return 0;
    }

    for (int i = 0; i <= index; i++)
    {
        if (i == index)
        {
            return top->data;
        }

        top = top->next;
    }

    return NULL;
}

bool CheckValueList(Node_T **head, void *value)
{
    Node_T *top = *head;

    for (int i = 0; i < list_size; i++)
    {
        if (CheckValueType(top, value))
        {
            return true;
        }

        top = top->next;
    }

    return false;
}

void InsertList(Node_T **head, int index, void *data)
{
    if (index < 0 || index > list_size)
    {
        return;
    }

    if (index == 0)
    {
        Node_T *temp = NewNode((*head)->T, data);
        temp->next = *head;
        *head = temp;
        return;
    }

    Node_T *prev = NULL;
    Node_T *curr = *head;

    for (int i = 0; i <= index; i++)
    {
        if (i == index)
        {
            Node_T *temp = NewNode(curr->T, data);
            prev->next = temp;
            temp->next = curr;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

void PopList(Node_T **head, int index)
{
    if (index < 0 || index > list_size)
    {
        return;
    }

    if (index == 0) 
    {
        Node_T* next = (*head)->next;
        FreeMem(*head);
        *head = next;
        return;
    }

    Node_T *curr = *head;
    Node_T *prev = NULL;

    for (int i = 0; i < list_size; i++)
    {
        if (i == index)
        {
            prev->next = curr->next;
            FreeMem(curr);
        }

        prev = curr;
        curr = curr->next;
    }
}

void ReverseList(Node_T **head)
{
    Node_T *curr = *head;
    Node_T *prev = NULL;
    Node_T *next = NULL;

    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    *head = prev;
}

void SortList(Node_T **head)
{
    Node_T *top = *head;
    Node_T *split_left = NULL;
    Node_T *split_right = NULL;

    if (top == NULL || top->next == NULL)
    {
        return;
    }

    PartitionList(top, &split_left, &split_right);

    SortList(&split_left);
    SortList(&split_right);

    *head = MergeLists(split_left, split_right);
}

void PrintList(Node_T **head)
{
    Node_T *top = *head;

    while (top != NULL)
    {
        printT(top);
        printf("-->");
        top = top->next;
    }

    printf("NULL\n");
}

void PrintAllocatedMemory()
{
    printf("%d Bytes Allocated\n", allocated_mem);
}