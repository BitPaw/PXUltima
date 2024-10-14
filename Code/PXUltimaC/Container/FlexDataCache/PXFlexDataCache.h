#ifndef PXFlexDataCacheIncluded
#define PXFlexDataCacheIncluded

#include <Media/PXType.h>

#define PXFlexDataCacheSizeObject1Byte 1
#define PXFlexDataCacheSizeObject2Byte 2
#define PXFlexDataCacheSizeObject4Byte 3
#define PXFlexDataCacheSizeObject8Byte 4

// Store a flexible amount of data in a stream of semi-fixed memory
typedef struct PXFlexDataCache_
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
PXFlexDataCache;


PXPublic void PXAPI PXFlexDataCacheInit(PXFlexDataCache* const pxFlexDataCache, const PXSize keySize, const PXInt8U sizeInBytes);

// Adds an entry, data is copyed given the range.
// a direct adress to the object is returned
PXPublic void* PXAPI PXFlexDataCacheAdd(PXFlexDataCache* const pxFlexDataCache, const char* const key,  const char* const data, const PXSize dataSize);
PXPublic void PXAPI PXFlexDataCacheGet(PXFlexDataCache* const pxFlexDataCache, const char* const key, char** data, PXSize* dataSize);

#endif