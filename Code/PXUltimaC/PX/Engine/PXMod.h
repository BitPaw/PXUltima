#pragma once

#ifndef PXModIncluded
#define PXModIncluded

#include <PX/Engine/PXResource.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/OS/Library/PXLibrary.h>

typedef struct PXMod_ PXMod;
typedef struct PXModCreateInfo_ PXModCreateInfo;
typedef struct PXEngineModManager_ PXEngineModManager;

// Function shall be called once on discoverry, then not again.
// The mods self does not exist jet. But we can give the mod a registartion form.
// It needs access to the mod manager, to set possible mod references and dependecys.
typedef PXResult(PXAPI* PXModCreateFunction)(PXEngineModManager PXREF PXEngineModManager, PXModCreateInfo PXREF pxModCreateInfo);

// Function to load the actual mod.
typedef PXResult (PXAPI* PXModLoadFunction)(PXMod PXREF pxMod);

// Function to unload all.
typedef PXResult (PXAPI* PXModUnloadFunction)(PXMod PXREF pxMod);


#define PXModBehaviourIsEssential   0b00001 // Is the mod essential for the functionaly of the program?

typedef struct PXMod_
{
    PXECSInfo Info;

    char Name[32];
    char BuildDate[32];

    void* Owner;

    PXModLoadFunction Load;
    PXModUnloadFunction Unload;

    PXLibrary Library;
}
PXMod;

typedef struct PXModCreateInfo_
{
    PXECSCreateInfo Info;

    PXBool IsEssential;
    PXBool RequestProgramTermination; // remove? If a mod is strongly expected, shutdown if misssing

    PXLibrary Library; // The .DLL or .SO of the mod
    PXText FilePath;

    PXModCreateFunction Create;
}
PXModCreateInfo;


PXPublic PXResult PXAPI PXModCreate(PXMod** pxModREF, PXModCreateInfo PXREF pxModCreateInfo);
PXPublic PXResult PXAPI PXModRelease(PXMod PXREF pxMod);

#endif
