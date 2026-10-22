#include "PXDictionary.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Type/PXText.h>
#include <PX/OS/PXOS.h>

const char PXDictionaryName[] = "Dictionary";
const PXI8U PXDictionaryNameLength = sizeof(PXDictionaryName);
const PXECSRegisterInfoStatic PXDictionaryInfoStatic =
{
    {sizeof(PXDictionaryName), sizeof(PXDictionaryName), PXDictionaryName, TextFormatASCII},
    sizeof(PXDictionary),
    __alignof(PXDictionary),
    PXECSTypeComponent,
    PXDictionaryCreate,
    PXDictionaryRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXDictionaryInfoDynamic;

void PXAPI PXDictionaryRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXDictionaryInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXDictionaryInfoDynamic;
}

PXResult PXAPI PXDictionaryCreate(PXDictionary** pxDictionaryREF, PXDictionaryCreateInfo PXREF pxDictionaryCreateInfo)
{
    PXDictionary* pxDictionary = PXNull;

    if(!(pxDictionaryREF && pxDictionaryCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxDictionaryCreateInfo->Info.Static = &PXDictionaryInfoStatic;
    pxDictionaryCreateInfo->Info.Dynamic = &PXDictionaryInfoDynamic;
    PXResult pxResult = PXECSCreate(pxDictionaryREF, pxDictionaryCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxDictionary = *pxDictionaryREF;
    pxDictionary->ValueLocality = pxDictionaryCreateInfo->ValueLocality;
    pxDictionary->KeyTypeSize = pxDictionaryCreateInfo->KeySize;
    pxDictionary->List.EntryGrowthOnAllocation = 128;

    PXAssert(pxDictionaryCreateInfo->ValueSize > 0, "We need values to store.");

    PXListCreate(&pxDictionary->List, pxDictionaryCreateInfo->ValueSize, pxDictionaryCreateInfo->StartAmount);

    return PXResultOK;
}

PXResult PXAPI PXDictionaryRelease(PXDictionary PXREF dictionary)
{
    PXListRelease(&dictionary->List);

    return PXResultOK;
}

PXSize PXAPI PXDictionaryValueSize(const PXDictionary PXREF dictionary)
{
    if(!dictionary)
    {
        return 0;
    }

    switch(dictionary->ValueLocality)
    {
        default:
        case PXDictionaryValueLocalityInvalid:
            return 0;

        case PXDictionaryValueLocalityInternalEmbedded:
            return dictionary->List.ValueTypeSize;

        case PXDictionaryValueLocalityExternalReference:
            return sizeof(void*);
    }
}

PXSize PXAPI PXDictionaryEntryAmount(const PXDictionary PXREF dictionary)
{
    return dictionary->List.EntryAmountUsed;
}

PXSize PXAPI PXDictionaryRowSize(const PXDictionary PXREF dictionary)
{
    return dictionary->KeyTypeSize + PXDictionaryValueSize(dictionary);
}

void PXAPI PXDictionaryResize(PXDictionary PXREF dictionary, const PXSize entrys)
{
    const PXSize rowSize = PXDictionaryRowSize(dictionary); 
    const PXSize fullSize = rowSize * entrys;

    //const PXSize oldPositionOffset = dictionary->EntryAmountMaximal * rowSize;
    //const PXSize oldPositionOffset = dictionary->EntryAmountMaximal * rowSize;
    //const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);

    PXListReserve(&dictionary->List, fullSize);
}

PXResult PXAPI PXDictionaryEntryCreate(PXDictionary PXREF pxDictionary, const void* key, void** value)
{
    if(!(pxDictionary && key && value))
    {
        return PXResultRefusedParameterNull;
    }

    const PXBool hasEnoughSpace = (pxDictionary->List.EntryAmountUsed + 1) < pxDictionary->List.EntryAmountAllocated;

    if(!hasEnoughSpace)
    {
#if PXLogEnable
        const PXSize sizeBefore = pxDictionary->List.EntryAmountAllocated;
#endif

        PXDictionaryResize(pxDictionary, pxDictionary->List.EntryAmountUsed + pxDictionary->List.EntryGrowthOnAllocation);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            PXDictionaryName,
            "Resize",
            "Size not sufficent. %i -> %i (%3i%%)",
            sizeBefore,
            pxDictionary->List.EntryAmountAllocated,
            (PXI32S)((pxDictionary->List.EntryAmountUsed / (PXF32)pxDictionary->List.EntryAmountAllocated) * 100.0f)
        );
#endif
    }

    PXBool found = PXFalse;

    PXDictionaryEntry target;
    PXClear(PXDictionaryEntry, &target);

    for(PXSize i = 0; i < pxDictionary->List.EntryAmountAllocated; ++i)
    {
        PXResult pxResult = PXDictionaryIndex(pxDictionary, i, &target);

        if(PXResultRefusedIndexOutOfBounce == pxResult)
        {
            PXDictionaryIndexUnsafe(pxDictionary, i, &target);

            found = PXTrue;
            break;
        }

        const PXBool isEmptyKeyField = PXMemoryCompareToByte(target.KeyAddress, pxDictionary->KeyTypeSize, 0xFF);

        if(isEmptyKeyField)
        {
            found = target.KeyAddress && target.ValueAddress;
            break;
        }
    }

    if(!found)
    {
        return PXResultRefusedNotFound;
    }

    // Copy Key
    PXMemoryCopy(key, target.KeyAddress, pxDictionary->KeyTypeSize);

    if(*value)
    {
        // We have data, insert it!
        PXMemoryCopy(*value, target.ValueAddress, pxDictionary->List.ValueTypeSize);
    }
    else
    {
        // We dont have attached data, return insertion point
        *value = target.ValueAddress;
    }

    return PXResultOK;
}

PXResult PXAPI PXDictionaryEntryAdd(PXDictionary PXREF pxDictionary, const void* key, const void* value)
{
    if(!(pxDictionary && key && value))
    {
        return PXResultRefusedParameterNull;
    }

    const void* valueSourceAddress = 0;
    void* valueTargetAddress = 0;
    PXSize valueSize = 0;

    const PXResult pxResult = PXDictionaryEntryCreate(pxDictionary, key, &valueTargetAddress);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    switch(pxDictionary->ValueLocality)
    {
        default:
        case PXDictionaryValueLocalityInvalid:
        {
            return PXResultRefusedParameterInvalid; // Illegal call
        }
        case PXDictionaryValueLocalityInternalEmbedded:
        {
            valueSourceAddress = value;
            valueSize = pxDictionary->List.ValueTypeSize;
            break;
        }
        case PXDictionaryValueLocalityExternalReference:
        {
            valueSourceAddress = &value;
            valueSize = sizeof(void*);
            break;
        }
    }

    PXSize amount = PXMemoryCopy(valueSourceAddress, valueTargetAddress, valueSize);

    PXAssert(amount == valueSize, "Copy failed!");

    ++pxDictionary->List.EntryAmountUsed;

#if PXLogEnable && 0
    char keyText[64];

    switch(dictionary->KeyTypeSize)
    {
        case 4:
        {
            PXTextPrintA(keyText, 64, "0x%8.8X", *(PXI32U*)key);
            break;
        }
        default:
        case 8:
        {
            PXTextPrintA(keyText, 64, "0x%16.16X", key);
            break;
        }
    }

    PXLogPrint
    (
        PXLoggingEvent,
        "Dictionary",
        "Add",
        "Key:%s Value:0x%p (%3i%%) Size:%i/%i",
        keyText,
        //valueSourceSize,
        valueTargetAddress,
        //valueTargetSize,
        (PXI32S)((dictionary->EntryAmountCurrent / (PXF32)dictionary->EntryAmountMaximal) * 100.0f),
        dictionary->EntryAmountCurrent,
        dictionary->EntryAmountMaximal
    );
#endif

    return pxResult;
}

PXResult PXAPI PXDictionaryEntryAddRange
(
    PXDictionary PXREF pxDictionary,
    const void* key,
    const void* value,
    const PXSize valueSize
)
{
    if(!(pxDictionary && key && value && valueSize))
    {
        return PXResultRefusedParameterNull;
    }

    const void* valueSourceAddress = 0;
    void* valueTargetAddress = 0;

    const PXResult pxResult = PXDictionaryEntryCreate(pxDictionary, key, &valueTargetAddress);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    switch(pxDictionary->ValueLocality)
    {
        default:
        case PXDictionaryValueLocalityInvalid:
        {
            return PXResultRefusedParameterInvalid; // Illegal call
        }
        case PXDictionaryValueLocalityInternalEmbedded:
        {
            valueSourceAddress = value;
            break;
        }
        case PXDictionaryValueLocalityExternalReference:
        {
            valueSourceAddress = &value;
            break;
        }
    }

    PXMemoryCopy(valueSourceAddress, valueTargetAddress, valueSize);

    ++pxDictionary->List.EntryAmountUsed;
}

PXBool PXAPI PXDictionaryEntryAddMultible(PXDictionary PXREF dictionary, const void** keyList, const void** valueList, const PXSize amount)
{
    const PXBool hasEnoughSpace = (dictionary->List.EntryAmountUsed + amount) < dictionary->List.EntryAmountAllocated;

    if(!hasEnoughSpace)
    {
        PXDictionaryResize(dictionary, dictionary->List.EntryAmountUsed + amount + dictionary->List.EntryGrowthOnAllocation);
    }

    for(PXSize i = 0; i < amount; ++i)
    {
        const void* key = keyList[i];
        const void* value = valueList[i];

        PXDictionaryEntryAdd(dictionary, key, value);
    }

    return PXTrue;
}

void PXAPI PXDictionaryRemove(PXDictionary PXREF dictionary, const void* key)
{
    // Find

    // Remove
}

PXBool PXAPI PXDictionaryRemoveFound(PXDictionary PXREF dictionary, const void* key, void PXREF value)
{
    PXMemorySet((void*)key, 0xFF, dictionary->KeyTypeSize);
    PXMemorySet((void*)value, 0xFF, dictionary->List.ValueTypeSize);

    --dictionary->List.EntryAmountUsed;
}

PXBool PXAPI PXDictionaryExtract(PXDictionary PXREF dictionary, const void PXREF key, void PXREF value)
{
    void* valteAddress = 0;

    PXDictionaryEntry pxDictionaryEntry;
    pxDictionaryEntry.KeyAddress = key;
    pxDictionaryEntry.KeySize = dictionary->KeyTypeSize;
    pxDictionaryEntry.ValueAddress = &valteAddress;
    pxDictionaryEntry.ValueSize = dictionary->List.ValueTypeSize;

    const PXBool found = PXDictionaryEntryFind(dictionary, &pxDictionaryEntry);

    if(!found)
    {
        PXMemoryClear(value, dictionary->List.ValueTypeSize);
        return PXFalse;
    }

    void* data = (char*)valteAddress + dictionary->KeyTypeSize;

    switch(dictionary->ValueLocality)
    {
        default:
        case PXDictionaryValueLocalityInvalid:
        {
            return 0; // Illegal call
        }
        case PXDictionaryValueLocalityInternalEmbedded:
        {
            PXMemoryCopy(data, value, dictionary->List.ValueTypeSize);
            break;
        }
        case PXDictionaryValueLocalityExternalReference:
        {
            PXMemoryCopy(data, value, sizeof(void*));
            break;
        }
    }

    PXDictionaryRemoveFound(dictionary, valteAddress, data);

    return PXTrue;
}

void PXAPI PXDictionaryIndexUnsafe(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry)
{
    const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);
    const PXSize blockSize = dictionary->KeyTypeSize + dataBlockSize;
    const PXByte* blockStart = ((PXByte*)dictionary->List.Buffer.Data) + blockSize * index;

    pxDictionaryEntry->KeySize = dictionary->KeyTypeSize;
    pxDictionaryEntry->KeyAddress = (void*)blockStart;

    pxDictionaryEntry->ValueSize = dataBlockSize;
    pxDictionaryEntry->ValueAddress = (void*)(blockStart + dictionary->KeyTypeSize);
}

PXResult PXAPI PXDictionaryIndex(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry)
{
    if(!(dictionary && pxDictionaryEntry))
    {
        return PXResultRefusedParameterNull;
    }

    pxDictionaryEntry->KeyAddress = 0;
    pxDictionaryEntry->ValueAddress = 0;

    if(!dictionary->List.Buffer.Data)
    {
        return PXResultRefusedParameterInvalid;
    }

    // Range check
    {
        PXBool is = dictionary->List.EntryAmountUsed > index;

        if(!is)
        {
            return PXResultRefusedIndexOutOfBounce;
        }
    }

    PXDictionaryIndexUnsafe(dictionary, index, pxDictionaryEntry);

    PXBool validResult =
        pxDictionaryEntry->KeySize &&
        pxDictionaryEntry->KeyAddress &&
        pxDictionaryEntry->ValueSize &&
        pxDictionaryEntry->ValueAddress;

    if(!validResult)
    {
        return PXResultInvalid;
    }

    return PXResultOK;
}

PXResult PXAPI PXDictionaryEntryFind(PXDictionary PXREF pxDictionary, PXDictionaryEntry PXREF pxDictionaryEntrya)
{
    for(PXSize i = 0; i < pxDictionary->List.EntryAmountUsed; ++i)
    {
        PXDictionaryEntry pxDictionaryEntryCurrent;

        PXDictionaryIndex(pxDictionary, i, &pxDictionaryEntryCurrent);

        const PXBool isTarget = PXMemoryCompareN
        (
            pxDictionaryEntryCurrent.KeyAddress,
            pxDictionaryEntrya->KeyAddress,
            pxDictionary->KeyTypeSize
        );

        if(!isTarget)
        {
            continue; // Not our match
        }

        switch(pxDictionary->ValueLocality)
        {
            default:
            case PXDictionaryValueLocalityInvalid:
            {
                return PXResultInvalid; // Illegal call
            }
            case PXDictionaryValueLocalityInternalEmbedded:
            {
                pxDictionaryEntrya->ValueAddress = pxDictionaryEntryCurrent.ValueAddress;
                break;
            }
            case PXDictionaryValueLocalityExternalReference:
            {
                pxDictionaryEntrya->ValueAddress = *(void**)pxDictionaryEntryCurrent.ValueAddress;
                break;
            }
        }

        return PXResultOK;
    }

    pxDictionaryEntrya->ValueAddress = PXNull;

    return PXResultRefusedNotFound;
}