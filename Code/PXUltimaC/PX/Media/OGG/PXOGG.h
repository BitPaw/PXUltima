#pragma once

#ifndef PXOGGIncluded
#define PXOGGIncluded

#include <PX/Engine/PXResource.h>

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
    PXI64U GranulePosition; // 8 Bytes
    PXI32U SerialNumber;
    PXI32U SequenceNumber;
    PXI32U CRC32CheckSum; // CRC32, is generated using a polynomial value of 0x04C11DB7.
    PXI8U PageSegments;
    PXI8U HeaderType;
    PXI8U Version; // Often a zero
}
PXOGGPage;

typedef struct PXOGG_
{
    unsigned int __dummy__;
}
PXOGG;

PXPublic PXResult PXAPI PXOGGLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXOGGSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
