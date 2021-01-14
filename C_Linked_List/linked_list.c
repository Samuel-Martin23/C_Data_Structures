
#include "linked_list.h"

#define LINKED_LIST_TYPE            0x00000001
#define LINKED_LIST_HEAD_NULL       0x00000002
#define LINKED_LIST_INDEX_LGE       0x00000004

static bool check_warnings(linked_list_t *list, int warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & LINKED_LIST_TYPE)
    {
        template_t T = (template_t)check_value;

        if (list->T != T)
        {
            printf("%s: %swarning:%s linked list type does not equal the new value type%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & LINKED_LIST_HEAD_NULL)
    {
        if (list->head == NULL)
        {
            printf("%s: %swarning:%s linked list head is NULL%s\n", function_name, purple, white, reset);
            return true;
        }
    }

    if (warning_code & LINKED_LIST_INDEX_LGE)
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

void *new_value(linked_list_t *list, void *value)
{
    switch (list->T)
    {
        case INT:
            {
                int *allocated_value = malloc(sizeof(int));
                *allocated_value = void_cast_int(value);
                list->allocated_mem += (int)sizeof(int);
                value = allocated_value;
            }
            break;
        case DOUBLE:
            {
                double *allocated_value = malloc(sizeof(double));
                *allocated_value = void_cast_double(value);
                list->allocated_mem += (int)sizeof(double);
                value = allocated_value;
            }
            break;
        case FLOAT:
            {
                float *allocated_value = malloc(sizeof(float));
                *allocated_value = void_cast_float(value);
                list->allocated_mem += (int)sizeof(float);
                value = allocated_value;
            }
            break;
        case CHAR:
             {
                char data = void_cast_char(value);
                const char *allocated_value = malloc(sizeof(char) + 1);

                list->allocated_mem += (int)(sizeof(char) + 1);
                memcpy((char*)allocated_value, &data, 1);
                *((char*)allocated_value + 1) = '\0';

                value = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *data = void_cast_str(value);
                size_t str_length = strlen(data);
                const char *allocated_value = malloc(sizeof(char) * (str_length + 1));

                list->allocated_mem += (int)(sizeof(char) * (str_length + 1));
                memcpy((char*)allocated_value, data, str_length);
                *((char*)allocated_value + str_length) = '\0';

                value = (char*)allocated_value;
            }
            break;
    }

    return value;
}

void free_value(linked_list_t *list, node_t**curr)
{
    list->allocated_mem -= get_bytes(list->T, (*curr)->data);
    free((*curr)->data);
}

node_t *new_node(linked_list_t *list, void *data)
{
    node_t *curr = malloc(sizeof(node_t));
    curr->data = new_value(list, data);
    curr->next = NULL;
    list->allocated_mem += (int)sizeof(node_t);
    return curr;
}

void free_node(linked_list_t *list, node_t **curr)
{
    free_value(list, curr);
    free(*curr);
    *curr = NULL;
    list->allocated_mem -= (int)sizeof(node_t);
}

void print_allocated_mem_list(linked_list_t *list)
{
    printf("Bytes Allocated: %d\n", list->allocated_mem);
}

void print_list_size(linked_list_t *list)
{
    if (check_warnings(list, LINKED_LIST_HEAD_NULL, "PrintListSize", -1))
    {
        return;
    }

    printf("Linked List Size: %d\n", list->size);
}


linked_list_t linked_list_init(template_t T, void *data, int size)
{
    linked_list_t new_list;
    node_t *top = NULL;
    node_t *curr = NULL;

    new_list.size = size;
    new_list.T = T;
    new_list.allocated_mem = 0;

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
                float *data_array= ((float*)data);

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
                char **data_array = (char**)((char*)data);

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
    }

    new_list.tail = top;
    return new_list;
}

void linked_list_append(linked_list_t *list, template_t T, void *data)
{
    if (list->head == NULL)
    {
        *list = linked_list_init(T, data, 1);
        return;
    }

    if (check_warnings(list, LINKED_LIST_TYPE, "linked_list_append", (int)T))
    {
        return;
    }

    node_t **tail = &(list->tail);
    node_t *curr = new_node(list, data);

    (*tail)->next = curr;
    *tail = (*tail)->next;
    list->size++;
}

void linked_list_insert(linked_list_t *list, int index, template_t T, void *data)
{
    if (list->head == NULL && index == 0)
    {
        *list = linked_list_init(T, data, 1);
        return;
    }

    if (check_warnings(list, LINKED_LIST_TYPE | LINKED_LIST_HEAD_NULL, "linked_list_insert", (int)T)
        || check_warnings(list, LINKED_LIST_INDEX_LGE, "linked_list_insert", index))
    {
        return;
    }

    node_t **head = &(list->head);

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
        linked_list_append(list, T, data);
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

void linked_list_extend(linked_list_t *list, template_t T, void *data, int size)
{
    if (list->head == NULL)
    {
        *list = linked_list_init(T, data, size);
        return;
    }

    if (check_warnings(list, LINKED_LIST_TYPE, "linked_list_extend", (int)T))
    {
        return;
    }

    node_t *curr = NULL;
    node_t **tail = &(list->tail);

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
                double *data_array= ((double*)data);

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
                float *data_array= ((float*)data);

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
    }
}

void linked_list_remove_index(linked_list_t *list, int index)
{
    if (check_warnings(list, LINKED_LIST_HEAD_NULL | LINKED_LIST_INDEX_LGE, "linked_list_remove_index", index))
    {
        return;
    }

    node_t **head = &(list->head);

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

void linked_list_remove_value(linked_list_t *list, template_t T, void *value)
{
    if (check_warnings(list, LINKED_LIST_TYPE | LINKED_LIST_HEAD_NULL, "linked_list_remove_value", (int)T))
    {
        return;
    }

    node_t **head = &(list->head);

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

node_t *linked_list_get_value(linked_list_t *list, int index)
{
    if (check_warnings(list, LINKED_LIST_HEAD_NULL | LINKED_LIST_INDEX_LGE, "linked_list_get_value", index))
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

bool linked_list_check_value(linked_list_t *list, template_t T, void *data)
{
    if (check_warnings(list, LINKED_LIST_TYPE | LINKED_LIST_HEAD_NULL, "linked_list_check_value", (int)T))
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

void linked_list_reverse(linked_list_t *list)
{
    if (check_warnings(list, LINKED_LIST_HEAD_NULL, "linked_list_reverse", -1))
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

void linked_list_sort(linked_list_t *list)
{
    if (check_warnings(list, LINKED_LIST_HEAD_NULL, "linked_list_sort", -1))
    {
        return;
    }

    sort_data_values(&(list->head), list->T);
    
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

void linked_list_free(linked_list_t *list)
{
    if (check_warnings(list, LINKED_LIST_HEAD_NULL, "linked_list_free", -1))
    {
        return;
    }

    node_t **head = &(list->head);
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

void linked_list_print(linked_list_t *list)
{
    if (check_warnings(list, LINKED_LIST_HEAD_NULL, "linked_list_print", -1))
    {
        return;
    }

    node_t *top = list->head;

    while (top != NULL)
    {
        print_t(list->T, top->data, "", "");
        printf("-->");
        top = top->next;
    }

    printf("NULL\n");
}
