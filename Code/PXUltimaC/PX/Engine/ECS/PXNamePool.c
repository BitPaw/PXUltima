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
    PXDictionaryConstruct(&_pxNamePool.NameLookup, sizeof(PXID), 0, PXDictionaryValueLocalityInternalEmbedded);

    return PXResultOK;
}

PXResult PXAPI PXNamePoolStore(const PXID pxID, const PXText* pxText, PXText* pxTextResult)
{
   // PXDictionaryEntryAddRange();

    PXResult pxResult = PXDictionaryEntryAdd(&_pxNamePool.NameLookup, pxText->A, pxText->SizeUsed);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }    

    return PXResultOK;
}
