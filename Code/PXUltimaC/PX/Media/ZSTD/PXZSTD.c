#include "PXZSTD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Async/PXLock.h>

#include <ZSTD/zstd.h>

const char PXZSTDText[] = "ZSTD";

PXResult PXAPI PXZSTDCompress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput, const PXI8U compressionLevel)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXZSTDText,
        "Compress",
        "..."
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXZSTDDecompress(PXFile PXREF pxFileInput, PXFile PXREF pxFileOutput)
{
#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXZSTDText,
        "Decompress",
        "..."
    );
#endif

    PXBuffer* pxBufferIN = PXFileBufferGET(pxFileInput);
    PXBuffer* pxBufferOUT = PXFileBufferGET(pxFileOutput);

    pxBufferOUT->CursorOffsetByte = ZSTD_decompress
    (
        pxBufferOUT->Data,
        pxBufferOUT->SizeAllocated,
        pxBufferIN->Data,
        pxBufferIN->SizeAllocated
    );

    return PXResultOK;
}