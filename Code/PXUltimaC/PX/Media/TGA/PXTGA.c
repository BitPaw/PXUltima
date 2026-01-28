#include "PXTGA.h"

#include <PX/OS/File/PXFile.h>
#include <PX/Engine/ECS/PXECS.h>

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

PXI8U PXAPI ConvertFromPixelDepth(const PXTGABitsPerPixel bitsPerPixel)
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

PXI8U PXAPI ConvertFromImageDataType(const PXTGAImageDataType imageDataType)
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

void PXAPI PXTGADestruct(PXTGA PXREF tga)
{

}

PXResult PXAPI PXTGALoadFromFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceLoadInfo)
{
    PXTGA tgaOBJ;
    PXTGA* tga = &tgaOBJ;

    PXClear(PXTGA, tga);

    PXI16U colorPaletteChunkEntryIndex = 0;
    PXI16U colorPaletteChunkSize = 0;
    PXI8U colorPaletteEntrySizeInBits = 0;

    PXSize footerEntryIndex = 0;
    PXI32U extensionOffset = 0;
    PXI32U developerAreaOffset = 0;
    PXSize firstFieldAfterHeader = 0;

    //---[ Parse Header ]-------------------------------
    {
        PXI8U imageIDLengh = 0;
        PXI8U pixelDepth = 0;
        PXI8U imageTypeValue = 0;

        const PXTypeEntry pxDataStreamElementList[] =
        {
            {&imageIDLengh, PXTypeI08U},
            {&tga->ColorPaletteType, PXTypeI08U},
            {&imageTypeValue, PXTypeI08U},
            {&colorPaletteChunkEntryIndex, PXTypeI16ULE},
            {&colorPaletteChunkSize, PXTypeI16ULE},
            {&colorPaletteEntrySizeInBits, PXTypeI08U},
            {&tga->OriginX, PXTypeI16ULE},
            {&tga->OriginY, PXTypeI16ULE},
            {&tga->Width, PXTypeI16ULE},
            {&tga->Height, PXTypeI16ULE},
            {&pixelDepth, PXTypeI08U},
            {&tga->ImageDescriptor, PXTypeI08U}
        };

        PXFileReadMultible(pxResourceLoadInfo->FileCurrent, pxDataStreamElementList, sizeof(pxDataStreamElementList));

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

        PXTextureResize(pxTexture, pxColorFormat, tga->Width, tga->Height);
    }
    //----------------------------------------------------

    //---[Parse Image ID]--------------
    if(tga->ImageInformationSize)
    {
        PXFileReadB(pxResourceLoadInfo->FileCurrent, tga->ImageInformation, tga->ImageInformationSize);
    }
    //----------------------------------

    //---[Parse Color-Palette]----------
    if(colorPaletteChunkSize > 0)
    {
        PXFileCursorAdvance(pxResourceLoadInfo->FileCurrent, colorPaletteChunkSize);
    }
    //--------------------------------

    //---[ ImageData ]------------------
    PXFileReadB(pxResourceLoadInfo->FileCurrent, pxTexture->PixelData.Adress, pxTexture->PixelData.CursorOffsetByte);
    //-----------------------------------------------------------------


    // Check end of pxFile if the pxFile is a Version 2.0 pxFile.
    {
        footerEntryIndex = PXFileDataPosition(pxResourceLoadInfo->FileCurrent) - (26u - 1u);

        const PXBool isPXTGAVersionTwo = PXFileReadAndCompare(pxResourceLoadInfo->FileCurrent, PXTGAFileIdentifier, sizeof(PXTGAFileIdentifier)); // Is this string at this address?;

        if(!isPXTGAVersionTwo) // Is this a PXTGA v.1.0 pxFile?
        {
            return PXResultOK; // Parsing finished. There should be no more data to parse. End of pxFile.
        }
    }

    firstFieldAfterHeader = PXFileDataPosition(pxResourceLoadInfo->FileCurrent);

    //---[ Parse Footer ]--------------------------------------------------------
    PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, footerEntryIndex); // Move 26 Bytes before the end. Start of the PXTGA-Footer.

    PXFileReadI32UE(pxResourceLoadInfo->FileCurrent, &extensionOffset, PXEndianLittle);
    PXFileReadI32UE(pxResourceLoadInfo->FileCurrent, &developerAreaOffset, PXEndianLittle);
    //---------------------------------------------------------------------------

    //---------------------------------------------------------------------------
    if(developerAreaOffset > 0)
    {
        PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, developerAreaOffset); // Jump to Developer Block
        // Parse Developer Fields
        // Parse Developer Directory
    }
    //---------------------------------------------------------------------------

    //---[ Extension Area ]--------------------------------------------------------
    if(extensionOffset > 0)
    {
        PXI16U extensionSize = 0;

        PXFileCursorMoveTo(pxResourceLoadInfo->FileCurrent, extensionOffset); // Jump to Extension Header

        PXFileReadI16UE(pxResourceLoadInfo->FileCurrent, &extensionSize, PXEndianLittle);

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
            {&tga->DateTimeMonth, PXTypeI16ULE},
            {&tga->JobTimeDay, PXTypeI16ULE},
            {&tga->JobTimeYear, PXTypeI16ULE},
            {&tga->JobTimeHour, PXTypeI16ULE},
            {&tga->JobTimeMinute, PXTypeI16ULE},
            {&tga->JobTimeSecond, PXTypeI16ULE},
            {tga->JobID, 41u},

            // 6 Bytes
            {&tga->JobTimeHours, PXTypeI16ULE},
            {&tga->JobTimeMinutes, PXTypeI16ULE},
            {&tga->JobTimeSeconds, PXTypeI16ULE},

            {PXNull, PXTypePadding(12u)},

            {tga->SoftwareName, 41u},
            {&tga->VersionNumber, PXTypeI16ULE},
            {&tga->SoftwareVersion, PXTypeI08U},

            {&tga->BackGroundColor, PXTypeI32ULE},
            {&tga->PixelAspectRatioCounter, PXTypeI16ULE},
            {&tga->PixelAspectRatioDenominator, PXTypeI16ULE},
            {&tga->GammaCounter, PXTypeI16ULE},
            {&tga->GammaDenominator, PXTypeI16ULE},
            {&tga->ColorCorrectionOffset, PXTypeI32ULE},
            {&tga->PostagestampOffset, PXTypeI32ULE},
            {&tga->ScanlineOffset, PXTypeI32ULE},
            {&tga->AttributesType, PXTypeI08U},
        };

        PXFileReadMultible(pxResourceLoadInfo->FileCurrent, pxDataStreamElementList, sizeof(pxDataStreamElementList));
    }

    return PXResultOK;
}

PXResult PXAPI PXTGASaveToFile(PXTexture PXREF pxTexture, PXECSCreateInfo PXREF pxResourceSaveInfo)
{
    return PXActionRefusedNotImplemented;
}