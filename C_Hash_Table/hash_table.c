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
    size_t first_index;
    size_t last_index;

    ht_get_hash_value get_hash_value;
    ht_equal_keys equal_keys;
    ht_print_slot print_slot;
    ht_free_pair free_pair;
} hash_table_t;

static table_slot_t *alloc_table_slot(void *key, void *value)
{
    table_slot_t *slot = malloc(sizeof(table_slot_t));

    slot->key = key;
    slot->value = value;
    slot->next = NULL;

    return slot;
}

static void free_table_slot(hash_table_t *ht, table_slot_t **slot)
{
    ht->free_pair((*slot)->key, (*slot)->value);
    free((*slot));
    (*slot) = NULL;
}

hash_table_t *hash_table_init(ht_get_hash_value get_hash_value, ht_equal_keys equal_keys, ht_print_slot print_slot,
                                ht_free_pair free_pair)
{
    hash_table_t *ht = malloc(sizeof(hash_table_t));

    ht->capacity = 31;
    ht->size = 0;
    ht->first_index = 0;
    ht->last_index = 0;

    ht->table = calloc(ht->capacity, sizeof(table_slot_t));

    ht->get_hash_value = get_hash_value;
    ht->equal_keys = equal_keys;
    ht->print_slot = print_slot;
    ht->free_pair = free_pair;

    return ht;
}

void hash_table_insert(hash_table_t *ht, void *key, void *value)
{
    size_t index = ht->get_hash_value(key) % ht->capacity;

    if (ht->table[index] != NULL && ht->equal_keys(ht->table[index]->key, key))
    {
        ht->free_pair(ht->table[index]->key, ht->table[index]->value);
        ht->table[index]->key = key;
        ht->table[index]->value = value;
        return;
    }

    table_slot_t *slot = alloc_table_slot(key, value);

    if (ht->size == 0)
    {
        ht->first_index = index;
        ht->last_index = index;
    }

    if (ht->table[index] == NULL)
    {
        ht->table[index] = slot;
    }
    else
    {
        slot->next = ht->table[index];
        ht->table[index] = slot;
    }

    if (index < ht->first_index)
    {
        ht->first_index = index;
    }
    else if (index > ht->last_index)
    {
        ht->last_index = index;
    }

    ht->size++;
}

void *hash_table_lookup(hash_table_t *ht, void *key)
{
    size_t index = ht->get_hash_value(key) % ht->capacity;
    table_slot_t *slot = ht->table[index];

    while (slot != NULL && !(ht->equal_keys(key, slot->key)))
    {
        slot = slot->next;
    }

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

    free_table_slot(ht, &curr);
}

void hash_table_print(hash_table_t *ht)
{
    if (ht->size == 0)
    {
        printf("{}\n");
        return;
    }

    size_t i = ht->first_index;
    table_slot_t *slot = ht->table[i];

    printf("{");

    while (i != ht->last_index)
    {
        while (slot != NULL)
        {
            ht->print_slot(slot->key, slot->value);
            printf(", ");
            slot = slot->next;
        }

        i++;
        slot = ht->table[i];
    }

    while (slot->next != NULL)
    {
        ht->print_slot(slot->key, slot->value);
        printf(", ");
        slot = slot->next;
    }

    ht->print_slot(slot->key, slot->value);
    printf("}\n");
}

void hash_table_free(hash_table_t *ht)
{
    table_slot_t *slot = NULL;
    table_slot_t *slot_succeeding = NULL;

    for (size_t i = ht->first_index; i <= ht->last_index; i++)
    {
        slot = ht->table[i];

        if (slot == NULL)
        {
            continue;
        }

        while (slot != NULL)
        {
            slot_succeeding = slot->next;
            free_table_slot(ht, &slot);
            slot = slot_succeeding;
        }
    }

    free(ht->table);
    free(ht);
}
