#include "PXWAD.h"

#include <PX/OS/File/PXFile.h>
#include <PX/OS/Console/PXConsole.h>

const char PXWADText[] = "WAD";


//---------------------------------------------------------
typedef struct PXWADVersion1_
{
    PXInt16U EntryHeaderOffset;
    PXInt16U EntryHeaderSize;
    PXInt32U EntryCount;
}
PXWADVersion1;

const PXInt32U PXWADVersion1List[] =
{
    PXTypeInt16U,
    PXTypeInt16U,
    PXTypeInt32U
};
const PXInt8U PXWADVersion1ListSize = sizeof(PXWADVersion1List) / sizeof(PXInt32U);
//---------------------------------------------------------



//---------------------------------------------------------
typedef struct PXWADVersion2_
{
    PXInt8U ECDSASignatureLength;
    char ECDSASignature[83];
    PXInt64U  XXH64Checksum;
    PXInt16U EntryHeaderOffset;
    PXInt16U EntryHeaderSize;
    PXInt32U  EntryCount;
}
PXWADVersion2;

const PXInt32U PXWADVersion2List[] =
{
    PXTypeInt08U,
    PXTypeText(83),
    PXTypeInt64U,
    PXTypeInt16U,
    PXTypeInt16U,
    PXTypeInt32U
};
const PXInt8U PXWADVersion2ListSize = sizeof(PXWADVersion2List) / sizeof(PXInt32U);
//---------------------------------------------------------



//---------------------------------------------------------
typedef struct PXWADVersion3_
{
    char ECDSASignature[256];
    char XXH64Checksum[8];
    PXInt32U EntryCount;
}
PXWADVersion3;

const PXInt32U PXWADVersion3List[] =
{
    PXTypeText(256),
    PXTypeInt64U,
    PXTypeInt32U
};
const PXInt8U PXWADVersion3ListSize = sizeof(PXWADVersion3List) / sizeof(PXInt32U);
//---------------------------------------------------------







//---------------------------------------------------------
typedef struct PXWADEntryHeader_
{
    PXInt64U PathHash;
    PXInt32U DataOffset;
    PXInt32U CompressedSize;
    PXInt32U UncompressedSize;
    PXInt8U DataType;
    PXInt8U CountOfSubbchunks;
    PXInt8U Setforduplicateentries;
    PXInt16U IndexOfFirstSubbchunk;
    PXInt64U EntryChecksum;
}
PXWADEntryHeader;



#define PXWADEntryDataTypeUncompressedData 0
#define PXWADEntryDataTypegzip 1
#define PXWADEntryDataTypeFileRedirection 2
#define PXWADEntryDataTypeZstandard    3
#define PXWADEntryDataTypeZstandardWithSubchunks 4



const PXInt32U PXWADEntryHeaderList[] =
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
const PXInt8U PXWADEntryHeaderListSize = sizeof(PXWADEntryHeaderList) / sizeof(PXInt32U);
//---------------------------------------------------------





typedef struct PXWADHeader_
{
    char MagicCode[2];
    PXInt8U VersionMajor;
    PXInt8U VersionMinor;
}
PXWADHeader;


const PXInt32U PXWADHeaderList[] =
{
    PXTypeDatax2,
    PXTypeInt08U,
    PXTypeInt08U
};
const PXInt8U PXWADHeaderListSize = sizeof(PXWADHeaderList) / sizeof(PXInt32U);



typedef struct PXWAD_
{
    PXWADHeader Header;

    union
    {
        char Data[1];
        PXWADVersion1 Version1;
        PXWADVersion2 Version2;
        PXWADVersion3 Version3;

    };
}
PXWAD;



PXActionResult PXAPI PXWADLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXFile* const pxFile = pxResourceLoadInfo->FileReference;

    PXWAD pxWAD;
    PXClear(PXWAD, &pxWAD);

    PXFileBinding
    (
        pxFile,
        &pxWAD.Header,
        PXWADHeaderList,
        PXWADHeaderListSize,
        PXFileBindingRead
    );


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXWADText,
        "Load",
        "Version: %i.%i",
        pxWAD.Header.VersionMajor,
        pxWAD.Header.VersionMinor
    );
#endif


    PXInt32U* dataList = 0;
    PXInt8U amount = 0;

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
    PXInt32U entryCount = 0;

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
        "EntryCount: %i",
        entryCount
    );
#endif

    for(PXInt32U i = 0; i < entryCount; ++i)
    {
        PXWADEntryHeader pxWADEntryHeader;

        PXFileBinding
        (
            pxFile,
            &pxWADEntryHeader,
            PXWADEntryHeaderList,
            PXWADEntryHeaderListSize,
            PXFileBindingRead
        );

        PXFile decompresed;

        switch(pxWADEntryHeader.DataType)
        {
            case PXWADEntryDataTypeUncompressedData:
            {
                break;
            }
            case PXWADEntryDataTypegzip:
            {
                break;
            }
            case PXWADEntryDataTypeFileRedirection:
            {
                break;
            }
            case PXWADEntryDataTypeZstandard:
            {


                break;
            }
            case PXWADEntryDataTypeZstandardWithSubchunks:
            {
                break;
            }
            default:
                break;
        }


#if PXLogEnable



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
    }


    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXWADSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}