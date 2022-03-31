#include "rbt.h"
#include <stdarg.h>

#define SIZE    12

bool less_nodes_int(void *value_1, void *value_2) {return (*((int*)value_1) < *((int*)value_2));}
bool equal_nodes_int(void *value_1, void *value_2) {return (*((int*)value_1) == *((int*)value_2));}
void print_node_int(void *value) {printf("%d:\t", *((int*)value));}

void *alloc_int(int value)
{
    int *allocated_value = malloc(sizeof(int));
    *allocated_value = value;
    return allocated_value;
}

void add_to_tree_int(rbt_t *tree, size_t size, ...)
{
    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < size; i++)
    {
        rbt_insert(tree, alloc_int(va_arg(args, int)));
    }

    va_end(args);
}

bool rbt_contains_int(rbt_t *tree, int value) {return rbt_cotains_value(tree, &value);}

int main()
{
    rbt_t *tree = rbt_init(less_nodes_int, equal_nodes_int, print_node_int, free);

    add_to_tree_int(tree, SIZE, 7, 3, 8, 2, 4, 5, 1, 6, 10, 9, 11, 12);

    if (rbt_contains_int(tree, 5))
    {
        printf("Found value\n\n");
    }

    rbt_print(tree, PRINT_PREORDER);

    rbt_free(tree);

    return 0;
}
