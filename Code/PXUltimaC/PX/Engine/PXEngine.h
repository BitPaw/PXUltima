#pragma once

#ifndef PXEngineIncluded
#define PXEngineIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/PXMod.h>
#include <PX/Engine/PXResource.h>
#include <PX/Engine/PXEngineScene.h>
#include <PX/Type/PXSprite.h>
#include <PX/Engine/PXEngineModule.h>
#include <PX/OS/Graphic/PXGraphic.h>
#include "PXEngineModManager.h"
#include "PXEngineMath.h"
#include "PXEngineInput.h"
#include "PXEngineTimer.h"

typedef struct PXEngine_ PXEngine;
typedef struct PXEngineCreateInfo_ PXEngineCreateInfo;

// Action to be committed to a given resource
typedef enum PXEngineResourceActionType_
{
    PXEngineResourceActionTypeInvalid,
    PXEngineResourceActionTypeCustom,
    PXEngineResourceActionTypeCreate,
    PXEngineResourceActionTypeDestroy,
    PXEngineResourceActionTypeUpdate,
    PXEngineResourceActionTypeFetch,
    PXEngineResourceActionTypeSelect,
    PXEngineResourceActionTypeRender,
    PXEngineResourceActionTypePlay,
    PXEngineResourceActionTypeStateChange // Enable or Disable object
}
PXEngineResourceActionType;

typedef struct PXPlayerMoveInfo_
{
    PXVector3F32 MovementWalk;
    PXVector3F32 MovementView;
    PXBool IsWindowInFocus;
    PXBool ActionCommit; // If this stays true, action will be commited
}
PXPlayerMoveInfo;


typedef struct PXEngineContext_
{
    PXDictionaryECS SystemLookup;
    PXEngineAudio* Audio;
    PXEngineRender* Render;
    PXEngineScene* Scene;
    PXEngineInput* Input;
    PXEngineUI* UI;
    PXEngineTime* Time;
    PXEngineMath* Math;
    PXEngineModManager* ModManager;
}
PXEngineContext;



typedef void (PXAPI* PXEngineResourceAdded)(void PXREF owner, PXEngine PXREF pxEngine, PXECSCreateInfo PXREF pxEngineResourceCreateInfo);

typedef void (PXAPI* PXEngineStartUpEvent)(void PXREF owner, PXEngine PXREF pxEngine);
typedef void (PXAPI* PXEngineShutDownEvent)(void PXREF owner, PXEngine PXREF pxEngine);

typedef void (PXAPI* PXEngineInteractCallBack)(void PXREF owner, PXEngine PXREF pxEngine);

typedef void (PXAPI* PXEngineUserUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine, PXPlayerMoveInfo PXREF pxPlayerMoveInfo);
typedef void (PXAPI* PXEngineNetworkUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine);
typedef void (PXAPI* PXEngineGameUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine);
typedef void (PXAPI* PXEngineRenderUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine);



//---------------------------------------------------------
// Create engine and modlules
//---------------------------------------------------------
typedef struct PXEngineCreateInfo_
{
    PXECSCreateInfo Info;

    PXSize SystemInitializeAmount;
    PXEngineModuleInitializeFunction* SystemInitializeList;

    PXGraphicInitializeMode Mode;

    PXEngineStartUpEvent OnStartUp;
    PXEngineShutDownEvent OnShutDown;
    PXEngineUserUpdateEvent OnUserUpdate;
    PXEngineNetworkUpdateEvent OnNetworkUpdate;
    PXEngineGameUpdateEvent OnGameUpdate;
    PXEngineRenderUpdateEvent OnRenderUpdate;
    PXEngineInteractCallBack OnInteract;

    char* Name;
    void* Owner;

    PXSize Width;
    PXSize Height;

    PXBool UseMods;
    PXBool UseMouseInput;

    PXWindow* WindowRenderTarget;
    PXWindow* WindowRenderParent;
}
PXEngineCreateInfo;

typedef PXResult(PXAPI* PXEngineModuleCreateFunction)(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
//---------------------------------------------------------



//---------------------------------------------------------
// Engine and module updates
//---------------------------------------------------------
typedef struct PXEngineUpdateInfo_
{
    // References
    PXEngine* Engine;

    // Time
    //PXBool IsASYNC;
}
PXEngineUpdateInfo;
//---------------------------------------------------------

PXPrivate void PXAPI PXEngineWindowEvent(PXEngineUIEvent PXREF pxWindowEvent);

PXPrivate PXResult PXAPI PXEngineCreateRender(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateAudio(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateInput(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateScene(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateUI(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateTime(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateMath(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateModManager(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
//---------------------------------------------------------
// CREATE/DESTROY
PXPublic void PXAPI PXEngineRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXEngineCreate(PXEngine** pxEngine, PXEngineCreateInfo PXREF pxEngineCreateInfo);
PXPublic PXResult PXAPI PXEngineRelease(PXEngine PXREF pxEngine);


PXPrivate PXThreadResult PXOSAPI PXEngineRootLoop(PXEngine PXREF pxEngine);
PXPublic PXThread* PXAPI PXEngineRootThread(const PXEngine PXREF pxEngine);

PXPrivate void PXAPI PXEngineModuleSerialActivate(PXEngine PXREF pxEngine);
PXPrivate void PXAPI PXEngineModuleSerialDeactivate(PXEngine PXREF pxEngine);
PXPrivate void PXAPI PXEngineModuleASYNCActivate(PXEngine PXREF pxEngine);
PXPrivate void PXAPI PXEngineModuleASYNCDeactivate(PXEngine PXREF pxEngine);
//---------------------------------------------------------


//---------------------------------------------------------
// STATE
PXPublic PXResult PXAPI PXEngineStart(PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineStop(PXEngine PXREF pxEngine);


PXPublic PXResult PXAPI PXEngineEventOnTick(PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineEventOnRender(PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineEventOnUI(PXEngine PXREF pxEngine);

// Single update
PXPublic PXResult PXAPI PXEngineUpdate(PXEngine PXREF pxEngine);

// Semi-permanent function to run as long as it does. 
// Until an event singals a stop.
PXPublic PXResult PXAPI PXEngineRun(PXEngine PXREF pxEngine);
//---------------------------------------------------------


//---------------------------------------------------------
// Register module for sub-modules
PXPublic PXSize PXAPI PXEngineModuleAmount(const PXEngine PXREF pxEngine);
PXPublic PXEngineModule* PXAPI PXEngineModuleIndex(PXEngine PXREF pxEngine, const PXSize index);
PXPublic PXResult PXAPI PXEngineModuleRegister
(
    PXEngine PXREF pxEngine,
    PXEngineModule PXREF pxEngineModule

);
PXPublic PXResult PXAPI PXEngineModuleRelease(PXEngine PXREF pxEngine);
//---------------------------------------------------------


//---------------------------------------------------------
// GET/SET
PXPublic PXEngineContext* PXAPI PXEngineContextGet(const PXEngine PXREF pxEngine);

PXPublic PXResult PXAPI PXEngineEventUIAviableSet(PXEngine PXREF pxEngine);
PXPublic PXResult PXAPI PXEngineEventThreadDoneSet(PXEngine PXREF pxEngine);
//---------------------------------------------------------



PXPublic void PXAPI PXEngineDrawSceneDefault(PXEngine PXREF pxEngine, PXEngineUIDrawInfo PXREF pxDrawInfo);

PXPrivate void PXAPI PXEngineUpdateMouse(PXEngine PXREF pxEngine, PXWindowEventMouseMoveData PXREF pxWindowEventInputMouseMove);
PXPrivate void PXAPI PXEngineUpdateKeyBoard(PXEngine PXREF pxEngine, PXWindowEventKeyboardData PXREF pxWindowEventInputKeyboard);

// Generate a random number with a maximum of the "limiter"
PXPublic PXI32U PXAPI PXEngineGenerateRandom(PXEngine PXREF pxEngine, const PXI32U limiter);

PXPublic PXResult PXAPI PXEngineResourceCreate(PXEngine PXREF pxEngine, PXECSCreateInfo PXREF pxEngineResourceCreateInfo);

PXPublic void PXAPI PXEngineResourceDefaultElements(PXEngine PXREF pxEngine);

PXPublic PXResult PXAPI PXEngineResourceRenderDefault(PXEngine PXREF pxEngine);

PXPrivate void PXAPI PXEngineUpdateSpriteAnimator(PXEngine PXREF pxEngine);

#endif
