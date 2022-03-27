#ifndef RBT_H
#define RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#define PRINT_PREORDER  1
#define PRINT_INORDER   2
#define PRINT_POSTORDER 4

#define RBT_CAPACITY    50

typedef struct rb_node
{
    int value;
    bool is_red;
} rb_node_t;

typedef struct rbt
{
    size_t size;
    size_t capacity;
    rb_node_t *NIL;
    rb_node_t **data;
} rbt_t;

rbt_t *rbt_init(void);
rbt_t *rbt_init_args(size_t size, ...);
void rbt_insert(rbt_t *tree, int value);
void rbt_print(rbt_t *tree, int print_order);
void rbt_free(rbt_t **tree);

#endif /* RBT_H */
