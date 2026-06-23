#include "PXNamePool.h"

#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Type/PXText.h>
#include <PX/OS/Memory/PXMemory.h>

typedef struct PXNamePool_
{
    PXECSInfo Info;

    PXDictionaryT(PXID, void*)* NameLookup;
}
PXNamePool;

PXNamePool _pxNamePool;

PXResult PXAPI PXNamePoolInit()
{
    PXDictionaryCreateInfo pxDictionaryCreateInfo;
    PXClear(PXDictionaryCreateInfo, &pxDictionaryCreateInfo);
    pxDictionaryCreateInfo.KeySize = sizeof(PXID);
    pxDictionaryCreateInfo.ValueLocality = PXDictionaryValueLocalityInternalEmbedded;

    PXDictionaryCreate(&_pxNamePool.NameLookup, &pxDictionaryCreateInfo);

    return PXResultOK;
}

PXResult PXAPI PXNamePoolStore(const PXID pxID, const PXText* pxText, PXText* pxTextResult)
{
    PXResult pxResult = PXDictionaryEntryAddRange
    (
        _pxNamePool.NameLookup, 
        &pxID,
        pxText->Data4,
        pxText->SizeUsed
    ); 

    return pxResult;
}

PXResult PXAPI PXNamePoolGet(const PXID pxID, PXText PXREF pxText)
{
    PXDictionaryEntry pxDictionaryEntry;
    pxDictionaryEntry.KeyAddress = &pxID;
    pxDictionaryEntry.KeySize = sizeof(PXID);
    pxDictionaryEntry.ValueAdress = 0;
    pxDictionaryEntry.ValueSize = 0;

    PXResult pxResult = PXDictionaryEntryFind(_pxNamePool.NameLookup, &pxDictionaryEntry);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    PXTextFromAdressA(pxText, (char*)pxDictionaryEntry.ValueAdress, pxDictionaryEntry.ValueSize, pxDictionaryEntry.ValueSize);

    return pxResult;
}