#include "PXButton.h"
#include <PX/OS/Console/PXConsole.h>

#include <PX/OS/PXOS.h>
#include <PX/Engine/PXUI.h>

#if OSUnix
#elif OSWindows
#include <CommCtrl.h>
#endif

const char PXButtonName[] = "Button";

PXResult PXAPI PXButtonDraw(PXButton PXREF pxButton, PXDrawInfo PXREF pxDrawInfo)
{
    PXUITheme* pxGUITheme = pxDrawInfo->UITheme;
    PXWindow PXREF pxWindow = pxButton->WindowBase;

#if PXLogEnable && 0
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

    //PXIconGetSystem(SIID_UP, );

    //pxDrawInfo->Brush = pxGUITheme->BrushMainPrimary;
    pxDrawInfo->Color.Red = 0;
    pxDrawInfo->Color.Green = 0xff;
    pxDrawInfo->Color.Blue = 0;
    PXWindowDrawRectangle(pxButton->WindowBase, pxDrawInfo);

    //PXWindowDrawRectangle3D(pxButton, pxDrawInfo);

#if 0
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

    PXWindowFontSet(pxWindow, pxGUITheme->FontContent);

   // pxDrawInfo->Text = PXNull;
    pxDrawInfo->Behaviour |= PXWindowAllignCenter;


#endif

    //PXWindowDrawText(pxWindow, pxDrawInfo);

    return PXResultOK;
}

PXResult PXAPI PXButtonCreate(PXButton** pxButtonREF, PXButtonCreateInfo PXREF pxButtonCreateInfo)
{
    PXWindowCreateInfo PXREF pxWindowCreateInfo = &pxButtonCreateInfo->Window;

    PXButton PXREF pxButton = PXMemoryHeapCallocT(PXButton, 1);

    *pxButtonREF = pxButton;
    pxWindowCreateInfo->EventList.CallBackOwner = pxButton;
    pxWindowCreateInfo->EventList.CallBackDraw = PXButtonDraw;

    // BS_DEFPUSHBUTTON
// BS_USERBUTTON
#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
   // pxWindowCreateInfo->WindowText.A = pxWindowCreateInfo->Button.TextInfo.Content;


    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
    pxWindowCreateInfo->StyleFlags |= BS_AUTORADIOBUTTON;


    pxWindowCreateInfo->WindowClassName.A = WC_BUTTON;
    pxWindowCreateInfo->StyleFlags |= BS_RADIOBUTTON;
#endif

    PXResult pxResult = PXWindowCreate(&pxButton->WindowBase, &pxButtonCreateInfo->Window);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    return PXResultOK;
}
