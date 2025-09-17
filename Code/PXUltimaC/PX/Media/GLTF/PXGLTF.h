#ifndef PXGLTFIncluded
#define PXGLTFIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXGLTFLoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXGLTFSaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif