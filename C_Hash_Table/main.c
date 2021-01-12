
#include "hash_table.h"

int main()
{
    Hash_Table_t names = InitHashTable(10);

    InsertHashTable(&names, StrCastVoid("sam"), IntCastVoid(18));
    InsertHashTable(&names, CharCastVoid('S'), StrCastVoid("vader"));
    InsertHashTable(&names, StrCastVoid("luke skywalker"), IntCastVoid(85));
    InsertHashTable(&names, FloatCastVoid(12.12f), StrCastVoid("temp"));
    InsertHashTable(&names, IntCastVoid(89), StrCastVoid("help!"));
    InsertHashTable(&names, DoubleCastVoid(511.21), FloatCastVoid(90.12));
    PrintHashTable(&names, "", "\n");

    Bucket_t *age = LookupHashTable(&names, StrCastVoid("sam"));
    PrintT(age->T_Value, age->value, "", "\n");

    DeleteHashTableBucket(&names, CharCastVoid('S'));
    PrintHashTable(&names, "", "\n");

    PrintAllocatedMemHT(&names);
    FreeHashTable(&names);
    PrintAllocatedMemHT(&names);

    return 0;
}
