
#include "linked_list.h"

int main()
{
    Linked_List_t scores = LinkedListInitInt(23, 54, 876, 234, 77);     // 23-->54-->876-->234-->77-->NULL
    LinkedListAppend(&scores, IntCastVoid(12));                         // 23-->54-->876-->234-->77-->12-->NULL
    LinkedListRemoveValue(&scores, IntCastVoid(54));                    // 23-->876-->234-->77-->12-->NULL
    LinkedListExtend(&scores, IntCastVoidArray(45, 495, 177));          // 23-->876-->234-->77-->12-->45-->495-->177-->NULL
    LinkedListInsert(&scores, 5, IntCastVoid(711));                     // 23-->876-->234-->77-->12-->711-->45-->495-->177-->NULL
    LinkedListRemoveIndex(&scores, 1);                                  // 23-->234-->77-->12-->711-->45-->495-->177-->NULL
    LinkedListReverse(&scores);                                         // 177-->495-->45-->711-->12-->77-->234-->23-->NULL
    LinkedListSort(&scores);                                            // 12-->23-->45-->77-->177-->234-->495-->711-->NULL
    LinkedListFree(&scores);
    LinkedListExtend(&scores, IntCastVoidArray(1, 2, 3));               // 1-->2-->3-->NULL
    LinkedListPrint(&scores);
    LinkedListFree(&scores);
    PrintAllocatedMemList(&scores);

    return 0;
}