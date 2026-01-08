#pragma once

#ifndef PXFileDirectoryIncluded
#define PXFileDirectoryIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXFileDirectory_
{
    PXWindow* WindowBase;
}
PXFileDirectory;

PXPublic PXResult PXAPI PXFileDirectoryDraw(PXFileDirectory PXREF pxFileDirectory, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPublic PXResult PXAPI PXFileDirectoryCreate(PXFileDirectory** pxFileDirectory, PXWindowCreateInfo PXREF pxWindowCreateInfo);

#endif