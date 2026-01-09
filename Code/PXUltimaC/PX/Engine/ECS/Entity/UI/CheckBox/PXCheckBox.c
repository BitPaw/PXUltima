#include "PXCheckBox.h"
#include <CommCtrl.h>

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