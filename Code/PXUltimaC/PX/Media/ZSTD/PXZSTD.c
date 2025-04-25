#include "PXZSTD.h"

#include <PX/OS/File/PXFile.h>
#include <ZSTD/zstd.h>

PXActionResult PXAPI PXZSTDCompress(PXFile* const pxFileInput, PXFile* const pxFileOutput, const PXInt8U compressionLevel)
{
    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXZSTDDecompress(PXFile* const pxFileInput, PXFile* const pxFileOutput)
{
    pxFileOutput->DataCursor = ZSTD_decompress(pxFileOutput->Data, pxFileOutput->DataUsed, pxFileInput->Data, pxFileInput->DataUsed);

    return PXActionSuccessful;
}