#include "PXFlexDataCache.h"

#include <OS/Memory/PXMemory.h>

void* PXAPI PXFlexDataCacheAdd(PXFlexDataCache* const pxFlexDataCache, const char* const key, const char* const data, const PXSize dataSize)
{
    const PXSize rowSizeNew = pxFlexDataCache->KeySize + pxFlexDataCache->SizeInBytes + dataSize + pxFlexDataCache->DoNullTerminate; //Key + datasize + data
    const PXSize requiredSize = pxFlexDataCache->DataSizeUsed + rowSizeNew;

    // Validate parameters


    // Do we have enough space?
    {
        const PXBool hasEnoughSize = (pxFlexDataCache->DataSizeAllocated - pxFlexDataCache->DataSizeUsed) >= requiredSize;

        if(!hasEnoughSize) 
        {
            // We dont have enough space, do we allocate or fail?

            if(pxFlexDataCache->ReallocationAllow)
            {
                // Reallocate memory now

                pxFlexDataCache->DataAdress = (char*)PXMemoryRealloc(pxFlexDataCache->DataAdress, requiredSize);
                pxFlexDataCache->DataSizeAllocated = requiredSize + pxFlexDataCache->DataSizeUsed;
            }
        }
    }


    // Get current insert point to store data into
    char* insertAdress = pxFlexDataCache->DataAdress + pxFlexDataCache->InsertionPointOffset;
    
    // Write Key
    insertAdress += PXMemoryCopy(key, pxFlexDataCache->KeySize, insertAdress, pxFlexDataCache->KeySize);
    
    // Write size for string
    switch(pxFlexDataCache->SizeInBytes)
    {
        case PXFlexDataCacheSizeObject1Byte:
        {
            *(PXInt8U*)insertAdress = dataSize;
            break;
        }
        case PXFlexDataCacheSizeObject2Byte:
        {
            *(PXInt16U*)insertAdress = dataSize;
            break;
        }
        case PXFlexDataCacheSizeObject4Byte:
        {
            *(PXInt32U*)insertAdress = dataSize;
            break;
        }
        case PXFlexDataCacheSizeObject8Byte:
        {
            *(PXInt64U*)insertAdress = dataSize;
            break;
        }
        default:
            break;
    }

    insertAdress += pxFlexDataCache->SizeInBytes;

    // Writze Data
    char* insertionBase = insertAdress;
    insertAdress += PXMemoryCopy(data, dataSize, insertAdress, dataSize);
    

     pxFlexDataCache->DataSizeUsed += rowSizeNew;
    pxFlexDataCache->InsertionPointOffset += rowSizeNew;
    ++(pxFlexDataCache->EntryAmount); // increse the amount of items we have stored

    // Special behaviour for using text. We need to add a \0 to be allowed to use this as a direct reference
    if(pxFlexDataCache->DoNullTerminate) // is String
    {
        insertAdress[0] = '\0';
    }

    return insertionBase;
}

void PXAPI PXFlexDataCacheGet(PXFlexDataCache* const pxFlexDataCache, const char* const key, char** data, PXSize* dataSize)
{
    char* dataCursor = pxFlexDataCache->DataAdress;

    for(PXSize i = 0; i < pxFlexDataCache->EntryAmount; i++)
    {
        const PXBool isTarget = PXMemoryCompare(dataCursor, pxFlexDataCache->KeySize, key, pxFlexDataCache->KeySize);
        dataCursor += pxFlexDataCache->KeySize;


        PXSize dataLength = 0; 

        switch(pxFlexDataCache->SizeInBytes)
        {
            case PXFlexDataCacheSizeObject1Byte:
            {
                dataLength = *(PXInt8U*)dataCursor;
                break;
            }
            case PXFlexDataCacheSizeObject2Byte:
            {
                dataLength = *(PXInt16U*)dataCursor;
                break;
            }
            case PXFlexDataCacheSizeObject4Byte:
            {
                dataLength = *(PXInt32U*)dataCursor;
                break;
            }
            case PXFlexDataCacheSizeObject8Byte:
            {
                dataLength = *(PXInt64U*)dataCursor;
                break;
            }
        }
        dataCursor += pxFlexDataCache->SizeInBytes;     

        if(isTarget)
        {
            *dataSize = dataLength;
            *data = dataCursor;

            return;
        }

        dataCursor += dataLength + 1;
    }

    *data = 0;
    *dataSize = 0;
}

void PXAPI PXFlexDataCacheInit(PXFlexDataCache* const pxFlexDataCache, const PXSize keySize, const PXInt8U sizeInBytes)
{
    PXClear(PXFlexDataCache, pxFlexDataCache);

    pxFlexDataCache->KeySize = keySize;
    pxFlexDataCache->ReallocationAllow = PXTrue; 
    pxFlexDataCache->DoNullTerminate = PXTrue;
    pxFlexDataCache->SizeInBytes = sizeInBytes;
}