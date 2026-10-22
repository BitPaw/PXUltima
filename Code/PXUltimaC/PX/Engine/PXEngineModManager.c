#include "PXEngineModManager.h"
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXDirectory.h>
#include <PX/OS/File/PXFile.h>
#include "PXMod.h"

static const char PXModCreateFunctionName[] = "PXModCreate";
static const char PXModLoadFunctionName[] = "PXModLoad";
static const char PXModUnloadFunctionName[] = "PXModUnload";

const char PXEngineModManagerText[] = "ModManager-PX";
const PXI8U PXEngineModManagerTextLength = sizeof(PXEngineModManagerText);
const PXECSRegisterInfoStatic PXEngineModManagerInfoStatic =
{
    {sizeof(PXEngineModManagerText), sizeof(PXEngineModManagerText), PXEngineModManagerText, TextFormatASCII},
    sizeof(PXEngineModManager),
    __alignof(PXEngineModManager),
    PXECSTypeSystem,
    PXEngineModManagerCreate,
    PXEngineModManagerRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineModManagerInfoDynamic;

void PXAPI PXEngineModManagerRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXEngineModManagerInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXEngineModManagerInfoDynamic;
}

PXResult PXAPI PXEngineModManagerCreate(PXEngineModManager** pxEngineModManagerREF, PXEngineModManagerCreateInfo PXREF pxEngineModManagerCreateInfo)
{
    PXEngineModManager* pxEngineModManager = PXNull;

    if(!(pxEngineModManagerREF && pxEngineModManagerCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxEngineModManagerCreateInfo->Info.Static = &PXEngineModManagerInfoStatic;
    pxEngineModManagerCreateInfo->Info.Dynamic = &PXEngineModManagerInfoDynamic;
    PXResult pxResult = PXECSCreate(pxEngineModManagerREF, pxEngineModManagerCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngineModManager = *pxEngineModManagerREF;

    PXEngineModuleCallbackUpdateASYNCSet(&pxEngineModManager->ModuleInfo, PXEngineModManagerUpdate);

    pxResult = PXEngineModManagerScan(pxEngineModManager, pxEngineModManagerCreateInfo);

    return pxResult;
}

PXResult PXAPI PXEngineModManagerRelease(PXEngineModManager PXREF pxEngineModManager)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineModManagerUpdate(PXEngineModManager PXREF pxEngineModManager, PXEngineUpdateInfo PXREF PXEngineUpdateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineModManagerScan(PXEngineModManager PXREF pxEngineModManager, PXEngineModManagerCreateInfo PXREF pxEngineModManagerCreateInfo)
{
    PXResult pxResult = PXResultInvalid;
    PXBool hasNextEntry = PXFalse;

    if(!(pxEngineModManager && pxEngineModManagerCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXText PXREF pxTextDtiectory = &pxEngineModManagerCreateInfo->FilePathModFolder;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineModManagerText,
        "Scan",
        "Checking mods folder <%s> ...",
        pxTextDtiectory->A
    );
#endif

    PXDirectorySearchCache pxDirectorySearchCache;
    PXFileEntry pxFileEntry;

    // Open    
    pxResult = PXDirectoryOpen(&pxDirectorySearchCache, &pxFileEntry, pxTextDtiectory);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }    

    for(;;)
    {
        PXModCreateInfo pxModCreateInfo;
        PXClear(PXModCreateInfo, &pxModCreateInfo);

        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineModManagerText,
            "Scan",
            "Possible Mod file deteced <%s>",
            pxFileEntry.FilePath.A
        );

        if(pxEngineModManagerCreateInfo->AvoidLoadingMods)
        {
            hasNextEntry = PXDirectoryNext(&pxDirectorySearchCache, &pxFileEntry);

            if(!hasNextEntry)
            {
                break;
            }

            continue;
        }

        pxResult = PXLibraryOpen(&pxModCreateInfo.Library, &pxFileEntry.FilePath);

        if(pxResult != PXResultOK)
        {
            PXLogPrint
            (
                PXLoggingError,
                PXEngineModManagerText,
                "Load",
                "File is not a Mod and cannot be opened <%s>",
                pxFileEntry.FilePath.A
            );

            continue;
        }

        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineModManagerText,
            "Load",
            "Library detected, try fetching functions in <%s>",
            pxFileEntry.FilePath.A
        );

        pxResult = PXLibraryGetSymbolA(&pxModCreateInfo.Library, &pxModCreateInfo.Create, PXModCreateFunctionName, PXTrue);

        if(!pxModCreateInfo.Create)
        {
            PXLogPrint
            (
                PXLoggingError,
                PXEngineModManagerText,
                "Load",
                "Missing or not exported function <%s>. Can not use mod <%s>",
                PXModCreateFunctionName,
                pxFileEntry.FilePath.A
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
            pxFileEntry.FilePath.A
        );


        // Create mod
        pxResult = PXSafeCallx2(pxModCreateInfo.Create, pxEngineModManager , &pxModCreateInfo);

        if(PXResultOK == pxResult)
        {
            PXMod* pxMod = 0;

            pxResult = PXModCreate(&pxMod, &pxModCreateInfo);

            if(PXResultOK == pxResult)
            {
                PXDictionaryECSAdd(&pxEngineModManager->ModLookup, pxMod);
            }
            else
            {
                // Mod create failure
            }
        }
        else
        {
            // Mod setup call failure
        }

        hasNextEntry = PXDirectoryNext(&pxDirectorySearchCache, &pxFileEntry);

        if(!hasNextEntry)
        {
            break;
        }
    } 

    PXDirectoryClose(&pxDirectorySearchCache);

    return pxResult;
}