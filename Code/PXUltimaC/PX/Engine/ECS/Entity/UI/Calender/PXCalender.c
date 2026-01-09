#include "PXCalender.h"
#include <ShlObj.h>

PXResult PXAPI PXCalenderDraw(PXCalender PXREF pxCalender, PXCalenderCreateInfo PXREF pxCalenderCreateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXCalenderCreate(PXCalender** pxCalender, PXCalenderCreateInfo PXREF pxCalenderCreateInfo)
{
#if OSWindows
    pxCalenderCreateInfo->WindowInfo.WindowClassName.A = MONTHCAL_CLASS;
#endif

    return PXResultOK;
}