
#include "hash_table.h"

int main()
{
    Hash_Table_t names = HashTableInit(10);

    HashTableInsert(&names, StrCastVoid("sam"), IntCastVoid(18));
    HashTableInsert(&names, CharCastVoid('S'), StrCastVoid("vader"));
    HashTableInsert(&names, StrCastVoid("luke skywalker"), IntCastVoid(85));
    HashTableInsert(&names, FloatCastVoid(12.12f), StrCastVoid("temp"));
    HashTableInsert(&names, IntCastVoid(89), StrCastVoid("help!"));
    HashTableInsert(&names, DoubleCastVoid(511.21), FloatCastVoid(90.12));
    HashTablePrint(&names, "", "\n");

    Bucket_t *age = HashTableLookup(&names, StrCastVoid("sam"));
    PrintT(age->T_Value, age->value, "", "\n");

    HashTableDeleteBucket(&names, CharCastVoid('S'));
    HashTablePrint(&names, "", "\n");

    PrintAllocatedMemHT(&names);
    HashTableFree(&names);
    PrintAllocatedMemHT(&names);

    return 0;
}
