#include "PXTextureSet.h"

#include <PX/OS/PXOS.h>

typedef struct PXTextureSet_
{
    PXECSInfo Info;

    PXSize TextureUsed;
    PXSize TextureAmount;
    PXECSReferece* TextureList;
}
PXTextureSet;


const char PXTextureSetName[] = "TextureSet";
const PXI8U PXTextureSetNameLength = sizeof(PXTextureSetName);
const PXECSRegisterInfoStatic PXTextureSetRegisterInfoStatic =
{
    {sizeof(PXTextureSetName), sizeof(PXTextureSetName), PXTextureSetName, TextFormatASCII},
    sizeof(PXTextureSet),
    __alignof(PXTextureSet),
    PXECSTypeContainer,
    PXTextureSetCreate,
    PXTextureSetRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXTextureSetRegisterInfoDynamic;

PXResult PXAPI PXTextureSetRegisterToECS()
{
    PXECSRegister(&PXTextureSetRegisterInfoStatic, &PXTextureSetRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXTextureSetCreate(PXTextureSet** pxTextureSetREF, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    PXTextureSet* pxTextureSet = PXNull;

    pxECSCreateInfo->Static = &PXTextureSetRegisterInfoStatic;
    pxECSCreateInfo->Dynamic = &PXTextureSetRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxTextureSetREF, pxECSCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTextureSet = *pxTextureSetREF;

    return PXResultOK;
}

PXResult PXAPI PXTextureSetRelease(PXTextureSet PXREF pxTextureSet)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXTextureSetReserveAmount(PXTextureSet PXREF pxTextureSet, const PXSize amount)
{
    pxTextureSet->TextureAmount = amount;
    pxTextureSet->TextureList = PXMemoryHeapReallocT(PXECSReferece, pxTextureSet->TextureList, amount);

    return PXResultOK;
}

PXResult PXAPI PXTextureSetEntryAdd(PXTextureSet PXREF pxTextureSet, PXTexture PXREF pxTexture)
{
    if(pxTextureSet->TextureUsed + 1 > pxTextureSet->TextureAmount)
    {
        PXTextureSetReserveAmount(pxTextureSet, pxTextureSet->TextureUsed + 1);
    }

    PXECSReferece* pxECSReferece = &pxTextureSet->TextureList[pxTextureSet->TextureUsed];

    pxECSReferece->Element = pxTexture;
    pxECSReferece->ExpectedID = ((PXECSInfo*)pxTexture)->ID;

    ++pxTextureSet->TextureUsed;

    return PXResultOK;
}

PXSize PXAPI PXTextureSetEntryAmountGet(PXTextureSet PXREF pxTextureSet)
{
    return pxTextureSet->TextureUsed;
}

PXTexture* PXAPI PXTextureSetEntryGet(PXTextureSet PXREF pxTextureSet, const PXSize index)
{
    PXECSReferece* pxECSReferece = &pxTextureSet->TextureList[index];

    //PXECSElementRefCheck();

    return pxECSReferece->Element;
}