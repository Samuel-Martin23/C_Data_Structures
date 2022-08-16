#include "ht_str_int.h"

int main()
{
    HashTable *names_ages = hash_table_init_str_int_args(3, "Sam", 23, "Luke", 32, "John", 67);

    hash_table_insert_str_int(names_ages, "Will", 50);

    hash_table_print(names_ages);

    hash_table_delete_str_int(names_ages, "John");

    hash_table_print(names_ages);

    printf("%d\n", hash_table_find_str_int(names_ages, "Sam"));

    HashTableIterator *iter = hash_table_iterator_init(names_ages);

    const char *key = "";
    int value = 0;
    while (hash_table_iterate_str_int(iter, &key, &value))
    {
        if (value == 32)
        {
            hash_table_delete_str_int(names_ages, key);
        }
    }

    hash_table_iterator_free(iter);

    hash_table_print(names_ages);

    hash_table_free(names_ages);

    return 0;
}
