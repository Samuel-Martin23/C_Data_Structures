
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
        case BOOL:
            return (void_cast_bool(key_1) == void_cast_bool(key_2));
        case NONE:
            break;
    }

    return false;
}

hash_elem_t *new_element(template_t T_Key, void *key, template_t T_Value, void *value)
{
    size_t number_of_bytes = sizeof(hash_elem_t);
    hash_elem_t *item = malloc(number_of_bytes);

    item->T_key = T_Key;
    item->key = key;
    item->T_value = T_Value;
    item->value = value;
    item->next = NULL;

    mem_usage.allocated += (u_int32_t)number_of_bytes;

    return item;
}

void free_elem(hash_elem_t *item)
{
    free(item->key);
    free(item->value);
    free(item);
    item = NULL;
    mem_usage.freed += (u_int32_t)sizeof(hash_elem_t);
}

void *new_element_value(template_t T, void **value)
{
    switch (T)
    {
        case INT:
            {
                size_t number_of_bytes = get_bytes(T, *value);
                int *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_int(*value);
                *value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case DOUBLE:
            {
                size_t number_of_bytes = get_bytes(T, *value);
                double *allocated_value = malloc(number_of_bytes);

                *allocated_value = void_cast_double(*value);
                *value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case FLOAT:
            {
                size_t number_of_bytes = get_bytes(T, *value);
                float *allocated_key = malloc(number_of_bytes);

                *allocated_key = void_cast_float(*value);
                *value = allocated_key;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case CHAR:
            {
                size_t number_of_bytes = get_bytes(T, *value);
                char key_data = void_cast_char(*value);
                const char *allocated_value = malloc(number_of_bytes);

                memcpy((char*)allocated_value, &key_data, 1);
                *value = (char*)allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case STR:
            {
                const char *key_data = void_cast_str(*value);
                size_t number_of_bytes = get_bytes(T, (void*)key_data);
                const char *allocated_value = malloc(number_of_bytes);

                mem_usage.allocated += (u_int32_t)number_of_bytes;

                number_of_bytes--;

                memcpy((char*)allocated_value, key_data, number_of_bytes);
                *((char*)allocated_value + number_of_bytes) = '\0';

                *value = (char*)allocated_value;
            }
            break;
        case BOOL:
            {
                size_t number_of_bytes = get_bytes(T, *value);
                bool *allocated_value = malloc(number_of_bytes);
            
                *allocated_value = void_cast_bool(*value);
                *value = allocated_value;

                mem_usage.allocated += (u_int32_t)number_of_bytes;
            }
            break;
        case NONE:
            break;
    }

    return value;
}

int generate_hash_code(template_t T_Key, void *key, int capacity)
{
    int hash_code = 0;

    switch (T_Key)
    {
        case INT:
            hash_code = (void_cast_int(key) % capacity);
            break;
        case DOUBLE:
            hash_code = (int)(fmod(void_cast_double(key), (double)capacity));
            break;
        case FLOAT:
            hash_code = (int)(fmod(void_cast_float(key), (double)capacity));
            break;
        case CHAR:
            hash_code = (void_cast_char(key) % capacity);
            break;
        case STR:
            {
                size_t temp_hash_code = 0;
                const char *word = void_cast_str(key);
                int str_length = ((int)strlen(word)) / 2;

                for (int i = 0; i < str_length; i++)
                {
                    temp_hash_code = 31 * temp_hash_code + (size_t)word[i];
                }

                temp_hash_code %= (size_t)capacity;
                hash_code = (int)temp_hash_code;
            }
            break;
        case BOOL:
            hash_code = (void_cast_bool(key) % capacity);
            break;
        case NONE:
            break;
    }

    return hash_code;
}

void table_insert(hash_table_t *ht, template_t T_key, void *key, template_t T_value, void *value)
{
    int index = generate_hash_code(T_key, key, ht->capacity);

    if (ht->table[index] != NULL)
    {
        if (check_two_equal_value(ht->table[index]->T_key, ht->table[index]->key, T_key, key))
        {
            return;
        }
    }

    new_element_value(T_key, &key);
    new_element_value(T_value, &value);

    hash_elem_t *item = new_element(T_key, key, T_value, value);

    if (ht->size == 0)
    {
        ht->first_index = index;
        ht->last_index = index;
    }

    if (ht->table[index] == NULL)
    {
        ht->table[index] = item;
    }
    else
    {
        item->next = ht->table[index];
        ht->table[index] = item;
    }

    if (index < ht->first_index)
    {
        ht->first_index = index;
    }
    else if (index > ht->last_index)
    {
        ht->last_index = index;
    }

    ht->size++;
}

void print_ht_size(hash_table_t *ht)
{
    printf("Hash Table Size: %d\n", ht->size);
}

hash_table_t hash_table_init(int capacity)
{
    hash_table_t new_ht;
    size_t number_of_bytes = sizeof(hash_elem_t*) * (size_t)capacity;

    new_ht.capacity = capacity;
    new_ht.size = 0;
    new_ht.table = malloc(number_of_bytes);
    new_ht.first_index = 0;
    new_ht.last_index = 0;
    mem_usage.allocated += (u_int32_t)number_of_bytes;

    for (int i = 0; i < new_ht.capacity; i++)
    {
        new_ht.table[i] = NULL;
    }

    return new_ht;
}

void hash_table_insert(hash_table_t *ht, template_t T_key, void *key, template_t T_value, void *value)
{
    convert_2d_str(T_key, &key);
    convert_2d_str(T_value, &value);
    table_insert(ht, T_key, key, T_value, value);
}

hash_elem_t *hash_table_lookup(hash_table_t *ht, template_t T_key, void *key)
{
    convert_2d_str(T_key, &key);
    int index = generate_hash_code(T_key, key, ht->capacity);
    hash_elem_t *item = ht->table[index];

    while (item != NULL && !(check_two_equal_value(item->T_key, item->key, T_key, key)))
    {
        item = item->next;
    }

    return item;
}

void hash_table_delete_elem(hash_table_t *ht, template_t T_key, void *key)
{
    convert_2d_str(T_key, &key);
    int index = generate_hash_code(T_key, key, ht->capacity);

    hash_elem_t *prev = NULL;
    hash_elem_t *curr = ht->table[index];

    while (curr != NULL && !(check_two_equal_value(curr->T_key, curr->key, T_key, key)))
    {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL)
    {
        ht->table[index] = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }

    mem_usage.freed += ((u_int32_t)get_bytes(curr->T_key, curr->key) +
                        (u_int32_t)get_bytes(curr->T_value, curr->value));
    free_elem(curr);
}

void hash_table_copy(hash_table_t *ht_dest, hash_table_t *ht_src)
{
    if (ht_dest->capacity == 0)
    {
        const char *purple = "\033[1;95m";
        const char *white = "\033[1;97m";
        const char *reset = "\033[0m";

        printf("hash_table_copy: %swarning:%s hash table is not initialized%s\n", purple, white, reset);
    }

    for (int i = ht_src->first_index; i <= ht_src->last_index; i++)
    {
        if (ht_src->table[i] == NULL)
        {
            continue;
        }

        table_insert(ht_dest, ht_src->table[i]->T_key,
                    ht_src->table[i]->key,  ht_src->table[i]->T_value,
                    ht_src->table[i]->value);
    }
}

void hash_table_clear(hash_table_t *ht)
{
    hash_elem_t **curr = NULL;
    hash_elem_t **curr_next = NULL;

    for (int i = ht->first_index; i <= ht->last_index; i++)
    {
        curr = &ht->table[i];

        if (*curr == NULL)
        {
            continue;
        }

        while ((*curr)->next != NULL)
        {
            curr_next = &((*curr)->next);
            mem_usage.freed += ((u_int32_t)get_bytes((*curr)->T_key, (*curr)->key) +
                                (u_int32_t)get_bytes((*curr)->T_value, (*curr)->value));
            free_elem(*curr);
            curr = curr_next;
        }

        mem_usage.freed += ((u_int32_t)get_bytes((*curr)->T_key, (*curr)->key) +
                            (u_int32_t)get_bytes((*curr)->T_value, (*curr)->value));
        free_elem(*curr);
    }
}

void hash_table_free(hash_table_t *ht)
{
    hash_table_clear(ht);
    free(ht->table);

    mem_usage.freed += (u_int32_t)(sizeof(hash_elem_t*) * (size_t)ht->capacity);
    ht->capacity = 0;
    ht->size = 0;
    ht->table = NULL;
    ht->first_index = 0;
    ht->last_index = 0;
}

void hash_table_print(hash_table_t *ht, const char *beginning, const char *end)
{
    if (ht->table == NULL || ht->size == 0)
    {
        return;
    }

    int i;
    hash_elem_t *temp = NULL;

    printf("%s|", beginning);
    for (i = ht->first_index; i < ht->last_index; i++)
    {
        temp = ht->table[i];

        while (temp != NULL)
        {
            printf("(%d) ", i);
            print_t(temp->T_key, temp->key, "", ":");
            print_t(temp->T_value, temp->value, " ", ", ");
            temp = temp->next;
        }
    }

    temp = ht->table[i];

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
