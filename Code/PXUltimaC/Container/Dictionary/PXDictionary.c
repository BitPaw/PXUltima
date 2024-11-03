#include "PXDictionary.h"

#include <OS/Memory/PXMemory.h>
#include <OS/Console/PXConsole.h>
#include <Media/PXText.h>

void PXAPI PXDictionaryConstruct(PXDictionary* const dictionary, const PXSize keySize, const PXSize valueSize, const PXDictionaryValueLocality pxDictionaryValueLocality)
{
    PXClear(PXDictionary, dictionary);

    dictionary->ValueLocality = pxDictionaryValueLocality;
    dictionary->KeyTypeSize = keySize;
    dictionary->ValueTypeSize = valueSize;
    dictionary->EntryAmountGrowth = 16;
}

void PXAPI PXDictionaryDestruct(PXDictionary* const dictionary)
{
    PXDeleteList(PXByte, dictionary->DataSize, dictionary->Data, &dictionary->DataSize);
}

PXSize PXAPI PXDictionaryValueSize(const PXDictionary* const dictionary)
{
    switch (dictionary->ValueLocality)
    {
    default:
    case PXDictionaryValueLocalityInvalid:
        return 0;

    case PXDictionaryValueLocalityInternalEmbedded:
        return dictionary->ValueTypeSize;

    case PXDictionaryValueLocalityExternalReference:
        return sizeof(void*);
    }
}

void PXAPI PXDictionaryResize(PXDictionary* const dictionary, const PXSize entrys)
{
    const PXSize rowSize = dictionary->KeyTypeSize + PXDictionaryValueSize(dictionary);
    //const PXSize oldPositionOffset = dictionary->EntryAmountMaximal * rowSize;
    //const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);


    //PXMemoryRealloc(rowSize * );


    PXMemoryHeapReallocateEventData pxMemoryHeapReallocateEventData;

    PXMemoryHeapReallocateEventDataFill
    (
        &pxMemoryHeapReallocateEventData,
        rowSize,
        entrys,
        &dictionary->EntryAmountMaximal,
        &dictionary->DataSize,
        &dictionary->Data
    );
    PXMemoryHeapReallocateEventDataFillNew(&pxMemoryHeapReallocateEventData, 0xFF);

    PXMemoryHeapReallocate(&pxMemoryHeapReallocateEventData);
}

PXBool PXAPI PXDictionaryAdd(PXDictionary* const dictionary, const void* key, const void* value)
{
    const PXBool hasEnoughSpace = (dictionary->EntryAmountCurrent + 1) < dictionary->EntryAmountMaximal;

    if (!hasEnoughSpace)
    {
#if PXLogEnable
        const PXSize sizeBefore = dictionary->EntryAmountMaximal;
#endif

        PXDictionaryResize(dictionary, dictionary->EntryAmountCurrent + dictionary->EntryAmountGrowth);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingEvent,
            "Dictionary",
            "Resize",
            "Size not sufficent. %i -> %i (%3i%%)",
            sizeBefore,
            dictionary->EntryAmountMaximal,
            (PXInt32S)((dictionary->EntryAmountCurrent / (float)dictionary->EntryAmountMaximal) * 100.0f)
        );
#endif
    }

    for (PXSize i = 0; i < dictionary->EntryAmountMaximal; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

        const PXBool isEmptyKeyField = PXMemoryCompareToByte(pxDictionaryEntry.Key, dictionary->KeyTypeSize, 0xff);

        if (isEmptyKeyField)
        {
            // Copy Key
            PXMemoryCopy(key, dictionary->KeyTypeSize, pxDictionaryEntry.Key, dictionary->KeyTypeSize);

            void* valueSourceAdress = 0;
            PXSize valueSourceSize = 0;
            void* valueTargetAdress = 0;
            PXSize valueTargetSize = 0;

            switch (dictionary->ValueLocality)
            {
            default:
            case PXDictionaryValueLocalityInvalid:
            {
                return 0; // Illegal call
            }
            case PXDictionaryValueLocalityInternalEmbedded:
            {
                valueSourceAdress = value;
                valueSourceSize = dictionary->ValueTypeSize;
                valueTargetAdress = pxDictionaryEntry.Value;
                valueTargetSize = dictionary->ValueTypeSize;
                break;
            }
            case PXDictionaryValueLocalityExternalReference:
            {
                valueSourceAdress = &value;
                valueSourceSize = sizeof(void*);
                valueTargetAdress = pxDictionaryEntry.Value;
                valueTargetSize = sizeof(void*);
                break;
            }
            }

            PXMemoryCopy(valueSourceAdress, valueSourceSize, valueTargetAdress, valueTargetSize);

            ++dictionary->EntryAmountCurrent;



#if PXLogEnable && 0
            char keyText[64];

            switch(dictionary->KeyTypeSize)
            {
            case 4:
            {
                PXTextPrintA(keyText, 64, "0x%8.8X", *(PXInt32U*)key);
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
                (PXInt32S)((dictionary->EntryAmountCurrent / (float)dictionary->EntryAmountMaximal) * 100.0f),
                dictionary->EntryAmountCurrent,
                dictionary->EntryAmountMaximal
            );
#endif

            return PXTrue;
        }
    }

    return PXFalse;
}

PXBool PXAPI PXDictionaryAddMultible(PXDictionary* const dictionary, const void** keyList, const void** valueList, const PXSize amount)
{
    const PXBool hasEnoughSpace = (dictionary->EntryAmountCurrent + amount) < dictionary->EntryAmountMaximal;

    if(!hasEnoughSpace)
    {
        PXDictionaryResize(dictionary, dictionary->EntryAmountCurrent + amount + dictionary->EntryAmountGrowth);
    }

    for(PXSize i = 0; i < amount; ++i)
    {
        const void* key = keyList[i];
        const void* value = valueList[i];

        PXDictionaryAdd(dictionary, key, value);
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
    PXMemorySet((void*)value, 0xFF, dictionary->ValueTypeSize);

    --dictionary->EntryAmountCurrent;
}

PXBool PXAPI PXDictionaryExtract(PXDictionary* const dictionary, const void* const key, void* const value)
{
    void* valteAdress = 0;

    const PXBool found = PXDictionaryFindEntry(dictionary, key, &valteAdress);

    if (!found)
    {
        PXMemoryClear(value, dictionary->ValueTypeSize);
        return PXFalse;
    }

    const void* data = (char*)valteAdress + dictionary->KeyTypeSize;

    switch (dictionary->ValueLocality)
    {
    default:
    case PXDictionaryValueLocalityInvalid:
    {
        return 0; // Illegal call
    }
    case PXDictionaryValueLocalityInternalEmbedded:
    {
        PXMemoryCopy(data, dictionary->ValueTypeSize, value, dictionary->ValueTypeSize);
        break;
    }
    case PXDictionaryValueLocalityExternalReference:
    {
        PXMemoryCopy(data, sizeof(void*), value, sizeof(void*));
        break;
    }
    }

    PXDictionaryRemoveFound(dictionary, valteAdress, data);

    return PXTrue;
}

void PXAPI PXDictionaryIndex(const PXDictionary* const dictionary, const PXSize index, PXDictionaryEntry* const pxDictionaryEntry)
{
    const PXSize dataBlockSize = PXDictionaryValueSize(dictionary);
    const PXSize blockSize = dictionary->KeyTypeSize + dataBlockSize;
    const PXByte* blockStart = ((PXByte*)dictionary->Data) + blockSize * index;

    pxDictionaryEntry->Key = (void*)blockStart;
    pxDictionaryEntry->Value = (void*)(blockStart + dictionary->KeyTypeSize);
}

PXBool PXAPI PXDictionaryFindEntry(PXDictionary* const dictionary, const void* const key, void** const valueResult)
{
    for (PXSize i = 0; i < dictionary->EntryAmountCurrent; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(dictionary, i, &pxDictionaryEntry);

        const PXBool isTarget = PXMemoryCompare(pxDictionaryEntry.Key, dictionary->KeyTypeSize, key, dictionary->KeyTypeSize);

        if (isTarget)
        {
            switch(dictionary->ValueLocality)
            {
            default:
            case PXDictionaryValueLocalityInvalid:
            {
                return PXFalse; // Illegal call
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

            return PXTrue;
        }
    }

    return PXFalse;
}
