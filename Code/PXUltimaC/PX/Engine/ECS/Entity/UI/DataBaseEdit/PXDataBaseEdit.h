#pragma once

#ifndef PXDataBaseEditIncluded
#define PXDataBaseEditIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXDataBaseEdit_
{
    int x;
}
PXDataBaseEdit;

PXPublic PXResult PXAPI Draw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo);

#endif