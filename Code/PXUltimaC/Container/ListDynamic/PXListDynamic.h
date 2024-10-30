#ifndef PXListDynamicIncluded
#define PXListDynamicIncluded

#include <Media/PXType.h>

#define PXListDynamicSizeObject1Byte 1
#define PXListDynamicSizeObject2Byte 2
#define PXListDynamicSizeObject4Byte 4
#define PXListDynamicSizeObject8Byte 8

// Store a flexible amount of data in a stream of semi-fixed memory
typedef struct PXListDynamic_
{
    char* DataAdress;
    PXSize DataSizeUsed;
    PXSize DataSizeAllocated;

    PXSize KeySize;

    PXSize InsertionPointOffset;

    PXSize EntryAmount;

    PXBool ReallocationAllow;
    PXBool DoNullTerminate;
    PXInt8U SizeInBytes;
}
PXListDynamic;


PXPublic void PXAPI PXListDynamicInit(PXListDynamic* const pxListDynamic, const PXSize keySize, const PXInt8U sizeInBytes);

// Adds an entry, data is copyed given the range.
// a direct adress to the object is returned
PXPublic void* PXAPI PXListDynamicAdd(PXListDynamic* const pxListDynamic, const char* const key,  const char* const data, const PXSize dataSize);
PXPublic void PXAPI PXListDynamicGet(PXListDynamic* const pxListDynamic, const char* const key, char** data, PXSize* dataSize);

#endif