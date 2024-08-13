#include "PXLevel.h"

#include <OS/File/PXFile.h>
#include <OS/Console/PXConsole.h>

const char PXLevelSignature[] = {'P', 'X', 'L', 'V'};

PXActionResult PXAPI PXLevelLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    // Signature

    // Version

    // Info

    // container

    return PXActionSuccessful;
}

PXActionResult PXAPI PXLevelSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXLevel pxLevel;

    // Write Header
    {
        PXFileDataElementType pxFileDataElementList[] =
        {
            {PXLevelSignature, PXDataTypeDatax4},
            {&pxLevel.Version, PXDataTypeInt16U},
            {&pxLevel.EntryAmount, PXDataTypeInt32U},
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
            PXFileDataElementType pxFileDataElementList[] =
            {
                {&pxLevelEntry.ResourceType, PXDataTypeDatax4},
                {&pxLevelEntry.DataSize, PXDataTypeInt64U}
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