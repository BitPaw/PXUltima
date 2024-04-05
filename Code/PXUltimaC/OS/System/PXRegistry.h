#ifndef PXRegistryINCLUDE
#define PXRegistryINCLUDE

#include <OS/Library/PXLibrary.h>

typedef enum PXRegistrySpace_
{
	PXRegistrySpaceInvalid,
	PXRegistrySpaceLocalMachine,
	PXRegistrySpacePerformanceData,
	PXRegistrySpaceUsers
}
PXRegistrySpace;

typedef struct PXRegistry_
{
	PXLibrary Library;

	void* ID;

	void* RegistryConnectRegistryA;
	void* RegistryConnectRegistryW;
	void* RegistryCloseKey;
	void* RegistryKeyDeleteA;
	void* RegistryKeyDeleteW;
	void* RegistryKeySaveA;
	void* RegistryKeySaveW;
	void* RegistryKeyLoadA;
	void* RegistryKeyLoadW;
	void* RegistryCreateKeyExA;
	void* RegistryCreateKeyExW;
	void* RegistryKeyEnumExA;
	void* RegistryKeyEnumExW;
}
PXRegistry;

PXPublic PXActionResult PXAPI PXRegistryInitialize(PXRegistry* const registry);
PXPublic PXActionResult PXAPI PXRegistryRelease(PXRegistry* const registry);

PXPublic PXActionResult PXAPI PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace);
PXPublic PXActionResult PXAPI PXRegistryConnectRemote(PXRegistry* const registry, const PXText* const computerName, const PXRegistrySpace registrySpace);

PXPublic PXActionResult PXAPI PXRegistryClose(PXRegistry* const registry);

PXPublic PXActionResult PXAPI PXRegistryKeyListAll(PXRegistry* const registry);

PXPublic PXActionResult PXAPI PXRegistryKeyCreate(PXRegistry* const registry, const PXText* const pxTextKeyName);
PXPublic PXActionResult PXAPI PXRegistryKeyLoad(PXRegistry* const registry, const PXText* const pxTextKeyName, const PXText* const pxTextFile);
PXPublic PXActionResult PXAPI PXRegistryKeySave(PXRegistry* const registry, const PXText* const pxTextKeyName);
PXPublic PXActionResult PXAPI PXRegistryKeyDelete(PXRegistry* const registry, const PXText* const pxTextKeyName);

#endif