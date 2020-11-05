
#include "vector.c"

int main()
{
    Vector_T *scores = VectorInit(intA(5, 1, 3, 2, 4));     // |5| |1| |3| |2| |4|
    VectorPush(&scores, int(6));                            // |5| |1| |3| |2| |4| |6|
    VectorPop(&scores);                                     // |5| |1| |3| |2| |4|
    VectorPopIndex(&scores, 0);                             // |1| |3| |2| |4|
    VectorRemoveValue(&scores, int(4));                     // |1| |3| |2|
    VectorExtened(&scores, intA(4, 5, 6));                  // |1| |3| |2| |4| |5| |6|
    VectorInsert(&scores, int(7), 3);                       // |1| |3| |2| |7| |4| |5| |6| 
    VectorReverse(&scores);                                 // |6| |5| |4| |7| |2| |3| |1| 
    VectorSort(&scores);                                    // |1| |2| |3| |4| |5| |6| |7|
    VectorFree(&scores);
    return 0;
}