
#include "hash_table.h"

int main()
{
    hash_table_t names = hash_table_init(10);

    hash_table_insert(&names, str_cast_void("sam"), int_cast_void(18));
    hash_table_insert(&names, char_cast_void('S'), str_cast_void("vader"));
    hash_table_insert(&names, str_cast_void("luke skywalker"), int_cast_void(85));
    hash_table_insert(&names, float_cast_void(12.12f), str_cast_void("temp"));
    hash_table_insert(&names, int_cast_void(89), str_cast_void("help!"));
    hash_table_insert(&names, double_cast_void(511.21), float_cast_void(90.12f));
    hash_table_print(&names, "", "\n");

    hash_elem_t *age = hash_table_lookup(&names, str_cast_void("sam"));
    print_t(age->T_value, age->value, "", "\n");

    hash_table_delete_elem(&names, char_cast_void('S'));
    hash_table_print(&names, "", "\n");

    print_total_mem();

    hash_table_free(&names);

    print_total_mem();

    return 0;
}
