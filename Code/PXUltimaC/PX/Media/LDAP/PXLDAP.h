#pragma once

#ifndef PXLDAPIncluded
#define PXLDAPIncluded

#include <PX/Engine/PXResource.h>

PXPublic PXResult PXAPI PXLDAPLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXLDAPSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif