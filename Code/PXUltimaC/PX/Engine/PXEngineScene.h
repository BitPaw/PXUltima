#pragma once

#ifndef PXEngineSceneIncluded
#define PXEngineSceneIncluded

#include <PX/Engine/PXEngineModule.h>
#include <PX/Type/PXScene.h>

typedef struct PXEngineScene_
{
    PXEngineModule ModuleInfo;

    PXDictionaryT(PXID, PXScene*) SceneLookup;

    PXScene* SceneCurrent;

    // TODO:
    PXEngineRender* RenderSystem;
}
PXEngineScene;

typedef struct PXEngineSceneCreateInfo_
{
    PXECSCreateInfo Info;
}
PXEngineSceneCreateInfo;

PXInternal void PXAPI PXEngineSceneRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXEngineSceneCreate(PXEngineScene** pxEngineSceneREF, PXEngineSceneCreateInfo PXREF pxEngineSceneCreateInfo);
PXPublic PXResult PXAPI PXEngineSceneRelease(PXEngineScene PXREF pxEngineScene);
PXPublic PXResult PXAPI PXEngineSceneUpdate(PXEngineScene PXREF pxEngineScene, PXEngineUpdateInfo PXREF pxEngineUpdateInfo);
PXPublic PXResult PXAPI PXEngineSceneOnActivate(PXEngineScene PXREF pxEngineScene, PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineSceneOnDeactivate(PXEngineScene PXREF pxEngineScene, PXEngine PXREF pxEngine);

PXPublic PXResult PXAPI PXEngineSceneSceneAdd(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);
PXPublic PXResult PXAPI PXEngineSceneSceneRemove(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);
PXPublic PXResult PXAPI PXEngineSceneSceneLoad(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);
PXPublic PXResult PXAPI PXEngineSceneSceneUnload(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);

PXPublic PXResult PXAPI PXEngineScenePrint(const PXEngineScene PXREF pxEngineScene, const PXScene PXREF pxScene);

PXPublic PXScene* PXAPI PXEngineSceneSceneCurrent(const PXEngineScene PXREF pxEngineScene);


// [#] Generate full list of objects from old scene.
// Mark all objects form the old scene to be unloaded.
// We DO NOT unload them here! Only mark.
PXPrivate PXResult PXAPI PXEngineSceneResourceMarkAllToUnload(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);

// [#] Loop over list, elimiate items we need.
// From the list of "indent to unload" objects, remove 
// items our new scene requires. This avoids unessesary reloading.
PXPrivate PXResult PXAPI PXEngineSceneResourceMarkReqiredToLoad(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);


// [#] Make secound list "Items to load", of items in our
// scene minus previous list.
PXPrivate PXResult PXAPI PXEngineSceneResourceFetch(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);
PXPrivate PXResult PXAPI PXEngineSceneResourceEvict(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene);


PXPrivate PXResult PXAPI PXEngineSceneResourceFetchModel(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene, PXModel PXREF pxModel);
PXPrivate PXResult PXAPI PXEngineSceneResourceFetchSkyBox(PXEngineScene PXREF pxEngineScene, PXScene PXREF pxScene, PXSkyBox PXREF pxSkyBox);
PXPrivate PXResult PXAPI PXEngineSceneResourceFetchMaterial(PXEngineScene PXREF pxEngineScene, PXMaterial PXREF pxMaterial);


#endif