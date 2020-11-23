
#ifndef HASH_TABLE_H
#define HASH_TABLE_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

typedef enum Data_Type {INT, DOUBLE, FLOAT, CHAR, STR} Type;

typedef struct Bucket
{
    Type T_Key;
    void *key;
    Type T_Value;
    void *value;
    struct Bucket *next;
} Bucket_T;

typedef struct Hash_Table
{
    int size;
    int number_of_buckets;
    Bucket_T **hash_table;
    int first_index;
    int last_index;
    int allocated_mem;
} Hash_Table_T;

#define IntCastVoid(value) INT, &(int){value}
#define DoubleCastVoid(value) DOUBLE, &(double){value}
#define FloatCastVoid(value) FLOAT, &(float){value}
#define CharCastVoid(value) CHAR, &(char){value}
#define StrCastVoid(value) STR, (char*){value}

int VoidCastInt(void *value);
double VoidCastDouble(void *value);
float VoidCastFloat(void *value);
char VoidCastChar(void *value);
const char *VoidCastStr(void *value);

int DataTypeSize(Type T, void *value);
bool CheckEqualValue(Type T_Key_1, void *key_1, Type T_Key_2, void *key_2);
Hash_Table_T InitHashTable(int size);
void FreeHashTable(Hash_Table_T *inventory);
Bucket_T *NewBucket(Type T_Key, void *key, Type T_Value, void *value);
void FreeBucket(Bucket_T **container);
void *NewBucketElement(Hash_Table_T *inventory, Type T, void *element);
int GenerateHashCode(Type T, void *key, int hash_table_size);
void InsertHashTable(Hash_Table_T *inventory, Type T_Key, void *key, Type T_Value, void *value);
Bucket_T *LookupHashTable(Hash_Table_T *inventory, Type T_Key, void *key);
void DeleteHashTableBucket(Hash_Table_T *inventory, Type T_Key, void *key);
void ClearHashTable(Hash_Table_T *inventory);
void PrintT(Type T, void *value, const char *beginning, const char *end);
void PrintHashTable(Hash_Table_T *inventory, const char *beginning, const char *end);
void PrintAllocatedMemory(Hash_Table_T *inventory);


#endif //HASH_TABLE_H