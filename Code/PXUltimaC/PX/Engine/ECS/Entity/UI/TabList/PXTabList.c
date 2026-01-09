#include "PXTabList.h"
#include <PX/Engine/ECS/Resource/Brush/PXBrush.h>
#include <CommCtrl.h>


const char PXTabListName[] = "PXTabList";
const PXI8U PXTabListNameLength = sizeof(PXTabListName);
const PXECSRegisterInfoStatic PXTabListRegisterInfoStatic =
{
    {sizeof(PXTabListName), sizeof(PXTabListName), PXTabListName, TextFormatASCII},
    sizeof(PXTabList),
    __alignof(PXTabList),
    PXECSTypeEntity
};
PXECSRegisterInfoDynamic PXTabListRegisterInfoDynamic;

PXResult PXAPI PXTabListRegisterToECS()
{
    PXECSRegister(&PXTabListRegisterInfoStatic, &PXTabListRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXTabListDraw(PXTabList PXREF pxTabList, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    /*
    PXWindowExtendedBehaviourTab* pxWindowExtendedBehaviourTab = (PXWindowExtendedBehaviourTab*)pxWindow->ExtendedData;
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "PXID:%i, TabList",
        pxWindow->Info.ID
    );
#endif

    if(!pxWindowExtendedBehaviourTab)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXGUIName,
            "Draw",
            "TabList. Extended data is missing! Canceling draw!"
        );
#endif

        return PXResultRefusedParameterInvalid;
    }

    // int size = 110;
    int offsetX = 3;
    int offsetY = 4;
    int offsetSeperator = 2;
    int height = 25; // 25
    int iconMargin = height - 16;
    int iconSize = height - iconMargin;

    int left = pxWindow->Position.Form.X;
    int right = offsetY;

    for(PXSize i = 0; i < pxWindowExtendedBehaviourTab->TABPageAmount; ++i)
    {
        PXWindow PXREF pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

        pxWindowTABPage->NameContentSize = PXTextLengthA(pxWindowTABPage->NameContent, 260);

        PXSize predictSIze = 4 + pxWindowTABPage->NameContentSize * 10 + iconSize + iconMargin;

        predictSIze = PXMathMinimumIU(predictSIze, 120); // Bad fix

        PXSize width = predictSIze + offsetX;

        PXGUIDrawRectangle3D
        (
            pxWindow,
            left,
            0,
            left + width,
            height
        );
        PXNativDrawIconDraw
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxWindowTABPage->Icon,
            left + iconMargin / 2,
            iconMargin / 2,
            iconSize,
            iconSize
        );

        PXUIElementPosition pxUIElementPositionPrev = pxWindow->Position;

        pxWindow->Position.Form.X = left + iconSize + iconMargin + 1;
        pxWindow->Position.Form.Y = offsetY;
        pxWindow->Position.Form.Width = right;
        pxWindow->Position.Form.Height = height - offsetY;
        pxWindow->Info.Setting &= ~PXWindowAllignFlags;
        pxWindow->Info.Setting |= PXWindowAllignLeft;


        const PXBool isHovered = (PXECSInfoSelected & pxWindowTABPage->Info.Behaviour) > 0;


        PXBrush* brushFront = pxGUITheme->TextWhite;
        PXBrush* brushBackground = pxGUITheme->MainPrimary;

        if(isHovered)
        {
            brushBackground = pxGUITheme->TextWhite;
            brushFront = pxGUITheme->MainPrimary;
        }

        PXWindowBrushSet
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            brushBackground,
            PXGUIDrawModeBack
        );

        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            brushFront,
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
            pxWindowTABPage->NameContent,
            pxWindowTABPage->NameContentSize,
            PXWindowAllignLeft
        );

        pxWindow->Position = pxUIElementPositionPrev;

        // int totalWidth = offsetX + (predictSIze + offsetSeperator) * (i + 1);

        left += width + offsetSeperator;
        right += width + offsetY;
    }

    */

    return PXResultOK;
}

PXResult PXAPI PXTabListCreate(PXTabList** pxTabListREF, PXTabListCreateInfo PXREF pxTabListCreateInfo)
{
    PXWindowCreateInfo* pxWindowCreateInfo = &pxTabListCreateInfo->Window;

    PXTabList* pxTabList = PXNull;

    pxTabListCreateInfo->Info.Static = &PXTabListRegisterInfoStatic;
    pxTabListCreateInfo->Info.Dynamic = &PXTabListRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxTabListREF, pxTabListCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxTabList = *pxTabListREF;



#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_TABCONTROL;
    //pxWindowCreateInfo->WindowsStyleFlags |= WS_CLIPSIBLINGS | TCS_BUTTONS;

    pxWindowCreateInfo->WindowClassName.A = WC_STATIC;
#endif

    pxWindowCreateInfo->EventList.CallBackOwner = pxTabList;
    pxWindowCreateInfo->EventList.CallBackDraw = PXTabListDraw;

    return PXResultOK;
}