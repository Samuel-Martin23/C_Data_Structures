#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct HashTable HashTable;
typedef struct HashTableIterator HashTableIterator;

typedef size_t (*ht_get_hash_value)(void *key);
typedef bool (*ht_equal_keys)(void *key_1, void *key_2);
typedef void (*ht_print_pair)(void *key, void *value);
typedef void *(*ht_alloc_key)(void *key);
typedef void *(*ht_alloc_value)(void *value);
typedef void (*ht_free_pair)(void *key, void *value);

HashTable *hash_table_init(ht_get_hash_value get_hash_value, ht_equal_keys equal_keys, ht_print_pair print_pair,
                           ht_alloc_key alloc_key, ht_alloc_value alloc_value, ht_free_pair free_pair);
void hash_table_insert(HashTable *ht, void *key, void *value);
void *hash_table_find(HashTable *ht, void *key);
void hash_table_delete(HashTable *ht, void *key);
void hash_table_print(HashTable *ht);
void hash_table_free(HashTable *ht);

HashTableIterator *hash_table_iterator_init(HashTable *ht);
bool hash_table_iterator_iterate(HashTableIterator *iter, void **key, void **value);
void hash_table_iterator_free(HashTableIterator *iter);

#endif /* HASH_TABLE_H */
