#include "PXFooter.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/Type/PXCPU.h>

#include <GL/gl.h>
#include <PX/Type/PXBrush.h>

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

void PXAPI PXFooterRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXFooterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXFooterInfoDynamic;
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

PXResult PXAPI PXFooterDraw(PXFooter PXREF pxFooter, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    PXWindow PXREF pxWindow = pxFooter->WindowBase;
    PXEngineUITheme* pxGUITheme = pxDrawInfo->UITheme;

#if PXLogEnable && PXFooterDebug
    PXLogPrint
    (
        PXLoggingInfo,
        PXFooterText,
        "Draw",
        "---------- Footer ---------- BEGIN"
    );
#endif

    pxDrawInfo->Brush = pxGUITheme->BrushMainPrimary;
    //pxDrawInfo->RectangleXYWH.Width = 150;
    //pxDrawInfo->RectangleXYWH.Height = 20;

    PXEngineUIDrawRectangle3D_GL
    (
        pxWindow,
        pxDrawInfo
    );

    PXWindowFontSet(pxWindow, pxGUITheme->FontContent);


#if 1

    PXProcessor pxProcessor;
    PXProcessorFetchInfo(&pxProcessor);

    PXText pxText;
    PXTextFromAddressA(&pxText, pxProcessor.BrandName, pxProcessor.BrandNameSize, pxProcessor.BrandNameSize);

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


    pxDrawInfo->Text = &pxText;
    pxDrawInfo->Behaviour |= PXWindowAllignCenter;
    pxDrawInfo->Size = 20;
    pxDrawInfo->OffsetX = 0.2;

    PXEngineUIDrawText
    (
        pxWindow,
        pxDrawInfo
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
