#pragma once

#ifndef PXMarkdownIncluded
#define PXMarkdownIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXMarkdownLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMarkdownSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif