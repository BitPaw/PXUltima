#pragma once

#ifndef PXPXTGAIncluded
#define PXPXTGAIncluded

#include <PX/Engine/PXResource.h>

typedef enum PXTGAColorType_
{
    PXTGAColorTypeInvalid,
    PXTGAColorTypeNoColorMap,
    PXTGAColorTypeColorMapPresent,
    PXTGAColorTypeReserverd,
    PXTGAColorTypeCustom
}
PXTGAColorType;

typedef enum PXTGABitsPerPixel_
{
    PXTGABitsPerPixelInvalid,
    PXTGABitsPerPixelX1,
    PXTGABitsPerPixelX8,
    PXTGABitsPerPixelX15,
    PXTGABitsPerPixelX16,
    PXTGABitsPerPixelX24,
    PXTGABitsPerPixelX32
}
PXTGABitsPerPixel;

typedef enum PXTGAImageDataType_
{
    PXTGAImageDataInvalid,
    PXTGAImageDataNoImageDataIsPresent, // 0
    PXTGAImageDataUncompressedColorMapped, // 1
    PXTGAImageDataUncompressedTrueColor, // 2
    PXTGAImageDataUncompressedBlackAndWhite, // 3 (grayscale) image
    PXTGAImageDataRunLengthEncodedColorMapped,  // 9
    PXTGAImageDataRunLengthEncodedTrueColor, // 10
    PXTGAImageDataRunLengthEncodedBlackAndWhite // 11 (grayscale) image
}
PXTGAImageDataType;

typedef struct PXTGA_
{
    //---[Header]----------------------
    PXI8U ColorPaletteType;
    PXTGAImageDataType ImageDataType;

    PXI16U OriginX;
    PXI16U OriginY;
    PXI16U Width;
    PXI16U Height;
    PXTGABitsPerPixel PixelDepth;
    PXI8U ImageDescriptor;
    //---------------------------------

    //---[Image specification]---------
    PXSize ImageInformationSize;
    char ImageInformation[255]; // Optional field containing identifying information
    PXSize ColorMapDataSize;
    unsigned char* ColorMapData; // Look-up table containing color map data
    PXSize ImageDataSize;
    PXByte* ImageData; // Stored according to the image descriptor
    //----------------------------------


    //---- Versiion 2.0 only----------------
    // Extension Area
    char AuthorName[41]; // Name of the author. If not used, bytes should be set to NULL (\0) or spaces
    char AuthorComment[324]; // A comment, organized as four lines, each consisting of 80 characters plus a NULL

    PXI16U DateTimeMonth;// Date and time at which the image was created
    PXI16U JobTimeDay;
    PXI16U JobTimeYear;
    PXI16U JobTimeHour;
    PXI16U JobTimeMinute;
    PXI16U JobTimeSecond;

    char JobID[41];
    PXI16U JobTimeHours; // spent creating the file (for billing, etc.)
    PXI16U JobTimeMinutes;
    PXI16U JobTimeSeconds;
    char SoftwareName[41]; // The application that created the file.
    PXI16U VersionNumber;
    char SoftwareVersion;
    PXI32U BackGroundColor;
    PXI16U PixelAspectRatioCounter;
    PXI16U PixelAspectRatioDenominator;
    PXI16U GammaCounter;
    PXI16U GammaDenominator;

    PXI32U ColorCorrectionOffset; // Number of bytes from the beginning of the file to the color correction table if present
    PXI32U PostagestampOffset; // Number of bytes from the beginning of the file to the postage stamp image if present
    PXI32U ScanlineOffset; // Number of bytes from the beginning of the file to the scan lines table if present
    PXI8U AttributesType; // Specifies the alpha channel
    //-------------------------------
}
PXTGA;

PXPrivate PXTGABitsPerPixel PXAPI ConvertToPixelDepth(const unsigned char pixelDepth);
PXPrivate PXI8U PXAPI ConvertFromPixelDepth(const PXTGABitsPerPixel bitsPerPixel);

PXPrivate PXTGAImageDataType PXAPI ConvertToImageDataType(const unsigned char id);
PXPrivate PXI8U PXAPI ConvertFromImageDataType(const PXTGAImageDataType imageDataType);

PXPublic PXSize PXAPI PXTGAFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

PXPublic void PXAPI PXTGADestruct(PXTGA PXREF tga);

PXPublic PXResult PXAPI PXTGALoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXTGASaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
