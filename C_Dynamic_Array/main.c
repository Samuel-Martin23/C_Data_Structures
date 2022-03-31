#include "dynamic_array.h"

typedef struct
{
    char name[25];
    int age;
} person_t;

bool dynamic_array_equal_person(void *value_1, void *value_2)
{
    person_t *person_1 = (person_t*)value_1;
    person_t *person_2 = (person_t*)value_2;

    bool is_name = (strncmp(person_1->name, person_2->name, 25) == 0);
    bool is_age = (person_1->age == person_2->age);

    return (is_name && is_age);
}

void dynamic_array_print_person(void *value)
{
    person_t *person = (person_t*)value;
    printf("(%s: %d)", person->name, person->age);
}

void *alloc_person(const char *name, int age)
{
    person_t *person = malloc(sizeof(person_t));

    person->age = age;
    memcpy(person->name, name, 25);

    return person;
}

void dynamic_array_remove_person(dynamic_array_t *persons, const char *name, int age)
{
    person_t dummy;

    dummy.age = age;
    memcpy(dummy.name, name, 25);

    dynamic_array_remove_value(persons, &dummy);
}

int main()
{
    dynamic_array_t *persons = dynamic_array_init_alloc(dynamic_array_equal_person, dynamic_array_print_person, free);

    dynamic_array_push(persons, alloc_person("Samuel", 45));
    dynamic_array_push(persons, alloc_person("John", 23));
    dynamic_array_push(persons, alloc_person("Luke", 654));
    dynamic_array_push(persons, alloc_person("Will", 1234));

    dynamic_array_print(persons);

    dynamic_array_pop_index(persons, 0);

    dynamic_array_print(persons);

    dynamic_array_remove_person(persons, "Luke", 654);

    dynamic_array_print(persons);

    dynamic_array_free(persons);

    return 0;
}
