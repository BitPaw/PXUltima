#include "PXOGG.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Memory/PXMemory.h>

const static char PXOGGHeaderSignature[4] = "OggS";

#define PXOGGHeaderTypeContinuation 0x01
#define PXOGGHeaderTypeBeginningOfStream 0x02
#define PXOGGHeaderTypeEndOfStream 0x04

#define PXOPGGDebug 0

PXResult PXAPI PXOGGLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXOGG ogg;

    PXClear(PXOGG, &ogg);

    while(!PXFileIsAtEnd(pxResourceLoadInfo->FileCurrent))
    {
        // Header tag does exist multible times.
        // You can refocus it when the file is corrupted.
        PXOGGPage page;

        PXI32UCluster signature;

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {signature.Data, PXTypeDatax4},
            {&page.Version, PXTypeI08U},
            {&page.HeaderType, PXTypeI08U},
            {&page.GranulePosition, PXTypeI32UBE},
            {&page.SerialNumber, PXTypeI32UBE},
            {&page.SequenceNumber, PXTypeI32UBE},
            {&page.CRC32CheckSum, PXTypeI32UBE},
            {&page.PageSegments, PXTypeI08U}
        };

        PXFileReadMultible(pxResourceLoadInfo->FileCurrent, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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
            PXFileReadI8U(pxResourceLoadInfo->FileCurrent, &segmentSizeList[i]);

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
                unsigned char* currentPos = (unsigned char*)PXFileDataAtCursor(pxResourceLoadInfo->FileCurrent) + i;

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

            PXFileCursorAdvance(pxResourceLoadInfo->FileCurrent, x);
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXOGGSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    PXFile* pxFile = pxResourceSaveInfo->FileCurrent;

    PXFileWriteB(pxFile, PXOGGHeaderSignature, sizeof(PXOGGHeaderSignature));

    return PXActionRefusedNotImplemented;
}
