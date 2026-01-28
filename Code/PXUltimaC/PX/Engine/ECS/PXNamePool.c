#include "PXNamePool.h"

#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Media/PXText.h>

typedef struct PXNamePool_
{
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
        pxText->Data,
        pxText->SizeUsed
    );

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }    

    return PXResultOK;
}
