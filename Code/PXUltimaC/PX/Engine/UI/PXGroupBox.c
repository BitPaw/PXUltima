#include "PXGroupBox.h"

#if OSUnix
#elif OSWindows
#include <CommCtrl.h>
#endif

PXResult PXAPI PXGroupBoxDraw(PXGroupBox PXREF pxGroupBox, PXGroupBoxCreateInfo PXREF pxGroupBoxCreateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXGroupBoxCreate(PXGroupBox** pxGroupBox, PXGroupBoxCreateInfo PXREF pxGroupBoxCreateInfo)
{
    PXWindowCreateInfo* pxWindowCreateInfo = &pxGroupBoxCreateInfo->WindowInfo;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
    pxWindowCreateInfo->StyleFlags |= BS_GROUPBOX;
#endif

    return PXResultOK;
}
