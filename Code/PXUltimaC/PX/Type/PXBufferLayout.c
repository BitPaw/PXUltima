#include "PXBufferLayout.h"

#include <PX/OS/PXOS.h>

PXResult PXAPI PXBufferLayoutPrepare(PXBufferLayout PXREF pxBufferLayout, const PXSize amount)
{
    pxBufferLayout->LayoutAmount = amount;

    if(amount <= PXEmbeddedArraySize)
    {
        return PXActionRefusedIndexOutOfBounce;
    }

    pxBufferLayout->LayoutAmount = amount;
    pxBufferLayout->LayoutList = PXMemoryHeapCallocT(PXBufferLayoutEntry, amount);

    return PXResultOK;
}

PXBufferLayoutEntry* PXAPI PXBufferLayoutGetViaIndex(PXBufferLayout PXREF pxBufferLayout, const PXSize index)
{
    if(!pxBufferLayout)
    {
        return PXNull;
    }

    // Outofbounce 
    PXBool isValidIndex = index < pxBufferLayout->LayoutAmount;

    if(!isValidIndex)
    {
        return PXNull;
    }

    PXBufferLayoutEntry* pxBufferLayoutEntryList = PXNull;

    const PXBool useEmbedded = PXEmbeddedArraySize >= pxBufferLayout->LayoutAmount;

    if(useEmbedded)
    {
        pxBufferLayoutEntryList = pxBufferLayout->LayoutPrime;
    }
    else
    {
        pxBufferLayoutEntryList = pxBufferLayout->LayoutList;
    }

    PXBufferLayoutEntry* target = &pxBufferLayoutEntryList[index];

    return target;
}

PXSize PXAPI PXBufferLayoutAmount(const PXBufferLayout PXREF pxBufferLayout)
{
    return pxBufferLayout->LayoutAmount;
}