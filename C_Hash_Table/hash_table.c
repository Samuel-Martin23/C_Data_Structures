#include "hash_table.h"

typedef struct TableSlot
{
    void *key;
    void *value;
    struct TableSlot *next;
} TableSlot;

typedef struct HashTable
{
    size_t size;
    size_t capacity;
    TableSlot **table;

    ht_get_hash_value get_hash_value;
    ht_equal_keys equal_keys;
    ht_print_pair print_pair;
    ht_alloc_key alloc_key;
    ht_alloc_value alloc_value;
    ht_free_pair free_pair;
} HashTable;

typedef struct HashTableIterator
{
    size_t size;
    size_t capacity;
    size_t index;
    TableSlot *slot;
    TableSlot **table;
} HashTableIterator;

static TableSlot *hash_table_lookup_slot(HashTable *ht, void *key)
{
    size_t index = ht->get_hash_value(key) % ht->capacity;
    TableSlot *slot = ht->table[index];

    while (slot != NULL && !(ht->equal_keys(key, slot->key)))
    {
        slot = slot->next;
    }

    return slot;
}

static TableSlot *alloc_table_slot(void *key, void *value)
{
    TableSlot *slot = malloc(sizeof(TableSlot));

    slot->key = key;
    slot->value = value;
    slot->next = NULL;

    return slot;
}

static void free_table_slot(HashTable *ht, TableSlot *slot)
{
    ht->free_pair(slot->key, slot->value);
    free(slot);
}

HashTable *hash_table_init(ht_get_hash_value get_hash_value, ht_equal_keys equal_keys, ht_print_pair print_pair,
                           ht_alloc_key alloc_key, ht_alloc_value alloc_value, ht_free_pair free_pair)
{
    HashTable *ht = malloc(sizeof(HashTable));

    ht->capacity = 31;
    ht->size = 0;

    ht->table = calloc(ht->capacity, sizeof(TableSlot));

    ht->get_hash_value = get_hash_value;
    ht->equal_keys = equal_keys;
    ht->print_pair = print_pair;
    ht->alloc_key = alloc_key;
    ht->alloc_value = alloc_value;
    ht->free_pair = free_pair;

    return ht;
}

void hash_table_insert(HashTable *ht, void *key, void *value)
{
    TableSlot *searched_slot = hash_table_lookup_slot(ht, key);

    // If the key is already in the hash table, free the key and the value
    // that was already there, since the incoming key and value are allocated.
    if (searched_slot)
    {
        ht->free_pair(searched_slot->key, searched_slot->value);
        searched_slot->key = ht->alloc_key(key);
        searched_slot->value = ht->alloc_value(value);
        return;
    }

    TableSlot *slot = alloc_table_slot(ht->alloc_key(key), ht->alloc_value(value));
    size_t index = ht->get_hash_value(key) % ht->capacity;

    if (ht->table[index] == NULL)
    {
        ht->table[index] = slot;
    }
    else
    {
        slot->next = ht->table[index];
        ht->table[index] = slot;
    }

    ht->size++;
}

void *hash_table_find(HashTable *ht, void *key)
{
    TableSlot *slot = hash_table_lookup_slot(ht, key);

    return (slot == NULL) ? NULL : slot->value;
}

void hash_table_delete(HashTable *ht, void *key)
{
    size_t index = ht->get_hash_value(key) % ht->capacity;
    TableSlot *prev = NULL;
    TableSlot *curr = ht->table[index];

    while (curr != NULL && !(ht->equal_keys(key, curr->key)))
    {
        prev = curr;
        curr = curr->next;
    }

    // If we hashed to a NULL index, or we looked through
    // the entire list and did not find the key.
    if (curr == NULL)
    {
        return;
    }

    if (prev == NULL)
    {
        ht->table[index] = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }

    free_table_slot(ht, curr);
}

void hash_table_print(HashTable *ht)
{
    if (ht->size == 0)
    {
        printf("{}\n");
        return;
    }

    TableSlot *slot = NULL;

    printf("{");

    for (size_t i = 0; i < ht->capacity; i++)
    {
        slot = ht->table[i];

        if (slot == NULL)
        {
            continue;
        }

        while (slot != NULL)
        {
            ht->print_pair(slot->key, slot->value);
            printf(", ");
            slot = slot->next;
        }
    }

    printf("\b\b}\n");
}

void hash_table_free(HashTable *ht)
{
    TableSlot *slot = NULL;
    TableSlot *slot_succeeding = NULL;

    for (size_t i = 0; i < ht->capacity; i++)
    {
        slot = ht->table[i];

        if (slot == NULL)
        {
            continue;
        }

        while (slot != NULL)
        {
            slot_succeeding = slot->next;
            free_table_slot(ht, slot);
            slot = slot_succeeding;
        }
    }

    free(ht->table);
    free(ht);
}

HashTableIterator *hash_table_iterator_init(HashTable *ht)
{
    HashTableIterator *iter = malloc(sizeof(HashTableIterator));

    iter->size = ht->size;
    iter->capacity = ht->capacity;
    iter->index = 0;
    iter->slot = NULL;
    iter->table = ht->table;

    return iter;
}

bool hash_table_iterator_iterate(HashTableIterator *iter, void **key, void **value)
{
    if (iter->size == 0)
    {
        return false;
    }

    // Where is the next slot at?
    while (iter->slot == NULL)
    {
        iter->slot = iter->table[iter->index];

        // We have reached the end.
        if (iter->index == iter->capacity)
        {
            // Reset everything back to the beginning.
            iter->index = 0;
            iter->slot = NULL;
            *key = NULL;
            *value = NULL;

            return false;
        }

        iter->index++;
    }

    // In case "key" is NULL.
    if (key)
    {
        *key = iter->slot->key;
    }

    // In case "value" is NULL.
    if (value)
    {
        *value = iter->slot->value;
    }

    iter->slot = iter->slot->next;

    return true;
}

void hash_table_iterator_free(HashTableIterator *iter)
{
    free(iter);
}
