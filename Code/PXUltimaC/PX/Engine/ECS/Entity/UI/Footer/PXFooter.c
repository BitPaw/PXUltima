#include "PXFooter.h"
#include <PX/Engine/PXGUI.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Hardware/PXProcessor.h>

#define PXFooterDebug 0

const char PXFooterText[] = "Footer";

PXResult PXAPI PXFooterDraw(PXWindow PXREF pxWindow, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = PXGUIThemeGet();

#if PXLogEnable && PXFooterDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXFooterText,
        "Draw",
        "---------- Footer ---------- BEGIN"
    );
#endif

    pxWindowDrawInfo->Brush = pxGUITheme->BrushMainPrimary;

    PXWindowDrawRectangle3D
    (
        pxWindow,
        pxWindowDrawInfo
    );

    PXWindowFontSet(pxWindow, pxGUITheme->FontContent);


#if 1

    PXProcessor pxProcessor;
    PXProcessorFetchInfo(&pxProcessor);

    PXText pxText;
    PXTextFromAdressA(&pxText, pxProcessor.BrandName, pxProcessor.BrandNameSize, pxProcessor.BrandNameSize);

    PXWindowBrushSet
    (
        pxWindow,
        pxGUITheme->BrushMainPrimary,
        PXGUIDrawModeBack
    );
    PXWindowBrushSet
    (
        pxWindow,
        pxGUITheme->BrushTextWhite,
        PXGUIDrawModeFront
    );

    PXWindowDrawText
    (
        pxWindow,
        pxWindowDrawInfo,
        &pxText
    );
#endif

#if PXLogEnable  && PXFooterDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXFooterText,
        "Draw",
        "---------- Footer ---------- END"
    );
#endif

    return PXActionSuccessful;
}
