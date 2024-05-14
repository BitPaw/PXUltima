#include "PXFlexDataCache.h"

#include <OS/Memory/PXMemory.h>

void PXAPI PXFlexDataCacheAdd(PXFlexDataCache* const pxFlexDataCache, const char* const key, const char* const data, const PXSize dataSize)
{
    const PXSize rowSize = pxFlexDataCache->KeySize + sizeof(PXSize) + dataSize;
    const PXSize requiredSize = pxFlexDataCache->DataSizeUsed + rowSize;

    // Validate parameters


    // Do we have enough space?
    {
        const PXBool hasEnoughSize = pxFlexDataCache->DataSizeAllocated >= requiredSize;

        if(!hasEnoughSize) 
        {
            // We dont have enough space, do we allocate or fail?

            if(pxFlexDataCache->ReallocationAllow)
            {
                // Reallocate memory now

                //PXMemoryHeapReallocateEventData pxMemoryHeapReallocateEventData;
                //PXMemoryInfoFill(pxMemoryHeapReallocateEventData, char, dataSize, &pxFlexDataCache->DataAdress, &pxFlexDataCache->DataSizeAllocated, PXTrue);

                //PXMemoryHeapReallocate();

                pxFlexDataCache->DataAdress = (char*)PXMemoryHeapRealloc(pxFlexDataCache->DataAdress, requiredSize);
            }
        }
    }


    // Get current insert point to store data into
    char* insertAdress = pxFlexDataCache->DataAdress + pxFlexDataCache->InsertionPointOffset;
    
    // Write Key
    insertAdress += PXMemoryCopy(key, pxFlexDataCache->KeySize, insertAdress, pxFlexDataCache->KeySize);
    
    // Write size
    *(PXSize*)insertAdress = dataSize; // Store size for string
    insertAdress += sizeof(PXSize);;

    // Writze Data
    insertAdress += PXMemoryCopy(data, dataSize, insertAdress, dataSize);
    
    ++(pxFlexDataCache->EntryAmount); // increse the amount of items we have stored
}

void PXAPI PXFlexDataCacheGet(PXFlexDataCache* const pxFlexDataCache, const char* const key, char** data, PXSize* dataSize)
{
    char* dataCursor = pxFlexDataCache->DataAdress;

    for(size_t i = 0; i < pxFlexDataCache->EntryAmount; i++)
    {
        const PXBool isTarget = PXMemoryCompare(dataCursor, pxFlexDataCache->KeySize, key, pxFlexDataCache->KeySize);

        dataCursor += pxFlexDataCache->KeySize;

        const PXSize length = *(PXSize*)dataCursor;
        dataCursor += sizeof(PXSize);

        if(isTarget)
        {
            *dataSize = length;
            *data = dataCursor;

            return;
        }

        dataCursor += length;
    }

    *data = 0;
    *dataSize = 0;
}

void PXAPI PXFlexDataCacheInit(PXFlexDataCache* const pxFlexDataCache, const PXSize keySize)
{
    PXClear(PXFlexDataCache, pxFlexDataCache);

    pxFlexDataCache->KeySize = keySize;
}