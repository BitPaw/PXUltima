#pragma once

#ifndef PXFileDirectoryIncluded
#define PXFileDirectoryIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXFileDirectory_
{
    PXECSInfo Info;

    PXWindow* WindowBase;
}
PXFileDirectory;

typedef struct PXFileDirectoryCreateInfo_
{
    PXECSCreateInfo Info;

    PXWindowCreateInfo Window;
}
PXFileDirectoryCreateInfo;

PXPublic PXResult PXAPI PXFileDirectoryRegisterToECS();

PXPublic PXResult PXAPI PXFileDirectoryDraw(PXFileDirectory PXREF pxFileDirectory, PXWindowDrawInfo PXREF pxWindowDrawInfo);
PXPublic PXResult PXAPI PXFileDirectoryCreate(PXFileDirectory** pxFileDirectory, PXFileDirectoryCreateInfo PXREF pxFileDirectoryCreateInfo);

#endif