#include "PXListLinked.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/Math/PXMath.h>

void PXAPI PXListLinkedFixedNodeConstruct(PXListLinkedFixed PXREF linkedListFixed)
{
    PXMemoryClear(linkedListFixed, sizeof(PXListLinkedFixed));
}

void PXAPI PXListLinkedFixedNodeDestruct(PXListLinkedFixed PXREF linkedListFixed)
{
    
}

void PXAPI PXListLinkedFixedNodeSet(PXListLinkedFixed PXREF linkedListFixed, void PXREF data, const PXSize dataSize, const PXSize nodeSize)
{
    linkedListFixed->Data = data;
    linkedListFixed->DataSize = dataSize;
    linkedListFixed->NodeListSizeCurrent = 0;
    linkedListFixed->NodeListSizeMaximal = dataSize;
    linkedListFixed->NodeSize = nodeSize;
    linkedListFixed->DataEntryLast = data;

    if(nodeSize != (PXSize)-1)
    {
        PXMemoryClear(data, nodeSize * dataSize);
    }
    else
    {
        PXMemoryClear(data, sizeof(void*));
    }
}

PXBool PXAPI PXListLinkedFixedNodeAt(const PXListLinkedFixed PXREF linkedListFixed, PXListLinkedNodeFixed PXREF pxLinkedListNodeFixed, const PXSize index)
{
    const PXSize nodeBlockSize = PXListLinkedFixedNodeStride(linkedListFixed);
    const void PXREF data = (PXAdress)linkedListFixed->Data + nodeBlockSize * index;

    PXMemoryCopy(data, &pxLinkedListNodeFixed->BlockData, sizeof(void*));

    {
        const PXSize dataSize = PXListLinkedFixedDataSize(linkedListFixed);
        PXListLinkedNodeFixed* nextNode = (PXListLinkedNodeFixed*)((PXAdress)pxLinkedListNodeFixed->BlockData + dataSize);

        pxLinkedListNodeFixed->NodeNext = nextNode;
    }    

    /*
    switch (adress)
    {
        case PXListLinkedNodeNotUsed:
        {
            pxLinkedListNodeFixed->BlockData = 0;
            pxLinkedListNodeFixed->NodeNext = 0;

            return PXNo;
        }
        case PXListLinkedNodeNoNext:
        {
            MemoryCopy(data, nodeBlockSize, &pxLinkedListNodeFixed->BlockData, nodeBlockSize);
            pxLinkedListNodeFixed->NodeNext = 0;
            break;
        }
        default:
        {
            MemoryCopy(data, nodeBlockSize, &pxLinkedListNodeFixed->BlockData, nodeBlockSize);
            pxLinkedListNodeFixed->NodeNext = nextNode;     
            break;
        }       
    }  */

    return PXYes;
}

PXBool PXAPI PXListLinkedFixedNodeNext(PXListLinkedFixed PXREF linkedListFixed, PXListLinkedNodeFixed PXREF pxLinkedListNodeFixed)
{
    // Check if node is valid
    {
        const PXBool validNode = pxLinkedListNodeFixed->BlockData && pxLinkedListNodeFixed->NodeNext && (pxLinkedListNodeFixed->NodeNext != (void*)-1);

        if (!validNode)
        {
            return PXNo;
        }
    }

    // Fetch next node & override it
    {
        const PXListLinkedNodeFixed PXREF fetchNode = pxLinkedListNodeFixed->NodeNext;

        pxLinkedListNodeFixed->BlockData = fetchNode->BlockData;
        pxLinkedListNodeFixed->NodeNext = fetchNode->NodeNext;
    }

    return PXYes;
}

PXBool PXAPI PXListLinkedFixedDataSize(const PXListLinkedFixed PXREF linkedListFixed)
{
    const PXBool isDataEmbedded = PXListLinkedFixedIsDataEmbedded(linkedListFixed);

    if (isDataEmbedded)
    {
        return linkedListFixed->NodeSize;
    }
    else
    {
        return sizeof(void*);
    }
}

PXBool PXAPI PXListLinkedFixedIsDataEmbedded(const PXListLinkedFixed PXREF linkedListFixed)
{
    return linkedListFixed->NodeSize != (PXSize)-1;
}

PXSize PXAPI PXListLinkedFixedNodeStride(const PXListLinkedFixed PXREF linkedListFixed)
{
    const PXSize dataSize = PXListLinkedFixedDataSize(linkedListFixed);

    return sizeof(void*) + dataSize;
}

void* PXAPI PXListLinkedFixedNodeEmptySlotFetch(const PXListLinkedFixed PXREF linkedListFixed)
{
    PXListLinkedNodeFixed nodeCurrent;

    while (PXListLinkedFixedNodeNext(linkedListFixed, &nodeCurrent));

    return 0;
}

PXBool PXAPI PXListLinkedFixedNodeAdd(PXListLinkedFixed PXREF linkedListFixed, void PXREF element)
{
    const PXBool hasSpace = linkedListFixed->NodeListSizeMaximal >= (linkedListFixed->NodeListSizeCurrent + 1);

    if (!hasSpace)
    {
        return PXNo;
    }

    const PXSize dataBlockSize = PXListLinkedFixedDataSize(linkedListFixed);
    const PXSize nodeStride = PXListLinkedFixedNodeStride(linkedListFixed);
    void* data = (PXAdress)linkedListFixed->Data + (nodeStride * linkedListFixed->NodeListSizeCurrent);
   
    // Add current data/adress
    {
        const PXBool isDataEmbedded = PXListLinkedFixedIsDataEmbedded(linkedListFixed);
        const void* dataSource = isDataEmbedded ? element : &element;

        PXMemoryCopy(dataSource, data, dataBlockSize); // Copy the whole data into this node or  Copy only the adress to the data into the node
    }   

    // Add next pointer
    {
        const PXSize targetSize = sizeof(void*);
        const void* value = PXListLinkedNodeNoNext;
        void* target = (PXAdress)data + dataBlockSize;

        PXMemoryCopy(&value, target, targetSize);
    } 

    // Fetch Last and register this node into it
    {
        if (linkedListFixed->DataEntryLast != data)
        {
            void* lastOBJ = (PXAdress)linkedListFixed->DataEntryLast + dataBlockSize;

            PXMemoryCopy(&data, lastOBJ, sizeof(void*));

            linkedListFixed->DataEntryLast = data;
        }      
    }    

    ++(linkedListFixed->NodeListSizeCurrent);

    return PXYes;
}

PXBool PXAPI PXListLinkedFixedNodeRemoveAt(PXListLinkedFixed PXREF linkedListFixed, const PXSize index)
{
    PXListLinkedNodeFixed pxLinkedListNodeFixed;

    const PXBool found = PXListLinkedFixedNodeAt(linkedListFixed, &pxLinkedListNodeFixed, index);

    if (!found)
    {
        return PXNo;            
    }

    pxLinkedListNodeFixed.NodeNext = PXListLinkedNodeNotUsed;
}