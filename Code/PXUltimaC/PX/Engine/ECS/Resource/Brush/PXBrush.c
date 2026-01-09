#include "PXBrush.h"

#include <PX/Engine/ECS/PXECS.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Engine/PXResource.h>
#include <PX/OS/PXOS.h>

const char PXBrushText[] = "Brush";
const PXI8U PXBrushTextLength = sizeof(PXBrushText);
const PXECSRegisterInfoStatic PXBrushRegisterInfo =
{
    {sizeof(PXBrushText), sizeof(PXBrushText), PXBrushText, TextFormatASCII},
    sizeof(PXBrush),
    __alignof(PXBrush),
    PXECSTypeResource,
    PXBrushCreate
};
PXECSRegisterInfoDynamic PXBrushRegisterInfoDynamic;

PXResult PXAPI PXBrushRegisterToECS()
{
    PXECSRegister(&PXBrushRegisterInfo, &PXBrushRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXBrushCreate(PXBrush** pxBrushRef, PXBrushCreateInfo PXREF pxBrushCreateInfo)
{
    PXColorRGBI8* pxColorRGBI8 = &pxBrushCreateInfo->Color;
    PXBrush* pxBrush = PXNull;

    pxBrushCreateInfo->Info.Static = &PXBrushRegisterInfo;
    pxBrushCreateInfo->Info.Dynamic = &PXBrushRegisterInfoDynamic;
    PXECSCreate(pxBrushRef, pxBrushCreateInfo);

    pxBrush = *pxBrushRef;

    PXBrushColorSet(pxBrush, pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);

#if OSUnix
    // X11 does not have a brush, right?
#elif OSWindows   
    const COLORREF brushColor = RGB(pxColorRGBI8->Red, pxColorRGBI8->Green, pxColorRGBI8->Blue);

    pxBrush->BrushHandle = CreateSolidBrush(brushColor);

    // Color xx = Color(255, 0, 0, 255);
    // SolidBrush ww = opaqueBrush();
#endif  

    pxBrush->Info.Behaviour |= PXECSInfoExist;

    return PXResultOK;
}

PXResult PXAPI PXBrushColorSet(PXBrush PXREF pxWindowBrush, const PXByte red, const PXByte green, const PXByte blue)
{
    pxWindowBrush->Info.Behaviour |= PXWindowBrushBehaviourColorEmbeded;
    pxWindowBrush->ColorDate.Red = red;
    pxWindowBrush->ColorDate.Green = green;
    pxWindowBrush->ColorDate.Blue = blue;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXBrushText,
        "Color-Set",
        "PXID:<%4i>, RGB:%2.2x%2.2x%2.2x",
        pxWindowBrush->Info.ID,
        red,
        green,
        blue
    );
#endif 

    return PXResultOK;
}