#include "PXRegistry.h"

#if PXRegistryUSE

#include <OS/Memory/PXMemory.h>

#if OSUnix
#elif PXOSWindowsDestop
//#pragma comment(lib, "Advapi32.lib")
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
			const LSTATUS statusID = registry->RegistryConnectRegistryA(computerName->TextA, hKey, &registry->ID);  // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = registry->RegistryConnectRegistryW(computerName->TextW, hKey, &registry->ID); // Windows 2000, Advapi32.dll, winreg.h
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
	const LSTATUS statusID = registry->RegistryCloseKey(registry->ID);
	const PXBool successful = ERROR_SUCCESS == statusID;

	PXActionOnErrorFetchAndReturn(!successful);

	registry->ID = 0;

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

		const LSTATUS statusID = registry->RegistryKeyEnumExA
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
			const LSTATUS statusID = registry->RegistryCreateKeyExA // Windows 2000, Advapi32.dll, winreg.h
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
			const LSTATUS statusID = registry->RegistryCreateKeyExW // Windows 2000, Advapi32.dll, winreg.h
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

	PXMemoryClear(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));

	switch (pxTextKeyName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const LSTATUS statusID = registry->RegistryKeyLoadA(registry->ID, pxTextKeyName->TextA, pxTextFile->TextA); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = registry->RegistryKeyLoadW(registry->ID, pxTextKeyName->TextW, pxTextFile->TextW); // Windows 2000, Advapi32.dll, winreg.h
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
			const LSTATUS statusID = registry->RegistryKeySaveA(registry->ID, pxTextKeyName->TextA, &securityAttributes); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = registry->RegistryKeySaveW(registry->ID, pxTextKeyName->TextW, &securityAttributes); // Windows 2000, Advapi32.dll, winreg.h
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
			const LSTATUS statusID = registry->RegistryKeyDeleteA(registry->ID, pxTextKeyName->TextA); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = registry->RegistryKeyDeleteW(registry->ID, pxTextKeyName->TextW); // Windows 2000, Advapi32.dll, winreg.h
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

#endif
