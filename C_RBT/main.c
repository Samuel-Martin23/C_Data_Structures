#include "rbt.h"

#define SIZE 5

int main()
{
    rbt_t *tree = rbt_init_args(SIZE, 10, 15, 12, 13, 11);

    rbt_print(tree, PRINT_PREORDER);

    rbt_free(&tree);

    return 0;
}
