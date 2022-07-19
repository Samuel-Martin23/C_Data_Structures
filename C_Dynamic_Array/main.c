#include "contact.h"

int main()
{
    DynamicArray *contacts = dynamic_array_init_contact();

    dynamic_array_push_contact(contacts, "Samuel", 45);
    dynamic_array_push_contact(contacts, "John", 23);
    dynamic_array_push_contact(contacts, "Luke", 72);
    dynamic_array_push_contact(contacts, "Will", 29);

    dynamic_array_print(contacts);

    dynamic_array_pop_index(contacts, 0);

    dynamic_array_print(contacts);

    dynamic_array_remove_contact(contacts, "Luke", 72);

    dynamic_array_print(contacts);

    DynamicArrayIterator *iter = dynamic_array_iterator_init(contacts);

    for (Contact *contact = NULL; dynamic_array_iterate_contact(iter, &contact);)
    {
        if (contact->age == 23)
        {
            dynamic_array_remove_contact(contacts, contact->name, contact->age);
        }
    }

    dynamic_array_iterator_free(iter);

    dynamic_array_print(contacts);

    dynamic_array_free(contacts);

    return 0;
}
