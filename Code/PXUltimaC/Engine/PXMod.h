#ifndef PXModInclude
#define PXModInclude

#include <Media/PXResource.h>
#include <OS/Error/PXActionResult.h>
#include <OS/Library/PXLibrary.h>

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

typedef PXActionResult (PXAPI* PXModLoadFunction)(void* const owner, PXMod* const pxMod, PXModLoadEventData* const pxModLoadEventData);
typedef PXActionResult (PXAPI* PXModInitializeFunction)(void* const owner, PXMod* const pxMod);
typedef PXActionResult (PXAPI* PXModDenitializeFunction)(void* const owner, PXMod* const pxMod);
typedef PXActionResult (PXAPI* PXModUnloadFunction)(void* const owner, PXMod* const pxMod);


#define PXModBehaviourIsEssential   0b00001 // Is the mod essential for the functionaly of the program?

typedef struct PXMod_
{
    PXResourceInfo Info;

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

PXPublic PXActionResult PXAPI PXModLoaderScan(PXModLoader* const pxModLoader, const PXText* const pxTextModFileDirectory);
PXPublic PXActionResult PXAPI PXModLoaderAdd(PXModLoader* const pxModLoader, PXMod* const pxMod);

#endif
