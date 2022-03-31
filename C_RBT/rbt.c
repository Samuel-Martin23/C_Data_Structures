#include "rbt.h"

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

    rb_node_less less_nodes;
    rb_node_equal equal_nodes;
    rb_node_print print_node;
    rb_node_free free_node_value;
} rbt_t;

static size_t parent(size_t index)
{
    if (index == 0)
    {
        return 0;
    }

    return (index - 1) / 2;
}

static size_t left_child(size_t index)
{
    return (2 * index) + 1;
}

static size_t right_child(size_t index)
{
    return (2 * index) + 2;
}

static bool is_node_red(rbt_t *tree, size_t index)
{
    return tree->data[index]->is_red;
}

static const char *get_node_color(rbt_t *tree, size_t index)
{
    return is_node_red(tree, index) ? "red" : "black";
}

static void pre_order(rbt_t *tree, size_t index)
{
    if (tree->data[index] == tree->NIL)
    {
        return;
    }

    tree->print_node(tree->data[index]->value);
    printf("%s\n", get_node_color(tree, index));

    pre_order(tree, left_child(index));
    pre_order(tree, right_child(index));
}

static void in_order(rbt_t *tree, size_t index)
{
    if (tree->data[index] == tree->NIL)
    {
        return;
    }

    in_order(tree, left_child(index));

    tree->print_node(tree->data[index]->value);
    printf("%s\n", get_node_color(tree, index));

    in_order(tree, right_child(index));
}

static void post_order(rbt_t *tree, size_t index)
{
    if (tree->data[index] == tree->NIL)
    {
        return;
    }

    post_order(tree, left_child(index));
    post_order(tree, right_child(index));

    tree->print_node(tree->data[index]->value);
    printf("%s\n", get_node_color(tree, index));
}

static void C_and_B_shift(rbt_t *tree, size_t old_index, size_t new_index)
{
    if (tree->data[old_index] == tree->NIL)
    {
        return;
    }

    tree->data[new_index] = tree->data[old_index];
    tree->data[old_index] = tree->NIL;

    C_and_B_shift(tree, left_child(old_index), left_child(new_index));
    C_and_B_shift(tree, right_child(old_index), right_child(new_index));
}

static void A_shift(rbt_t *tree, size_t old_index, size_t new_index, bool is_left)
{
    if (tree->data[old_index] == tree->NIL)
    {
        return;
    }

    if (is_left)
    {
        // Get the right most subtree.
        A_shift(tree, left_child(old_index), left_child(new_index), true);
        A_shift(tree, right_child(old_index), right_child(new_index), true);
    }
    else
    {
        // Get the left most subtree.
        A_shift(tree, right_child(old_index), right_child(new_index), false);
        A_shift(tree, left_child(old_index), left_child(new_index), false);
    }

    tree->data[new_index] = tree->data[old_index];
    tree->data[old_index] = tree->NIL;
}

/*
   A
 /   \
B
 \
  C
*/
static void right_rotate(rbt_t *tree, size_t rotate_index)
{
    // Shift all of the elements to the right of A downwards to get their correct location.
    A_shift(tree, right_child(rotate_index), right_child(right_child(rotate_index)), false);
    // Copy A into the its right child.
    tree->data[right_child(rotate_index)] = tree->data[rotate_index];

    size_t old_C_index = right_child(left_child(rotate_index));

    // C could not be there.
    if (tree->data[old_C_index] != tree->NIL)
    {
        size_t new_C_index = old_C_index + 1;

        // Copy C into the left child of A.
        tree->data[new_C_index] = tree->data[old_C_index];
        tree->data[old_C_index] = tree->NIL;

        // Shift all of the elements to the left and right of C downwards to get their correct location.
        C_and_B_shift(tree, left_child(old_C_index), left_child(new_C_index));
        C_and_B_shift(tree, right_child(old_C_index), right_child(new_C_index));
    }

    // Copy B into A's original location.
    size_t B_index = left_child(rotate_index);
    tree->data[rotate_index] = tree->data[B_index];

    // If B doesn't have any nodes to the left, just set the left side to NIL.
    if (tree->data[left_child(B_index)] == tree->NIL)
    {
        tree->data[B_index] = tree->NIL;
    }
    else
    {
        // Shift all of the elements to the left of B upwards to get their correct location.
        C_and_B_shift(tree, left_child(B_index), B_index);
    }
}

/*
  A
/   \
     B
    /
   C
*/
static void left_rotate(rbt_t *tree, size_t rotate_index)
{
    // Shift all of the elements to the left of A downwards to get their correct location.
    A_shift(tree, left_child(rotate_index), left_child(left_child(rotate_index)), true);
    // Copy A into the its left child.
    tree->data[left_child(rotate_index)] = tree->data[rotate_index];

    size_t old_C_index = left_child(right_child(rotate_index));

    // C could not be there.
    if (tree->data[old_C_index] != tree->NIL)
    {
        size_t new_C_index = old_C_index - 1;

        // Copy C into the right child of A.
        tree->data[new_C_index] = tree->data[old_C_index];
        tree->data[old_C_index] = tree->NIL;

        // Shift all of the elements to the left and right of C downwards to get their correct location.
        C_and_B_shift(tree, left_child(old_C_index), left_child(new_C_index));
        C_and_B_shift(tree, right_child(old_C_index), right_child(new_C_index));
    }

    // Copy B into A's original location.
    size_t B_index = right_child(rotate_index);
    tree->data[rotate_index] = tree->data[B_index];

    // If B doesn't have any nodes to the right, just set the right side to NIL.
    if (tree->data[right_child(B_index)] == tree->NIL)
    {
        tree->data[B_index] = tree->NIL;
    }
    else
    {
        // Shift all of the elements to the right of B upwards to get their correct location.
        C_and_B_shift(tree, right_child(B_index), B_index);
    }
}

static void rbt_set_node_red(rbt_t *tree, size_t index)
{
    tree->data[index]->is_red = true;
}

static void rbt_set_node_black(rbt_t *tree, size_t index)
{
    tree->data[index]->is_red = false;
}

static void rbt_fixup(rbt_t *tree, size_t node_index)
{
    size_t parent_index = 0;
    size_t grandparent_index = 0;
    size_t uncle_index = 0;

    while (is_node_red(tree, parent(node_index)))
    {
        parent_index = parent(node_index);
        grandparent_index = parent(parent_index);

        // The parent node is on the left side, so get the
        // uncle on the right side.
        if (parent_index == left_child(grandparent_index))
        {
            uncle_index = right_child(grandparent_index);
        }
        // The parent node is on the right side, so get the
        // uncle on the left side.
        else
        {
            uncle_index = left_child(grandparent_index);
        }

        // Case 1: The uncle is red.
        if (is_node_red(tree, uncle_index))
        {
            rbt_set_node_black(tree, parent_index);
            rbt_set_node_black(tree, uncle_index);

            // If the grandparent is 0(root), don't set it to red.
            if (grandparent_index)
            {
                rbt_set_node_red(tree, grandparent_index);
            }

            node_index = grandparent_index;
        }
        else
        {
            // Case 2.1: Forms left triangle.
            if (uncle_index == right_child(grandparent_index) && node_index == right_child(parent_index))
            {
                left_rotate(tree, parent_index);
                // Update it to find the child violation node.
                node_index = left_child(parent_index);
            }
            // Case 2.1: Forms right triangle.
            else if (uncle_index == left_child(grandparent_index) && node_index == left_child(parent_index))
            {
                right_rotate(tree, parent_index);
                // Update it to find the child violation node.
                node_index = right_child(parent_index);
            }

            // Update indices.
            parent_index = parent(node_index);
            grandparent_index = parent(parent_index);

            // Case 2.2: Forms a stright line.
            rbt_set_node_black(tree, parent_index);
            rbt_set_node_red(tree, grandparent_index);

            if (uncle_index == right_child(grandparent_index))
            {
                right_rotate(tree, grandparent_index);
            }
            else
            {
                left_rotate(tree, grandparent_index);
            }
            
            // Update node_index to where it actually is
            // since it got rotated.
            node_index = parent(node_index);
        }
    }

    // Case 0: Root is always black.
    rbt_set_node_black(tree, 0);
}

rbt_t *rbt_init(rb_node_less less_nodes, rb_node_equal equal_nodes, rb_node_print print_node, rb_node_free free_node_value)
{
    rbt_t *tree = malloc(sizeof(rbt_t));

    tree->size = 0;
    tree->capacity = RBT_CAPACITY;

    tree->NIL = malloc(sizeof(rb_node_t*));
    memset(tree->NIL, 0, sizeof(*tree->NIL));

    tree->data = malloc(sizeof(rb_node_t*) * tree->capacity);

    tree->less_nodes = less_nodes;
    tree->equal_nodes = equal_nodes;
    tree->print_node = print_node;
    tree->free_node_value = free_node_value;

    for (size_t i = 0; i < tree->capacity; i++)
    {
        tree->data[i] = tree->NIL;
    }

    return tree;
}

void rbt_insert(rbt_t *tree, void *value)
{
    rb_node_t *node = malloc(sizeof(rb_node_t));
    node->value = value;
    node->is_red = true;

    if (tree->data[0] == tree->NIL)
    {
        tree->data[0] = node;
        tree->size++;
        rbt_set_node_black(tree, 0);
        return;
    }

    size_t insert_index = 0;

    while (true)
    {
        if (tree->less_nodes(node, tree->data[insert_index]))
        {
            insert_index = left_child(insert_index);
        }
        else
        {
            insert_index = right_child(insert_index);
        }

        if (tree->data[insert_index] == tree->NIL)
        {
            tree->data[insert_index] = node;
            break;
        }
    }

    tree->size++;

    rbt_fixup(tree, insert_index);
}

bool rbt_cotains_value(rbt_t *tree, void *value)
{
    size_t searched_index = 0;

    while (tree->data[searched_index] != tree->NIL)
    {
        if (tree->equal_nodes(value, tree->data[searched_index]->value))
        {
            return true;
        }

        if (tree->less_nodes(value, tree->data[searched_index]->value))
        {
            searched_index = left_child(searched_index);
        }
        else
        {
            searched_index = right_child(searched_index);
        }
    }

    return false;
}

void rbt_print(rbt_t *tree, int print_order)
{
    if (tree->data[0] == tree->NIL)
    {
        return;
    }

    if (print_order & PRINT_PREORDER)
    {
        pre_order(tree, 0);
    }

    if (print_order & PRINT_INORDER)
    {
        in_order(tree, 0);
    }

    if (print_order & PRINT_POSTORDER)
    {
        post_order(tree, 0);
    }
}

void rbt_free(rbt_t *tree)
{
    for (size_t i = 0; i < tree->capacity; i++)
    {
        if (tree->data[i] != tree->NIL)
        {
            tree->free_node_value(tree->data[i]->value);
            free(tree->data[i]);
        }
    }

    free(tree->data);
    free(tree->NIL);
    free(tree);
}
