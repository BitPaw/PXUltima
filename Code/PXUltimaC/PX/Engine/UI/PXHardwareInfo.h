#pragma once

#ifndef PXHardwareInfoIncluded
#define PXHardwareInfoIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXHardwareInfo_
{
    int x;
}
PXHardwareInfo;

PXPublic PXResult PXAPI Draw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif