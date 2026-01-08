#include "PXGroupBox.h"
#include <CommCtrl.h>

PXResult PXAPI PXGroupBoxDraw(PXGroupBox PXREF pxGroupBox, PXGroupBoxCreateInfo PXREF pxGroupBoxCreateInfo)
{
    return PXActionSuccessful;
}

PXResult PXAPI PXGroupBoxCreate(PXGroupBox** pxGroupBox, PXGroupBoxCreateInfo PXREF pxGroupBoxCreateInfo)
{
    PXWindowCreateInfo* pxWindowCreateInfo = &pxGroupBoxCreateInfo->WindowInfo;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
    pxWindowCreateInfo->StyleFlags |= BS_GROUPBOX;
#endif

    return PXActionSuccessful;
}