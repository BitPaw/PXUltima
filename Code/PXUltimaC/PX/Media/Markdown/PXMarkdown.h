#pragma once

#ifndef PXMarkdownIncluded
#define PXMarkdownIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXMarkdownLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMarkdownSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif