#include "PXLibrary.h"

#include <OS/Error/PXActionResult.h>

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

#include <OS/Memory/PXMemory.h>

/*
dlopen() - gain access to an executable object file
dclose() - close a dlopen object
dlsym() - obtain the address of a symbol from a dlopen object
dlvsym() - Programming interface to dynamic linking loader.
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

PXBool LibraryOpenA(PXLibrary* const pxLibrary, const char* filePath)
{
#if OSUnix
	const int mode = RTLD_NOW;
	pxLibrary->ID = dlopen(filePath, mode);

#elif OSWindows
	pxLibrary->ID = LoadLibraryA(filePath);
#endif

	return pxLibrary->ID != PXNull;
}

PXBool LibraryOpenW(PXLibrary* const pxLibrary, const wchar_t* filePath)
{
#if OSUnix
	return Open((char*)filePath);
#elif OSWindows
	pxLibrary->ID = LoadLibraryW(filePath);

	return pxLibrary->ID != PXNull;
#endif
}

PXBool LibraryClose(PXLibrary* const pxLibrary)
{
	const PXBool result =
#if OSUnix
		dlclose(pxLibrary->ID);
#elif OSWindows
		FreeLibrary(pxLibrary->ID);
#endif

	pxLibrary->ID = PXNull;

	return result;
}

PXBool LibraryGetSymbol(PXLibrary* const pxLibrary, LibraryFunction* libraryFunction, const char* symbolName)
{
#if OSUnix
	const LibraryFunction functionPointer = (LibraryFunction*)dlsym(pxLibrary->ID, symbolName);
	const char* errorString = dlerror();
	const PXBool successful = errorString;
#elif OSWindows
	const LibraryFunction functionPointer = GetProcAddress(pxLibrary->ID, symbolName);
	const PXBool successful = functionPointer;
#endif

#if OSUnix
	libraryFunction = (void*)functionPointer;
#elif OSWindows
	libraryFunction = functionPointer;
#endif

	return 1u;
}

PXSize PXLibraryNameA(PXLibrary* const pxLibrary, const PXTextASCII libraryName, const PXSize libraryNameMaxSize)
{
	MemoryClear(libraryName, libraryNameMaxSize);

#if OSUnix
	return 0;

#elif OSWindows
	const DWORD result = GetModuleFileNameExA(pxLibrary->ProcessHandle, pxLibrary->ID, libraryName, libraryNameMaxSize);
	
	if (!result)
	{
		PXActionResult px = GetCurrentError();

		printf("");
	}

	return result;

#endif
}

unsigned char LibraryParseSymbols()
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
