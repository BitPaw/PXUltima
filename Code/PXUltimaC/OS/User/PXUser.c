#include "PXUser.h"

#include <OS/System/PXOSVersion.h>

#if OSUnix
#include <unistd.h>
#elif OSWindows
#include <windows.h>
#include <userenv.h> // Could use GetUserProfileDirectoryW() but not needed yet

#if WindowsAtleastVista
#include <ShlObj_core.h>
#endif // 0



#endif

PXActionResult PXUserNameGet(PXText* const name)
{
	switch (name->Format)
	{
		case TextFormatUTF8:
		case TextFormatASCII:
		{
#if OSUnix
            name->SizeUsed  = getlogin_r(name->TextA, name->SizeAllocated); // unistd.h

            const PXBool success = name->SizeUsed > 0;

			return success;

#elif OSWindows
			DWORD size = name->SizeAllocated;

			const PXBool sucessful = GetComputerNameA(name->TextA, &size); // Windows 2000 (+UWP), Kernel32.dll, winbase.h
			
			PXActionOnErrorFetchAndReturn(!sucessful);

			name->SizeUsed = (PXSize)sucessful * (PXSize)size;

			return PXActionSuccessful;
#endif

			break;
		}
		case TextFormatUNICODE:
		{
#if OSUnix

			return 0;

#elif OSWindows
			DWORD size = name->SizeAllocated;

			const PXBool sucessful = GetComputerNameW(name->TextW, &size); // Windows 2000 (+UWP), Kernel32.dll, winbase.h

			PXActionOnErrorFetchAndReturn(!sucessful);

			name->NumberOfCharacters = (PXSize)sucessful * (PXSize)size;
			name->SizeUsed = name->NumberOfCharacters * sizeof(wchar_t);

			return PXActionSuccessful;
#endif

			break;
		}
	}

	return PXActionRefusedFormatNotSupported;
}

PXBool PXUserEnviromentFolderGet(PXText* const name, const PXUserEnviromentFolderID pxUserEnviromentFolderID)
{
#if OSUnix

    return PXFalse;

#elif OSWindows

#if WindowsAtleastVista && PXOSWindowsDestop

	const GUID* pathID = 0;

	switch (pxUserEnviromentFolderID)
	{
		case PXUserEnviromentFolderIDAppData:
			pathID = &FOLDERID_RoamingAppData;
			break;

		case PXUserEnviromentFolderIDDownload:
			pathID = &FOLDERID_Downloads;
			break;

		case PXUserEnviromentFolderIDDocuments:
			pathID = &FOLDERID_Documents;
			break;

		case PXUserEnviromentFolderIDPictures:
			pathID = &FOLDERID_Pictures;
			break;

		case PXUserEnviromentFolderIDScreenshots:
			pathID = &FOLDERID_Screenshots;
			break;

		case PXUserEnviromentFolderIDVideo:
			pathID = &FOLDERID_Videos;
			break;

		case PXUserEnviromentFolderIDDestop:
			pathID = &FOLDERID_Desktop;
			break;

		case PXUserEnviromentFolderIDFonts:
			pathID = &FOLDERID_Fonts;
			break;

		case PXUserEnviromentFolderIDMusic:
			pathID = &FOLDERID_Music;
			break;

		default:
			return PXFalse;
	}

	PXText temporalCache;
	temporalCache.SizeAllocated = -1;
	temporalCache.SizeUsed = 0;
	temporalCache.NumberOfCharacters = 0;
	temporalCache.TextW = 0;
	temporalCache.Format = TextFormatUNICODE;

	const HRESULT result = SHGetKnownFolderPath((KNOWNFOLDERID*)pathID, KF_FLAG_DEFAULT_PATH, PXNull, (PWSTR*)temporalCache.TextW); // Windows Vista, Shell32.dll, shlobj_core.h
	const PXBool success = S_OK == result;

	PXTextCopy(&temporalCache, name);

	CoTaskMemFree(temporalCache.TextW); // Needs to be called in ANY case.

	return success;
#else
	return PXFalse;
#endif
#endif
}
