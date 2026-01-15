#include "PXFooter.h"
#include <PX/Engine/PXGUI.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Hardware/PXProcessor.h>
#include <gl/GL.h>

#define PXFooterDebug 0

const char PXFooterText[] = "Footer";
const PXI8U PXFooterTextLength = sizeof(PXFooterText);
const PXECSRegisterInfoStatic PXFooterInfoStatic =
{
    {sizeof(PXFooterText), sizeof(PXFooterText), PXFooterText, TextFormatASCII},
    sizeof(PXFooter),
    __alignof(PXFooter),
    PXECSTypeEntity,
    PXFooterCreate,
    PXNull,
    PXFooterDraw
};
PXECSRegisterInfoDynamic PXFooterInfoDynamic;

PXResult PXAPI PXFooterRegisterToECS()
{
    PXECSRegister(&PXFooterInfoStatic, &PXFooterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXFooterCreate(PXFooter** pxFooterREF, PXFooterCreateInfo PXREF pxFooterCreateInfo)
{
    PXFooter* pxFooter = PXNull;

    pxFooterCreateInfo->Info.Static = &PXFooterInfoStatic;
    pxFooterCreateInfo->Info.Dynamic = &PXFooterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxFooterREF, pxFooterCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxFooter = *pxFooterREF;

    pxFooterCreateInfo->Window.EventList.CallBackOwner = pxFooter;
    pxFooterCreateInfo->Window.EventList.CallBackDraw = PXFooterDraw;

    PXWindowCreate(&pxFooter->WindowBase, &pxFooterCreateInfo->Window);

    return PXResultOK;
}

PXResult PXAPI PXFooterDraw(PXFooter PXREF pxFooter, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXWindow PXREF pxWindow = pxFooter->WindowBase;
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



    PXTextDrawInfo pxTextDrawInfo;
    PXClear(PXTextDrawInfo, &pxTextDrawInfo);
    pxTextDrawInfo.WindowDrawInfo = pxWindowDrawInfo;
    pxTextDrawInfo.Text = &pxText;
    pxTextDrawInfo.Behaviour = PXWindowAllignCenter;
    pxTextDrawInfo.Size = 20;
    pxTextDrawInfo.OffsetX = 0.2;

    PXWindowDrawText
    (
        pxWindow,
        &pxTextDrawInfo
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

    return PXResultOK;
}