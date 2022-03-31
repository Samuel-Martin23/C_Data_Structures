#include "vector.h"

int main()
{
    vector_t *scores = vector_alloc(INT, 5, 23, 1, 3, 2, 4);    // |23| |1| |3| |2| |4|
    vector_push(scores, 6);                                     // |23| |1| |3| |2| |4| |6|
    vector_pop(scores);                                         // |23| |1| |3| |2| |4|
    vector_pop_index(scores, 0);                                // |1| |3| |2| |4|
    vector_remove_value(scores, 4);                             // |1| |3| |2|
    vector_extend(scores, 3, 4, 5, 6);                          // |1| |3| |2| |4| |5| |6|
    vector_insert(scores, 3, 7);                                // |1| |3| |2| |7| |4| |5| |6|
    vector_reverse(scores);                                     // |6| |5| |4| |7| |2| |3| |1|
    vector_sort(scores);                                        // |1| |2| |3| |4| |5| |6| |7|

    print_total_mem();                                          // Total Memory: 424 Bytes

    vector_free(&scores);

    print_total_mem();                                          // Total Memory: 0 Bytes

    return 0;
}
