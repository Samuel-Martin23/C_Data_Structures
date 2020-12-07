
#include "list.h"

int main()
{
    Linked_List_T scores = InitLinkedListInt(23, 54, 876, 234, 77);     // 23-->54-->876-->234-->77-->NULL
    LinkedListAppend(&scores, IntCastVoid(12));                         // 23-->54-->876-->234-->77-->12-->NULL
    LinkedListRemoveValue(&scores, IntCastVoid(54));                    // 23-->876-->234-->77-->12-->NULL
    LinkedListExtend(&scores, IntCastVoidArray(45, 495, 177));          // 23-->876-->234-->77-->12-->45-->495-->177-->NULL
    PrintT(LinkedListGetValue(&scores, 7), "Index of 5 has the value of ", "\n");
    printf("495 is in the linked list: %d\n", LinkedListCheckValue(&scores, IntCastVoid(495)));
    LinkedListInsert(&scores, 5, IntCastVoid(711));                     // 23-->876-->234-->77-->12-->711-->45-->495-->177-->NULL
    LinkedListRemoveIndex(&scores, 1);                                  // 23-->234-->77-->12-->711-->45-->495-->177-->NULL
    LinkedListReverse(&scores);                                         // 177-->495-->45-->711-->12-->77-->234-->23-->NULL
    LinkedListSort(&scores);                                            // 12-->23-->45-->77-->177-->234-->495-->711-->NULL
    LinkedListFree(&scores);
    LinkedListExtend(&scores, IntCastVoidArray(1, 2, 3));               // 1-->2-->3-->NULL
    LinkedListPrint(&scores);
    LinkedListFree(&scores);
    return 0;
}