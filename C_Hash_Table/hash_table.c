
#include "hash_table.h"

bool check_two_equal_value(template_t T_Key_1, void *key_1, template_t T_Key_2, void *key_2)
{
    if (T_Key_1 != T_Key_2)
    {
        return false;
    }

    switch (T_Key_1)
    {
        case INT:
            return (void_cast_int(key_1) == void_cast_int(key_2));
        case DOUBLE:
            return (check_double_equal(void_cast_double(key_1), void_cast_double(key_2)));
        case FLOAT:
            return (check_float_equal(void_cast_float(key_1), void_cast_float(key_2)));
        case CHAR:
            return (void_cast_char(key_1) == void_cast_char(key_2));
        case STR:
            return (!(strcmp(void_cast_str(key_1), void_cast_str(key_2))));
    }

    return false;
}

bucket_t *new_bucket(template_t T_Key, void *key, template_t T_Value, void *value)
{
    bucket_t *container = malloc(sizeof(bucket_t));
    container->T_key = T_Key;
    container->key = key;
    container->T_value = T_Value;
    container->value = value;
    container->next = NULL;

    return container;
}

void free_bucket(bucket_t *container)
{
    free(container->key);
    free(container->value);
    free(container);
    container = NULL;
}

void *new_bucket_element(hash_table_t *inventory, template_t T, void *element)
{
    switch (T)
    {
        case INT:
            {
                int *allocated_value = malloc(sizeof(int));
                inventory->allocated_mem += (int)sizeof(int);
                *allocated_value = void_cast_int(element);
                element = allocated_value;
            }
            break;
        case DOUBLE:
            {
                double *allocated_value = malloc(sizeof(double));
                inventory->allocated_mem += (int)sizeof(double);
                *allocated_value = void_cast_double(element);
                element = allocated_value;
            }
            break;
        case FLOAT:
            {
                float *allocated_key = malloc(sizeof(float));
                inventory->allocated_mem += (int)sizeof(float);
                *allocated_key = void_cast_float(element);
                element = allocated_key;
            }
            break;
        case CHAR:
            {
                char key_data = void_cast_char(element);
                const char *allocated_value = malloc(sizeof(char) + 1);

                inventory->allocated_mem += (int)(sizeof(char) + 1);
                memcpy((char*)allocated_value, &key_data, 1);
                *((char*)allocated_value + 1) = '\0';

                element = (char*)allocated_value;
            }
            break;
        case STR:
            {
                const char *key_data = void_cast_str(element);
                size_t str_length = strlen(key_data);
                const char *allocated_value = malloc(sizeof(char) * (str_length + 1));

                inventory->allocated_mem += (int)(sizeof(char) * (str_length + 1));
                memcpy((char*)allocated_value, key_data, str_length);
                *((char*)allocated_value + str_length) = '\0';

                element = (char*)allocated_value;
            }
            break;
    }

    return element;
}

int generate_hash_code(template_t T_Key, void *key, int hash_table_size)
{
    int hash_code = 0;

    switch (T_Key)
    {
        case INT:
            hash_code = (void_cast_int(key) % hash_table_size);
            break;
        case DOUBLE:
            hash_code = (int)(fmod(void_cast_double(key), (double)hash_table_size));
            break;
        case FLOAT:
            hash_code = (int)(fmod(void_cast_float(key), (double)hash_table_size));
            break;
        case CHAR:
            hash_code = (void_cast_char(key) % hash_table_size);
            break;
        case STR:
            {
                int i;
                const char *word = void_cast_str(key);
                int str_length = (int)strlen(word);

                for (i = 0; i < str_length; i++)
                {
                    hash_code += word[i] * (int)pow(31, str_length - (i + 1));
                }

                hash_code = (hash_code + word[i]) % hash_table_size;
            }
            break;
    }

    return hash_code;
}

void print_allocated_mem_ht(hash_table_t *inventory)
{
    printf("Bytes Allocated: %d\n", inventory->allocated_mem);
}

void print_num_of_buckets(hash_table_t *inventory)
{
    printf("Hash Table Size: %d\n", inventory->number_of_buckets);
}

hash_table_t hash_table_init(int size)
{
    hash_table_t curr_table;
    curr_table.size = size;
    curr_table.number_of_buckets = 0;
    curr_table.hash_table = malloc(sizeof(bucket_t*) * (size_t)curr_table.size);
    curr_table.first_index = 0;
    curr_table.last_index = 0;
    curr_table.allocated_mem = (int)(sizeof(bucket_t*) * (size_t)curr_table.size);

    for (int i = 0; i < curr_table.size; i++)
    {
        curr_table.hash_table[i] = NULL;
    }

    return curr_table;
}

void hash_table_insert(hash_table_t *inventory, template_t T_key, void *key, template_t T_value, void *value)
{
    int index = generate_hash_code(T_key, key, inventory->size);

    if (inventory->hash_table[index] != NULL)
    {
        if (check_two_equal_value(inventory->hash_table[index]->T_key, inventory->hash_table[index]->key, T_key, key))
        {
            return;
        }
    }

    key = new_bucket_element(inventory, T_key, key);
    value = new_bucket_element(inventory, T_value, value);

    bucket_t *container = new_bucket(T_key, key, T_value, value);
    inventory->allocated_mem += (int)sizeof(bucket_t);

    if (inventory->number_of_buckets == 0)
    {
        inventory->first_index = index;
        inventory->last_index = index;
    }

    if (inventory->hash_table[index] == NULL)
    {
        inventory->hash_table[index] = container;
    }
    else
    {
        container->next = inventory->hash_table[index];
        inventory->hash_table[index] = container;
    }

    if (index < inventory->first_index)
    {
        inventory->first_index = index;
    }
    else if (index > inventory->last_index)
    {
        inventory->last_index = index;
    }

    inventory->number_of_buckets++;
}

bucket_t *hash_table_lookup(hash_table_t *inventory, template_t T_key, void *key)
{
    int index = generate_hash_code(T_key, key, inventory->size);
    bucket_t *temp = inventory->hash_table[index];

    while (temp != NULL && !(check_two_equal_value(temp->T_key, temp->key, T_key, key)))
    {
        temp = temp->next;
    }

    return temp;
}

 void hash_table_delete_bucket(hash_table_t *inventory, template_t T_key, void *key)
 {
     int index = generate_hash_code(T_key, key, inventory->size);

     bucket_t *prev = NULL;
     bucket_t *curr = inventory->hash_table[index];

     while (curr != NULL && !(check_two_equal_value(curr->T_key, curr->key, T_key, key)))
     {
         prev = curr;
         curr = curr->next;
     }

     if (prev == NULL)
     {
         inventory->hash_table[index] = curr->next;
     }
     else
     {
         prev->next = curr->next;
     }

     inventory->allocated_mem -= (get_bytes(curr->T_key, curr->key) +
                                  get_bytes(curr->T_value,  curr->value));
     free_bucket(curr);
     inventory->allocated_mem -= (int)sizeof(bucket_t);
 }

void hash_table_clear(hash_table_t *inventory)
{
    bucket_t **curr = NULL;
    bucket_t **curr_next = NULL;

    for (int i = inventory->first_index; i <= inventory->last_index; i++)
    {
        curr = &inventory->hash_table[i];

        if (*curr == NULL)
        {
            continue;
        }

        while ((*curr)->next != NULL)
        {
            curr_next = &((*curr)->next);
            inventory->allocated_mem -= (get_bytes((*curr)->T_key, (*curr)->key) +
                                         get_bytes((*curr)->T_value,  (*curr)->value));
            free_bucket(*curr);
            inventory->allocated_mem -= (int)sizeof(bucket_t);
            curr = curr_next;
        }

        inventory->allocated_mem -= (get_bytes((*curr)->T_key, (*curr)->key) +
                                     get_bytes((*curr)->T_value,  (*curr)->value));
        free_bucket(*curr);
        inventory->allocated_mem -= (int)sizeof(bucket_t);
    }
}

void hash_table_free(hash_table_t *inventory)
{
    hash_table_clear(inventory);
    free(inventory->hash_table);

    inventory->allocated_mem -= (int)sizeof(bucket_t*) * (size_t)inventory->size;
    inventory->size = -1;
    inventory->number_of_buckets = -1;
    inventory->hash_table = NULL;
    inventory->first_index = -1;
    inventory->last_index = -1;
}

void hash_table_print(hash_table_t *inventory, const char *beginning, const char *end)
{
    if (inventory->hash_table == NULL)
    {
        return;
    }

    int i;
    bucket_t *temp = NULL;

    printf("%s|", beginning);
    for (i = inventory->first_index; i < inventory->last_index; i++)
    {
        temp = inventory->hash_table[i];

        while (temp != NULL)
        {
            printf("(%d) ", i);
            print_t(temp->T_key, temp->key, "", ":");
            print_t(temp->T_value, temp->value, " ", ", ");
            temp = temp->next;
        }
    }

    temp = inventory->hash_table[i];

    if (temp == NULL)
    {
        printf("|%s", end);
        return;
    }

    while (temp->next != NULL)
    {
        printf("(%d) ", i);
        print_t(temp->T_key, temp->key, "", ":");
        print_t(temp->T_value, temp->value, " ", ", ");
        temp = temp->next;
    }

    printf("(%d) ", i);
    print_t(temp->T_key, temp->key, "", ":");
    print_t(temp->T_value, temp->value, " ", "|");
    printf("%s", end);
}
