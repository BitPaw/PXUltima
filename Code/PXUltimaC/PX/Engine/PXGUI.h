#pragma once

#ifndef PXGUIIncluded
#define PXGUIIncluded

#include <PX/Engine/PXResource.h>
#include "ECS/Resource/Font/PXFont.h"
#include "ECS/Resource/Brush/PXBrush.h"
#include "ECS/Component/Rectangle/PXRectangle.h"
#include "ECS/Entity/UI/ColorPicker/PXColorPicker.h"

typedef struct PXGUITheme_
{
    PXBrush* BrushMainPrimary;
    PXBrush* BrushMainSecoundary;

    PXBrush* BrushDarkBackground;
    PXBrush* BrushTextWhite;

    PXFont* FontTitle;
    PXFont* FontContent;
}
PXGUITheme;


// Manager to spawn and handle all physical and virtual windows.
// Buttons, Labels and all.
typedef struct PXGUIManager_
{
    PXGUITheme ThemeDefault;
    PXGUITheme* ThemeSelected;

    PXNativDraw NativDraw;
}
PXGUIManager;


// Info about a window property like position or text content
typedef struct PXGUIProperty_
{
    // List of all current windows overall, in this system
    // List of all current windows locally, in this program.

    //PXWindow* WindowCurrent;
    //PXWindow* WindowParent;
    PXUIElementProperty Property;
    //PXWindowCreateInfoData Data;

    union
    {
        PXRectangleXYWHI32 Size;
    };

    PXBool Show;
    PXWindowPropertyUpdateType UpdateType;
}
PXGUIProperty;


// UI Element, button, text and everything else
typedef struct PXGUIElement_
{
    int x;
}
PXGUIElement;





PXPublic PXGUIManager* PXAPI PXGUIInitialize(void);
PXPublic PXResult PXAPI PXGUIShutdown(void);

PXPublic PXGUITheme* PXAPI PXGUIThemeGet(void);


// Change any property of a window. This has to be used for all changes!
PXPublic PXResult PXAPI PXGUIPropertyFetch(PXWindow PXREF pxWindow, PXGUIProperty PXREF pxGUIPropertyList, const PXSize amount, const PXBool doWrite);

//---------------------------------------------------------
// Display/Monitors
//---------------------------------------------------------
#if OSUnix
PXPrivate PXResult PXAPI PXWindowErrorFromXSystem(const int xSysstemErrorID);
#elif PXOSWindowsDestop
PXPrivate void PXAPI PXGUIChildEnumerate(PXGUIManager PXREF pxGUIManager, PXWindow PXREF parent, PXBool visible);
PXPrivate BOOL CALLBACK PXGUIChildEnum(const HWND hwnd, const LPARAM lParam);
#endif
//---------------------------------------------------------


PXPrivate void PXMathCircle(PXColorCircle PXREF pxColorCircle);

// UNSORTERD
PXPublic PXResult PXAPI PXWindowTabListSwapPage(PXWindow PXREF pxWindow);
PXPublic PXBool PXAPI PXWindowValueFetch
(
    PXWindow PXREF pxUIElementList, //
    const PXSize dataListAmount,
    const PXUIElementProperty pxUIElementProperty,
    void PXREF dataList // The given data
);

#endif