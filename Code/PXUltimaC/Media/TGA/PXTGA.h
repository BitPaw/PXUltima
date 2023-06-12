#ifndef PXPXTGAINCLUDE
#define PXPXTGAINCLUDE

#include <Media/PXImage.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef enum PXTGAColorType_
	{
		NoColorMap,
		ColorMapPresent,
		Reserverd,
		Custom
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
		unsigned char ColorPaletteType;
		PXTGAImageDataType ImageDataType;

		unsigned short OriginX;
		unsigned short OriginY;
		unsigned short Width;
		unsigned short Height;
		PXTGABitsPerPixel PixelDepth;
		unsigned char ImageDescriptor;
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

		unsigned short DateTimeMonth;// Date and time at which the image was created 
		unsigned short JobTimeDay;
		unsigned short JobTimeYear;
		unsigned short JobTimeHour;
		unsigned short JobTimeMinute;
		unsigned short JobTimeSecond;

		char JobID[41];
		unsigned short JobTimeHours; // spent creating the file (for billing, etc.) 
		unsigned short JobTimeMinutes;
		unsigned short JobTimeSeconds;
		char SoftwareName[41]; // The application that created the file. 
		unsigned short VersionNumber;
		char SoftwareVersion;
		unsigned int BackGroundColor;
		unsigned short PixelAspectRatioCounter;
		unsigned short PixelAspectRatioDenominator;
		unsigned short GammaCounter;
		unsigned short GammaDenominator;

		unsigned int ColorCorrectionOffset; // Number of bytes from the beginning of the file to the color correction table if present
		unsigned int PostagestampOffset; // Number of bytes from the beginning of the file to the postage stamp image if present
		unsigned int ScanlineOffset; // Number of bytes from the beginning of the file to the scan lines table if present 
		unsigned char AttributesType; // Specifies the alpha channel
		//-------------------------------
	}
	PXTGA;

	PXPrivate PXTGABitsPerPixel ConvertToPixelDepth(const unsigned char pixelDepth);
	PXPrivate unsigned char ConvertFromPixelDepth(const PXTGABitsPerPixel bitsPerPixel);

	PXPrivate PXTGAImageDataType ConvertToImageDataType(const unsigned char id);
	PXPrivate unsigned char ConvertFromImageDataType(const PXTGAImageDataType imageDataType);


	PXPublic PXSize PXTGAFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp);


	PXPublic void PXTGAConstruct(PXTGA* const tga); 
	PXPublic void PXTGADestruct(PXTGA* const tga);


	PXPublic PXActionResult PXTGAParse(PXTGA* const tga, PXFile* const pxFile);
	PXPublic PXActionResult PXTGAParseToImage(PXImage* const image, PXFile* const dataStream);

	PXPublic PXActionResult PXTGASerializeFromImage(const PXImage* const image, PXFile* const dataStream);

#ifdef __cplusplus
}
#endif

#endif