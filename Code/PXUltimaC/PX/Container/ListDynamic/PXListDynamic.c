#include "PXListDynamic.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>

void* PXAPI PXListDynamicAdd(PXListDynamic PXREF pxListDynamic, const void PXREF key, const char PXREF data, const PXSize dataSize)
{
    if(!(pxListDynamic && key && data && dataSize))
    {
        return PXResultRefusedParameterNull;
    }

    const PXSize payLoadSize = dataSize + pxListDynamic->NullPaddingAmount;
    const PXSize sizeRow = pxListDynamic->KeySize + pxListDynamic->SizeInBytesOfLengthKey + payLoadSize; //Key + datasize + data

    // Validate parameters
    PXBufferEnsureAdditional(&pxListDynamic->Buffer, sizeRow);

    // Get current insert point to store data into
    PXByte* insertAdress = PXListDynamicInsertFind(pxListDynamic);

    if(!insertAdress)
    {
        return PXResultRefusedNotEnoughMemory;
    }

    // Write Key
    insertAdress += PXMemoryCopy(key, insertAdress, pxListDynamic->KeySize);
    
    // Write size for string
    switch(pxListDynamic->SizeInBytesOfLengthKey)
    {
        case PXListDynamicSizeObject1Byte:
        {
            *(PXI8U*)insertAdress = dataSize;
            break;
        }
        case PXListDynamicSizeObject2Byte:
        {
            *(PXI16U*)insertAdress = dataSize;
            break;
        }
        case PXListDynamicSizeObject4Byte:
        {
            *(PXI32U*)insertAdress = dataSize;
            break;
        }
        case PXListDynamicSizeObject8Byte:
        {
            *(PXI64U*)insertAdress = dataSize;
            break;
        }
        default:
            break;
    }

    insertAdress += pxListDynamic->SizeInBytesOfLengthKey;

    // DEBUG
    //PXMemorySet(insertAdress, 0xFF, dataSize + pxListDynamic->NullPaddingAmount);

    // Writze Data
    PXMemoryCopy(data, insertAdress, dataSize);

    // Special behaviour for using text. We need to add a \0 to be allowed to use this as a direct reference
    PXMemoryClear(&insertAdress[dataSize], pxListDynamic->NullPaddingAmount);

    //pxListDynamic->InsertionPointOffset += rowSizeNew;
    ++(pxListDynamic->EntryAmount); // increse the amount of items we have stored

    return insertAdress;
}

void PXAPI PXListDynamicGet(PXListDynamic PXREF pxListDynamic, const void PXREF key, char** data, PXSize* dataSize)
{
    PXByte* dataAdress = pxListDynamic->Buffer.Data;

    if(!dataAdress)
    {
        return PXResultRefusedNotEnoughMemory;
    }

    for(PXSize i = 0; i < pxListDynamic->EntryAmount; i++)
    {
        const PXBool isTarget = PXMemoryCompare(dataAdress, pxListDynamic->KeySize, key, pxListDynamic->KeySize);
               
        dataAdress += pxListDynamic->KeySize;

        PXSize dataLength = 0; 

        switch(pxListDynamic->SizeInBytesOfLengthKey)
        {
            case PXListDynamicSizeObject1Byte:
            {
                dataLength = *(PXI8U*)dataAdress;
                break;
            }
            case PXListDynamicSizeObject2Byte:
            {
                dataLength = *(PXI16U*)dataAdress;
                break;
            }
            case PXListDynamicSizeObject4Byte:
            {
                dataLength = *(PXI32U*)dataAdress;
                break;
            }
            case PXListDynamicSizeObject8Byte:
            {
                dataLength = *(PXI64U*)dataAdress;
                break;
            }
        }
        dataAdress += pxListDynamic->SizeInBytesOfLengthKey;

        if(isTarget)
        {
            if(dataSize)
            {
                *dataSize = dataLength;
            }

            *data = dataAdress;

            return;
        }

        dataAdress += dataLength + pxListDynamic->NullPaddingAmount;
    }

    if(dataSize)
    {
        *dataSize = 0;
    }

    *data = 0;
}

void PXAPI PXListDynamicClearAll(PXListDynamic PXREF pxListDynamic)
{
    PXMemoryClear(&pxListDynamic->Buffer.Data, pxListDynamic->Buffer.SizeAllowedToUse);
    pxListDynamic->EntryAmount = 0;
}

void PXAPI PXListDynamicInit(PXListDynamic PXREF pxListDynamic, const PXSize keySize, const PXI8U sizeInBytes)
{
    PXClear(PXListDynamic, pxListDynamic);

    pxListDynamic->KeySize = keySize;
    pxListDynamic->ReallocationAllow = PXTrue; 
    pxListDynamic->NullPaddingAmount = sizeof(wchar_t);
    pxListDynamic->SizeInBytesOfLengthKey = sizeInBytes;
}

void* PXAPI PXListDynamicInsertFind(PXListDynamic PXREF pxListDynamic)
{
    if(!pxListDynamic)
    {
        return PXNull;
    }

    PXBuffer PXREF pxBuffer = &pxListDynamic->Buffer;

    for(PXSize index = 0; index < pxBuffer->SizeAllowedToUse; ++index)
    {
        PXByte* data = &pxBuffer->Data[index];

        PXSize payLoadSize = 0;
        PXI64U emptyKey = 0;

        const PXBool isTarget = PXMemoryCompare(data, pxListDynamic->KeySize, &emptyKey, pxListDynamic->KeySize);
   
        if(isTarget)
        {
            return data;
        }

        data += pxListDynamic->KeySize;

        switch(pxListDynamic->SizeInBytesOfLengthKey)
        {
            case PXListDynamicSizeObject1Byte:
            {
                payLoadSize = *(PXI8U*)data;                
                break;
            }
            case PXListDynamicSizeObject2Byte:
            {
                payLoadSize = *(PXI16U*)data;
                break;
            }
            case PXListDynamicSizeObject4Byte:
            {
                payLoadSize = *(PXI32U*)data;
                break;
            }
            case PXListDynamicSizeObject8Byte:
            {
                payLoadSize = *(PXI64U*)data;
                break;
            }
        }

        index += 
            pxListDynamic->KeySize + 
            pxListDynamic->SizeInBytesOfLengthKey + 
            payLoadSize + 
            pxListDynamic->NullPaddingAmount
            -1;
    }

    return PXNull;
}