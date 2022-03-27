#include "rbt.h"

#define SIZE 8

int main()
{
    rbt_t *tree = rbt_init_args(SIZE, 7, 3, 8, 2, 4, 5, 1, 6);

    rbt_print(tree, PRINT_PREORDER);

    rbt_free(&tree);

    return 0;
}
