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

HashTable *hash_table_init_str_int_args(size_t num_pairs, ...)
{
    HashTable *ht = hash_table_init_str_int();

    va_list args;
    va_start(args, num_pairs);

    for (size_t i = 0; i < num_pairs; i++)
    {
        hash_table_insert_str_int(ht, va_arg(args, char*), va_arg(args, int));
    }

    va_end(args);

    return ht;
}

void hash_table_insert_str_int(HashTable *ht, const char *key, int value)
{
    hash_table_insert(ht, (void*)key, &value);
}

void hash_table_delete_str_int(HashTable *ht, const char *key) {hash_table_delete(ht, (void*)key);}

int hash_table_find_str_int(HashTable *ht, const char *key)
{
    void *value = hash_table_find(ht, (void*)key);

    if (value == NULL)
    {
        fprintf(stderr, "Bad key\n");
        exit(1);
    }

    return *(int*)value;
}

bool hash_table_iterate_str_int(HashTableIterator *iter, const char **key, int *value)
{
    void *iter_key = NULL;
    void *iter_value = NULL;

    bool is_iterating = hash_table_iterator_iterate(iter, &iter_key, &iter_value);

    if (key)
    {
        *key = (is_iterating) ? (char*)iter_key : "";
    }

    if (value)
    {
        *value = (is_iterating) ? *(int*)iter_value : 0;
    }

    return is_iterating;
}
