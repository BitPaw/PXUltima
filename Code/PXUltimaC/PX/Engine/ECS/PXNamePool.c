#include "PXNamePool.h"

#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Media/PXText.h>

typedef struct PXNamePool_
{
    PXDictionaryT(PXID, void*) NameLookup;
}
PXNamePool;

PXNamePool _pxNamePool;

PXResult PXAPI PXNamePoolInit()
{
    PXDictionaryConstruct(&_pxNamePool.NameLookup, sizeof(PXID), PXNull, PXDictionaryValueLocalityInternalEmbedded);

    return PXActionSuccessful;
}

PXResult PXAPI PXNamePoolStore(const PXID pxID, PXText PXREF pxText, PXText PXREF pxTextResult)
{
   // PXDictionaryEntryAddRange();

    PXResult pxResult = PXDictionaryEntryAdd(&_pxNamePool.NameLookup, pxText->A, pxText->SizeUsed);

    if(PXActionSuccessful != pxResult)
    {
        return pxResult;
    }

    

    return PXActionSuccessful;
}
