#include "list.h"

#define LIST_TYPE_NONE          0x00000001u
#define LIST_HEAD_NULL          0x00000002u
#define LIST_INDEX_GE           0x00000004u
#define LIST_SIZE               0x00000008u
#define LIST_INDEX_G            0x00000010u
#define LIST_TYPE               0x00000020u
#define TURN_OFF_WARNING        0x00000040u

#define WARNING_PLACEHOLDER     -1

static bool check_warnings(list_t *list, u_int16_t warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & LIST_TYPE_NONE)
    {
        if (list->T == NONE)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s list type equals none%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & LIST_HEAD_NULL)
    {
        if (list->head == NULL)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s list head is NULL%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & LIST_INDEX_GE)
    {
        int index = check_value;

        if (index < 0 || index >= list->size)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & LIST_SIZE)
    {
        if (list->size)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s list size is not zero%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & LIST_INDEX_G)
    {
        int index = check_value;

        if (index < 0 || index > list->size)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & LIST_TYPE)
    {
        template_t T = (template_t)check_value;

        if (list->T != T)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s list type does not equal the new data type%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    return false;
}

static node_list_t *merge_lists(node_list_t *left, node_list_t *right, template_t T)
{
    if (left == NULL)
    {
        return right;
    }
    else if (right == NULL)
    {
        return left;
    }

    node_list_t *merged_head = NULL;

    if (check_less_equal_value(T, left->value, right->value, false))
    {
        merged_head = left;
        merged_head->next = merge_lists(left->next, right, T);
    }
    else
    {
        merged_head = right;
        merged_head->next = merge_lists(left, right->next, T);
    }

    return merged_head;
}

static void partition_list(node_list_t *head, node_list_t **front, node_list_t **back)
{
    node_list_t *slow = NULL;
    node_list_t *fast = NULL;

    if (head == NULL || head->next == NULL)
    {
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

static void sort_data_values(node_list_t **head, template_t T)
{
    node_list_t *top = *head;
    node_list_t *split_left = NULL;
    node_list_t *split_right = NULL;

    if (top == NULL || top->next == NULL)
    {
        return;
    }

    partition_list(top, &split_left, &split_right);

    sort_data_values(&split_left, T);
    sort_data_values(&split_right, T);

    *head = merge_lists(split_left, split_right, T);
}

static node_list_t *new_node(void *value)
{
    size_t number_of_bytes = sizeof(node_list_t);
    node_list_t *curr = malloc(number_of_bytes);

    curr->value = value;
    curr->next = NULL;
    mem_usage.allocated += (u_int32_t)number_of_bytes;
    return curr;
}

static void free_node(list_t *list, node_list_t **curr)
{
    free_T_value(list->T, (*curr)->value);
    free(*curr);
    *curr = NULL;
    mem_usage.freed += (u_int32_t)sizeof(node_list_t);
}

static void append(list_t *list, node_list_t *curr)
{
    node_list_t **tail = &list->tail;

    (*tail)->next = curr;
    *tail = (*tail)->next;
    list->size++;
}

void print_list_size(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL,
        "print_list_size", WARNING_PLACEHOLDER))
    {
        return;
    }

    printf("List Size: %d\n", list->size);
}

list_t list_init(template_t T, int size, ...)
{
    list_t new_list;
    new_list.size = size;
    new_list.T = T;
    new_list.head = NULL;
    new_list.tail = NULL;

    if (new_list.size == 0)
    {
        return new_list;
    }

    node_list_t *top = NULL;
    node_list_t *curr = NULL;

    va_list args;
    va_start(args, size);

    new_list.head = new_node(new_arg_T_value(new_list.T, args));
    top = new_list.head;

    for (int i = 1; i < new_list.size; i++)
    {
        curr = new_node(new_arg_T_value(new_list.T, args));
        top->next = curr;
        top = top->next;
    }

    va_end(args);

    new_list.tail = top;
    return new_list;
}

void list_append(list_t *list, ...)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_TYPE_NONE,
        "list_append", WARNING_PLACEHOLDER))
    {
        return;
    }

    va_list args;
    va_start(args, list);

    node_list_t *curr = new_node(new_arg_T_value(list->T, args));

    va_end(args);

    append(list, curr);
}

void list_insert(list_t *list, int index, ...)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_TYPE_NONE | LIST_INDEX_G,
        "list_insert", index))
    {
        return;
    }

    node_list_t **head = &list->head;

    va_list args;
    va_start(args, index);

    if (index == 0)
    {
        node_list_t *temp = new_node(new_arg_T_value(list->T, args));
        temp->next = *head;
        *head = temp;
        list->size++;
        va_end(args);
        return;
    }
    else if (index == list->size)
    {
        node_list_t *temp = new_node(new_arg_T_value(list->T, args));
        append(list, temp);
        va_end(args);
        return;
    }

    node_list_t *prev = NULL;
    node_list_t *curr = *head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    node_list_t *temp = new_node(new_arg_T_value(list->T, args));
    prev->next = temp;
    temp->next = curr;
    list->size++;
    va_end(args);
}

void list_extend(list_t *list, int size, ...)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_TYPE_NONE,
        "list_extend", WARNING_PLACEHOLDER))
    {
        return;
    }

    node_list_t *curr = NULL;
    node_list_t **tail = &list->tail;

    va_list args;
    va_start(args, size);

    for (int i = 0; i < size; i++)
    {
        curr = new_node(new_arg_T_value(list->T, args));
        (*tail)->next = curr;
        *tail = (*tail)->next;
    }

    va_end(args);

    list->tail = curr;
    list->size += size;
}

void list_remove_index(list_t *list, int index)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_INDEX_GE,
        "list_remove_index", index))
    {
        return;
    }

    node_list_t **head = &list->head;

    if (index == 0) 
    {
        node_list_t* next = (*head)->next;
        free_node(list, head);
        *head = next;
        list->size--;
        return;
    }

    node_list_t *curr = *head;
    node_list_t *prev = NULL;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    prev->next = curr->next;
    free_node(list, &curr);
    list->size--;
}

void list_remove_value(list_t *list, ...)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_TYPE_NONE,
        "list_remove_value", WARNING_PLACEHOLDER))
    {
        return;
    }

    node_list_t **head = &list->head;

    va_list args;
    va_start(args, list);

    void *value = new_arg_T_value(list->T, args);

    va_end(args);

    if (check_equal_value(list->T, (*head)->value, value, false))
    {
        node_list_t *next = (*head)->next;
        free_node(list, head);
        *head = next;
        list->size--;
        free_T_value(list->T, value);
        return;
    }

    node_list_t *curr = *head;
    node_list_t *prev = NULL;

    while (curr != NULL)
    {
        if (check_equal_value(list->T, curr->value, value, false))
        {
            if (curr->next == NULL)
            {
                list->tail = prev;
            }

            prev->next = curr->next;
            free_node(list, &curr);
            list->size--;
            free_T_value(list->T, value);
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

void *list_get_value(list_t *list, int index)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_INDEX_GE,
        "list_get_value", index))
    {
        return NULL;
    }

    node_list_t *top = list->head;

    for (int i = 0; i < index; i++)
    {
        if (top == NULL)
        {
            return NULL;
        }

        top = top->next;
    }

    return top->value;
}

bool list_check_value(list_t *list, ...)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_TYPE_NONE,
        "list_check_value", WARNING_PLACEHOLDER))
    {
        return false;
    }

    node_list_t *top = list->head;

    va_list args;
    va_start(args, list);

    void *key = new_arg_T_value(list->T, args);

    va_end(args);

    while (top != NULL)
    {
        if (check_equal_value(list->T, top->value, key, false))
        {
            free_T_value(list->T, key);
            return true;
        }

        top = top->next;
    }

    free_T_value(list->T, key);
    return false;
}

void list_reverse(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_reverse", WARNING_PLACEHOLDER))
    {
        return;
    }
  
    node_list_t *curr = list->head;
    node_list_t *prev = NULL;
    node_list_t *next = NULL;

    while (curr != NULL)
    {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    list->head = prev;
}

void list_sort(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_sort", WARNING_PLACEHOLDER))
    {
        return;
    }

    sort_data_values(&list->head, list->T);
    
    if (list->tail != NULL)
    {
        node_list_t *tail = list->tail;

        while (tail->next != NULL)
        {
            tail = tail->next;
        }

        list->tail = tail;
    }
}

void list_copy(list_t *list_dest, list_t *list_src)
{
    if (check_warnings(list_src, LIST_HEAD_NULL | LIST_TYPE, 
        "list_copy", (int)list_dest->T) || 
        check_warnings(list_dest, LIST_SIZE, "list_copy", WARNING_PLACEHOLDER))
    {
        return;
    }

    list_dest->head = NULL;
    list_dest->tail = NULL;
    list_dest->size = list_src->size;
    list_dest->T = list_src->T;

    if (list_dest->size == 0)
    {
        return;
    }

    node_list_t *top_dest = NULL;
    node_list_t *top_src = NULL;
    node_list_t *curr = NULL;

    top_src = list_src->head;
    list_dest->head = new_node(new_T_value(list_dest->T, top_src->value));
    top_dest = list_dest->head;
    top_src = top_src->next;

    for (int i = 1; i < list_dest->size; i++)
    {
        curr = new_node(new_T_value(list_dest->T, top_src->value));
        top_dest->next = curr;
        top_dest = top_dest->next;
        top_src = top_src->next;
    }
}

void list_free(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_free", WARNING_PLACEHOLDER))
    {
        return;
    }

    node_list_t **head = &list->head;
    node_list_t *succeeding = (*head)->next;

    while (succeeding != NULL)
    {
        free_node(list, head);
        *head = succeeding;
        succeeding = succeeding->next;
    }

    free_node(list, head);
    list->tail = NULL;
    list->size = 0;
    list->T = NONE;
}

void list_print(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_print", WARNING_PLACEHOLDER))
    {
        return;
    }

    node_list_t *top = list->head;

    while (top != NULL)
    {
        print_t(list->T, top->value, "", "");
        printf(" ==> ");
        top = top->next;
    }

    printf("NULL\n");
}
