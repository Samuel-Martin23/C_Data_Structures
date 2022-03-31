#include "vector.h"

typedef struct
{
    char name[25];
    int age;
} person_t;

bool vec_equal_person(void *value_1, void *value_2)
{
    person_t *person_1 = (person_t*)value_1;
    person_t *person_2 = (person_t*)value_2;

    bool is_name = (strncmp(person_1->name, person_2->name, 25) == 0);
    bool is_age = (person_1->age == person_2->age);

    return (is_name && is_age);
}

void vec_print_person(void *value)
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

void vector_remove_person(vector_t *persons, const char *name, int age)
{
    person_t dummy;

    dummy.age = age;
    memcpy(dummy.name, name, 25);

    vector_remove_value(persons, &dummy);
}

int main()
{
    vector_t *persons = vector_init_alloc(vec_equal_person, vec_print_person, free);

    vector_push(persons, alloc_person("Samuel", 45));
    vector_push(persons, alloc_person("John", 23));
    vector_push(persons, alloc_person("Luke", 654));
    vector_push(persons, alloc_person("Will", 1234));

    vector_print(persons);

    vector_pop_index(persons, 0);

    vector_print(persons);

    vector_remove_person(persons, "Luke", 654);

    vector_print(persons);

    vector_free(persons);

    return 0;
}
