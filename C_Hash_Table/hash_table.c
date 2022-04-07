#include "hash_table.h"

typedef struct table_slot
{
    void *key;
    void *value;
    struct table_slot *next;
} table_slot_t;

typedef struct hash_table
{
    size_t size;
    size_t capacity;
    table_slot_t **table;

    size_t iter_index;
    table_slot_t *iter_slot;

    ht_get_hash_value get_hash_value;
    ht_equal_keys equal_keys;
    ht_print_slot print_slot;
    ht_free_pair free_pair;
} hash_table_t;

static table_slot_t *hash_table_lookup_slot(hash_table_t *ht, void *key)
{
    size_t index = ht->get_hash_value(key) % ht->capacity;
    table_slot_t *slot = ht->table[index];

    while (slot != NULL && !(ht->equal_keys(key, slot->key)))
    {
        slot = slot->next;
    }

    return slot;
}

static table_slot_t *alloc_table_slot(void *key, void *value)
{
    table_slot_t *slot = malloc(sizeof(table_slot_t));

    slot->key = key;
    slot->value = value;
    slot->next = NULL;

    return slot;
}

static void free_table_slot(hash_table_t *ht, table_slot_t *slot)
{
    ht->free_pair(slot->key, slot->value);
    free(slot);
}

hash_table_t *hash_table_init(ht_get_hash_value get_hash_value, ht_equal_keys equal_keys, ht_print_slot print_slot,
                                ht_free_pair free_pair)
{
    hash_table_t *ht = malloc(sizeof(hash_table_t));

    ht->capacity = 31;
    ht->size = 0;

    ht->table = calloc(ht->capacity, sizeof(table_slot_t));

    ht->iter_index = 0;
    ht->iter_slot = NULL;

    ht->get_hash_value = get_hash_value;
    ht->equal_keys = equal_keys;
    ht->print_slot = print_slot;
    ht->free_pair = free_pair;

    return ht;
}

void hash_table_insert(hash_table_t *ht, void *key, void *value)
{
    table_slot_t *searched_slot = hash_table_lookup_slot(ht, key);

    // If the key is already in the hash table, free the key and the value
    // that was already there, since the incoming key and value are allocated.
    if (searched_slot)
    {
        ht->free_pair(searched_slot->key, searched_slot->value);
        searched_slot->key = key;
        searched_slot->value = value;
        return;
    }

    table_slot_t *slot = alloc_table_slot(key, value);
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

void *hash_table_find(hash_table_t *ht, void *key)
{
    table_slot_t *slot = hash_table_lookup_slot(ht, key);

    return (slot == NULL) ? NULL : slot->value;
}

void hash_table_delete(hash_table_t *ht, void *key)
{
    size_t index = ht->get_hash_value(key) % ht->capacity;

    table_slot_t *prev = NULL;
    table_slot_t *curr = ht->table[index];

    if (curr == NULL)
    {
        return;
    }

    while (curr != NULL && !(ht->equal_keys(key, curr->key)))
    {
        prev = curr;
        curr = curr->next;
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

bool hash_table_iterate(hash_table_t *ht, void **key, void **value)
{
    if (ht->size == 0)
    {
        return false;
    }

    // Where is the next slot at?
    while (ht->iter_slot == NULL)
    {
        ht->iter_slot = ht->table[ht->iter_index];

        // We have reached the end.
        if (ht->iter_index == ht->capacity)
        {
            // Reset everything back to the beginning.
            ht->iter_index = 0;
            ht->iter_slot = NULL;
            *key = NULL;
            *value = NULL;

            return false;
        }

        ht->iter_index++;
    }

    // In case "key" is NULL.
    if (key)
    {
        *key = ht->iter_slot->key;
    }

    // In case "value" is NULL.
    if (value)
    {
        *value = ht->iter_slot->value;
    }

    ht->iter_slot = ht->iter_slot->next;

    return true;
}

void hash_table_print(hash_table_t *ht)
{
    if (ht->size == 0)
    {
        printf("{}\n");
        return;
    }

    table_slot_t *slot = NULL;

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
            ht->print_slot(slot->key, slot->value);
            printf(", ");
            slot = slot->next;
        }
    }

    printf("\b\b}\n");
}

void hash_table_free(hash_table_t *ht)
{
    table_slot_t *slot = NULL;
    table_slot_t *slot_succeeding = NULL;

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
