#pragma once

#ifndef PXHardwareInfoIncluded
#define PXHardwareInfoIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXHardwareInfo_
{
    int x;
}
PXHardwareInfo;

PXPublic PXResult PXAPI Draw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif