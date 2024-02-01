#include "PXList.h"

void PXAPI PXListConstruct(PXList* const list)
{

}

void PXAPI PXListDestruct(PXList* const list)
{

}

void PXAPI PXListInitialize(PXList* const list, void* const data, const PXSize dataTypeSize, const PXSize sizeAllocated)
{

}

PXBool PXAPI PXListAdd(PXList* const list, void* const dataElement)
{
    /*
    const PXBool hasSpace = linkedListFixed->NodeListSizeMaximal > (linkedListFixed->NodeListSizeCurrent + 1);

    if (!hasSpace)
    {
        return PXNo;
    }

    void* const data = (unsigned char*)linkedListFixed->Data + (linkedListFixed->NodeSize * linkedListFixed->NodeListSizeCurrent);

    MemoryCopy(element, linkedListFixed->NodeSize, data, linkedListFixed->NodeSize);

    ++(linkedListFixed->NodeListSizeCurrent);*/

    return PXYes;
}
