
#include "hash_table.h"

int main()
{
    Hash_Table_T names = InitHashTable(10);

    InsertHashTable(&names, StrCastVoid("sam"), IntCastVoid(12));
    InsertHashTable(&names, CharCastVoid('S'), StrCastVoid("vader"));
    InsertHashTable(&names, StrCastVoid("luke skywalker"), IntCastVoid(85));
    InsertHashTable(&names, FloatCastVoid(12.12f), StrCastVoid("temp"));
    InsertHashTable(&names, IntCastVoid(89), StrCastVoid("help!"));
    InsertHashTable(&names, DoubleCastVoid(511.21), FloatCastVoid(90.12));
    PrintHashTable(&names, "", "\n");

    DeleteHashTableBucket(&names, CharCastVoid('S'));
    PrintHashTable(&names, "", "\n");

    PrintAllocatedMemory(&names);
    FreeHashTable(&names);
    PrintAllocatedMemory(&names);

    return 0;
}
