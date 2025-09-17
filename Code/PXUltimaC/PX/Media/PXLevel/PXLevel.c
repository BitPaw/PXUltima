#include "PXLevel.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

const char PXLevelSignature[] = {'P', 'X', 'L', 'V'};

PXResult PXAPI  PXLevelLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    // Signature

    // Version

    // Info

    // container

    return PXActionSuccessful;
}

PXResult PXAPI  PXLevelSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXLevel pxLevel;

    // Write Header
    {
        PXTypeEntry pxFileDataElementList[] =
        {
            {PXLevelSignature, PXTypeDatax4},
            {&pxLevel.Version, PXTypeInt16U},
            {&pxLevel.EntryAmount, PXTypeInt32U},
        };
        const PXSize pxFileDataElementListSize = sizeof(pxFileDataElementList);

        PXFileWriteMultible(pxResourceSaveInfo->FileReference, pxFileDataElementList, pxFileDataElementListSize);
    }

    // Parse entrys
    {
        PXLevelEntry pxLevelEntry;
        PXClear(PXLevelEntry, &pxLevelEntry);

        while(!PXFileIsAtEnd(pxResourceSaveInfo->FileReference))
        {
            PXTypeEntry pxFileDataElementList[] =
            {
                {&pxLevelEntry.ResourceType, PXTypeDatax4},
                {&pxLevelEntry.DataSize, PXTypeInt64U}
            };
            const PXSize pxFileDataElementListSize = sizeof(pxFileDataElementList);

            PXFileWriteMultible(pxResourceSaveInfo->FileReference, pxFileDataElementList, pxFileDataElementListSize);

            pxLevelEntry.DataAdress = pxResourceSaveInfo->FileReference->DataCursor;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Level",
                "Load",
                "Entry detected : %c%c%c%c, %i B, 0x%p",
                ((char*)pxLevelEntry.ResourceType)[0],
                ((char*)pxLevelEntry.ResourceType)[1],
                ((char*)pxLevelEntry.ResourceType)[2],
                ((char*)pxLevelEntry.ResourceType)[3],
                pxLevelEntry.DataSize,
                pxLevelEntry.DataAdress
            );
#endif
        }
    }


    return PXActionRefusedNotImplemented;
}