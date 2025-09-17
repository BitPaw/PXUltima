#include "PXZSTD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Async/PXLock.h>

#include <ZSTD/zstd.h>

const char PXZSTDText[] = "ZSTD";


PXResult PXAPI  PXZSTDCompress(PXFile* const pxFileInput, PXFile* const pxFileOutput, const PXI8U compressionLevel)
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

PXLock pxLockTest;
PXBool isCreated = 0;

PXResult PXAPI  PXZSTDDecompress(PXFile* const pxFileInput, PXFile* const pxFileOutput)
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

    if(!isCreated)
    {
        isCreated = 1;
        PXLockCreate(&pxLockTest, PXLockTypeProcessOnly);

    }

   // PXLockEngage(&pxLockTest);

    pxFileOutput->DataUsed = ZSTD_decompress(pxFileOutput->Data, pxFileOutput->DataAllocated, pxFileInput->Data, pxFileInput->DataUsed);

   // PXLockRelease(&pxLockTest);


    return PXActionSuccessful;
}