#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include "../C_Template/template.h"
#include "../C_Allocation_Metrics/allocation_metrics.h"

#define BST_INORDER                 0x00000001u
#define BST_INORDER_INDEX           0x00000002u
#define BST_PREORDER                0x00000004u
#define BST_POSTORDER               0x00000008u

typedef struct node_bst
{
    void *value;
    struct node_bst *left;
    struct node_bst *right;
} node_bst_t;

typedef struct bst
{
    int size;
    int last_index;
    template_t T;
    node_bst_t *root;
} bst_t;

bst_t bst_init(template_t T, int size, ...);
void bst_insert(bst_t *tree, ...);
void bst_remove_value(bst_t *tree, ...);
node_bst_t *bst_lookup(bst_t *tree, ...);
node_bst_t *bst_lookup_with_parent(bst_t *tree, node_bst_t **parent, ...);
void *bst_get_index(bst_t *tree, int index);
void bst_copy(bst_t *dest_tree, bst_t *src_tree);
void bst_free(bst_t *tree);
void bst_print(bst_t *tree, unsigned short bst_print_options);

#endif /* BST_H */
