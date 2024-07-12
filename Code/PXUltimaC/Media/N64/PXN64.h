#ifndef PXN64INCLUDE
#define PXN64INCLUDE

#include "../PXResource.h"

typedef enum PXN64CountryCode_
{
	PXN64CountryCodeInvalid,
	PXN64CountryCodeBeta,
	PXN64CountryCodeAsianNTSC,
	PXN64CountryCodeBrazilian,
	PXN64CountryCodeChinese,
	PXN64CountryCodeGerman,
	PXN64CountryCodeNorthAmerica,
	PXN64CountryCodeFrench,
	PXN64CountryCodeGateway64NTSC,
	PXN64CountryCodeDutch,
	PXN64CountryCodeItalian,
	PXN64CountryCodeJapanese,
	PXN64CountryCodeKorean,
	PXN64CountryCodeGateway64PAL,
	PXN64CountryCodeCanadian,
	PXN64CountryCodeEuropeanBasicSpec,
	PXN64CountryCodeSpanish,
	PXN64CountryCodeAustralian,
	PXN64CountryCodeScandinavian,
	PXN64CountryCodeEuropean
}
PXN64CountryCode;

typedef struct PXN64_
{
	PXEndian Endian;

	PXInt8U EndiannessID;
	PXInt8U initialPI_BSB_DOM1_LAT_REG;
	PXInt8U initialPI_BSD_DOM1_PGS_REG;
	PXInt8U initialPI_BSD_DOM1_PWD_REG;
	PXInt8U initialPI_BSB_DOM1_PGS_REG;
	PXInt32U ClockRateOverride;
	PXInt32U ProgramCounter;
	PXInt32U ReleaseAddress;
	PXInt32U CRC1Checksum;
	PXInt32U CRC2;
	PXInt64U UnknownA;
	char ImageName[21];
	PXInt32U UnknownB;
	PXInt32U MediaFormatID;
	char CartridgeID[3];
	PXInt8U CountryCodeID;
	PXN64CountryCode CountryCode;
	PXInt8U Version;
	void* BootCode;
	PXSize BootCodeSize;
}
PXN64;

PXPrivate PXN64CountryCode PXAPI PXN64CountryCodeFromID(PXInt8U pxN64CountryCodeID);
PXPrivate const char* PXN64CountryCodeToString(const PXN64CountryCode pxN64CountryCode);

PXPublic PXActionResult PXAPI PXN64LoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXN64SaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif