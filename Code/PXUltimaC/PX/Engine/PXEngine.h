#ifndef PXEngineInclude
#define PXEngineInclude

#include <PX/Engine/PXMod.h>
#include <PX/Media/PXResource.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Signal/PXSignal.h>
#include <PX/OS/GUI/PXGUI.h>
#include <PX/OS/Audio/PXAudio.h>
#include <PX/OS/Hardware/PXController.h>
#include <PX/OS/DataBase/PXDataBase.h>

typedef struct PXEngine_ PXEngine;




//-----------------------------------------------------
// StateChange
//-----------------------------------------------------
typedef struct PXEngineResourceStateChangeInfo_
{
    PXInt32U Type;
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
        PXResourceCreateInfo Create;
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


typedef void (PXAPI* PXEngineResourceAdded)(void* const owner, PXEngine* const pxEngine, PXResourceCreateInfo* const pxEngineResourceCreateInfo);

typedef void (PXAPI* PXEngineStartUpEvent)(void* const owner, PXEngine* const pxEngine);
typedef void (PXAPI* PXEngineShutDownEvent)(void* const owner, PXEngine* const pxEngine);

typedef void (PXAPI* PXEngineInteractCallBack)(void* const owner, PXEngine* const pxEngine);

typedef void (PXAPI* PXEngineUserUpdateEvent)(void* const owner, PXEngine* const pxEngine, PXPlayerMoveInfo* const pxPlayerMoveInfo);
typedef void (PXAPI* PXEngineNetworkUpdateEvent)(void* const owner, PXEngine* const pxEngine);
typedef void (PXAPI* PXEngineGameUpdateEvent)(void* const owner, PXEngine* const pxEngine);
typedef void (PXAPI* PXEngineRenderUpdateEvent)(void* const owner, PXEngine* const pxEngine);

typedef struct PXEngineTimeData_
{
    PXInt32U CounterTimeLast;
    PXInt32U CounterTimeDelta;

    PXInt32U CounterTimeWindow;
    PXInt32U CounterTimeUser;
    PXInt32U CounterTimeNetwork;
    PXInt64U CounterTimeRenderLast;
    PXInt32U CounterTimeGPU;
    PXInt32U CounterTimeCPU;
    PXInt32U TimeFrequency;

    PXInt32U FramesPerSecound;
    PXInt32U FrameTime;
}
PXEngineTimeData;

typedef struct PXEngine_
{
    PXResourceManager* ResourceManager;
    PXGUISystem GUISystem;
    PXGraphic Graphic;
    PXWindow* Window; // PXWindow
    PXModLoader ModLoader;
    PXCamera CameraDefault;
    PXAudio Audio;
    PXControllerSystem ControllerSystem;
    PXDataBase DataBase;

    PXKeyBoard KeyBoardCurrentInput;
    PXMouse MouseCurrentInput;

    PXAudioDevice AudioStandardOutDevice;

    void* Owner;
    PXCamera* CameraCurrent;

    PXEngineResourceAdded ResourceAdded;

    PXEngineStartUpEvent OnStartUp;
    PXEngineShutDownEvent OnShutDown;
    PXEngineUserUpdateEvent OnUserUpdate;
    PXEngineNetworkUpdateEvent OnNetworkUpdate;
    PXEngineGameUpdateEvent OnGameUpdate;
    PXEngineRenderUpdateEvent OnRenderUpdate;
    PXEngineInteractCallBack OnInteract;

    PXEngineTimeData TimeData;

    PXBool IsRunning;
    PXBool UpdateUI;
    PXBool DoRendering;


    // Cached most-common objects
    PXModel SpriteScaled;
    PXModel SpriteUnScaled;

    PXMathRandomGeneratorSeed RandomGeneratorSeed;

    PXBool HasGraphicInterface;
    PXBool InteractionLock;

    char ApplicationName[64];
}
PXEngine;

typedef struct PXEngineStartInfo_
{
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
PXEngineStartInfo;

// Signal/Event
PXPrivate void PXCDECL PXEngineOnIllegalInstruction(const int signalID);
PXPrivate void PXCDECL PXEngineOnMemoryViolation(const int signalID);

PXPrivate void PXAPI PXEngineWindowEvent(PXEngine* const pxEngine, PXWindowEvent* const pxWindowEvent);

// Generate a random number with a maximum of the "limiter"
PXPublic PXInt32U PXAPI PXEngineGenerateRandom(PXEngine* const pxEngine, const PXInt32U limiter);

PXPublic PXBool PXAPI PXEngineIsRunning(const PXEngine* const pxEngine);


PXPrivate void PXAPI PXEngineCreatePRE(PXEngine* const pxEngine, PXEngineStartInfo* const pxEngineStartInfo);
PXPrivate void PXAPI PXEngineCreateAudio(PXEngine* const pxEngine, PXEngineStartInfo* const pxEngineStartInfo);
PXPrivate void PXAPI PXEngineCreateGraphic(PXEngine* const pxEngine, PXEngineStartInfo* const pxEngineStartInfo);
PXPrivate void PXAPI PXEngineCreateMod(PXEngine* const pxEngine, PXEngineStartInfo* const pxEngineStartInfo);


PXPublic PXActionResult PXAPI PXEngineStart(PXEngine* const pxEngine, PXEngineStartInfo* const pxEngineStartInfo);
PXPublic void PXAPI PXEngineStop(PXEngine* const pxEngine);
PXPublic void PXAPI PXEngineUpdate(PXEngine* const pxEngine);

PXPublic PXActionResult PXAPI PXEngineResourceAction(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfo);
PXPublic PXActionResult PXAPI PXEngineResourceActionBatch(PXEngine* const pxEngine, PXEngineResourceActionInfo* const pxEngineResourceActionInfoList, const PXSize amount);

PXPublic PXActionResult PXAPI PXEngineResourceCreate(PXEngine* const pxEngine, PXResourceCreateInfo* const pxEngineResourceCreateInfo);
PXPublic PXActionResult PXAPI PXEngineResourceRender(PXEngine* const pxEngine, PXRenderEntity* const pxRenderEntity);

PXPublic PXActionResult PXAPI PXEngineDeviceDataRegister(PXEngine* const pxEngine, PXResourceTransphereInfo* const pxResourceTransphereInfo);
PXPublic PXActionResult PXAPI PXEngineDeviceDataUpload(PXEngine* const pxEngine, PXResourceTransphereInfo* const pxResourceTransphereInfo);

PXPublic void PXAPI PXEngineResourceDefaultElements(PXEngine* const pxEngine);

PXPublic PXActionResult PXAPI PXEngineResourceRenderDefault(PXEngine* const pxEngine);

PXPublic void PXAPI PXEngineCollsisionSolve(PXEngine* const pxEngine);

PXPublic PXActionResult PXAPI PXEngineSpriteTextureSet(PXEngine* const pxEngine, PXSprite* const pxSprite, PXTexture2D* const pxTexture2D);


PXPrivate void PXAPI PXEngineUpdateCollision(PXEngine* const pxEngine);
PXPrivate void PXAPI PXEngineUpdateTimer(PXEngine* const pxEngine);
PXPrivate void PXAPI PXEngineUpdateSpriteAnimator(PXEngine* const pxEngine);

#endif
