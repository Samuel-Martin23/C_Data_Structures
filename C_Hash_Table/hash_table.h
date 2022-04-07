#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct hash_table hash_table_t;

typedef size_t (*ht_get_hash_value)(void *key);
typedef bool (*ht_equal_keys)(void *key_1, void *key_2);
typedef void (*ht_print_slot)(void *key, void *value);
typedef void (*ht_free_pair)(void *key, void *value);

hash_table_t *hash_table_init(ht_get_hash_value get_hash_value, ht_equal_keys equal_keys, ht_print_slot print_slot,
                                ht_free_pair free_pair);
void hash_table_insert(hash_table_t *ht, void *key, void *value);
void *hash_table_find(hash_table_t *ht, void *key);
void hash_table_delete(hash_table_t *ht, void *key);
bool hash_table_iterate(hash_table_t *ht, void **key, void **value);
void hash_table_print(hash_table_t *ht);
void hash_table_free(hash_table_t *ht);

#endif /* HASH_TABLE_H */
