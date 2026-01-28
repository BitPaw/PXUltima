#pragma once

#ifndef PXMarkdownIncluded
#define PXMarkdownIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXMarkdownLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMarkdownSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif