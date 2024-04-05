#ifndef PXOGGINCLUDE
#define PXOGGINCLUDE

#include "../PXResource.h"

typedef enum PXOGGHeaderType_
{
	PXOGGHeaderInvalid,
	PXOGGHeaderContinuation,
	PXOGGHeaderBeginning,
	PXOGGHeaderEnd
}
PXOGGHeaderType;

typedef struct PXOGGPage_
{
	PXInt64U GranulePosition; // 8 Bytes
	PXInt32U SerialNumber;
	PXInt32U SequenceNumber;
	PXInt32U CRC32CheckSum; // CRC32, is generated using a polynomial value of 0x04C11DB7.
	PXInt8U PageSegments;
	PXInt8U HeaderType;
	PXInt8U Version; // Often a zero
}
PXOGGPage;

typedef struct PXOGG_
{
	unsigned int __dummy__;
}
PXOGG;

PXPublic PXActionResult PXAPI PXOGGLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXOGGSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif