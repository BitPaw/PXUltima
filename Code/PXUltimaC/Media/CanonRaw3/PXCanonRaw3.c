#include "PXCanonRaw3.h"

#include <Media/TIFF/PXTIFF.h>
#include <OS/File/PXFile.h>
#include <OS/Console/PXConsole.h>

#define  PXCanonRaw3FTYPID PXInt32Make('f', 't', 'y', 'p')
#define  PXCanonRaw3FREEID PXInt32Make('f', 'r', 'e', 'e')
#define  PXCanonRaw3CLIPID PXInt32Make('c', 'l', 'i', 'p')
#define  PXCanonRaw3CRGNID PXInt32Make('c', 'r', 'g', 'n')
#define  PXCanonRaw3MATTID PXInt32Make('m', 'a', 't', 't')
#define  PXCanonRaw3KMATID PXInt32Make('k', 'm', 'a', 't')
#define  PXCanonRaw3PNOTID PXInt32Make('p', 'n', 'o', 't')
#define  PXCanonRaw3CTABID PXInt32Make('c', 't', 'a', 'b')
#define  PXCanonRaw3LOADID PXInt32Make('l', 'o', 'a', 'd')
#define  PXCanonRaw3IMAPID PXInt32Make('i', 'm', 'a', 'p')
#define  PXCanonRaw3TMCDID PXInt32Make('t', 'm', 'c', 'd')
#define  PXCanonRaw3CHAPID PXInt32Make('c', 'h', 'a', 'p')
#define  PXCanonRaw3SYNCID PXInt32Make('s', 'y', 'n', 'c')
#define  PXCanonRaw3SCPTID PXInt32Make('s', 'c', 'p', 't')
#define  PXCanonRaw3SSRCID PXInt32Make('s', 's', 'r', 'c')
#define  PXCanonRaw3ISOMID PXInt32Make('i', 's', 'o', 'm')
#define  PXCanonRaw3UUIDID PXInt32Make('u', 'u', 'i', 'd')

#define  PXCanonRaw3MOOVID PXInt32Make('m', 'o', 'o', 'v')
#define  PXCanonRaw3MOOVCCTPID PXInt32Make('C', 'C', 'T', 'P')
#define  PXCanonRaw3MOOVCMT1ID PXInt32Make('C', 'M', 'T', '1')
#define  PXCanonRaw3MOOVCMT2ID PXInt32Make('C', 'M', 'T', '2')
#define  PXCanonRaw3MOOVCMT3ID PXInt32Make('C', 'M', 'T', '3')
#define  PXCanonRaw3MOOVGPSInfoID PXInt32Make('C', 'M', 'T', '4')
#define  PXCanonRaw3MOOVCompressorVersionID PXInt32Make('C', 'N', 'C', 'V')
#define  PXCanonRaw3MOOVCNOPID PXInt32Make('C', 'N', 'O', 'P')
#define  PXCanonRaw3MOOVCNTHID PXInt32Make('C', 'N', 'T', 'H')
#define  PXCanonRaw3MOOVThumbnailImageID PXInt32Make('T', 'H', 'M', 'B')
#define  PXCanonRaw3MOOVCTBOID PXInt32Make('C', 'T', 'B', 'O')

#define PXCanonRaw3Debug 1

typedef struct PXCanonRaw3ChunkMOOV_
{
    char UUID[16];

    PXInt8U CompressiorVersionSize;
    char CompressiorVersion[64];

}
PXCanonRaw3ChunkMOOV;


typedef struct PXCanonRaw3ChunkFTYP_
{
    PXInt32UCluster TypeBrandMajor;
    PXInt32U TypeVersionMinor;
    PXInt32UCluster TypeCompatibleBrands;
}
PXCanonRaw3ChunkFTYP;

typedef struct PXCanonRaw3ChunkUUID_
{
    char UUID[16];
}
PXCanonRaw3ChunkUUID;

typedef struct PXCanonRaw3Chunk_
{
    PXInt32UCluster ID;
    PXInt32U SizeTotal;
    PXInt32U SizeData;

    PXSize PredictedFinalPosition;

    PXSize BufferSize;
    char Buffer[64];

    union
    {
        PXCanonRaw3ChunkUUID ChunkUUID;
        PXCanonRaw3ChunkFTYP ChunkFTYP;


    };
}
PXCanonRaw3Chunk;


PXActionResult PXAPI PXCanonRaw3BlockRead(PXCanonRaw3Chunk* const pxCanonRaw3Chunk, PXFile* const pxFile)
{
    const PXDataTypeEntry pxDataStreamElementList[] =
    {
        {&pxCanonRaw3Chunk->SizeTotal, PXDataTypeInt32UBE},
        {pxCanonRaw3Chunk->ID.Data, PXDataTypeDatax4}
    };

    const PXSize readBytes = PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

    if (pxCanonRaw3Chunk->SizeTotal > 0)
    {
        pxCanonRaw3Chunk->SizeData = pxCanonRaw3Chunk->SizeTotal - readBytes;
    }
    else
    {
        pxCanonRaw3Chunk->SizeData = 0;
    }

    pxCanonRaw3Chunk->PredictedFinalPosition = pxFile->DataCursor + pxCanonRaw3Chunk->SizeData;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "CanonRaw3",
        "Parse",
        "Chunk %4.4s Size:%iB",
        pxCanonRaw3Chunk->ID.Data,
        pxCanonRaw3Chunk->SizeTotal
    );
#endif

    switch (pxCanonRaw3Chunk->ID.Value)
    {
    case PXCanonRaw3FTYPID:
    {
        const PXDataTypeEntry pxDataStreamElementList[] =
        {
            {pxCanonRaw3Chunk->ChunkFTYP.TypeBrandMajor.Data, PXDataTypeDatax4},
            {&pxCanonRaw3Chunk->ChunkFTYP.TypeVersionMinor, PXDataTypeInt32UBE},
            {pxCanonRaw3Chunk->ChunkFTYP.TypeCompatibleBrands.Data, PXDataTypeDatax4}
        };

        PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        break;
    }
    case PXCanonRaw3MOOVID:
    {
        PXCanonRaw3ChunkMOOV pxCanonRaw3ChunkMOOV;

        for (PXBool done = PXFalse ; !done ; )
        {
            PXCanonRaw3Chunk pxCanonRaw3ChunkChild;
            PXCanonRaw3BlockRead(&pxCanonRaw3ChunkChild, pxFile);

            switch (pxCanonRaw3ChunkChild.ID.Value)
            {
            case PXCanonRaw3UUIDID:
            {
                PXMemoryCopy(pxCanonRaw3ChunkChild.ChunkUUID.UUID, 16, pxCanonRaw3ChunkMOOV.UUID, 16);
                break;
            }
            case PXCanonRaw3MOOVCompressorVersionID:
            {
                pxCanonRaw3ChunkMOOV.CompressiorVersionSize = PXTextCopyA(pxCanonRaw3ChunkChild.Buffer, pxCanonRaw3ChunkChild.BufferSize, pxCanonRaw3ChunkMOOV.CompressiorVersion, 64);
                break;
            }
            case PXCanonRaw3MOOVCMT1ID:
            {
                PXFile tiffData;
                // PXFileBufferExternal(&tiffData, PXFileCursorPosition(pxFile), pxCanonRaw3ChunkChild.SizeData);

                PXImage pxxx;

                //PXTIFFLoadFromFile(&pxxx, &tiffData);

                break;
            }
            case PXCanonRaw3MOOVCCTPID:
            case PXCanonRaw3MOOVCMT2ID:
            case PXCanonRaw3MOOVCMT3ID:
            case PXCanonRaw3MOOVGPSInfoID:
            case PXCanonRaw3MOOVCNOPID:
            case PXCanonRaw3MOOVCNTHID:
            case PXCanonRaw3MOOVThumbnailImageID:
            case PXCanonRaw3FREEID:
            case PXCanonRaw3MOOVCTBOID:
            default:
            {
                PXFileCursorAdvance(pxFile, pxCanonRaw3ChunkChild.SizeData);
                break;
            }

            }
        }

        break;
    }
    case PXCanonRaw3ISOMID:
    {





        // parse moov

        // parse header mvhd

        // trak

        // tref

        // trgr

        break;
    }
    case PXCanonRaw3UUIDID:
    {
        pxCanonRaw3Chunk->BufferSize = PXFileReadB(pxFile, pxCanonRaw3Chunk->ChunkUUID.UUID,16u);
        break;
    }
    case PXCanonRaw3MOOVCCTPID:
    {
        //printf("\n");
        break;
    }
    case PXCanonRaw3MOOVCMT1ID:
    {
        //printf("\n");
        break;
    }
    case PXCanonRaw3MOOVCMT2ID:
    {
        //printf("\n");
        break;
    }
    case PXCanonRaw3MOOVCMT3ID:
    {
        //printf("\n");
        break;
    }
    case PXCanonRaw3MOOVGPSInfoID:
    {
        //printf("\n");
        break;
    }
    case PXCanonRaw3MOOVCompressorVersionID:
    {
        pxCanonRaw3Chunk->BufferSize = PXFileReadB(pxFile, pxCanonRaw3Chunk->Buffer, pxCanonRaw3Chunk->SizeData);
        break;
    }
    case PXCanonRaw3MOOVCNOPID:
    {
        //    printf("\n");
        break;
    }
    case PXCanonRaw3MOOVCNTHID:
    {
        //printf("\n");
        break;
    }
    case PXCanonRaw3MOOVThumbnailImageID:
    {
        //printf("\n");
        break;
    }

    default:
        break;
    }

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCanonRaw3LoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXFile* const pxFile = pxResourceLoadInfo->FileReference;

    PXCanonRaw3 pxCanonRaw3;
    PXClear(PXCanonRaw3, &pxCanonRaw3);

#if PXLogEnable && PXCanonRaw3Debug
    PXLogPrint
    (
        PXLoggingInfo,
        "CanonRaw3",
        "Load",
        "Start parsing..."
    );
#endif

    while (!PXFileIsAtEnd(pxFile))
    {
        PXCanonRaw3Chunk pxCanonRaw3Chunk;

        PXCanonRaw3BlockRead(&pxCanonRaw3Chunk, pxFile);

        PXFileCursorMoveTo(pxFile, pxCanonRaw3Chunk.PredictedFinalPosition);
    }




    // Format is orderd in "boxes". No other data is contained.

    // File Type Box
    {
        PXInt32U amountOfContainers = 0;

        // if no ftyp is contaned, use "Major_brand='mp41', minor_version=0, and the single compatible brand 'mp41'."
        PXFileReadI32UE(pxFile, &amountOfContainers, PXEndianBig);



#if PXLogEnable && PXCanonRaw3Debug
        PXLogPrint
        (
            PXLoggingInfo,
            "CanonRaw3",
            "Load",
            "Boxes detected <%i>",
            amountOfContainers
        );
#endif

        for (PXInt32U i = 0; i < amountOfContainers; ++i)
        {
            PXInt32UCluster boxID;

            PXFileReadI32U(pxFile, &boxID.Value);



        }
    }

    // int, 24 BE
    // ftypcrx // char[8]
    // int, 1 BE
    // cry isom => char[x] defined by first int

    // MOOV

#if PXLogEnable && PXCanonRaw3Debug
    PXLogPrint
    (
        PXLoggingInfo,
        "CanonRaw3",
        "Load",
        "Finished parsing"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXCanonRaw3SaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}
