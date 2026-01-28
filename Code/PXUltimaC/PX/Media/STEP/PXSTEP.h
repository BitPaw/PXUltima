#pragma once

#ifndef PXSTEPIncluded
#define PXSTEPIncluded

#include <PX/Engine/PXResource.h>

// (St)andard (E)xchange (P)roduct Data (ISO 10303-2xx)
typedef struct PXSTEP_
{
    unsigned int __dummy__;
}
PXSTEP;

PXPublic PXResult PXAPI PXSTEPLoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXSTEPSaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
