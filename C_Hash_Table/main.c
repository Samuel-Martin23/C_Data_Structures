#include "hash_table.h"

size_t generate_hash_str(void *key)
{
    size_t hash_value = 0;
    const char *str_key = (const char*)key;
    size_t str_length = (strlen(str_key)) / 2;

    for (size_t i = 0; i < str_length; i++)
    {
        hash_value = 31 * hash_value + str_key[i];
    }

    return hash_value;
}
bool keys_equal_str(void *key_1, void *key_2) {return (strcmp((const char*)key_1, (const char*)key_2) == 0);}
void print_contact(void *key, void *value) {printf("\"%s\": %d", (const char*)key, *(int*)value);}
void free_contact(void *key, void *value) {free(key); free(value);}

int *alloc_int(int value)
{
    int *allocated_value = malloc(sizeof(int));
    *allocated_value = value;
    return allocated_value;
}

void insert_contact(hash_table_t *ht, const char *name, int age)
{
    hash_table_insert(ht, strdup(name), alloc_int(age));
}

void delete_contact(hash_table_t *ht, const char *name)
{
    hash_table_delete(ht, (void*)name);
}

int main()
{
    hash_table_t *contacts = hash_table_init(generate_hash_str, keys_equal_str, print_contact, free_contact);

    insert_contact(contacts, "Sam", 89);
    insert_contact(contacts, "Luke", 32);
    insert_contact(contacts, "John", 67);

    hash_table_print(contacts);

    delete_contact(contacts, "Sam");

    hash_table_print(contacts);

    hash_table_free(contacts);

    return 0;
}
