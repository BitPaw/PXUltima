#include "Registry.h"

#include <Memory/Memory.h>

#if defined(OSUnix)
#elif defined(OSWindows)
#pragma comment(lib, "Advapi32.lib")
#endif

RegistryResult RegistryConnectSpace(Registry* const registry, const RegistrySpace registrySpace)
{
	return RegistryConnectRemote(registry, 0, registrySpace);
}

RegistryResult RegistryConnectRemote(Registry* const registry, const wchar_t* computerName, const RegistrySpace registrySpace)
{
#if defined(OSUnix)
#elif defined(OSWindows)
	HKEY hKey = 0;

	switch (registrySpace)
	{
	default:
	case RegistrySpaceInvalid:
		break;

	case RegistrySpaceLocalMachine:
		hKey = HKEY_LOCAL_MACHINE;
		break;

	case RegistrySpacePerformanceData:
		hKey = HKEY_PERFORMANCE_DATA;
		break;

	case RegistrySpaceUsers:
		hKey = HKEY_USERS;
		break;
	}


	const LSTATUS status = RegConnectRegistryW(computerName, hKey, &registry->ID);
	const unsigned char sucessful = status == ERROR_SUCCESS;

	if (!sucessful)
	{
		// DO stuff;
		return RegistryResultInvalid;
	}

	return RegistryResultSucessful;
#endif
}

void RegistryClose(Registry* const registry)
{
#if defined(OSUnix)
#elif defined(OSWindows)
	const LSTATUS status = RegCloseKey(registry->ID);

	registry->ID = 0;
#endif
}

void RegistryKeyCreate(Registry* const registry)
{
#if defined(OSUnix)
#elif defined(OSWindows)
	HKEY hKey = 0;
	LPCWSTR lpSubKey = 0;
	DWORD Reserved = 0;
	LPWSTR lpClass = 0;
	DWORD dwOptions = 0;
	REGSAM samDesired = 0;
	SECURITY_ATTRIBUTES securityAttributes;
	PHKEY phkResult = 0;
	DWORD dwDisposition = 0;

	MemorySet(&securityAttributes, sizeof(SECURITY_ATTRIBUTES), 0);

	const LSTATUS status = RegCreateKeyExW
	(
		hKey,
		lpSubKey,
		Reserved,
		lpClass,
		dwOptions,
		samDesired,
		&securityAttributes,
		phkResult,
		&dwDisposition
	);
#endif
}

void RegistryKeyLoad(Registry* const registry, const wchar_t* file)
{
#if defined(OSUnix)
#elif defined(OSWindows)
	HKEY hKey = 0;
	SECURITY_ATTRIBUTES securityAttributes;
	DWORD Flags = 0;

	MemorySet(&securityAttributes, sizeof(SECURITY_ATTRIBUTES), 0);

	const LSTATUS status = RegSaveKeyExW(hKey, file, &securityAttributes, Flags);
#endif
}

void RegistryKeySave(Registry* const registry)
{
#if defined(OSUnix)
#elif defined(OSWindows)
#endif
}

void RegistryKeyDelete(Registry* const registry)
{
#if defined(OSUnix)
#elif defined(OSWindows)
#endif
}