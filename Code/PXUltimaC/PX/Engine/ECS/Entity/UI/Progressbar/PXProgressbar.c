#include "PXProgressbar.h"
#include <CommCtrl.h>

PXResult PXAPI PXProgressbarDraw(PXProgressbar PXREF pxProgressbar, PXProgressbarCreateInfo PXREF pxProgressbarCreateInfo)
{
    return PXResultOK;
}

PXResult PXAPI PXProgressbarCreate(PXProgressbar** pxProgressbar, PXProgressbarCreateInfo PXREF pxProgressbarCreateInfo)
{
    PXWindowCreateInfo* pxWindowCreateInfo = &pxProgressbarCreateInfo->WindowInfo;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = PROGRESS_CLASS;
#endif

    return PXResultOK;
}