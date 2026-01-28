#include "PXLabel.h"

PXResult PXAPI PXLabelDraw(PXLabel PXREF pxLabel, PXLabelCreateInfo PXREF pxLabelCreateInfo)
{
    /*
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

    const char* text = pxDrawInfo->Content ? pxDrawInfo->Content : pxWindow->NameContent;
    const PXSize textSize = pxDrawInfo->Content ? pxDrawInfo->ContentSize : pxWindow->NameContentSize;

    const int shadowOffset = 1;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Text",
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i - (%i) %s",
        pxWindow->Info.ID,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height,
        textSize,
        text
    );
#endif

    // Draw background
    {
        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxGUITheme->TextWhite,
            PXGUIDrawModeFront
        );

        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxGUITheme->MainPrimary,
            PXGUIDrawModeBack
        );

        PXNativDrawRectangle
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxWindow->Position.Form.X,
            pxWindow->Position.Form.Y,
            pxWindow->Position.Form.Width,
            pxWindow->Position.Form.Height,
            pxGUITheme->MainPrimary
        );
    }

    // Load font
    PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);


    // Text-Shadow
    {
        PXNativDrawSetRGB
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxGUITheme->TextWhite->ColorDate.Red * 0.25f,
            pxGUITheme->TextWhite->ColorDate.Green * 0.25f,
            pxGUITheme->TextWhite->ColorDate.Blue * 0.25f,
            PXGUIDrawModeFront
        );

        PXNativDrawA
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxWindow->Position.Form.X - shadowOffset,
            pxWindow->Position.Form.Y + shadowOffset,
            pxWindow->Position.Form.Width - shadowOffset,
            pxWindow->Position.Form.Height + shadowOffset,
            text,
            textSize,
            PXWindowAllignLeft
        );
    }

    // Text-Content
    {
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
            text,
            textSize,
            PXWindowAllignLeft
        );
    }

    return PXResultOK;
}


/*
            PXUIElementTextInfo PXREF pxUIElementTextInfo = &pxWindowCreateInfo->Data.Text;

            pxWindowCreateInfo->WindowsTextContent = pxUIElementTextInfo->Content;

            switch(pxUIElementTextInfo->Allign)
            {
                case PXUIElementAllignLeft:
                    pxWindowCreateInfo->WindowsStyleFlags |= SS_LEFT;
                    break;

                case PXUIElementAllignRight:
                    pxWindowCreateInfo->WindowsStyleFlags |= SS_RIGHT;
                    break;

                default:
                case PXUIElementAllignCenter:
                    pxWindowCreateInfo->WindowsStyleFlags |= SS_CENTER;
                    break;
            }
            */

    PXBool hasParenet = 0;// pxWindowCreateInfo->UIElementParent;

    /*
    if(hasParenet)
    {
        pxWindowCreateInfo->AvoidCreation = PXUIElementTypeButton == pxWindow->Parent->Type;

        if(pxWindowCreateInfo->AvoidCreation)
        {
            pxWindow->Type = PXUIElementTypeButtonText;
            pxWindow->Info.Handle.WindowHandle = PXNull;
            //return;
        }
    }
    */
}