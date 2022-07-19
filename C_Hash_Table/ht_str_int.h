#ifndef HT_STR_INT_H
#define HT_STR_INT_H

#include "hash_table.h"

HashTable *hash_table_init_str_int(void);
void hash_table_insert_str_int(HashTable *ht, const char *key_name, int value_age);
void hash_table_delete_str_int(HashTable *ht, const char *key_name);
int hash_table_find_str_int(HashTable *ht, const char *key_name);
bool hash_table_iterate_str_int(HashTableIterator *iter, const char **key_str, int *value_int);

#endif /* HT_STR_INT_H */
