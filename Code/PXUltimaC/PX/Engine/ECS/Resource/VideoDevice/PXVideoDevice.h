#pragma once

#ifndef PXVideoIncluded
#define PXVideoIncluded

#include <PX/Engine/PXResource.h>

typedef struct PXVideoDeviceEE_
{
    char Name[32];
}
PXVideoDeviceEE;

PXPublic PXResult PXAPI PXVideoCaptureDeviceList();

#endif