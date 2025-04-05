#include "PXTGA.h"

#include <PX/OS/File/PXFile.h>

const char PXTGAFileIdentifier[19] = "TRUEVISION - XFILE.";

PXTGABitsPerPixel PXAPI ConvertToPixelDepth(const unsigned char pixelDepth)
{
    switch(pixelDepth)
    {
        case 1u:
            return PXTGABitsPerPixelX1;

        case 8u:
            return PXTGABitsPerPixelX8;

        case 15u:
            return PXTGABitsPerPixelX15;

        case 16u:
            return PXTGABitsPerPixelX16;

        case 24u:
            return PXTGABitsPerPixelX24;

        case 32u:
            return PXTGABitsPerPixelX32;

        default:
            return PXTGABitsPerPixelInvalid;
    }
}

PXInt8U PXAPI ConvertFromPixelDepth(const PXTGABitsPerPixel bitsPerPixel)
{
    switch(bitsPerPixel)
    {
        default:
        case PXTGABitsPerPixelInvalid:
            return -1;

        case PXTGABitsPerPixelX1:
            return 1u;

        case PXTGABitsPerPixelX8:
            return 8u;

        case PXTGABitsPerPixelX15:
            return 15u;

        case PXTGABitsPerPixelX16:
            return 16u;

        case PXTGABitsPerPixelX24:
            return 24u;

        case PXTGABitsPerPixelX32:
            return 32u;
    }
}

PXTGAImageDataType PXAPI ConvertToImageDataType(const unsigned char id)
{
    switch(id)
    {
        case 0u:
            return PXTGAImageDataNoImageDataIsPresent;

        case 1u:
            return PXTGAImageDataUncompressedColorMapped;

        case 2u:
            return PXTGAImageDataUncompressedTrueColor;

        case 3u:
            return PXTGAImageDataUncompressedBlackAndWhite;

        case 9u:
            return PXTGAImageDataRunLengthEncodedColorMapped;

        case 10u:
            return PXTGAImageDataRunLengthEncodedTrueColor;

        case 11u:
            return PXTGAImageDataRunLengthEncodedBlackAndWhite;

        default:
            return PXTGAImageDataInvalid;
    }
}

PXInt8U PXAPI ConvertFromImageDataType(const PXTGAImageDataType imageDataType)
{
    switch(imageDataType)
    {
        default:
        case PXTGAImageDataInvalid:
            return -1;

        case PXTGAImageDataNoImageDataIsPresent:
            return 0;

        case PXTGAImageDataUncompressedColorMapped:
            return 1u;

        case PXTGAImageDataUncompressedTrueColor:
            return 2u;

        case PXTGAImageDataUncompressedBlackAndWhite:
            return 3u;

        case PXTGAImageDataRunLengthEncodedColorMapped:
            return 9u;

        case PXTGAImageDataRunLengthEncodedTrueColor:
            return 10u;

        case PXTGAImageDataRunLengthEncodedBlackAndWhite:
            return 11u;
    }
}

PXSize PXAPI PXTGAFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return 0;
}

void PXAPI PXTGADestruct(PXTGA* const tga)
{

}

PXActionResult PXAPI PXTGALoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo)
{
    PXImage* const pxImage = (PXImage*)pxResourceLoadInfo->ResourceTarget;

    PXTGA tgaOBJ;
    PXTGA* tga = &tgaOBJ;

    PXClear(PXTGA, tga);

    PXInt16U colorPaletteChunkEntryIndex = 0;
    PXInt16U colorPaletteChunkSize = 0;
    PXInt8U colorPaletteEntrySizeInBits = 0;

    PXSize footerEntryIndex = 0;
    PXInt32U extensionOffset = 0;
    PXInt32U developerAreaOffset = 0;
    PXSize firstFieldAfterHeader = 0;

    //---[ Parse Header ]-------------------------------
    {
        PXInt8U imageIDLengh = 0;
        PXInt8U pixelDepth = 0;
        PXInt8U imageTypeValue = 0;

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&imageIDLengh, PXTypeInt08U},
            {&tga->ColorPaletteType, PXTypeInt08U},
            {&imageTypeValue, PXTypeInt08U},
            {&colorPaletteChunkEntryIndex, PXTypeInt16ULE},
            {&colorPaletteChunkSize, PXTypeInt16ULE},
            {&colorPaletteEntrySizeInBits, PXTypeInt08U},
            {&tga->OriginX, PXTypeInt16ULE},
            {&tga->OriginY, PXTypeInt16ULE},
            {&tga->Width, PXTypeInt16ULE},
            {&tga->Height, PXTypeInt16ULE},
            {&pixelDepth, PXTypeInt08U},
            {&tga->ImageDescriptor, PXTypeInt08U}
        };

        PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));

        tga->ImageInformationSize = imageIDLengh;

        tga->ImageDataType = ConvertToImageDataType(imageTypeValue);
        tga->PixelDepth = ConvertToPixelDepth(pixelDepth);

        tga->ImageDataSize = tga->Width * tga->Height * (pixelDepth / 8u);

        PXColorFormat pxColorFormat;

        switch(tga->ImageDataType)
        {
            case PXTGAImageDataUncompressedTrueColor: // RGB
            case PXTGAImageDataRunLengthEncodedTrueColor:
            {
                pxColorFormat = PXColorFormatRGBI8;
                break;
            }
            case PXTGAImageDataUncompressedBlackAndWhite:
            {
                pxColorFormat = PXColorFormatAlphaBinary;
                break;
            }
            case PXTGAImageDataNoImageDataIsPresent:
            case PXTGAImageDataUncompressedColorMapped:
            case PXTGAImageDataRunLengthEncodedColorMapped:
            case PXTGAImageDataRunLengthEncodedBlackAndWhite:
            default:
                pxColorFormat = PXColorFormatInvalid;
                break;
        }

        PXImageResize(pxImage, pxColorFormat, tga->Width, tga->Height);
    }
    //----------------------------------------------------

    //---[Parse Image ID]--------------
    if(tga->ImageInformationSize)
    {
        PXFileReadB(pxResourceLoadInfo->FileReference, tga->ImageInformation, tga->ImageInformationSize);
    }
    //----------------------------------

    //---[Parse Color-Palette]----------
    if(colorPaletteChunkSize > 0)
    {
        PXFileCursorAdvance(pxResourceLoadInfo->FileReference, colorPaletteChunkSize);
    }
    //--------------------------------

    //---[ ImageData ]------------------
    PXFileReadB(pxResourceLoadInfo->FileReference, pxImage->PixelData, pxImage->PixelDataSize);
    //-----------------------------------------------------------------


    // Check end of pxFile if the pxFile is a Version 2.0 pxFile.
    {
        footerEntryIndex = pxResourceLoadInfo->FileReference->DataUsed - (26u - 1u);

        const PXBool isPXTGAVersionTwo = PXFileReadAndCompare(pxResourceLoadInfo->FileReference, PXTGAFileIdentifier, sizeof(PXTGAFileIdentifier)); // Is this string at this address?;

        if(!isPXTGAVersionTwo) // Is this a PXTGA v.1.0 pxFile?
        {
            return PXActionSuccessful; // Parsing finished. There should be no more data to parse. End of pxFile.
        }
    }

    firstFieldAfterHeader = pxResourceLoadInfo->FileReference->DataCursor;

    //---[ Parse Footer ]--------------------------------------------------------
    pxResourceLoadInfo->FileReference->DataCursor = footerEntryIndex; // Move 26 Bytes before the end. Start of the PXTGA-Footer.

    PXFileReadI32UE(pxResourceLoadInfo->FileReference, &extensionOffset, PXEndianLittle);
    PXFileReadI32UE(pxResourceLoadInfo->FileReference, &developerAreaOffset, PXEndianLittle);
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    if(developerAreaOffset > 0)
    {
        pxResourceLoadInfo->FileReference->DataCursor = developerAreaOffset;// Jump to Developer Block
        // Parse Developer Fields
        // Parse Developer Directory
    }
    //---------------------------------------------------------------------------

    //---[ Extension Area ]--------------------------------------------------------
    if(extensionOffset > 0)
    {
        PXInt16U extensionSize = 0;

        pxResourceLoadInfo->FileReference->DataCursor = extensionOffset; // Jump to Extension Header
        PXFileReadI16UE(pxResourceLoadInfo->FileReference, &extensionSize, PXEndianLittle);

        const PXBool isExtensionSizeAsExpected = extensionSize == 495u;

        if(!isExtensionSizeAsExpected)
        {
            return PXActionFailedFormatNotAsExpected;
        }

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {tga->AuthorName, 41u},
            {tga->AuthorComment, 324u},

            // 12 Bytes
            {&tga->DateTimeMonth, PXTypeInt16ULE},
            {&tga->JobTimeDay, PXTypeInt16ULE},
            {&tga->JobTimeYear, PXTypeInt16ULE},
            {&tga->JobTimeHour, PXTypeInt16ULE},
            {&tga->JobTimeMinute, PXTypeInt16ULE},
            {&tga->JobTimeSecond, PXTypeInt16ULE},
            {tga->JobID, 41u},

            // 6 Bytes
            {&tga->JobTimeHours, PXTypeInt16ULE},
            {&tga->JobTimeMinutes, PXTypeInt16ULE},
            {&tga->JobTimeSeconds, PXTypeInt16ULE},

            {PXNull, PXTypePadding(12u)},

            {tga->SoftwareName, 41u},
            {&tga->VersionNumber, PXTypeInt16ULE},
            {&tga->SoftwareVersion, PXTypeInt08U},

            {&tga->BackGroundColor, PXTypeInt32ULE},
            {&tga->PixelAspectRatioCounter, PXTypeInt16ULE},
            {&tga->PixelAspectRatioDenominator, PXTypeInt16ULE},
            {&tga->GammaCounter, PXTypeInt16ULE},
            {&tga->GammaDenominator, PXTypeInt16ULE},
            {&tga->ColorCorrectionOffset, PXTypeInt32ULE},
            {&tga->PostagestampOffset, PXTypeInt32ULE},
            {&tga->ScanlineOffset, PXTypeInt32ULE},
            {&tga->AttributesType, PXTypeInt08U},
        };

        PXFileReadMultible(pxResourceLoadInfo->FileReference, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXTGASaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}