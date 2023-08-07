#include "PXLinkedList.h"

#include <OS/Memory/PXMemory.h>
#include <Math/PXMath.h>

void PXLinkedListFixedNodeConstruct(PXLinkedListFixed* const linkedListFixed)
{
    PXMemoryClear(linkedListFixed, sizeof(PXLinkedListFixed));
}

void PXLinkedListFixedNodeDestruct(PXLinkedListFixed* const linkedListFixed)
{
    
}

void PXLinkedListFixedNodeSet(PXLinkedListFixed* const linkedListFixed, void* const data, const PXSize dataSize, const PXSize nodeSize)
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

PXBool PXLinkedListFixedNodeAt(const PXLinkedListFixed* const linkedListFixed, PXLinkedListNodeFixed* const pxLinkedListNodeFixed, const PXSize index)
{
    const PXSize nodeBlockSize = PXLinkedListFixedNodeStride(linkedListFixed);
    const void* const data = (PXAdress)linkedListFixed->Data + nodeBlockSize * index;

    PXMemoryCopy(data, sizeof(void*), &pxLinkedListNodeFixed->BlockData, sizeof(void*));

    {
        const PXSize dataSize = PXLinkedListFixedDataSize(linkedListFixed);
        PXLinkedListNodeFixed* nextNode = (PXLinkedListNodeFixed*)((PXAdress)pxLinkedListNodeFixed->BlockData + dataSize);

        pxLinkedListNodeFixed->NodeNext = nextNode;
    }    

    /*
    switch (adress)
    {
        case PXLinkedListNodeNotUsed:
        {
            pxLinkedListNodeFixed->BlockData = 0;
            pxLinkedListNodeFixed->NodeNext = 0;

            return PXNo;
        }
        case PXLinkedListNodeNoNext:
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

PXBool PXLinkedListFixedNodeNext(PXLinkedListFixed* const linkedListFixed, PXLinkedListNodeFixed* const pxLinkedListNodeFixed)
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
        const PXLinkedListNodeFixed* const fetchNode = pxLinkedListNodeFixed->NodeNext;

        pxLinkedListNodeFixed->BlockData = fetchNode->BlockData;
        pxLinkedListNodeFixed->NodeNext = fetchNode->NodeNext;
    }

    return PXYes;
}

PXBool PXLinkedListFixedDataSize(const PXLinkedListFixed* const linkedListFixed)
{
    const PXBool isDataEmbedded = PXLinkedListFixedIsDataEmbedded(linkedListFixed);

    if (isDataEmbedded)
    {
        return linkedListFixed->NodeSize;
    }
    else
    {
        return sizeof(void*);
    }
}

PXBool PXLinkedListFixedIsDataEmbedded(const PXLinkedListFixed* const linkedListFixed)
{
    return linkedListFixed->NodeSize != (PXSize)-1;
}

PXSize PXLinkedListFixedNodeStride(const PXLinkedListFixed* const linkedListFixed)
{
    const PXSize dataSize = PXLinkedListFixedDataSize(linkedListFixed);

    return sizeof(void*) + dataSize;
}

void* PXLinkedListFixedNodeEmptySlotFetch(const PXLinkedListFixed* const linkedListFixed)
{
    PXLinkedListNodeFixed nodeCurrent;

    while (PXLinkedListFixedNodeNext(linkedListFixed, &nodeCurrent));

    return 0;
}

PXBool PXLinkedListFixedNodeAdd(PXLinkedListFixed* const linkedListFixed, void* const element)
{
    const PXBool hasSpace = linkedListFixed->NodeListSizeMaximal >= (linkedListFixed->NodeListSizeCurrent + 1);

    if (!hasSpace)
    {
        return PXNo;
    }

    const PXSize dataBlockSize = PXLinkedListFixedDataSize(linkedListFixed);
    const PXSize nodeStride = PXLinkedListFixedNodeStride(linkedListFixed);
    void* data = (PXAdress)linkedListFixed->Data + (nodeStride * linkedListFixed->NodeListSizeCurrent);
   
    // Add current data/adress
    {
        const PXBool isDataEmbedded = PXLinkedListFixedIsDataEmbedded(linkedListFixed);
        const void* const dataSource = isDataEmbedded ? element : &element;

        PXMemoryCopy(dataSource, dataBlockSize, data, dataBlockSize); // Copy the whole data into this node or  Copy only the adress to the data into the node
    }   

    // Add next pointer
    {
        const PXSize targetSize = sizeof(void*);
        const void* value = PXLinkedListNodeNoNext;
        void* const target = (PXAdress)data + dataBlockSize;

        PXMemoryCopy(&value, targetSize, target, targetSize);
    } 

    // Fetch Last and register this node into it
    {
        if (linkedListFixed->DataEntryLast != data)
        {
            void* const lastOBJ = (PXAdress)linkedListFixed->DataEntryLast + dataBlockSize;

            PXMemoryCopy(&data, sizeof(void*), lastOBJ, sizeof(void*));

            linkedListFixed->DataEntryLast = data;
        }      
    }    

    ++(linkedListFixed->NodeListSizeCurrent);

    return PXYes;
}

PXBool PXLinkedListFixedNodeRemoveAt(PXLinkedListFixed* const linkedListFixed, const PXSize index)
{
    PXLinkedListNodeFixed pxLinkedListNodeFixed;

    const PXBool found = PXLinkedListFixedNodeAt(linkedListFixed, &pxLinkedListNodeFixed, index);

    if (!found)
    {
        return PXNo;            
    }

    pxLinkedListNodeFixed.NodeNext = PXLinkedListNodeNotUsed;
}