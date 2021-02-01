
#include "list.h"

int main()
{
    list_t scores = list_init_int(23, 54, 876, 234, 77);            // 23 ==> 54 ==> 876 ==> 234 ==> 77 ==> NULL
    list_append(&scores, int_cast_void(12));                        // 23 ==> 54 ==> 876 ==> 234==> 77 ==> 12 ==> NULL
    list_remove_value(&scores, int_cast_void(54));                  // 23 ==> 876 ==> 234 ==> 77 ==> 12 ==> NULL
    list_extend(&scores, int_array_cast_void(45, 495, 177));        // 23 ==> 876 ==> 234 ==> 77 ==> 12 ==> 45 ==> 495 ==> 177 ==> NULL
    list_insert(&scores, int_cast_void(711), 5);                    // 23 ==> 876 ==> 234 ==> 77 ==> 12 ==> 711 ==> 45 ==> 495 ==> 177 ==> NULL
    list_remove_index(&scores, 1);                                  // 23 ==> 234 ==> 77 ==> 12 ==> 711 ==> 45 ==> 495 ==> 177 ==> NULL
    list_reverse(&scores);                                          // 177 ==> 495 ==> 45 ==> 711 ==> 12 ==> 77 ==> 234 ==> 23 ==> NULL
    list_sort(&scores);                                             // 12 ==> 23 ==> 45 ==> 77 ==> 177 ==> 234 ==> 495 ==> 711 ==> NULL
    list_free(&scores);
    list_extend(&scores, int_array_cast_void(1, 2, 3));             // 1 ==> 2 ==> 3 ==> NULL
    list_print(&scores);
    list_free(&scores);
    print_allocated_mem_list(&scores);

    return 0;
}
