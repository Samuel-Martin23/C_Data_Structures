#include "list.c"


int main()
{
    List *scores = InitList(intA(23, 54, 876, 234, 77));
    AppendList(&scores, int(12));                   // 23-->54-->876-->234-->77-->12-->NULL
    RemoveValueList(&scores, int(54));              // 23-->876-->234-->77-->12-->NULL
    ExtendList(&scores, intA(45, 495, 177));        // 23-->876-->234-->77-->12-->45-->495-->177-->NULL
    PrintT(GetValueFromIndex(&scores, 7), "Index of 5 has the value of ", "\n");
    printf("495 is in the array: %d\n", CheckValueList(&scores, int(495)));
    InsertList(&scores, 5, int(711));               // 23-->876-->234-->77-->12-->711-->45-->495-->177-->NULL
    PopListIndex(&scores, 1);                       // 23-->234-->77-->12-->711-->45-->495-->177-->NULL
    ReverseList(&scores);                           // 177-->495-->45-->711-->12-->77-->234-->23-->NULL
    SortList(&scores);                              // 12-->23-->45-->77-->177-->234-->495-->711-->NULL
    PrintAllocatedMemory();                         // 192 Bytes Allocated
    ClearList(&scores);
    PrintAllocatedMemory();                         // 24 Bytes Allocated
    ExtendList(&scores, intA(1, 2, 3));             // 1-->2-->3-->NULL
    PrintAllocatedMemory();                         // 96 Bytes Allocated
    FreeLinkedList(&scores);
    PrintAllocatedMemory();                         // 0 Bytes Allocated

    return 0;
}

/*
Node_T* InitList(Type T, void *data, int data_len)
void AppendLists(void* data)
void RemoveValueList(Node_T** head, void* value)
void ClearList(Node_T** head)
void ExtendList(Type T, void* data, int data_len)
void* GetIndexList(Node_T** head, int index)
bool CheckValueList(Node_T** head, void* value)
void InsertList(Node_T** head, int index, void* data)
void PopList(Node_T** head, int index)
void ReverseList(Node_T** head)
void SortList(Node_T** head)
void PrintList(Node_T** head)
void PrintAllocatedMemory()
*/