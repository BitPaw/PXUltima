#pragma once

#ifndef PXM4VIncluded
#define PXM4VIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXM4VLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXM4VSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif