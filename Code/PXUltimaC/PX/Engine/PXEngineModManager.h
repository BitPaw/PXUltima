#pragma once

#include <PX/Engine/PXEngineModule.h>
#include "PXDictionaryECS.h"

// System for modules to be loaded and stored in
typedef struct PXEngineModManager_
{
    PXEngineModule ModuleInfo;

    PXDictionaryECS ModLookup;


    PXSize ModsInDirectoryCounted;
}
PXEngineModManager;

typedef struct PXEngineModManagerCreateInfo_
{
    PXECSCreateInfo Info;

    PXText FilePathModFolder;

    PXBool AvoidLoadingMods; // If set true, we scan for mods but not load them.
}
PXEngineModManagerCreateInfo;


PXPublic void PXAPI PXEngineModManagerRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo);
PXPublic PXResult PXAPI PXEngineModManagerCreate(PXEngineModManager** pxEngineModManagerREF, PXEngineModManagerCreateInfo PXREF pxEngineModManagerCreateInfo);
PXPublic PXResult PXAPI PXEngineModManagerRelease(PXEngineModManager PXREF pxEngineModManager);
PXPublic PXResult PXAPI PXEngineModManagerUpdate(PXEngineModManager PXREF pxEngineModManager, PXEngineUpdateInfo PXREF PXEngineUpdateInfo);

PXPublic PXResult PXAPI PXEngineModManagerScan(PXEngineModManager PXREF pxEngineModManager, PXEngineModManagerCreateInfo PXREF pxEngineModManagerCreateInfo);