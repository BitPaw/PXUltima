#pragma once

#ifndef PXSceneIncluded
#define PXSceneIncluded

#include <PX/Type/PXType.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXModel.h>
#include <PX/Type/PXCamera.h>
#include <PX/Type/PXTexturePool.h>
#include <PX/Type/PXModelPool.h>

//typedef struct PXScene_ PXScene;

typedef struct PXScene_
{
    PXECSInfo Info;

    PXTexturePool* TexturePool;
    PXModelPool* ModelPool;

    PXCamera* Camera;
}
PXScene;

PXPublic void PXAPI PXSceneRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXSceneCreate(PXScene** pxSceneREF, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXSceneRelease(PXScene PXREF pxScene);
PXPublic PXResult PXAPI PXSceneDraw(PXScene PXREF pxScene, PXDrawInfo PXREF pxDrawInfo);

PXPublic PXResult PXAPI PXSceneAddModel(PXScene PXREF pxScene, PXModel PXREF pxModel);
PXPublic PXResult PXAPI PXSceneAddCamera(PXScene PXREF pxScene, PXCamera PXREF pxCamera);

#endif