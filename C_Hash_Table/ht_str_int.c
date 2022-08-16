#include "ht_str_int.h"

static size_t get_hash_str(void *key)
{
    size_t hash_value = 0;
    const char *str_key = (const char*)key;
    size_t str_length = (strlen(str_key)) / 2;

    for (size_t i = 0; i < str_length; i++)
    {
        hash_value = 31 * hash_value + (unsigned char)str_key[i];
    }

    return hash_value;
}

static bool str_keys_equal(void *key_1, void *key_2) {return (strncmp((const char*)key_1, (const char*)key_2, 50) == 0);}

static void str_int_print(void *key, void *value)
{
    printf("\"%s\": %d", (const char*)key, *(int*)value);
}

static void *alloc_str(void *value)
{
    char *s = (char*)value;
    size_t size = strlen(s) + 1;
    char *str = malloc(sizeof(char) * size);

    if (str != NULL)
    {
        memcpy(str, s, size);
    }

    return str;
}

static void *alloc_int(void *value)
{
    int *alloc_value = malloc(sizeof(int));
    *alloc_value = *(int*)value;
    return alloc_value;
}

static void str_int_free(void *key, void *value) {free(key); free(value);}

HashTable *hash_table_init_str_int(void)
{
    return hash_table_init(get_hash_str, str_keys_equal, str_int_print, alloc_str, alloc_int, str_int_free);
}

void hash_table_insert_str_int(HashTable *ht, const char *key_name, int value_age)
{
    hash_table_insert(ht, (void*)key_name, &value_age);
}

void hash_table_delete_str_int(HashTable *ht, const char *key_name) {hash_table_delete(ht, (void*)key_name);}

int hash_table_find_str_int(HashTable *ht, const char *key_name)
{
    void *value = hash_table_find(ht, (void*)key_name);

    if (value == NULL)
    {
        fprintf(stderr, "Bad key\n");
        exit(1);
    }

    return *(int*)value;
}

bool hash_table_iterate_str_int(HashTableIterator *iter, const char **key_str, int *value_int)
{
    void *key = NULL;
    void *value = NULL;

    bool is_next = hash_table_iterator_iterate(iter, &key, &value);

    if (is_next)
    {
        *key_str = (char*)key;
        *value_int = *(int*)value;
    }
    else
    {
        *key_str = "";
        *value_int = 0;
    }

    return is_next;
}
