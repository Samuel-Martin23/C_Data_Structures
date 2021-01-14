
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../C_Template/template.h"

typedef struct bucket
{
    template_t T_key;
    void *key;
    template_t T_value;
    void *value;
    struct bucket *next;
} bucket_t;

typedef struct hash_table
{
    int size;
    int number_of_buckets;
    bucket_t **hash_table;
    int first_index;
    int last_index;
    int allocated_mem;
} hash_table_t;

void print_allocated_mem_ht(hash_table_t *inventory);
void print_num_of_buckets(hash_table_t *inventory);

hash_table_t hash_table_init(int size);
void hash_table_insert(hash_table_t *inventory, template_t T_key, void *key, template_t T_value, void *value);
bucket_t *hash_table_lookup(hash_table_t *inventory, template_t T_key, void *key);
void hash_table_delete_bucket(hash_table_t *inventory, template_t T_key, void *key);
void hash_table_clear(hash_table_t *inventory);
void hash_table_free(hash_table_t *inventory);
void hash_table_print(hash_table_t *inventory, const char *beginning, const char *end);


#endif /* HASH_TABLE_H */
