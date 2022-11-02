#include "PXLinkedList.h"

#include <Memory/PXMemory.h>
#include <Math/PXMath.h>

void PXLinkedListFixedNodeConstruct(PXLinkedListFixed* const linkedListFixed)
{
    MemorySet(linkedListFixed, sizeof(PXLinkedListFixed), 0);
}

void PXLinkedListFixedNodeDestruct(PXLinkedListFixed* const linkedListFixed)
{
    
}

void PXLinkedListFixedNodeSet(PXLinkedListFixed* const linkedListFixed, void* const data, const size_t dataSize, const size_t nodeSize)
{
    linkedListFixed->Data = data;
    linkedListFixed->NodeListSizeCurrent = 0;
    linkedListFixed->NodeListSizeMaximal = dataSize;
    linkedListFixed->NodeSize = nodeSize;

    if(nodeSize != (void*)nodeSize)
    {
        MemorySet(data, nodeSize * dataSize, 0);
    }
    else
    {
        MemorySet(data, sizeof(void*), 0);
    }
}

PXBool PXLinkedListFixedNodeAt(PXLinkedListFixed* const linkedListFixed, PXLinkedListNodeFixed* const pxLinkedListNodeFixed, const size_t index)
{
    const size_t nodeBlockSize = MathMinimumIU(linkedListFixed->NodeSize, sizeof(void*));
    const void* const data = (unsigned char*)linkedListFixed->Data + (nodeBlockSize + sizeof(void*)) * index;
    const PXLinkedListNodeFixed** nextNode = (unsigned char*)data + nodeBlockSize;
    const size_t adress = (void*)*nextNode;

   

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
    }  

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

PXBool PXLinkedListFixedNodeAdd(PXLinkedListFixed* const linkedListFixed, void* const element)
{
    const PXBool hasSpace = linkedListFixed->NodeListSizeMaximal > (linkedListFixed->NodeListSizeCurrent + 1);

    if (!hasSpace)
    {
        return PXNo;
    }
    const size_t nodeBlockSize = MathMinimumIU(linkedListFixed->NodeSize, sizeof(void*));
    void* data = (unsigned char*)linkedListFixed->Data + (nodeBlockSize * linkedListFixed->NodeListSizeCurrent);
    void* value = PXLinkedListNodeNoNext;

    if (linkedListFixed->NodeSize == (void*)-1)
    {
        MemoryCopy(&element, nodeBlockSize, data, nodeBlockSize);
    }
    else
    {
        MemoryCopy(element, nodeBlockSize, data, nodeBlockSize);
    }

    MemoryCopy(&value, linkedListFixed->NodeSize, (unsigned char*)data + nodeBlockSize, sizeof(void*));

    ++(linkedListFixed->NodeListSizeCurrent);

    return PXYes;
}

PXBool PXLinkedListFixedNodeRemoveAt(PXLinkedListFixed* const linkedListFixed, const size_t index)
{
    PXLinkedListNodeFixed pxLinkedListNodeFixed;

    const PXBool found = PXLinkedListFixedNodeAt(linkedListFixed, &pxLinkedListNodeFixed, index);

    if (!found)
    {
        return PXNo;            
    }

    pxLinkedListNodeFixed.NodeNext = PXLinkedListNodeNotUsed;
}