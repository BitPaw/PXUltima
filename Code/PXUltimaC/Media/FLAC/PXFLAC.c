#include "PXFLAC.h"

#include <OS/Console/PXConsole.h>
#include <OS/File/PXFile.h>

const char PXFLACSignature[4] = {'f','L','a','C'};

PXFLACBlockType PXAPI PXFLACBlockTypeFromID(const PXInt8U typeID)
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

PXActionResult PXAPI PXFLACLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
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
        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {&pxFLACSteamInfo.SampleSizeMinimum, PXDataTypeInt16ULE},
            {&pxFLACSteamInfo.SampleSizeMaximum, PXDataTypeInt16ULE},
            {&pxFLACSteamInfo.FrameSizeMinimum, PXDataTypeBit32U(24u)},
            {&pxFLACSteamInfo.FrameSizeMaximum, PXDataTypeBit32U(24u)},
            {&pxFLACSteamInfo.SampleRate, PXDataTypeBit32U(20u)},
            {&pxFLACSteamInfo.NumberOfChannels, PXDataTypeBit08U(3u)},
            {&pxFLACSteamInfo.BitsPerSample, PXDataTypeBit08U(5u)},
            {&pxFLACSteamInfo.TotalSamples, PXDataTypeBit64U(36u)},
            {pxFLACSteamInfo.MD5Signature, PXDataTypeDatax16}
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
        PXInt8U typeID;

        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {&pxFLACBlock.IsLastMetaDataBlock, PXDataTypeBit32U(1u)},
            {&typeID, PXDataTypeBit32U(7u)},
            {&pxFLACBlock.BlockLength, PXDataTypeInt32ULE}
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

PXActionResult PXAPI PXFLACSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    PXFLACSteamInfo pxFLACSteamInfo;

    {
        const PXFileDataElementType pxDataStreamElementList[] =
        {
            {PXFLACSignature, sizeof(PXFLACSignature)},
            {&pxFLACSteamInfo.SampleSizeMinimum, PXDataTypeInt16ULE},
            {&pxFLACSteamInfo.SampleSizeMaximum, PXDataTypeInt16ULE},
            {&pxFLACSteamInfo.FrameSizeMinimum, PXDataTypeBit32U(24u)},
            {&pxFLACSteamInfo.FrameSizeMaximum, PXDataTypeBit32U(24u)},
            {&pxFLACSteamInfo.SampleRate, PXDataTypeBit32U(20u)},
            {&pxFLACSteamInfo.NumberOfChannels, PXDataTypeBit08U(3u)},
            {&pxFLACSteamInfo.BitsPerSample, PXDataTypeBit08U(5u)},
            {&pxFLACSteamInfo.TotalSamples, PXDataTypeBit64U(36u)},
            {pxFLACSteamInfo.MD5Signature, PXDataTypeDatax16}
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
