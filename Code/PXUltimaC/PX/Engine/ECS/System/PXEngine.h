#pragma once

#ifndef PXEngineIncluded
#define PXEngineIncluded

#include <PX/Engine/ECS/PXECS.h>
#include <PX/Engine/PXMod.h>
#include <PX/Engine/PXResource.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Signal/PXSignal.h>
#include <PX/OS/Audio/PXAudio.h>
#include <PX/OS/Hardware/PXController.h>
#include <PX/OS/DataBase/PXDataBase.h>
#include <PX/OS/Graphic/PXGraphic.h>

typedef struct PXEngine_ PXEngine;




//-----------------------------------------------------
// StateChange
//-----------------------------------------------------
typedef struct PXEngineResourceStateChangeInfo_
{
    PXI32U Type;
    void* Object;

    PXBool Enable;
}
PXEngineResourceStateChangeInfo;
//-----------------------------------------------------



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

typedef struct PXEngineResourceActionInfo_
{
    PXEngineResourceActionType Type;

    union
    {
        PXECSCreateInfo Create;
        PXRenderEntity Render;
        PXEngineResourceStateChangeInfo ChangeInfo;
    };
}
PXEngineResourceActionInfo;

typedef struct PXPlayerMoveInfo_
{
    PXVector3F32 MovementWalk;
    PXVector3F32 MovementView;
    PXBool IsWindowInFocus;
    PXBool ActionCommit; // If this stays true, action will be commited
}
PXPlayerMoveInfo;


typedef void (PXAPI* PXEngineResourceAdded)(void PXREF owner, PXEngine PXREF pxEngine, PXECSCreateInfo PXREF pxEngineResourceCreateInfo);

typedef void (PXAPI* PXEngineStartUpEvent)(void PXREF owner, PXEngine PXREF pxEngine);
typedef void (PXAPI* PXEngineShutDownEvent)(void PXREF owner, PXEngine PXREF pxEngine);

typedef void (PXAPI* PXEngineInteractCallBack)(void PXREF owner, PXEngine PXREF pxEngine);

typedef void (PXAPI* PXEngineUserUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine, PXPlayerMoveInfo PXREF pxPlayerMoveInfo);
typedef void (PXAPI* PXEngineNetworkUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine);
typedef void (PXAPI* PXEngineGameUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine);
typedef void (PXAPI* PXEngineRenderUpdateEvent)(void PXREF owner, PXEngine PXREF pxEngine);

typedef struct PXEngineTimeData_
{
    PXI64U TimeNow;

    PXI32U CounterTimeLast;
    PXI32U CounterTimeDelta;

    PXI32U CounterTimeWindow;
    PXI32U CounterTimeUser;
    PXI32U CounterTimeNetwork;
    PXI64U CounterTimeRenderLast;
    PXI32U CounterTimeGPU;
    PXI32U CounterTimeCPU;
    PXI32U TimeFrequency;

    PXI32U FramesPerSecound;
    PXI32U FrameTime;
}
PXEngineTimeData;

typedef struct PXEngineCreateInfo_
{
    PXECSCreateInfo Info;

    PXGraphicInitializeMode Mode;
    PXGraphicSystem System;

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

PXPrivate void PXAPI PXEngineWindowEvent(PXEngine PXREF pxEngine, PXWindowEvent PXREF pxWindowEvent);
PXPrivate PXResult PXAPI PXEngineCreateAudio(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateGraphic(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);
PXPrivate PXResult PXAPI PXEngineCreateMod(PXEngine PXREF pxEngine, PXEngineCreateInfo PXREF pxEngineStartInfo);

PXPublic PXResult PXAPI PXEngineRegisterToECS();
PXPublic PXResult PXAPI PXEngineCreate(PXEngine** pxEngine, PXEngineCreateInfo PXREF pxEngineCreateInfo);
PXPublic PXResult PXAPI PXEngineStart(PXEngine PXREF pxEngine);
PXPublic void PXAPI PXEngineStop(PXEngine PXREF pxEngine);
PXPublic void PXAPI PXEngineUpdate(PXEngine PXREF pxEngine);

PXPrivate void PXAPI PXEngineUpdateTimers(PXEngine PXREF pxEngine);
PXPrivate void PXAPI PXEngineUpdateInput(PXEngine PXREF pxEngine);

PXPrivate void PXAPI PXEngineUpdateMouse(PXEngine PXREF pxEngine, PXWindowEventInputMouseMove PXREF pxWindowEventInputMouseMove);
PXPrivate void PXAPI PXEngineUpdateKeyBoard(PXEngine PXREF pxEngine, PXWindowEventInputKeyboard PXREF pxWindowEventInputKeyboard);

PXPublic PXBool PXAPI PXEngineIsRunning(const PXEngine PXREF pxEngine);

// Generate a random number with a maximum of the "limiter"
PXPublic PXI32U PXAPI PXEngineGenerateRandom(PXEngine PXREF pxEngine, const PXI32U limiter);

PXPublic PXResult PXAPI PXEngineResourceAction(PXEngine PXREF pxEngine, PXEngineResourceActionInfo PXREF pxEngineResourceActionInfo);
PXPublic PXResult PXAPI PXEngineResourceActionBatch(PXEngine PXREF pxEngine, PXEngineResourceActionInfo PXREF pxEngineResourceActionInfoList, const PXSize amount);

PXPublic PXResult PXAPI PXEngineResourceCreate(PXEngine PXREF pxEngine, PXECSCreateInfo PXREF pxEngineResourceCreateInfo);
PXPublic PXResult PXAPI PXEngineResourceRender(PXEngine PXREF pxEngine, PXRenderEntity PXREF pxRenderEntity);

PXPublic PXResult PXAPI PXEngineDeviceDataRegister(PXEngine PXREF pxEngine, PXResourceMoveInfo PXREF PXResourceMoveInfo);
PXPublic PXResult PXAPI PXEngineDeviceDataUpload(PXEngine PXREF pxEngine, PXResourceMoveInfo PXREF PXResourceMoveInfo);

PXPublic void PXAPI PXEngineResourceDefaultElements(PXEngine PXREF pxEngine);

PXPublic PXResult PXAPI PXEngineResourceRenderDefault(PXEngine PXREF pxEngine);

PXPublic void PXAPI PXEngineCollsisionSolve(PXEngine PXREF pxEngine);

PXPublic PXResult PXAPI PXEngineSpriteTextureSet(PXEngine PXREF pxEngine, PXSprite PXREF pxSprite, PXTexture PXREF PXTexture);


PXPrivate void PXAPI PXEngineUpdateCollision(PXEngine PXREF pxEngine);
PXPrivate void PXAPI PXEngineUpdateTimer(PXEngine PXREF pxEngine);
PXPrivate void PXAPI PXEngineUpdateSpriteAnimator(PXEngine PXREF pxEngine);

#endif
