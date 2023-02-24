#include "Registry.h"

#include <OS/Memory/PXMemory.h>

#if OSUnix
#elif OSWindows
#pragma comment(lib, "Advapi32.lib")

#ifndef LSTATUS
#define LSTATUS long 
#endif
#endif

RegistryResult RegistryConnectSpace(Registry* const registry, const RegistrySpace registrySpace)
{
	return RegistryConnectRemote(registry, 0, registrySpace);
}

RegistryResult RegistryConnectRemote(Registry* const registry, const wchar_t* computerName, const RegistrySpace registrySpace)
{
#if OSUnix
#elif OSWindows
	HKEY hKey = 0;

	switch (registrySpace)
	{
	case RegistrySpaceLocalMachine:
		hKey = HKEY_LOCAL_MACHINE;
		break;

	case RegistrySpacePerformanceData:
		hKey = HKEY_PERFORMANCE_DATA;
		break;

	case RegistrySpaceUsers:
		hKey = HKEY_USERS;
		break;

	default:
		break;
	}


	const LSTATUS status = RegConnectRegistryW(computerName, hKey, &registry->ID); // LSTATUS
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
#if OSUnix
#elif OSWindows
	const LSTATUS status = RegCloseKey(registry->ID);

	registry->ID = 0;
#endif
}

void RegistryKeyCreate(Registry* const registry)
{
#if OSUnix
#elif OSWindows
	HKEY hKey = 0;
	LPCWSTR lpSubKey = 0;
	DWORD Reserved = 0;
	LPWSTR lpClass = 0;
	DWORD dwOptions = 0;
	REGSAM samDesired = 0;
	SECURITY_ATTRIBUTES securityAttributes;
	PHKEY phkResult = 0;
	DWORD dwDisposition = 0;

	MemoryClear(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));

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
#if OSUnix
#elif OSWindows
	HKEY hKey = 0;
	SECURITY_ATTRIBUTES securityAttributes;
	DWORD Flags = 0;

	MemoryClear(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));

	const LSTATUS status = RegSaveKeyExW(hKey, file, &securityAttributes, Flags);
#endif
}

void RegistryKeySave(Registry* const registry)
{
#if OSUnix
#elif OSWindows
#endif
}

void RegistryKeyDelete(Registry* const registry)
{
#if OSUnix
#elif OSWindows
#endif
}