#include "PXLibrary.h"

#if PXLibraryUSE

#include <OS/Error/PXActionResult.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Async/PXEvent.h>

#if OSUnix

#elif OSWindows
//#include <dbghelp.h> // MISSING
#include <stdio.h>
#include <Psapi.h> // Psapi.lib

#pragma comment( lib, "Dbghelp.lib" )

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



PXBool PXLibraryOpen(PXLibrary* const pxLibrary, const PXText* const filePath)
{
	switch (filePath->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
#if OSUnix
			const int mode = RTLD_NOW;
			pxLibrary->ID = dlopen(filePath, mode);

#elif PXOSWindowsDestop
			pxLibrary->ID = LoadLibraryA(filePath); // Windows XP, Kernel32.dll, libloaderapi.h
#endif

			return pxLibrary->ID != PXNull;
		}

		case TextFormatUNICODE:
		{
#if OSUnix
			return 0;

#elif PXOSWindowsDestop
			pxLibrary->ID = LoadLibraryW(filePath); // Windows XP, Kernel32.dll, libloaderapi.h

			return pxLibrary->ID != PXNull;
#endif
		}
	}

	return PXFalse;
}

PXBool PXLibraryClose(PXLibrary* const pxLibrary)
{
	const PXBool result =
#if OSUnix
		dlclose(pxLibrary->ID);
#elif OSWindows
		FreeLibrary(pxLibrary->ID); // Windows XP (+UWP), Kernel32.dll, libloaderapi.h
#endif

	pxLibrary->ID = PXNull;

	return result;
}

PXBool PXLibraryGetSymbol(PXLibrary* const pxLibrary, LibraryFunction* libraryFunction, const PXText* symbolName)
{
#if OSUnix
	const LibraryFunction functionPointer = (LibraryFunction*)dlsym(pxLibrary->ID, symbolName->TextA);
	const char* errorString = dlerror();
	const PXBool successful = errorString;
#elif OSWindows
	const LibraryFunction functionPointer = GetProcAddress(pxLibrary->ID, symbolName->TextA); // Windows XP, Kernel32.dll, libloaderapi.h
	const PXBool successful = functionPointer;
#endif

#if OSUnix
	libraryFunction = (void*)functionPointer;
#elif OSWindows
	libraryFunction = functionPointer;
#endif

	return 1u;
}

PXActionResult PXLibraryName(PXLibrary* const pxLibrary, PXText* const libraryName)
{
	PXMemoryClear(libraryName->TextA, libraryName->SizeAllocated);

	switch (libraryName->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
#if OSUnix
			return PXActionNotImplemented;

#elif OSWindows
			libraryName->SizeUsed = GetModuleFileNameExA(pxLibrary->ProcessHandle, pxLibrary->ID, libraryName->TextA, libraryName->SizeAllocated); // Windows XP, Kernel32.dll, psapi.h

			PXActionReturnOnError(libraryName->SizeUsed == 0);
#endif

			break;
		}

		case TextFormatUNICODE:
		{
#if OSUnix
			return PXActionNotImplemented;

#elif OSWindows
			libraryName->SizeUsed = GetModuleFileNameExW(pxLibrary->ProcessHandle, pxLibrary->ID, libraryName->TextW, libraryName->SizeAllocated); // Windows XP, Kernel32.dll, psapi.h

			PXActionReturnOnError(libraryName->SizeUsed == 0);
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
	pxSymbol.Type = pSymInfo->Tag;
	//pxSymbol.Name;
	PXTextConstructFromAdressA(&pxSymbol.Name, pSymInfo->Name, pSymInfo->NameLen);

	InvokeEvent(((PXSymbolDetectedEvent)UserContext), &pxSymbol);

	return PXTrue;
}
#endif
/*
//BOOL PXLibraryNameSymbolEnumerate(PCSTR SymbolName, DWORD64 SymbolAddress, ULONG SymbolSize, PVOID UserContext)
{
	printf("| %p | %zi | %-30s |\n", SymbolAddress, SymbolSize, SymbolName);

	return PXTrue;
}*/

PXActionResult PXLibraryParseSymbols(const PXText* const libraryFilePath, PXSymbolDetectedEvent pxSymbolDetectedEvent)
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