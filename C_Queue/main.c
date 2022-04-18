#include "queue.h"

void print_int(void *value) {printf("%d", *(int*)value);}

void queue_list_enqueue_int(queue_list_t *ql, int value)
{
    int *alloc_value = malloc(sizeof(int));
    *alloc_value = value;
    queue_list_enqueue(ql, alloc_value);
}

int queue_list_dequeue_int(queue_list_t *ql)
{
    int value = *(int*)queue_list_get_front(ql);

    queue_list_dequeue(ql);

    return value;
}

bool queue_list_iterate_int(queue_list_t *ql, int *value)
{
    void *temp = NULL;
    bool is_iterating = queue_list_iterate(ql, &temp);

    if (temp)
    {
        *value = *(int*)temp;
    }

    return is_iterating;
}

int main()
{
    queue_list_t *queue = queue_list_init(print_int, free);

    queue_list_enqueue_int(queue, 12);
    queue_list_enqueue_int(queue, 654);
    queue_list_enqueue_int(queue, 1500);
    queue_list_enqueue_int(queue, 17654);
    queue_list_enqueue_int(queue, 90080);

    queue_list_print(queue);

    for (int value = 0; queue_list_iterate_int(queue, &value);)
    {
        if (value < 1000)
        {
            queue_list_dequeue_int(queue);
        }
    }

    queue_list_print(queue);
    
    queue_list_free(queue);

    return 0;
}
