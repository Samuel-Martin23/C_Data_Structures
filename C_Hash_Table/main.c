#include "hash_table.h"

typedef struct contact
{
    int age;
    char phone_number[13];
} contact_t;

size_t get_hash_str(void *key)
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

bool keys_equal_contacts(void *key_1, void *key_2) {return (strncmp((const char*)key_1, (const char*)key_2, 50) == 0);}

void print_contact(void *key, void *value) {printf("\"%s\": (%d, %s)", (const char*)key, ((contact_t*)value)->age, ((contact_t*)value)->phone_number);}

void free_contact(void *key, void *value) {free(key); free(value);}

void hash_table_insert_contact(hash_table_t *ht, const char *key_name, int age, const char *phone_number)
{
    contact_t *person = malloc(sizeof(contact_t));

    person->age = age;
    memcpy(person->phone_number, phone_number, 13);

    hash_table_insert(ht, strdup(key_name), person);
}

int main()
{
    hash_table_t *contacts = hash_table_init(get_hash_str, keys_equal_contacts, print_contact, free_contact);

    hash_table_insert_contact(contacts, "Sam", 89, "48012356780");
    hash_table_insert_contact(contacts, "Luke", 32, "4808359281");
    hash_table_insert_contact(contacts, "John", 67, "4801675678");
    hash_table_insert_contact(contacts, "Will", 50, "4808359281");

    hash_table_print(contacts);

    for (void *key = NULL, *value = NULL; hash_table_iterate(contacts, &key, &value);)
    {
        contact_t *contact = (contact_t*)value;

        if (contact->age == 89 || contact->age == 50)
        {
            hash_table_delete(contacts, key);
        }
    }

    hash_table_print(contacts);

    hash_table_free(contacts);

    return 0;
}
