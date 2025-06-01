#include "PXListDynamic.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>

void* PXAPI PXListDynamicAdd(PXListDynamic* const pxListDynamic, const void* const key, const char* const data, const PXSize dataSize)
{
    const PXSize rowSizeNew = pxListDynamic->KeySize + pxListDynamic->SizeInBytes + dataSize + pxListDynamic->DoNullTerminate; //Key + datasize + data
    const PXSize requiredSize = pxListDynamic->DataSizeUsed + rowSizeNew;

    // Validate parameters


    // Do we have enough space?
    {
        const PXBool hasEnoughSize = (pxListDynamic->DataSizeAllocated - pxListDynamic->DataSizeUsed) >= requiredSize;

        if(!hasEnoughSize) 
        {
            // We dont have enough space, do we allocate or fail?

            if(pxListDynamic->ReallocationAllow)
            {
                // Reallocate memory now

                pxListDynamic->DataAdress = (char*)PXMemoryHeapRealloc(PXNull, pxListDynamic->DataAdress, requiredSize);
                pxListDynamic->DataSizeAllocated = requiredSize + pxListDynamic->DataSizeUsed;
            }
        }
    }


    // Get current insert point to store data into
    char* insertAdress = pxListDynamic->DataAdress + pxListDynamic->InsertionPointOffset;
    
    // Write Key
    insertAdress += PXMemoryCopy(key, pxListDynamic->KeySize, insertAdress, pxListDynamic->KeySize);
    
    // Write size for string
    switch(pxListDynamic->SizeInBytes)
    {
        case PXListDynamicSizeObject1Byte:
        {
            *(PXInt8U*)insertAdress = dataSize;
            break;
        }
        case PXListDynamicSizeObject2Byte:
        {
            *(PXInt16U*)insertAdress = dataSize;
            break;
        }
        case PXListDynamicSizeObject4Byte:
        {
            *(PXInt32U*)insertAdress = dataSize;
            break;
        }
        case PXListDynamicSizeObject8Byte:
        {
            *(PXInt64U*)insertAdress = dataSize;
            break;
        }
        default:
            break;
    }

    insertAdress += pxListDynamic->SizeInBytes;

    // Writze Data
    char* insertionBase = insertAdress;
    insertAdress += PXMemoryCopy(data, dataSize, insertAdress, dataSize);
    

     pxListDynamic->DataSizeUsed += rowSizeNew;
    pxListDynamic->InsertionPointOffset += rowSizeNew;
    ++(pxListDynamic->EntryAmount); // increse the amount of items we have stored

    // Special behaviour for using text. We need to add a \0 to be allowed to use this as a direct reference
    if(pxListDynamic->DoNullTerminate) // is String
    {
        insertAdress[0] = '\0';
    }

    return insertionBase;
}

void PXAPI PXListDynamicGet(PXListDynamic* const pxListDynamic, const void* const key, char** data, PXSize* dataSize)
{
    char* dataCursor = pxListDynamic->DataAdress;

    for(PXSize i = 0; i < pxListDynamic->EntryAmount; i++)
    {
        const PXBool isTarget = PXMemoryCompare(dataCursor, pxListDynamic->KeySize, key, pxListDynamic->KeySize);
        dataCursor += pxListDynamic->KeySize;


        PXSize dataLength = 0; 

        switch(pxListDynamic->SizeInBytes)
        {
            case PXListDynamicSizeObject1Byte:
            {
                dataLength = *(PXInt8U*)dataCursor;
                break;
            }
            case PXListDynamicSizeObject2Byte:
            {
                dataLength = *(PXInt16U*)dataCursor;
                break;
            }
            case PXListDynamicSizeObject4Byte:
            {
                dataLength = *(PXInt32U*)dataCursor;
                break;
            }
            case PXListDynamicSizeObject8Byte:
            {
                dataLength = *(PXInt64U*)dataCursor;
                break;
            }
        }
        dataCursor += pxListDynamic->SizeInBytes;     

        if(isTarget)
        {
            if(dataSize)
            {
                *dataSize = dataLength;
            }

            *data = dataCursor;

            return;
        }

        dataCursor += dataLength + 1;
    }

    if(dataSize)
    {
        *dataSize = 0;
    }

    *data = 0;
}

void PXAPI PXListDynamicInit(PXListDynamic* const pxListDynamic, const PXSize keySize, const PXInt8U sizeInBytes)
{
    PXClear(PXListDynamic, pxListDynamic);

    pxListDynamic->KeySize = keySize;
    pxListDynamic->ReallocationAllow = PXTrue; 
    pxListDynamic->DoNullTerminate = PXTrue;
    pxListDynamic->SizeInBytes = sizeInBytes;
}