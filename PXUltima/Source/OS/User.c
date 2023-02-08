#include "User.h"

#include <OS/OSVersion.h>

#if OSUnix

#elif OSWindows
#include <Windows.h>
#endif

PXSize UserNameGetA(char* name, const PXSize nameSizeMax)
{
#if OSUnix

	return 0;

#elif OSWindows
	DWORD size = nameSizeMax;

	const PXBool sucessful = GetComputerNameA(name, &size);
	const PXSize sizeresult = sucessful * size;

	return sizeresult;
#endif
}

PXSize UserNameGetW(wchar_t* name, const PXSize nameSizeMax)
{
#if OSUnix

	return 0;

#elif OSWindows
	DWORD size = nameSizeMax;

	const PXBool sucessful = GetComputerNameW(name, &size);
	const PXSize sizeresult = sucessful * size;

	return sizeresult;
#endif
}