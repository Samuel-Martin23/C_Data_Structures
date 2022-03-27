#include "rbt.h"

/*
static size_t parent(size_t index)
{
    if (index == 0)
    {
        return 0;
    }

    return (index - 1) / 2;
}
*/

static size_t left_child(size_t index)
{
    return (2 * index) + 1;
}

static size_t right_child(size_t index)
{
    return (2 * index) + 2;
}

static void pre_order(rbt_t *tree, size_t index)
{
    if (tree->data[index] == NULL)
    {
        return;
    }

    printf("%d\n", *tree->data[index]);
    pre_order(tree, left_child(index));
    pre_order(tree, right_child(index));
}

static void in_order(rbt_t *tree, size_t index)
{
    if (tree->data[index] == NULL)
    {
        return;
    }

    in_order(tree, left_child(index));
    printf("%d\n", *tree->data[index]);
    in_order(tree, right_child(index));
}

static void post_order(rbt_t *tree, size_t index)
{
    if (tree->data[index] == NULL)
    {
        return;
    }

    post_order(tree, left_child(index));
    post_order(tree, right_child(index));
    printf("%d\n", *tree->data[index]);
}

static void C_and_B_rotation(int **data, size_t old_index, size_t new_index)
{
    if (data[old_index] == NULL)
    {
        return;
    }

    data[new_index] = data[old_index];
    data[old_index] = NULL;

    C_and_B_rotation(data, left_child(old_index), left_child(new_index));
    C_and_B_rotation(data, right_child(old_index), right_child(new_index));
}

static void A_rotation(int **data, size_t old_index, size_t new_index, bool is_left)
{
    if (data[old_index] == NULL)
    {
        return;
    }

    if (is_left)
    {
        // Get the right most subtree.
        A_rotation(data, left_child(old_index), left_child(new_index), is_left);
        A_rotation(data, right_child(old_index), right_child(new_index), is_left);
    }
    else
    {
        // Get the left most subtree.
        A_rotation(data, right_child(old_index), right_child(new_index), is_left);
        A_rotation(data, left_child(old_index), left_child(new_index), is_left);
    }

    data[new_index] = data[old_index];
    data[old_index] = NULL;
}

static void right_rotate(int **data, size_t rotate_index)
{
    // Shift all of the elements to the right of A downwards to get their correct location.
    A_rotation(data, right_child(rotate_index), right_child(right_child(rotate_index)), false);
    // Copy A into the its right child.
    data[right_child(rotate_index)] = data[rotate_index];

    // Copy C into the left child of A.
    size_t old_C_index = right_child(left_child(rotate_index));
    size_t new_C_index = old_C_index + 1;

    data[new_C_index] = data[old_C_index];
    data[old_C_index] = NULL;

    // Shift all of the elements to the left and right of C downwards to get their correct location.
    C_and_B_rotation(data, left_child(old_C_index), left_child(new_C_index));
    C_and_B_rotation(data, right_child(old_C_index), right_child(new_C_index));

    // Copy B into A's original location.
    data[rotate_index] = data[left_child(rotate_index)];
    // Shift all of the elements to the left of B upwards to get their correct location.
    C_and_B_rotation(data, left_child(left_child(rotate_index)), left_child(rotate_index));
}

static void left_rotate(int **data, size_t rotate_index)
{
    // Shift all of the elements to the left of A downwards to get their correct location.
    A_rotation(data, left_child(rotate_index), left_child(left_child(rotate_index)), true);
    // Copy A into the its left child.
    data[left_child(rotate_index)] = data[rotate_index];

    // Copy C into the right child of A.
    size_t old_C_index = left_child(right_child(rotate_index));
    size_t new_C_index = old_C_index - 1;

    data[new_C_index] = data[old_C_index];
    data[old_C_index] = NULL;

    // Shift all of the elements to the left and right of C downwards to get their correct location.
    C_and_B_rotation(data, left_child(old_C_index), left_child(new_C_index));
    C_and_B_rotation(data, right_child(old_C_index), right_child(new_C_index));

    // Copy B into A's original location.
    data[rotate_index] = data[right_child(rotate_index)];
    // Shift all of the elements to the right of B upwards to get their correct location.
    C_and_B_rotation(data, right_child(right_child(rotate_index)), right_child(rotate_index));
}

static int *alloc_int(int x)
{
    int *alloc_x = malloc(sizeof(int));
    *alloc_x = x;
    return alloc_x;
}

rbt_t *rbt_init(void)
{
    rbt_t *tree = malloc(sizeof(rbt_t));

    tree->size = 0;
    tree->capacity = RBT_CAPACITY;
    tree->last_index = 0;

    tree->data = malloc(sizeof(int*) * tree->capacity);
    memset(tree->data, 0, tree->capacity * sizeof(tree->data[0]));

    return tree;
}

rbt_t *rbt_init_args(size_t size, ...)
{
    rbt_t *tree = rbt_init();

    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < size; i++)
    {
        rbt_insert(tree, va_arg(args, int));
    }

    va_end(args);

    return tree;
}

void rbt_insert(rbt_t *tree, int value)
{
    if (tree->data[0] == NULL)
    {
        tree->data[0] = alloc_int(value);
        tree->size++;
        return;
    }

    size_t search_index = 0;

    while (true)
    {
        if (value < *tree->data[search_index])
        {
            search_index = left_child(search_index);
        }
        else
        {
            search_index = right_child(search_index);
        }

        if (tree->data[search_index] == NULL)
        {
            tree->data[search_index] = alloc_int(value);
            break;
        }
    }

    tree->size++;

    if (search_index > tree->last_index )
    {
        tree->last_index = search_index;
    }
}

void rbt_print(rbt_t *tree, int print_order)
{
    if (tree->data == NULL)
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

void rbt_free(rbt_t **tree)
{
    for (size_t i = 0; i <= (*tree)->last_index; i++)
    {
        if ((*tree)->data[i] != NULL)
        {
            free((*tree)->data[i]);
        }
    }

    free((*tree)->data);
    free((*tree));

    *tree = NULL;
}
