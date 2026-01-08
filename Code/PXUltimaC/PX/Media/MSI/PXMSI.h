#pragma once

#ifndef PXMSIIncluded
#define PXMSIIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMSI_
{
    int x;
}
PXMSI;

PXPublic PXResult PXAPI PXMSILoadFromFile(PXResourceMoveInfo PXREF pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMSISaveToFile(PXResourceMoveInfo PXREF pxResourceSaveInfo);

#endif
