#include "PXRegistry.h"

#if PXRegistryUSE

#include <OS/Memory/PXMemory.h>

#if OSUnix
#elif PXOSWindowsDestop
#pragma comment(lib, "Advapi32.lib")

#ifndef LSTATUS
#define LSTATUS long 
#endif
#endif

PXActionResult PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace)
{
	return PXRegistryConnectRemote(registry, 0, registrySpace);
}

PXActionResult PXRegistryConnectRemote(PXRegistry* const registry, const wchar_t* computerName, const PXRegistrySpace registrySpace)
{
#if OSUnix
#elif PXOSWindowsDestop
	HKEY hKey = 0;

	switch (registrySpace)
	{
		case PXRegistrySpaceLocalMachine:
			hKey = HKEY_LOCAL_MACHINE;
			break;

		case PXRegistrySpacePerformanceData:
			hKey = HKEY_PERFORMANCE_DATA;
			break;

		case PXRegistrySpaceUsers:
			hKey = HKEY_USERS;
			break;

		default:
			break;
	}


	const LSTATUS status = RegConnectRegistryW(computerName, hKey, &registry->ID); // Windows 2000, Advapi32.dll, winreg.h
	const PXBool sucessful = status == ERROR_SUCCESS;

	if (!sucessful)
	{
		// DO stuff;
		return PXActionInvalid;
	}

	return PXActionSuccessful;
#else 
	return PXActionNotSupportedByOperatingSystem;
#endif
}

void PXRegistryClose(PXRegistry* const registry)
{
#if OSUnix
#elif PXOSWindowsDestop
	const LSTATUS status = RegCloseKey(registry->ID);

	registry->ID = 0;
#endif
}

void PXRegistryKeyCreate(PXRegistry* const registry)
{
#if OSUnix
#elif PXOSWindowsDestop
	HKEY hKey = 0;
	LPCWSTR lpSubKey = 0;
	DWORD Reserved = 0;
	LPWSTR lpClass = 0;
	DWORD dwOptions = 0;
	REGSAM samDesired = 0;
	SECURITY_ATTRIBUTES securityAttributes;
	PHKEY phkResult = 0;
	DWORD dwDisposition = 0;

	PXMemoryClear(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));

	const LSTATUS status = RegCreateKeyExW // Windows 2000, Advapi32.dll, winreg.h
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

void PXRegistryKeyLoad(PXRegistry* const registry, const wchar_t* file)
{
#if OSUnix
#elif PXOSWindowsDestop
	HKEY hKey = 0;
	SECURITY_ATTRIBUTES securityAttributes;
	DWORD Flags = 0;

	PXMemoryClear(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));

	const LSTATUS status = RegSaveKeyExW(hKey, file, &securityAttributes, Flags); // Windows 2000, Advapi32.dll, winreg.h
#endif
}

void PXRegistryKeySave(PXRegistry* const registry)
{
#if OSUnix
#elif PXOSWindowsDestop
#endif
}

void PXRegistryKeyDelete(PXRegistry* const registry)
{
#if OSUnix
#elif PXOSWindowsDestop
#endif
}

#endif