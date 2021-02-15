
#include "hash_table.h"

int main()
{
    hash_table_t table = hash_table_init(10);

    hash_table_insert(&table, STR, INT, "sam", 18);
    hash_table_insert(&table, CHAR, STR, 'S', "Vader");
    hash_table_insert(&table, STR, INT, "luke skywalker", 23);
    hash_table_insert(&table, FLOAT, STR, 12.12f, "temp");
    hash_table_insert(&table, INT, STR, 89, "help!");
    hash_table_insert(&table, DOUBLE, FLOAT, 511.21, 90.12f);
    hash_table_print(&table, "", "\n");
    print_total_mem();

    hash_elem_t *age = hash_table_lookup(&table, STR, "sam");
    print_t(age->T_value, age->value, "", "\n");

    hash_table_delete_elem(&table, CHAR, 'S');
    hash_table_print(&table, "", "\n");

    print_total_mem();

    hash_table_free(&table);

    print_total_mem();

    return 0;
}
