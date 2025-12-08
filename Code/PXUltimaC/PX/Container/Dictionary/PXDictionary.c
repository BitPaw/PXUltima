#include "PXDictionary.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXText.h>
#include <PX/OS/PXOS.h>

const char PXDictionaryName[] = "Dictionary";

void PXAPI PXDictionaryConstruct(PXDictionary PXREF dictionary, const PXSize keySize, const PXSize valueSize, const PXDictionaryValueLocality pxDictionaryValueLocality)
{
    PXClear(PXDictionary, dictionary);

    dictionary->ValueLocality = pxDictionaryValueLocality;
    dictionary->KeyTypeSize = keySize;

    PXListInitialize(&dictionary->List, valueSize, 0);
    dictionary->List.EntryGrowthOnAllocation = 128;
}

void PXAPI PXDictionaryDestruct(PXDictionary PXREF dictionary)
{
    PXListRelease(&dictionary->List);
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
    if(!(pxDictionary && key))
    {
        return PXActionRefusedArgumentNull;
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

    PXDictionaryEntry target;
    target.Key = 0;
    target.Value = 0;

    for(PXSize i = 0; i < pxDictionary->List.EntryAmountAllocated; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(pxDictionary, i, &pxDictionaryEntry);

        const PXBool isEmptyKeyField = PXMemoryCompareToByte(pxDictionaryEntry.Key, pxDictionary->KeyTypeSize, 0x00);

        if(isEmptyKeyField)
        {
            target = pxDictionaryEntry;
            break;
        }
    }

    PXBool found = target.Key && target.Value;

    if(found && value)
    {
        // Copy Key
        PXMemoryCopy(key, target.Key, pxDictionary->KeyTypeSize);

        *value = target.Value; 

        return PXActionSuccessful;
    }
    else
    {
        return PXActionInvalid;
    } 
}

PXResult PXAPI PXDictionaryEntryAdd(PXDictionary PXREF pxDictionary, const void* key, const void* value)
{
    if(!(pxDictionary && key && value))
    {
        return PXActionRefusedArgumentNull;
    }

    const void* valueSourceAdress = 0;
    void* valueTargetAdress = 0;
    PXSize valueSize = 0;

    const PXResult pxResult = PXDictionaryEntryCreate(pxDictionary, key, &valueTargetAdress);

    if(PXActionSuccessful != pxResult)
    {
        return pxResult;
    }

    switch(pxDictionary->ValueLocality)
    {
        default:
        case PXDictionaryValueLocalityInvalid:
        {
            return PXActionInvalid; // Illegal call
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

PXResult PXAPI PXDictionaryEntryAddRange(PXDictionary PXREF pxDictionary, const void* key, const void* value, const PXSize valueSize)
{
    return PXActionSuccessful;
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

    const PXBool found = PXDictionaryEntryFind(dictionary, key, &valteAdress);

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

void PXAPI PXDictionaryIndex(const PXDictionary PXREF dictionary, const PXSize index, PXDictionaryEntry PXREF pxDictionaryEntry)
{
    if(!dictionary->List.Buffer.Data)
    {
        pxDictionaryEntry->Key = 0;
        pxDictionaryEntry->Value = 0;
        return;
    }

    const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);
    const PXSize blockSize = dictionary->KeyTypeSize + dataBlockSize;
    const PXByte* blockStart = ((PXByte*)dictionary->List.Buffer.Data) + blockSize * index;

    pxDictionaryEntry->Key = (void*)blockStart;
    pxDictionaryEntry->Value = (void*)(blockStart + dictionary->KeyTypeSize);
}

PXResult PXAPI PXDictionaryEntryFind(PXDictionary PXREF pxDictionary, const void PXREF key, void* PXREF valueResult)
{
    for(PXSize i = 0; i < pxDictionary->List.EntryAmountUsed; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(pxDictionary, i, &pxDictionaryEntry);

        const PXBool isTarget = PXMemoryCompare(pxDictionaryEntry.Key, pxDictionary->KeyTypeSize, key, pxDictionary->KeyTypeSize);

        if(!isTarget)
        {
            continue; // Not our match
        }

        switch(pxDictionary->ValueLocality)
        {
            default:
            case PXDictionaryValueLocalityInvalid:
            {
                return PXActionInvalid; // Illegal call
            }
            case PXDictionaryValueLocalityInternalEmbedded:
            {
                *valueResult = pxDictionaryEntry.Value;
                break;
            }
            case PXDictionaryValueLocalityExternalReference:
            {
                *valueResult = *(void**)pxDictionaryEntry.Value;
                break;
            }
        }

        return PXActionSuccessful;
    }

    *valueResult = PXNull;

    return PXActionRefusedNotFound;
}