#include "PXHeader.h"
#include <PX/Engine/PXGUI.h>
#include <PX/OS/Console/PXConsole.h>

const char  PXHeaderText[] = "Header";

/*
#if OSWindows
pxWindowCreateInfo->WindowClassName.A = WC_HEADER;
#endif
*/

PXResult PXAPI PXHeaderDraw(PXWindow PXREF pxWindow, PXDrawInfo PXREF pxDrawInfo)
{
    PXGUITheme* pxGUITheme = PXGUIThemeGet();

   // const PXBool isHovered = (PXECSInfoSelected & pxWindow->Info.Behaviour) > 0;
    //PXWindowExtendedMenuItem PXREF pxWindowExtendedMenuItem = (PXWindowExtendedMenuItem*)pxWindow->ExtendedData;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXHeaderText,
        "Header",
        "---------- Header ---------- BEGIN"
    );
#endif

    /*
    PXNativDrawRectangle
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height
    );
    * /




    int y = 0;
    int height = 16;

    PXRectangleXYWHI32 block = pxWindow->Position.Form;

    for(PXSize i = 0; i < pxWindowExtendedMenuItem->MenuItemAmount; ++i)
    {
        PXWindowMenuItem PXREF pxWindowMenuItem = &pxWindowExtendedMenuItem->MenuItemList[i];


        //    PXUIElementPosition pxUIElementPositionPrev = pxWindow->Position;

           // pxWindow->Position.Top = y;
           // pxWindow->Position.Right = pxWindow->Position.Right;
           //pxWindow->Position.Bottom -= 1;

        //    int behavipur = pxWindow->Info.Behaviour;

          //  pxWindow->Info.Behaviour |= PXWindowKeepWidth;
          //  pxWindow->Info.Behaviour &= ~PXWindowAllignFlags;
          //  pxWindow->Info.Behaviour |= PXWindowAllignCenter;

         //   pxWindow->Position.Form.Right = pxWindow->Position.Left + pxWindow->Position.Width;

            // Set the width to the text size
        block.Width = pxWindowMenuItem->TextSize * 15;

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


#if 1
        PXGUIDrawRectangle3D
        (
            pxWindow,
            block.X,
            block.Y,
            block.Height,
            block.Height
        );
#endif

        // if we have an icon, draw it
        if(pxWindowMenuItem->Icon)
        {
            // Draw icom
            PXNativDrawIcon
            (
                &_pxGUIManager.NativDraw,
                pxWindow,
                pxWindowMenuItem->Icon,
                block.X,
                block.Y,
                block.Width
            );

            // Adjust offset
            block.X += block.Width;
        }

        if(!pxWindowMenuItem->TextData)
        {
            block.X += 25;

            continue;
        }





        // Allign with an offset for text
        int offset = 3;

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
            block.X + offset,
            block.Y + offset,
            block.Width - offset,
            block.Height - offset,
            pxWindowMenuItem->TextData,
            pxWindowMenuItem->TextSize,
            PXWindowAllignCenter
        );

        if(pxWindowMenuItem->TextData)
        {
            block.X += block.Width + 1;
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Header",
        "---------- Header ---------- END"
    );
#endif

    */

    return PXResultOK;
}