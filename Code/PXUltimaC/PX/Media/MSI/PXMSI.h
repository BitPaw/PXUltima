#ifndef PXMSIIncluded
#define PXMSIIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXMSI_
{
    int x;
}
PXMSI;

PXPublic PXResult PXAPI PXMSILoadFromFile(PXResourceTransphereInfo* const pxResourceLoadInfo);
PXPublic PXResult PXAPI PXMSISaveToFile(PXResourceTransphereInfo* const pxResourceSaveInfo);

#endif
