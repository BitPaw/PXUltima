#include "PXJFS.h"

#include <PX/Media/ZLIB/PXZLIB.h>

PXResult PXAPI PXJFSRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXJFSLoadFromFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxECSCreateInfo)
{
    PXFile* pxFile = pxECSCreateInfo->FileCurrent;

    PXJFS dummy;
    PXClear(PXJFS, &dummy);

    PXJFS* pxJFS = &dummy;

    PXFileReadI32U(pxFile, &pxJFS->FileEntryAmount);

    for(PXI32U i = 0; i < pxJFS->FileEntryAmount; i++)
    {
        PXJFSEntry* pxJFSEntry = &pxJFS->FileEntry[i];

        PXFileReadI32U(pxFile, &pxJFSEntry->Hash);
        PXFileReadI32U(pxFile, &pxJFSEntry->Offset);
        PXFileReadI32U(pxFile, &pxJFSEntry->CompressedSize);
        PXFileReadI32U(pxFile, &pxJFSEntry->DecompressedSize);
        PXFileReadI32U(pxFile, &pxJFSEntry->CompressionLevel);
        PXFileReadI32U(pxFile, &pxJFSEntry->Compressed);
    }

    for(PXI32U i = 0; i < pxJFS->FileEntryAmount; i++)
    {
        PXJFSEntry* pxJFSEntry = &pxJFS->FileEntry[i];

        PXFileCursorMoveTo(pxFile, pxJFSEntry->Offset);
        
        PXFileCreateInfo pxFileCreateInfo;
        PXClear(PXFileCreateInfo, &pxFileCreateInfo);
        pxFileCreateInfo.AccessMode = PXAccessModeReadAndWrite;
        pxFileCreateInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileCreateInfo.FlagList = PXFileIOInfoFileVirtual;
        pxFileCreateInfo.FileSizeRequest = pxJFSEntry->DecompressedSize;

        PXResult pxResult = PXFileCreate(&pxJFSEntry->Data, &pxFileCreateInfo);

        if(PXResultOK != pxResult)
        {
            continue;
        }

        switch(pxJFSEntry->CompressionLevel)
        {
            case PXJFSEntryCompressionNone:
            {
                PXFileRead(pxFile, pxJFSEntry->Data, pxJFSEntry->DecompressedSize);
                break;
            }
            case PXJFSEntryCompressionZLIB:
            {
                PXZLIBDecompress(pxFile, pxJFSEntry->Data);
                break;
            }
            default:
            {
                continue;
            }
        }
    }

    return PXResultOK;
}
