#ifndef PXBitmapINCLUDE
#define PXBitmapINCLUDE

#include "../PXResource.h"

typedef enum PXBitmapType_
{
	PXBitmapInvalid,
	PXBitmapWindows,				// [BM] Windows 3.1x, 95, NT, ... etc.
	PXBitmapOS2StructBitmapArray,	// [BA] OS/2 struct bitmap array        
	PXBitmapOS2StructColorIcon,		// [CI] OS/2 struct color icon       
	PXBitmapOS2ConstColorPointer,	// [CP] OS/2 const color pointer       
	PXBitmapOS2StructIcon,			// [IC] OS/2 struct icon       
	PXBitmapOS2Pointer				// [PT] OS/2 pointer
}
PXBitmapType;

// DIB header (bitmap information header) - Type / Version
typedef enum PXBitmapInfoHeaderType_
{
	PXBitmapHeaderUnkownOrInvalid,
	PXBitmapHeaderBitMapCoreHeader, 	// [12-Bytes] Windows 2.0 or later		
	PXBitmapHeaderOS21XBitMapHeader,	// [12-Bytes] OS/2 1.x		
	PXBitmapHeaderOS22XBitMapHeader,	// [16-Bytes] This variant of the previous header contains only the first 16 bytes and the remaining bytes are assumed to be zero values.	
	PXBitmapHeaderBitMapInfoHeader, 	// [40-Bytes] Windows NT, 3.1x or later		
	PXBitmapHeaderBitMapV2InfoHeader,	// [52-Bytes] Undocumented 
	PXBitmapHeaderBitMapV3InfoHeader,	// [56-Bytes] Not officially documented, but this documentation was posted on Adobe's forums,	
	PXBitmapHeaderBitMapV4Header,		// [108-Bytes] Windows NT 4.0, 95 or later 
	PXBitmapHeaderBitMapV5Header 		// [124-Bytes] Windows NT 5.0, 98 or later 
}
PXBitmapInfoHeaderType;

typedef struct PXBitMapInfoHeader_
{
	PXInt32U CompressionMethod; // [4-Bytes] compression method being used.See the next table for a list of possible values	
	PXInt32U ImageSize; // [4-Bytes] image size.This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.

	PXInt32S HorizontalResolution; 	// [4-Bytes] horizontal resolution of the image. (pixel per metre, signed integer)		
	PXInt32S VerticalResolution; // [4-Bytes] vertical resolution of the image. (pixel per metre, signed integer)		

	PXInt32U NumberOfColorsInTheColorPalette; // [4-Bytes] number of colors in the color palette, or 0 to default to 2n 
	PXInt32U NumberOfImportantColorsUsed; 	// [4-Bytes] number of important colors used, or 0 when every color is important; generally ignored
}
PXBitMapInfoHeader;

typedef struct PXOS22XBitMapHeader_
{
	PXInt16U HorizontalandVerticalResolutions; // An enumerated value specifying the units for the horizontaland vertical resolutions(offsets 38 and 42).The only defined value is 0, meaning pixels per metre		
	PXInt16U DirectionOfBits; // An enumerated value indicating the direction in which the bits fill the bitmap.The only defined value is 0, meaning the origin is the lower - left corner.Bits fill from left - to - right, then bottom - to - top.
	PXInt16U halftoningAlgorithm; // An enumerated value indicating a halftoning algorithm that should be used when rendering the image.
	PXInt32U HalftoningParameterA; // Halftoning parameter 1 (see below)
	PXInt32U HalftoningParameterB; // Halftoning parameter 2 (see below)
	PXInt32U ColorEncoding; // An enumerated value indicating the color encoding for each entry in the color table.The only defined value is 0, indicating RGB.
	PXInt32U ApplicationDefinedByte; // 	An application - defined identifier.Not used for image rendering
}
PXOS22XBitMapHeader;

typedef struct PXBitmapHeaderV5_
{
	PXBitMapInfoHeader Header;

	PXInt32U ClrUsed;
	PXInt32U ClrImportant;
	PXInt32U RedMask;
	PXInt32U GreenMask;
	PXInt32U BlueMask;
	PXInt32U AlphaMask;
	PXInt32U CSType;

	PXInt32U EndpointRedX;
	PXInt32U EndpointRedY;
	PXInt32U EndpointRedZ;

	PXInt32U EndpointGreenX;
	PXInt32U EndpointGreenY;
	PXInt32U EndpointGreenZ;

	PXInt32U EndpointBlueX;
	PXInt32U EndpointBlueY;
	PXInt32U EndpointBlueZ;

	PXInt32U GammaRed;
	PXInt32U GammaGreen;
	PXInt32U GammaBlue;
	PXInt32U Intent;
	PXInt32U ProfileData;
	PXInt32U ProfileSize;
}
PXBitmapHeaderV5;

typedef struct PXBitmapInfoHeader_
{
	//---<Shared>---
	PXInt32U HeaderSize; // Size of this header, in bytes(40)

	PXInt16U NumberOfBitsPerPixel; // [2-Bytes] number of bits per pixel, which is the color depth of the image.Typical values are 1, 4, 8, 16, 24 and 32.
	PXInt16U NumberOfColorPlanes; // [2-Bytes] number of color planes(must be 1)

	PXInt32S Width; // [4-Bytes] bitmap width in pixels(signed integer)
	PXInt32S Height; // [4-Bytes] bitmap height in pixels(signed integer)
	//------------

	union
	{
		PXBitMapInfoHeader BitMapInfo;
		PXOS22XBitMapHeader OS22XBitMap;
		PXBitmapHeaderV5 HeaderV5;
	};
}
PXBitmapInfoHeader;

typedef struct PXBitmap_
{
	PXBitmapType Type;

	PXBitmapInfoHeaderType InfoHeaderType;
	PXBitmapInfoHeader InfoHeader;

	PXSize PixelDataSize;
	void* PixelData;

	PXInt32U SizeOfFile;
	PXInt32U ReservedBlock;
	PXInt32U DataOffset;
}
PXBitmap;

typedef struct PXBitmapImageDataLayout_
{
	PXSize ImageSize;
	PXSize RowImageDataSize;
	PXSize RowPaddingSize;
	PXSize RowFullSize;
	PXSize RowAmount;
}
PXBitmapImageDataLayout;

//---<Private Functions>------------------------------------------------------
PXPrivate inline PXBitmapType PXBitmapTypeFromID(const PXInt16U bmpTypeID);
PXPrivate inline PXInt16U PXBitmapTypeToID(const PXBitmapType headerType);

PXPrivate inline PXBitmapInfoHeaderType PXBitmapInfoHeaderTypeFromID(const PXInt8U infoHeaderType);
PXPrivate inline PXInt8U PXBitmapInfoHeaderTypeToID(const PXBitmapInfoHeaderType infoHeaderType);
//----------------------------------------------------------------------------

//---<Public Functions--------------------------------------------------------
PXPublic void PXAPI PXBitmapConstruct(PXBitmap* const bmp);
PXPublic void PXAPI PXBitmapDestruct(PXBitmap* const bmp);

// Calculate information about the layout how the raw image data is stored.
// There will be "amount of vertical rows", and "pixeldata" + "padding" .
PXPrivate void PXBitmapImageDataLayoutCalculate(PXBitmapImageDataLayout* const bmpImageDataLayout, const PXSize width, const PXSize height, const PXSize bbp);

//----------------------------------------------------------------------------
PXPublic PXSize PXAPI PXBitmapFilePredictSize(const PXSize width, const PXSize height, const PXSize bitsPerPixel);

PXPublic PXActionResult PXAPI PXBitmapLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXBitmapSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);
//----------------------------------------------------------------------------

#endif