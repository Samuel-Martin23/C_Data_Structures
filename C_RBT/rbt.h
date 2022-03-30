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
typedef struct rb_node rb_node_t;

// Function pointers for rbt_t.
typedef bool (*rb_node_compare)(rb_node_t *node_1, rb_node_t *node_2);
typedef bool (*rb_node_equal)(rb_node_t *node_1, rb_node_t *node_2);
typedef void (*rb_node_print)(rbt_t *tree, size_t index);

typedef struct rb_node
{
    void *value;
    bool is_red;
} rb_node_t;

typedef struct rbt
{
    size_t size;
    size_t capacity;
    rb_node_t *NIL;
    rb_node_t **data;

    rb_node_compare compare_nodes;
    rb_node_equal equal_nodes;
    rb_node_print print_node;
} rbt_t;

rbt_t *rbt_init(rb_node_compare compare_nodes, rb_node_equal equal_nodes, rb_node_print print_node);
void rbt_insert(rbt_t *tree, rb_node_t *node);
rb_node_t *rbt_search(rbt_t *tree, rb_node_t *node);
void rbt_print(rbt_t *tree, int print_order);
void rbt_free(rbt_t **tree);

#endif /* RBT_H */
