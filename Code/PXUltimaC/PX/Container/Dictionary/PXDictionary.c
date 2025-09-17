#include "PXDictionary.h"

#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/PXText.h>
#include <PX/OS/PXOS.h>

const char PXDictionaryName[] = "Dictionary";

void PXAPI PXDictionaryConstruct(PXDictionary* const dictionary, const PXSize keySize, const PXSize valueSize, const PXDictionaryValueLocality pxDictionaryValueLocality)
{
    PXClear(PXDictionary, dictionary);

    dictionary->ValueLocality = pxDictionaryValueLocality;
    dictionary->KeyTypeSize = keySize;

    PXListInitialize(&dictionary->List, valueSize, 0);
    dictionary->List.EntryGrowthOnAllocation = 128;
}

void PXAPI PXDictionaryDestruct(PXDictionary* const dictionary)
{
    PXListRelease(&dictionary->List);
}

PXSize PXAPI PXDictionaryValueSize(const PXDictionary* const dictionary)
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

void PXAPI PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys)
{
    const PXSize rowSize = dictionary->KeyTypeSize + PXDictionaryValueSize(dictionary);
    const PXSize fullSize = rowSize * entrys;

    //const PXSize oldPositionOffset = dictionary->EntryAmountMaximal * rowSize;
    //const PXSize oldPositionOffset = dictionary->EntryAmountMaximal * rowSize;
    //const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);

    PXListReserve(&dictionary->List, fullSize);
}

PXResult PXAPI PXDictionaryEntryCreate(PXDictionary* const dictionary, const void* key, const void** value)
{
    if(!(dictionary && key))
    {
        return PXActionRefusedArgumentNull;
    }

    const PXBool hasEnoughSpace = (dictionary->List.EntryAmountUsed + 1) < dictionary->List.EntryAmountAllocated;

    if(!hasEnoughSpace)
    {
#if PXLogEnable
        const PXSize sizeBefore = dictionary->List.EntryAmountAllocated;
#endif

        PXDictionaryResize(dictionary, dictionary->List.EntryAmountUsed + dictionary->List.EntryGrowthOnAllocation);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            PXDictionaryName,
            "Resize",
            "Size not sufficent. %i -> %i (%3i%%)",
            sizeBefore,
            dictionary->List.EntryAmountAllocated,
            (PXI32S)((dictionary->List.EntryAmountUsed / (PXF32)dictionary->List.EntryAmountAllocated) * 100.0f)
        );
#endif
    }

    PXDictionaryEntry target;
    target.Key = 0;
    target.Value = 0;

    for(PXSize i = 0; i < dictionary->List.EntryAmountAllocated; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

        const PXBool isEmptyKeyField = PXMemoryCompareToByte(pxDictionaryEntry.Key, dictionary->KeyTypeSize, 0x00);

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
        PXMemoryCopy(key, target.Key, dictionary->KeyTypeSize);

        *value = target.Value; 

        return PXActionSuccessful;
    }
    else
    {
        return PXActionInvalid;
    } 
}

PXResult PXAPI PXDictionaryEntryAdd(PXDictionary* const dictionary, const void* key, const void* value)
{
    if(!(dictionary && key && value))
    {
        return PXActionRefusedArgumentNull;
    }

    void* valueSourceAdress = 0;
    void* valueTargetAdress = 0;
    PXSize valueSize = 0;

    const PXResult pxResult = PXDictionaryEntryCreate(dictionary, key, &valueTargetAdress);

    if(PXActionSuccessful != pxResult)
    {
        return pxResult;
    }

    switch(dictionary->ValueLocality)
    {
        default:
        case PXDictionaryValueLocalityInvalid:
        {
            return 0; // Illegal call
        }
        case PXDictionaryValueLocalityInternalEmbedded:
        {
            valueSourceAdress = value;
            valueSize = dictionary->List.ValueTypeSize;
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

    ++dictionary->List.EntryAmountUsed;

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

PXBool PXAPI PXDictionaryEntryAddMultible(PXDictionary* const dictionary, const void** keyList, const void** valueList, const PXSize amount)
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

void PXAPI PXDictionaryRemove(PXDictionary* const dictionary, const void* key)
{
    // Find

    // Remove
}

PXBool PXAPI PXDictionaryRemoveFound(PXDictionary* const dictionary, const void* key, void* const value)
{
    PXMemorySet((void*)key, 0xFF, dictionary->KeyTypeSize);
    PXMemorySet((void*)value, 0xFF, dictionary->List.ValueTypeSize);

    --dictionary->List.EntryAmountUsed;
}

PXBool PXAPI PXDictionaryExtract(PXDictionary* const dictionary, const void* const key, void* const value)
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

void PXAPI PXDictionaryIndex(const PXDictionary* const dictionary, const PXSize index, PXDictionaryEntry* const pxDictionaryEntry)
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

PXResult PXAPI PXDictionaryEntryFind(PXDictionary* const dictionary, const void* const key, void** const valueResult)
{
    for(PXSize i = 0; i < dictionary->List.EntryAmountUsed; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

        const PXBool isTarget = PXMemoryCompare(pxDictionaryEntry.Key, dictionary->KeyTypeSize, key, dictionary->KeyTypeSize);

        if(isTarget)
        {
            switch(dictionary->ValueLocality)
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
    }

    return PXActionRefusedNotFound;
}