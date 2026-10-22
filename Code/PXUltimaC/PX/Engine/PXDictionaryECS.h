#pragma once

#include <PX/Container/Dictionary/PXDictionary.h>

typedef struct PXDictionaryECS_
{
    PXDictionaryT(PXID, PXECSReferece)* Dictionary;
}
PXDictionaryECS;

PXPublic PXResult PXAPI PXDictionaryECSCreate(PXDictionaryECS PXREF pxDictionaryECS);
PXPublic PXResult PXAPI PXDictionaryECSDestroy(PXDictionaryECS PXREF pxDictionaryECS);


// Function to resolve the "PXECSInfo" reference.
// Check it against the ID we expect.
// If it does not match, we found a stale reference.
// It will be remove from the list automatically.
PXPrivate PXECSInfo* PXAPI PXDictionaryECSCheck(PXDictionaryECS PXREF pxDictionaryECS, PXECSInfo PXREF pxECSInfo, const PXID pxIDExpected);

PXPublic PXSize PXAPI PXDictionaryECSAmount(const PXDictionaryECS PXREF pxDictionaryECS);

PXPublic PXResult PXAPI PXDictionaryECSAdd(PXDictionaryECS PXREF pxDictionaryECS, PXECSInfo PXREF pxECSInfo);
PXPublic PXResult PXAPI PXDictionaryECSReplace(PXDictionaryECS PXREF pxDictionaryECS, PXECSInfo PXREF pxECSInfo, const PXID pxIDToReplace);

PXPublic PXResult PXAPI PXDictionaryECSRemoveViaIndex(PXDictionaryECS PXREF pxDictionaryECS, const PXSize index);
PXPublic PXResult PXAPI PXDictionaryECSRemoveViaID(PXDictionaryECS PXREF pxDictionaryECS, const PXID pxID);

PXPublic PXECSInfo* PXAPI PXDictionaryECSGetViaIndex(PXDictionaryECS PXREF pxDictionaryECS, const PXSize index);
PXPublic PXECSInfo* PXAPI PXDictionaryECSGetViaID(PXDictionaryECS PXREF pxDictionaryECS, const PXID pxID);
//PXPublic PXECSInfo* PXAPI PXDictionaryECSGetViaText(const PXDictionaryECS PXREF pxDictionaryECS, PXText PXREF pxText);