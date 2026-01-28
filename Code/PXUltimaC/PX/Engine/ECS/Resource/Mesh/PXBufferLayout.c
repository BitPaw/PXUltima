#include "PXBufferLayout.h"

#include <PX/OS/PXOS.h>

void PXAPI PXBufferLayoutSET
(
    PXBufferLayout PXREF pxBufferLayout,
    PXBufferLayoutInfo PXREF pxBufferLayoutInfo
)
{
    if(0 == pxBufferLayoutInfo->Amount || 1 == pxBufferLayoutInfo->Amount)
    {
        pxBufferLayout->LayoutAmount = 1;

        return;
    }

    pxBufferLayout->LayoutAmount = pxBufferLayoutInfo->Amount;
    pxBufferLayout->LayoutList = PXMemoryHeapCallocT(PXBufferLayoutEntry, pxBufferLayoutInfo->Amount);
}

PXBufferLayoutEntry* PXAPI PXBufferLayoutGET(PXBufferLayout PXREF pxBufferLayout)
{
    if(1 == pxBufferLayout->LayoutAmount)
    {
        return pxBufferLayout->LayoutPrime;
    }
    else
    {
        return pxBufferLayout->LayoutList;
    }
}
