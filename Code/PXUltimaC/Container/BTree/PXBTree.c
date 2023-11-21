#include "PXBTree.h"

#include <OS/Memory/PXMemory.h>

void PXAPI PXBTreeInitialize(PXBTree* const pxBTree, const PXSize keySize, const PXSize valueSize, const PXInt8U keyOrder)
{
    pxBTree->ValueDataAdress = PXNull;
    pxBTree->ValueAmount = 0;
    pxBTree->ValueSize = valueSize;
    pxBTree->KeyDataAdress = PXNull;
    pxBTree->KeyAmount = 0;
    pxBTree->KeySize = keySize;
    pxBTree->KeyOrder = keyOrder;
}

PXActionResult PXAPI PXBTreeResize(PXBTree* const pxBTree, const PXSize amount)
{
    // Allocate Data
    {
        PXMemoryHeapReallocate(pxBTree->ValueSize, &pxBTree->ValueDataAdress, &pxBTree->ValueAmount, amount);
    }

    // Allocate Index
    {
        const PXSize rowSize = pxBTree->KeyOrder * pxBTree->KeySize + ((pxBTree->KeyOrder + 1) * sizeof(void*));

        PXMemoryHeapReallocate(rowSize, &pxBTree->KeyDataAdress, &pxBTree->KeyAmount, amount);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXBTreeInsert(PXBTree* const pxBTree, void* const key, void* const value)
{
    PXBTreeNode pxBTreeNode; 

    // Get root
    PXBTreeNodeExtractFromAdress(pxBTree, &pxBTreeNode, pxBTree->KeyDataAdress);

    // Try insert into current node
    {
        const PXActionResult actionResult = PXBTreeNodeInsert(pxBTree, &pxBTreeNode, key, value); // Add to node and be done.

        PXActionReturnOnSuccess(actionResult);
    }


    // Go to child
    {
        for (PXSize i = 0; i < pxBTreeNode.KeyListAmount; i++)
        {
            void* const keyEntryCurrent = (PXAdress)pxBTreeNode.KeyList + i * pxBTree->KeySize;

            const int swapElement = PXMemoryCompareThreeWay(key, pxBTree->KeySize, keyEntryCurrent, pxBTree->KeySize);

            switch (swapElement)
            {
                case 0: // Item cant be added, already exists
                    return PXActionRefusedObjectAlreadyExists;

                default:
                    break;
            }
        }
    }


    // Break the key row


    // Check how to break up the row. What value needs to be a child?

    // If subrow is full, do it there.



    // Make new row with new value
   








    // 


    // 

    return PXActionSuccessful;
}

PXActionResult PXAPI PXBTreeFindIndexed(PXBTree* const pxBTree, const void* const key, void** const value)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXBTreeFindUnindexed(PXBTree* const pxBTree, const void* const key, void** const value)
{
    for (PXSize i = 0; i < pxBTree->ValueAmount; ++i)
    {       
        void* keyCurrent = PXNull;
        void* valueCurrent = PXNull;

        PXBTreeValueIndex(pxBTree, i, &keyCurrent, &valueCurrent);

        PXBool isTarget = PXMemoryCompare(key, pxBTree->KeySize, keyCurrent, pxBTree->KeySize);

        if (!isTarget)
        {
            continue;
        }

       *value = valueCurrent;

       return PXActionSuccessful;
    }

    return PXActionFailedElementNotFound;
}

void PXAPI PXBTreeValueIndex(PXBTree* const pxBTree, const PXSize index, void** const key, void** const value)
{
    *key = (PXAdress)pxBTree->ValueDataAdress + index * (pxBTree->KeySize + pxBTree->ValueSize);
    *value = (PXAdress)*key + pxBTree->ValueSize;
}

PXActionResult PXAPI PXBTreeNodeExtractFromAdress(PXBTree* const pxBTree, PXBTreeNode* const pxBTreeNode, void* adress)
{
    pxBTreeNode->Data = adress; 
    
    pxBTreeNode->KeyList = adress;
    pxBTreeNode->NodeList = (PXAdress)pxBTreeNode->KeyList + pxBTree->KeySize * pxBTree->KeyOrder;

    // Generate KeyListAmount
    for (PXSize i = 0; i < pxBTree->KeyOrder; ++i)
    {
        void* const keyEntry = (PXAdress)pxBTreeNode->KeyList + pxBTree->KeySize * i;
        const PXBool isEmpty = PXMemoryIsEmpty(keyEntry, pxBTree->KeySize);

        pxBTreeNode->KeyListAmount += !isEmpty;
    }

    pxBTreeNode->NodeListAmount = pxBTreeNode->KeyListAmount + 1;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXBTreeNodeInsert(PXBTree* const pxBTree, PXBTreeNode* const pxBTreeNode, const void* const key, const void* const value)
{
    // Has enough space 
    {
        const PXBool hasSpace = pxBTreeNode->NodeListAmount < pxBTree->KeyOrder;

        if (!hasSpace)
        {
            return PXActionRefuedBufferSizeTooSmal;
        }
    }

    // insert element into node
    {
        void* const keyEntry = (PXAdress)pxBTreeNode->KeyList + pxBTreeNode->KeyListAmount * pxBTree->KeySize;
        void* const valueEntry = (PXAdress)pxBTreeNode->NodeList + pxBTreeNode->NodeListAmount * pxBTree->ValueSize;

        PXMemoryCopy(key, pxBTree->KeySize, keyEntry, pxBTree->KeySize);
        PXMemoryCopy(value, pxBTree->ValueSize, valueEntry, pxBTree->ValueSize);
    }  

    // Sort node
    {
        PXBool didSwap = PXFalse;

        do
        {
            for (PXSize i = 0; i < pxBTreeNode->NodeListAmount; ++i)
            {
                void* const keyEntryCurrent = (PXAdress)pxBTreeNode->KeyList + i * pxBTree->KeySize;
                void* const valueEntryCurrent = (PXAdress)pxBTreeNode->NodeList + i * pxBTree->ValueSize;
                void* const keyEntryNext = (PXAdress)pxBTreeNode->KeyList + (i + 1) * pxBTree->KeySize;
                void* const valueEntryNext = (PXAdress)pxBTreeNode->NodeList + (i + 1) * pxBTree->ValueSize;

                const int swapElement = PXMemoryCompareThreeWay(keyEntryCurrent, pxBTree->KeySize, keyEntryNext, pxBTree->KeySize) > 0;

                switch (swapElement)
                {
                    case 0: // Item cant be added, already exists
                        return PXActionRefusedObjectAlreadyExists;

                    case -1:
                        continue;

                    case 1: // A > B, Swap elemets
                    {
                        PXMemorySwap(keyEntryCurrent, keyEntryNext, pxBTree->KeySize);
                        PXMemorySwap(valueEntryCurrent, valueEntryNext, pxBTree->ValueSize);
                        break;
                    }                 
                }
            }
        }
        while (didSwap);
    }

    ++pxBTreeNode->NodeListAmount;

    return PXActionSuccessful;
}