#pragma once

#ifndef PXDataBaseEditIncluded
#define PXDataBaseEditIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXDataBaseEdit_
{
    int x;
}
PXDataBaseEdit;

PXPublic PXResult PXAPI Draw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo);

#endif