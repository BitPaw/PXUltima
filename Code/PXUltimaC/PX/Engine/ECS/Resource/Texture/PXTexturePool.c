#include "PXTexturePool.h"

#include <PX/OS/PXOS.h>

typedef struct PXTexturePool_
{
    PXECSInfo Info;

    PXSize TextureSetUsed;
    PXSize TextureSetAmount;
    PXECSReferece* TextureSetList;
}
PXTexturePool;

const char PXTexturePoolName[] = "TexturePool";
const PXI8U PXTexturePoolNameLength = sizeof(PXTexturePoolName);
const PXECSRegisterInfoStatic PXTexturePoolRegisterInfoStatic =
{
    {sizeof(PXTexturePoolName), sizeof(PXTexturePoolName), PXTexturePoolName, TextFormatASCII},
    sizeof(PXTexturePool),
    __alignof(PXTexturePool),
    PXECSTypeContainer,
    PXTexturePoolCreate,
    PXTexturePoolRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXTexturePoolRegisterInfoDynamic;


PXResult PXAPI PXTexturePoolRegisterToECS()
{
    PXECSRegister(&PXTexturePoolRegisterInfoStatic, &PXTexturePoolRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXTexturePoolCreate(PXTexturePool** pxTexturePoolREF, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    PXTexturePool* pxTexturePool = PXNull;

    pxECSCreateInfo->Static = &PXTexturePoolRegisterInfoStatic;
    pxECSCreateInfo->Dynamic = &PXTexturePoolRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxTexturePoolREF, pxECSCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTexturePool = *pxTexturePoolREF;

    return PXResultOK;
}

PXResult PXAPI PXTexturePoolRelease(PXTexturePool PXREF pxTexturePool)
{
    return PXActionRefusedNotImplemented;
}

PXSize PXAPI PXTexturePoolAmountGet(PXTexturePool PXREF pxTexturePool)
{
    return  pxTexturePool->TextureSetUsed;
}

PXResult PXAPI PXTexturePoolReserveAmount(PXTexturePool PXREF pxTexturePool, const PXSize amount)
{
    pxTexturePool->TextureSetAmount = amount;
    pxTexturePool->TextureSetList = PXMemoryHeapCallocT(PXECSReferece, amount);

    for(size_t i = 0; i < amount; i++)
    {
        PXTextureSet* pxTextureSet = 0;

        PXECSCreateInfo pxECSCreateInfo;
        PXClear(PXECSCreateInfo, &pxECSCreateInfo);

        PXTextureSetCreate(&pxTextureSet, &pxECSCreateInfo);

        PXTexturePoolSetSet(pxTexturePool, pxTextureSet);
    }

    return PXResultOK;
}

PXTextureSet* PXAPI PXTexturePoolSetGet(PXTexturePool PXREF pxTexturePool, const PXSize index)
{
    PXECSReferece* pxECSReferece = &pxTexturePool->TextureSetList[index];

    //PXECSElementRefCheck();

    return pxECSReferece->Element;
}

PXTextureSet* PXAPI PXTexturePoolTextureGet(const PXTexturePool PXREF pxTexturePool, const PXSize indexTotal)
{
    PXSize totalCounter = 0;

    for(size_t i = 0; i < pxTexturePool->TextureSetUsed; i++)
    {
        PXTextureSet PXREF pxTextureSet = PXTexturePoolSetGet(pxTexturePool, i);

        const PXSize amount = PXTextureSetEntryAmountGet(pxTextureSet);

        totalCounter += amount;

        PXBool isInSet = totalCounter < indexTotal;

        if(!isInSet)
        {
            continue;
        }      

        PXSize indexW = indexTotal - totalCounter;

        PXTexture* pxTextue = PXTextureSetEntryGet(pxTextureSet, indexW);

        return pxTextue;
    }

    return PXNull;
}

PXResult PXAPI PXTexturePoolSetSet(PXTexturePool PXREF pxTexturePool, PXTextureSet PXREF pxTextureSet)
{
    PXECSReferece* pxECSReferece = &pxTexturePool->TextureSetList[pxTexturePool->TextureSetUsed];

    pxECSReferece->Element = pxTextureSet;
    pxECSReferece->ExpectedID = ((PXECSInfo*)pxTextureSet)->ID;

    ++pxTexturePool->TextureSetUsed;

    return PXResultOK;
}