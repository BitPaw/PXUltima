#ifndef PXVideoINCLUDE
#define PXVideoINCLUDE

#include <Media/PXResource.h>

typedef struct PXVideoDeviceEE_
{
    char Name[32];
}
PXVideoDeviceEE;

PXPublic PXActionResult PXAPI PXVideoCaptureDeviceList();

#endif