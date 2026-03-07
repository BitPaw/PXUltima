#include "PXScene.h"

#include <PX/Engine/ECS/Entity/Model/PXModel.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexturePool.h>
#include <PX/Engine/ECS/Entity/Model/PXModelPool.h>

/*
typedef struct PXScene_
{
    PXECSInfo Info;

    PXTexturePool* TexturePool;
    PXModelPool* ModelPool;

    PXCamera* Camera;
}
PXScene;*/

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
    PXSceneDraw
};
PXECSRegisterInfoDynamic PXSceneRegisterInfoDynamic;

PXResult PXAPI PXSceneRegisterToECS()
{
    PXECSRegister(&PXSceneRegisterInfoStatic, &PXSceneRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXSceneCreate(PXScene** pxSceneREF, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    PXScene* pxScene = PXNull;

    pxECSCreateInfo->Static = &PXSceneRegisterInfoStatic;
    pxECSCreateInfo->Dynamic = &PXSceneRegisterInfoDynamic;
    const PXResult pxResult = PXECSCreate(pxSceneREF, pxECSCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxScene = *pxSceneREF;

    PXModelPoolCreate(&pxScene->ModelPool, pxECSCreateInfo);
    PXTexturePoolCreate(&pxScene->TexturePool, pxECSCreateInfo);

    return PXResultOK;
}

PXResult PXAPI PXSceneRelease(PXScene PXREF pxScene)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXSceneDraw(PXScene PXREF pxScene, PXDrawInfo PXREF pxDrawInfo)
{
    if(!(pxScene && pxDrawInfo))
    {
        return PXResultRefusedParameterNull;
    }
    PXModelPool PXREF pxModelPool = pxScene->ModelPool;

    PXSize amount = PXModelPoolModelAmount(pxModelPool);

    for(PXSize i = 0; i < amount; i++)
    {
        PXModel* pxModel = PXModelPoolModelGet(pxModelPool, i);

        PXModelDraw(pxModel, pxDrawInfo);
    }

    return PXResultOK;
}

PXResult PXAPI PXSceneAddModel(PXScene PXREF pxScene, PXModel PXREF pxModel)
{
    PXModelPoolModelSet(pxScene->ModelPool, pxModel);

    return PXResultOK;
}

PXResult PXAPI PXSceneAddCamera(PXScene PXREF pxScene, PXCamera PXREF pxCamera)
{
    pxScene->Camera = pxCamera;

    return PXResultOK;
}