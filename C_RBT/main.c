#include "rbt.h"

int main()
{
    rbt_t *tree = rbt_init_args(7, 20, 10, 30, 5, 15, 25, 30);

    rbt_print(tree, PRINT_PREORDER);

    rbt_free(&tree);

    return 0;
}
