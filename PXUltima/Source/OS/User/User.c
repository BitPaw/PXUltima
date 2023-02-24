#include "User.h"

#include <OS/System/OSVersion.h>

#if OSUnix

#elif OSWindows
#include <Windows.h>
#endif

PXSize UserNameGetA(PXTextASCII* name, const PXSize nameSizeMax)
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

PXSize UserNameGetW(PXTextUNICODE* name, const PXSize nameSizeMax)
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

PXSize UserNameGetU(PXTextUTF8* name, const PXSize nameSizeMax)
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