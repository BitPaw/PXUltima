#include "PXScene.h"

#include <PX/Type/PXModel.h>

const char PXSceneName[] = "Scene";
const PXI8U PXSceneNameLength = sizeof(PXSceneName);
const PXECSRegisterInfoStatic PXSceneRegisterInfoStatic =
{
    {sizeof(PXSceneName), sizeof(PXSceneName), PXSceneName, TextFormatASCII},
    sizeof(PXScene),
    __alignof(PXScene),
    PXECSTypeContainer,
    PXSceneCreate,
    PXSceneRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXSceneRegisterInfoDynamic;

void PXAPI PXSceneRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXSceneRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXSceneRegisterInfoDynamic;
}

PXResult PXAPI PXSceneCreate(PXScene** pxSceneREF, PXSceneCreateInfo PXREF pxSceneCreateInfo)
{
    PXScene* pxScene = PXNull;

    pxSceneCreateInfo->Info.Static = &PXSceneRegisterInfoStatic;
    pxSceneCreateInfo->Info.Dynamic = &PXSceneRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxSceneREF, pxSceneCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxScene = *pxSceneREF;

    PXDictionaryECSCreate(&pxScene->ModelLookup);
    PXDictionaryECSCreate(&pxScene->CameraLookup);

    return PXResultOK;
}

PXResult PXAPI PXSceneRelease(PXScene PXREF pxScene)
{
    if(!pxScene)
    {
        return PXResultRefusedParameterNull;
    }

    return PXResultRefusedNotImplemented;
}

PXSize PXAPI PXSceneModelAmount(const PXScene PXREF pxScene)
{
    if(!pxScene)
    {
        return 0;
    }

    return PXDictionaryECSAmount(&pxScene->ModelLookup);
}

PXModel* PXAPI PXSceneModelGet(const PXScene PXREF pxScene, const PXSize index)
{
    if(!pxScene)
    {
        return PXNull;
    }

    return (PXModel*)PXDictionaryECSGetViaIndex(&pxScene->ModelLookup, index);
}

PXResult PXAPI PXSceneModelAdd(PXScene PXREF pxScene, PXModel PXREF pxModel)
{
    if(!pxScene)
    {
        return PXResultRefusedParameterNull;
    }

    return PXDictionaryECSAdd(&pxScene->ModelLookup, pxModel);
}

PXSkyBox* PXAPI PXSceneSkyBoxGet(const PXScene PXREF pxScene)
{
    if(!pxScene)
    {
        return PXNull;
    }

    return pxScene->SkyBox;
}

PXResult PXAPI PXSceneSkyBoxSet(PXScene PXREF pxScene, PXSkyBox PXREF pxSkyBox)
{
    if(!(pxScene && pxSkyBox))
    {
        return PXResultRefusedParameterNull;
    }

    pxScene->SkyBox = pxSkyBox;

    return PXResultOK;
}

PXSize PXAPI PXSceneCameraAmount(const PXScene PXREF pxScene)
{
    if(!pxScene)
    {
        return 0;
    }

    return PXDictionaryECSAmount(&pxScene->CameraLookup);
}

PXCamera* PXAPI PXSceneCameraGet(const PXScene PXREF pxScene, const PXSize index)
{
    if(!pxScene)
    {
        return PXNull;
    }

    return PXDictionaryECSGetViaIndex(&pxScene->CameraLookup, index);
}

PXResult PXAPI PXSceneCameraAdd(PXScene PXREF pxScene, PXCamera PXREF pxCamera)
{
    if(!pxScene)
    {
        return PXResultRefusedParameterNull;
    }

    return PXDictionaryECSAdd(&pxScene->CameraLookup, pxCamera);
}

PXResult PXAPI PXSceneCameraSet(PXScene PXREF pxScene, PXCamera PXREF pxCamera)
{
    if(!(pxScene && pxCamera))
    {
        return PXResultRefusedParameterNull;
    }

    pxScene->CameraCurrent = pxCamera;

    return PXResultOK;
}

PXCamera* PXAPI PXSceneCameraCurrent(const PXScene PXREF pxScene)
{
    if(!pxScene)
    {
        return PXNull;
    }

    return pxScene->CameraCurrent;
}