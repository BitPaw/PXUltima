#include "PXDictionaryECS.h"

#include <PX/OS/PXOS.h>

PXResult PXAPI PXDictionaryECSCreate(PXDictionaryECS PXREF pxDictionaryECS)
{
    PXClear(PXDictionaryECS, pxDictionaryECS);

    PXDictionaryCreateInfo pxDictionaryCreateInfo;
    PXClear(PXDictionaryCreateInfo, &pxDictionaryCreateInfo);
    pxDictionaryCreateInfo.KeySize = sizeof(PXID);
    pxDictionaryCreateInfo.ValueSize = sizeof(PXECSInfo*);
    pxDictionaryCreateInfo.ValueLocality = PXDictionaryValueLocalityInternalEmbedded;

    PXResult pxResult = PXDictionaryCreate(&pxDictionaryECS->Dictionary, &pxDictionaryCreateInfo);

    return pxResult;
}

PXResult PXAPI PXDictionaryECSDestroy(PXDictionaryECS PXREF pxDictionaryECS)
{
    return PXResultOK;
}

PXECSInfo* PXAPI PXDictionaryECSCheck(PXDictionaryECS PXREF pxDictionaryECS, PXECSInfo PXREF pxECSInfo, const PXID pxIDExpected)
{
    const PXID pxIDActual = pxECSInfo->ID;
    const PXBool isOK = pxIDExpected == pxIDActual;

    if(pxIDActual)
    {
        return pxECSInfo;
    }

    // IDs are not equal. Stale reference detected!
    PXDictionaryECSRemoveViaID(pxDictionaryECS, pxIDActual);

    return PXNull;
}

PXSize PXAPI PXDictionaryECSAmount(const PXDictionaryECS PXREF pxDictionaryECS)
{
    return PXDictionaryEntryAmount(pxDictionaryECS->Dictionary);
}

PXECSInfo* PXAPI PXDictionaryECSGetViaIndex(PXDictionaryECS PXREF pxDictionaryECS, const PXSize index)
{
    PXDictionaryEntry pxDictionaryEntry;

    PXResult pxResult = PXDictionaryIndex(pxDictionaryECS->Dictionary, index, &pxDictionaryEntry);

    if(PXResultOK != pxResult)
    {
        return PXNull;
    }

    PXECSInfo* pxECSInfo = *(PXECSInfo**)pxDictionaryEntry.ValueAddress;
    const PXID pxIDExpected = *(PXID*)pxDictionaryEntry.KeyAddress;

    pxECSInfo = PXDictionaryECSCheck(pxDictionaryECS, pxECSInfo, pxIDExpected);

    return pxECSInfo;
}

PXResult PXAPI PXDictionaryECSAdd(PXDictionaryECS PXREF pxDictionaryECS, PXECSInfo PXREF pxECSInfo)
{
    if(!(pxDictionaryECS && pxECSInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXResult pxResult = PXDictionaryEntryAdd(pxDictionaryECS->Dictionary, &pxECSInfo->ID, &pxECSInfo);

    return pxResult;
}

PXResult PXAPI PXDictionaryECSReplace(PXDictionaryECS PXREF pxDictionaryECS, PXECSInfo PXREF pxECSInfo, const PXID pxIDToReplace)
{
    PXDictionaryEntry pxDictionaryEntry;
    pxDictionaryEntry.KeyAddress = &pxIDToReplace;
    pxDictionaryEntry.KeySize = sizeof(PXID);
    pxDictionaryEntry.ValueAddress = 0;
    pxDictionaryEntry.ValueSize = sizeof(PXECSInfo*);

    PXResult pxResult = PXDictionaryEntryFind(pxDictionaryECS->Dictionary, &pxDictionaryEntry);

    switch(pxResult)
    {
        case PXResultOK:
        {
            break; // Nothing
        }
        case PXResultRefusedNotFound:
        {
            return PXDictionaryECSAdd(pxDictionaryECS, pxECSInfo);
        }
        default:
        {
            return PXNull;
        }
    }

    PXID* pxIDOld = (PXID*)pxDictionaryEntry.KeyAddress;
    PXECSInfo** pxECSInfoOld = (PXECSInfo**)pxDictionaryEntry.ValueAddress;

    *pxIDOld = pxECSInfo->ID;
    *pxECSInfoOld = pxECSInfo;

    return PXResultOK;
}

PXResult PXAPI PXDictionaryECSRemoveViaIndex(PXDictionaryECS PXREF pxDictionaryECS, const PXSize index)
{
    return PXResultOK;
}

PXResult PXAPI PXDictionaryECSRemoveViaID(PXDictionaryECS PXREF pxDictionaryECS, const PXID pxID)
{
    return PXResultOK;
}

PXECSInfo* PXAPI PXDictionaryECSGetViaID(PXDictionaryECS PXREF pxDictionaryECS, const PXID pxID)
{
    PXDictionaryEntry pxDictionaryEntry;
    pxDictionaryEntry.KeyAddress = &pxID;
    pxDictionaryEntry.KeySize = sizeof(PXID);
    pxDictionaryEntry.ValueAddress = 0;
    pxDictionaryEntry.ValueSize = sizeof(PXECSInfo*);

    PXResult pxResult = PXDictionaryEntryFind(pxDictionaryECS->Dictionary, &pxDictionaryEntry);

    if(PXResultOK != pxResult)
    {
        return PXNull;
    }

    PXECSInfo* pxECSInfo = *(PXECSInfo**)pxDictionaryEntry.ValueAddress;

    pxECSInfo = PXDictionaryECSCheck(pxDictionaryECS, pxECSInfo, pxID);

    return pxECSInfo;
}