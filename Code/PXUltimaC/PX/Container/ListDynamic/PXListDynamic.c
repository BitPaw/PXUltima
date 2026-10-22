#include "PXListDynamic.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/PXOS.h>

void* PXAPI PXListDynamicAdd(PXListDynamic PXREF pxListDynamic, const void PXREF key, const void PXREF data, const PXSize dataSize)
{
    if(!(pxListDynamic && key && data && dataSize))
    {
        return  PXNull;//PXResultRefusedParameterNull;
    }

    const PXSize payLoadSize = dataSize + pxListDynamic->NullPaddingAmount;
    const PXSize sizeRow = pxListDynamic->KeySize + pxListDynamic->SizeInBytesOfLengthKey + payLoadSize; //Key + datasize + data

    // Validate parameters
    PXBufferEnsureAdditional(&pxListDynamic->Buffer, sizeRow);

    // Get current insert point to store data into
    PXByte* insertAddress = (PXByte*)PXListDynamicInsertFind(pxListDynamic);

    if(!insertAddress)
    {
        return PXNull;// PXResultRefusedNotEnoughMemory;
    }

    // Write Key
    insertAddress += PXMemoryCopy(key, insertAddress, pxListDynamic->KeySize);
    
    // Write size for string
    switch(pxListDynamic->SizeInBytesOfLengthKey)
    {
        case PXListDynamicSizeObject1Byte:
        {
            *(PXI8U*)insertAddress = dataSize;
            break;
        }
        case PXListDynamicSizeObject2Byte:
        {
            *(PXI16U*)insertAddress = dataSize;
            break;
        }
        case PXListDynamicSizeObject4Byte:
        {
            *(PXI32U*)insertAddress = dataSize;
            break;
        }
        case PXListDynamicSizeObject8Byte:
        {
            *(PXI64U*)insertAddress = dataSize;
            break;
        }
        default:
            break;
    }

    insertAddress += pxListDynamic->SizeInBytesOfLengthKey;

    // DEBUG
    //PXMemorySet(insertAddress, 0xFF, dataSize + pxListDynamic->NullPaddingAmount);

    // Writze Data
    PXMemoryCopy(data, insertAddress, dataSize);

    // Special behaviour for using text. We need to add a \0 to be allowed to use this as a direct reference
    PXMemoryClear(&insertAddress[dataSize], pxListDynamic->NullPaddingAmount);

    //pxListDynamic->InsertionPointOffset += rowSizeNew;
    ++(pxListDynamic->EntryAmount); // increse the amount of items we have stored

    return insertAddress;
}

PXResult PXAPI PXListDynamicGet(PXListDynamic PXREF pxListDynamic, const void PXREF key, char** data, PXSize* dataSize)
{
    PXByte* dataAddress = pxListDynamic->Buffer.Data;

    if(!dataAddress)
    {
        return PXResultRefusedNotEnoughMemory;
    }

    for(PXSize i = 0; i < pxListDynamic->EntryAmount; i++)
    {
        const PXBool isTarget = PXMemoryCompareN(dataAddress, key, pxListDynamic->KeySize);
               
        dataAddress += pxListDynamic->KeySize;

        PXSize dataLength = 0; 

        switch(pxListDynamic->SizeInBytesOfLengthKey)
        {
            case PXListDynamicSizeObject1Byte:
            {
                dataLength = *(PXI8U*)dataAddress;
                break;
            }
            case PXListDynamicSizeObject2Byte:
            {
                dataLength = *(PXI16U*)dataAddress;
                break;
            }
            case PXListDynamicSizeObject4Byte:
            {
                dataLength = *(PXI32U*)dataAddress;
                break;
            }
            case PXListDynamicSizeObject8Byte:
            {
                dataLength = *(PXI64U*)dataAddress;
                break;
            }
        }
        dataAddress += pxListDynamic->SizeInBytesOfLengthKey;

        if(isTarget)
        {
            if(dataSize)
            {
                *dataSize = dataLength;
            }

            *data = dataAddress;

            return;
        }

        dataAddress += dataLength + pxListDynamic->NullPaddingAmount;
    }

    if(dataSize)
    {
        *dataSize = 0;
    }

    *data = 0;

    return PXResultOK;
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

        const PXBool isTarget = PXMemoryCompareN(data, &emptyKey, pxListDynamic->KeySize);
   
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