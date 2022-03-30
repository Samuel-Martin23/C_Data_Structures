#include "rbt.h"
#include <stdarg.h>

#define SIZE    12

void print_node_int(rbt_t *tree, size_t index)
{
    printf("%d:\t", *((int*)tree->data[index]->value));
}

bool compare_nodes_int(rb_node_t *node_1, rb_node_t *node_2)
{
    return (*((int*)node_1->value) < *((int*)node_2->value));
}

bool equal_nodes_int(rb_node_t *node_1, rb_node_t *node_2)
{
    return (*((int*)node_1->value) == *((int*)node_2->value));
}

rb_node_t *alloc_node_int(int value)
{
    rb_node_t *node = malloc(sizeof(rb_node_t));
    int *node_value = malloc(sizeof(int));

    *node_value = value;
    
    node->value = node_value;
    node->is_red = true;

    return node;
}

void add_to_tree_int(rbt_t *tree, size_t size, ...)
{
    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < size; i++)
    {
        rbt_insert(tree, alloc_node_int(va_arg(args, int)));
    }

    va_end(args);
}

bool is_value_in_tree_int(rbt_t *tree, int value)
{
    rb_node_t temp_node = {&value, false};
    return (rbt_search(tree, &temp_node) != tree->NIL);
}

int main()
{
    rbt_t *tree = rbt_init(compare_nodes_int, equal_nodes_int, print_node_int);
    int search_value = 6;

    add_to_tree_int(tree, SIZE, 7, 3, 8, 2, 4, 5, 1, 6, 10, 9, 11, 12);

    if (is_value_in_tree_int(tree, search_value))
    {
        printf("The value %d is in our tree.\n\n", search_value);
    }

    rbt_print(tree, PRINT_PREORDER);

    rbt_free(&tree);

    return 0;
}
