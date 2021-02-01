
#include "list.h"

#define LIST_TYPE            0x00000001
#define LIST_HEAD_NULL       0x00000002
#define LIST_INDEX_LGE       0x00000004

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
            printf("%s: %swarning:%s list type does not equal the new value type%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & LIST_HEAD_NULL)
    {
        if (list->head == NULL)
        {
            printf("%s: %swarning:%s list head is NULL%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & LIST_INDEX_LGE)
    {
        int index = check_value;

        if (index < 0 || index >= list->size)
        {
            printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
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

    if (check_less_equal_value(T, left->data, right->data))
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
    size_t number_of_bytes = 0;

    switch (list->T)
    {
        case INT:
            {
                number_of_bytes = sizeof(int);
                int *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_int(value);
                list->allocated_mem += (int)number_of_bytes;
                value = allocated_value;
            }
            break;
        case DOUBLE:
            {
                number_of_bytes = sizeof(double);
                double *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_double(value);
                list->allocated_mem += (int)number_of_bytes;
                value = allocated_value;
            }
            break;
        case FLOAT:
            {
                number_of_bytes = sizeof(float);
                float *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_float(value);
                list->allocated_mem += (int)number_of_bytes;
                value = allocated_value;
            }
            break;
        case CHAR:
             {
                number_of_bytes = sizeof(char) + 1;
                char data = void_cast_char(value);
                const char *allocated_value = malloc(number_of_bytes);

                list->allocated_mem += (int)(number_of_bytes);
                memcpy((char*)allocated_value, &data, 1);
                *((char*)allocated_value + 1) = '\0';

                value = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *data = void_cast_str(value);
                size_t str_length = strlen(data);
                number_of_bytes = sizeof(char) * (str_length + 1);
                const char *allocated_value = malloc(number_of_bytes);

                list->allocated_mem += (int)(number_of_bytes);
                memcpy((char*)allocated_value, data, str_length);
                *((char*)allocated_value + str_length) = '\0';

                value = (char*)allocated_value;
            }
            break;
        case BOOL:
            {
                number_of_bytes = sizeof(bool);
                bool *allocated_value = malloc(number_of_bytes);
                *allocated_value = void_cast_bool(value);
                list->allocated_mem += (int)(number_of_bytes);
                value = allocated_value;
            }
            break;
    }

    return value;
}

void free_value(list_t *list, node_t **curr)
{
    list->allocated_mem -= get_bytes(list->T, (*curr)->data);
    free((*curr)->data);
}

node_t *new_node(list_t *list, void *data)
{
    size_t number_of_bytes = sizeof(node_t);
    node_t *curr = malloc(number_of_bytes);
    curr->data = new_value(list, data);
    curr->next = NULL;
    list->allocated_mem += (int)number_of_bytes;
    return curr;
}

void free_node(list_t *list, node_t **curr)
{
    free_value(list, curr);
    free(*curr);
    *curr = NULL;
    list->allocated_mem -= (int)sizeof(node_t);
}

void print_allocated_mem_list(list_t *list)
{
    printf("Bytes Allocated: %d\n", list->allocated_mem);
}

void print_list_size(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "print_list_size", -1))
    {
        return;
    }

    printf("List Size: %d\n", list->size);
}

list_t list_init(template_t T, void *data, int size)
{
    list_t new_list;
    node_t *top = NULL;
    node_t *curr = NULL;

    new_list.size = size;
    new_list.T = T;
    new_list.allocated_mem = 0;
    new_list.head = NULL;
    new_list.tail = NULL;

    if (new_list.size == 0)
    {
        return new_list;
    }

    switch (T)
    {
        case INT:
            {
                int *data_array = ((int*)data);

                new_list.head = new_node(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = new_node(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)data);

                new_list.head = new_node(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = new_node(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)data);

                new_list.head = new_node(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = new_node(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);

                new_list.head = new_node(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = new_node(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
        case STR:
            {
                char **data_array = ((char**)data);

                new_list.head = new_node(&new_list, *data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = new_node(&new_list, *data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
         case BOOL:
            {
                bool *data_array = ((bool*)data);

                new_list.head = new_node(&new_list, data_array);
                top = new_list.head;
                data_array++;

                for (int i = 1; i < new_list.size; i++)
                {
                    curr = new_node(&new_list, data_array);
                    top->next = curr;
                    top = top->next;
                    data_array++;
                }
            }
            break;
    }

    new_list.tail = top;
    return new_list;
}

void list_append(list_t *list, template_t T, void *data)
{
    if (list->head == NULL)
    {
        *list = list_init(T, data, 1);
        return;
    }

    if (check_warnings(list, LIST_TYPE, "list_append", (int)T))
    {
        return;
    }

    node_t **tail = &list->tail;
    node_t *curr = new_node(list, data);

    (*tail)->next = curr;
    *tail = (*tail)->next;
    list->size++;
}

void list_insert(list_t *list, template_t T, void *data, int index)
{
    if (list->head == NULL && index == 0)
    {
        *list = list_init(T, data, 1);
        return;
    }

    if (check_warnings(list, LIST_TYPE | LIST_HEAD_NULL, "list_insert", (int)T)
        || check_warnings(list, LIST_INDEX_LGE, "list_insert", index))
    {
        return;
    }

    node_t **head = &list->head;

    if (index == 0)
    {
        node_t *temp = new_node(list, data);
        temp->next = *head;
        *head = temp;
        list->size++;
        return;
    }
    else if (index == list->size-1)
    {
        list_append(list, T, data);
        return;
    }

    node_t *prev = NULL;
    node_t *curr = *head;

    for (int i = 0; i < index; i++)
    {
        prev = curr;
        curr = curr->next;
    }

    node_t *temp = new_node(list, data);
    prev->next = temp;
    temp->next = curr;
    list->size++;
}

void list_extend(list_t *list, template_t T, void *data, int size)
{
    if (list->head == NULL)
    {
        *list = list_init(T, data, size);
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
            {
                int *data_array = ((int*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = new_node(list, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
                list->size += size;
            }
            break;
        case DOUBLE:
            {
                double *data_array = ((double*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = new_node(list, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
                list->size += size;
            }
            break;
        case FLOAT:
            {
                float *data_array = ((float*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = new_node(list, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
                list->size += size;
            }
            break;
        case CHAR:
            {
                char *data_array = ((char*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = new_node(list, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
                list->size += size;
            }
            break;
        case STR:
            {
                char **data_array = (char**)((char*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = new_node(list, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
                list->size += size;
            }
            break;
        case BOOL:
            {
                bool *data_array = ((bool*)data);

                for (int i = 0; i < size; i++)
                {
                    curr = new_node(list, data_array);
                    (*tail)->next = curr;
                    *tail = (*tail)->next;
                    data_array++;
                }

                list->tail = curr;
                list->size += size;
            }
            break;
    }
}

void list_remove_index(list_t *list, int index)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_INDEX_LGE, "list_remove_index", index))
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
    if (check_warnings(list, LIST_TYPE | LIST_HEAD_NULL, "list_remove_value", (int)T))
    {
        return;
    }

    node_t **head = &list->head;

    if (check_equal_value(T, (*head)->data, value))
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
        if (check_equal_value(T, curr->data, value))
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

node_t *list_get_value(list_t *list, int index)
{
    if (check_warnings(list, LIST_HEAD_NULL | LIST_INDEX_LGE, "list_get_value", index))
    {
        return NULL;
    }

    node_t *top = list->head;

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

bool list_check_value(list_t *list, template_t T, void *data)
{
    if (check_warnings(list, LIST_TYPE | LIST_HEAD_NULL, "list_check_value", (int)T))
    {
        return false;
    }

    node_t *top = list->head;

    while (top != NULL)
    {
        if (check_equal_value(T, top->data, data))
        {
            return true;
        }

        top = top->next;
    }

    return false;
}

void list_reverse(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_reverse", -1))
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
    if (check_warnings(list, LIST_HEAD_NULL, "list_sort", -1))
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

void list_free(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_free", -1))
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
    list->size = -1;
}

void list_print(list_t *list)
{
    if (check_warnings(list, LIST_HEAD_NULL, "list_print", -1))
    {
        return;
    }

    node_t *top = list->head;

    while (top != NULL)
    {
        print_t(list->T, top->data, "", "");
        printf(" ==> ");
        top = top->next;
    }

    printf("NULL\n");
}
