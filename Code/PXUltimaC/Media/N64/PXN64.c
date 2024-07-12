#include "PXN64.h"

#include <OS/File/PXFile.h>
#include <OS/Console/PXConsole.h>
#include <OS/Error/PXActionResult.h>

PXN64CountryCode PXAPI PXN64CountryCodeFromID(PXInt8U pxN64CountryCodeID)
{
	switch(pxN64CountryCodeID)
	{
		case '7': return PXN64CountryCodeBeta;
		case 'A': return PXN64CountryCodeAsianNTSC;
		case 'B': return PXN64CountryCodeBrazilian;
		case 'C': return PXN64CountryCodeChinese;
		case 'D': return PXN64CountryCodeGerman;
		case 'E': return PXN64CountryCodeNorthAmerica;
		case 'F': return PXN64CountryCodeFrench;
		case 'G': return PXN64CountryCodeGateway64NTSC;
		case 'H': return PXN64CountryCodeDutch;
		case 'I': return PXN64CountryCodeItalian;
		case 'J': return PXN64CountryCodeJapanese;
		case 'K': return PXN64CountryCodeKorean;
		case 'L': return PXN64CountryCodeGateway64PAL;
		case 'N': return PXN64CountryCodeCanadian;
		case 'P': return PXN64CountryCodeEuropeanBasicSpec;
		case 'S': return PXN64CountryCodeSpanish;
		case 'U': return PXN64CountryCodeAustralian;
		case 'W': return PXN64CountryCodeScandinavian;
		case 'X': return PXN64CountryCodeEuropean;
		case 'Y': return PXN64CountryCodeEuropean;

		default:
			return PXN64CountryCodeInvalid;
	}
}

const char* PXN64CountryCodeToString(const PXN64CountryCode pxN64CountryCode)
{
	switch(pxN64CountryCode)
	{
		case PXN64CountryCodeBeta: return "Beta";
		case PXN64CountryCodeAsianNTSC: return "AsianNTSC";
		case PXN64CountryCodeBrazilian: return "Brazilian";
		case PXN64CountryCodeChinese: return "Chinese";
		case PXN64CountryCodeGerman: return "German";
		case PXN64CountryCodeNorthAmerica: return "NorthAmerica";
		case PXN64CountryCodeFrench: return "French";
		case PXN64CountryCodeGateway64NTSC: return "Gateway64NTSC";
		case PXN64CountryCodeDutch: return "Dutch";
		case PXN64CountryCodeItalian: return "Italian";
		case PXN64CountryCodeJapanese: return "Japanese";
		case PXN64CountryCodeKorean: return "Korean";
		case PXN64CountryCodeGateway64PAL: return "Gateway64PAL";
		case PXN64CountryCodeCanadian: return "Canadian";
		case PXN64CountryCodeEuropeanBasicSpec: return "EuropeanBasicSpec";
		case PXN64CountryCodeSpanish:return "Spanish";
		case PXN64CountryCodeAustralian: return "Australian";
		case PXN64CountryCodeScandinavian: return "Scandinavian";
		case PXN64CountryCodeEuropean: return "European";

		default:
			return PXNull;
	}
}

PXActionResult PXAPI PXN64LoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo)
{
	PXFile* pxFile = pxResourceLoadInfo->FileReference;

	PXFile pxN64Data;
	PXClear(PXFile, &pxN64Data);

    PXN64 n64;
    PXClear(PXN64, &n64);

	// Because some formats are byte swapped (middle endian) we need to check for that
	// We will expect a "0x80 37 12 40", if it is not, we need to swap
	{
		const char bufferExpectedByteSwapped[4] = { 0x37, 0x80,0x40 , 0x12 };
		const char bufferExpectedEndianLittle[4] = { 0x40, 0x12,0x37,0x80 };
		const char bufferExpectedEndianBig[4] = {0x80, 0x37, 0x12, 0x40};
		char buffer[4];

		PXFileReadB(pxResourceLoadInfo->FileReference, buffer, 4u);

		if(PXTextCompareA(bufferExpectedByteSwapped, 4, buffer, 4))
		{
			n64.Endian = PXEndianMiddle;
		}
		else if(PXTextCompareA(bufferExpectedEndianLittle, 4, buffer, 4))
		{
			n64.Endian = PXEndianLittle;
		}
		else if(PXTextCompareA(bufferExpectedEndianBig, 4, buffer, 4))
		{
			n64.Endian = PXEndianBig;
		}
		else
		{
			return PXActionRefusedInvalidHeaderSignature;
		}	
	}


	// if byteswapped, so so
	{
		if(PXEndianMiddle == n64.Endian)
		{
			PXFileCursorToBeginning(pxResourceLoadInfo->FileReference);
			PXFileBufferAllocate(&pxN64Data, pxResourceLoadInfo->FileReference->DataAllocated);
			PXFileByteSwap(&pxN64Data, pxResourceLoadInfo->FileReference);

			pxFile = &pxN64Data;
		}
	}


	{
		PXFileDataElementType pxFileDataElementType[] =
		{
			{&n64.EndiannessID, PXDataTypeNibble},
			{PXNull, PXDataTypeNibble},
			{&n64.initialPI_BSB_DOM1_LAT_REG, PXDataTypeNibble},
			{&n64.initialPI_BSD_DOM1_PGS_REG, PXDataTypeNibble},
			{&n64.initialPI_BSD_DOM1_PWD_REG, PXDataTypeInt08U},
			{&n64.initialPI_BSB_DOM1_PGS_REG, PXDataTypeInt08U},
			{&n64.ClockRateOverride, PXDataTypeInt32U},
			{&n64.ProgramCounter, PXDataTypeInt32U},
			{&n64.ReleaseAddress, PXDataTypeInt32U},
			{&n64.CRC1Checksum, PXDataTypeInt32U},
			{&n64.CRC2, PXDataTypeInt32U},
			{&n64.UnknownA, PXDataTypeInt64U},
			{n64.ImageName, PXDataTypeText(20)},
			{&n64.UnknownB, PXDataTypeInt32U},
			{&n64.MediaFormatID, PXDataTypeInt32U},
			{&n64.CartridgeID, PXDataTypeDatax2},
			{&n64.CountryCodeID, PXDataTypeInt08U},
			{&n64.Version, PXDataTypeInt08U}
		};
		const PXSize pxFileDataElementTypeSize = sizeof(pxFileDataElementType);

		PXFileReadMultible(pxFile, pxFileDataElementType, pxFileDataElementTypeSize);

		n64.BootCode = PXFileCursorPosition(pxFile);
		n64.BootCodeSize = 4032;
		n64.CountryCode = PXN64CountryCodeFromID(n64.CountryCodeID);

		// Special behaviour for the string. its not null terminated but with spaces?
		for(size_t i = 19; n64.ImageName[i] == ' '; --i)
		{
			n64.ImageName[i] = '\0';
		}

	}

	PXText sizeText;
	PXTextConstructBufferA(&sizeText, 64);
	PXTextFormatSize(&sizeText, pxN64Data.DataSize);
	
	const char* countryCodeName = PXN64CountryCodeToString(n64.CountryCode);



	PXLogPrint
	(
		PXLoggingInfo,
		"N64",
		"Load",
		"Data\n"
		"%8s : %s\n"
		"%8s : %s\n"
		"%8s : %s",
		"Name", n64.ImageName,
		"Size", sizeText.TextA,
		"Country", countryCodeName
	);


	return PXActionSuccessful;
}

PXActionResult PXAPI PXN64SaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo)
{
	return PXActionRefusedNotImplemented;
}