#include "PXMod.h"

#include <PX/OS/File/PXDirectory.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Library/PXLibrary.h>

PXResult PXAPI  PXModLoaderScan(PXModLoader* const pxModLoader, const PXText* const pxTextModFileDirectory)
{
#if 00000000000

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "ModLoader",
        "Scan",
        "Loading Mods..."
    );
#endif

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
            "Load",
            "Possible Mod file deteced <%s>",
            pxDirectoryIterator.EntryCurrent.Name
        );

        PXText pyText;
        PXTextConstructFromAdressA(&pyText, pxDirectoryIterator.EntryCurrent.FullPath, pxDirectoryIterator.EntryCurrent.FullPathSize, pxDirectoryIterator.EntryCurrent.FullPathSize);

        const PXActionResult pxActionResult = PXLibraryOpen(&pxMod.Library, &pyText);

        if (pxActionResult != PXActionSuccessful)
        {
            PXLogPrint
            (
                PXLoggingError,
                "Mod",
                "Load",
                "File is not a Mod and cannot be opened <%s>",
                pxDirectoryIterator.EntryCurrent.FullPath
            );

            continue;
        }

        PXLogPrint
        (
            PXLoggingInfo,
            "Mod",
            "Load",
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
                "Load",
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
            "Load",
            "Function detected <%s>. Linking.. <%s>",
            PXModLoadFunctionName,
            pxDirectoryIterator.EntryCurrent.FullPath
        );

        PXModLoaderAdd(pxModLoader, &pxMod);
    }
    while (PXDirectoryNext(&pxDirectoryIterator));

    PXDirectoryClose(&pxDirectoryIterator);

#endif

    return PXActionSuccessful;
}

PXResult PXAPI  PXModLoaderAdd(PXModLoader* const pxModLoader, PXMod* const pxMod)
{
    pxMod->Load(PXNull, pxMod, 0);

    PXLogPrint
    (
        PXLoggingInfo,
        "Mod",
        "Load",
        "<%s>, <%s>",
        pxMod->Name,
        pxMod->BuildDate
    );
}
