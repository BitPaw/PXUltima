#include "PXFLAC.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/File/PXFile.h>

const char PXFLACSignature[4] = "fLaC";

PXFLACBlockType PXAPI PXFLACBlockTypeFromID(const PXI8U typeID)
{
    switch (typeID)
    {
    case 0:
        return PXFLACBlockTypeSTREAMINFO;
    case 1:
        return PXFLACBlockTypePADDING;
    case 2:
        return PXFLACBlockTypeAPPLICATION;
    case 3:
        return PXFLACBlockTypeSEEKTABLE;
    case 4:
        return PXFLACBlockTypeVORBIS_COMMENT;
    case 5:
        return PXFLACBlockTypeCUESHEET;
    case 6:
        return PXFLACBlockTypePICTURE;
    case 127:
        return PXFLACBlockTypeInvalid;
    default:
        return PXFLACBlockTypeReserved;
    }
}

PXResult PXAPI PXFLACLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo)
{
    PXFLACSteamInfo pxFLACSteamInfo;

    // Check header{
    {
        const PXBool isValidSignature = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXFLACSignature, sizeof(PXFLACSignature));

        if (!isValidSignature)
        {
            return PXActionRefusedInvalidHeaderSignature;
        }
    }

    {
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&pxFLACSteamInfo.SampleSizeMinimum, PXTypeInt16ULE},
            {&pxFLACSteamInfo.SampleSizeMaximum, PXTypeInt16ULE},
            {&pxFLACSteamInfo.FrameSizeMinimum, PXTypeBit32U(24u)},
            {&pxFLACSteamInfo.FrameSizeMaximum, PXTypeBit32U(24u)},
            {&pxFLACSteamInfo.SampleRate, PXTypeBit32U(20u)},
            {&pxFLACSteamInfo.NumberOfChannels, PXTypeBit08U(3u)},
            {&pxFLACSteamInfo.BitsPerSample, PXTypeBit08U(5u)},
            {&pxFLACSteamInfo.TotalSamples, PXTypeBit64U(36u)},
            {pxFLACSteamInfo.MD5Signature, PXTypeDatax16}
        };

        const PXSize readBytes = PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
        const PXBool expectedSize = 34u == readBytes;

        if (!expectedSize)
        {
            return PXActionFailedFormatNotAsExpected;
        }

        PXLogPrint
        (
            PXLoggingInfo,
            "FLAC",
            "Parsing",
            "E"
        );
    }

    {

        PXFLACBlock pxFLACBlock;
        PXI8U typeID;

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&pxFLACBlock.IsLastMetaDataBlock, PXTypeBit32U(1u)},
            {&typeID, PXTypeBit32U(7u)},
            {&pxFLACBlock.BlockLength, PXTypeInt32ULE}
        };

        pxFLACBlock.BlockType = PXFLACBlockTypeFromID(typeID);

        const PXSize readBytes = PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        PXLogPrint
        (
            PXLoggingInfo,
            "FLAC",
            "Parsing",
            "E"
        );
    }


    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXFLACSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo)
{
    PXFLACSteamInfo pxFLACSteamInfo;

    {
        const PXTypeEntry pxDataStreamElementList[] =
        {
            {PXFLACSignature, sizeof(PXFLACSignature)},
            {&pxFLACSteamInfo.SampleSizeMinimum, PXTypeInt16ULE},
            {&pxFLACSteamInfo.SampleSizeMaximum, PXTypeInt16ULE},
            {&pxFLACSteamInfo.FrameSizeMinimum, PXTypeBit32U(24u)},
            {&pxFLACSteamInfo.FrameSizeMaximum, PXTypeBit32U(24u)},
            {&pxFLACSteamInfo.SampleRate, PXTypeBit32U(20u)},
            {&pxFLACSteamInfo.NumberOfChannels, PXTypeBit08U(3u)},
            {&pxFLACSteamInfo.BitsPerSample, PXTypeBit08U(5u)},
            {&pxFLACSteamInfo.TotalSamples, PXTypeBit64U(36u)},
            {pxFLACSteamInfo.MD5Signature, PXTypeDatax16}
        };

        const PXSize readBytes = PXFileWriteMultible(pxResourceSaveInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
        const PXBool expectedSize = 38u == readBytes;

        if (!expectedSize)
        {
            return PXActionFailedFormatNotAsExpected;
        }
    }



    return PXActionRefusedNotImplemented;
}
