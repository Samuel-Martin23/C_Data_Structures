#include "rbt.h"
#include <stdarg.h>

#define SIZE    8

void print_node_int(rbt_t *tree, size_t index)
{
    int value = *((int*)tree->data[index]->value);
    printf("%d:", value);
}

bool compare_nodes_int(rb_node_t *inserting_node, rb_node_t *curr_node)
{
    int inserting_value = *((int*)inserting_node->value);
    int curr_value = *((int*)curr_node->value);

    return (inserting_value < curr_value);
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

void add_to_tree(rbt_t *tree, size_t size, ...)
{
    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < size; i++)
    {
        rbt_insert(tree, alloc_node_int(va_arg(args, int)));
    }

    va_end(args);
}

int main()
{
    rbt_t *tree = rbt_init(compare_nodes_int, print_node_int);

    add_to_tree(tree, SIZE, 7, 3, 8, 2, 4, 5, 1, 6);

    rbt_print(tree, PRINT_PREORDER);

    rbt_free(&tree);

    return 0;
}
