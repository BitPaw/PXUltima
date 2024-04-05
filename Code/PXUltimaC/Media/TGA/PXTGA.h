#ifndef PXPXTGAINCLUDE
#define PXPXTGAINCLUDE

#include "../PXResource.h"

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
	PXInt8U ColorPaletteType;
	PXTGAImageDataType ImageDataType;

	PXInt16U OriginX;
	PXInt16U OriginY;
	PXInt16U Width;
	PXInt16U Height;
	PXTGABitsPerPixel PixelDepth;
	PXInt8U ImageDescriptor;
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

	PXInt16U DateTimeMonth;// Date and time at which the image was created 
	PXInt16U JobTimeDay;
	PXInt16U JobTimeYear;
	PXInt16U JobTimeHour;
	PXInt16U JobTimeMinute;
	PXInt16U JobTimeSecond;

	char JobID[41];
	PXInt16U JobTimeHours; // spent creating the file (for billing, etc.) 
	PXInt16U JobTimeMinutes;
	PXInt16U JobTimeSeconds;
	char SoftwareName[41]; // The application that created the file. 
	PXInt16U VersionNumber;
	char SoftwareVersion;
	PXInt32U BackGroundColor;
	PXInt16U PixelAspectRatioCounter;
	PXInt16U PixelAspectRatioDenominator;
	PXInt16U GammaCounter;
	PXInt16U GammaDenominator;

	PXInt32U ColorCorrectionOffset; // Number of bytes from the beginning of the file to the color correction table if present
	PXInt32U PostagestampOffset; // Number of bytes from the beginning of the file to the postage stamp image if present
	PXInt32U ScanlineOffset; // Number of bytes from the beginning of the file to the scan lines table if present 
	PXInt8U AttributesType; // Specifies the alpha channel
	//-------------------------------
}
PXTGA;

PXPrivate PXTGABitsPerPixel PXAPI ConvertToPixelDepth(const unsigned char pixelDepth);
PXPrivate PXInt8U PXAPI ConvertFromPixelDepth(const PXTGABitsPerPixel bitsPerPixel);

PXPrivate PXTGAImageDataType PXAPI ConvertToImageDataType(const unsigned char id);
PXPrivate PXInt8U PXAPI ConvertFromImageDataType(const PXTGAImageDataType imageDataType);

PXPublic PXSize PXAPI PXTGAFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);

PXPublic void PXAPI PXTGADestruct(PXTGA* const tga);

PXPublic PXActionResult PXAPI PXTGALoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXTGASaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif