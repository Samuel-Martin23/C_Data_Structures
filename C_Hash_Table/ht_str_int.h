#ifndef HT_STR_INT_H
#define HT_STR_INT_H

#include <stdarg.h>

#include "hash_table.h"

HashTable *hash_table_init_str_int(void);
HashTable *hash_table_init_str_int_args(size_t num_pairs, ...);
void hash_table_insert_str_int(HashTable *ht, const char *key, int value);
void hash_table_delete_str_int(HashTable *ht, const char *key);
int hash_table_find_str_int(HashTable *ht, const char *key);
bool hash_table_iterate_str_int(HashTableIterator *iter, const char **key, int *value);

#endif /* HT_STR_INT_H */
