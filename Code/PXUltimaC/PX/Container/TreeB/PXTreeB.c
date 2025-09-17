#include "PXTreeB.h"

#include <PX/OS/Memory/PXMemory.h>

void PXAPI PXTreeBInitialize(PXTreeB* const pxTreeB, const PXSize keySize, const PXSize valueSize, const PXI8U keyOrder)
{
    pxTreeB->ValueDataAdress = PXNull;
    pxTreeB->ValueAmount = 0;
    pxTreeB->ValueSize = valueSize;
    pxTreeB->KeyDataAdress = PXNull;
    pxTreeB->KeyAmount = 0;
    pxTreeB->KeySize = keySize;
    pxTreeB->KeyOrder = keyOrder;
}

PXResult PXAPI  PXTreeBResize(PXTreeB* const pxTreeB, const PXSize amount)
{
    // Allocate Data
    {
       // PXMemoryHeapReallocate(pxTreeB->ValueSize, &pxTreeB->ValueDataAdress, &pxTreeB->ValueAmount, amount);
    }

    // Allocate Index
    {
        const PXSize rowSize = pxTreeB->KeyOrder * pxTreeB->KeySize + ((pxTreeB->KeyOrder + 1) * sizeof(void*));

       // PXMemoryHeapReallocate(rowSize, &pxTreeB->KeyDataAdress, &pxTreeB->KeyAmount, amount);
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXTreeBInsert(PXTreeB* const pxTreeB, const void* const key, const void* const value)
{
    PXTreeBNode pxTreeBNode; 

    // Get root
    PXTreeBNodeExtractFromAdress(pxTreeB, &pxTreeBNode, pxTreeB->KeyDataAdress);

    // Try insert into current node
    {
        const PXActionResult actionResult = PXTreeBNodeInsert(pxTreeB, &pxTreeBNode, key, value); // Add to node and be done.

        PXActionReturnOnSuccess(actionResult);
    }


    // Go to child
    {
        for (PXSize i = 0; i < pxTreeBNode.KeyListAmount; i++)
        {
            void* const keyEntryCurrent = (PXAdress)pxTreeBNode.KeyList + i * pxTreeB->KeySize;

            const int swapElement = PXMemoryCompareThreeWay(key, pxTreeB->KeySize, keyEntryCurrent, pxTreeB->KeySize);

            switch (swapElement)
            {
                case 0: // Item cant be added, already exists
                    return PXActionRefusedAlreadyExists;

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

PXResult PXAPI  PXTreeBFindIndexed(PXTreeB* const pxTreeB, const void* const key, void** const value)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXTreeBFindUnindexed(PXTreeB* const pxTreeB, const void* const key, void** const value)
{
    for (PXSize i = 0; i < pxTreeB->ValueAmount; ++i)
    {       
        void* keyCurrent = PXNull;
        void* valueCurrent = PXNull;

        PXTreeBValueIndex(pxTreeB, i, &keyCurrent, &valueCurrent);

        PXBool isTarget = PXMemoryCompare(key, pxTreeB->KeySize, keyCurrent, pxTreeB->KeySize);

        if (!isTarget)
        {
            continue;
        }

       *value = valueCurrent;

       return PXActionSuccessful;
    }

    return PXActionFailedElementNotFound;
}

void PXAPI PXTreeBValueIndex(PXTreeB* const pxTreeB, const PXSize index, void** const key, void** const value)
{
    *key = (PXAdress)pxTreeB->ValueDataAdress + index * (pxTreeB->KeySize + pxTreeB->ValueSize);
    *value = (PXAdress)*key + pxTreeB->ValueSize;
}

PXResult PXAPI  PXTreeBNodeExtractFromAdress(PXTreeB* const pxTreeB, PXTreeBNode* const pxTreeBNode, void* adress)
{
    pxTreeBNode->Data = adress; 
    
    pxTreeBNode->KeyList = adress;
    pxTreeBNode->NodeList = (PXAdress)pxTreeBNode->KeyList + pxTreeB->KeySize * pxTreeB->KeyOrder;

    // Generate KeyListAmount
    for (PXSize i = 0; i < pxTreeB->KeyOrder; ++i)
    {
        void* const keyEntry = (PXAdress)pxTreeBNode->KeyList + pxTreeB->KeySize * i;
        const PXBool isEmpty = PXMemoryIsEmpty(keyEntry, pxTreeB->KeySize);

        pxTreeBNode->KeyListAmount += !isEmpty;
    }

    pxTreeBNode->NodeListAmount = pxTreeBNode->KeyListAmount + 1;

    return PXActionSuccessful;
}

PXResult PXAPI  PXTreeBNodeInsert(PXTreeB* const pxTreeB, PXTreeBNode* const pxTreeBNode, const void* const key, const void* const value)
{
    // Has enough space 
    {
        const PXBool hasSpace = pxTreeBNode->NodeListAmount < pxTreeB->KeyOrder;

        if (!hasSpace)
        {
            return PXActionRefuedBufferSizeTooSmal;
        }
    }

    // insert element into node
    {
        void* const keyEntry = (PXAdress)pxTreeBNode->KeyList + pxTreeBNode->KeyListAmount * pxTreeB->KeySize;
        void* const valueEntry = (PXAdress)pxTreeBNode->NodeList + pxTreeBNode->NodeListAmount * pxTreeB->ValueSize;

        PXMemoryCopy(key, keyEntry, pxTreeB->KeySize);
        PXMemoryCopy(value, valueEntry, pxTreeB->ValueSize);
    }  

    // Sort node
    {
        PXBool didSwap = PXFalse;

        do
        {
            for (PXSize i = 0; i < pxTreeBNode->NodeListAmount; ++i)
            {
                void* const keyEntryCurrent = (PXAdress)pxTreeBNode->KeyList + i * pxTreeB->KeySize;
                void* const valueEntryCurrent = (PXAdress)pxTreeBNode->NodeList + i * pxTreeB->ValueSize;
                void* const keyEntryNext = (PXAdress)pxTreeBNode->KeyList + (i + 1) * pxTreeB->KeySize;
                void* const valueEntryNext = (PXAdress)pxTreeBNode->NodeList + (i + 1) * pxTreeB->ValueSize;

                const int swapElement = PXMemoryCompareThreeWay(keyEntryCurrent, pxTreeB->KeySize, keyEntryNext, pxTreeB->KeySize) > 0;

                switch (swapElement)
                {
                    case 0: // Item cant be added, already exists
                        return PXActionRefusedAlreadyExists;

                    case -1:
                        continue;

                    case 1: // A > B, Swap elemets
                    {
                        PXMemorySwap(keyEntryCurrent, keyEntryNext, pxTreeB->KeySize);
                        PXMemorySwap(valueEntryCurrent, valueEntryNext, pxTreeB->ValueSize);
                        break;
                    }                 
                }
            }
        }
        while (didSwap);
    }

    ++pxTreeBNode->NodeListAmount;

    return PXActionSuccessful;
}