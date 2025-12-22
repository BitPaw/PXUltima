#pragma once

#ifndef PXModIncluded
#define PXModIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Library/PXLibrary.h>

#define PXModLoadFunctionName "PXModLoad"

typedef struct PXMod_ PXMod;

typedef struct PXModLoadEventData_
{
    PXBool IsEssential;

    // Is
    PXBool RequestProgramTermination;

    PXLibrary Library;
}
PXModLoadEventData;

typedef PXActionResult (PXAPI* PXModLoadFunction)(void PXREF owner, PXMod PXREF pxMod, PXModLoadEventData PXREF pxModLoadEventData);
typedef PXActionResult (PXAPI* PXModInitializeFunction)(void PXREF owner, PXMod PXREF pxMod);
typedef PXActionResult (PXAPI* PXModDenitializeFunction)(void PXREF owner, PXMod PXREF pxMod);
typedef PXActionResult (PXAPI* PXModUnloadFunction)(void PXREF owner, PXMod PXREF pxMod);


#define PXModBehaviourIsEssential   0b00001 // Is the mod essential for the functionaly of the program?

typedef struct PXMod_
{
    PXECSInfo Info;

    char Name[32];
    char BuildDate[32];

    void* Owner;

    PXModLoadFunction Load;
    PXModInitializeFunction Initialize;
    PXModDenitializeFunction Denitialize;
    PXModUnloadFunction Unload;

    PXLibrary Library;
}
PXMod;

typedef struct PXModLoader_
{
    PXSize ModListSize;
    PXMod* ModList;
}
PXModLoader;

PXPublic PXResult PXAPI PXModLoaderScan(PXModLoader PXREF pxModLoader, const PXText PXREF pxTextModFileDirectory);
PXPublic PXResult PXAPI PXModLoaderAdd(PXModLoader PXREF pxModLoader, PXMod PXREF pxMod);

#endif
