#pragma once

#ifndef PXListDynamicIncludedd
#define PXListDynamicIncludedd

#include <PX/Media/PXType.h>
#include <PX/Container/Buffer/PXBuffer.h>

#define PXListDynamicSizeObject1Byte 1
#define PXListDynamicSizeObject2Byte 2
#define PXListDynamicSizeObject4Byte 4
#define PXListDynamicSizeObject8Byte 8

// Store a flexible amount of data in a stream of semi-fixed memory
typedef struct PXListDynamic_
{
    // Data
    PXBuffer Buffer;

    // Primary element to store
    PXSize KeySize;

    PXSize InsertionPointOffset;

    PXSize EntryAmount;

    PXBool ReallocationAllow;

    PXI8U NullPaddingAmount; // If 1=ASCII NullTerminate, 2=UNICODE NullTerminate

    // Size of data/payload
    PXI8U SizeInBytesOfLengthKey;
}
PXListDynamic;


PXPublic void PXAPI PXListDynamicInit(PXListDynamic PXREF pxListDynamic, const PXSize keySize, const PXI8U sizeInBytes);

PXPublic void* PXAPI PXListDynamicInsertFind(PXListDynamic PXREF pxListDynamic);

// Adds an entry, data is copyed given the range.
// a direct adress to the object is returned
PXPublic void* PXAPI PXListDynamicAdd(PXListDynamic PXREF pxListDynamic, const void PXREF key,  const char PXREF data, const PXSize dataSize);
PXPublic void PXAPI PXListDynamicGet(PXListDynamic PXREF pxListDynamic, const void PXREF key, char** data, PXSize* dataSize);

PXPublic void PXAPI PXListDynamicClearAll(PXListDynamic PXREF pxListDynamic);

#endif