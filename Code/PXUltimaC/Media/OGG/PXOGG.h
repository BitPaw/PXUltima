#ifndef PXPXOGGINCLUDE
#define PXPXOGGINCLUDE

#include <Media/PXType.h>
#include <OS/Error/PXActionResult.h>
#include <OS/File/PXFile.h>

#include <Media/PXResource.h>

#ifdef __cplusplus
extern "C"
{
#endif

	typedef struct PXOGGPage_
	{
		unsigned char Version; // Often a zero
		unsigned char HeaderType;
		unsigned long long GranulePosition; // 8 Bytes
		unsigned int SerialNumber;
		unsigned int SequenceNumber;
		unsigned int CRC32CheckSum; // CRC32, is generated using a polynomial value of 0x04C11DB7.
		unsigned char PageSegments;
	}
	PXOGGPage;

	typedef enum PXOGGHeaderType_
	{
		PXOGGHeaderInvalid,
		PXOGGHeaderContinuation,
		PXOGGHeaderBeginning,
		PXOGGHeaderEnd
	}
	PXOGGHeaderType;

	typedef struct PXOGG_
	{
		unsigned int __dummy__;
	}
	PXOGG;

	PXPublic PXActionResult PXOGGLoadFromFile(PXSound* const pxSound, PXFile* const pxFile);
	PXPublic PXActionResult PXOGGSaveToFile(PXSound* const pxSound, PXFile* const pxFile);

#ifdef __cplusplus
}
#endif

#endif