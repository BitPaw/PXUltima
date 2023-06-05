#ifndef PXRegistryINCLUDE
#define PXRegistryINCLUDE

#include <OS/System/PXOSVersion.h>

#define PXRegistryUSE OSDeviceToUse == OSDeviceDestop
#if PXRegistryUSE

#include <Media/PXType.h>
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

	PXPublic PXActionResult PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace);
	PXPublic PXActionResult PXRegistryConnectRemote(PXRegistry* const registry, const wchar_t* computerName, const PXRegistrySpace registrySpace);

	PXPublic void PXRegistryClose(PXRegistry* const registry);

	PXPublic void PXRegistryKeyCreate(PXRegistry* const registry);
	PXPublic void PXRegistryKeyLoad(PXRegistry* const registry, const wchar_t* file);
	PXPublic void PXRegistryKeySave(PXRegistry* const registry);
	PXPublic void PXRegistryKeyDelete(PXRegistry* const registry);

#ifdef __cplusplus
}
#endif

#endif
#endif