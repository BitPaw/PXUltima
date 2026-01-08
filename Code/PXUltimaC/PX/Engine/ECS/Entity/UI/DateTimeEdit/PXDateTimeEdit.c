#include "PXDateTimeEdit.h"
#include <CommCtrl.h>

PXResult PXAPI PXDateTimeEditDraw(PXDateTimeEdit PXREF pxDateTimeEdit, PXDateTimeEditCreateInfo PXREF pxDateTimeEditCreateInfo)
{
    return PXActionSuccessful;
}

PXResult PXAPI PXDateTimeEditCreate(PXDateTimeEdit** pxDateTimeEdit, PXDateTimeEditCreateInfo PXREF pxDateTimeEditCreateInfo)
{
    PXWindowCreateInfo* pxWindowCreateInfo = &pxDateTimeEditCreateInfo->WindowInfo;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = DATETIMEPICK_CLASS;
#endif

    return PXActionSuccessful;
}