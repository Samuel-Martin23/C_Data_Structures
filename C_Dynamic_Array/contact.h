#ifndef CONTACT_H
#define CONTACT_H

#include "dynamic_array.h"

typedef struct
{
    char name[25];
    int age;
} Contact;

DynamicArray *dynamic_array_init_contact(void);
void dynamic_array_append_contact(DynamicArray *dyn_array, const char *name, int age);
void dynamic_array_remove_contact(DynamicArray *dyn_array, const char *name, int age);
bool dynamic_array_iterate_contact(DynamicArrayIterator *iter, Contact **contact);

#endif /* CONTACT_H */
