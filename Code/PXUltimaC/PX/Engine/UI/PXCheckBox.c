#include "PXCheckBox.h"
#if OSUnix
#elif OSWindows
#include <CommCtrl.h>
#endif

PXResult PXAPI PXCheckBoxDraw(PXCheckBox PXREF pxCheckBox, PXCheckBoxCreateInfo PXREF pxCheckBoxCreateInfo)
{
    PXWindowCreateInfo PXREF pxWindowCreateInfo = &pxCheckBoxCreateInfo->Info;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
    pxWindowCreateInfo->StyleFlags |= BS_CHECKBOX;

    //CheckDlgButton(uiCheckBox->ID, 1, BST_CHECKED); // BST_UNCHECKED
#endif
    
    return PXResultOK;
}