#ifndef PXRegistryINCLUDE
#define PXRegistryINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXRegistryUSE OSDeviceToUse == OSDeviceDestop
#if PXRegistryUSE

#include <Media/PXType.h>
#include <Media/PXText.h>
#include <OS/Error/PXActionResult.h>

#if OSUnix
typedef unsigned int PXRegistryID;

#elif OSWindows
#include <Windows.h>
#include <winreg.h>

typedef HKEY PXRegistryID;
#endif

#ifdef __cplusplus
extern "C"
{
#endif

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
		PXRegistryID ID;
	}
	PXRegistry;

	PXPublic PXActionResult PXAPI PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace);
	PXPublic PXActionResult PXAPI PXRegistryConnectRemote(PXRegistry* const registry, const PXText* const computerName, const PXRegistrySpace registrySpace);

	PXPublic PXActionResult PXAPI PXRegistryClose(PXRegistry* const registry);

	PXPublic PXActionResult PXAPI PXRegistryKeyListAll(PXRegistry* const registry);

	PXPublic PXActionResult PXAPI PXRegistryKeyCreate(PXRegistry* const registry, const PXText* const pxTextKeyName);
	PXPublic PXActionResult PXAPI PXRegistryKeyLoad(PXRegistry* const registry, const PXText* const pxTextKeyName, const PXText* const pxTextFile);
	PXPublic PXActionResult PXAPI PXRegistryKeySave(PXRegistry* const registry, const PXText* const pxTextKeyName);
	PXPublic PXActionResult PXAPI PXRegistryKeyDelete(PXRegistry* const registry, const PXText* const pxTextKeyName);

#ifdef __cplusplus
}
#endif

#endif
#endif