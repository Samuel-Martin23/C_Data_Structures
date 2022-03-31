#include "bst.h"

int main()
{
    bst_t scores = bst_init(INT, 7, 25, 15, 50, 10, 22, 35, 70);
    bst_insert(&scores, 99);

    void *value = bst_get_index(&scores, 2);
    node_bst_t *found_node = bst_lookup(&scores, 15);
    print_t(scores.T, found_node->value, "", "\n");
    print_t(scores.T, value, "", "\n");

    bst_remove_value(&scores, 50);

    bst_print(&scores, BST_INORDER | BST_POSTORDER);

    bst_free(&scores);

    print_total_mem();

    return 0;
}
