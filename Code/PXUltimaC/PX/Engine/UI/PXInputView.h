#pragma once

#ifndef PXInputViewIncluded
#define PXInputViewIncluded

#include <PX/Type/PXWindow.h>

typedef struct PXInputView_
{
    int x;
}
PXInputView;

PXPrivate PXResult PXAPI PXInputViewDraw(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo);

#endif