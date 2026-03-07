#include "PXModelPool.h"

#include <PX/OS/PXOS.h>

typedef struct PXModelPool_
{
    PXECSInfo Info;

    PXSize ModelUsed;
    PXSize ModelAmount;
    PXECSReferece* ModelList;
}
PXModelPool;


const char PXModelPoolName[] = "ModelPool";
const PXI8U PXModelPoolNameLength = sizeof(PXModelPoolName);
const PXECSRegisterInfoStatic PXModelPoolRegisterInfoStatic =
{
    {sizeof(PXModelPoolName), sizeof(PXModelPoolName), PXModelPoolName, TextFormatASCII},
    sizeof(PXModelPool),
    __alignof(PXModelPool),
    PXECSTypeContainer,
    PXModelPoolCreate,
    PXModelPoolRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXModelPoolRegisterInfoDynamic;



PXResult PXAPI PXModelPoolRegisterToECS()
{
    PXECSRegister(&PXModelPoolRegisterInfoStatic, &PXModelPoolRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXModelPoolCreate(PXModelPool** pxModelPoolREF, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    PXModelPool* pxModelPool = PXNull;

    pxECSCreateInfo->Static = &PXModelPoolRegisterInfoStatic;
    pxECSCreateInfo->Dynamic = &PXModelPoolRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxModelPoolREF, pxECSCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxModelPool = *pxModelPoolREF;

    return PXResultOK;
}

PXResult PXAPI PXModelPoolRelease(PXModelPool PXREF pxModelPool)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXModelPoolReserveAmount(PXModelPool PXREF pxModelPool, const PXSize amount)
{
    pxModelPool->ModelAmount = amount;
    pxModelPool->ModelList = PXMemoryHeapReallocT(PXECSReferece, pxModelPool->ModelList, amount);

    return PXResultOK;
}

PXSize PXAPI PXModelPoolModelAmount(const PXModelPool PXREF pxModelPool)
{
    if(!pxModelPool)
    {
        return 0;
    }

    return pxModelPool->ModelUsed;
}

PXModel* PXAPI PXModelPoolModelGet(const PXModelPool PXREF pxModelPool, const PXSize index)
{
    PXECSReferece* pxECSReferece = &pxModelPool->ModelList[index];

    //PXECSElementRefCheck();

    return pxECSReferece->Element;
}

PXModel* PXAPI PXModelPoolModelSet(PXModelPool PXREF pxModelPool, PXModel PXREF pxModel)
{
    if(pxModelPool->ModelUsed +1 > pxModelPool->ModelAmount)
    {
        PXModelPoolReserveAmount(pxModelPool, pxModelPool->ModelUsed + 1);
    }


    PXECSReferece* pxECSReferece = &pxModelPool->ModelList[pxModelPool->ModelUsed];

    pxECSReferece->Element = pxModel;
    pxECSReferece->ExpectedID = ((PXECSInfo*)pxModel)->ID;

    ++pxModelPool->ModelUsed;

    return PXResultOK;
}