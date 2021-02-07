
#include "list.h"

#define LIST_TYPE               0x00000001
#define LIST_HEAD_NULL          0x00000002
#define LIST_INDEX_LGE          0x00000004
#define LIST_SIZE               0x00000008
#define TURN_OFF_WARNING        0x00000010

#define WARNING_PLACEHOLDER     -1

static bool check_warnings(list_t *list, int warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

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

    if (warning_code & LIST_INDEX_LGE)
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

    return false;
}

node_t *merge_lists(node_t *left, node_t *right, template_t T)
{
    if (left == NULL)
    {
        return right;
    }
    else if (right == NULL)
    {
        return left;
    }

    node_t *merged_head = NULL;

    if (check_less_equal_value(T, left->value, right->value))
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

void partition_list(node_t *head, node_t **front, node_t **back)
{
    node_t *slow = NULL;
    node_t *fast = NULL;

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

void sort_data_values(node_t **head, template_t T)
{
    node_t *top = *head;
    node_t *split_left = NULL;
    node_t *split_right = NULL;

    if (top == NULL || top->next == NULL)
    {
        return;
    }

    partition_list(top, &split_left, &split_right);

    sort_data_values(&split_left, T);
    sort_data_values(&split_right, T);

    *head = merge_lists(split_left, split_right, T);
}

void *new_value(list_t *list, void *value)
{
    switch (list->T)
    {
        case INT:
            {
                size_t number_of_bytes = get_bytes(list->T, value);
                int *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_int(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;;
            }
            break;
        case DOUBLE:
            {
                size_t number_of_bytes = get_bytes(list->T, value);
                double *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_double(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;;
            }
            break;
        case FLOAT:
            {
                size_t number_of_bytes = get_bytes(list->T, value);
                float *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_float(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;;
            }
            break;
        case CHAR:
             {
                size_t number_of_bytes = get_bytes(list->T, value);
                char cast_value = void_cast_char(value);
                const char *allocated_value = malloc(number_of_bytes);

                memcpy((char*)allocated_value, &cast_value, 1);
                value = (char*)allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;;
            }
            break;
        case STR:
            {
                const char *cast_value = void_cast_str(value);
                size_t number_of_bytes = get_bytes(list->T, (void*)cast_value);
                const char *allocated_value = malloc(number_of_bytes);

                mem_usage.allocated += (u_int32_t)number_of_bytes;;

                number_of_bytes--;
                memcpy((char*)allocated_value, cast_value, number_of_bytes);
                *((char*)allocated_value + number_of_bytes) = '\0';

                value = (char*)allocated_value;
            }
            break;
        case BOOL:
            {
                size_t number_of_bytes = get_bytes(list->T, value);
                bool *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_bool(value);
                value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;;
            }
            break;
        case NONE: // default:
            break;
    }

    return value;
}

void free_value(list_t *list, node_t **curr)
{
    mem_usage.freed += (u_int32_t)get_bytes(list->T, (*curr)->value);
    free((*curr)->value);
}

node_t *new_node(list_t *list, void *value)
{
    size_t number_of_bytes = sizeof(node_t);
    node_t *curr = malloc(number_of_bytes);

    curr->value = new_value(list, value);
    curr->next = NULL;
    mem_usage.allocated += (u_int32_t)number_of_bytes;
    return curr;
}

void free_node(list_t *list, node_t **curr)
{
    free_value(list, curr);
    free(*curr);
    *curr = NULL;
    mem_usage.freed += (u_int32_t)sizeof(node_t);
}

bool check_list_null_init(list_t *list, template_t T, void *data, int size)
{
    if (list->head == NULL)
    {
        *list = list_init(T, data, size);
        return true;
    }

    return false;
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

list_t list_init(template_t T, void *data, int size)
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

    node_t *top = NULL;
    node_t *curr = NULL;

    switch (T)
    {
        case INT:
        case DOUBLE:
        case FLOAT:
        case CHAR:
        case BOOL:
            new_list.head = new_node(&new_list, data);
            top = new_list.head;
            data += (int)get_bytes(new_list.T, data);

            for (int i = 1; i < new_list.size; i++)
            {
                curr = new_node(&new_list, data);
                top->next = curr;
                top = top->next;
                data += (int)get_bytes(new_list.T, data);
            }
            break;
        case STR:
            {
                char **str_array = ((char**)data);

                new_list.head = new_node(&new_list, *str_array);
                top = new_list.head;
                str_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = new_node(&new_list, *str_array);
                    top->next = curr;
                    top = top->next;
                    str_array++;
                }
            }
            break;
        case NONE: // default:
            break;
    }

    new_list.tail = top;
    return new_list;
}

void list_append(list_t *list, template_t T, void *value)
{
    if (check_list_null_init(list, T, value, 1))
    {
        return;
    }

    if (check_warnings(list, LIST_TYPE, "list_append", (int)T))
    {
        return;
    }

    convert_2d_str(list->T, &value);

    node_t **tail = &list->tail;
    node_t *curr = new_node(list, value);

    (*tail)->next = curr;
    *tail = (*tail)->next;
    list->size++;
}

void list_insert(list_t *list, template_t T, void *value, int index)
{
    if (index == 0)
    {
        if (check_list_null_init(list, T, value, 1))
        {
            return;
        }
    }

    if (check_warnings(list, LIST_TYPE | LIST_HEAD_NULL, "list_insert", (int)T)
        || check_warnings(list, LIST_INDEX_LGE, "list_insert", index))
    {
        return;
    }

    convert_2d_str(list->T, &value);

    node_t **head = &list->head;

    if (index == 0)
    {
        node_t *temp = new_node(list, value);
        temp->next = *head;
        *head = temp;
        list->size++;
        return;
    }
    else if (index == list->size-1)
    {
        list_append(list, T, value);
        return;
    }

    node_t *prev = NULL;
    node_t *curr = *head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    node_t *temp = new_node(list, value);
    prev->next = temp;
    temp->next = curr;
    list->size++;
}

void list_extend(list_t *list, template_t T, void *data, int size)
{
    if (check_list_null_init(list, T, data, size))
    {
        return;
    }

    if (check_warnings(list, LIST_TYPE, "list_extend", (int)T))
    {
        return;
    }

    node_t *curr = NULL;
    node_t **tail = &list->tail;

    switch (T)
    {
        case INT:
        case DOUBLE:
        case FLOAT:
        case CHAR:
        case BOOL:
            for (int i = 0; i < size; i++)
            {
                curr = new_node(list, data);
                (*tail)->next = curr;
                *tail = (*tail)->next;
                data += (int)get_bytes(list->T, data);
            }

            list->tail = curr;
            list->size += size;
            break;
        case STR:
            {
                char **str_array = ((char**)data);

                for (int i = 0; i < size; i++)
                {
                    curr = new_node(list, *str_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    str_array++;
                }

                list->tail = curr;
                list->size += size;
            }
            break;
        case NONE: // default:
            break;
    }
}

void list_remove_index(list_t *list, int index)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_INDEX_LGE,
        "list_remove_index", index))
    {
        return;
    }

    node_t **head = &list->head;

    if (index == 0) 
    {
        node_t* next = (*head)->next;
        free_node(list, head);
        *head = next;
        list->size--;
        return;
    }

    node_t *curr = *head;
    node_t *prev = NULL;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    prev->next = curr->next;
    free_node(list, &curr);
    list->size--;
}

void list_remove_value(list_t *list, template_t T, void *value)
{
    if (check_warnings(list, LIST_TYPE | LIST_HEAD_NULL,
        "list_remove_value", (int)T))
    {
        return;
    }

    convert_2d_str(list->T, &value);

    node_t **head = &list->head;

    if (check_equal_value(T, (*head)->value, value))
    {
        node_t *next = (*head)->next;
        free_node(list, head);
        *head = next;
        list->size--;
        return;
    }

    node_t *curr = *head;
    node_t *prev = NULL;

    while (curr != NULL)
    {
        if (check_equal_value(T, curr->value, value))
        {
            if (curr->next == NULL)
            {
                list->tail = prev;
            }

            prev->next = curr->next;
            free_node(list, &curr);
            list->size--;
            break;
        }

        prev = curr;
        curr = curr->next;
    }
}

void *list_get_value(list_t *list, int index)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_INDEX_LGE,
        "list_get_value", index))
    {
        return NULL;
    }

    node_t *top = list->head;

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

bool list_check_value(list_t *list, template_t T, void *value)
{
    if (check_warnings(list, LIST_TYPE | LIST_HEAD_NULL,
        "list_check_value", (int)T))
    {
        return false;
    }

    convert_2d_str(list->T, &value);

    node_t *top = list->head;

    while (top != NULL)
    {
        if (check_equal_value(T, top->value, value))
        {
            return true;
        }

        top = top->next;
    }

    return false;
}

void list_reverse(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_reverse", WARNING_PLACEHOLDER))
    {
        return;
    }
  
    node_t *curr = list->head;
    node_t *prev = NULL;
    node_t *next = NULL;

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
        node_t *tail = list->tail;

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

    node_t *top_dest = NULL;
    node_t *top_src = NULL;
    node_t *curr = NULL;

    switch (list_dest->T)
    {
        case INT:
        case DOUBLE:
        case FLOAT:
        case CHAR:
        case STR:
        case BOOL:
            top_src = list_src->head;
            list_dest->head = new_node(list_dest, top_src->value);
            top_dest = list_dest->head;
            top_src = top_src->next;

            for (int i = 1; i < list_dest->size; i++)
            {
                curr = new_node(list_dest, top_src->value);
                top_dest->next = curr;
                top_dest = top_dest->next;
                top_src = top_src->next;
            }
            break;
        case NONE: // default:
            break;
    }
}

void list_free(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_free", WARNING_PLACEHOLDER))
    {
        return;
    }

    node_t **head = &list->head;
    node_t *succeeding = (*head)->next;

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

    node_t *top = list->head;

    while (top != NULL)
    {
        print_t(list->T, top->value, "", "");
        printf(" ==> ");
        top = top->next;
    }

    printf("NULL\n");
}
