
#include "bst.h"

int main()
{
    bst_t scores = bst_init_int(25, 15, 50, 10, 22, 35, 70);
    bst_insert(&scores, int_cast_void(99));

    void *value = bst_get_index(&scores, 2);
    node_bst_t *found_node = bst_lookup(&scores, int_cast_void(15));
    print_t(scores.T, found_node->value, "", "\n");
    print_t(scores.T, value, "", "\n");

    bst_remove_value(&scores, int_cast_void(50));

    bst_print(&scores, BST_INORDER | BST_POSTORDER);

    bst_free(&scores);

    return 0;
}
