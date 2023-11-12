#include "PXLibrary.h"

#if PXLibraryUSE

#include <OS/Error/PXActionResult.h>
#include <OS/Memory/PXMemory.h>

#if OSUnix
#include <dlfcn.h>
#elif OSWindows
//#include <dbghelp.h> // MISSING
#include <stdio.h>
#include <Psapi.h> // Psapi.lib

#pragma comment( lib, "Dbghelp.lib" ) 
#pragma comment( lib, "Psapi.lib " )

/* PSYMBOL_INFO:MISSING

BOOL CALLBACK EnumSymProc(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
	UNREFERENCED_PARAMETER(UserContext);

	printf("%08X %4u %s\n",	pSymInfo->Address, SymbolSize, pSymInfo->Name);

	return TRUE;
}*/

#endif

// SymGetModuleInfo64 



/*
dlopen() - gain access to an executable object file
dclose() - close a dlopen object
dlsym() - obtain the address of a symbol from a dlopen object
dlvsym() - PXProgramming interface to dynamic linking loader.
dlerror() - get diagnostic information
*/

/*
BF::ErrorCode BF::Library::SearchDirectoryAdd(const wchar_t* directoryPath, LibraryDirectoryID& libraryDirectoryID)
{
#if OSUnix
#elif OSWindows
	const DLL_DIRECTORY_COOKIE cookie = AddDllDirectory(directoryPath);
	const bool successful = cookie != 0;

	if(!successful)
	{
		const ErrorCode errorCode = GetCurrentError();

		return errorCode;
	}

	libraryDirectoryID = cookie;
#endif

	return ErrorCode::Successful;
}

BF::ErrorCode BF::Library::SearchDirectoryRemove(LibraryDirectoryID& libraryDirectoryID)
{
#if OSUnix
#elif OSWindows
	const bool successful = RemoveDllDirectory(libraryDirectoryID);

	if(!successful)
	{
		const ErrorCode errorCode = GetCurrentError();

		return errorCode;
	}

	libraryDirectoryID = 0;
#endif

	return ErrorCode::Successful;
}*/



PXActionResult PXAPI PXLibraryOpen(PXLibrary* const pxLibrary, const PXText* const filePath)
{
	switch (filePath->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
#if OSUnix
			dlerror(); // Clear any existing error

			const int mode = RTLD_NOW;
			pxLibrary->ID = dlopen(filePath->TextA, mode);

			PXActionOnErrorFetchAndReturn(pxLibrary->ID != PXNull);

			return PXActionSuccessful;

#elif PXOSWindowsDestop
			SetLastError(0);

			pxLibrary->ID = LoadLibraryA(filePath->TextA); // Windows XP, Kernel32.dll, libloaderapi.h

			PXActionOnErrorFetchAndReturn(pxLibrary->ID == PXNull);

			return PXActionSuccessful;
#else 
			return PXActionRefusedNotSupported;
#endif	
		}

		case TextFormatUNICODE:
		{
#if OSUnix
			return 0;

#elif PXOSWindowsDestop
			pxLibrary->ID = LoadLibraryW(filePath->TextW); // Windows XP, Kernel32.dll, libloaderapi.h

			PXActionOnErrorFetchAndReturn(pxLibrary->ID == PXNull);

			return PXActionSuccessful;
#else 
			return PXActionRefusedNotSupported;
#endif	
		}
	}

	return PXActionRefusedNotSupported;
}

PXActionResult PXAPI PXLibraryClose(PXLibrary* const pxLibrary)
{
	const PXBool result =
#if OSUnix
		dlclose(pxLibrary->ID);
#elif OSWindows
		FreeLibrary(pxLibrary->ID); // Windows XP (+UWP), Kernel32.dll, libloaderapi.h
#endif

	PXActionOnErrorFetchAndReturn(!result);

	pxLibrary->ID = PXNull;

	return PXActionSuccessful;
}

PXBool PXAPI PXLibraryGetSymbolA(PXLibrary* const pxLibrary, LibraryFunction* const libraryFunction, const char* const symbolName)
{
#if OSUnix
	*libraryFunction = (LibraryFunction*)dlsym(pxLibrary->ID, symbolName);
	const char* errorString = dlerror();
	const PXBool successful = errorString;
#elif OSWindows
	*libraryFunction = (LibraryFunction)GetProcAddress(pxLibrary->ID, symbolName); // Windows XP, Kernel32.dll, libloaderapi.h
	const PXBool successful = *libraryFunction != PXNull;

	PXActionOnErrorFetchAndReturn(!successful);

#endif

	return PXTrue;
}

PXBool PXAPI PXLibraryGetSymbol(PXLibrary* const pxLibrary, LibraryFunction* const libraryFunction, const PXText* symbolName)
{
	return PXLibraryGetSymbolA(pxLibrary, libraryFunction, symbolName->TextA);
}

PXActionResult PXAPI PXLibraryName(PXLibrary* const pxLibrary, PXText* const libraryName)
{
	PXMemoryClear(libraryName->TextA, libraryName->SizeAllocated);

	switch (libraryName->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
#if OSUnix
			return PXActionRefusedNotImplemented;

#elif OSWindows
			libraryName->SizeUsed = GetModuleFileNameExA(pxLibrary->ProcessHandle, pxLibrary->ID, libraryName->TextA, libraryName->SizeAllocated); // Windows XP, Kernel32.dll, psapi.h

			PXActionOnErrorFetchAndReturn(libraryName->SizeUsed == 0);
#endif

			break;
		}

		case TextFormatUNICODE:
		{
#if OSUnix
			return PXActionRefusedNotImplemented;

#elif OSWindows
			libraryName->SizeUsed = GetModuleFileNameExW(pxLibrary->ProcessHandle, pxLibrary->ID, libraryName->TextW, libraryName->SizeAllocated); // Windows XP, Kernel32.dll, psapi.h

			PXActionOnErrorFetchAndReturn(libraryName->SizeUsed == 0);
#endif
			break;
		}
	}

	return PXActionSuccessful;
}

#if OSUnix
#elif PXOSWindowsDestop
BOOL CALLBACK PXLibraryNameSymbolEnumerate(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
{
	PXSymbol pxSymbol;
	pxSymbol.TypeIndex = pSymInfo->TypeIndex;
	pxSymbol.Index = pSymInfo->Index;
	pxSymbol.Size = pSymInfo->Size;
	pxSymbol.ModBase = pSymInfo->ModBase;
	pxSymbol.Flags = pSymInfo->Flags;
	pxSymbol.Address = pSymInfo->Address;
	pxSymbol.Register = pSymInfo->Register;
	pxSymbol.Scope = pSymInfo->Scope;
	pxSymbol.Type = (PXSymbolType)pSymInfo->Tag;
	//pxSymbol.Name;
	PXTextConstructFromAdressA(&pxSymbol.Name, pSymInfo->Name, pSymInfo->NameLen, pSymInfo->NameLen);

	PXFunctionInvoke(((PXSymbolDetectedEvent)UserContext), &pxSymbol);

	return PXTrue;
}
#endif
/*
//BOOL PXLibraryNameSymbolEnumerate(PCSTR SymbolName, DWORD64 SymbolAddress, ULONG SymbolSize, PVOID UserContext)
{
	printf("| %p | %zi | %-30s |\n", SymbolAddress, SymbolSize, SymbolName);

	return PXTrue;
}*/

PXActionResult PXAPI PXLibraryParseSymbols(const PXText* const libraryFilePath, PXSymbolDetectedEvent pxSymbolDetectedEvent)
{
#if OSUnix
	return PXFalse;

#elif PXOSWindowsDestop

	const HANDLE hCurrentProcess = GetCurrentProcess();

	// Initialize
	{		
		const PXBool status = SymInitialize(hCurrentProcess, PXNull, PXFalse); // DbgHelp.dll 5.1 or later 

		if (!status)
		{
			return PXActionFailedInitialization;
		}
	}

	const DWORD64 baseOfDll = SymLoadModuleEx // DbgHelp.dll 6.0 or later
	(
		hCurrentProcess,
		PXNull,
		libraryFilePath->TextA,
		PXNull,
		0,
		0,
		PXNull,
		0
	);

	// On load failure, cleanup and exit
	{
		const PXBool wasLoadingSuccessful = baseOfDll != 0;

		if (!wasLoadingSuccessful)
		{
			const PXBool cleanupSuccess = SymCleanup(hCurrentProcess); // DbgHelp.dll 5.1 or later 

			if (!cleanupSuccess)
			{
				return PXActionFailedCleanup; 
			}

			return PXActionFailedModuleLoad;
		}
	}

	// Enumerate all symvols
	{
		// SymEnumSym, SymEnumerateSymbols64 is outdated?

		const PXBool enumerateResult = SymEnumSymbols // DbgHelp.dll 5.1 or later 
		(
			hCurrentProcess,
			baseOfDll,
			0,
			PXLibraryNameSymbolEnumerate,
			pxSymbolDetectedEvent
		);

		if (!enumerateResult)
		{
			return PXActionFailedDataFetch;
		}
	}

	// Final cleanup
	{
		const PXBool cleanupSuccess = SymCleanup(hCurrentProcess); // DbgHelp.dll 5.1 or later 

		if (!cleanupSuccess)
		{
			return PXActionFailedCleanup;
		}
	}

	return PXActionSuccessful;
#else
	return PXActionNotSupportedByOperatingSystem;
#endif
}
#endif