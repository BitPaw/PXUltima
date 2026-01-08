#include "PXCalender.h"
#include <ShlObj.h>

PXResult PXAPI PXCalenderDraw(PXCalender PXREF pxCalender, PXCalenderCreateInfo PXREF pxCalenderCreateInfo)
{
    return PXActionSuccessful;
}

PXResult PXAPI PXCalenderCreate(PXCalender** pxCalender, PXCalenderCreateInfo PXREF pxCalenderCreateInfo)
{
#if OSWindows
    pxCalenderCreateInfo->WindowInfo.WindowClassName.A = MONTHCAL_CLASS;
#endif

    return PXActionSuccessful;
}