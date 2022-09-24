#include "Library.h"


#if defined(OSUnix)

#elif defined(OSWindows)
//#include <dbghelp.h> // MISSING
#include <stdio.h>

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
dlvsym() - Programming interface to dynamic linking loader.
dlerror() - get diagnostic information
*/

/*
BF::ErrorCode BF::Library::SearchDirectoryAdd(const wchar_t* directoryPath, LibraryDirectoryID& libraryDirectoryID)
{
#if defined(OSUnix)
#elif defined(OSWindows)
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
#if defined(OSUnix)
#elif defined(OSWindows)
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

unsigned char LibraryOpenA(LibraryHandle* Handle, const char* filePath)
{
#if defined(OSUnix)
	const int mode = RTLD_NOW;
	LibraryHandle libraryHandle = dlopen(filePath, mode);

#elif defined(OSWindows)
	const LibraryHandle libraryHandle = LoadLibraryA(filePath);

#endif

	{
		const unsigned char sucessful = libraryHandle != 0;

		if(!sucessful)
		{
			return 0;
		}
	}

	Handle = libraryHandle;

	return 1u;
}

unsigned char LibraryOpenW(LibraryHandle* Handle, const wchar_t* filePath)
{
#if defined(OSUnix)
	return Open((char*)filePath);
#elif defined(OSWindows)
	const LibraryHandle libraryHandle = LoadLibraryW(filePath);

	{
		const unsigned char sucessful = libraryHandle != 0;

		if(!sucessful)
		{
			return 0;
		}
	}

	Handle = libraryHandle;

	return 1u;
#endif
}

unsigned char LibraryClose(LibraryHandle* Handle)
{
#if defined(OSUnix)
	const unsigned char result = dlclose(Handle);
#elif defined(OSWindows)
	const unsigned char result = FreeLibrary(Handle);
#endif

	Handle = 0;

	return result;
}

unsigned char LibraryGetSymbol(LibraryHandle* handle, LibraryFunction* libraryFunction, const char* symbolName)
{
#if defined(OSUnix)
	const LibraryFunction functionPointer = (LibraryFunction*)dlsym(handle, symbolName);
	const char* errorString = dlerror();
	const unsigned char successful = errorString;
#elif defined(OSWindows)
	const LibraryFunction functionPointer = GetProcAddress(handle, symbolName);
	const unsigned char successful = functionPointer;
#endif

#if defined(OSUnix)
	libraryFunction = (void*)functionPointer;
#elif defined(OSWindows)
	libraryFunction = functionPointer;
#endif

	return 1u;
}

unsigned char LibraryParseSymbols()
{
#if defined(OSUnix)


#elif defined(OSWindows)
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
