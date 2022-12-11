#ifndef BMPInclude
#define BMPInclude

#include <Format/Type.h>
#include <Error/ActionResult.h>
#include <File/PXDataStream.h>

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef Image_
	typedef struct Image_ Image;
#endif

    typedef enum BMPType_
    {
        BMPInvalid,    
        BMPWindows,					// [BM] Windows 3.1x, 95, NT, ... etc.
        BMPOS2StructBitmapArray,	// [BA] OS/2 struct bitmap array        
        BMPOS2StructColorIcon,		// [CI] OS/2 struct color icon       
        BMPOS2ConstColorPointer,	// [CP] OS/2 const color pointer       
        BMPOS2StructIcon,			// [IC] OS/2 struct icon       
        BMPOS2Pointer				// [PT] OS/2 pointer
    }
	BMPType;

	// DIB header (bitmap information header) - Type / Version
	typedef enum BMPInfoHeaderType_
	{
		UnkownOrInvalid,
		BitMapCoreHeader, 	// [12-Bytes] Windows 2.0 or later		
		OS21XBitMapHeader,  // [12-Bytes] OS/2 1.x		
		OS22XBitMapHeader,  // [16-Bytes] This variant of the previous header contains only the first 16 bytes and the remaining bytes are assumed to be zero values.	
		BitMapInfoHeader, 	// [40-Bytes] Windows NT, 3.1x or later		
		BitMapV2InfoHeader, // [52-Bytes] Undocumented 
		BitMapV3InfoHeader, // [56-Bytes] Not officially documented, but this documentation was posted on Adobe's forums,	
		BitMapV4Header, 	// [108-Bytes] Windows NT 4.0, 95 or later 
		BitMapV5Header 		// [124-Bytes] Windows NT 5.0, 98 or later 
	}
	BMPInfoHeaderType;

	typedef struct BMPInfoHeader_
	{
		//---<Shared>---
		unsigned int HeaderSize; 	// Size of this header, in bytes(40)		

		unsigned short NumberOfBitsPerPixel; // [2-Bytes] number of bits per pixel, which is the color depth of the image.Typical values are 1, 4, 8, 16, 24 and 32.
		unsigned short NumberOfColorPlanes; // [2-Bytes] number of color planes(must be 1)

		int Width; // [4-Bytes] bitmap width in pixels(signed integer)
		int Height; // [4-Bytes] bitmap height in pixels(signed integer)
		//------------

		//---<BitMapInfoHeader ONLY>-------------------------------------------	
		unsigned int CompressionMethod; // [4-Bytes] compression method being used.See the next table for a list of possible values	
		unsigned int ImageSize; 	// [4-Bytes] image size.This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.

		int HorizontalResolution; 	// [4-Bytes] horizontal resolution of the image. (pixel per metre, signed integer)		
		int VerticalResolution; // [4-Bytes] vertical resolution of the image. (pixel per metre, signed integer)		

		unsigned int NumberOfColorsInTheColorPalette; // [4-Bytes] number of colors in the color palette, or 0 to default to 2n 
		unsigned int NumberOfImportantColorsUsed; 	// [4-Bytes] number of important colors used, or 0 when every color is important; generally ignored
		//---------------------------------------------------------------------

		//---<OS22XBitMapHeader ONLY>---
		unsigned short HorizontalandVerticalResolutions; // An enumerated value specifying the units for the horizontaland vertical resolutions(offsets 38 and 42).The only defined value is 0, meaning pixels per metre		
		unsigned short DirectionOfBits; // An enumerated value indicating the direction in which the bits fill the bitmap.The only defined value is 0, meaning the origin is the lower - left corner.Bits fill from left - to - right, then bottom - to - top.
		unsigned short halftoningAlgorithm; // An enumerated value indicating a halftoning algorithm that should be used when rendering the image.
		unsigned int HalftoningParameterA; // Halftoning parameter 1 (see below)
		unsigned int HalftoningParameterB; // Halftoning parameter 2 (see below)
		unsigned int ColorEncoding; // An enumerated value indicating the color encoding for each entry in the color table.The only defined value is 0, indicating RGB.
		unsigned int ApplicationDefinedByte; // 	An application - defined identifier.Not used for image rendering
		//------------------------------
	}
	BMPInfoHeader;
	   
	typedef struct BMP_
	{
		BMPType Type;

		BMPInfoHeaderType InfoHeaderType;
		BMPInfoHeader InfoHeader;

		PXSize PixelDataSize;
		unsigned char* PixelData;
	}
	BMP;

	typedef struct BMPImageDataLayout_
	{
		PXSize ImageSize;
		PXSize RowImageDataSize;
		PXSize RowPaddingSize;
		PXSize RowFullSize;
		PXSize RowAmount;
	}
	BMPImageDataLayout;

	//---<Private Functions>------------------------------------------------------
	PXPrivate BMPType ConvertToBMPType(const unsigned short bmpTypeID);
	PXPrivate unsigned short ConvertFromBMPType(const BMPType headerType);

	PXPrivate BMPInfoHeaderType ConvertToBMPInfoHeaderType(const unsigned int infoHeaderType);
	PXPrivate unsigned int ConvertFromBMPInfoHeaderType(const BMPInfoHeaderType infoHeaderType);
	//----------------------------------------------------------------------------

	//---<Public Functions--------------------------------------------------------
	PXPublic void BMPConstruct(BMP* const bmp);
	PXPublic void BMPDestruct(BMP* const bmp);

	// Calculate information about the layout how the raw image data is stored.
	// There will be "amount of vertical rows", and "pixeldata" + "padding" .
	PXPublic void BMPImageDataLayoutCalculate(BMPImageDataLayout* const bmpImageDataLayout, const PXSize width, const PXSize height, const PXSize bbp);

	//----------------------------------------------------------------------------
	PXPublic PXSize BMPFilePredictSize(const PXSize width, const PXSize height, const PXSize bitsPerPixel);

	PXPublic ActionResult BMPParseToImage(Image* const image, PXDataStream* const dataStream);

	PXPublic ActionResult BMPSerialize(const BMP* const bmp, PXDataStream* const dataStream);
	PXPublic ActionResult BMPSerializeFromImage(const Image* const image, PXDataStream* const dataStream);
	//----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif