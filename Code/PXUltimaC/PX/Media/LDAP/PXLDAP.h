#pragma once

#ifndef PXLDAPIncluded
#define PXLDAPIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXLDAPLoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLDAPSaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif