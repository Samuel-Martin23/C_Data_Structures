
#include "list.c"

int main()
{
    Linked_List_T *scores = InitLinkedList(intA(23, 54, 876, 234, 77));
    LinkedListAppend(&scores, int(12));                     // 23-->54-->876-->234-->77-->12-->NULL
    LinkedListRemoveValue(&scores, int(54));                // 23-->876-->234-->77-->12-->NULL
    LinkedListExtend(&scores, intA(45, 495, 177));          // 23-->876-->234-->77-->12-->45-->495-->177-->NULL
    PrintT(GetValueFromIndex(&scores, 7), "Index of 5 has the value of ", "\n");
    printf("495 is in the array: %d\n", CheckValueLinkedList(&scores, int(495)));
    LinkedListInsert(&scores, 5, int(711));                 // 23-->876-->234-->77-->12-->711-->45-->495-->177-->NULL
    LinkedListRemoveIndex(&scores, 1);                      // 23-->234-->77-->12-->711-->45-->495-->177-->NULL
    LinkedListReverse(&scores);                             // 177-->495-->45-->711-->12-->77-->234-->23-->NULL
    LinkedListSort(&scores);                                // 12-->23-->45-->77-->177-->234-->495-->711-->NULL
    PrintAllocatedMemory();                                 // 192 Bytes Allocated
    LinkedListClear(&scores);
    PrintAllocatedMemory();                                 // 24 Bytes Allocated
    LinkedListExtend(&scores, intA(1, 2, 3));               // 1-->2-->3-->NULL
    PrintAllocatedMemory();                                 // 96 Bytes Allocated
    LinkedListFree(&scores);
    PrintAllocatedMemory();                                 // 0 Bytes Allocated
    return 0;
}