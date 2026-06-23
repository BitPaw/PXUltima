#pragma once

#ifndef PXDateTimeEditIncluded
#define PXDateTimeEditIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXDateTimeEdit_
{
    PXWindow* WindowBase;
}
PXDateTimeEdit;

typedef struct PXDateTimeEditCreateInfo_
{
    PXWindowCreateInfo WindowInfo;
}
PXDateTimeEditCreateInfo;

PXPublic PXResult PXAPI PXDateTimeEditDraw(PXDateTimeEdit PXREF pxDateTimeEdit, PXDateTimeEditCreateInfo PXREF pxDateTimeEditCreateInfo);
PXPublic PXResult PXAPI PXDateTimeEditCreate(PXDateTimeEdit** pxDateTimeEdit, PXDateTimeEditCreateInfo PXREF pxDateTimeEditCreateInfo);

#endif