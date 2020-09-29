
#include "vector.c"


int main()
{
    Vector_T scroes = VectorInit(intA(5, 1, 3, 2, 4));  // |5| |1| |3| |2| |4|
    VectorPush(&scroes, int(6));                        // |5| |1| |3| |2| |4| |6| 
    VectorPop(&scroes);                                 // |5| |1| |3| |2| |4|
    VectorPopIndex(&scroes, 0);                         // |1| |3| |2| |4|
    VectorRemoveValue(&scroes, int(4));                 // |1| |3| |2|
    VectorExtened(&scroes, intA(4, 5, 6));              // |1| |3| |2| |4| |5| |6|
    VectorInsert(&scroes, int(7), 3);                   // |1| |3| |2| |7| |4| |5| |6| 
    VectorReverse(&scroes);                             // |6| |5| |4| |7| |2| |3| |1| 
    VectorSort(&scroes);                                // |1| |2| |3| |4| |5| |6| |7|
    VectorClear(&scroes);
    return 0;
}

/*
Vctr_T *VectorInit(Type T, void *array, int size)
void VectorPush(Vctr_T **v, void *value)
void VectorPop(Vctr_T **v)
void VectorPopIndex(Vctr_T **v, int index)
void VectorRemoveValue(Vctr_T** v, void *value)
void VectorClear(Vctr_T **v)
void VectorExtened(Vctr_T **v, Type T, void *array, int size_array)
void VectorInsert(Vctr_T **v, void *value, int index)
void VectorReverse(Vctr_T **v)
void VectorSort(Vctr_T **v)
void VectorPrint(Vctr_T **v)
void PrintAllocatedMemory()
*/