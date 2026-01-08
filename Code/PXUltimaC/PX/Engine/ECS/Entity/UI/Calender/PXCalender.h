#pragma once

#ifndef PXButtonIncluded
#define PXButtonIncluded

#include <PX/Engine/ECS/Resource/Window/PXWindow.h>

typedef struct PXCalender_
{
    PXWindow* WindowBase;
}
PXCalender;

typedef struct PXCalenderCreateInfo_
{
    PXWindowCreateInfo WindowInfo;
}
PXCalenderCreateInfo;

PXPublic PXResult PXAPI PXCalenderDraw(PXCalender PXREF pxCalender, PXCalenderCreateInfo PXREF pxCalenderCreateInfo);
PXPublic PXResult PXAPI PXCalenderCreate(PXCalender** pxCalender, PXCalenderCreateInfo PXREF pxCalenderCreateInfo);

#endif