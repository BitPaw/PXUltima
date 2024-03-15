#include "PXRegistry.h"

#include <OS/Memory/PXMemory.h>

#if OSUnix
#elif PXOSWindowsDestop

#include <Windows.h>
#include <winreg.h>

typedef HKEY PXRegistryID;

//#pragma comment(lib, "Advapi32.lib")



typedef	LSTATUS(APIENTRY* PXRegConnectRegistryA)(_In_opt_ LPCSTR lpMachineName, _In_ HKEY hKey, _Out_ PHKEY phkResult);
typedef	LSTATUS(APIENTRY* PXRegConnectRegistryW)(_In_opt_ LPCWSTR lpMachineName, _In_ HKEY hKey, _Out_ PHKEY phkResult);
typedef	LSTATUS(APIENTRY* PXRegCloseKey)(_In_ HKEY hKey);
typedef	LSTATUS(APIENTRY* PXRegDeleteKeyA)(_In_ HKEY hKey, _In_ LPCSTR lpSubKey);
typedef	LSTATUS(APIENTRY* PXRegDeleteKeyW)(_In_ HKEY hKey, _In_ LPCWSTR lpSubKey);
typedef	LSTATUS(APIENTRY* PXRegSaveKeyA)(_In_ HKEY hKey, _In_ LPCSTR lpFile, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes);
typedef	LSTATUS(APIENTRY* PXRegSaveKeyW)(_In_ HKEY hKey, _In_ LPCWSTR lpFile, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes);
typedef	LSTATUS(APIENTRY* PXRegLoadKeyA)(_In_ HKEY hKey, _In_opt_ LPCSTR lpSubKey, _In_ LPCSTR lpFile);
typedef	LSTATUS(APIENTRY* PXRegLoadKeyW)(_In_ HKEY hKey, _In_opt_ LPCWSTR lpSubKey, _In_ LPCWSTR lpFile);
typedef	LSTATUS(APIENTRY* PXRegCreateKeyExA)(_In_ HKEY hKey, _In_ LPCSTR lpSubKey, _Reserved_ DWORD Reserved, _In_opt_ LPSTR lpClass, _In_ DWORD dwOptions, _In_ REGSAM samDesired, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, _Out_ PHKEY phkResult, _Out_opt_ LPDWORD lpdwDisposition);
typedef	LSTATUS(APIENTRY* PXRegCreateKeyExW)(_In_ HKEY hKey, _In_ LPCWSTR lpSubKey, _Reserved_ DWORD Reserved, _In_opt_ LPWSTR lpClass, _In_ DWORD dwOptions, _In_ REGSAM samDesired, _In_opt_ CONST LPSECURITY_ATTRIBUTES lpSecurityAttributes, _Out_ PHKEY phkResult, _Out_opt_ LPDWORD lpdwDisposition);
typedef	LSTATUS(APIENTRY* PXRegEnumKeyExA)(_In_ HKEY hKey, _In_ DWORD dwIndex, _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPSTR lpName, _Inout_ LPDWORD lpcchName, _Reserved_ LPDWORD lpReserved, _Out_writes_to_opt_(*lpcchClass, *lpcchClass + 1) LPSTR lpClass, _Inout_opt_ LPDWORD lpcchClass, _Out_opt_ PFILETIME lpftLastWriteTime);
typedef	LSTATUS(APIENTRY* PXRegEnumKeyExW)(_In_ HKEY hKey, _In_ DWORD dwIndex, _Out_writes_to_opt_(*lpcchName, *lpcchName + 1) LPWSTR lpName, _Inout_ LPDWORD lpcchName, _Reserved_ LPDWORD lpReserved, _Out_writes_to_opt_(*lpcchClass, *lpcchClass + 1) LPWSTR lpClass, _Inout_opt_ LPDWORD lpcchClass, _Out_opt_ PFILETIME lpftLastWriteTime);

#endif

PXActionResult PXAPI PXRegistryInitialize(PXRegistry* const registry)
{
#if OSUnix
	return PXActionRefusedNotSupported;
#elif OSWindows

	// open Librarfy
	{
		const PXActionResult result = PXLibraryOpenA(&registry->Library, "ADVAPI32.DLL");

		if (!result)
		{
			return PXActionRefusedNotSupported;
		}
	}

	// Load all functions
	{
		const PXLibraryFuntionEntry pxLibraryFuntionEntry[] =
		{
			{&registry->RegistryConnectRegistryA, "RegConnectRegistryA"},
			{&registry->RegistryConnectRegistryW, "RegConnectRegistryW"},
			{&registry->RegistryCloseKey, "RegCloseKey"},
			{&registry->RegistryKeyDeleteA, "RegDeleteKeyA"},
			{&registry->RegistryKeyDeleteW, "RegDeleteKeyW"},
			{&registry->RegistryKeySaveA, "RegSaveKeyA"},
			{&registry->RegistryKeySaveW, "RegSaveKeyW"},
			{&registry->RegistryKeyLoadA, "RegLoadKeyA"},
			{&registry->RegistryKeyLoadW, "RegLoadKeyW"},
			{&registry->RegistryCreateKeyExA, "RegCreateKeyExA"},
			{&registry->RegistryCreateKeyExW, "RegCreateKeyExW"},
			{&registry->RegistryKeyEnumExA, "RegEnumKeyExA" },
			{&registry->RegistryKeyEnumExW, "RegEnumKeyExW"}
		};
		const PXSize amount = sizeof(pxLibraryFuntionEntry) / sizeof(PXLibraryFuntionEntry);

		PXLibraryGetSymbolListA(&registry->Library, pxLibraryFuntionEntry, amount);
	}

	return PXActionSuccessful;

#else
	return PXActionRefusedNotSupported;
#endif
}

PXActionResult PXAPI PXRegistryRelease(PXRegistry* const registry)
{
	PXLibraryClose(&registry->Library);
}

PXActionResult PXAPI PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace)
{
	return PXRegistryConnectRemote(registry, 0, registrySpace);
}

PXActionResult PXAPI PXRegistryConnectRemote(PXRegistry* const registry, const PXText* const computerName, const PXRegistrySpace registrySpace)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
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
			return PXActionRefuedInputInvalid;
	}


	switch (computerName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const PXRegConnectRegistryA pxRegistryConnectRegistryA = registry->RegistryConnectRegistryA;
			const LSTATUS statusID = pxRegistryConnectRegistryA(computerName->TextA, hKey, &registry->ID);  // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const PXRegConnectRegistryW pxRegistryConnectRegistryW = registry->RegistryConnectRegistryW;
			const LSTATUS statusID = pxRegistryConnectRegistryW(computerName->TextW, hKey, &registry->ID); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}

		default:
			return PXActionRefuedTextFormatUnsupported;
	}

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXRegistryClose(PXRegistry* const registry)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop
	const PXRegCloseKey pxRegCloseKey = registry->RegistryCloseKey;
	const LSTATUS statusID = pxRegCloseKey(registry->ID);
	const PXBool successful = ERROR_SUCCESS == statusID;

	PXActionOnErrorFetchAndReturn(!successful);

	registry->ID = PXNull;

	return PXActionSuccessful;

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXRegistryKeyListAll(PXRegistry* const registry)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop
	PXBool repeat = 0;

	PXText pxTextKeyName;
	PXTextConstructBufferA(&pxTextKeyName, 256);

	for (DWORD i = 0; repeat ; ++i) // repeat until "ERROR_NO_MORE_ITEMS"
	{
		DWORD textSize = pxTextKeyName.SizeAllocated;

		const PXRegEnumKeyExA pxRegEnumKeyExA = registry->RegistryKeyEnumExA;
		const LSTATUS statusID = pxRegEnumKeyExA
		(
			registry->ID,
			i,
			pxTextKeyName.TextA,
			&textSize,
			0, // lpReserved
			0, // lpClass
			0, // lpcchClass
			0 // lpftLastWriteTime
		);
		const PXBool successful = ERROR_SUCCESS == statusID;

		if (successful)
		{
			pxTextKeyName.SizeUsed = textSize;
		}

		repeat = ERROR_NO_MORE_ITEMS != statusID;
	}

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXRegistryKeyCreate(PXRegistry* const registry, const PXText* const pxTextKeyName)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop
	DWORD Reserved = 0;
	LPWSTR lpClass = 0;
	DWORD dwOptions = 0;
	REGSAM samDesired = 0;
	SECURITY_ATTRIBUTES securityAttributes;
	PHKEY phkResult = 0;
	DWORD dwDisposition = 0;

	PXClear(SECURITY_ATTRIBUTES, &securityAttributes);

	switch (pxTextKeyName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const PXRegCreateKeyExA pxRegCreateKeyExA = registry->RegistryCreateKeyExA;
			const LSTATUS statusID = pxRegCreateKeyExA // Windows 2000, Advapi32.dll, winreg.h
			(
				registry->ID,
				pxTextKeyName->TextA,
				Reserved,
				 lpClass,
				dwOptions,
				samDesired,
				&securityAttributes,
				phkResult,
				&dwDisposition
			);
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const PXRegCreateKeyExW pxRegCreateKeyExW = registry->RegistryCreateKeyExW;
			const LSTATUS statusID = pxRegCreateKeyExW // Windows 2000, Advapi32.dll, winreg.h
			(
				registry->ID,
				pxTextKeyName->TextW,
				Reserved,
				lpClass,
				dwOptions,
				samDesired,
				&securityAttributes,
				phkResult,
				&dwDisposition
			);
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		default:
			return PXActionRefuedTextFormatUnsupported;
	}


#else
	return PXActionNotSupportedByOperatingSystem;
#endif

	return PXActionSuccessful;
}

PXActionResult PXAPI PXRegistryKeyLoad(PXRegistry* const registry, const PXText* const pxTextKeyName, const PXText* const pxTextFile)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop
	SECURITY_ATTRIBUTES securityAttributes;
	DWORD Flags = 0;

	PXClear(SECURITY_ATTRIBUTES, &securityAttributes);

	switch (pxTextKeyName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const PXRegLoadKeyA pxRegLoadKeyA = registry->RegistryKeyLoadA;
			const LSTATUS statusID = pxRegLoadKeyA(registry->ID, pxTextKeyName->TextA, pxTextFile->TextA); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const PXRegLoadKeyW pxRegLoadKeyW = registry->RegistryKeyLoadW;
			const LSTATUS statusID = pxRegLoadKeyW(registry->ID, pxTextKeyName->TextW, pxTextFile->TextW); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}

		default:
			return PXActionRefuedTextFormatUnsupported;
	}

#else
	return PXActionNotSupportedByOperatingSystem;
#endif

	return PXActionSuccessful;
}

PXActionResult PXAPI PXRegistryKeySave(PXRegistry* const registry, const PXText* const pxTextKeyName)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop

	SECURITY_ATTRIBUTES securityAttributes;

	PXMemoryClear(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));

	switch (pxTextKeyName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const PXRegSaveKeyA pxRegSaveKeyA = registry->RegistryKeySaveA;
			const LSTATUS statusID = pxRegSaveKeyA(registry->ID, pxTextKeyName->TextA, &securityAttributes); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const PXRegSaveKeyW pxRegSaveKeyw = registry->RegistryKeySaveW;
			const LSTATUS statusID = pxRegSaveKeyw(registry->ID, pxTextKeyName->TextW, &securityAttributes); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}

		default:
			return PXActionRefuedTextFormatUnsupported;
	}

	return PXActionSuccessful;

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXRegistryKeyDelete(PXRegistry* const registry, const PXText* const pxTextKeyName)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop

	switch (pxTextKeyName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const PXRegDeleteKeyA pxRegDeleteKeyA = registry->RegistryKeyDeleteA;
			const LSTATUS statusID = pxRegDeleteKeyA(registry->ID, pxTextKeyName->TextA); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const PXRegDeleteKeyW pxRegDeleteKeyW = registry->RegistryKeyDeleteW;
			const LSTATUS statusID = pxRegDeleteKeyW(registry->ID, pxTextKeyName->TextW); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}

		default:
			return PXActionRefuedTextFormatUnsupported;
	}

	return PXActionSuccessful;

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}