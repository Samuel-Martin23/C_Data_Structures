#include "rbt.h"

int main()
{
    rbt_t *tree = rbt_init();
    tree->size = RBT_CAPACITY;
    
    /* Right Rotate
    tree->data[0] = alloc_int('A');
    tree->data[1] = alloc_int('B');
    tree->data[2] = alloc_int('D');
    tree->data[3] = alloc_int('E');
    tree->data[4] = alloc_int('C');
    tree->data[5] = alloc_int('H');
    tree->data[6] = alloc_int('G');
    tree->data[7] = alloc_int('J');
    tree->data[8] = alloc_int('K');
    tree->data[10] = alloc_int('F');
    tree->data[13] = alloc_int('I');
    */

    /* Left Rotate
    tree->data[0] = alloc_int('A');
    tree->data[1] = alloc_int('H');
    tree->data[2] = alloc_int('B');
    tree->data[3] = alloc_int('J');
    tree->data[4] = alloc_int('I');
    tree->data[5] = alloc_int('C');
    tree->data[6] = alloc_int('E');
    tree->data[8] = alloc_int('K');
    tree->data[11] = alloc_int('D');
    tree->data[13] = alloc_int('F');
    tree->data[14] = alloc_int('G');
    */

    rbt_print(tree);
    left_rotate(tree->data, 0);
    printf("\n");
    rbt_print(tree);

    rbt_free(&tree);

    return 0;
}
