#include "queue.h"

typedef struct queue_node
{
    void *value;
    struct queue_node *next;
} queue_node_t;

typedef struct queue_list
{
    size_t size;
    queue_node_t *rear;
    queue_node_t *front;

    queue_node_t *iter;

    queue_node_print_value print_value;
    queue_node_free_value free_value;
} queue_list_t;


static queue_node_t *alloc_queue_node(void *value)
{
    queue_node_t *node = malloc(sizeof(queue_node_t));

    node->value = value;
    node->next = NULL;

    return node;
}

size_t queue_list_get_size(queue_list_t *ql) {return ql->size;}

queue_list_t *queue_list_init(queue_node_print_value print_value, queue_node_free_value free_value)
{
    queue_list_t *ql = malloc(sizeof(queue_list_t));

    memset(ql, 0, sizeof(*ql));

    ql->print_value = print_value;
    ql->free_value = free_value;

    return ql;
}

void queue_list_enqueue(queue_list_t *ql, void *value)
{
    if (ql->front == NULL)
    {
        ql->front = alloc_queue_node(value);
        ql->rear = ql->front;
        ql->iter = ql->front;
        ql->size++;
        return;
    }

    ql->rear->next = alloc_queue_node(value);
    ql->rear = ql->rear->next;
    ql->size++;
}

void *queue_list_get_front(queue_list_t *ql)
{
    if (ql->front == NULL)
    {
        return NULL;
    }

    return ql->front->value;
}

void queue_list_dequeue(queue_list_t *ql)
{
    if (ql->front == NULL)
    {
        return;
    }

    queue_node_t *node = ql->front->next;

    ql->free_value(ql->front->value);
    free(ql->front);

    ql->front = node;

    ql->size--;
}

bool queue_list_iterate(queue_list_t *ql, void **value)
{
    // The list is empty.
    if (ql->front == NULL)
    {
        return false;
    }
    
    // We reached the end.
    if (ql->iter == NULL)
    {
        (*value) = NULL;
        ql->iter = ql->front;
        return false;
    }

    (*value) = ql->iter->value;
    ql->iter = ql->iter->next;

    return true;
}

void queue_list_print(queue_list_t *ql)
{
    queue_node_t *node = ql->front;

    printf("{");

    while (node->next != NULL)
    {
        ql->print_value(node->value);
        node = node->next;

        printf(", ");
    }

    ql->print_value(node->value);
    printf("}\n");
}

void queue_list_free(queue_list_t *ql)
{
    queue_node_t *temp = NULL;

    while (ql->front != NULL)
    {
        temp = ql->front->next;
    
        ql->free_value(ql->front->value);
        free(ql->front);
    
        ql->front = temp;
    }

    free(ql);
}
