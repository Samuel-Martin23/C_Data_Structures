
#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../Template/template.h"

typedef struct Bucket
{
    Template T_Key;
    void *key;
    Template T_Value;
    void *value;
    struct Bucket *next;
} Bucket_t;

typedef struct Hash_Table
{
    int size;
    int number_of_buckets;
    Bucket_t **hash_table;
    int first_index;
    int last_index;
    int allocated_mem;
} Hash_Table_t;


Hash_Table_t InitHashTable(int size);
void FreeHashTable(Hash_Table_t *inventory);
void InsertHashTable(Hash_Table_t *inventory, Template T_Key, void *key, Template T_Value, void *value);
Bucket_t *LookupHashTable(Hash_Table_t *inventory, Template T_Key, void *key);
void DeleteHashTableBucket(Hash_Table_t *inventory, Template T_Key, void *key);
void ClearHashTable(Hash_Table_t *inventory);
void PrintHashTable(Hash_Table_t *inventory, const char *beginning, const char *end);
void PrintAllocatedMemHT(Hash_Table_t *inventory);


#endif //HASH_TABLE_H