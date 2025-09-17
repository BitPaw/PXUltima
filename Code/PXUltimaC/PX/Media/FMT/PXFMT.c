#include "PXFMT.h"

#include <PX/OS/File/PXFile.h>

static const char FMTSignature[4] = "fmt ";

PXResult PXAPI  PXFMTLoadFromFile(PXFMT* const fmt, PXFile* const pxFile, const PXEndian endian)
{
    PXClear(PXFMT, fmt);

    // Check header signature
    {
        const PXBool valid = PXFileReadAndCompare(pxFile, FMTSignature, sizeof(FMTSignature));

        if(!valid)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    const PXI32U x16 = endian == PXEndianLittle ? PXTypeInt16ULE : PXTypeInt16UBE;
    const PXI32U x32 = endian == PXEndianLittle ? PXTypeInt32ULE : PXTypeInt32UBE;

    const PXTypeEntry pxDataStreamElementList[] =
    {
        {&fmt->ChunkSize,x32},
        {&fmt->AudioFormat,x16},
        {&fmt->NumerOfChannels,x16},
        {&fmt->SampleRate,x32},
        {&fmt->ByteRate,x32},
        {&fmt->BlockAllign,x16},
        {&fmt->BitsPerSample,x16}
    };

    PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

    return PXActionSuccessful;
}

PXResult PXAPI  PXFMTSaveToFile(const PXFMT* const fmt, PXFile* const pxFile, const PXEndian endian)
{
    const PXI32U x16 = endian == PXEndianLittle ? PXTypeInt16ULE : PXTypeInt16UBE;
    const PXI32U x32 = endian == PXEndianLittle ? PXTypeInt32ULE : PXTypeInt32UBE;

    const PXTypeEntry pxDataStreamElementList[] =
    {
        {FMTSignature, PXTypeDatax4},
        {&fmt->ChunkSize,x32},
        {&fmt->AudioFormat,x16},
        {&fmt->NumerOfChannels,x16},
        {&fmt->SampleRate,x32},
        {&fmt->ByteRate,x32},
        {&fmt->BlockAllign,x16},
        {&fmt->BitsPerSample,x16}
    };

    PXFileWriteMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

    return PXActionSuccessful;
}
