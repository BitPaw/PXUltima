#pragma once

#ifndef PXButtonIncluded
#define PXButtonIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXProgressbar_
{
    PXWindow* WindowBase;
}
PXProgressbar;

typedef struct PXProgressbarCreateInfo_
{
    PXWindowCreateInfo WindowInfo;
}
PXProgressbarCreateInfo;

PXPublic PXResult PXAPI PXProgressbarDraw(PXProgressbar PXREF pxProgressbar, PXProgressbarCreateInfo PXREF pxProgressbarCreateInfo);
PXPublic PXResult PXAPI PXProgressbarCreate(PXProgressbar** pxProgressbar, PXProgressbarCreateInfo PXREF pxProgressbarCreateInfo);

#endif