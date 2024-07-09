#ifndef PXFlexDataCacheIncluded
#define PXFlexDataCacheIncluded

#include <Media/PXType.h>

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
}
PXFlexDataCache;


PXPublic void PXAPI PXFlexDataCacheInit(PXFlexDataCache* const pxFlexDataCache, const PXSize keySize);

PXPublic void PXAPI PXFlexDataCacheAdd(PXFlexDataCache* const pxFlexDataCache, const char* const key,  const char* const data, const PXSize dataSize);

PXPublic void PXAPI PXFlexDataCacheGet(PXFlexDataCache* const pxFlexDataCache, const char* const key, char** data, PXSize* dataSize);

#endif