#include "PXComboBox.h"
#include <CommCtrl.h>

PXResult PXAPI PXComboBoxDraw(PXComboBox PXREF pxComboBox, PXComboBoxCreateInfo PXREF pxComboBoxCreateInfo)
{
    return PXActionSuccessful;
}

PXResult PXAPI PXComboBoxCreate(PXComboBox** pxComboBox, PXComboBoxCreateInfo PXREF pxComboBoxCreateInfo)
{
    PXWindowCreateInfo PXREF pxWindowCreateInfo = &pxComboBoxCreateInfo->Info;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_COMBOBOXEX;
    pxWindowCreateInfo->WindowClassName.A = WC_COMBOBOX;
    pxWindowCreateInfo->StyleFlags |= CBS_HASSTRINGS | CBS_DROPDOWNLIST | WS_OVERLAPPED;
#endif

    return PXActionSuccessful;
}