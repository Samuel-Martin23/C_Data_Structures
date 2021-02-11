
#include "bst.h"

#define BST_ROOT_NULL               0x00000001u
#define BST_TYPE                    0x00000002u
#define BST_INDEX                   0x00000004u
#define BST_SIZE                    0x00000008u
#define TURN_OFF_WARNING            0x00000010u

#define WARNING_PLACEHOLDER         -1

static bool check_warnings(bst_t *tree, u_int16_t warning_code, const char *function_name, int check_value)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & BST_ROOT_NULL)
    {
        if (tree->root == NULL)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s bst root is NULL%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & BST_TYPE)
    {
        template_t T = (template_t)check_value;

        if (tree->T != T)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s bst type does not equal the new value type%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & BST_INDEX)
    {
        int index = check_value;

        if (index < 0 || index > tree->last_index)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s index out of range%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & BST_SIZE)
    {
        if (tree->size)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s bst size is not zero%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    return false;
}

static node_bst_t *new_node(template_t T, void *value)
{
    size_t number_of_bytes = sizeof(node_bst_t);
    
    node_bst_t *node = malloc(number_of_bytes);
    node->value = new_T_value(T, value);
    node->left = NULL;
    node->right = NULL;

    mem_usage.allocated += (u_int32_t)number_of_bytes;

    return node;
}

static void free_node(node_bst_t **curr, template_t T)
{
    free_T_value(T, (*curr)->value);
    free(*curr);
    *curr = NULL;

    mem_usage.freed += (u_int32_t)sizeof(node_bst_t);
}

static node_bst_t *lookup_node(node_bst_t *root, template_t T, void *value, node_bst_t **parent)
{
    node_bst_t *top = root;

    while (top != NULL)
    {
        if (check_equal_value(T, value, top->value))
        {
            break;
        }
        else if (check_less_value(T, value, top->value))
        {
            *parent = top;
            top = top->left;
        }
        else if (check_greater_value(T, value, top->value))
        {
            *parent = top;
            top = top->right;
        }
    }

    return top;
}

static void *find_minimum(node_bst_t *curr)
{
    while (curr->left != NULL)
    {
        curr = curr->left;
    }

    return curr->value;
}

static bool remove_value(node_bst_t **root, template_t T, void *value)
{
    node_bst_t *parent = NULL;
    node_bst_t *curr = lookup_node(*root, T, value, &parent);

    if (curr == NULL)
    {
        return false;
    }

    if (curr->left == NULL && curr->right == NULL)
    {
        if (curr == *root)
        {
            free_node(root, T);
        }
        else if (check_less_value(T, curr->value, parent->value))
        {
            free_node(&parent->left, T);
        }
        else if (check_greater_value(T, curr->value, parent->value))
        {
            free_node(&parent->right, T);
        }
    }
    else if (curr->left != NULL && curr->right != NULL)
    {
        void *min_value = new_T_value(T, find_minimum(curr->right));

        remove_value(root, T, min_value);

        free_T_value(T, curr->value);

        curr->value = min_value;
    }
    else
    {
        node_bst_t *child = curr->left ? curr->left : curr->right;

        if (curr != *root)
        {
            if (curr == parent->left)
            {
                parent->left = child;
            }
            else
            {
                parent->right = child;
            }

            free_node(&curr, T);
        }
        else
        {
            free_node(root, T);
            *root = child;
        }
    }

    return true;
}

static bool check_int(float value)
{
    int truncated = (int)value;
    return check_float_equal(value, (float)truncated);
}

static void get_index(node_bst_t **root, int index, float left_value, float right_value)
{
    if (index == 0)
    {
        return;
    }

    left_value = (index - 1) / 2.0f;
    right_value = (index - 2) / 2.0f;

    if (check_int(left_value))
    {
        get_index(root, (int)left_value, 0.0f, 0.0f);

        if (*root == NULL)
        {
            return;
        }

        *root = (*root)->left;
    }
    else
    {
        get_index(root, (int)right_value, 0.0f, 0.0f);

        if (*root == NULL)
        {
            return;
        }

        *root = (*root)->right;
    }
}

static void copy_nodes(bst_t *tree_dest, node_bst_t *root_src)
{
    if (root_src == NULL)
    {
        return;
    }

    if (tree_dest->T == STR)
    {
        bst_insert(tree_dest, str_cast_void(root_src->value));
    }
    else
    {
        bst_insert(tree_dest, tree_dest->T, root_src->value);
    }

    copy_nodes(tree_dest, root_src->left);
    copy_nodes(tree_dest, root_src->right);
}

static void free_nodes(node_bst_t **curr, node_bst_t *parent, template_t T)
{
    if (*curr == NULL)
    {
        return;
    }

    free_nodes(&(*curr)->left, *curr, T);
    free_nodes(&(*curr)->right, *curr, T);

    if (parent == NULL)
    {
        free_node(curr, T);
    }
    else if (parent->left == *curr)
    {
        free_node(&parent->left, T);
    }
    else
    {
        free_node(&parent->right, T);
    }
}

static void inorder_print(template_t T, node_bst_t *root)
{
    if (root == NULL)
    {
        return;
    }

    inorder_print(T, root->left);
    print_t(T, root->value, "", " ");
    inorder_print(T, root->right);
}

static void inorder_print_index(bst_t *tree)
{
    void *value = NULL;

    for (int i = 0; i <= tree->last_index; i++)
    {
        value = bst_get_index(tree, i);

        if (value != NULL)
        {
            printf("%d: ", i);
            print_t(tree->T, value, "", "\n");
        }
    }
}

static void preorder_print(template_t T, node_bst_t *root)
{
    if (root == NULL)
    {
        return;
    }

    print_t(T, root->value, "", " ");
    preorder_print(T, root->left);
    preorder_print(T, root->right);
}

static void postorder_print(template_t T, node_bst_t *root)
{
    if (root == NULL)
    {
        return;
    }

    postorder_print(T, root->left);
    postorder_print(T, root->right);
    print_t(T, root->value, "", " ");
}

static bool check_bst_root_null(bst_t *tree, template_t T, void *value)
{
    if (tree->root == NULL)
    {
        tree->root = new_node(T, value);
        tree->size++;

        if (tree->T == NONE)
        {
            tree->T = T;
        }

        return true;
    }

    return false;
}

bst_t bst_init(template_t T, void *data, int size)
{
    bst_t new_tree;
    new_tree.root = NULL;
    new_tree.T = T;
    new_tree.size = 0;
    new_tree.last_index = 0;

    if (size == 0)
    {
        return new_tree;
    }

    switch (new_tree.T)
    {
        case INT:
        case DOUBLE:
        case FLOAT:
        case CHAR:
            for (int i = 0; i < size; i++)
            {
                bst_insert(&new_tree, new_tree.T, data);
                data += get_bytes(new_tree.T, data);
            }
            break;
        case STR:
            {
                char **str_array = ((char**)data);

                for (int i = 0; i < size; i++)
                {
                    bst_insert(&new_tree, str_cast_void(*str_array));
                    str_array++;
                }
            }
            break;
        case BOOL:
        case NONE: // default:
            break;
    }

    return new_tree;
}

void bst_insert(bst_t *tree, template_t T, void *value)
{
    convert_2d_str(T, &value);

    if (check_bst_root_null(tree, T, value))
    {
        return;
    }

    if (check_warnings(tree, BST_TYPE, "bst_insert", (int)T))
    {
        return;
    }

    int index = 0;
    node_bst_t *top = tree->root;

    while (top != NULL)
    {
        if (check_equal_value(T, value, top->value))
        {
            return;
        }
        else if (check_less_value(T, value, top->value))
        {
            index = (index * 2) + 1;

            if (top->left == NULL)
            {
                break;
            }

            top = top->left;
        }
        else if (check_greater_value(T, value, top->value))
        {
            index = (index * 2) + 2;

            if (top->right == NULL)
            {
                break;
            }

            top = top->right;
        }
    }

    if (check_less_value(T, value, top->value))
    {
        top->left = new_node(T, value);
    }
    else if (check_greater_value(T, value, top->value))
    {
        top->right = new_node(T, value);
    }

    tree->size++;

    if (index > tree->last_index)
    {
        tree->last_index = index;
    }
}

void bst_remove_value(bst_t *tree, template_t T, void *value)
{
    if (check_warnings(tree, BST_ROOT_NULL | BST_TYPE, "bst_remove_value", (int)T))
    {
        return;
    }

    convert_2d_str(T, &value);

    if (remove_value(&tree->root, T, value))
    {
        tree->size--;
    }
}

node_bst_t *bst_lookup(bst_t *tree, template_t T, void *value)
{
    if (check_warnings(tree, BST_ROOT_NULL | BST_TYPE, "bst_lookup", (int)T))
    {
        return NULL;
    }

    convert_2d_str(T, &value);

    node_bst_t *parent = NULL;
    return lookup_node(tree->root, T, value, &parent);
}

node_bst_t *bst_lookup_with_parent(bst_t *tree, template_t T, void *value, node_bst_t **parent)
{
    if (check_warnings(tree, BST_ROOT_NULL | BST_TYPE, "bst_lookup_with_parent", (int)T))
    {
        return NULL;
    }

    convert_2d_str(T, &value);

    return lookup_node(tree->root, T, value, parent);
}

void *bst_get_index(bst_t *tree, int index)
{
    if (check_warnings(tree, BST_ROOT_NULL | BST_INDEX, "bst_get_index", index))
    {
        return NULL;
    }

    node_bst_t *top = tree->root;

    get_index(&top, index, 0.0f, 0.0f);

    if (top != NULL) 
    {
        return top->value;
    }

    return NULL;
}

void bst_copy(bst_t *tree_dest, bst_t *tree_src)
{
    if (check_warnings(tree_src, BST_ROOT_NULL | BST_TYPE,
        "bst_copy", (int)tree_dest->T)
        || check_warnings(tree_dest, BST_SIZE, "bst_copy", WARNING_PLACEHOLDER))
    {
        return;
    }

    tree_dest->root = NULL;
    tree_dest->T = tree_src->T;
    tree_dest->size = tree_src->size;
    tree_dest->last_index = 0;

    if (tree_dest->size == 0)
    {
        return;
    }

    copy_nodes(tree_dest, tree_src->root);
}

void bst_free(bst_t *tree)
{
    if (check_warnings(tree, BST_ROOT_NULL, "bst_free", WARNING_PLACEHOLDER))
    {
        return;
    }

    free_nodes(&tree->root, NULL, tree->T);

    tree->T = NONE;
    tree->size = 0;
    tree->last_index = 0;
}

void bst_print(bst_t *tree, u_int16_t bst_print_options)
{
    if (check_warnings(tree, BST_ROOT_NULL, "bst_print", WARNING_PLACEHOLDER))
    {
        return;
    }

    if (bst_print_options & BST_INORDER)
    {
        inorder_print(tree->T, tree->root);
        printf("\n");
    }

    if (bst_print_options & BST_INORDER_INDEX)
    {
        inorder_print_index(tree);
        printf("\n");
    }

    if (bst_print_options & BST_PREORDER)
    {
        preorder_print(tree->T, tree->root);
        printf("\n");
    }

    if (bst_print_options & BST_POSTORDER)
    {
        postorder_print(tree->T, tree->root);
        printf("\n");
    }
}
