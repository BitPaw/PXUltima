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

    PXListCreate(&pxDictionary->List, pxDictionaryCreateInfo->ValueSize, 0);
    pxDictionary->List.EntryGrowthOnAllocation = 128;

    return PXResultOK;
}

PXResult PXAPI PXDictionaryRelease(PXDictionary PXREF dictionary)
{
    PXListRelease(&dictionary->List);

    return PXResultOK;
}

PXSize PXAPI PXDictionaryValueSize(const PXDictionary PXREF dictionary)
{
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

void PXAPI PXDictionaryResize(PXDictionary PXREF dictionary, const PXSize entrys)
{
    const PXSize rowSize = dictionary->KeyTypeSize + PXDictionaryValueSize(dictionary);
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

        if(PXActionRefusedIndexOutOfBounce == pxResult)
        {
            PXDictionaryIndexUnsafe(pxDictionary, i, &target);

            found = PXTrue;
            break;
        }

        const PXBool isEmptyKeyField = PXMemoryCompareToByte(target.KeyAddress, pxDictionary->KeyTypeSize, 0xFF);

        if(isEmptyKeyField)
        {
            found = target.KeyAddress && target.ValueAdress;
            break;
        }
    }

    if(!found)
    {
        return PXActionRefusedNotFound;
    }

    // Copy Key
    PXMemoryCopy(key, target.KeyAddress, pxDictionary->KeyTypeSize);

    if(*value)
    {
        // We have data, insert it!
        PXMemoryCopy(*value, target.ValueAdress, pxDictionary->List.ValueTypeSize);
    }
    else
    {
        // We dont have attached data, return insertion point
        *value = target.ValueAdress;
    }

    return PXResultOK;
}

PXResult PXAPI PXDictionaryEntryAdd(PXDictionary PXREF pxDictionary, const void* key, const void* value)
{
    if(!(pxDictionary && key && value))
    {
        return PXResultRefusedParameterNull;
    }

    const void* valueSourceAdress = 0;
    void* valueTargetAdress = 0;
    PXSize valueSize = 0;

    const PXResult pxResult = PXDictionaryEntryCreate(pxDictionary, key, &valueTargetAdress);

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
            valueSourceAdress = value;
            valueSize = pxDictionary->List.ValueTypeSize;
            break;
        }
        case PXDictionaryValueLocalityExternalReference:
        {
            valueSourceAdress = &value;
            valueSize = sizeof(void*);
            break;
        }
    }

    PXMemoryCopy(valueSourceAdress, valueTargetAdress, valueSize);

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
        valueTargetAdress,
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

    const void* valueSourceAdress = 0;
    void* valueTargetAdress = 0;

    const PXResult pxResult = PXDictionaryEntryCreate(pxDictionary, key, &valueTargetAdress);

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
            valueSourceAdress = value;
            break;
        }
        case PXDictionaryValueLocalityExternalReference:
        {
            valueSourceAdress = &value;
            break;
        }
    }

    PXMemoryCopy(valueSourceAdress, valueTargetAdress, valueSize);

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
    void* valteAdress = 0;

    PXDictionaryEntry pxDictionaryEntry;
    pxDictionaryEntry.KeyAddress = key;
    pxDictionaryEntry.KeySize = dictionary->KeyTypeSize;
    pxDictionaryEntry.ValueAdress = &valteAdress;
    pxDictionaryEntry.ValueSize = dictionary->List.ValueTypeSize;

    const PXBool found = PXDictionaryEntryFind(dictionary, &pxDictionaryEntry);

    if(!found)
    {
        PXMemoryClear(value, dictionary->List.ValueTypeSize);
        return PXFalse;
    }

    void* data = (char*)valteAdress + dictionary->KeyTypeSize;

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

    PXDictionaryRemoveFound(dictionary, valteAdress, data);

    return PXTrue;
}

void PXAPI PXDictionaryIndexUnsafe(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry)
{
    const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);
    const PXSize blockSize = dictionary->KeyTypeSize + dataBlockSize;
    const PXByte* blockStart = ((PXByte*)dictionary->List.Buffer.Data4) + blockSize * index;

    pxDictionaryEntry->KeyAddress = (void*)blockStart;
    pxDictionaryEntry->ValueAdress = (void*)(blockStart + dictionary->KeyTypeSize);
}

PXResult PXAPI PXDictionaryIndex(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry)
{
    pxDictionaryEntry->KeyAddress = 0;
    pxDictionaryEntry->ValueAdress = 0;

    if(!dictionary->List.Buffer.Data4)
    {
        return PXResultRefusedParameterInvalid;
    }

    // Range check
    {
        PXBool is = dictionary->List.EntryAmountUsed > index;

        if(!is)
        {
            return PXActionRefusedIndexOutOfBounce;
        }
    }

    PXDictionaryIndexUnsafe(dictionary, index, pxDictionaryEntry);

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
                pxDictionaryEntrya->ValueAdress = pxDictionaryEntryCurrent.ValueAdress;
                break;
            }
            case PXDictionaryValueLocalityExternalReference:
            {
                pxDictionaryEntrya->ValueAdress = *(void**)pxDictionaryEntryCurrent.ValueAdress;
                break;
            }
        }

        return PXResultOK;
    }

    pxDictionaryEntrya->ValueAdress = PXNull;

    return PXActionRefusedNotFound;
}