
#include "dictionary.h"

int get_str_key(dictionary_t *dict, char *key)
{
    void *value = dict_lookup(dict, key);
    return value == NULL ? -1 : void_cast_int(value);
}

int main()
{
    dictionary_t names_ages = dict_init(STR, INT);

    dict_insert(&names_ages, "Jim", 54);
    dict_insert(&names_ages, "Kelly", 78);
    dict_insert(&names_ages, "Ryan", 34);
    dict_insert(&names_ages, "Dwight", 98);
    dict_insert(&names_ages, "Pam", 31);

    dict_remove_key(&names_ages, "Ryan");
    dict_print(&names_ages);

    printf("%d\n", get_str_key(&names_ages, "Pam"));

    dict_free(&names_ages);

    return 0;
}
