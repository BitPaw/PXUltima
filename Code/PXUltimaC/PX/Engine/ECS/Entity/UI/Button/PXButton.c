#include "PXButton.h"
#include <PX/OS/Console/PXConsole.h>
#include <CommCtrl.h>
#include <PX/Engine/PXGUI.h>
#include <PX/OS/PXOS.h>

const char PXButtonName[] = "Button";

PXResult PXAPI PXButtonDraw(PXButton PXREF pxButton, PXDrawInfo PXREF pxDrawInfo)
{
    PXGUITheme* pxGUITheme = PXGUIThemeGet();

#if PXLogEnable
    PXECSInfo* pxECSInfo = pxButton;

    PXLogPrint
    (
        PXLoggingInfo,
        PXButtonName,
        "Draw",
        "PXID:<%4i>",
        pxECSInfo->ID
    );
#endif

    pxDrawInfo->Brush = pxGUITheme->BrushMainPrimary;
 
    PXWindowDrawRectangle3D(pxButton, pxDrawInfo);

#if 1
    PXWindowBrushSet
    (
        pxButton,
        pxGUITheme->BrushMainPrimary,
        PXGUIDrawModeBack
    );
    PXWindowBrushSet
    (
        pxButton,
        pxGUITheme->BrushTextWhite,
        PXGUIDrawModeFront
    );

    PXWindowFontSet(pxButton, pxGUITheme->FontContent);

    PXTextDrawInfo pxTextDrawInfo;
    PXClear(PXTextDrawInfo, &pxTextDrawInfo);
    pxTextDrawInfo.WindowDrawInfo = pxDrawInfo;
    pxTextDrawInfo.Text = PXNull;
    pxTextDrawInfo.Behaviour = PXWindowAllignCenter;

    PXWindowDrawText(pxButton, &pxTextDrawInfo);
#endif

    return PXResultOK;
}

PXResult PXAPI PXButtonCreate(PXButton** pxButtonREF, PXButtonCreateInfo PXREF pxButtonCreateInfo)
{
    PXWindowCreateInfo* pxWindowCreateInfo = &pxButtonCreateInfo->WindowInfo;

    PXButton PXREF pxButton = PXMemoryHeapCallocT(PXButton, 1);

    *pxButtonREF = pxButton;

    // BS_DEFPUSHBUTTON
// BS_USERBUTTON
#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
   // pxWindowCreateInfo->WindowText.A = pxWindowCreateInfo->Button.TextInfo.Content;
#endif
    pxWindowCreateInfo->EventList.CallBackDraw = PXButtonDraw;

    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
    pxWindowCreateInfo->StyleFlags |= BS_AUTORADIOBUTTON;

#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
    pxWindowCreateInfo->StyleFlags |= BS_RADIOBUTTON;
#endif

   // PXWindowCreate(, );

    return PXResultOK;
}