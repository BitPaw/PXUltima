#pragma once

#ifndef PXSceneIncluded
#define PXSceneIncluded

#include <PX/Type/PXType.h>
#include <PX/Engine/ECS/PXECS.h>
#include <PX/Type/PXModel.h>
#include <PX/Type/PXCamera.h>
#include <PX/Engine/PXDictionaryECS.h>
#include <PX/Type/PXSkyBox.h>

//typedef struct PXScene_ PXScene;

typedef struct PXScene_
{
    PXECSInfo Info;

    //PXTexturePool* TexturePool; // ???
   
    PXDictionaryECS ModelLookup;
    PXDictionaryECS CameraLookup;

    PXSkyBox* SkyBox;

    PXCamera* CameraCurrent;
}
PXScene;

typedef struct PXSceneCreateInfo_
{
    PXECSCreateInfo Info;
}
PXSceneCreateInfo;

PXPublic void PXAPI PXSceneRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXSceneCreate(PXScene** pxSceneREF, PXSceneCreateInfo PXREF pxSceneCreateInfo);
PXPublic PXResult PXAPI PXSceneRelease(PXScene PXREF pxScene);



// Model
PXPublic PXSize PXAPI PXSceneModelAmount(const PXScene PXREF pxScene);
PXPublic PXModel* PXAPI PXSceneModelGet(const PXScene PXREF pxScene, const PXSize index);
PXPublic PXResult PXAPI PXSceneModelAdd(PXScene PXREF pxScene, PXModel PXREF pxModel);

// Skybox
PXPublic PXSkyBox* PXAPI PXSceneSkyBoxGet(const PXScene PXREF pxScene);
PXPublic PXResult PXAPI PXSceneSkyBoxSet(PXScene PXREF pxScene, PXSkyBox PXREF pxSkyBox);

// Camera
PXPublic PXSize PXAPI PXSceneCameraAmount(const PXScene PXREF pxScene);
PXPublic PXCamera* PXAPI PXSceneCameraGet(const PXScene PXREF pxScene, const PXSize index);
PXPublic PXResult PXAPI PXSceneCameraAdd(PXScene PXREF pxScene, PXCamera PXREF pxCamera);

PXPublic PXResult PXAPI PXSceneCameraSet(PXScene PXREF pxScene, PXCamera PXREF pxCamera);
PXPublic PXCamera* PXAPI PXSceneCameraCurrent(const PXScene PXREF pxScene);



#endif