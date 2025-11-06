#pragma once

#ifndef PXLDAPIncluded
#define PXLDAPIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXLDAPLoadFromFile(PXResourceTransphereInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLDAPSaveToFile(PXResourceTransphereInfo PXREF pxResourceSaveInfo);

#endif