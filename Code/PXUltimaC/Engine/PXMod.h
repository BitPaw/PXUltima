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

typedef PXBool(PXAPI* PXModLoadFunction)(void* const owner, PXMod* const pxMod, PXModLoadEventData* const pxModLoadEventData);
typedef PXBool(PXAPI* PXModInitializeFunction)(void* const owner, PXMod* const pxMod);
typedef PXBool(PXAPI* PXModDenitializeFunction)(void* const owner, PXMod* const pxMod);
typedef PXBool(PXAPI* PXModUnloadFunction)(void* const owner, PXMod* const pxMod);

typedef struct PXMod_
{
	char Name[32];
	char BuildDate[32];

	void* Owner;

	PXModLoadFunction Load;
	PXModInitializeFunction Initialize;
	PXModDenitializeFunction Denitialize;
	PXModUnloadFunction Unload;

	PXLibrary Library;

	// This ID is regulated by the engine.
	// Used for internal identifaction and will be random each run.
	PXInt32U ID;
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