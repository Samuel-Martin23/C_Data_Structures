#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PRINT_PREORDER  1
#define PRINT_INORDER   2
#define PRINT_POSTORDER 4

#define RBT_CAPACITY    200

// Forward declarations.
typedef struct rbt rbt_t;

// Function pointers for rbt_t.
typedef bool (*rb_node_less)(void *value_1, void *value_2);
typedef bool (*rb_node_equal)(void *value_1, void *value_2);
typedef void (*rb_node_print)(void *value);
typedef void (*rb_node_free)(void *value);

rbt_t *rbt_init(rb_node_less less_nodes, rb_node_equal equal_nodes, rb_node_print print_node, rb_node_free free_node_value);
void rbt_insert(rbt_t *tree, void *value);
bool rbt_cotains_value(rbt_t *tree, void *value);
void rbt_print(rbt_t *tree, int print_order);
void rbt_free(rbt_t *tree);

#endif /* RBT_H */
