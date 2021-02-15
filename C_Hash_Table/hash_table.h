
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../C_Template/template.h"
#include "../C_Allocation_Metrics/allocation_metrics.h"

typedef struct hash_elem
{
    template_t T_key;
    void *key;
    template_t T_value;
    void *value;
    struct hash_elem *next;
} hash_elem_t;

typedef struct hash_table
{
    int size;
    int capacity;
    hash_elem_t **table;
    int first_index;
    int last_index;
} hash_table_t;

void print_ht_size(hash_table_t *ht);

hash_table_t hash_table_init(int capacity);
void hash_table_insert(hash_table_t *ht, template_t T_key, template_t T_value, ...);
hash_elem_t *hash_table_lookup(hash_table_t *ht, template_t T_key, ...);
void hash_table_delete_elem(hash_table_t *ht, template_t T_key, ...);
void hash_table_copy(hash_table_t *ht_dest, hash_table_t *ht_src);
void hash_table_clear(hash_table_t *ht);
void hash_table_free(hash_table_t *ht);
void hash_table_print(hash_table_t *ht, const char *beginning, const char *end);


#endif /* HASH_TABLE_H */
