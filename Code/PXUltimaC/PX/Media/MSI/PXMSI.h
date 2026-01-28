#pragma once

#ifndef PXMSIIncluded
#define PXMSIIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMSI_
{
    int x;
}
PXMSI;

PXPublic PXResult PXAPI PXMSILoadFromFile(PXECSCreateInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMSISaveToFile(PXECSCreateInfo PXREF pxResourceSaveInfo);

#endif
