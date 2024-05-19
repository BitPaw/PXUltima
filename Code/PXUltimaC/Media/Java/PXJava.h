#ifndef PXJavaIncluded
#define PXJavaIncluded

#include "../PXResource.h"

typedef struct PXJava_
{
	int Value;
}
PXJava;

PXPublic PXActionResult PXAPI PXJavaLoadFromFile(PXResourceLoadInfo* const pxResourceLoadInfo);
PXPublic PXActionResult PXAPI PXJavaSaveToFile(PXResourceSaveInfo* const pxResourceSaveInfo);

#endif