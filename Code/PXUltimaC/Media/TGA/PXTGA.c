#include "PXTGA.h"

const char PXTGAFileIdentifier[] = {'T','R','U','E','V','I','S','I','O','N',' ','-',' ','X','F','I','L','E','.'};

PXTGABitsPerPixel ConvertToPixelDepth(const unsigned char pixelDepth)
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

unsigned char ConvertFromPixelDepth(const PXTGABitsPerPixel bitsPerPixel)
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

PXTGAImageDataType ConvertToImageDataType(const unsigned char id)
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

unsigned char ConvertFromImageDataType(const PXTGAImageDataType imageDataType)
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

PXSize PXTGAFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
	return 0;
}

void PXTGAConstruct(PXTGA* const tga)
{
	PXClear(PXTGA, tga);
}

void PXTGADestruct(PXTGA* const tga)
{

}

PXActionResult PXTGALoadFromFile(PXImage* const pxImage, PXFile* const pxFile)
{
	PXTGA tgaOBJ;
	PXTGA* tga = &tgaOBJ;

	PXTGAConstruct(tga);

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

		const PXFileDataElementType pxDataStreamElementList[] =
		{
			{PXDataTypeInt8U, &imageIDLengh},
			{PXDataTypeInt8U, &tga->ColorPaletteType},
			{PXDataTypeInt8U, &imageTypeValue},
			{PXDataTypeLEInt16U, &colorPaletteChunkEntryIndex},
			{PXDataTypeLEInt16U, &colorPaletteChunkSize},
			{PXDataTypeInt8U, &colorPaletteEntrySizeInBits},
			{PXDataTypeLEInt16U, &tga->OriginX},
			{PXDataTypeLEInt16U, &tga->OriginY},
			{PXDataTypeLEInt16U, &tga->Width},
			{PXDataTypeLEInt16U, &tga->Height},
			{PXDataTypeInt8U, &pixelDepth},
			{PXDataTypeInt8U, &tga->ImageDescriptor}
		};

		PXFileReadMultible(pxFile, pxDataStreamElementList, sizeof(pxDataStreamElementList));

		tga->ImageInformationSize = imageIDLengh;

		tga->ImageDataType = ConvertToImageDataType(imageTypeValue);
		tga->PixelDepth = ConvertToPixelDepth(pixelDepth);

		tga->ImageDataSize = tga->Width * tga->Height * (pixelDepth / 8u);

		PXColorFormat pxColorFormat;

		switch (tga->ImageDataType)
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
		PXFileReadB(pxFile, tga->ImageInformation, tga->ImageInformationSize);
	}
	//----------------------------------

	//---[Parse Color-Palette]----------
	if(colorPaletteChunkSize > 0)
	{
		PXFileCursorAdvance(pxFile, colorPaletteChunkSize);
	}
	//--------------------------------

	//---[ ImageData ]------------------
	PXFileReadB(pxFile, pxImage->PixelData, pxImage->PixelDataSize);
	//-----------------------------------------------------------------


	// Check end of pxFile if the pxFile is a Version 2.0 pxFile.
	{
		footerEntryIndex = pxFile->DataSize - (26u - 1u);

		const PXBool isPXTGAVersionTwo = PXFileReadAndCompare(pxFile, PXTGAFileIdentifier, sizeof(PXTGAFileIdentifier)); // Is this string at this address?;

		if(!isPXTGAVersionTwo) // Is this a PXTGA v.1.0 pxFile?
		{
			return PXActionSuccessful; // Parsing finished. There should be no more data to parse. End of pxFile.
		}
	}

	firstFieldAfterHeader = pxFile->DataCursor;

	//---[ Parse Footer ]--------------------------------------------------------
	pxFile->DataCursor = footerEntryIndex; // Move 26 Bytes before the end. Start of the PXTGA-Footer.

	PXFileReadI32UE(&pxFile, &extensionOffset, PXEndianLittle);
	PXFileReadI32UE(&pxFile, &developerAreaOffset, PXEndianLittle);
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	if(developerAreaOffset > 0)
	{
		pxFile->DataCursor = developerAreaOffset;// Jump to Developer Block
		// Parse Developer Fields
		// Parse Developer Directory
	}
	//---------------------------------------------------------------------------

	//---[ Extension Area ]--------------------------------------------------------
	if(extensionOffset > 0)
	{
		unsigned short extensionSize = 0;

		pxFile->DataCursor = extensionOffset; // Jump to Extension Header
		PXFileReadI16UE(&pxFile, &extensionSize, PXEndianLittle);

		const unsigned char isExtensionSizeAsExpected = extensionSize == 495u;

		if(!isExtensionSizeAsExpected)
		{
			return PXActionFailedFormatNotAsExpected;
		}

		PXFileReadB(&pxFile, tga->AuthorName, 41u);
		PXFileReadB(&pxFile, tga->AuthorComment, 324u);

		// 12 Bytes
		PXFileReadI16UE(&pxFile, &tga->DateTimeMonth, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->JobTimeDay, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->JobTimeYear, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->JobTimeHour, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->JobTimeMinute, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->JobTimeSecond, PXEndianLittle);

		PXFileReadB(&pxFile, tga->JobID, 41u);

		// 6 Bytes
		PXFileReadI16UE(&pxFile, &tga->JobTimeHours, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->JobTimeMinutes, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->JobTimeSeconds, PXEndianLittle);

		pxFile->DataCursor += 12u;

		PXFileReadB(&pxFile, tga->SoftwareName, 41u);

		PXFileReadI16UE(&pxFile, &tga->VersionNumber, PXEndianLittle);
		PXFileReadI8U(&pxFile, &tga->SoftwareVersion);

		PXFileReadI32UE(&pxFile, &tga->BackGroundColor, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->PixelAspectRatioCounter, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->PixelAspectRatioDenominator, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->GammaCounter, PXEndianLittle);
		PXFileReadI16UE(&pxFile, &tga->GammaDenominator, PXEndianLittle);
		PXFileReadI32UE(&pxFile, &tga->ColorCorrectionOffset, PXEndianLittle);
		PXFileReadI32UE(&pxFile, &tga->PostagestampOffset, PXEndianLittle);
		PXFileReadI32UE(&pxFile, &tga->ScanlineOffset, PXEndianLittle);
		PXFileReadI8U(&pxFile, &tga->AttributesType);

		/*
	if (ColorCorrectionOffset > 0)
	{
		byteSteam.CurrentPosition += ColorCorrectionOffset;
	}

	if (PostagestampOffset > 0)
	{
		byteSteam.CurrentPosition += PostagestampOffset;
	}

	if (ScanlineOffset > 0)
	{
		byteSteam.CurrentPosition += ScanlineOffset;
	}*/
	}
	//-----------------------------------------------------------

	return PXActionSuccessful;
}

PXActionResult PXTGASaveToFile(PXImage* const pxImage, PXFile* const pxFile)
{
	return PXActionRefusedNotImplemented;
}