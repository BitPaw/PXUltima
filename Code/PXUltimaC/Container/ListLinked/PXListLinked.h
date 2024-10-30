#ifndef PXListLinkedINCLUDED
#define PXListLinkedINCLUDED

#include <Media/PXType.h>

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

PXPublic void PXAPI PXListLinkedFixedNodeConstruct(PXListLinkedFixed* const linkedListFixed);
PXPublic void PXAPI PXListLinkedFixedNodeDestruct(PXListLinkedFixed* const linkedListFixed);


PXPublic void PXAPI PXListLinkedFixedNodeSet(PXListLinkedFixed* const linkedListFixed, void* const data, const PXSize dataSize, const PXSize nodeSize);

PXPublic PXBool PXAPI PXListLinkedFixedNodeAt(const PXListLinkedFixed* const linkedListFixed, PXListLinkedNodeFixed* const pxLinkedListNodeFixed, const PXSize index);
PXPublic PXBool PXAPI PXListLinkedFixedNodeNext(PXListLinkedFixed* const linkedListFixed, PXListLinkedNodeFixed* const pxLinkedListNodeFixed);

//PXPublic PXBool PXListLinkedFixedNodeFetch(PXListLinkedFixed* const linkedListFixed, void* const element);

PXPrivate PXBool PXAPI PXListLinkedFixedDataSize(const PXListLinkedFixed* const linkedListFixed);
PXPrivate PXBool PXAPI PXListLinkedFixedIsDataEmbedded(const PXListLinkedFixed* const linkedListFixed);
PXPrivate PXSize PXAPI PXListLinkedFixedNodeStride(const PXListLinkedFixed* const linkedListFixed);
PXPrivate void* PXAPI PXListLinkedFixedNodeEmptySlotFetch(const PXListLinkedFixed* const linkedListFixed);

PXPublic PXBool PXAPI PXListLinkedFixedNodeAdd(PXListLinkedFixed* const linkedListFixed, void* const element);
PXPublic PXBool PXAPI PXListLinkedFixedNodeRemoveAt(PXListLinkedFixed* const linkedListFixed, const PXSize index);

#endif