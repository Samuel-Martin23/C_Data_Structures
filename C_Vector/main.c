
#include "vector.h"

int main()
{
    vector_t scores = vector_init_int(5, 1, 3, 2, 4);           // |5| |1| |3| |2| |4|
    vector_push(&scores, int_cast_void(6));                     // |5| |1| |3| |2| |4| |6|
    vector_pop(&scores);                                        // |5| |1| |3| |2| |4|
    vector_pop_index(&scores, 0);                               // |1| |3| |2| |4|
    vector_remove_value(&scores, int_cast_void(4));             // |1| |3| |2|
    vector_extend(&scores, int_array_cast_void(4, 5, 6));       // |1| |3| |2| |4| |5| |6|
    vector_insert(&scores, int_cast_void(7), 3);                // |1| |3| |2| |7| |4| |5| |6|
    vector_reverse(&scores);                                    // |6| |5| |4| |7| |2| |3| |1|
    vector_sort(&scores);                                       // |1| |2| |3| |4| |5| |6| |7|

    vector_clear(&scores);
    print_allocated_mem_vector(&scores);                        // Bytes Allocated: 800

    scores = vector_init_int(12, 2, 1);                         // |12| |2| |1|
    print_allocated_mem_vector(&scores);                        // Bytes Allocated: 812

    vector_free(&scores);
    print_allocated_mem_vector(&scores);                        // Bytes Allocated: 0

    return 0;
}
