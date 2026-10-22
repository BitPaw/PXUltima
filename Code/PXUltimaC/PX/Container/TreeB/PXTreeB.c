#include "PXTreeB.h"

#include <PX/OS/Memory/PXMemory.h>

void PXAPI PXTreeBInitialize(PXTreeB PXREF pxTreeB, const PXSize keySize, const PXSize valueSize, const PXI8U keyOrder)
{
    pxTreeB->ValueDataAddress = PXNull;
    pxTreeB->ValueAmount = 0;
    pxTreeB->ValueSize = valueSize;
    pxTreeB->KeyDataAddress = PXNull;
    pxTreeB->KeyAmount = 0;
    pxTreeB->KeySize = keySize;
    pxTreeB->KeyOrder = keyOrder;
}

PXResult PXAPI PXTreeBResize(PXTreeB PXREF pxTreeB, const PXSize amount)
{
    // Allocate Data
    {
       // PXMemoryHeapReallocate(pxTreeB->ValueSize, &pxTreeB->ValueDataAddress, &pxTreeB->ValueAmount, amount);
    }

    // Allocate Index
    {
        const PXSize rowSize = pxTreeB->KeyOrder * pxTreeB->KeySize + ((pxTreeB->KeyOrder + 1) * sizeof(void*));

       // PXMemoryHeapReallocate(rowSize, &pxTreeB->KeyDataAddress, &pxTreeB->KeyAmount, amount);
    }

    return PXResultOK;
}

PXResult PXAPI PXTreeBInsert(PXTreeB PXREF pxTreeB, const void PXREF key, const void PXREF value)
{
    PXTreeBNode pxTreeBNode; 

    // Get root
    PXTreeBNodeExtractFromAddress(pxTreeB, &pxTreeBNode, pxTreeB->KeyDataAddress);

    // Try insert into current node
    {
        const PXResult actionResult = PXTreeBNodeInsert(pxTreeB, &pxTreeBNode, key, value); // Add to node and be done.

        if(PXResultOK == actionResult) 
            return actionResult;
    }


    // Go to child
    {
        for (PXSize i = 0; i < pxTreeBNode.KeyListAmount; i++)
        {
            void PXREF keyEntryCurrent = (PXAddress)pxTreeBNode.KeyList + i * pxTreeB->KeySize;

            const int swapElement = PXMemoryCompareThreeWay(key, pxTreeB->KeySize, keyEntryCurrent, pxTreeB->KeySize);

            switch (swapElement)
            {
                case 0: // Item cant be added, already exists
                    return PXResultRefusedAlreadyExists;

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

    return PXResultOK;
}

PXResult PXAPI PXTreeBFindIndexed(PXTreeB PXREF pxTreeB, const void PXREF key, void* PXREF value)
{
    return PXResultRefusedNotImplemented;
}

PXResult PXAPI PXTreeBFindUnindexed(PXTreeB PXREF pxTreeB, const void PXREF key, void* PXREF value)
{
    for (PXSize i = 0; i < pxTreeB->ValueAmount; ++i)
    {       
        void* keyCurrent = PXNull;
        void* valueCurrent = PXNull;

        PXTreeBValueIndex(pxTreeB, i, &keyCurrent, &valueCurrent);

        PXBool isTarget = PXMemoryCompareN(key, keyCurrent, pxTreeB->KeySize);

        if (!isTarget)
        {
            continue;
        }

       *value = valueCurrent;

       return PXResultOK;
    }

    return PXResultFailedElementNotFound;
}

void PXAPI PXTreeBValueIndex(PXTreeB PXREF pxTreeB, const PXSize index, void* PXREF key, void* PXREF value)
{
    *key = (PXAddress)pxTreeB->ValueDataAddress + index * (pxTreeB->KeySize + pxTreeB->ValueSize);
    *value = (PXAddress)*key + pxTreeB->ValueSize;
}

PXResult PXAPI PXTreeBNodeExtractFromAddress(PXTreeB PXREF pxTreeB, PXTreeBNode PXREF pxTreeBNode, void* address)
{
    pxTreeBNode->Data = address; 
    
    pxTreeBNode->KeyList = address;
    pxTreeBNode->NodeList = (PXAddress)pxTreeBNode->KeyList + pxTreeB->KeySize * pxTreeB->KeyOrder;

    // Generate KeyListAmount
    for (PXSize i = 0; i < pxTreeB->KeyOrder; ++i)
    {
        void PXREF keyEntry = (PXAddress)pxTreeBNode->KeyList + pxTreeB->KeySize * i;
        const PXBool isEmpty = PXMemoryIsEmpty(keyEntry, pxTreeB->KeySize);

        pxTreeBNode->KeyListAmount += !isEmpty;
    }

    pxTreeBNode->NodeListAmount = pxTreeBNode->KeyListAmount + 1;

    return PXResultOK;
}

PXResult PXAPI PXTreeBNodeInsert(PXTreeB PXREF pxTreeB, PXTreeBNode PXREF pxTreeBNode, const void PXREF key, const void PXREF value)
{
    // Has enough space 
    {
        const PXBool hasSpace = pxTreeBNode->NodeListAmount < pxTreeB->KeyOrder;

        if (!hasSpace)
        {
            return PXResultRefuedBufferSizeTooSmal;
        }
    }

    // insert element into node
    {
        void PXREF keyEntry = (PXAddress)pxTreeBNode->KeyList + pxTreeBNode->KeyListAmount * pxTreeB->KeySize;
        void PXREF valueEntry = (PXAddress)pxTreeBNode->NodeList + pxTreeBNode->NodeListAmount * pxTreeB->ValueSize;

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
                void PXREF keyEntryCurrent = (PXAddress)pxTreeBNode->KeyList + i * pxTreeB->KeySize;
                void PXREF valueEntryCurrent = (PXAddress)pxTreeBNode->NodeList + i * pxTreeB->ValueSize;
                void PXREF keyEntryNext = (PXAddress)pxTreeBNode->KeyList + (i + 1) * pxTreeB->KeySize;
                void PXREF valueEntryNext = (PXAddress)pxTreeBNode->NodeList + (i + 1) * pxTreeB->ValueSize;

                const int swapElement = PXMemoryCompareThreeWay(keyEntryCurrent, pxTreeB->KeySize, keyEntryNext, pxTreeB->KeySize) > 0;

                switch (swapElement)
                {
                    case 0: // Item cant be added, already exists
                        return PXResultRefusedAlreadyExists;

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

    return PXResultOK;
}