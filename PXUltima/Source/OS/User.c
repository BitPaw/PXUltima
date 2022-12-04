#include "User.h"

#include <OS/OSVersion.h>

#if OSUnix

#elif OSWindows
#include <Windows.h>
#endif

PXBool UserNameGetA(char* name, const PXSize nameSizeMax, PXSize* nameSizeWritten)
{
#if OSUnix

	return 0;

#elif OSWindows
	DWORD size = nameSizeMax;

	const PXBool sucessful = GetComputerNameA(name, &size);

	nameSizeWritten = sucessful * size;

	return sucessful;
#endif
}

PXBool UserNameGetW(wchar_t* name, const PXSize nameSizeMax, PXSize* nameSizeWritten)
{
#if OSUnix

	return 0;

#elif OSWindows
	DWORD size = nameSizeMax;

	const PXBool sucessful = GetComputerNameW(name, &size);

	nameSizeWritten = sucessful * size;

	return sucessful;
#endif
}