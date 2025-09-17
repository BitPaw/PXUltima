#include "PXWAD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Media/ZSTD/PXZSTD.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/PXOS.h>

const char PXWADText[] = "WAD";


//---------------------------------------------------------
const PXI32U PXWADVersion1List[] =
{
    PXTypeInt16U,
    PXTypeInt16U,
    PXTypeInt32U
};
const PXI8U PXWADVersion1ListSize = sizeof(PXWADVersion1List) / sizeof(PXI32U);
//---------------------------------------------------------


//---------------------------------------------------------
const PXI32U PXWADVersion2List[] =
{
    PXTypeInt08U,
    PXTypeText(83),
    PXTypeInt64U,
    PXTypeInt16U,
    PXTypeInt16U,
    PXTypeInt32U
};
const PXI8U PXWADVersion2ListSize = sizeof(PXWADVersion2List) / sizeof(PXI32U);
//---------------------------------------------------------


//---------------------------------------------------------
const PXI32U PXWADVersion3List[] =
{
    PXTypeText(256),
    PXTypeInt64U,
    PXTypeInt32U
};
const PXI8U PXWADVersion3ListSize = sizeof(PXWADVersion3List) / sizeof(PXI32U);
//---------------------------------------------------------




//---------------------------------------------------------
#define PXWADEntryDataTypeUncompressedData 0
#define PXWADEntryDataTypegzip 1
#define PXWADEntryDataTypeFileRedirection 2
#define PXWADEntryDataTypeZstandard    3
#define PXWADEntryDataTypeZstandardWithSubchunks 4

//---------------------------------------------------------
const PXI32U PXWADEntryHeaderList[] =
{
    PXTypeInt64U,
    PXTypeInt32U,
    PXTypeInt32U,
    PXTypeInt32U,
    PXTypeBit08U(4),
    PXTypeBit08U(4),
    PXTypeInt08U,
    PXTypeInt16U,
    PXTypeInt64U
};
const PXI8U PXWADEntryHeaderListSize = sizeof(PXWADEntryHeaderList) / sizeof(PXI32U);
//---------------------------------------------------------
const PXI32U PXWADHeaderList[] =
{
    PXTypeDatax2,
    PXTypeInt08U,
    PXTypeInt08U
};
const PXI8U PXWADHeaderListSize = sizeof(PXWADHeaderList) / sizeof(PXI32U);
//---------------------------------------------------------


PXResult PXAPI  PXWADEntryHandle(PXWADEntry* const pxWADEntry, PXFile* const pxFile)
{
    PXFile dataCompressed;
    PXFile dataUncompressed;

    PXFileOpenInfo pxFileOpenInfo;

    // Open/Wrap compressed data
    {
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
        pxFileOpenInfo.BufferSize = pxWADEntry->CompressedSize;
        pxFileOpenInfo.BufferData = (PXByte*)pxFile->Data + pxWADEntry->DataOffset;

        PXFileOpen(&dataCompressed, &pxFileOpenInfo);
    }

    // target
    {
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeReadAndWrite;
        pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenInfo.FileSizeRequest = pxWADEntry->UncompressedSize;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileVirtual;
    }

    switch(pxWADEntry->DataType)
    {
        case PXWADEntryDataTypeUncompressedData:
        { 
#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWADText,
                "Load",
                "UncompressedData"
            );
#endif 
            pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
            pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
            PXFileOpen(&dataUncompressed, &pxFileOpenInfo);

            break;
        }
        case PXWADEntryDataTypegzip:
        {

#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWADText,
                "Load",
                "gzip"
            );
#endif 

            break;
        }
        case PXWADEntryDataTypeFileRedirection:
        {
#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWADText,
                "Load",
                "File redirect"
            );
#endif 

            DebugBreak();  // How to handle???

            break;
        }
        case PXWADEntryDataTypeZstandard:
        {
#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWADText,
                "Load",
                "ZSTD"
            );
#endif 

            PXFileOpen(&dataUncompressed, &pxFileOpenInfo);

            PXZSTDDecompress(&dataCompressed, &dataUncompressed);

            break;
        }
        case PXWADEntryDataTypeZstandardWithSubchunks:
        {
#if PXLogEnable && 0
            PXLogPrint
            (
                PXLoggingInfo,
                PXWADText,
                "Load",
                "ZSTD+"
            );
#endif 

            break;
        }
        default:
            break;
    }

    if(PXWADEntryDataTypeZstandard != pxWADEntry->DataType)
    {
        return PXActionSuccessful;
    }

    if(1)
    {
        char temp[260];

        PXResourceTransphereInfo pxResourceTransphereInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceTransphereInfo);
        pxResourceTransphereInfo.FileReference = &dataUncompressed;

        // Because the filename is hashed, we cant know the actual filetype from the name.
        // So we need to poke around to find the file. This is very bad.

        PXFileFormatInfoViaContent(&pxResourceTransphereInfo.FormatInfo, &dataUncompressed);
        PXFileTypeInfoProbe(&pxResourceTransphereInfo.FormatInfo, PXNull);

        pxResourceTransphereInfo.FileReference->FilePathData = temp;
        pxResourceTransphereInfo.FileReference->FilePathSize = PXTextPrintA
        (
            temp,
            260,
            "_TEST_DATA_OUTPUT_/WAD_TEMP/%16.16x.%s",
            pxWADEntry->PathHash,
            pxResourceTransphereInfo.FormatInfo.ExtensionText
        );

        PXFileStoreOnDiskA(&dataUncompressed, temp);

        if(pxResourceTransphereInfo.FormatInfo.ResourceLoad)
        {
            pxResourceTransphereInfo.FormatInfo.ResourceLoad(&pxResourceTransphereInfo);
        }
    }

    PXFileClose(&dataUncompressed);
}



PXResult PXAPI  PXWADLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXFile* const pxFile = pxResourceLoadInfo->FileReference;

    PXWAD pxWAD;
    PXClear(PXWAD, &pxWAD);

    PXBool doDumpToFolder = 1;

    PXFileBinding
    (
        pxFile,
        &pxWAD.Header,
        PXWADHeaderList,
        PXWADHeaderListSize,
        PXFileBindingRead
    );


    PXI32U* dataList = 0;
    PXI8U amount = 0;

    switch(pxWAD.Header.VersionMajor)
    {
        case 1:
        {
            dataList = PXWADVersion1List;
            amount = PXWADVersion1ListSize;
            break;
        }
        case 2:
        {
            dataList = PXWADVersion2List;
            amount = PXWADVersion2ListSize;
            break;
        }
        case 3:
        {
            dataList = PXWADVersion3List;
            amount = PXWADVersion3ListSize;
            break;
        }
        default:
            return PXActionFailedFormatNotAsExpected;
    }

    PXFileBinding
    (
        pxFile,
        pxWAD.Data,
        dataList,
        amount,
        PXFileBindingRead
    );



    // Parse entry headers
    PXI32U entryCount = 0;

    switch(pxWAD.Header.VersionMajor)
    {
        case 1:
        {
            entryCount = pxWAD.Version1.EntryCount;
            break;
        }
        case 2:
        {
            entryCount = pxWAD.Version2.EntryCount;
            break;
        }
        case 3:
        {
            entryCount = pxWAD.Version3.EntryCount;
            break;
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWADText,
        "Load",
        "\n"
        "%20s : %i.%i\n"
        "%20s : %i",
        "Version", pxWAD.Header.VersionMajor, pxWAD.Header.VersionMinor,
        "EntryCount", entryCount
    );
#endif

    // Allocate list 
    pxWAD.EntryList = PXMemoryHeapCallocT(PXWADEntry, entryCount);

    for(PXI32U i = 0; i < entryCount; ++i)
    {
        PXWADEntry* const pxWADEntry = &pxWAD.EntryList[i];

        PXFileBinding
        (
            pxFile,
            pxWADEntry,
            PXWADEntryHeaderList,
            PXWADEntryHeaderListSize,
            PXFileBindingRead
        );

#if PXLogEnable && 0
        PXLogPrint
        (
            PXLoggingInfo,
            PXWADText,
            "Load",
            "Entry (%i/%i) : \n"
            "%32s : %16.16x\n"
            "%32s : %i\n"
            "%32s : %i\n"
            "%32s : %i\n"
            "%32s : %i\n"
            "%32s : %i\n"
            "%32s : %i\n"
            "%32s : %i\n"
            "%32s : %16.16x",

            i + 1,
            entryCount,
            "path hash", pxWADEntryHeader.PathHash,
            "data offset in the WAD archive", pxWADEntryHeader.DataOffset,
            "compressed size", pxWADEntryHeader.CompressedSize,
            "uncompressed size", pxWADEntryHeader.UncompressedSize,
            "data type", pxWADEntryHeader.DataType,
            "count of subbchunks", pxWADEntryHeader.CountOfSubbchunks,
            "set for duplicate entries", pxWADEntryHeader.Setforduplicateentries,
            "index of first subbchunk", pxWADEntryHeader.IndexOfFirstSubbchunk,
            "entry checksum", pxWADEntryHeader.EntryChecksum
        );
#endif 
    };



    PXPerformanceInfo pxPerformanceInfo;
    pxPerformanceInfo.UpdateCounter = 0;
    PXPerformanceInfoGet(&pxPerformanceInfo);

    PXLogEnableASYNC();
    PXThreadPoolCreate(PXNull);

    PXI32U* taskListID = 0;

    PXThreadPoolQueuePrepare(PXNull, &taskListID, entryCount);

    for(PXI32U i = 0; i < entryCount; ++i)
    {
        PXWADEntry* const pxWADEntry = &pxWAD.EntryList[i];
        const PXI32U taskID = taskListID[i];

        PXThreadPoolTaskUpdateWork(PXNull, taskID, PXWADEntryHandle, pxWADEntry, pxFile, 0);
    }

    PXThreadPoolWaitForSpesific(PXNull, taskListID, entryCount, PXFalse);

    PXMemoryHeapFree(PXNull, taskListID);

    PXPerformanceInfoGet(&pxPerformanceInfo);


    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXWADSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}