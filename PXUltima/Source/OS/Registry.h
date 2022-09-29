#ifndef RegistryINCLUDE
#define RegistryINCLUDE

#include <Format/Type.h>

#if OSUnix
#define RegistryID unsigned int

#elif OSWindows
#include <Windows.h>
#include <winreg.h>

#define RegistryID HKEY
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum RegistryResult_
	{
		RegistryResultInvalid,
		RegistryResultSucessful
	}
	RegistryResult;

	typedef enum RegistrySpace_
	{
		RegistrySpaceInvalid,
		RegistrySpaceLocalMachine,
		RegistrySpacePerformanceData,
		RegistrySpaceUsers
	}
	RegistrySpace;

	typedef struct Registry_
	{
		RegistryID ID;
	}
	Registry;

	CPublic RegistryResult RegistryConnectSpace(Registry* const registry, const RegistrySpace registrySpace);
	CPublic RegistryResult RegistryConnectRemote(Registry* const registry, const wchar_t* computerName, const RegistrySpace registrySpace);

	CPublic void RegistryClose(Registry* const registry);

	CPublic void RegistryKeyCreate(Registry* const registry);
	CPublic void RegistryKeyLoad(Registry* const registry, const wchar_t* file);
	CPublic void RegistryKeySave(Registry* const registry);
	CPublic void RegistryKeyDelete(Registry* const registry);

#ifdef __cplusplus
}
#endif

#endif