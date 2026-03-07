#pragma once

#ifndef PXSceneIncluded
#define PXSceneIncluded

#include <PX/Media/PXType.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/ECS/Entity/Model/PXModel.h>
#include <PX/Engine/ECS/Entity/Camera/PXCamera.h>
#include <PX/Engine/ECS/Resource/Texture/PXTexturePool.h>
#include <PX/Engine/ECS/Entity/Model/PXModelPool.h>

//typedef struct PXScene_ PXScene;

typedef struct PXScene_
{
    PXECSInfo Info;

    PXTexturePool* TexturePool;
    PXModelPool* ModelPool;

    PXCamera* Camera;
}
PXScene;

PXPublic PXResult PXAPI PXSceneRegisterToECS();
PXPublic PXResult PXAPI PXSceneCreate(PXScene** pxSceneREF, PXECSCreateInfo PXREF pxECSCreateInfo);
PXPublic PXResult PXAPI PXSceneRelease(PXScene PXREF pxScene);
PXPublic PXResult PXAPI PXSceneDraw(PXScene PXREF pxScene, PXDrawInfo PXREF pxDrawInfo);

PXPublic PXResult PXAPI PXSceneAddModel(PXScene PXREF pxScene, PXModel PXREF pxModel);
PXPublic PXResult PXAPI PXSceneAddCamera(PXScene PXREF pxScene, PXCamera PXREF pxCamera);

#endif