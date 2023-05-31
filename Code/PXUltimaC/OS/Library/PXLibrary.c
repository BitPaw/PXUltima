#include "PXLibrary.h"

#if PXLibraryUSE

#include <OS/Error/PXActionResult.h>
#include <OS/Memory/PXMemory.h>

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

#elif OSWindows
			pxLibrary->ID = LoadLibraryA(filePath); // Windows XP, Kernel32.dll, libloaderapi.h
#endif

			return pxLibrary->ID != PXNull;
		}

		case TextFormatUNICODE:
		{
#if OSUnix
			return 0;

#elif OSWindows
			pxLibrary->ID = LoadLibraryW(filePath); // Windows XP, Kernel32.dll, libloaderapi.h

			return pxLibrary->ID != PXNull;
#endif
		}
	}
}

PXBool PXLibraryClose(PXLibrary* const pxLibrary)
{
	const PXBool result =
#if OSUnix
		dlclose(pxLibrary->ID);
#elif OSWindows
		FreeLibrary(pxLibrary->ID); // Windows XP, Kernel32.dll, libloaderapi.h
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

PXBool PXLibraryParseSymbols()
{

#if OSUnix
	return PXFalse;

#elif OSWindows
	/*

	auto x = LoadLibraryA("user32.dll");

	//HANDLE hProcess = GetCurrentProcess(); //  GetProcAddress
	HANDLE hProcess = GetProcAddress(x, 0); //  GetProcAddress

	DWORD64 BaseOfDll;
	char* Mask = "*";
	BOOL status;

	status = SymInitialize(hProcess, NULL, FALSE);
	if (status == FALSE)
	{
		return;
	}

	BaseOfDll = SymLoadModuleEx
	(
		hProcess,
		NULL,
		"user32.dll",
		NULL,
		0,
		0,
		NULL,
		0
	);

	if (BaseOfDll == 0)
	{
		SymCleanup(hProcess);
		return;
	}

	if (SymEnumSymbols(hProcess,     // Process handle from SymInitialize.
		BaseOfDll,   // Base address of module.
		Mask,        // Name of symbols to match.
		EnumSymProc, // Symbol handler procedure.
		NULL))       // User context.
	{
		// SymEnumSymbols succeeded
	}
	else
	{
		// SymEnumSymbols failed
		printf("SymEnumSymbols failed: %d\n", GetLastError());
	}

	SymCleanup(hProcess);*/
#endif

	return 0;
}
#endif