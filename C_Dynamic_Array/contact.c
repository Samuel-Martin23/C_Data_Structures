#include "contact.h"

static bool contact_equals_contact(void *value_1, void *value_2)
{
    Contact *contact_1 = (Contact*)value_1;
    Contact *contact_2 = (Contact*)value_2;

    bool is_name = (strncmp(contact_1->name, contact_2->name, 25) == 0);
    bool is_age = (contact_1->age == contact_2->age);

    return (is_name && is_age);
}

static void contact_print_contact(void *value)
{
    Contact *contact = (Contact*)value;
    printf("(%s: %d)", contact->name, contact->age);
}

DynamicArray *dynamic_array_init_contact(void)
{
    return dynamic_array_init(contact_equals_contact, contact_print_contact, free);
}

void dynamic_array_push_contact(DynamicArray *dyn_array, const char *name, int age)
{
    Contact *contact = malloc(sizeof(Contact));

    contact->age = age;
    memcpy(contact->name, name, 25);

    dynamic_array_push(dyn_array, contact);
}

void dynamic_array_remove_contact(DynamicArray *dyn_array, const char *name, int age)
{
    Contact dummy;

    dummy.age = age;
    memcpy(dummy.name, name, 25);

    dynamic_array_remove(dyn_array, &dummy);
}

bool dynamic_array_iterate_contact(DynamicArrayIterator *iter, Contact **contact)
{
    void *value = NULL;

    bool is_next = dynamic_array_iterator_iterate(iter, &value);

    if (is_next)
    {
        *contact = (Contact*)value;
    }
    else
    {
        *contact = NULL;
    }

    return is_next;
}
