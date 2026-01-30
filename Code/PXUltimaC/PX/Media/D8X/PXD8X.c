#include "PXD8X.h"

#include <PX/OS/PXOS.h>
#include <PX/Media/DEFLATE/PXDEFLATE.h>
#include <PX/OS/Console/PXConsole.h>

const char PXD8XName[] = "D8X";

PXResult PXAPI PXD8XLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXD8X pxD8X;

    return PXResultOK;
}

PXResult PXAPI PXD8DatLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
  // PXPNG* png = PXNull;
    PXFile* pxFile = pxResourceLoadInfo->FileCurrent;

    PXD8XDat dummy;
    PXClear(PXD8XDat, &dummy);

    PXD8XDat* pxD8XDat = &dummy;

    PXFileReadI32U(pxFile, &pxD8XDat->TableEntryAmount);
    PXFileReadI32U(pxFile, &pxD8XDat->TableOffset);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXD8XName,
        "Header",
        "\n"
        "%20s : %8i\n"
        "%20s : %8i\n",
        "TableEntryAmount", pxD8XDat->TableEntryAmount,
        "TableOffset", pxD8XDat->TableOffset
    );
#endif

    PXBool invalidLength = PXFileIsIndexInRegion(pxFile, pxD8XDat->TableOffset);

    if(!invalidLength)
    {
        return PXActionRefusedIndexOutOfBounce;
    }

    pxD8XDat->PXD8XDatEntry = PXMemoryHeapCallocT(PXD8XDatEntry, pxD8XDat->TableEntryAmount);

    PXFileCursorMoveTo(pxFile, pxD8XDat->TableOffset);

    for(PXI32U i = 0; i < pxD8XDat->TableEntryAmount; ++i)
    {
        PXD8XDatEntry* pxD8XDatEntry = &pxD8XDat->PXD8XDatEntry[i];

        PXFileReadI32U(pxFile, &pxD8XDatEntry->Hash);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->Offset);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->ChunksCount);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->CompressedSize);
        PXFileReadI32U(pxFile, &pxD8XDatEntry->DecompressedSize);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXD8XName,
            "Entry",
            "\n"
            "%20s : %8.8x\n"
            "%20s : %8i\n"
            "%20s : %8i\n"
            "%20s : %8i\n"
            "%20s : %8i\n",
            "Hash", pxD8XDatEntry->Hash,
            "Offset", pxD8XDatEntry->Offset,
            "ChunksCount", pxD8XDatEntry->ChunksCount,
            "CompressedSize", pxD8XDatEntry->CompressedSize,
            "DecompressedSize", pxD8XDatEntry->DecompressedSize
        );
#endif
    }

    for(PXI32U i = 0; i < pxD8XDat->TableEntryAmount; ++i)
    {
        PXD8XDatEntry* pxD8XDatEntry = &pxD8XDat->PXD8XDatEntry[i];

        // Jump to data
        PXFileCursorMoveTo(pxFile, pxD8XDatEntry->Offset);

        // 0 means we dont have compression, data is "as is"?
        PXBool isCompressed = pxD8XDatEntry->CompressedSize != 0;

        
        pxD8XDatEntry->FileData = PXFileCreate();

        PXFileOpenInfo pxFileOpenInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeWriteOnly;
        pxFileOpenInfo.FileSizeRequest = pxD8XDatEntry->DecompressedSize+4096*10;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual;

        PXFileOpen(pxD8XDatEntry->FileData, &pxFileOpenInfo);
           

        if(isCompressed)
        {
            // We dont have one single continius ZLIB stream, it is chunked.. for some reason.
            PXI32U offset = 0;
            PXI32S compressedSize = 0;
            PXFile* pxFileChunk = PXFileCreate();
            PXFile* pxFileChunkOut = PXFileCreate();

            for(PXI32U chunkIndex = 0; chunkIndex < pxD8XDatEntry->ChunksCount; ++chunkIndex)
            {
                PXFileReadI32U(pxFile, &offset);
                PXFileReadI32S(pxFile, &compressedSize);

                PXSize returnPosition = PXFileDataPosition(pxFile);

                // Jump to location
                PXFileCursorMoveTo(pxFile, offset);

                {
                    PXFileOpenInfo pxFileOpenInfo;
                    PXClear(PXFileOpenInfo, &pxFileOpenInfo);
                    pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
                    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
                    pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
                    PXBufferSet
                    (
                        &pxFileOpenInfo.Data,
                        (PXSize)PXFileDataAtCursor(pxFile) + 2,
                        compressedSize - 2
                    );
                    PXFileOpen(pxFileChunk, &pxFileOpenInfo);

                    PXClear(PXFileOpenInfo, &pxFileOpenInfo);
                    pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
                    pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
                    pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
                    PXBufferSet
                    (
                        &pxFileOpenInfo.Data,
                        PXFileDataAtCursor(pxD8XDatEntry->FileData),
                        PXFileRemainingSize(pxD8XDatEntry->FileData)
                    );
                    PXFileOpen(pxFileChunkOut, &pxFileOpenInfo);
                }

                //PXFileCursorMoveTo(pxFileChunk, 2);                

                PXDEFLATEParse(pxFileChunk, pxFileChunkOut);

                PXFileCursorAdvance(pxD8XDatEntry->FileData, PXFileDataPosition(pxFileChunkOut));

                // We go back
                PXFileCursorMoveTo(pxFile, returnPosition);
            }

            //PXFileRelese(pxFileChunk);
        }
        else
        {
            PXFileReadB(pxFile, pxD8XDatEntry->FileData, pxD8XDatEntry->DecompressedSize);
        }



        char fileName[64];
        PXTextFromAdressA(&pxFileOpenInfo.FilePath, fileName, 0, sizeof(fileName));
        PXTextPrint(&pxFileOpenInfo.FilePath, "J/Unnamed_%8x8", pxD8XDatEntry->Hash);

        PXFileStoreOnDiskA(pxD8XDatEntry->FileData, fileName);
        PXFileClose(pxD8XDatEntry->FileData);
    }

    return PXResultOK;
}

PXResult PXAPI PXD8GLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXD8WLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXD8TLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXD8MLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    return PXResultOK;
}
