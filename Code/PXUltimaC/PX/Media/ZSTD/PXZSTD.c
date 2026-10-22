#include "PXZSTD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Async/PXLock.h>

#define ZSTDLibUse 0

#if ZSTDLibUse
#include <ZSTD/zstd.h>
#endif


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

    return PXResultRefusedNotImplemented;
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

#if ZSTDLibUse
    PXBuffer* pxBufferIN = PXFileBufferGET(pxFileInput);
    PXBuffer* pxBufferOUT = PXFileBufferGET(pxFileOutput);

    pxBufferOUT->CursorOffsetByte = ZSTD_decompress
    (
        pxBufferOUT->Data4,
        pxBufferOUT->SizeAllocated,
        pxBufferIN->Data4,
        pxBufferIN->SizeAllocated
    );
#endif

    return PXResultOK;
}
