#pragma once

#ifndef PXListLinkedIncludedD
#define PXListLinkedIncludedD

#include <PX/Media/PXType.h>

#define PXListLinkedNodeNotUsed (PXSize)-1
#define PXListLinkedNodeNoNext (PXSize)0

#define PXListLinkedUseAdress (PXSize)-1

typedef struct PXListLinkedNodeFixed_ PXListLinkedNodeFixed;

typedef struct PXListLinkedNodeFixed_
{
    void* BlockData;
    PXListLinkedNodeFixed* NodeNext; // 0=No Next 0xFFF=InvalidData
}
PXListLinkedNodeFixed;

typedef struct PXListLinkedNodeDynamic_
{
    void* BlockData;
    PXSize BlockSize;
}
PXListLinkedNodeDynamic;

typedef struct PXListLinkedFixed_
{
    void* Data;
    PXSize DataSize;

    PXSize NodeSize; // if size is -1, we will store the data pointer itself
    PXSize NodeListSizeCurrent;
    PXSize NodeListSizeMaximal;


    void* DataEntryLast;
}
PXListLinkedFixed;

PXPublic void PXAPI PXListLinkedFixedNodeConstruct(PXListLinkedFixed PXREF linkedListFixed);
PXPublic void PXAPI PXListLinkedFixedNodeDestruct(PXListLinkedFixed PXREF linkedListFixed);


PXPublic void PXAPI PXListLinkedFixedNodeSet(PXListLinkedFixed PXREF linkedListFixed, void PXREF data, const PXSize dataSize, const PXSize nodeSize);

PXPublic PXBool PXAPI PXListLinkedFixedNodeAt(const PXListLinkedFixed PXREF linkedListFixed, PXListLinkedNodeFixed PXREF pxLinkedListNodeFixed, const PXSize index);
PXPublic PXBool PXAPI PXListLinkedFixedNodeNext(PXListLinkedFixed PXREF linkedListFixed, PXListLinkedNodeFixed PXREF pxLinkedListNodeFixed);

//PXPublic PXBool PXListLinkedFixedNodeFetch(PXListLinkedFixed PXREF linkedListFixed, void PXREF element);

PXPrivate PXBool PXAPI PXListLinkedFixedDataSize(const PXListLinkedFixed PXREF linkedListFixed);
PXPrivate PXBool PXAPI PXListLinkedFixedIsDataEmbedded(const PXListLinkedFixed PXREF linkedListFixed);
PXPrivate PXSize PXAPI PXListLinkedFixedNodeStride(const PXListLinkedFixed PXREF linkedListFixed);
PXPrivate void* PXAPI PXListLinkedFixedNodeEmptySlotFetch(const PXListLinkedFixed PXREF linkedListFixed);

PXPublic PXBool PXAPI PXListLinkedFixedNodeAdd(PXListLinkedFixed PXREF linkedListFixed, void PXREF element);
PXPublic PXBool PXAPI PXListLinkedFixedNodeRemoveAt(PXListLinkedFixed PXREF linkedListFixed, const PXSize index);

#endif