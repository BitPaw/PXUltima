#include "PXOGG.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

const static char PXOGGHeaderSignature[4] = { 'O','g','g','S' };

#define PXOGGHeaderTypeContinuation 0x01
#define PXOGGHeaderTypeBeginningOfStream 0x02
#define PXOGGHeaderTypeEndOfStream 0x04

#define PXOPGGDebug 0

PXActionResult PXAPI PXOGGLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXOGG ogg;

    PXClear(PXOGG, &ogg);

    while(!PXFileIsAtEnd(pxResourceLoadInfo->FileReference))
    {
        // Header tag does exist multible times.
        // You can refocus it when the file is corrupted.
        PXOGGPage page;

        PXInt32UCluster signature;

        const PXDataTypeEntry pxDataStreamElementList[] =
        {
            {signature.Data, PXDataTypeDatax4},
            {&page.Version, PXDataTypeInt08U},
            {&page.HeaderType, PXDataTypeInt08U},
            {&page.GranulePosition, PXDataTypeInt32UBE},
            {&page.SerialNumber, PXDataTypeInt32UBE},
            {&page.SequenceNumber, PXDataTypeInt32UBE},
            {&page.CRC32CheckSum, PXDataTypeInt32UBE},
            {&page.PageSegments, PXDataTypeInt08U}
        };

        PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        const PXBool validHeaderSignature = PXMemoryCompare(signature.Data, 4u, PXOGGHeaderSignature, sizeof(PXOGGHeaderSignature));

        if (!validHeaderSignature)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }



        unsigned char segmentSizeList[0xFF];

#if PXOPGGDebug
        printf
        (
            "|         | %7s | %4s | %8s | %6s | %7s |

            s | %4s |\n"
            "|  Chunk  | %7i | %4i | %8zi | %6i | %7i | %11i | %4i |\n",

            "Version",
            "Type",
            "Position",
            "Serial",
            "Sequenc",
            "CRC",
            "Page",

            page.Version,
            page.HeaderType,
            page.GranulePosition,
            page.SerialNumber,
            page.SequenceNumber,
            page.CRC32CheckSum,
            page.PageSegments
        );
#endif

        for(PXSize i = 0; i < page.PageSegments; ++i)
        {
            PXFileReadI8U(pxResourceLoadInfo->FileReference, &segmentSizeList[i]);

#if PXOPGGDebug
            printf
            (
                "| Segment | %3i/%3i | %3i Bytes %-45s |\n",
                i + 1,
                page.PageSegments,
                segmentSizeList[i],
                ""
            );
#endif
        }

        for(PXSize i = 0; i < page.PageSegments; ++i)
        {
            unsigned char x = segmentSizeList[i];

            for(PXSize i = 0; i < x; i++)
            {
                unsigned char* currentPos = (unsigned char*)PXFileCursorPosition(pxResourceLoadInfo->FileReference) + i;

                char print = (*currentPos >= ' ' && *currentPos <= '~') ? *currentPos : '.';

#if PXOPGGDebug
                printf("%c", print);

                if(((i + 1) % 64u) == 0)
                {
                    printf("\n");
                }
#endif
            }

#if PXOPGGDebug
            printf("\n");
#endif

            PXFileCursorAdvance(pxResourceLoadInfo->FileReference, x);
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOGGSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXFileWriteB(pxResourceSaveInfo->FileReference, PXOGGHeaderSignature, sizeof(PXOGGHeaderSignature));

    return PXActionRefusedNotImplemented;
}
