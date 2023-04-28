#include "TGA.h"

#include <OS/File/PXFile.h>
#include <OS/Memory/PXMemory.h>

#define TGAFileIdentifier "TRUEVISION-XFILE."
#define TGAFileIdentifierSize 18u

TGABitsPerPixel ConvertToPixelDepth(const unsigned char pixelDepth)
{
	switch(pixelDepth)
	{
		case 1u:
			return TGABitsPerPixelX1;

		case 8u:
			return TGABitsPerPixelX8;

		case 15u:
			return TGABitsPerPixelX15;

		case 16u:
			return TGABitsPerPixelX16;

		case 24u:
			return TGABitsPerPixelX24;

		case 32u:
			return TGABitsPerPixelX32;

		default:
			return TGABitsPerPixelInvalid;
	}
}

unsigned char ConvertFromPixelDepth(const TGABitsPerPixel bitsPerPixel)
{
	switch(bitsPerPixel)
	{
		default:
		case TGABitsPerPixelInvalid:
			return -1;

		case TGABitsPerPixelX1:
			return 1u;

		case TGABitsPerPixelX8:
			return 8u;

		case TGABitsPerPixelX15:
			return 15u;

		case TGABitsPerPixelX16:
			return 16u;

		case TGABitsPerPixelX24:
			return 24u;

		case TGABitsPerPixelX32:
			return 32u;
	}
}

TGAImageDataType ConvertToImageDataType(const unsigned char id)
{
	switch(id)
	{
		case 0u:
			return TGAImageDataNoImageDataIsPresent;

		case 1u:
			return TGAImageDataUncompressedColorMapped;

		case 2u:
			return TGAImageDataUncompressedTrueColor;

		case 3u:
			return TGAImageDataUncompressedBlackAndWhite;

		case 9u:
			return TGAImageDataRunLengthEncodedColorMapped;

		case 10u:
			return TGAImageDataRunLengthEncodedTrueColor;

		case 11u:
			return TGAImageDataRunLengthEncodedBlackAndWhite;

		default:
			return TGAImageDataInvalid;
	}
}

unsigned char ConvertFromImageDataType(const TGAImageDataType imageDataType)
{
	switch(imageDataType)
	{
		default:
		case TGAImageDataInvalid:
			return -1;

		case TGAImageDataNoImageDataIsPresent:
			return 0;

		case TGAImageDataUncompressedColorMapped:
			return 1u;

		case TGAImageDataUncompressedTrueColor:
			return 2u;

		case TGAImageDataUncompressedBlackAndWhite:
			return 3u;

		case TGAImageDataRunLengthEncodedColorMapped:
			return 9u;

		case TGAImageDataRunLengthEncodedTrueColor:
			return 10u;

		case TGAImageDataRunLengthEncodedBlackAndWhite:
			return 11u;
	}
}

PXSize TGAFilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
	return 0;
}

void TGAConstruct(TGA* const tga)
{
	MemoryClear(tga, sizeof(TGA));
}

void TGADestruct(TGA* const tga)
{

}

PXActionResult TGAParse(TGA* tga, const void* data, const PXSize dataSize, PXSize* dataRead)
{
	PXFile dataStream;

	PXFileConstruct(&dataStream);
	PXFileBufferExternal(&dataStream, data, dataSize);
	TGAConstruct(tga);
	*dataRead = 0;

	unsigned short colorPaletteChunkEntryIndex = 0;
	unsigned short colorPaletteChunkSize = 0;
	unsigned char colorPaletteEntrySizeInBits = 0;

	PXSize footerEntryIndex = 0;
	unsigned int extensionOffset = 0;
	unsigned int developerAreaOffset = 0;
	PXSize firstFieldAfterHeader = 0;

	//---[ Parse Header ]-------------------------------
	{
		unsigned char imageIDLengh = 0;
		unsigned char pixelDepth = 0;
		unsigned char imageTypeValue = 0;

		PXFileReadI8U(&dataStream, &imageIDLengh);
		PXFileReadI8U(&dataStream, &tga->ColorPaletteType);
		PXFileReadI8U(&dataStream, &imageTypeValue);

		PXFileReadI16UE(&dataStream, &colorPaletteChunkEntryIndex, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &colorPaletteChunkSize, PXEndianLittle);
		PXFileReadI8U(&dataStream, &colorPaletteEntrySizeInBits);

		PXFileReadI16UE(&dataStream, &tga->OriginX, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->OriginY, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->Width, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->Height, PXEndianLittle);
		PXFileReadI8U(&dataStream, &pixelDepth);
		PXFileReadI8U(&dataStream, &tga->ImageDescriptor);

		tga->ImageInformationSize = imageIDLengh;

		tga->ImageDataType = ConvertToImageDataType(imageTypeValue);
		tga->PixelDepth = ConvertToPixelDepth(pixelDepth);

		tga->ImageDataSize = tga->Width * tga->Height * (pixelDepth / 8u);
		tga->ImageData = MemoryAllocate(sizeof(unsigned char) * tga->ImageDataSize);
	}
	//----------------------------------------------------

	//---[Parse Image ID]--------------
	if(tga->ImageInformationSize)
	{
		PXFileReadB(&dataStream, tga->ImageInformation, tga->ImageInformationSize);
	}
	//----------------------------------

	//---[Parse Color-Palette]----------
	if(colorPaletteChunkSize > 0)
	{
		PXFileCursorAdvance(&dataStream, colorPaletteChunkSize);
	}
	//--------------------------------

	//---[ ImageData ]------------------
	PXFileReadB(&dataStream, tga->ImageData, tga->ImageDataSize);
	//-----------------------------------------------------------------


	// Check end of dataStream if the dataStream is a Version 2.0 dataStream.
	{
		const unsigned int stringLengh = TGAFileIdentifierSize;
		unsigned int compareLength = stringLengh;
		const char lastCharacter = ((char*)dataStream.Data)[dataStream.DataSize - 1];
		const char isLastCharacter = lastCharacter == '.';
		char* string = (char*)dataStream.Data + (dataStream.DataSize - stringLengh);

		if(isLastCharacter)
		{
			compareLength--;
			string++;
		}

		footerEntryIndex = dataStream.DataSize - (26u - 1u);

		const unsigned char isTGAVersionTwo = MemoryCompare(TGAFileIdentifier, TGAFileIdentifierSize, string, compareLength - 1); // Is this string at this address?;

		if(!isTGAVersionTwo) // Is this a TGA v.1.0 dataStream?
		{
			return PXActionSuccessful; // Parsing finished. There should be no more data to parse. End of dataStream.
		}
	}

	firstFieldAfterHeader = dataStream.DataCursor;

	//---[ Parse Footer ]--------------------------------------------------------
	dataStream.DataCursor = footerEntryIndex; // Move 26 Bytes before the end. Start of the TGA-Footer.

	PXFileReadI32UE(&dataStream, &extensionOffset, PXEndianLittle);
	PXFileReadI32UE(&dataStream, &developerAreaOffset, PXEndianLittle);
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	if(developerAreaOffset > 0)
	{
		dataStream.DataCursor = developerAreaOffset;// Jump to Developer Block
		// Parse Developer Fields
		// Parse Developer Directory
	}
	//---------------------------------------------------------------------------

	//---[ Extension Area ]--------------------------------------------------------
	if(extensionOffset > 0)
	{
		unsigned short extensionSize = 0;

		dataStream.DataCursor = extensionOffset; // Jump to Extension Header
		PXFileReadI16UE(&dataStream,extensionSize, PXEndianLittle);

		const unsigned char isExtensionSizeAsExpected = extensionSize == 495u;

		if(!isExtensionSizeAsExpected)
		{
			return PXActionFailedFormatNotAsExpected;
		}

		PXFileReadB(&dataStream, tga->AuthorName, 41u);
		PXFileReadB(&dataStream, tga->AuthorComment, 324u);

		// 12 Bytes
		PXFileReadI16UE(&dataStream, &tga->DateTimeMonth, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->JobTimeDay, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->JobTimeYear, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->JobTimeHour, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->JobTimeMinute, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->JobTimeSecond, PXEndianLittle);

		PXFileReadB(&dataStream, tga->JobID, 41u);

		// 6 Bytes
		PXFileReadI16UE(&dataStream, &tga->JobTimeHours, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->JobTimeMinutes, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->JobTimeSeconds, PXEndianLittle);

		dataStream.DataCursor += 12u;

		PXFileReadB(&dataStream, tga->SoftwareName, 41u);

		PXFileReadI16UE(&dataStream, &tga->VersionNumber, PXEndianLittle);
		PXFileReadI8U(&dataStream, &tga->SoftwareVersion);

		PXFileReadI32UE(&dataStream, &tga->BackGroundColor, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->PixelAspectRatioCounter, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->PixelAspectRatioDenominator, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->GammaCounter, PXEndianLittle);
		PXFileReadI16UE(&dataStream, &tga->GammaDenominator, PXEndianLittle);
		PXFileReadI32UE(&dataStream, tga->ColorCorrectionOffset, PXEndianLittle);
		PXFileReadI32UE(&dataStream, tga->PostagestampOffset, PXEndianLittle);
		PXFileReadI32UE(&dataStream, tga->ScanlineOffset, PXEndianLittle);
		PXFileReadI8U(&dataStream,tga->AttributesType);

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

PXActionResult TGAParseToImage(PXImage* const image, PXFile* const dataStream)
{
	TGA tga;

	TGAConstruct(&tga);

	unsigned short colorPaletteChunkEntryIndex = 0;
	unsigned short colorPaletteChunkSize = 0;
	unsigned char colorPaletteEntrySizeInBits = 0;

	PXSize footerEntryIndex = 0;
	unsigned int extensionOffset = 0;
	unsigned int developerAreaOffset = 0;
	PXSize firstFieldAfterHeader = 0;

	//---[ Parse Header ]-------------------------------
	{
		unsigned char imageIDLengh = 0;
		unsigned char pixelDepth = 0;
		unsigned char imageTypeValue = 0;

		PXFileReadI8U(dataStream, &imageIDLengh);
		PXFileReadI8U(dataStream, &tga.ColorPaletteType);
		PXFileReadI8U(dataStream, &imageTypeValue);

		PXFileReadI16UE(dataStream, &colorPaletteChunkEntryIndex, PXEndianLittle);
		PXFileReadI16UE(dataStream, &colorPaletteChunkSize, PXEndianLittle);
		PXFileReadI8U(dataStream, &colorPaletteEntrySizeInBits);

		PXFileReadI16UE(dataStream, &tga.OriginX, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.OriginY, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.Width, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.Height, PXEndianLittle);
		PXFileReadI8U(dataStream, &pixelDepth);
		PXFileReadI8U(dataStream, &tga.ImageDescriptor);

		tga.ImageInformationSize = imageIDLengh;

		tga.ImageDataType = ConvertToImageDataType(imageTypeValue);
		tga.PixelDepth = ConvertToPixelDepth(pixelDepth);

		const PXSize size = tga.Width * tga.Height * (pixelDepth / 8u);

		tga.ImageDataSize = size;
		tga.ImageData = 0;

		PXImageResize(image, PXColorFormatRGBI8, tga.Width, tga.Height);
	}
	//----------------------------------------------------

	//---[Parse Image ID]--------------
	if(tga.ImageInformationSize)
	{
		PXFileReadB(dataStream, tga.ImageInformation, tga.ImageInformationSize);
	}
	//----------------------------------

	//---[Parse Color-Palette]----------
	if(colorPaletteChunkSize > 0)
	{
		PXFileCursorAdvance(dataStream, colorPaletteChunkSize);
	}
	//--------------------------------

	//---[ ImageData ]------------------
	PXFileReadB(dataStream, image->PixelData, image->PixelDataSize);
	//-----------------------------------------------------------------


	// Check end of dataStream if the dataStream is a Version 2.0 dataStream.
	{
		const unsigned int stringLengh = TGAFileIdentifierSize;
		unsigned int compareLength = stringLengh;
		const char lastCharacter = ((char*)dataStream->Data)[dataStream->DataSize - 1];
		const char isLastCharacter = lastCharacter == '.';
		char* string = (char*)dataStream->Data + (dataStream->DataSize - stringLengh);

		if(isLastCharacter)
		{
			compareLength--;
			string++;
		}

		footerEntryIndex = dataStream->DataSize - (26u - 1u);

		const PXBool isTGAVersionTwo = MemoryCompare(TGAFileIdentifier, TGAFileIdentifierSize, string, compareLength - 1); // Is this string at this address?;

		if(!isTGAVersionTwo) // Is this a TGA v.1.0 dataStream?
		{
			return PXActionSuccessful; // Parsing finished. There should be no more data to parse. End of dataStream.
		}
	}

	firstFieldAfterHeader = dataStream->DataCursor;

	//---[ Parse Footer ]--------------------------------------------------------
	dataStream->DataCursor = footerEntryIndex; // Move 26 Bytes before the end. Start of the TGA-Footer.

	PXFileReadI32UE(dataStream, &extensionOffset, PXEndianLittle);
	PXFileReadI32UE(dataStream, &developerAreaOffset, PXEndianLittle);
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	if(developerAreaOffset > 0)
	{
		dataStream->DataCursor = developerAreaOffset;// Jump to Developer Block
		// Parse Developer Fields
		// Parse Developer Directory
	}
	//---------------------------------------------------------------------------

	//---[ Extension Area ]--------------------------------------------------------
	if(extensionOffset > 0)
	{
		unsigned short extensionSize = 0;

		dataStream->DataCursor = extensionOffset; // Jump to Extension Header
		PXFileReadI16UE(dataStream, extensionSize, PXEndianLittle);

		const PXBool isExtensionSizeAsExpected = extensionSize == 495u;

		if(!isExtensionSizeAsExpected)
		{
			return PXActionFailedFormatNotAsExpected;
		}

		PXFileReadB(dataStream, tga.AuthorName, 41u);
		PXFileReadB(dataStream, tga.AuthorComment, 324u);

		// 12 Bytes
		PXFileReadI16UE(dataStream, &tga.DateTimeMonth, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.JobTimeDay, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.JobTimeYear, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.JobTimeHour, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.JobTimeMinute, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.JobTimeSecond, PXEndianLittle);

		PXFileReadB(dataStream, tga.JobID, 41u);

		// 6 Bytes
		PXFileReadI16UE(dataStream, &tga.JobTimeHours, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.JobTimeMinutes, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.JobTimeSeconds, PXEndianLittle);

		dataStream->DataCursor += 12u;

		PXFileReadB(dataStream, tga.SoftwareName, 41u);

		PXFileReadI16UE(dataStream, &tga.VersionNumber, PXEndianLittle);
		PXFileReadI8U(dataStream, &tga.SoftwareVersion);

		PXFileReadI32UE(dataStream, &tga.BackGroundColor, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.PixelAspectRatioCounter, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.PixelAspectRatioDenominator, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.GammaCounter, PXEndianLittle);
		PXFileReadI16UE(dataStream, &tga.GammaDenominator, PXEndianLittle);
		PXFileReadI32UE(dataStream, tga.ColorCorrectionOffset, PXEndianLittle);
		PXFileReadI32UE(dataStream, tga.PostagestampOffset, PXEndianLittle);
		PXFileReadI32UE(dataStream, tga.ScanlineOffset, PXEndianLittle);
		PXFileReadI8U(dataStream, tga.AttributesType);

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

PXActionResult TGASerializeFromImage(const PXImage* const image, PXFile* const dataStream)
{
	return PXActionInvalid;
}



/*
PXActionResult TGA::Save(const wchar_t* filePath)
{
	const char footer[18] = TGAFileIdentifier;
	unsigned int fileLength = 500;
	File file;// (fileLength);


	// Data Stuff

	file.WriteToDisk(filePath);

	return PXActionSuccessful;
}

PXActionResult TGA::ConvertTo(Image& image)
{
	ImageDataFormat imageFormat = ImageDataFormat::Invalid;
	PXSize pixelDataLengh = 0;
	PXSize bytesPerPixel = 0;
	unsigned char* newImageData = nullptr;

	switch (PixelDepth)
	{
		case TGABitsPerPixel::X1:
		{
			imageFormat = ImageDataFormat::AlphaMaskBinary;
			break;
		}
		case TGABitsPerPixel::X8:
		{
			imageFormat = ImageDataFormat::AlphaMaskBinary;
			bytesPerPixel = 1;
			break;
		}
		case TGABitsPerPixel::X15:
		{
			break;
		}
		case TGABitsPerPixel::X16:
		{
			break;
		}
		case TGABitsPerPixel::X24:
		{
			imageFormat = ImageDataFormat::BGR;
			bytesPerPixel = 3;
			break;
		}
		case TGABitsPerPixel::X32:
		{
			imageFormat = ImageDataFormat::BGRA;
			bytesPerPixel = 4;
			break;
		}
		case TGABitsPerPixel::Invalid:
		{
			return ResultFormatNotSupported;
		}
	}

	pixelDataLengh = Width * Height * bytesPerPixel;
	newImageData = Memory::Allocate<unsigned char>(pixelDataLengh);

	if (!newImageData)
	{
		return ResultOutOfMemory;
	}

	image.Format = imageFormat;
	image.Height = Height;
	image.Width = Width;
	image.PixelData = newImageData;
	image.PixelDataSize = pixelDataLengh;

	MemoryCopy(ImageData, pixelDataLengh, newImageData, pixelDataLengh);
}*/
