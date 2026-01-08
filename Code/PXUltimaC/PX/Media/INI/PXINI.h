#pragma once

#ifndef PXINIIncluded
#define PXINIIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXINILoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXINISaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
