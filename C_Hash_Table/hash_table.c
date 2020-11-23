
#include "hash_table.h"

int VoidCastInt(void *value) {return (*(int*)value);}
double VoidCastDouble(void *value) {return (*(double*)value);}
float VoidCastFloat(void *value) {return (*(float*)value);}
char VoidCastChar(void *value) {return (*(const char*)value);}
const char *VoidCastStr(void *value) {return ((const char*)value);}

int DataTypeSize(Type T, void *value)
{
    int size = -1;

    switch (T)
    {
        case INT:
            size = sizeof(int);
            break;
        case DOUBLE:
            size = sizeof(double);
            break;
        case FLOAT:
            size = sizeof(float);
            break;
        case CHAR:
            size = sizeof(char) + 1;
            break;
        case STR:
            {
                const char *str_value = VoidCastStr(value);
                size = (int)(sizeof(char) * strlen(str_value) + 1);
            }
            break;
        default:
            break;
    }

    return size;
}

bool CheckEqualValue(Type T_Key_1, void *key_1, Type T_Key_2, void *key_2)
{
    if (T_Key_1 != T_Key_2)
    {
        return false;
    }

    switch (T_Key_1)
    {
        case INT:
            return (VoidCastInt(key_1) == VoidCastInt(key_2));
        case DOUBLE:
            return (VoidCastDouble(key_1) == VoidCastDouble(key_2));
        case FLOAT:
            return (VoidCastFloat(key_1) == VoidCastFloat(key_2));
        case CHAR:
            return (VoidCastChar(key_1) == VoidCastChar(key_2));
        case STR:
            return (!(strcmp(VoidCastStr(key_1), VoidCastStr(key_2))));
        default:
            break;
    }

    return false;
}

Hash_Table_T InitHashTable(int size)
{
    Hash_Table_T curr_table;
    curr_table.size = size;
    curr_table.number_of_buckets = 0;
    curr_table.hash_table = malloc(sizeof(Bucket_T*) * curr_table.size);
    curr_table.first_index = 0;
    curr_table.last_index = 0;
    curr_table.allocated_mem = (int)sizeof(Bucket_T*) * curr_table.size;

    for (int i = 0; i < curr_table.size; i++)
    {
        curr_table.hash_table[i] = NULL;
    }

    return curr_table;
}

void FreeHashTable(Hash_Table_T *inventory)
{
    ClearHashTable(inventory);
    free(inventory->hash_table);

    inventory->allocated_mem -= (int)sizeof(Bucket_T*) * inventory->size;
    inventory->size = -1;
    inventory->number_of_buckets = -1;
    inventory->hash_table = NULL;
    inventory->first_index = -1;
    inventory->last_index = -1;
}

Bucket_T *NewBucket(Type T_Key, void *key, Type T_Value, void *value)
{
    Bucket_T *container = malloc(sizeof(Bucket_T));
    container->T_Key = T_Key;
    container->key = key;
    container->T_Value = T_Value;
    container->value = value;
    container->next = NULL;

    return container;
}

void FreeBucket(Bucket_T **container)
{
    free((*container)->key);
    free((*container)->value);
    free(*container);
    *container = NULL;
}

void *NewBucketElement(Hash_Table_T *inventory, Type T, void *element)
{
    switch (T)
    {
        case INT:
            {
                int *allocated_key = malloc(sizeof(int));
                inventory->allocated_mem += (int)sizeof(int);
                *allocated_key = VoidCastInt(element);
                element = allocated_key;
            }
            break;
        case DOUBLE:
            {
                double *allocated_key = malloc(sizeof(double));
                inventory->allocated_mem += (int)sizeof(double);
                *allocated_key = VoidCastDouble(element);
                element = allocated_key;
            }
            break;
        case FLOAT:
            {
                float *allocated_key = malloc(sizeof(float));
                inventory->allocated_mem += (int)sizeof(float);
                *allocated_key = VoidCastFloat(element);
                element = allocated_key;
            }
            break;
        case CHAR:
            {
                char key_data = VoidCastChar(element);
                const char *allocated_key = malloc(sizeof(char) * 2);

                inventory->allocated_mem += (int)(sizeof(char) * 2);
                memcpy((char*)allocated_key, &key_data, 1);
                *((char*)allocated_key + 1) = '\0';

                element = (char*)allocated_key;
            }
            break;
        case STR:
            {
                const char *key_data = VoidCastStr(element);
                int str_length = (int)strlen(key_data);
                const char *allocated_key = malloc(sizeof(char) * (str_length + 1));

                inventory->allocated_mem += (int)(sizeof(char) * (str_length + 1));
                memcpy((char*)allocated_key, key_data, str_length);
                *((char*)allocated_key + str_length) = '\0';

                element = (char*)allocated_key;
            }
            break;
        default:
            break;
    }

    return element;
}

int GenerateHashCode(Type T_Key, void *key, int hash_table_size)
{
    int hash_code = -1;

    switch (T_Key)
    {
        case INT:
            hash_code = (VoidCastInt(key) % hash_table_size);
            break;
        case DOUBLE:
            hash_code = (int)(fmod(VoidCastDouble(key), (double)hash_table_size));
            break;
        case FLOAT:
            hash_code = (int)(fmod(VoidCastFloat(key), (double)hash_table_size));
            break;
        case CHAR:
            hash_code = (VoidCastChar(key) % hash_table_size);
            break;
        case STR:
            {
                int i;
                const char *word = VoidCastStr(key);
                int str_length = (int)strlen(word);
                hash_code = 0;

                for (i = 0; i < str_length; i++)
                {
                    hash_code += word[i] * (int) pow(31, str_length - (i + 1));
                }

                hash_code = (hash_code + word[i]) % hash_table_size;
            }
            break;
        default:
            break;
    }

    return hash_code;
}

void InsertHashTable(Hash_Table_T *inventory, Type T_Key, void *key, Type T_Value, void *value)
{
    key = NewBucketElement(inventory, T_Key, key);
    value = NewBucketElement(inventory, T_Value, value);

    Bucket_T *container = NewBucket(T_Key, key, T_Value, value);
    inventory->allocated_mem += (int)sizeof(Bucket_T);

    int index = GenerateHashCode(container->T_Key, container->key, inventory->size);

    if (inventory->number_of_buckets == 0)
    {
        inventory->first_index = index;
        inventory->last_index = index;
    }

    if (inventory->hash_table[index] == NULL)
    {
        inventory->hash_table[index] = container;
    }
    else if (strcmp(inventory->hash_table[index]->key, container->key) == 0)
    {
        return;
    }
    else
    {
        container->next = inventory->hash_table[index];
        inventory->hash_table[index] = container;
    }

    if (index < inventory->first_index)
    {
        inventory->first_index = index;
    }
    else if (index > inventory->last_index)
    {
        inventory->last_index = index;
    }

    inventory->number_of_buckets++;
}

Bucket_T *LookupHashTable(Hash_Table_T *inventory, Type T_Key, void *key)
{
    int index = GenerateHashCode(T_Key, key, inventory->size);
    Bucket_T *temp = inventory->hash_table[index];

    while (temp != NULL && !(CheckEqualValue(temp->T_Key, temp->key, T_Key, key)))
    {
        temp = temp->next;
    }

    return temp;
}

 void DeleteHashTableBucket(Hash_Table_T *inventory, Type T_Key, void *key)
 {
     Bucket_T *curr = LookupHashTable(inventory, T_Key, key);


     if (curr == NULL)
     {
         return;
     }

     int index = GenerateHashCode(curr->T_Key, curr->key, inventory->size);
     Bucket_T *prev = NULL;

     while (curr != NULL && !(CheckEqualValue(curr->T_Key, curr->key, T_Key, key)))
     {
         prev = curr;
         curr = curr->next;
     }

     if (prev == NULL)
     {
         inventory->hash_table[index] = curr->next;
     }
     else
     {
         prev->next = curr->next;
     }

     inventory->allocated_mem -= (DataTypeSize(curr->T_Key, curr->key) +
                                  DataTypeSize(curr->T_Value,  curr->value));
     FreeBucket(&curr);
     inventory->allocated_mem -= sizeof(Bucket_T);
 }

void ClearHashTable(Hash_Table_T *inventory)
{
    Bucket_T **curr = NULL;
    Bucket_T **curr_next = NULL;

    for (int i = inventory->first_index; i <= inventory->last_index; i++)
    {
        curr = &inventory->hash_table[i];

        if (*curr == NULL)
        {
            continue;
        }

        while ((*curr)->next != NULL)
        {
            curr_next = &((*curr)->next);
            inventory->allocated_mem -= (DataTypeSize((*curr)->T_Key, (*curr)->key) +
                                         DataTypeSize((*curr)->T_Value,  (*curr)->value));
            FreeBucket(curr);
            inventory->allocated_mem -= (int)sizeof(Bucket_T);
            curr = curr_next;
        }

        inventory->allocated_mem -= (DataTypeSize((*curr)->T_Key, (*curr)->key) +
                                     DataTypeSize((*curr)->T_Value,  (*curr)->value));
        FreeBucket(curr);
        inventory->allocated_mem -= (int)sizeof(Bucket_T);
    }
}

void PrintT(Type T, void *value, const char *beginning, const char *end)
{
    switch(T)
    {
        case INT:
            printf("%s%d%s", beginning, VoidCastInt(value), end);
            break;
        case DOUBLE:
            printf("%s%.2f%s", beginning, VoidCastDouble(value), end);
            break;
        case FLOAT:
            printf("%s%.2f%s", beginning, VoidCastFloat(value), end);
            break;
        case CHAR:
            printf("%s\'%c\'%s", beginning, VoidCastChar(value), end);
            break;
        case STR:
            printf("%s\"%s\"%s", beginning, VoidCastStr(value), end);
            break;
        default:
            break;
    }
}

void PrintHashTable(Hash_Table_T *inventory, const char *beginning, const char *end)
{
    if (inventory->hash_table == NULL)
    {
        return;
    }

    int i;
    Bucket_T *temp = NULL;

    printf("%s|", beginning);
    for (i = inventory->first_index; i < inventory->last_index; i++)
    {
        temp = inventory->hash_table[i];

        while (temp != NULL)
        {
            printf("(%d) ", i);
            PrintT(temp->T_Key, temp->key, "", ":");
            PrintT(temp->T_Value, temp->value, " ", ", ");
            temp = temp->next;
        }
    }

    temp = inventory->hash_table[i];

    if (temp == NULL)
    {
        printf("|%s", end);
        return;
    }

    while (temp->next != NULL)
    {
        printf("(%d) ", i);
        PrintT(temp->T_Key, temp->key, "", ":");
        PrintT(temp->T_Value, temp->value, " ", ", ");
        temp = temp->next;
    }

    printf("(%d) ", i);
    PrintT(temp->T_Key, temp->key, "", ":");
    PrintT(temp->T_Value, temp->value, " ", "|");
    printf("%s", end);
}

void PrintAllocatedMemory(Hash_Table_T *inventory)
{
    printf("Bytes Allocated: %d\n", inventory->allocated_mem);
}