#include "PXRegistry.h"

#if PXRegistryUSE

#include <OS/Memory/PXMemory.h>

#if OSUnix
#elif PXOSWindowsDestop
#pragma comment(lib, "Advapi32.lib")
#endif

PXActionResult PXRegistryConnectSpace(PXRegistry* const registry, const PXRegistrySpace registrySpace)
{
	return PXRegistryConnectRemote(registry, 0, registrySpace);
}

PXActionResult PXRegistryConnectRemote(PXRegistry* const registry, const PXText* const computerName, const PXRegistrySpace registrySpace)
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
			const LSTATUS statusID = RegConnectRegistryA(computerName->TextA, hKey, &registry->ID);  // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = RegConnectRegistryW(computerName->TextW, hKey, &registry->ID); // Windows 2000, Advapi32.dll, winreg.h
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

PXActionResult PXRegistryClose(PXRegistry* const registry)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop
	const LSTATUS statusID = RegCloseKey(registry->ID);
	const PXBool successful = ERROR_SUCCESS == statusID;

	PXActionOnErrorFetchAndReturn(!successful);

	registry->ID = 0;

	return PXActionSuccessful;

#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXRegistryKeyListAll(PXRegistry* const registry)
{
	PXBool repeat = 0;

	PXText pxTextKeyName;
	PXTextConstructBufferA(&pxTextKeyName, 256);

	for (DWORD i = 0; repeat ; ++i) // repeat until "ERROR_NO_MORE_ITEMS"
	{
		DWORD textSize = pxTextKeyName.SizeAllocated;

		const LSTATUS statusID = RegEnumKeyExA
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
}

PXActionResult PXRegistryKeyCreate(PXRegistry* const registry, const PXText* const pxTextKeyName)
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

	PXMemoryClear(&securityAttributes, sizeof(SECURITY_ATTRIBUTES));

	switch (pxTextKeyName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const LSTATUS statusID = RegCreateKeyExA // Windows 2000, Advapi32.dll, winreg.h
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
			const LSTATUS statusID = RegCreateKeyExW // Windows 2000, Advapi32.dll, winreg.h
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
}

PXActionResult PXRegistryKeyLoad(PXRegistry* const registry, const PXText* const pxTextKeyName, const PXText* const pxTextFile)
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
			const LSTATUS statusID = RegLoadKeyA(registry->ID, pxTextKeyName->TextA, pxTextFile->TextA); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = RegLoadKeyW(registry->ID, pxTextKeyName->TextW, pxTextFile->TextW); // Windows 2000, Advapi32.dll, winreg.h
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
}

PXActionResult PXRegistryKeySave(PXRegistry* const registry, const PXText* const pxTextKeyName)
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
			const LSTATUS statusID = RegSaveKeyA(registry->ID, pxTextKeyName->TextA, &securityAttributes); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = RegSaveKeyW(registry->ID, pxTextKeyName->TextW, &securityAttributes); // Windows 2000, Advapi32.dll, winreg.h
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

PXActionResult PXRegistryKeyDelete(PXRegistry* const registry, const PXText* const pxTextKeyName)
{
#if OSUnix
	return PXActionNotSupportedByOperatingSystem;
#elif PXOSWindowsDestop

	switch (pxTextKeyName->Format)
	{
		case TextFormatASCII:
		case TextFormatUTF8:
		{
			const LSTATUS statusID = RegDeleteKeyA(registry->ID, pxTextKeyName->TextA); // Windows 2000, Advapi32.dll, winreg.h
			const PXBool successful = ERROR_SUCCESS == statusID;

			PXActionOnErrorFetchAndReturn(!successful);

			break;
		}
		case TextFormatUNICODE:
		{
			const LSTATUS statusID = RegDeleteKeyW(registry->ID, pxTextKeyName->TextW); // Windows 2000, Advapi32.dll, winreg.h
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