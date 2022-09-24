#include "TGA.h"

#include <File/DataStream.h>
#include <Memory/Memory.h>

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

size_t TGAFilePredictSize(const size_t width, const size_t height, const size_t bbp)
{
	return 0;
}

void TGAConstruct(TGA* const tga)
{
	MemorySet(tga, sizeof(TGA), 0);
}

void TGADestruct(TGA* const tga)
{

}

ActionResult TGAParse(TGA* tga, const void* data, const size_t dataSize, size_t* dataRead)
{
	DataStream dataStream;

	DataStreamConstruct(&dataStream);
	DataStreamFromExternal(&dataStream, data, dataSize);
	TGAConstruct(tga);
	*dataRead = 0;

	unsigned short colorPaletteChunkEntryIndex = 0;
	unsigned short colorPaletteChunkSize = 0;
	unsigned char colorPaletteEntrySizeInBits = 0;

	size_t footerEntryIndex = 0;
	unsigned int extensionOffset = 0;
	unsigned int developerAreaOffset = 0;
	size_t firstFieldAfterHeader = 0;

	//---[ Parse Header ]-------------------------------
	{
		unsigned char imageIDLengh = 0;
		unsigned char pixelDepth = 0;
		unsigned char imageTypeValue = 0;

		DataStreamReadCU(&dataStream, &imageIDLengh);
		DataStreamReadCU(&dataStream, &tga->ColorPaletteType);
		DataStreamReadCU(&dataStream, &imageTypeValue);

		DataStreamReadSU(&dataStream, &colorPaletteChunkEntryIndex, EndianLittle);
		DataStreamReadSU(&dataStream, &colorPaletteChunkSize, EndianLittle);
		DataStreamReadCU(&dataStream, &colorPaletteEntrySizeInBits);

		DataStreamReadSU(&dataStream, &tga->OriginX, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->OriginY, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->Width, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->Height, EndianLittle);
		DataStreamReadCU(&dataStream, &pixelDepth);
		DataStreamReadCU(&dataStream, &tga->ImageDescriptor);

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
		DataStreamReadD(&dataStream, tga->ImageInformation, tga->ImageInformationSize);
	}
	//----------------------------------

	//---[Parse Color-Palette]----------
	if(colorPaletteChunkSize > 0)
	{
		DataStreamCursorAdvance(&dataStream, colorPaletteChunkSize);
	}
	//--------------------------------

	//---[ ImageData ]------------------
	DataStreamReadD(&dataStream, tga->ImageData, tga->ImageDataSize);
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
			return ActionSuccessful; // Parsing finished. There should be no more data to parse. End of dataStream.
		}
	}

	firstFieldAfterHeader = dataStream.DataCursor;

	//---[ Parse Footer ]--------------------------------------------------------
	dataStream.DataCursor = footerEntryIndex; // Move 26 Bytes before the end. Start of the TGA-Footer.

	DataStreamReadIU(&dataStream, &extensionOffset, EndianLittle);
	DataStreamReadIU(&dataStream, &developerAreaOffset, EndianLittle);
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
		DataStreamReadSU(&dataStream,extensionSize, EndianLittle);

		const unsigned char isExtensionSizeAsExpected = extensionSize == 495u;

		if(!isExtensionSizeAsExpected)
		{
			return ResultFormatNotAsExpected;
		}

		DataStreamReadD(&dataStream, tga->AuthorName, 41u);
		DataStreamReadD(&dataStream, tga->AuthorComment, 324u);

		// 12 Bytes
		DataStreamReadSU(&dataStream, &tga->DateTimeMonth, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->JobTimeDay, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->JobTimeYear, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->JobTimeHour, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->JobTimeMinute, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->JobTimeSecond, EndianLittle);

		DataStreamReadD(&dataStream, tga->JobID, 41u);

		// 6 Bytes
		DataStreamReadSU(&dataStream, &tga->JobTimeHours, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->JobTimeMinutes, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->JobTimeSeconds, EndianLittle);

		dataStream.DataCursor += 12u;

		DataStreamReadD(&dataStream, tga->SoftwareName, 41u);

		DataStreamReadSU(&dataStream, &tga->VersionNumber, EndianLittle);
		DataStreamReadCU(&dataStream, &tga->SoftwareVersion);

		DataStreamReadIU(&dataStream, &tga->BackGroundColor, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->PixelAspectRatioCounter, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->PixelAspectRatioDenominator, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->GammaCounter, EndianLittle);
		DataStreamReadSU(&dataStream, &tga->GammaDenominator, EndianLittle);
		DataStreamReadIU(&dataStream, tga->ColorCorrectionOffset, EndianLittle);
		DataStreamReadIU(&dataStream, tga->PostagestampOffset, EndianLittle);
		DataStreamReadIU(&dataStream, tga->ScanlineOffset, EndianLittle);
		DataStreamReadCU(&dataStream,tga->AttributesType);

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

	return ActionSuccessful;
}

ActionResult TGAParseToImage(Image* const image, const void* const data, const size_t dataSize, size_t* dataRead)
{
	DataStream DataStream;
	TGA tga;

	DataStreamConstruct(&DataStream);
	DataStreamFromExternal(&DataStream, data, dataSize);
	TGAConstruct(&tga);
	*dataRead = 0;

	unsigned short colorPaletteChunkEntryIndex = 0;
	unsigned short colorPaletteChunkSize = 0;
	unsigned char colorPaletteEntrySizeInBits = 0;

	size_t footerEntryIndex = 0;
	unsigned int extensionOffset = 0;
	unsigned int developerAreaOffset = 0;
	size_t firstFieldAfterHeader = 0;

	//---[ Parse Header ]-------------------------------
	{
		unsigned char imageIDLengh = 0;
		unsigned char pixelDepth = 0;
		unsigned char imageTypeValue = 0;

		DataStreamReadCU(&DataStream, &imageIDLengh);
		DataStreamReadCU(&DataStream, &tga.ColorPaletteType);
		DataStreamReadCU(&DataStream, &imageTypeValue);

		DataStreamReadSU(&DataStream, &colorPaletteChunkEntryIndex, EndianLittle);
		DataStreamReadSU(&DataStream, &colorPaletteChunkSize, EndianLittle);
		DataStreamReadCU(&DataStream, &colorPaletteEntrySizeInBits);

		DataStreamReadSU(&DataStream, &tga.OriginX, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.OriginY, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.Width, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.Height, EndianLittle);
		DataStreamReadCU(&DataStream, &pixelDepth);
		DataStreamReadCU(&DataStream, &tga.ImageDescriptor);

		tga.ImageInformationSize = imageIDLengh;

		tga.ImageDataType = ConvertToImageDataType(imageTypeValue);
		tga.PixelDepth = ConvertToPixelDepth(pixelDepth);

		const size_t size = tga.Width * tga.Height * (pixelDepth / 8u);

		tga.ImageDataSize = size;
		tga.ImageData = 0;

		image->Format = ImageDataFormatRGB8;
		image->Width = tga.Width;
		image->Height = tga.Height;
		image->PixelDataSize = size;
		image->PixelData = MemoryAllocate(sizeof(unsigned char) * size);
	}
	//----------------------------------------------------

	//---[Parse Image ID]--------------
	if(tga.ImageInformationSize)
	{
		DataStreamReadD(&DataStream, tga.ImageInformation, tga.ImageInformationSize);
	}
	//----------------------------------

	//---[Parse Color-Palette]----------
	if(colorPaletteChunkSize > 0)
	{
		DataStreamCursorAdvance(&DataStream, colorPaletteChunkSize);
	}
	//--------------------------------

	//---[ ImageData ]------------------
	DataStreamReadD(&DataStream, image->PixelData, image->PixelDataSize);
	//-----------------------------------------------------------------


	// Check end of dataStream if the dataStream is a Version 2.0 dataStream.
	{
		const unsigned int stringLengh = TGAFileIdentifierSize;
		unsigned int compareLength = stringLengh;
		const char lastCharacter = ((char*)DataStream.Data)[DataStream.DataSize - 1];
		const char isLastCharacter = lastCharacter == '.';
		char* string = (char*)DataStream.Data + (DataStream.DataSize - stringLengh);

		if(isLastCharacter)
		{
			compareLength--;
			string++;
		}

		footerEntryIndex = DataStream.DataSize - (26u - 1u);

		const unsigned char isTGAVersionTwo = MemoryCompare(TGAFileIdentifier, TGAFileIdentifierSize, string, compareLength - 1); // Is this string at this address?;

		if(!isTGAVersionTwo) // Is this a TGA v.1.0 dataStream?
		{
			return ActionSuccessful; // Parsing finished. There should be no more data to parse. End of dataStream.
		}
	}

	firstFieldAfterHeader = DataStream.DataCursor;

	//---[ Parse Footer ]--------------------------------------------------------
	DataStream.DataCursor = footerEntryIndex; // Move 26 Bytes before the end. Start of the TGA-Footer.

	DataStreamReadIU(&DataStream, &extensionOffset, EndianLittle);
	DataStreamReadIU(&DataStream, &developerAreaOffset, EndianLittle);
	//---------------------------------------------------------------------------

	//---------------------------------------------------------------------------
	if(developerAreaOffset > 0)
	{
		DataStream.DataCursor = developerAreaOffset;// Jump to Developer Block
		// Parse Developer Fields
		// Parse Developer Directory
	}
	//---------------------------------------------------------------------------

	//---[ Extension Area ]--------------------------------------------------------
	if(extensionOffset > 0)
	{
		unsigned short extensionSize = 0;

		DataStream.DataCursor = extensionOffset; // Jump to Extension Header
		DataStreamReadSU(&DataStream, extensionSize, EndianLittle);

		const unsigned char isExtensionSizeAsExpected = extensionSize == 495u;

		if(!isExtensionSizeAsExpected)
		{
			return ResultFormatNotAsExpected;
		}

		DataStreamReadD(&DataStream, tga.AuthorName, 41u);
		DataStreamReadD(&DataStream, tga.AuthorComment, 324u);

		// 12 Bytes
		DataStreamReadSU(&DataStream, &tga.DateTimeMonth, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.JobTimeDay, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.JobTimeYear, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.JobTimeHour, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.JobTimeMinute, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.JobTimeSecond, EndianLittle);

		DataStreamReadD(&DataStream, tga.JobID, 41u);

		// 6 Bytes
		DataStreamReadSU(&DataStream, &tga.JobTimeHours, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.JobTimeMinutes, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.JobTimeSeconds, EndianLittle);

		DataStream.DataCursor += 12u;

		DataStreamReadD(&DataStream, tga.SoftwareName, 41u);

		DataStreamReadSU(&DataStream, &tga.VersionNumber, EndianLittle);
		DataStreamReadCU(&DataStream, &tga.SoftwareVersion);

		DataStreamReadIU(&DataStream, &tga.BackGroundColor, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.PixelAspectRatioCounter, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.PixelAspectRatioDenominator, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.GammaCounter, EndianLittle);
		DataStreamReadSU(&DataStream, &tga.GammaDenominator, EndianLittle);
		DataStreamReadIU(&DataStream, tga.ColorCorrectionOffset, EndianLittle);
		DataStreamReadIU(&DataStream, tga.PostagestampOffset, EndianLittle);
		DataStreamReadIU(&DataStream, tga.ScanlineOffset, EndianLittle);
		DataStreamReadCU(&DataStream, tga.AttributesType);

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

	return ActionSuccessful;
}

ActionResult TGASerializeFromImage(const Image* const image, void* data, const size_t dataSize, size_t* dataWritten)
{
	return ActionInvalid;
}



/*
ActionResult TGA::Save(const wchar_t* filePath)
{
	const char footer[18] = TGAFileIdentifier;
	unsigned int fileLength = 500;
	File file;// (fileLength);


	// Data Stuff

	file.WriteToDisk(filePath);

	return ActionSuccessful;
}

ActionResult TGA::ConvertTo(Image& image)
{
	ImageDataFormat imageFormat = ImageDataFormat::Invalid;
	size_t pixelDataLengh = 0;
	size_t bytesPerPixel = 0;
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
