#pragma once

#ifndef PXComboBoxIncluded
#define PXComboBoxIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXComboBox_
{
    PXWindow* WindowBase;
}
PXComboBox;

typedef struct PXComboBoxCreateInfo_
{
    PXDrawInfo Info;
}
PXComboBoxCreateInfo;

PXPublic PXResult PXAPI PXComboBoxDraw(PXComboBox PXREF pxComboBox, PXComboBoxCreateInfo PXREF pxComboBoxCreateInfo);
PXPublic PXResult PXAPI PXComboBoxCreate(PXComboBox** pxComboBox, PXComboBoxCreateInfo PXREF pxComboBoxCreateInfo);

#endif