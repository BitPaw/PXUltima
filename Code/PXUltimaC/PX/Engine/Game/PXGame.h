#pragma once

#ifndef PXGameIncluded
#define PXGameIncluded

#include <PX/Type/PXType.h>
#include <PX/Engine/Player/PXPlayerAction.h>
#include <PX/Engine/PXGameEvent.h>


typedef struct PXGame_ PXGame;

// Rules
typedef void (PXAPI* PXGameInitializeFunction)(PXGame PXREF pxGame);
typedef void (PXAPI* PXGameDestroyFunction)(PXGame PXREF pxGame);
typedef int  (PXAPI* PXGameActionValidateFunction)(PXGame PXREF pxGame, PXPlayerAction PXREF pxPlayerAction);
typedef void (PXAPI* PXGameActionApplyFunction)(PXGame PXREF pxGame, PXPlayerAction PXREF pxPlayerAction);
typedef int  (PXAPI* PXGameIsRoundOverFunction)(PXGame PXREF pxGame);
typedef PXBool(PXAPI* PXGameIsGameOverFunction)(PXGame PXREF pxGame);

typedef PXResult(PXAPI* PXGameSceneDrawFunction)(PXGame PXREF pxGame, PXDrawInfo PXREF pxDrawInfo);



typedef void (PXAPI* PXGameModuleLoadFunction)(PXGame PXREF pxGame, void PXREF owner);
typedef void (PXAPI* PXGameModuleUnloadFunction)(PXGame PXREF pxGame, void PXREF owner);
typedef void (PXAPI* PXGameModuleTickFunction)(PXGame PXREF pxGame, void PXREF owner);


// Module 
typedef struct PXGameModule_
{
    PXGameModuleLoadFunction Load;
    PXGameModuleUnloadFunction Unload;
    PXGameModuleTickFunction Tick;
}
PXGameModule;



typedef struct PXGameCallBinding_
{
    PXGameInitializeFunction Initialize;
    PXGameDestroyFunction Release;
    PXGameActionValidateFunction ActionValidate;
    PXGameActionApplyFunction ActionApply;
    PXGameIsRoundOverFunction IsRoundOver;
    PXGameIsGameOverFunction IsGameOver;
    PXGameSceneDrawFunction SceneDraw;
}
PXGameCallBinding;

typedef struct PXGameCreateInfo_
{
    PXECSCreateInfo Info;

    PXGameCallBinding Binding;
}
PXGameCreateInfo;

PXPublic PXResult PXAPI PXGameCreate(PXGame** pxGame, PXGameCreateInfo PXREF pxGameCreateInfo);
PXPublic PXResult PXAPI PXGameRelease(PXGame PXREF pxGame);


PXPublic PXResult PXAPI PXGameTick(PXGame PXREF pxGame);


PXPublic PXResult PXAPI PXGameModuleRegister(PXGame PXREF pxGame, PXGameModule PXREF pxGameModule);

// Events
PXPublic PXResult PXAPI PXGameEventEnqueue(PXGame PXREF pxGame, PXGameEvent PXREF pxGameEvent);
PXPublic PXGameEvent* PXAPI PXGameEventDequeue(PXGame PXREF pxGame);

// Player
PXPublic PXPlayer* PXAPI PXGamePlayerCurrentGet(PXGame PXREF pxGame);
PXPublic PXPlayer* PXAPI PXGamePlayerNextGet(PXGame PXREF pxGame);
PXPublic PXSize PXAPI PXGamePlayerAmount(PXGame PXREF pxGame);
PXPublic PXPlayer* PXAPI PXGamePlayerGet(PXGame PXREF pxGame, const PXSize playerIndex);

// Game Events
PXPublic PXBool PXAPI PXGameConditionGameStart(PXGame PXREF pxGame);

#endif