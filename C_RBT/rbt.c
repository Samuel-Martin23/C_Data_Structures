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

void right_rotate(int **data, size_t rotate_index)
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

void left_rotate(int **data, size_t rotate_index)
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

int *alloc_int(int x)
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
    tree->longest_path = 0;

    tree->data = malloc(sizeof(int*) * tree->capacity);
    memset(tree->data, 0, tree->capacity * sizeof(tree->data[0]));

    return tree;
}

rbt_t *rbt_init_args(size_t size, ...)
{
    rbt_t *tree = rbt_init();
    tree->size = size;

    va_list args;
    va_start(args, size);

    for (size_t i = 0; i < tree->size; i++)
    {
        tree->data[i] = alloc_int(va_arg(args, int));
    }

    va_end(args);

    return tree;
}

void rbt_print(rbt_t *tree)
{
    if (tree->data == NULL)
    {
        return;
    }

    for (size_t i = 0; i < tree->size; i++)
    {
        if (tree->data[i] != NULL)
        {
            printf("|%zu", i);
        }
    }

    printf("|\n");

    for (size_t i = 0; i < tree->size; i++)
    {
        if (tree->data[i] != NULL)
        {
            printf("|%c", *(tree->data[i]));
        }
    }

    printf("|\n");
}

void rbt_free(rbt_t **tree)
{
    for (size_t i = 0; i < (*tree)->size; i++)
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
