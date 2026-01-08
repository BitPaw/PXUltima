#include "PXGraphBehaviour.h"

#include <PX/OS/Console/PXConsole.h>

const char PXGraphBehaviourText[] = "GraphBehaviour";

PXResult PXAPI PXGraphBehaviourDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    /*
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGraphBehaviourText,
        "Draw",
        "GraphBehaviour\n\n"
    );
#endif

    PXWindowDrawClear(&_pxGUIManager.NativDraw, pxWindow);

    PXGUIDrawRectangle3D
    (
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height
    );

    PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);


    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->MainPrimary,
        PXGUIDrawModeBack
    );
    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->TextWhite,
        PXGUIDrawModeFront
    );

    PXNativDrawA
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height,
        pxWindow->NameContent,
        pxWindow->NameContentSize,
        PXWindowAllignCenter
    );

    */

    return PXActionSuccessful;
}