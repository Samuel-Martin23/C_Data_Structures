
#include "vector.h"

int main()
{
    Vector_T scores = VectorInitInt(5, 1, 3, 2, 4);         // |5| |1| |3| |2| |4|
    VectorPush(&scores, IntCastVoid(6));                    // |5| |1| |3| |2| |4| |6|
    VectorPop(&scores);                                     // |5| |1| |3| |2| |4|
    VectorPopIndex(&scores, 0);                             // |1| |3| |2| |4|
    VectorRemoveValue(&scores, IntCastVoid(4));             // |1| |3| |2|
    VectorExtend(&scores, IntCastVoidArray(4, 5, 6));       // |1| |3| |2| |4| |5| |6|
    VectorInsert(&scores, IntCastVoid(7), 3);               // |1| |3| |2| |7| |4| |5| |6|
    VectorReverse(&scores);                                 // |6| |5| |4| |7| |2| |3| |1|
    VectorSort(&scores);                                    // |1| |2| |3| |4| |5| |6| |7|

    PrintVectorSize(&scores);
    VectorPrint(&scores);
    VectorClear(&scores);
    PrintVectorSize(&scores);
    VectorFree(&scores);
    PrintAllocatedMemory(&scores);

    return 0;
}