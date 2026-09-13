#include "PXNamePool.h"

#include <PX/Container/Dictionary/PXDictionary.h>
#include <PX/Type/PXText.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Console/PXConsole.h>

const char PXNamePoolName[] = "NamePool";



typedef struct PXNamePool_
{
    PXECSInfo Info;

    PXSize Amount;

    PXBuffer NameList;

    PXDictionaryT(PXID, PXNamePoolEntry)* IDToPositionLookup;
}
PXNamePool;

PXNamePool _pxNamePool;

PXResult PXAPI PXNamePoolInit()
{
    PXDictionaryCreateInfo pxDictionaryCreateInfo;
    PXClear(PXDictionaryCreateInfo, &pxDictionaryCreateInfo);
    pxDictionaryCreateInfo.KeySize = sizeof(PXID);
    pxDictionaryCreateInfo.ValueSize = sizeof(PXNamePoolEntry);
    pxDictionaryCreateInfo.ValueLocality = PXDictionaryValueLocalityInternalEmbedded;
    pxDictionaryCreateInfo.StartAmount = 4096;

    PXDictionaryCreate(&_pxNamePool.IDToPositionLookup, &pxDictionaryCreateInfo);

    PXBufferAllocate(&_pxNamePool.NameList, 4096);

    return PXResultOK;
}

PXResult PXAPI PXNamePoolStore(const PXID pxID, const PXText* pxText, PXText* pxTextResult)
{
    if(!(pxID && pxText))
    {
        return PXResultRefusedParameterNull;
    }
       

    const PXBool isOK = PXTextHasData(pxText);

    if(!isOK)
    {
        return PXResultRefusedParameterInvalid;
    }

    PXNamePoolEntry pxNamePoolEntry;
    pxNamePoolEntry.Length = pxText->SizeUsed;
    pxNamePoolEntry.Data = 0;

    // redjust
    pxNamePoolEntry.Length = PXTextLengthA(pxText->A, pxText->SizeUsed);

    // Store string
    PXBufferAppend(&_pxNamePool.NameList, pxText->Data, pxText->SizeUsed, &pxNamePoolEntry.Data);
    //void* address = PXListAppend(&_pxNamePool.NameList, pxText->Data, pxText->SizeUsed);

    // Store ID to Adress
    PXResult pxResult = PXDictionaryEntryAdd
    (
        _pxNamePool.IDToPositionLookup,
        &pxID,
        &pxNamePoolEntry
    ); 

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNamePoolName,
        "Store",
        "PXID:<%4i> name is now <%.*s> [%i]",
        pxID,
        pxNamePoolEntry.Length,
        pxNamePoolEntry.Data,
        pxNamePoolEntry.Length
    );
#endif

    ++_pxNamePool.Amount;

    //PXNamePoolPrintAll();

    return pxResult;
}

PXResult PXAPI PXNamePoolGet(const PXID pxID, PXText PXREF pxText)
{
    PXDictionaryEntry pxDictionaryEntry;
    pxDictionaryEntry.KeyAddress = &pxID;
    pxDictionaryEntry.KeySize = sizeof(PXID);
    pxDictionaryEntry.ValueAddress = 0;
    pxDictionaryEntry.ValueSize = sizeof(PXNamePoolEntry);

    PXResult pxResult = PXDictionaryEntryFind(_pxNamePool.IDToPositionLookup, &pxDictionaryEntry);

    if(PXResultOK != pxResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXNamePoolName,
            "Fetch",
            "PXID:<%4i> name was searched but is unkown!",
            pxID
        );
#endif
        PXClear(PXText, pxText);

        return pxResult;
    }

    PXNamePoolEntry pxNamePoolEntry = *(PXNamePoolEntry*)pxDictionaryEntry.ValueAddress;

    PXTextFromAdressA(pxText, pxNamePoolEntry.A, pxNamePoolEntry.Length, pxNamePoolEntry.Length);   


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXNamePoolName,
        "Fetch",
        "PXID:<%4i> name is <%.*s>",
        pxID,
        pxText->SizeUsed,
        pxText->A
    );
#endif

    return pxResult;
}

PXID PXAPI PXNamePoolFindIDViaName(const PXText PXREF pxText)
{
    if(!_pxNamePool.IDToPositionLookup)
    {
        return PXActionRefusedHandleNull;
    }

    const PXSize amount = PXNamePoolEntryAmount();

    PXNamePoolEntryRow pxNamePoolEntryRow;

    for(PXSize i = 0; i < amount; i++)
    {
        PXResult pxResult = PXNamePoolEntryGet(&pxNamePoolEntryRow, i);

        if(PXResultOK != pxResult)
        {
            continue;
        }

        const PXBool isSame = PXTextCompare
        (
            pxText,
            &pxNamePoolEntryRow.Name,
            PXTextCompareRequireSameLength | PXTextCompareIgnoreCase
        );

        if(isSame)
        {
            return pxNamePoolEntryRow.ID;
        }
    }

    return -1;
}

PXSize PXAPI PXNamePoolEntryAmount()
{
    return _pxNamePool.Amount;
}

PXResult PXAPI PXNamePoolEntryGet(PXNamePoolEntryRow PXREF pxNamePoolEntryRow, const PXSize index)
{
    PXDictionaryEntry pxDictionaryEntry;

    PXResult pxResult = PXDictionaryIndex(_pxNamePool.IDToPositionLookup, index, &pxDictionaryEntry);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    PXNamePoolEntry* pxNamePoolEntry = (PXNamePoolEntry*)pxDictionaryEntry.ValueAddress;

    pxNamePoolEntryRow->ID = *(PXID*)pxDictionaryEntry.KeyAddress;

    PXTextFromAdressA(&pxNamePoolEntryRow->Name, pxNamePoolEntry->Data, pxNamePoolEntry->Length, pxNamePoolEntry->Length);

    return pxResult;
}

PXResult PXAPI PXNamePoolPrintAll()
{
    if(!_pxNamePool.IDToPositionLookup)
    {
        return PXActionRefusedHandleNull;
    }

    const PXSize amount = PXNamePoolEntryAmount();

    PXNamePoolEntryRow pxNamePoolEntryRow;

    for(PXSize i = 0; i < amount; i++)
    {
        PXResult pxResult = PXNamePoolEntryGet(&pxNamePoolEntryRow, i);

        if(PXResultOK != pxResult)
        {
            continue;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXNamePoolName,
            "List",
            "Index:<%4i> PXID:<%4i> Name:<%.*s> (%i)",
            i,
            pxNamePoolEntryRow.ID,
            pxNamePoolEntryRow.Name.SizeUsed,
            pxNamePoolEntryRow.Name.A,
            pxNamePoolEntryRow.Name.SizeUsed
        );
#endif
    }

    return PXResultOK;
}