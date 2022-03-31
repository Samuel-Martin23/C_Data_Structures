#include "list.h"

int main()
{
    list_t scores = list_init(INT, 5, 23, 54, 876, 234, 77);            // 23 ==> 54 ==> 876 ==> 234 ==> 77 ==> NULL
    list_append(&scores, 12);                                           // 23 ==> 54 ==> 876 ==> 234==> 77 ==> 12 ==> NULL
    list_remove_value(&scores, 54);                                     // 23 ==> 876 ==> 234 ==> 77 ==> 12 ==> NULL
    list_extend(&scores, 3, 45, 495, 177);                              // 23 ==> 876 ==> 234 ==> 77 ==> 12 ==> 45 ==> 495 ==> 177 ==> NULL
    list_insert(&scores, 5, 711);                                       // 23 ==> 876 ==> 234 ==> 77 ==> 12 ==> 711 ==> 45 ==> 495 ==> 177 ==> NULL
    list_remove_index(&scores, 1);                                      // 23 ==> 234 ==> 77 ==> 12 ==> 711 ==> 45 ==> 495 ==> 177 ==> NULL
    list_reverse(&scores);                                              // 177 ==> 495 ==> 45 ==> 711 ==> 12 ==> 77 ==> 234 ==> 23 ==> NULL
    list_sort(&scores);                                                 // 12 ==> 23 ==> 45 ==> 77 ==> 177 ==> 234 ==> 495 ==> 711 ==> NULL
    list_print(&scores);
    print_total_mem();                                                  // Total Memory: 160 Bytes
    list_free(&scores);
    print_total_mem();                                                  // Total Memory: 0 Bytes

    return 0;
}
