
#include "dictionary.h"

#define DICT_ROOT_NULL              0x00000001u
#define DICT_SIZE                   0x00000002u
#define TURN_OFF_WARNING            0x00000004u

static bool check_warnings(dictionary_t *dict, u_int16_t warning_code, const char *function_name)
{
    const char *purple = "\033[1;95m";
    const char *white = "\033[1;97m";
    const char *reset = "\033[0m";

    if (warning_code & DICT_ROOT_NULL)
    {
        if (dict->root == NULL)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s dictionary root is NULL%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    if (warning_code & DICT_SIZE)
    {
        if (dict->size)
        {
            if (!(warning_code & TURN_OFF_WARNING))
            {
                printf("%s: %swarning:%s dictionary size is not zero%s\n", function_name, purple, white, reset);
            }

            return true;
        }
    }

    return false;
}

static node_dict_t *new_node(void *key, void *value)
{
    size_t number_of_bytes = sizeof(node_dict_t);
    
    node_dict_t *node = malloc(number_of_bytes);
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;

    mem_usage.allocated += (u_int32_t)number_of_bytes;

    return node;
}

static void free_node(node_dict_t **curr, template_t T_key, template_t T_value)
{
    free_T_value(T_key, (*curr)->key);
    free_T_value(T_value, (*curr)->value);
    free(*curr);
    *curr = NULL;

    mem_usage.freed += (u_int32_t)sizeof(node_dict_t);
}

static node_dict_t *lookup_node(node_dict_t *root, template_t T, void *key, node_dict_t **parent)
{
    node_dict_t *top = root;

    while (top != NULL)
    {
        if (check_equal_value(T, key, top->key, true))
        {
            break;
        }
        else if (check_less_value(T, key, top->key, true))
        {
            *parent = top;
            top = top->left;
        }
        else if (check_greater_value(T, key, top->key, true))
        {
            *parent = top;
            top = top->right;
        }
    }

    return top;
}

static void *find_minimum(node_dict_t *curr)
{
    while (curr->left != NULL)
    {
        curr = curr->left;
    }

    return curr->key;
}

static bool remove_key(node_dict_t **root, template_t T_key, void *key, template_t T_value)
{
    node_dict_t *parent = NULL;
    node_dict_t *curr = lookup_node(*root, T_key, key, &parent);

    if (curr == NULL)
    {
        return false;
    }

    if (curr->left == NULL && curr->right == NULL)
    {
        if (curr == *root)
        {
            free_node(root, T_key, T_value);
        }
        else if (check_less_value(T_key, curr->key, parent->key, true))
        {
            free_node(&parent->left, T_key, T_value);
        }
        else if (check_greater_value(T_key, curr->key, parent->key, true))
        {
            free_node(&parent->right, T_key, T_value);
        }
    }
    else if (curr->left != NULL && curr->right != NULL)
    {
        void *min_value = new_T_value(T_key, find_minimum(curr->right));

        remove_key(root, T_key, min_value, T_value);

        free_T_value(T_key, curr->key);

        curr->key = min_value;
    }
    else
    {
        node_dict_t *child = curr->left ? curr->left : curr->right;

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

            free_node(&curr, T_key, T_value);
        }
        else
        {
            free_node(root, T_key, T_value);
            *root = child;
        }
    }

    return true;
}

static void insert_copy(dictionary_t *dict, void *key, void *value)
{
    switch (dict->T_key)
    {
        case INT:
            switch (dict->T_value)
            {
                case INT:
                    dict_insert(dict, void_cast_int(key), void_cast_int(value));
                    break;
                case DOUBLE:
                    dict_insert(dict, void_cast_int(key), void_cast_double(value));
                    break;
                case FLOAT:
                    dict_insert(dict, void_cast_int(key), void_cast_float(value));
                    break;
                case CHAR:
                    dict_insert(dict, void_cast_int(key), void_cast_char(value));
                    break;
                case STR:
                    dict_insert(dict, void_cast_int(key), void_cast_str(value));
                    break;
                case BOOL:
                    dict_insert(dict, void_cast_int(key), void_cast_bool(value));
                    break;
                case NONE: // defualt:
                    break;
            }
            break;
        case DOUBLE:
            switch (dict->T_value)
            {
                case INT:
                    dict_insert(dict, void_cast_double(key), void_cast_int(value));
                    break;
                case DOUBLE:
                    dict_insert(dict, void_cast_double(key), void_cast_double(value));
                    break;
                case FLOAT:
                    dict_insert(dict, void_cast_double(key), void_cast_float(value));
                    break;
                case CHAR:
                    dict_insert(dict, void_cast_double(key), void_cast_char(value));
                    break;
                case STR:
                    dict_insert(dict, void_cast_double(key), void_cast_str(value));
                    break;
                case BOOL:
                    dict_insert(dict, void_cast_double(key), void_cast_bool(value));
                    break;
                case NONE: // default:
                    break;
            }
            break;
        case FLOAT:
            switch (dict->T_value)
            {
                case INT:
                    dict_insert(dict, void_cast_float(key), void_cast_int(value));
                    break;
                case DOUBLE:
                    dict_insert(dict, void_cast_float(key), void_cast_double(value));
                    break;
                case FLOAT:
                    dict_insert(dict, void_cast_float(key), void_cast_float(value));
                    break;
                case CHAR:
                    dict_insert(dict, void_cast_float(key), void_cast_char(value));
                    break;
                case STR:
                    dict_insert(dict, void_cast_float(key), void_cast_str(value));
                    break;
                case BOOL:
                    dict_insert(dict, void_cast_float(key), void_cast_bool(value));
                    break;
                case NONE: // default:
                    break;
            }
            break;
        case CHAR:
            switch (dict->T_value)
            {
                case INT:
                    dict_insert(dict, void_cast_char(key), void_cast_int(value));
                    break;
                case DOUBLE:
                    dict_insert(dict, void_cast_char(key), void_cast_double(value));
                    break;
                case FLOAT:
                    dict_insert(dict, void_cast_char(key), void_cast_float(value));
                    break;
                case CHAR:
                    dict_insert(dict, void_cast_char(key), void_cast_char(value));
                    break;
                case STR:
                    dict_insert(dict, void_cast_char(key), void_cast_str(value));
                    break;
                case BOOL:
                    dict_insert(dict, void_cast_char(key), void_cast_bool(value));
                    break;
                case NONE: // default:
                    break;
            }
            break;
        case STR:
            switch (dict->T_value)
            {
                case INT:
                    dict_insert(dict, void_cast_str(key), void_cast_int(value));
                    break;
                case DOUBLE:
                    dict_insert(dict, void_cast_str(key), void_cast_double(value));
                    break;
                case FLOAT:
                    dict_insert(dict, void_cast_str(key), void_cast_float(value));
                    break;
                case CHAR:
                    dict_insert(dict, void_cast_str(key), void_cast_char(value));
                    break;
                case STR:
                    dict_insert(dict, void_cast_str(key), void_cast_str(value));
                    break;
                case BOOL:
                    dict_insert(dict, void_cast_str(key), void_cast_bool(value));
                    break;
                case NONE: // default:
                    break;
            }
            break;
        case BOOL:
            switch (dict->T_value)
            {
                case INT:
                    dict_insert(dict, void_cast_bool(key), void_cast_int(value));
                    break;
                case DOUBLE:
                    dict_insert(dict, void_cast_bool(key), void_cast_double(value));
                    break;
                case FLOAT:
                    dict_insert(dict, void_cast_bool(key), void_cast_float(value));
                    break;
                case CHAR:
                    dict_insert(dict, void_cast_bool(key), void_cast_char(value));
                    break;
                case STR:
                    dict_insert(dict, void_cast_bool(key), void_cast_str(value));
                    break;
                case BOOL:
                    dict_insert(dict, void_cast_bool(key), void_cast_bool(value));
                    break;
                case NONE: // default:
                    break;
            }
            break;
        case NONE:
            break;
    }
}

static void copy_nodes(dictionary_t *dict_dest, node_dict_t *root_src)
{
    if (root_src == NULL)
    {
        return;
    }

    insert_copy(dict_dest, root_src->key, root_src->value);

    copy_nodes(dict_dest, root_src->left);
    copy_nodes(dict_dest, root_src->right);
}

static void free_nodes(node_dict_t **curr, node_dict_t *parent, template_t T_key, template_t T_value)
{
    if (*curr == NULL)
    {
        return;
    }

    free_nodes(&(*curr)->left, *curr, T_key, T_value);
    free_nodes(&(*curr)->right, *curr, T_key, T_value);

    if (parent == NULL)
    {
        free_node(curr, T_key, T_value);
    }
    else if (parent->left == *curr)
    {
        free_node(&parent->left, T_key, T_value);
    }
    else
    {
        free_node(&parent->right, T_key, T_value);
    }
}

static void preorder_print(template_t T_key, template_t T_value, node_dict_t *root)
{
    if (root == NULL)
    {
        return;
    }

    preorder_print(T_key, T_value, root->left);
    preorder_print(T_key, T_value, root->right);

    print_t(T_key, root->key, "", ": ");
    print_t(T_value, root->value, "", ", ");
}

dictionary_t dict_init(template_t T_key, template_t T_value)
{
    dictionary_t new_dict;
    new_dict.size = 0;
    new_dict.T_key = T_key;
    new_dict.T_value = T_value;
    new_dict.root = NULL;

    return new_dict;
}

void dict_insert(dictionary_t *dict, ...)
{
    va_list args;
    va_start(args, dict);

    void *key = new_arg_T_value(dict->T_key, args);
    void *value = new_arg_T_value(dict->T_value, args);

    if (dict->root == NULL)
    {
        dict->root = new_node(key, value);
        dict->size = 1;
        va_end(args);
        return;
    }

    va_end(args);

    node_dict_t *top = dict->root;

    while (top != NULL)
    {
        if (check_equal_value(dict->T_key, key, top->key, true))
        {
            free_T_value(dict->T_key, key);
            free_T_value(dict->T_value, value);
            return;
        }
        else if (check_less_value(dict->T_key, key, top->key, true))
        {
            if (top->left == NULL)
            {
                break;
            }

            top = top->left;
        }
        else if (check_greater_value(dict->T_key, key, top->key, true))
        {
            if (top->right == NULL)
            {
                break;
            }

            top = top->right;
        }
    }

    if (check_less_value(dict->T_key, key, top->key, true))
    {
        top->left = new_node(key, value);
    }
    else if (check_greater_value(dict->T_key, key, top->key, true))
    {
        top->right = new_node(key, value);
    }

    dict->size++;
}

void dict_remove_key(dictionary_t *dict, ...)
{
    if (check_warnings(dict, DICT_ROOT_NULL, "dict_remove_key"))
    {
        return;
    }

    va_list args;
    va_start(args, dict);

    void *key = new_arg_T_value(dict->T_key, args);

    va_end(args);

    if (remove_key(&dict->root, dict->T_key, key, dict->T_value))
    {
        dict->size--;
    }

    free_T_value(dict->T_key, key);
}

void *dict_lookup(dictionary_t *dict, ...)
{
    if (check_warnings(dict, DICT_ROOT_NULL, "dict_lookup"))
    {
        return NULL;
    }

    va_list args;
    va_start(args, dict);

    void *key = new_arg_T_value(dict->T_key, args);

    va_end(args);

    node_dict_t *parent = NULL;
    node_dict_t *curr = lookup_node(dict->root, dict->T_key, key, &parent);
    free_T_value(dict->T_key, key);

    if (curr == NULL)
    {
        return NULL;
    }
    else
    {
        return curr->value;
    }
}

void dict_copy(dictionary_t *dict_dest, dictionary_t *dict_src)
{
    if (check_warnings(dict_src, DICT_ROOT_NULL, "dict_copy")
        || check_warnings(dict_dest, DICT_SIZE, "dict_copy"))
    {
        return;
    }

    if (dict_dest->T_key != dict_src->T_key || dict_dest->T_value != dict_src->T_value)
    {
        printf("dict_copy: \033[1;95mwarning:\033[1;97m dictionary dest type does not equal dictionary src type\033[0m\n");
        return;
    }

    copy_nodes(dict_dest, dict_src->root);
}

void dict_free(dictionary_t *dict)
{
    if (check_warnings(dict, DICT_ROOT_NULL, "dict_free"))
    {
        return;
    }

    free_nodes(&dict->root, NULL, dict->T_key, dict->T_value);

    dict->size = 0;
}

void dict_print(dictionary_t *dict)
{
    if (check_warnings(dict, DICT_ROOT_NULL, "dict_print"))
    {
        return;
    }

    printf("{");
    preorder_print(dict->T_key, dict->T_value, dict->root);
    printf("\b\b}");
    printf("\n");
}
