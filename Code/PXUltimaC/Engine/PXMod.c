#include "PXMod.h"

#include <OS/File/PXDirectory.h>
#include <Log/PXLog.h>
#include <OS/Library/PXLibrary.h>

PXActionResult PXAPI PXModLoaderScan(PXModLoader* const pxModLoader, const PXText* const pxTextModFileDirectory)
{
	PXDirectoryIterator pxDirectoryIterator;	

	// Open
	{
		const PXActionResult directoryOpenResult = PXDirectoryOpen(&pxDirectoryIterator, pxTextModFileDirectory);

		PXActionReturnOnError(directoryOpenResult);
	}

	do
	{
		PXMod pxMod;
		PXClear(PXMod, &pxMod);

		PXLogPrint
		(
			PXLoggingInfo,
			"Mod",
			"Possible Mod file deteced <%s>",
			pxDirectoryIterator.EntryCurrent.Name
		);

		PXText pyText;
		PXTextConstructFromAdressA(&pyText, pxDirectoryIterator.EntryCurrent.FullPath, pxDirectoryIterator.EntryCurrent.FullPathSize);

		const PXActionResult pxActionResult = PXLibraryOpen(&pxMod.Library, &pyText);

		if (pxActionResult != PXActionSuccessful)
		{
			PXLogPrint
			(
				PXLoggingError,
				"Mod",
				"File is not a Mod and cannot be opened <%s>",
				pxDirectoryIterator.EntryCurrent.FullPath
			);

			continue;
		}

		PXLogPrint
		(
			PXLoggingInfo,
			"Mod",
			"Library detected, try fetching functions in <%s>",
			pxDirectoryIterator.EntryCurrent.FullPath
		);

		PXLibraryGetSymbolA(&pxMod.Library, &pxMod.Load, PXModLoadFunctionName);

		if (!pxMod.Load)
		{
			PXLogPrint
			(
				PXLoggingError,
				"Mod",
				"Library does not contain function <%s>. Cannot use mod <%s>",
				PXModLoadFunctionName,
				pxDirectoryIterator.EntryCurrent.FullPath
			);

			continue;
		}
		 
		PXLogPrint
		(
			PXLoggingInfo,
			"Mod",
			"Function detected <%s>. Linking.. <%s>",
			PXModLoadFunctionName,
			pxDirectoryIterator.EntryCurrent.FullPath
		);

		PXModLoaderAdd(pxModLoader, &pxMod);
	}
	while (PXDirectoryNext(&pxDirectoryIterator));

	PXDirectoryClose(&pxDirectoryIterator);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXModLoaderAdd(PXModLoader* const pxModLoader, PXMod* const pxMod)
{
	pxMod->Load(PXNull, pxMod, 0);

	PXLogPrint
	(
		PXLoggingInfo,
		"Mod",
		"<%s>, <%s>",
		pxMod->Name,
		pxMod->BuildDate
	);
}