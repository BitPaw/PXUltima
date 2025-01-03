#include "PXGUI.h"

#include <Media/PXText.h>
#include <Math/PXMath.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Async/PXAwait.h>
#include <OS/Graphic/PXGraphic.h>
#include <OS/Console/PXConsole.h>
#include <Engine/PXEngine.h>
#include <OS/File/PXDirectory.h>
#include <OS/Graphic/NativDraw/PXNativDraw.h>
#include <OS/Hardware/PXProcessor.h>

#if OSUnix

// Xlib / X-System
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include <X11/Xmd.h>

//#include <wayland-client.h>

#define DefautPositionX 00000000000000
#define DefautPositionY 00000000000000

#define MouseButtonLeft 1
#define MouseButtonMiddle 2
#define MouseButtonRight 3
#define MouseScrollUp 4
#define MouseScrollDown 5

#elif OSWindows
#include <windowsx.h>
#include <WinUser.h>
#include <wtypes.h>
//#include <hidusage.h>
#include <Dbt.h>
#include <ole2.h> // Object Linking and Embedding
#include <dwmapi.h> // Set tilebar color
//#include <gdiplusgraphics.h>
//#include <ddrawgdi.h>
#include <shellapi.h>

#define Windows10DarkModeID 20 // DWMWA_USE_IMMERSIVE_DARK_MODE

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Msimg32.lib")

#define DefautPositionX CW_USEDEFAULT
#define DefautPositionY CW_USEDEFAULT

#endif

#if OSUnix
//#include <gtk/gtk.h>
#elif OSWindows
#include <Windows.h>
#include <commctrl.h>
#include <commdlg.h>
#include <Richedit.h>
#include <WindowsX.h>



//#include <ShObjIdl.h>

#if WindowsEnableModernColorSceme

// use this define to disable manifest dependency
#define _PFD_DISABLE_MANIFEST 1

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "Comctl32.lib")

#endif
#endif






#if OSUnix

#define CursorIconNormalID 1
#define CursorIconIBeamID 2
#define CursorIconWaitID 3
#define CursorIconCrossID 4
#define CursorIconUpID 5
#define CursorIconSizeID 6
#define CursorIconIconID 7
#define CursorIconResizeClockwiseCounterID 8
#define CursorIconResizeClockwiseID 10
#define CursorIconResizeHorizontalID 11
#define CursorIconResizeVerticalID 12
#define CursorIconResizeAllID 13
#define CursorIconCursorID 14
#define CursorIconNoAllowedID 15
#define CursorIconHandID 16
#define CursorIconAppStartingID 17

#elif OSWindows

#include <winuser.rh> // MISSING
#include <Engine/PXEngine.h>
#define CursorIconNormalID OCR_NORMAL
#define CursorIconIBeamID OCR_IBEAM
#define CursorIconWaitID OCR_WAIT
#define CursorIconCrossID OCR_CROSS
#define CursorIconUpID OCR_UP
#define CursorIconSizeID OCR_SIZE               // OBSOLETE: use OCR_SIZEALL
#define CursorIconIconID OCR_ICON               // OBSOLETE: use OCR_NORMAL
#define CursorIconResizeClockwiseCounterID OCR_SIZENWSE
#define CursorIconResizeClockwiseID OCR_SIZENESW
#define CursorIconResizeHorizontalID OCR_SIZEWE
#define CursorIconResizeVerticalID OCR_SIZENS
#define CursorIconResizeAllID OCR_SIZEALL
#define CursorIconCursorID OCR_ICOCUR             // OBSOLETE: use OIC_WINLOGO
#define CursorIconNoAllowedID OCR_NO

#if defined(WindowsAtleast2000)
#define CursorIconHandID OCR_HAND
#else
#define CursorIconHandID 32649
#endif

#if defined(WindowsAtleastNT)
#define CursorIconAppStartingID OCR_APPSTARTING
#else
#define CursorIconAppStartingID 32650
#endif

#endif






PXActionResult PXAPI PXWindowDelete(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
    const int resultID = XDestroyWindow(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUIElement->Info.Handle.WindowID);
    result = PXWindowErrorFromXSystem(resultID);
#elif OSWindows
    const PXBool success = DestroyWindow(pxGUIElement->Info.Handle.WindowID);
    result = PXErrorCurrent(success);
#else
    result PXActionRefusedNotSupportedByLibrary;
#endif

    if(PXActionSuccessful == result)
    {
        pxGUIElement->Info.Handle.WindowID = PXNull;
    }

    return result;
}

PXActionResult PXAPI PXWindowDrawCustomRectangle3D(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, const int x, const int y, const int width, const int height)
{
    const PXBool isHovered = (PXWindowBehaviourIsBeingHovered & pxGUIElement->Info.Behaviour) > 0;

    PXWindowBrush* brushFront = pxGUIElement->BrushFront;
    PXWindowBrush* brushBackground = pxGUIElement->BrushBackground;

    PXNativDrawColorSetBrush
    (
        pxGUISystem,
        pxGUIElement,
        brushFront,
        PXGUIDrawModeFront
    );

    if(isHovered)
    {
        PXNativDrawColorSetBrush
        (
            pxGUISystem,
            pxGUIElement,
            brushFront,
            PXGUIDrawModeBack
        );
    }
    else
    {
        PXNativDrawColorSetBrush
        (
            pxGUISystem,
            pxGUIElement,
            brushBackground,
            PXGUIDrawModeBack
        );
    }

    PXNativDrawRectangle
    (
        pxGUISystem,
        pxGUIElement,
        x,
        y,
        width,
        height
    );

    const float colorTint = isHovered * 0.4f;
    const float highFactor = 1.35f;
    const float lowFactor = 0.65f;


#if OSWindows

    const COLORREF highColor = RGB
    (
        colorTint + highFactor * brushBackground->ColorDate.Red,
        colorTint + highFactor * brushBackground->ColorDate.Green,
        colorTint + highFactor * brushBackground->ColorDate.Blue
    );
    const COLORREF lowColor = RGB
    (
        colorTint + lowFactor * brushBackground->ColorDate.Red,
        colorTint + lowFactor * brushBackground->ColorDate.Green,
        colorTint + lowFactor * brushBackground->ColorDate.Blue
    );

    const DWORD penStyle = PS_ENDCAP_SQUARE | PS_GEOMETRIC | PS_SOLID | PS_JOIN_MITER;
    const DWORD penSize = 2;
    const PXInt16U borderInwardOffset = penSize - 1;

    if(penSize == 0)
    {
        return PXActionSuccessful;
    }

    const HPEN penLight = CreatePen(penStyle, penSize, highColor);
    const HPEN penDark = CreatePen(penStyle, penSize, lowColor);

    // Bottom, right, dark line
    MoveToEx(pxGUIElement->DeviceContextHandle, width - borderInwardOffset, y + borderInwardOffset, PXNull);
    SelectPen(pxGUIElement->DeviceContextHandle, penDark);
    LineTo(pxGUIElement->DeviceContextHandle, width - borderInwardOffset, height - borderInwardOffset);
    LineTo(pxGUIElement->DeviceContextHandle, x + borderInwardOffset, height - borderInwardOffset);

    SelectPen(pxGUIElement->DeviceContextHandle, penLight);
    LineTo(pxGUIElement->DeviceContextHandle, x + borderInwardOffset, y + borderInwardOffset);
    LineTo(pxGUIElement->DeviceContextHandle, width - borderInwardOffset, y + borderInwardOffset);

    DeletePen(penLight);
    DeletePen(penDark);
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomHeader(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
    const PXBool isHovered = (PXWindowBehaviourIsBeingHovered & pxGUIElement->Info.Behaviour) > 0;

    PXWindowExtendedMenuItem* const pxWindowExtendedMenuItem = (PXWindowExtendedMenuItem*)pxGUIElement->ExtendedData;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "Header"
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);

    PXNativDrawRectangle
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->Position.Left,
        pxGUIElement->Position.Top,
        pxGUIElement->Position.Right,
        pxGUIElement->Position.Bottom
    );


    int left = pxGUIElement->Position.Left;
    int y = 0;
    int height = 16;

    for(PXSize i = 0; i < pxWindowExtendedMenuItem->MenuItemAmount; ++i)
    {
        PXWindowMenuItem* const pxWindowMenuItem = &pxWindowExtendedMenuItem->MenuItemList[i];


        PXUIElementPosition pxUIElementPositionPrev = pxGUIElement->Position;

        pxGUIElement->Position.Left = left;
       // pxGUIElement->Position.Top = y;
       // pxGUIElement->Position.Right = pxGUIElement->Position.Right;
       //pxGUIElement->Position.Bottom -= 1;
       pxGUIElement->Position.Width = pxWindowMenuItem->TextSize * 15;
        pxGUIElement->Info.Behaviour |= PXWindowKeepWidth;
        pxGUIElement->Info.Behaviour &= ~PXWindowAllignFlags;
        pxGUIElement->Info.Behaviour |= PXWindowAllignCenter;

        pxGUIElement->Position.Right = pxGUIElement->Position.Left + pxGUIElement->Position.Width;

        pxGUIElement->Position.Height -= 2;


#if 1
        PXWindowDrawCustomRectangle3D
        (
            pxGUISystem,
            pxGUIElement,
            pxGUIElement->Position.Left,
            pxGUIElement->Position.Top,
            pxGUIElement->Position.Left + pxGUIElement->Position.Width,
            pxGUIElement->Position.Height
        );
#endif



        // Allign with an offset for text
        int offset = 3;

        pxGUIElement->Position.Left += offset;
        pxGUIElement->Position.Top += offset;
       // pxGUIElement->Position.Width -= offset;
        pxGUIElement->Position.Height -= offset;


        PXNativDrawTextA(pxGUISystem, pxGUIElement, pxWindowMenuItem->TextData, pxWindowMenuItem->TextSize);

        if(pxWindowMenuItem->TextData)
        {
            left += pxGUIElement->Position.Width + 1;
        }

        pxGUIElement->Position = pxUIElementPositionPrev;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomFooter(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "Footer"
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);

    PXNativDrawRectangle
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->Position.Left,
        pxGUIElement->Position.Top,
        pxGUIElement->Position.Right,
        pxGUIElement->Position.Bottom
    );

    PXProcessor pxProcessor;
    PXProcessorFetchInfo(&pxProcessor);

    PXNativDrawTextA(pxGUISystem, pxGUIElement, pxProcessor.BrandName, pxProcessor.BrandNameSize);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomResourceView(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
    PXWindowExtendedBehaviourResourceView* const pxWindowExtendedBehaviourResourceView = (PXWindowExtendedBehaviourResourceView*)pxGUIElement->ExtendedData;

    PXResourceManager* pxResourceManagerScene = pxGUISystem->ResourceManager;
    PXResourceManager* pxResourceManagerInGame = pxWindowExtendedBehaviourResourceView->ResourceManager;


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "ResourceView"
    );
#endif
    PXIcon pxIconPlaceHolder;

    PXNativDrawLoad(pxGUISystem, &pxIconPlaceHolder, PXNull);



    PXNativDrawClear(pxGUISystem, pxGUIElement);

    int left = pxGUIElement->Position.Left;
    int y = 0;
    int height = 16;

    // textures
    PXResourceManager* pxResourceManager = pxGUISystem->ResourceManager;
    PXDictionary* textureLookup = &pxResourceManager->ImageLookUp;
    PXDictionary* shaderLookup = &pxResourceManager->ShaderProgramLookup;
    PXDictionary* guiLookup = &pxResourceManager->GUIElementLookup;

    // Title
    {
        PXUIElementPosition pxUIElementPositionPrev = pxGUIElement->Position;

        pxGUIElement->Position.Left = left;
        pxGUIElement->Position.Top = y;
        pxGUIElement->Position.Right = pxGUIElement->Position.Right;
        pxGUIElement->Position.Bottom = y + height;
        pxGUIElement->Info.Behaviour &= ~PXWindowAllignFlags;
        pxGUIElement->Info.Behaviour |= PXWindowAllignLeft;

        PXNativDrawTextA
        (
            pxGUISystem,
            pxGUIElement,
            "Image",
            5
        );

        pxGUIElement->Position = pxUIElementPositionPrev;

        y += height + 3;
    }



    for(PXSize i = 0; i < textureLookup->EntryAmountCurrent; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXImage* pxImage = PXNull;

        PXDictionaryIndex(textureLookup, i, &pxDictionaryEntry);

        pxImage = *(PXImage**)pxDictionaryEntry.Value;

        PXWindowDrawCustomRectangle3D
        (
            pxGUISystem,
            pxGUIElement,
            left,
            y,
            pxGUIElement->Position.Right,
            y + height
        );

        PXUIElementPosition pxUIElementPositionPrev = pxGUIElement->Position;

        pxGUIElement->Position.Left = left;
        pxGUIElement->Position.Top = y;
        pxGUIElement->Position.Right = pxGUIElement->Position.Right;
        pxGUIElement->Position.Bottom = y + height;
        pxGUIElement->Info.Behaviour &= ~PXWindowAllignFlags;
        pxGUIElement->Info.Behaviour |= PXWindowAllignLeft;


        PXNativDrawIcon
        (
            pxGUISystem,
            pxGUIElement,
            &pxIconPlaceHolder,
            pxGUIElement->Position.Left + 2,
            pxGUIElement->Position.Top,
            16,
            16
        );

        pxGUIElement->Position.Left += 16 + 4;

        char buffer[260];
        PXSize wrizze = PXTextPrintA(buffer, 260, "[%i] %ix%i", 1000, pxImage->Width, pxImage->Height);

        PXNativDrawTextA
        (
            pxGUISystem,
            pxGUIElement,
            buffer,
            wrizze
        );

        pxGUIElement->Position = pxUIElementPositionPrev;


        y += height + 3;
    }






    // Title
    {
        PXUIElementPosition pxUIElementPositionPrev = pxGUIElement->Position;

        pxGUIElement->Position.Left = left;
        pxGUIElement->Position.Top = y;
        pxGUIElement->Position.Right = pxGUIElement->Position.Right;
        pxGUIElement->Position.Bottom = y + height;
        pxGUIElement->Info.Behaviour &= ~PXWindowAllignFlags;
        pxGUIElement->Info.Behaviour |= PXWindowAllignLeft;

        PXNativDrawTextA
        (
            pxGUISystem,
            pxGUIElement,
            "GUI Elements",
            12
        );

        pxGUIElement->Position = pxUIElementPositionPrev;

        y += height + 3;
    }


    for(PXSize i = 0; i < 20; i+=2)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXWindow* pxWindow = PXNull;

        PXDictionaryIndex(guiLookup, i, &pxDictionaryEntry);

        pxWindow = *(PXWindow**)pxDictionaryEntry.Value;



        PXWindowDrawCustomRectangle3D
        (
            pxGUISystem,
            pxGUIElement,
            left,
            y,
            pxGUIElement->Position.Right,
            y + height
        );

        PXUIElementPosition pxUIElementPositionPrev = pxGUIElement->Position;

        pxGUIElement->Position.Left = left;
        pxGUIElement->Position.Top = y;
        pxGUIElement->Position.Right = pxGUIElement->Position.Right;
        pxGUIElement->Position.Bottom = y + height;
        pxGUIElement->Info.Behaviour &= ~PXWindowAllignFlags;
        pxGUIElement->Info.Behaviour |= PXWindowAllignLeft;


        PXNativDrawIcon
        (
            pxGUISystem,
            pxGUIElement,
            &pxIconPlaceHolder,
            pxGUIElement->Position.Left + 2,
            pxGUIElement->Position.Top,
            16,
            16
        );


        pxGUIElement->Position.Left += 16 + 4;

        char buffer[260];

        char* typeName = PXUIElementTypeToString(pxWindow->Type);

        PXSize wrizze = PXTextPrintA(buffer, 260, "[%i] %s:%s", pxWindow->Info.ID, typeName, pxWindow->NameContent);

        PXNativDrawTextA
        (
            pxGUISystem,
            pxGUIElement,
            buffer,
            wrizze
        );

        pxGUIElement->Position = pxUIElementPositionPrev;


        y += height + 3;
    }


    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomResourceInfo(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "TabList"
    );
#endif

    PXResourceInfo* pxResourceInfo = 0;

    PXDictionaryEntry pxDictionaryEntry;


    PXDictionaryIndex(&pxGUISystem->ResourceManager->GUIElementLookup, 0, &pxDictionaryEntry);

    pxResourceInfo = &(*(PXWindow**)pxDictionaryEntry.Value)->Info;


    PXNativDrawClear(pxGUISystem, pxGUIElement);

    char nameID[32];

    PXSize textSize = PXTextPrintA(nameID, 32, "ID: %i", pxResourceInfo->ID);

    PXNativDrawTextA(pxGUISystem, pxGUIElement, nameID, textSize);

    PXIcon pxIcon;

    PXIconLoad(&pxIcon);


    int left = pxGUIElement->Position.Left;
    int y = 16 + 10;
    int height = 16;


    char* table[16] =
    {
        "Exist",
        "Active",
        "Render",

        "Drive",
        "Cached",
        "Memory",
        "Device",

        "Name",
        "Source",

        "OS",
        "User",
        "Engine",
        "Undefined",

        "READ",
        "WRITE",
        "EXECUTE",
    };


    for(size_t i = 0; i < 16; i++)
    {
        PXUIElementPosition pxUIElementPositionPrev = pxGUIElement->Position;

        pxGUIElement->Position.Left = left;
        pxGUIElement->Position.Top = y;
        pxGUIElement->Position.Right = pxGUIElement->Position.Right;
        pxGUIElement->Position.Bottom = y + height;
        pxGUIElement->Info.Behaviour &= ~PXWindowAllignFlags;
        pxGUIElement->Info.Behaviour |= PXWindowAllignLeft;


        PXNativDrawIcon
        (
            pxGUISystem,
            pxGUIElement,
            &pxIcon,
            pxGUIElement->Position.Left + 2,
            pxGUIElement->Position.Top,
            16,
            16
        );

        pxGUIElement->Position.Left += 16 + 4;


        PXSize len = PXTextLengthA(table[i], 30);

        PXNativDrawTextA(pxGUISystem, pxGUIElement, table[i], len);



        pxGUIElement->Position.Left += 120;

        char* textxx = (pxResourceInfo->Flags & (1<<i)) != 0 ? "Yes" : "No";
        len = PXTextLengthA(textxx, 4);

        PXNativDrawTextA(pxGUISystem, pxGUIElement, textxx, len);


        pxGUIElement->Position = pxUIElementPositionPrev;


        y += height + 3;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomTabList(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
    PXWindowExtendedBehaviourTab* pxWindowExtendedBehaviourTab = (PXWindowExtendedBehaviourTab*)pxGUIElement->ExtendedData;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "TabList"
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);

   // int size = 110;
    int offsetX = 3;
    int offsetY = 4;
    int offsetSeperator = 2;
    int height = 25; // 25
    int iconMargin = height - 16;
    int iconSize = height- iconMargin;

    int left = pxGUIElement->Position.X;
    int right = offsetY;

    for(PXSize i = 0; i < pxWindowExtendedBehaviourTab->TABPageAmount; ++i)
    {
        PXWindow* const pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

        pxWindowTABPage->NameContentSize = PXTextLengthA(pxWindowTABPage->NameContent, 260);

        PXSize predictSIze = 4 + pxWindowTABPage->NameContentSize * 10 + iconSize + iconMargin;

        predictSIze = PXMathMinimumIU(predictSIze, 120); // Bad fix

        PXSize width = predictSIze + offsetX;

        //PXNativDrawRectangle(pxGUISystem, pxGUIElement, left, 0, left+ size+ offsetX, height);
        PXWindowDrawCustomRectangle3D
        (
            pxGUISystem,
            pxGUIElement,
            left,
            0,
            left + width,
            height
        );
        PXNativDrawIcon
        (
            pxGUISystem,
            pxGUIElement,
            pxWindowTABPage->Icon,
            left + iconMargin / 2,
            iconMargin / 2,
            iconSize,
            iconSize
        );

        PXUIElementPosition pxUIElementPositionPrev = pxGUIElement->Position;

        pxGUIElement->Position.Left = left + iconSize+ iconMargin +1;
        pxGUIElement->Position.Top = offsetY;
        pxGUIElement->Position.Right = right;
        pxGUIElement->Position.Bottom = height - offsetY;
        pxGUIElement->Info.Behaviour &= ~PXWindowAllignFlags;
        pxGUIElement->Info.Behaviour |= PXWindowAllignLeft;

        PXNativDrawTextA(pxGUISystem, pxGUIElement, pxWindowTABPage->NameContent, pxWindowTABPage->NameContentSize);

        pxGUIElement->Position = pxUIElementPositionPrev;

       // int totalWidth = offsetX + (predictSIze + offsetSeperator) * (i + 1);

        left += width + offsetSeperator;
        right += width + offsetY;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomFailback(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "FailBack"
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);

    PXWindowBrush brushRed;
    PXWindowBrushColorSet(&brushRed, 0xFF, 0x00, 0x00);
    PXWindowBrush brushFont;
    PXWindowBrushColorSet(&brushFont, 0xFF, 0xA0, 0xA0);

    PXNativDrawColorSetBrush
    (
        pxGUISystem,
        pxGUIElement,
        &brushRed,
        PXGUIDrawModeFront
    );

    PXNativDrawColorSetBrush
    (
        pxGUISystem,
        pxGUIElement,
        &brushFont,
        PXGUIDrawModeBack
    );

    PXNativDrawRectangle
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->Position.Left,
        pxGUIElement->Position.Top,
        pxGUIElement->Position.Right,
        pxGUIElement->Position.Bottom
    );

    PXNativDrawTextA(pxGUISystem, pxGUIElement, pxGUIElement->NameContent, pxGUIElement->NameContentSize);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomText(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "Text: %s",
        pxGUIElement->NameContent
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);

    PXNativDrawColorSetBrush
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->BrushFront,
        PXGUIDrawModeFront
    );
    PXNativDrawColorSetBrush
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->BrushBackground,
        PXGUIDrawModeBack
    );

    // PXWindowDrawBegin(pxGUISystem, pxGUIElement);

    PXNativDrawRectangle
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->Position.Left,
        pxGUIElement->Position.Top,
        pxGUIElement->Position.Right,
        pxGUIElement->Position.Bottom
    );

    PXNativDrawTextA(pxGUISystem, pxGUIElement, pxGUIElement->NameContent, pxGUIElement->NameContentSize);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomButton(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "Button"
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);

    PXWindowDrawCustomRectangle3D
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->Position.Left,
        pxGUIElement->Position.Top,
        pxGUIElement->Position.Right,
        pxGUIElement->Position.Bottom
    );

    PXNativDrawTextA(pxGUISystem, pxGUIElement, pxGUIElement->NameContent, pxGUIElement->NameContentSize);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowDrawCustomComboBox(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
    return PXActionRefusedNotImplemented;
}


typedef struct PXColorCircleVertex_
{
    float X;
    float Y;
    PXInt8U Red;
    PXInt8U Green;
    PXInt8U Blue;
}
PXColorCircleVertex;

typedef struct PXColorCircle_
{
    int StartX;
    int StartY;
    int Size;
    int Precision;
    PXColorCircleVertex* VertexList;




    // Triangle
    PXColorCircleVertex VertexListTriangle[3];

    PXColorHSV ColorSelected;
}
PXColorCircle;




void PXMathCircle(PXColorCircle* const pxColorCircle)
{


    for(size_t i = 0; i < pxColorCircle->Precision; ++i)
    {
        PXColorCircleVertex* vertex = &pxColorCircle->VertexList[i];

        const float steps = 360.f / (float)pxColorCircle->Precision;
        const float degree = (steps * (i + 0));
        const float rad = degree * (3.14f / 180.f);

        vertex->X = pxColorCircle->StartX + pxColorCircle->Size * PXMathCosinus(rad);
        vertex->Y = pxColorCircle->StartY + pxColorCircle->Size * PXMathSinus(rad);


        PXColorHSV hsv;
        hsv.Hue = degree;
        hsv.Saturation = 1;
        hsv.Value = 1;

        PXColorRGBAI8 pxColorRGBAI8;

        PXColorHSVToRGBAI8(&hsv, &pxColorRGBAI8);

        vertex->Red = pxColorRGBAI8.Red;
        vertex->Green = pxColorRGBAI8.Green;
        vertex->Blue = pxColorRGBAI8.Blue;
    }

    // Triangle
    for(size_t i = 0; i < 3; ++i)
    {
        PXColorCircleVertex* vertex = &pxColorCircle->VertexListTriangle[i];

        const float steps = 360.f / (float)3;
        const float degree = (int)((steps * (i + 0)) + pxColorCircle->ColorSelected.Hue) % 360;
        const float rad = degree * (3.14f / 180.f);

        vertex->X = pxColorCircle->StartX + pxColorCircle->Size * 0.85f * PXMathCosinus(rad);
        vertex->Y = pxColorCircle->StartY + pxColorCircle->Size * 0.85f * PXMathSinus(rad);
    }


}

float colorTemp = 20;

PXActionResult PXAPI PXWindowDrawCustomColorPicker(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "ColorPicker"
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);



    PXNativDrawRectangle
    (
        pxGUISystem,
        pxGUIElement,
        pxGUIElement->Position.Left,
        pxGUIElement->Position.Top,
        pxGUIElement->Position.Right,
        pxGUIElement->Position.Bottom
    );


    float scaling = 0.80f;
    int width = (pxGUIElement->Position.Right - pxGUIElement->Position.Left) / 2;
    int height = (pxGUIElement->Position.Bottom - pxGUIElement->Position.Top) / 2;
    int widthS = (pxGUIElement->Position.Right * scaling - pxGUIElement->Position.Left * scaling) / 2;
    int heightS = (pxGUIElement->Position.Bottom * scaling - pxGUIElement->Position.Top * scaling) / 2;

    int precision = 8;

    float r = 250;

#if OSUnix
#elif OSWindows
    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(pxGUIElement->DeviceContextHandle, brush);

    {
        TRIVERTEX vertexList[120];
        GRADIENT_TRIANGLE gRect[120];

        for(size_t i = 0; i < precision; ++i)
        {
            TRIVERTEX* const vertex = &vertexList[i];
            GRADIENT_TRIANGLE* const index = &gRect[i];

            const float steps = 360.f / (float)precision;
            float degree = (steps * (i + 1)) * (3.14f / 180.f);

            float x = PXMathCosinus(degree);
            float y = PXMathSinus(degree);

            vertex->x = r * x + width;
            vertex->y = r * y + height;
            vertex->Red = 0xFF;
            vertex->Green = ((i) / (float)precision) * 0xFF;
            vertex->Blue = 0x0000;
            vertex->Alpha = 0;

            index->Vertex1 = 0;
            index->Vertex2 = i;
            index->Vertex3 = i + 1;
        }




        // const int precision = 360;
        PXColorCircleVertex pxColorCircleVertexList[360];

        PXColorCircle pxColorCircle;
        pxColorCircle.Precision = 64;
        pxColorCircle.Size = 225;
        pxColorCircle.VertexList = pxColorCircleVertexList;
        pxColorCircle.StartX = width;
        pxColorCircle.StartY = height;
        pxColorCircle.ColorSelected.Hue = colorTemp;
        pxColorCircle.ColorSelected.Saturation = 1;
        pxColorCircle.ColorSelected.Value = 1;

        PXMathCircle(&pxColorCircle);

        colorTemp += 15;
        colorTemp = (int)colorTemp % 360;

        for(size_t i = 0; i < pxColorCircle.Precision; ++i)
        {
            int indexA = i;
            int indexB = (i + 1) % pxColorCircle.Precision;

            PXColorCircleVertex* dataA = &pxColorCircleVertexList[indexA];
            PXColorCircleVertex* dataB = &pxColorCircleVertexList[indexB];

            // TRIVERTEX* vertexA = &vertexList[i];
            // TRIVERTEX* vertexB = &vertexList[indexB];

            auto hPen = CreatePen(PS_SOLID, 20, RGB(dataA->Red, dataA->Green, dataA->Blue));
            SelectObject(pxGUIElement->DeviceContextHandle, hPen);

            MoveToEx(pxGUIElement->DeviceContextHandle, dataA->X, dataA->Y, NULL);
            LineTo(pxGUIElement->DeviceContextHandle, dataB->X, dataB->Y);

        }

        GradientFill(pxGUIElement->DeviceContextHandle, vertexList, pxColorCircle.Precision * 3, &gRect, 1, GRADIENT_FILL_TRIANGLE);














        PXColorRGBAI8 pxColorRGBAI8;

        PXColorHSVToRGBAI8(&pxColorCircle.ColorSelected, &pxColorRGBAI8);

        TRIVERTEX vertices[3];


        // Define the vertices of the triangle
        vertices[0].x = pxColorCircle.VertexListTriangle[1].X;
        vertices[0].y = pxColorCircle.VertexListTriangle[1].Y;
        vertices[0].Red = 0x0000;
        vertices[0].Green = 0x0000;
        vertices[0].Blue = 0x0000;
        vertices[0].Alpha = 0xFFFF;

        vertices[1].x = pxColorCircle.VertexListTriangle[0].X;
        vertices[1].y = pxColorCircle.VertexListTriangle[0].Y;
        vertices[1].Red = (pxColorRGBAI8.Red / (float)0xFF) * 0xFFFF;
        vertices[1].Green = (pxColorRGBAI8.Green / (float)0xFF) * 0xFFFF;
        vertices[1].Blue = (pxColorRGBAI8.Blue / (float)0xFF) * 0xFFFF;
        vertices[1].Alpha = 0xFFFF;

        vertices[2].x = pxColorCircle.VertexListTriangle[2].X;
        vertices[2].y = pxColorCircle.VertexListTriangle[2].Y;
        vertices[2].Red = 0xFFFF;
        vertices[2].Green = 0xFFFF;
        vertices[2].Blue = 0xFFFF;
        vertices[2].Alpha = 0xFFFF;

        // Draw the triangle
        // HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
        GRADIENT_TRIANGLE  gRectaaa;
        gRectaaa.Vertex1 = 0;
        gRectaaa.Vertex2 = 1;
        gRectaaa.Vertex3 = 2;




        // Draw the gradient rectangle
        GradientFill(pxGUIElement->DeviceContextHandle, vertices, 3, &gRectaaa, 1, GRADIENT_FILL_TRIANGLE);



    }


#endif


    /*

    int width = 128;
    int height = 128;
    float scale = 80.5;

    for(int i = 0; i < width; ++i) {
        for(int j = 0; j < height; ++j) {
            HBRUSH brush = CreateSolidBrush(RGB(i * width * scale, j * height * scale, 128));
            RECT rect =
            {
                i * 15,
                j * 15,
                (i + 1) * 15,
                (j + 1) * 15
            };

            rect.left = pxGUIElement->Position.Left - rect.left;
            rect.top = pxGUIElement->Position.Top - rect.top;
            rect.right = pxGUIElement->Position.Right - rect.right;
            rect.bottom = pxGUIElement->Position.Bottom - rect.bottom;

            FillRect(pxGUIElement->DeviceContextHandle, &rect, brush);
            DeleteObject(brush);
        }
    }
    */




    //   PXWindowDrawTextA(pxGUISystem, pxGUIElement, pxGUIElement->NameContent, pxGUIElement->NameContentSize);

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXWindowDrawCustomHexView(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "HexView"
    );
#endif

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXWindowDrawFileDirectoryView(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXWindowDrawInfo* const pxGUIElementDrawInfo)
{
    PXDirectorySearchCache* const pxDirectorySearchCache = (PXDirectorySearchCache*)pxGUIElement->ExtendedData;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Draw",
        "FileDirectoryView"
    );
#endif

    PXNativDrawClear(pxGUISystem, pxGUIElement);

    PXText pxTExt;
    PXTextMakeFixedA(&pxTExt, "./");

    PXDirectorySearch(pxDirectorySearchCache, &pxTExt);

    PXWindow pxGUIElementSub;
    PXCopy(PXWindow, pxGUIElement, &pxGUIElementSub);
    pxGUIElementSub.Info.Behaviour = PXResourceInfoOK | PXWindowAllignTop | PXWindowKeepHeight | PXWindowAllignLeft;
    pxGUIElementSub.Position.Height = 20;
    pxGUIElementSub.Position.Left = 20;

    for(PXSize i = 0; i < pxDirectorySearchCache->EntryList.AmountUsed; ++i)
    {
        PXFileEntry* const pxFileEntry = PXListEntyrGetT(PXFileEntry, &pxDirectorySearchCache->EntryList, i);

        PXIcon pxIcon;

        const PXActionResult iconResult = PXGUIIconGetViaFilePath(&pxIcon, pxFileEntry->FilePathData);

#if OSUnix
#elif OSWindows
        HICON icon = 0;

        if(iconResult == PXActionSuccessful)
        {
            icon = pxIcon.Info.Handle.IconHandle;
        }
#endif
        /*

        switch(pxFileEntry->Type)
        {
            default:
            case PXFileElementInfoTypeFile:
            {
                icon = fileIcon;

        #if 0

                PXText pxTextA;
                PXTextConstructFromAdressA(&pxTextA, pxFileEntry->FilePathData, pxFileEntry->FilePathSize, pxFileEntry->FilePathSize);

                PXText pxTextB;
                PXTextConstructBufferA(&pxTextB, 64);

                PXSize extensionSize = PXFilePathExtensionGet(&pxTextA, &pxTextB);

                if(extensionSize != 0)
                {
                    HICON smsm = PXGUIIconGetViaExtension(pxTextB.TextA);

                    if(smsm)
                    {
                        icon = smsm;
                    }
                }
        #else



        #endif

                break;
            }
            case PXFileElementInfoTypeDictionary:
            {
                icon = folderIcon;
                break;
            }
        }
        */

#if OSUnix
#elif OSWindows
        DrawIconEx
        (
            pxGUIElementSub.DeviceContextHandle,
            pxGUIElementSub.Position.Left - (pxGUIElementSub.Position.Width + 19),
            pxGUIElementSub.Position.Top + 2,
            icon,
            16, // Width same as height
            16,
            0,
            0,
            DI_NORMAL
        );
#endif

        PXNativDrawTextA(pxGUISystem, &pxGUIElementSub, pxFileEntry->FilePathData, pxFileEntry->FilePathSize);

        pxGUIElementSub.Position.Top += 1 * pxGUIElementSub.Position.Height;
    }


    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUIIconGetSystem(PXIcon* const pxIcon, const int iconID)
{
    if(!pxIcon)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    SHSTOCKICONINFO stockIconInfo;
    PXClear(SHSTOCKICONINFO, &stockIconInfo);
    stockIconInfo.cbSize = sizeof(SHSTOCKICONINFO);

    HRESULT resultFile = SHGetStockIconInfo(SIID_DOCNOASSOC, SHGSI_ICON, &stockIconInfo);

    pxIcon->Info.Handle.IconHandle = 0;

    return PXActionSuccessful;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXGUIIconGetViaFilePath(PXIcon* const pxIcon, const char* fileName)
{
    if(!(pxIcon && fileName))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    SHFILEINFOA shFileInfo;
    ZeroMemory(&shFileInfo, sizeof(shFileInfo));

    const PXBool fetch = SHGetFileInfoA // Windows 2000, Shell32.dll 4.0, shellapi.h
    (
        fileName,
        0,
        &shFileInfo,
        sizeof(SHFILEINFOA),
        SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_SMALLICON | SHGFI_LARGEICON
    );

    pxIcon->Info.Handle.IconHandle = shFileInfo.hIcon;

    return PXActionSuccessful;


    /*




    char moduleName[MAX_PATH];
    PXInt16U moduleNameSize = 0;
    PXInt16U iconID = 0;

    char nameFile[MAX_PATH];
    DWORD nameFileSize = MAX_PATH;
    char objectName[MAX_PATH];
    DWORD dataSize = MAX_PATH;


    // Search file extension
    {
        char fileExtensionKey[MAX_PATH];

        PXTextPrintA(fileExtensionKey, MAX_PATH, ".%s", fileExtension);

        LSTATUS res = RegGetValueA(HKEY_CLASSES_ROOT, fileExtensionKey, NULL, RRF_RT_REG_SZ, NULL, objectName, &dataSize);
        PXErrorCurrent(ERROR_SUCCESS == res);

        PXConsoleWriteF(0, "\n");
    }

    // Search redirect
    {
        // Most programs dont put the default icon under the extension, so we need to follow a redirect

        LSTATUS res = RegGetValueA(HKEY_CLASSES_ROOT, objectName, NULL, RRF_RT_REG_SZ, NULL, nameFile, &nameFileSize);
        PXErrorCurrent(ERROR_SUCCESS == res);

        // Probe for default icon


        char defaultIconPath[MAX_PATH];

        PXTextPrintA(defaultIconPath, MAX_PATH, "%s\\%s", objectName, "DefaultIcon");

        char iconPath[MAX_PATH];
        DWORD iconPathSize = MAX_PATH;

        LSTATUS ressss = RegGetValueA(HKEY_CLASSES_ROOT, defaultIconPath, NULL, RRF_RT_REG_SZ, NULL, iconPath, &iconPathSize);
        PXErrorCurrent(ERROR_SUCCESS == res);


        // Unravel default icon string, format ""xxx", 0"
        const PXBool isFormat = iconPath[0] == '\"';
        PXSize comma = 0;
        char* text = 0;
        PXSize textSize = 0;

        char* iconTextData = 0;
        PXSize iconTextSize = 0;

        if(isFormat)
        {
            comma = PXTextFindLastCharacterA(iconPath, iconPathSize, ',');
            text = &iconPath[1];
            textSize = comma - 2;

            iconTextData = &iconPath[comma+1];
            iconTextSize = comma - textSize;

            iconID = PXTextToIntA(iconTextData, iconTextSize, &iconID);
        }

        moduleNameSize = PXTextCopyA(text, textSize, moduleName, MAX_PATH);


        PXConsoleWriteF(0, "\n");





    }


#if 1
    // SHGetFileInfoA() would make sense to be used but can crash the whole thread on error.
    SHFILEINFOA shFileInfo;
    ZeroMemory(&shFileInfo, sizeof(shFileInfo));


    const PXBool fetch = SHGetFileInfoA(moduleName, 0, &shFileInfo, sizeof(SHFILEINFOA), SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_SMALLICON | SHGFI_LARGEICON);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Icon-Fetch",
        "%s",
        shFileInfo.szDisplayName
    );
#endif

    return shFileInfo.hIcon; // Return the handle to the icon

#else
    LoadModule();
#endif


    */


#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

}

PXActionResult PXAPI PXWindowDragStart(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement)
{
#if OSUnix
#elif OSWindows
    HWND hwndParent = PXNull;
    HWND hwndFrom = PXNull;
    UINT fmt = PXNull;
    ULONG_PTR data = PXNull;
    HCURSOR hcur = PXNull;

    const DWORD aa = DragObject(hwndParent, hwndFrom, fmt, data, hcur);
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif

    //const BOOL res = DragDetect();

    return PXActionRefusedNotImplemented;
}

PXBool PXAPI PXWindowValueFetch(PXWindow* const pxUIElementList, const PXSize dataListAmount, const PXUIElementProperty pxUIElementProperty, void* const dataList)
{
    for(size_t i = 0; i < dataListAmount; ++i)
    {
        PXWindow* const pxGUIElement = &pxUIElementList[i];

        switch(pxUIElementProperty)
        {
            case PXUIElementPropertySliderPercentage:
            {
                float* target = &((float*)dataList)[i];

#if OSUnix

#elif OSWindows
                * target = SendMessageA(pxGUIElement->Info.Handle.WindowID, TBM_GETPOS, 0, 0) / 100.f;
#endif

                break;
            }

            default:
                break;
        }
    }

    return PXTrue;
}

void PXAPI PXGUIScreenGetSize(PXInt32S* const width, PXInt32S* const height)
{
#if OSUnix
    * width = 1200;
    *height = 800;

    // XRRGetScreenResources ?

#elif PXOSWindowsDestop
    RECT desktop;

    // Get a handle to the desktop window
    const HWND hDesktop = GetDesktopWindow();

    if(hDesktop)
    {
        // Get the size of screen to the variable desktop
        const BOOL rectResult = GetWindowRect(hDesktop, &desktop);

        // The top left corner will have coordinates (0,0)
        // and the bottom right corner will have coordinates
        // (horizontal, vertical)
        *width = desktop.right;
        *height = desktop.bottom;
    }
#else
    * width = -1;
    *height = -1;
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Monitor",
        "Size-Get",
        "%i x %i",
        *width,
        *height
    );
#endif
}

PXActionResult PXAPI PXGUISystemInitialize(PXGUISystem* const pxGUISystem)
{
    if(PXGUISystemGlobalReference)
    {
        return PXActionRefusedAlreadyInizialized;
    }

    PXGUISystemGlobalReference = pxGUISystem;

    // Enable multithreading
    {
#if OSUnix
        // This is only needed if we access the X-System with different threads
        // reason is timing collisions where zwo X-Calls are handled at the same time
        // This will leed to problems.
        // On systems where threads are not supported, this will always return NULL
        const int result = XInitThreads();

#elif OSWindows
        // Windows UI is not threadsafe
#endif
    }

    // Connect to display service
    PXNativDrawDisplayOpen(&pxGUISystem->NativDraw, &pxGUISystem->DisplayCurrent, PXNull);

       

#if OSWindows

    // Ensures that the common control DLL (Comctl32.dll) is loaded
    INITCOMMONCONTROLSEX initCommonControls;
    initCommonControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    initCommonControls.dwICC = ICC_TAB_CLASSES;
    InitCommonControlsEx(&initCommonControls);

#endif


    //  PXGUIDisplayScreenListRefresh(pxGUISystem);



    // Create brushes

    PXResourceCreateInfo pxResourceCreateInfoList[4];
    PXClearList(PXResourceCreateInfo, pxResourceCreateInfoList, 4);
    pxResourceCreateInfoList[0].ObjectReference = &pxGUISystem->BrushBackgroundDark;
    pxResourceCreateInfoList[0].Name = "BackgroundDark";
    pxResourceCreateInfoList[0].Type = PXResourceTypeBrush;
    pxResourceCreateInfoList[0].Brush.Color.Red = 100; // 30-30-30, 160, 40, 40
    pxResourceCreateInfoList[0].Brush.Color.Green = 40;
    pxResourceCreateInfoList[0].Brush.Color.Blue = 40;

    pxResourceCreateInfoList[1].ObjectReference = &pxGUISystem->BrushTextWhite;
    pxResourceCreateInfoList[1].Name = "TextWhite";
    pxResourceCreateInfoList[1].Type = PXResourceTypeBrush;
    pxResourceCreateInfoList[1].Brush.Color.Red = 0xff; // 200-200-200
    pxResourceCreateInfoList[1].Brush.Color.Green = 200;
    pxResourceCreateInfoList[1].Brush.Color.Blue = 200;

    pxResourceCreateInfoList[2].ObjectReference = &pxGUISystem->FontTitle;
    pxResourceCreateInfoList[2].Name = "FontTitle";
    pxResourceCreateInfoList[2].Type = PXResourceTypeFont;
    pxResourceCreateInfoList[3].Font.RegisteredName = "UniSpace";

    pxResourceCreateInfoList[3].ObjectReference = &pxGUISystem->FontContent;
    pxResourceCreateInfoList[3].Name = "FontContent";
    pxResourceCreateInfoList[3].Type = PXResourceTypeFont;
    pxResourceCreateInfoList[3].Font.RegisteredName = "Eras Medium ITC";

    PXResourceManagerAdd(pxGUISystem->ResourceManager, pxResourceCreateInfoList, 4);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGUISystemRelease(PXGUISystem* const pxGUISystem)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
    const int resultID = XCloseDisplay(pxGUISystem->DisplayCurrent.DisplayHandle);
    result = PXActionRefusedNotImplemented;
#elif OSWindows
    // TODO: ???
    result = PXActionRefusedNotImplemented;
#else
    result = PXnursupported;
#endif

    PXGUISystemGlobalReference = PXNull;

    return result;
}

PXActionResult PXAPI PXWindowStyleUpdate(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
    result = PXActionRefusedNotImplemented;
#elif OSWindows
    SetWindowLongPtr(pxGUIElement->Info.Handle.WindowID, GWL_STYLE, WS_SYSMENU); //3d argument=style
    const PXBool setSuccess = SetWindowPos(pxGUIElement->Info.Handle.WindowID, HWND_TOPMOST, 100, 100, 100, 100, SWP_SHOWWINDOW);
#else
    result = PXActionRefusedNotSupportedByLibrary;
#endif

    return result;
}

PXActionResult PXAPI PXWindowCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount)
{
    if(!(pxGUISystem && pxResourceCreateInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    //assert();

    PXWindowCreateInfo* pxGUIElementCreateInfo = &pxResourceCreateInfo->UIElement;
    PXWindow* pxWindowCurrent = *(PXWindow**)pxResourceCreateInfo->ObjectReference;

    pxWindowCurrent->Type = pxGUIElementCreateInfo->Type;
    pxWindowCurrent->InteractCallBack = pxGUIElementCreateInfo->InteractCallBack;
    pxWindowCurrent->InteractOwner = pxGUIElementCreateInfo->InteractOwner;
    pxWindowCurrent->Info.Hierarchy.Parrent = pxGUIElementCreateInfo->UIElementParent;
    pxWindowCurrent->Info.Behaviour = pxGUIElementCreateInfo->BehaviourFlags;
    pxWindowCurrent->BrushFront = pxGUISystem->BrushTextWhite;
    pxWindowCurrent->BrushBackground = pxGUISystem->BrushBackgroundDark;

    PXCopy(PXUIElementPosition, &pxGUIElementCreateInfo->Position, &pxWindowCurrent->Position);




    // Resize

    PXWindowPropertyInfo sizeInfoAA;
    PXClear(PXWindowPropertyInfo, &sizeInfoAA);
    //sizeInfoAA.UIElement = *pxGUIElementCreateInfo->UIElement;
    sizeInfoAA.WindowReference = pxGUIElementCreateInfo->UIElementWindow;
    sizeInfoAA.Property = PXUIElementPropertySizeParent;

    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

    // PXWindowSizeInfo pxWindowSizeInfo;

    // PXWindowID windowID = pxGUIElementCreateInfo->UIElementWindow ? pxGUIElementCreateInfo->UIElementWindow->ID : PXNull;

    //  PXWindowSizeGet(windowID, &pxWindowSizeInfo);

    PXWindowFetch(pxGUISystem, &sizeInfoAA, 1);

    pxUIElementPositionCalulcateInfo.WindowWidth = sizeInfoAA.Data.Size.Width;
    pxUIElementPositionCalulcateInfo.WindowHeight = sizeInfoAA.Data.Size.Height;

    PXUIElementPositionCalculcate(pxWindowCurrent, &pxUIElementPositionCalulcateInfo);


#if OSUnix

    pxGUIElementCreateInfo->Simple = PXFalse;
    pxGUIElementCreateInfo->X = 10;
    pxGUIElementCreateInfo->Y = 10;
    pxGUIElementCreateInfo->Width = pxUIElementPositionCalulcateInfo.Width / 2;
    pxGUIElementCreateInfo->Height = pxUIElementPositionCalulcateInfo.Height / 2;
    pxGUIElementCreateInfo->BorderWidth = 1;
    pxGUIElementCreateInfo->Border = 1;

    PXNativDrawWindowCreate(PXNativDrawInstantance(), pxWindowCurrent, pxGUIElementCreateInfo);


#if 0 // Grab means literally Drag%Drop grab. This is not mouse motion
    //bool   ret    = false;
    XID cursor = XCreateFontCursor(display, XC_crosshair);
    int    root = DefaultRootWindow(display);

    const int grabResult = XGrabPointer
    (
        display,
        root,
        0,
        ButtonMotionMask,
        GrabModeAsync,
        GrabModeAsync,
        root,
        None,
        CurrentTime
    );
#endif


    // Raw mouse movement
    XIEventMask eventmask;
    PXClear(XIEventMask, &eventmask);

    const PXSize maskLength = (XI_LASTEVENT + 7) / 8;
    char mask[maskLength];

    PXClearList(char, mask, maskLength);
    XISetMask(mask, XI_RawMotion);
    XISetMask(mask, XI_RawButtonPress);
    XISetMask(mask, XI_RawKeyPress);

    eventmask.deviceid = XIAllMasterDevices;
    eventmask.mask_len = maskLength;
    eventmask.mask = mask;


    const int selectResultID = XISelectEvents(pxGUISystem->DisplayCurrent.DisplayHandle, pxGUISystem->DisplayCurrent.WindowRootHandle, &eventmask, 1u);
    const PXBool success = PXNull != selectResultID;

    if(!success)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "X-System",
            "Event-Select",
            "Failed: Display:<%p>, Root:<%i>, Mask:%8x",
            pxGUISystem->DisplayCurrent.DisplayHandle,
            pxGUISystem->DisplayCurrent.WindowRootHandle,
            mask
        );
#endif
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "X-System",
            "Event-Select",
            "ID:%i OK",
            pxWindowCurrent->Info.Handle.WindowID
        );
#endif
    }





  




#elif OSWindows

    //  pxUIElementCreateData->CreationSkip = PXFalse;


    pxGUIElementCreateInfo->WindowsStyleFlags = WS_CLIPCHILDREN | SS_NOTIFY; // WS_CLIPSIBLINGS

    if(PXResourceInfoRender & pxGUIElementCreateInfo->BehaviourFlags && !pxGUIElementCreateInfo->Invisible)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_VISIBLE;
    }

    if(PXWindowBehaviourBorder & pxGUIElementCreateInfo->BehaviourFlags)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_BORDER;
    }

    if(pxGUIElementCreateInfo->UIElementParent)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_CHILD;
    }

    if(PXWindowBehaviourSelectable & pxGUIElementCreateInfo->BehaviourFlags)
    {
        pxGUIElementCreateInfo->WindowsStyleFlags |= WS_TABSTOP;
    }

    switch(pxWindowCurrent->Type)
    {
        case PXUIElementTypePanel:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;

            pxGUIElementCreateInfo->WindowsStyleFlags |= SS_BLACKFRAME;

            break;
        }
        case PXUIElementTypeText:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomText;

            /*
            PXUIElementTextInfo* const pxUIElementTextInfo = &pxGUIElementCreateInfo->Data.Text;

            pxGUIElementCreateInfo->WindowsTextContent = pxUIElementTextInfo->Content;

            switch(pxUIElementTextInfo->Allign)
            {
                case PXUIElementTextAllignLeft:
                    pxGUIElementCreateInfo->WindowsStyleFlags |= SS_LEFT;
                    break;

                case PXUIElementTextAllignRight:
                    pxGUIElementCreateInfo->WindowsStyleFlags |= SS_RIGHT;
                    break;

                default:
                case PXUIElementTextAllignCenter:
                    pxGUIElementCreateInfo->WindowsStyleFlags |= SS_CENTER;
                    break;
            }
            */

            PXBool hasParenet = 0;// pxGUIElementCreateInfo->UIElementParent;

            /*
            if(hasParenet)
            {
                pxGUIElementCreateInfo->AvoidCreation = PXUIElementTypeButton == pxGUIElement->Parent->Type;

                if(pxGUIElementCreateInfo->AvoidCreation)
                {
                    pxGUIElement->Type = PXUIElementTypeButtonText;
                    pxGUIElement->Info.Handle.WindowID = PXNull;
                    //return;
                }
            }
            */

            break;
        }
        case PXUIElementTypeButton:
        {
            // BS_DEFPUSHBUTTON
            // BS_USERBUTTON
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomButton;
            pxGUIElementCreateInfo->WindowsTextContent = pxGUIElementCreateInfo->Data.Button.TextInfo.Content;
            break;
        }
        case PXUIElementTypeImage:
        {
            // pxUIElementCreateData->WindowsClassName = WC_IMAGE;
            //pxUIElementCreateData->WindowsStyleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeDropDown:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_COMBOBOX;
            pxGUIElementCreateInfo->WindowsStyleFlags |= CBS_HASSTRINGS | CBS_DROPDOWNLIST | WS_OVERLAPPED;
            break;
        }
        case PXUIElementTypeListBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_LISTBOX;
            break;
        }
        case PXUIElementTypeTextEdit:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_EDIT;
            pxGUIElementCreateInfo->WindowsStyleFlags |= ES_MULTILINE;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomText;
            pxGUIElementCreateInfo->WindowsTextContent = pxGUIElementCreateInfo->Data.Button.TextInfo.Content;
            break;
        }
        case PXUIElementTypeRichEdit:
        {
            pxGUIElementCreateInfo->WindowsClassName = "RICHEDIT_CLASS";
            break;
        }
        case PXUIElementTypeScrollBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_SCROLLBAR;
            break;
        }
        case PXUIElementTypeTrackBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = TRACKBAR_CLASS;
            pxGUIElementCreateInfo->WindowsStyleFlags |= TBS_AUTOTICKS | TBS_TOOLTIPS;
            break;
        }
        case PXUIElementTypeStatusBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = STATUSCLASSNAMEA;
            //pxGUIElementCreateInfo->WindowsStyleFlags |= SBARS_SIZEGRIP;


            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomFooter;

            pxWindowCurrent->Info.Behaviour &= ~PXResourceInfoUseByMask;
            pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;

            break;
        }
        case PXUIElementTypeUpDown:
        {
            pxGUIElementCreateInfo->WindowsClassName = UPDOWN_CLASS;
            break;
        }
        case PXUIElementTypeProgressBar:
        {
            pxGUIElementCreateInfo->WindowsClassName = PROGRESS_CLASS;
            break;
        }
        case PXUIElementTypeHotKey:
        {
            pxGUIElementCreateInfo->WindowsClassName = HOTKEY_CLASS;
            break;
        }
        case PXUIElementTypeCalender:
        {
            pxGUIElementCreateInfo->WindowsClassName = MONTHCAL_CLASS;
            break;
        }
        case PXUIElementTypeToolTip:
        {
            pxGUIElementCreateInfo->WindowsClassName = TOOLTIPS_CLASS;
            break;
        }
        case PXUIElementTypeAnimate:
        {
            pxGUIElementCreateInfo->WindowsClassName = ANIMATE_CLASS;
            break;
        }
        case PXUIElementTypeDatePicker:
        {
            pxGUIElementCreateInfo->WindowsClassName = DATETIMEPICK_CLASS;
            break;
        }
        case PXUIElementTypeGroupBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_GROUPBOX;
            break;
        }
        case PXUIElementTypeRadioButton:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_RADIOBUTTON;
            break;
        }
        case PXUIElementTypeGroupRadioButton:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_AUTORADIOBUTTON;
            break;
        }
        case PXUIElementTypeTreeView:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_TREEVIEW;
            pxGUIElementCreateInfo->WindowsStyleFlags |=
                TVIF_TEXT |
                TVIF_IMAGE |
                TVIF_SELECTEDIMAGE |
                TVIF_PARAM | // Required to get the a selected item, otherwise its just NULL.
                TVS_HASBUTTONS |
                TVS_HASLINES |
                TVS_LINESATROOT;
            break;
        }
        case PXUIElementTypeFileDirectyView:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawFileDirectoryView;

            pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;
            break;
        }
        case PXUIElementTypeIPInput:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_IPADDRESS;
            break;
        }
        case PXUIElementTypeLink:
        {
            pxGUIElementCreateInfo->WindowsClassName = "SysLink"; // WC_LINK
            break;
        }
        case PXUIElementTypeHeader:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_HEADER;
            break;
        }
        case PXUIElementTypeFontSelector:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_NATIVEFONTCTL;
            // NFS_USEFONTASSOC
            break;
        }
        case PXUIElementTypePageScroll:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_PAGESCROLLER;
            break;
        }
        case PXUIElementTypeTabControll:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_TABCONTROL;
            //pxGUIElementCreateInfo->WindowsStyleFlags |= WS_CLIPSIBLINGS | TCS_BUTTONS;

            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomTabList;

            pxWindowCurrent->Info.Behaviour &= ~PXResourceInfoUseByMask;
            pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;
            break;
        }
        case PXUIElementTypeTabPage:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            break;
        }
        case PXUIElementTypeToggle:
        {
            pxGUIElementCreateInfo->WindowsClassName = 00000000000000000000;
            pxGUIElementCreateInfo->WindowsStyleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeColorPicker:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomColorPicker;
            //pxGUIElementCreateInfo->WindowsStyleFlags |= SS_WHITEFRAME;
            break;
        }
        case PXUIElementTypeSlider:
        {
            pxGUIElementCreateInfo->WindowsClassName = 00000000000000000000;
            pxGUIElementCreateInfo->WindowsStyleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeCheckBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_BUTTON;
            pxGUIElementCreateInfo->WindowsStyleFlags |= BS_CHECKBOX;

            //CheckDlgButton(uiCheckBox->ID, 1, BST_CHECKED); // BST_UNCHECKED

            break;
        }
        case PXUIElementTypeComboBox:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_COMBOBOXEX;
            break;
        }
        case PXUIElementTypeRenderFrame:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->WindowsStyleFlags |= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
            break;
        }
        case PXUIElementTypeResourceManger:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomResourceView;

            pxWindowCurrent->Info.Behaviour &= ~PXResourceInfoUseByMask;
            pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;

            break;
        }
        case PXUIElementTypeResourceInfo:
        {
            pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
            pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomResourceInfo;

            pxWindowCurrent->Info.Behaviour &= ~PXResourceInfoUseByMask;
            pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;

            break;
        }
        case PXUIElementTypeWindow:
        {
            PXWindowCreateWindowInfo* const windowInfo = &pxGUIElementCreateInfo->Data.Window;

            pxGUIElementCreateInfo->WindowsWindowsStyleFlagsExtended = WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME | WS_EX_CONTEXTHELP;
            pxGUIElementCreateInfo->WindowsClassName = "PXWindow";
            pxGUIElementCreateInfo->WindowsStyleFlags |=
                WS_OVERLAPPEDWINDOW |
                CS_OWNDC |
                CS_HREDRAW |
                CS_VREDRAW;

            PXNativDrawWindowHandle windowID = 0;

            DWORD dwStyle = 0;
            HWND hWndParent = 0;
            HMENU hMenu = 0;
            void* lpParam = 0;

            // HICON       hIcon =
            // HCURSOR     hCursor = pxWindow->CursorID;
            // HBRUSH      hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1); //(HBRUSH)GetStockObject(COLOR_BACKGROUND);

            //  HBRUSH hbrBackground =

            /*
            // Cursor setup
            {
               // const HCURSOR cursorID = LoadCursor(hInstance, IDC_ARROW);
                //pxWindow->CursorID = cursorID;

                if(pxWindow->Mode == PXWindowModeNormal)
                {
                    const int xx = WS_OVERLAPPEDWINDOW;

                    dwStyle |= WS_OVERLAPPEDWINDOW;// | WS_VISIBLE;
                }
            }
            */

            // Registering of class
            {
                WNDCLASSA wndclass;

                PXClear(WNDCLASSA, &wndclass);

                wndclass.style = 0; // Can't apply
                wndclass.lpfnWndProc = PXNativDrawEventReceiver;
                wndclass.cbClsExtra = 0;
                wndclass.cbWndExtra = 0;
                wndclass.hInstance = GetModuleHandleA(NULL);
                wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
                wndclass.hCursor = PXNull;
                wndclass.hbrBackground = CreateSolidBrush(RGB(windowInfo->BackGroundColor.Red, windowInfo->BackGroundColor.Green, windowInfo->BackGroundColor.Blue));
                wndclass.lpszMenuName = 0;
                wndclass.lpszClassName = "PXWindow";


                const ATOM classID = RegisterClassA(&wndclass);
                const PXBool success = 0 != classID;

                if(!success)
                {
                    const DWORD errorID = GetLastError();
                    const PXActionResult result = PXWindowsHandleErrorFromID(errorID);
                }

                pxGUIElementCreateInfo->WindowsClassName = (char*)classID;
            }

            // Calc size
            {
                const PXBool isDefaultSize = 1;// windowInfo->Width == 0 && windowInfo->Height == 0;

                if(isDefaultSize)
                {
                    PXInt32S screenWidth = 0;
                    PXInt32S screenHeight = 0;

                    PXGUIScreenGetSize(&screenWidth, &screenHeight);

                    pxUIElementPositionCalulcateInfo.X = screenWidth * 0.125f;
                    pxUIElementPositionCalulcateInfo.Y = screenHeight * 0.125f;
                    pxUIElementPositionCalulcateInfo.Width = screenWidth * 0.75f;
                    pxUIElementPositionCalulcateInfo.Height = screenHeight * 0.75f;
                }
            }

            break;
        }
        case PXUIElementTypeMenuStrip:
        {
            if(1)
            {
                pxGUIElementCreateInfo->WindowsClassName = WC_STATIC;
                pxGUIElementCreateInfo->DrawFunctionEngine = PXWindowDrawCustomHeader;

                pxWindowCurrent->Info.Behaviour &= ~PXResourceInfoUseByMask;
                pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;
            }
            else
            {
                pxGUIElementCreateInfo->AvoidCreation = PXTrue;
            }

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            pxGUIElementCreateInfo->AvoidCreation = PXTrue;
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }


    // If we hav
    {
        switch(PXResourceInfoUseByMask & pxWindowCurrent->Info.Behaviour)
        {
            case PXResourceInfoUseByOS:
            {
                // Do nothing
                break;
            }
            case PXResourceInfoUseByUser:
            {
                pxWindowCurrent->DrawFunction = pxGUIElementCreateInfo->DrawFunctionEngine;
                pxWindowCurrent->BrushBackground = pxGUISystem->BrushBackgroundDark;
                break;
            }
            case PXResourceInfoUseByEngine:
            {
                if(pxGUIElementCreateInfo->DrawFunctionEngine)
                {
                    pxWindowCurrent->DrawFunction = pxGUIElementCreateInfo->DrawFunctionEngine;
                }
                else
                {
                    pxWindowCurrent->DrawFunction = PXWindowDrawCustomFailback;
                }

                break;
            }
            default:
            {

                break;
            }
        }

        if(pxWindowCurrent->DrawFunction)
        {
            DWORD magicID = 0;

            switch(pxWindowCurrent->Type)
            {
                case PXUIElementTypeButton:
                    magicID = BS_OWNERDRAW;
                    break;

                case PXUIElementTypeRenderFrame:
                case PXUIElementTypeColorPicker:
                case PXUIElementTypePanel:
                default: // TODO: problem with default value, we cant detect if we have the wrong enum type
                    magicID = SS_OWNERDRAW;
                    break;
            }

            pxGUIElementCreateInfo->WindowsStyleFlags |= magicID;
        }
    }



    // If we a text that should be rendered on a button, the OS does not really intent to do this.
    // To avoid wierd graphical bugs, we will merge these into one object.
    // Still this object needs to be seperate for other render systems
    // pxGUIElement->Type == PXUIElementTypeText&&;


    if(!pxGUIElementCreateInfo->AvoidCreation)
    {
        HINSTANCE hInstance = PXNull;
        PXNativDrawWindowHandle windowID = pxGUIElementCreateInfo->UIElementWindow ? pxGUIElementCreateInfo->UIElementWindow->Info.Handle.WindowID : PXNull;

        if(pxGUIElementCreateInfo->UIElementWindow)
        {
            hInstance = (HINSTANCE)GetWindowLongPtr(pxGUIElementCreateInfo->UIElementWindow->Info.Handle.WindowID, GWLP_HINSTANCE);
            //windowID = pxGUIElementCreateInfo->UIElementWindow->ID;
        }
        else
        {
            hInstance = GetModuleHandle(NULL);
        }

        // const HINSTANCE hInstance = GetModuleHandle(NULL);



        PXWindowCreateInfo pxWindowCreateInfo;
        PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
        pxWindowCreateInfo.ParentID = windowID;
        pxWindowCreateInfo.InstanceHandle = hInstance;
        pxWindowCreateInfo.X = pxUIElementPositionCalulcateInfo.X;
        pxWindowCreateInfo.Y = pxUIElementPositionCalulcateInfo.Y;
        pxWindowCreateInfo.Width = pxUIElementPositionCalulcateInfo.Width;
        pxWindowCreateInfo.Height = pxUIElementPositionCalulcateInfo.Height;
        pxWindowCreateInfo.BorderWidth = 2;
        pxWindowCreateInfo.Border = 2;
        pxWindowCreateInfo.WindowsWindowsStyleFlagsExtended = pxGUIElementCreateInfo->WindowsWindowsStyleFlagsExtended;
        pxWindowCreateInfo.WindowsStyleFlags = pxGUIElementCreateInfo->WindowsStyleFlags;
        pxWindowCreateInfo.WindowsTextContent = pxGUIElementCreateInfo->WindowsTextContent;
    //    pxNativDrawWindow.WindowsTextSize;
        pxWindowCreateInfo.WindowsClassName = pxGUIElementCreateInfo->WindowsClassName;

        const PXActionResult pxActionResult = PXNativDrawWindowCreate(PXNativDrawInstantance(), pxWindowCurrent, &pxWindowCreateInfo);


        if(PXActionSuccessful != pxActionResult)
        {
            // Failed to create window

#if PXLogEnable
            const char* uielementName = PXUIElementTypeToString(pxGUIElementCreateInfo->Type);

            PXLogPrint
            (
                PXLoggingError,
                "GUI",
                "Element-Create",
                "Failed: X:%4i, Y:%4i, W:%4i, H:%4i, (%s) : [%s]",
                (int)pxUIElementPositionCalulcateInfo.X,
                (int)pxUIElementPositionCalulcateInfo.Y,
                (int)pxUIElementPositionCalulcateInfo.Width,
                (int)pxUIElementPositionCalulcateInfo.Height,
                uielementName,
                pxGUIElementCreateInfo->WindowsTextContent
            );
#endif

            return pxActionResult;
        }

        pxWindowCurrent->Info.Flags |= PXResourceInfoExist | PXResourceInfoActive | PXResourceInfoRender | PXResourceInfoStorageDevice;

        // Get additional device context for rendering purpose
        pxWindowCurrent->DeviceContextHandle = GetDC(pxWindowCurrent->Info.Handle.WindowID);

        // Gegister drag&Drop
        IDropTarget dropTarget;

        const HRESULT dragResult = RegisterDragDrop(pxWindowCurrent->Info.Handle.WindowID, &dropTarget); // Windows 2000, Ole32.dll, ole2.h


        PXDictionaryAdd(&pxGUISystem->ResourceManager->GUIElementLookup, &pxWindowCurrent->Info.Handle.WindowID, pxWindowCurrent);

#if PXLogEnable
        const char* uielementName = PXUIElementTypeToString(pxGUIElementCreateInfo->Type);

        PXLogPrint
        (
            PXLoggingInfo,
            "GUI",
            "Element-Create",
            "X:%4i Y:%4i W:%4i H:%4i %12s : %s", // 0x%p
            (int)pxUIElementPositionCalulcateInfo.X,
            (int)pxUIElementPositionCalulcateInfo.Y,
            (int)pxUIElementPositionCalulcateInfo.Width,
            (int)pxUIElementPositionCalulcateInfo.Height,
            // pxGUIElement->Info.Handle.WindowID,
            uielementName,
            pxGUIElementCreateInfo->WindowsTextContent
        );
#endif





#if 0
        if(pxGUIElement->ID && pxGUIElementCreateInfo->UIElementParent)
        {
            PXWindow* parent = pxGUIElementCreateInfo->UIElementParent;

            HDWP resA = BeginDeferWindowPos(1);

            if(PXUIElementTypeButtonText == pxGUIElement->Type)
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    parent->ID,
                    pxGUIElement->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );
                EndDeferWindowPos(resB);
            }
            else
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    pxGUIElement->ID,
                    parent->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );
                EndDeferWindowPos(resB);
            }


        }
#endif



    }




#if 0

    char buffer[255];
    PXClearList(char, buffer, 255);

    PXSize* size = buffer;
    *size = 255;

    COLORREF colorAA = RGB(255, 0, 0);

    HDC xx = GetDC(pxGUIElement->ID);

    HBRUSH brush = SendMessageA(windowHandle, WM_CTLCOLORSTATIC, xx, pxGUIElement->ID); // RB_SETBKCOLOR
    SetTextColor(xx, RGB(255, 0, 0));
    SetBkColor(xx, RGB(0, 255, 0));


    Pager_SetBkColor(pxGUIElement->ID, &colorAA);
#endif


    // SetTextColor(xx, colorAA);


    //LRESULT ww = SendMessageA(pxGUIElement->ID, EM_GETLINE, 0, buffer); // RB_SETBKCOLOR


    //  PXConsoleWriteA("\n\n%i     %i %s", brush, ww, buffer);


    // SendMessageA(pxGUIElement->ID, RB_SETTEXTCOLOR, 0, &colorAA);




#endif












    //-------------------------------------------------------------------------
    // POST-Update
    //-------------------------------------------------------------------------
    switch(pxWindowCurrent->Type)
    {
        case PXUIElementTypeMenuStrip:
        {
            PXWindowMenuItemList* const menuItemListInput = &pxGUIElementCreateInfo->Data.MenuItem;

            PXWindowExtendedMenuItem* menuItemListOut = PXMemoryCallocT(PXWindowExtendedMenuItem, 1);

            pxWindowCurrent->ExtendedData = menuItemListOut;

            // setup extended data
            {
                menuItemListOut->MenuItemAmount = menuItemListInput->MenuItemInfoListAmount;
                menuItemListOut->MenuItemList = PXMemoryCallocT(PXWindowMenuItem, menuItemListInput->MenuItemInfoListAmount);

                for(PXSize i = 0; i < menuItemListInput->MenuItemInfoListAmount; ++i)
                {
                    PXWindowMenuItemInfo* const pxWindowMenuItemSource = &menuItemListInput->MenuItemInfoListData[i];
                    PXWindowMenuItem* const pxWindowMenuItemTarget = &menuItemListOut->MenuItemList[i];

                    pxWindowMenuItemTarget->TextData = pxWindowMenuItemSource->TextData;
                    pxWindowMenuItemTarget->TextSize = pxWindowMenuItemSource->TextSize;
                }
            }


            switch(PXResourceInfoUseByMask & pxWindowCurrent->Info.Behaviour)
            {
                case PXResourceInfoUseByOS:
                {

#if OSWindows
                    pxWindowCurrent->Info.Handle.MenuHandle = CreateMenu(); // Windows 2000, User32.dll, winuser.h
                    const PXBool succeess = PXNull != pxWindowCurrent->Info.Handle.MenuHandle;
                    const PXActionResult res = PXErrorCurrent(succeess);



                    HMENU hSubMenu = CreatePopupMenu();


                    for(PXSize i = 0; i < menuItemListInput->MenuItemInfoListAmount; ++i)
                    {
                        PXWindowMenuItemInfo* const pxGUIElementMenuItemInfo = &menuItemListInput->MenuItemInfoListData[i];

                        MENUITEMINFOA menuItemInfo;
                        PXClear(MENUITEMINFOA, &menuItemInfo);
                        menuItemInfo.cbSize = sizeof(MENUITEMINFOA);
                        menuItemInfo.fMask = MIIM_STRING | MIIM_STATE | MIIM_SUBMENU;
                        menuItemInfo.fType = MFT_STRING;         // used if MIIM_TYPE (4.0) or MIIM_FTYPE (>4.0)
                        menuItemInfo.fState = MFS_DEFAULT;        // used if MIIM_STATE
                        menuItemInfo.wID = 0;           // used if MIIM_ID
                        menuItemInfo.hSubMenu = pxWindowCurrent->Info.Handle.MenuHandle;      // used if MIIM_SUBMENU
                        menuItemInfo.hbmpChecked = 0;   // used if MIIM_CHECKMARKS
                        menuItemInfo.hbmpUnchecked = 0; // used if MIIM_CHECKMARKS
                        menuItemInfo.dwItemData = 0;   // used if MIIM_DATA
                        menuItemInfo.dwTypeData = pxGUIElementMenuItemInfo->TextData;    // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
                        menuItemInfo.cch = pxGUIElementMenuItemInfo->TextSize;           // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
                        menuItemInfo.hbmpItem = 0;      // used if MIIM_BITMAP

                        UINT newID = i;

                        const PXBool itemAddResult = AppendMenuA(pxWindowCurrent->Info.Handle.MenuHandle, menuItemInfo.fState, &newID, menuItemInfo.dwTypeData);
                        // const PXBool itemAddResult = AppendMenuA(pxGUIElement->Info.Handle.MenuHandle, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, menuItemInfo.dwTypeData);


                        const PXActionResult sdfsdfghg = PXErrorCurrent(itemAddResult);

                        PXWindowMenuItemList* sub = pxGUIElementMenuItemInfo->ChildList;

                        if(sub)
                        {
                            for(size_t i = 0; i < sub->MenuItemInfoListAmount; i++)
                            {
                                UINT newIEED = newID;

                                PXWindowMenuItemInfo* const pxGUIElementMenuItemInfo = &sub->MenuItemInfoListData[i];

                                const PXBool asddassdad = AppendMenuA(hSubMenu, menuItemInfo.fState | MF_POPUP, &newIEED, pxGUIElementMenuItemInfo->TextData);
                                const PXActionResult wewrerew = PXErrorCurrent(asddassdad);
                            }
                        }




                        //  const PXBool itemAddResult = InsertMenuItemA(pxGUIElement->Info.Handle.MenuHandle, 0, PXTrue, &menuItemInfo);
                        const PXActionResult res = PXErrorCurrent(itemAddResult);

                        // DrawMenuBar(pxGUIElement->Parent->Info.Handle.WindowID);

                        menuItemInfo.wID += 0;
                    }

                    PXWindow* parentElement = (PXWindow*)pxWindowCurrent->Info.Hierarchy.Parrent;
                    const PXBool setResult = SetMenu(parentElement->Info.Handle.WindowID, pxWindowCurrent->Info.Handle.MenuHandle);
#endif

                    break;
                }
                case PXResourceInfoUseByEngine:
                {

                    // ToDo:

                    break;
                }

                default:
                    break;
            }

            break;
        }

        case PXUIElementTypePanel:
        {


            break;
        }
        case PXUIElementTypeFileDirectyView:
        {
            PXDirectorySearchCache* pxDirectorySearchCache = PXMemoryCallocT(PXDirectorySearchCache, 1);

            pxWindowCurrent->ExtendedData = pxDirectorySearchCache;

            break;
        }
        case PXUIElementTypeText:
        {
            PXWindowPropertyInfo pxUIElementUpdateInfo[2];
            PXClearList(PXWindowPropertyInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxWindowCurrent;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;

            pxUIElementUpdateInfo[1].UIElement = pxWindowCurrent;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyTextAllign;

            // PXWindowUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeButton:
        {
            PXWindowPropertyInfo pxUIElementUpdateInfo[2];
            PXClearList(PXWindowPropertyInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxWindowCurrent;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElement = pxWindowCurrent;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;


            //  PXWindowUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            PXUIElementTreeViewItemInfo* const pxUIElementTreeViewItemInfo = &pxGUIElementCreateInfo->Data.TreeViewItem;
            // Create ui item for a tree view

#if OSUnix
#elif OSWindows

            TVINSERTSTRUCT item;
            PXClear(TVINSERTSTRUCT, &item);

            // item.item.iImage = 1;


            const char text[] = "[N/A]";

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                item.hParent = pxUIElementTreeViewItemInfo->ItemParent->Info.Handle.WindowID;
            }

            if(pxUIElementTreeViewItemInfo->TextDataOverride)
            {
                item.item.pszText = pxUIElementTreeViewItemInfo->TextDataOverride;
                item.item.cchTextMax = pxUIElementTreeViewItemInfo->TextSizeOverride;
            }
            else
            {
                item.item.pszText = pxGUIElementCreateInfo->WindowsTextContent;
                item.item.cchTextMax = pxGUIElementCreateInfo->WindowsTextSize;
            }



            if(PXResourceTypeGUIElement == pxUIElementTreeViewItemInfo->OwningObjectType)
            {
                PXWindow* const uiElement = (PXWindow*)pxUIElementTreeViewItemInfo->OwningObject;

                switch(uiElement->Type)
                {
                    // case PXUIElementTypeRenderFrame:


                    case PXUIElementTypeWindow:
                        item.item.iImage = 0;
                        break;
                    case PXUIElementTypeButton:
                        item.item.iImage = 1;
                        break;
                    case PXUIElementTypeTextEdit:
                        item.item.iImage = 2;
                        break;
                    case PXUIElementTypeRenderFrame:
                        item.item.iImage = 3;
                        break;
                    case PXUIElementTypeText:
                        item.item.iImage = 5;
                        break;
                    case PXUIElementTypeTreeView:
                        item.item.iImage = 4;
                        break;
                    case PXUIElementTypePanel:
                        item.item.iImage = 6;
                        break;

                        // case PXUIElementTypeWindow:                    item.item.iImage = 1;                    break;
                        // case PXUIElementTypeWindow:                    item.item.iImage = 1;                    break;



                    default:
                        item.item.iImage = -1;
                        break;
                }
            }

            item.item.iSelectedImage = item.item.iImage;

            switch(pxUIElementTreeViewItemInfo->InsertMode)
            {
                case PXUIElementTreeViewItemInsertModeROOT:
                    item.hInsertAfter = TVI_ROOT;
                    break;

                case PXUIElementTreeViewItemInsertModeFIRST:
                    item.hInsertAfter = TVI_FIRST;
                    break;

                case PXUIElementTreeViewItemInsertModeLAST:
                    item.hInsertAfter = TVI_LAST;
                    break;

                case PXUIElementTreeViewItemInsertModeSORT:
                    item.hInsertAfter = TVI_SORT;
                    break;
            }

            item.item.mask = TVIF_TEXT | TVIF_IMAGE;

            HTREEITEM itemID = TreeView_InsertItem(pxUIElementTreeViewItemInfo->TreeView->Info.Handle.WindowID, &item);

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                TreeView_Expand
                (
                    pxUIElementTreeViewItemInfo->TreeView->Info.Handle.WindowID,
                    pxUIElementTreeViewItemInfo->ItemParent->Info.Handle.WindowID,
                    TVE_EXPAND
                );
            }

            pxWindowCurrent->Info.Handle.WindowID = itemID;
            pxUIElementTreeViewItemInfo->ItemHandle = itemID;
#endif

            break;
        }
        case PXUIElementTypeImage:
        {

            break;
        }
        case PXUIElementTypeDropDown:
        {

            break;
        }
        case PXUIElementTypeListBox:
        {

            break;
        }
        case PXUIElementTypeResourceManger:
        {
            pxWindowCurrent->ExtendedData = PXMemoryCallocT(PXWindowExtendedBehaviourResourceView, 1);



            break;
        }
        case PXUIElementTypeTextEdit:
        {




            break;
        }
        case PXUIElementTypeRichEdit:
        {


            // EM_SETBKGNDCOLOR(pxGUIElement->ID, RGB(10, 10, 10));
            // EM_SETTEXTCOLOR(pxGUIElement->ID, RGB(200, 200, 200));

            break;
        }
        case PXUIElementTypeScrollBar:
        {

            break;
        }
        case PXUIElementTypeTrackBar:
        {
#if OSUnix
#elif OSWindows

            HDC dc = GetDC(pxWindowCurrent->Info.Handle.WindowID);


            SetTextColor(dc, RGB(255, 0, 0));
            SetBkColor(dc, RGB(0, 0, 255));


            SendMessageA(pxWindowCurrent->Info.Handle.WindowID, TBM_SETTIPSIDE, TBTS_RIGHT, PXNull);


            // SendMessageA(pxGUIElement->ID, SET_BACKGROUND_COLOR, RGB(30, 30, 30), RGB(30, 30, 30));

            // SET_BACKGROUND_COLOR;

            // TreeView_SetBkColor(pxGUIElement->ID, RGB(30, 30, 30));

            //   TreeView_SetTextColor(pxGUIElement->ID, RGB(200, 200, 200));
#endif

            break;
        }
        case PXUIElementTypeStatusBar:
        {

            break;
        }
        case PXUIElementTypeUpDown:
        {

            break;
        }
        case PXUIElementTypeProgressBar:
        {
            PXWindowPropertyInfo pxUIElementUpdateInfo[2];
            PXClearList(PXWindowPropertyInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].UIElement = pxWindowCurrent;
            pxUIElementUpdateInfo[0].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElement = pxWindowCurrent;
            pxUIElementUpdateInfo[1].WindowReference = pxGUIElementCreateInfo->UIElementParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;

            PXNativDrawWindowProperty(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeHotKey:
        {

            break;
        }
        case PXUIElementTypeCalender:
        {

            break;
        }
        case PXUIElementTypeToolTip:
        {

            break;
        }
        case PXUIElementTypeAnimate:
        {

            break;
        }
        case PXUIElementTypeDatePicker:
        {

            break;
        }
        case PXUIElementTypeGroupBox:
        {

            break;
        }
        case PXUIElementTypeRadioButton:
        {

            break;
        }
        case PXUIElementTypeGroupRadioButton:
        {

            break;
        }
        case PXUIElementTypeTreeView:
        {
#if OSUnix
#elif OSWindows

            if(0)
            {



            }
            else
            {


                int sizeX = 16;
                int sizeY = 16;
                int amount = 11;

                TreeView_SetBkColor(pxWindowCurrent->Info.Handle.WindowID, RGB(30, 30, 30));

                TreeView_SetTextColor(pxWindowCurrent->Info.Handle.WindowID, RGB(200, 200, 200));


                // Add icons
                HIMAGELIST imageListHandle = ImageList_Create
                (
                    sizeX,
                    sizeY,
                    ILC_COLOR,
                    amount,
                    amount
                );


                HMODULE currentModule = GetModuleHandle(NULL);


                const char* pathList[11] =
                {
                    "Texture/Window.bmp",
                    "Texture/Button.bmp",
                    "Texture/Edit.bmp",
                    "Texture/FrameBuffer.bmp",
                    "Texture/TreeView.bmp",
                    "Texture/Text.bmp",
                    "Texture/Panel.bmp",
                    "Texture/Folder.bmp",
                    "Texture/H.bmp",
                    "Texture/HPP.bmp",
                    "Texture/SquareBlue.bmp"
                };


                for(size_t i = 0; i < amount; i++)
                {
                    // HBITMAP bitmapHandle = LoadBitmapA(PXNull, OBM_CLOSE);

                    const char* filePath = pathList[i];

                    HBITMAP bitmapHandle = LoadImageA
                    (
                        PXNull,
                        filePath,
                        IMAGE_BITMAP,
                        sizeX,
                        sizeY,
                        LR_LOADFROMFILE
                    );

                    int addedID = ImageList_Add(imageListHandle, bitmapHandle, PXNull);

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        "GUI",
                        "ImageList-Add",
                        "New icon %i",
                        addedID
                    );
                }

                TreeView_SetImageList(pxWindowCurrent->Info.Handle.WindowID, imageListHandle, TVSIL_NORMAL);
            }
#endif

            break;
        }
        case PXUIElementTypeIPInput:
        {

            break;
        }
        case PXUIElementTypeLink:
        {

            break;
        }
        case PXUIElementTypeHeader:
        {

            break;
        }
        case PXUIElementTypeFontSelector:
        {

            break;
        }
        case PXUIElementTypePageScroll:
        {

            break;
        }
        case PXUIElementTypeTabControll:
        {
#if OSUnix
#elif OSWindows

            PXUIElementTabPageInfo* const pxUIElementTabPageInfo = &pxGUIElementCreateInfo->Data.TabPage;


            // Create space for extended data
            PXWindowExtendedBehaviourTab* pxWindowExtendedBehaviourTab = PXMemoryCallocT(PXWindowExtendedBehaviourTab, 1);
            pxWindowCurrent->ExtendedData = pxWindowExtendedBehaviourTab;

            // Fill extended data
            pxWindowExtendedBehaviourTab->TABPageIndexCurrent = -1;
            pxWindowExtendedBehaviourTab->TABPageAmount = pxGUIElementCreateInfo->Data.TabPage.TabPageSingleInfoAmount;
            pxWindowExtendedBehaviourTab->TABPageList = PXMemoryCallocT(PXWindow, pxGUIElementCreateInfo->Data.TabPage.TabPageSingleInfoAmount);


            PXIcon* pxIcon = PXMemoryCallocT(PXIcon, 1);

            PXNativDrawLoad(pxGUISystem, pxIcon, 0);

            for(PXSize i = 0; i < pxUIElementTabPageInfo->TabPageSingleInfoAmount; ++i)
            {
                PXWindow* const pxWindow = &pxWindowExtendedBehaviourTab->TABPageList[i];

                PXUIElementTabPageSingleInfo* const pxUIElementTabPageSingleInfo = &pxUIElementTabPageInfo->TabPageSingleInfoList[i];
                *pxUIElementTabPageSingleInfo->UIElement = pxWindow; // Store reference to have it for the caller

                pxWindow->NameContent = pxUIElementTabPageSingleInfo->PageName;
                pxWindow->NameContentSize = PXTextLengthA(pxWindow->NameContent, PXTextLengthUnkown);

                char buffer[64];
                PXTextPrintA(buffer, 64, "TabPage-%i-%s", i, pxUIElementTabPageSingleInfo->PageName);

                // Create a panel for each page, to contain all elements, so that we can hide and show all at once
                PXResourceCreateInfo pxResourceCreateInfo;
                PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);
                pxResourceCreateInfo.Type = PXResourceTypeGUIElement;
                pxResourceCreateInfo.ObjectReference = &pxWindow;
                pxResourceCreateInfo.UIElement.Type = PXUIElementTypeTabPage;
                pxResourceCreateInfo.UIElement.Name = buffer;
                pxResourceCreateInfo.UIElement.Invisible = PXTrue;
                pxResourceCreateInfo.UIElement.UIElementWindow = pxGUIElementCreateInfo->UIElementWindow;
                pxResourceCreateInfo.UIElement.UIElementParent = pxWindowCurrent;
                pxResourceCreateInfo.UIElement.BehaviourFlags = PXWindowBehaviourDefaultDecorative | PXWindowAllignLeft;
                pxResourceCreateInfo.UIElement.Position.MarginLeft = 0;
                pxResourceCreateInfo.UIElement.Position.MarginTop = 0.08;
                pxResourceCreateInfo.UIElement.Position.MarginRight = 0;
                pxResourceCreateInfo.UIElement.Position.MarginBottom = 0;

                PXResourceManagerAdd(pxGUISystem->ResourceManager, &pxResourceCreateInfo, 1);

                PXWindowCreate(pxGUISystem, &pxResourceCreateInfo, 1);



                if(pxUIElementTabPageSingleInfo->TABIcon)
                {
                    pxWindow->Icon = pxUIElementTabPageSingleInfo->TABIcon;
                }
                else
                {
                    pxWindow->Icon = pxIcon;
                }


                switch(PXResourceInfoUseByMask & pxWindowCurrent->Info.Flags)
                {
                    case PXResourceInfoUseByOS:
                    {
#if OSUnix
#elif OSWindows && 0
                        TCITEM tie;
                        tie.mask = TCIF_TEXT | TCIF_IMAGE;
                        tie.iImage = pxUIElementTabPageSingleInfo->ImageID;
                        tie.pszText = pxUIElementTabPageSingleInfo->PageName;

                        auto x = TabCtrl_InsertItem(pxGUIElement->Info.Handle.WindowID, i, &tie);
#endif
                        break;
                    }

                    case PXResourceInfoUseByEngine:
                    {


                        break;
                    }

                    default:
                        break;
                }

                PXLogPrint
                (
                    PXLoggingInfo,
                    "GUI",
                    "TabControl-Add",
                    "Page added %s",
                    pxUIElementTabPageSingleInfo->PageName
                );
            }

#if OSUnix
#elif OSWindows && 0
            SendMessage(pxGUIElement->Info.Handle.WindowID, TCM_SETCURFOCUS, 0, 0);
#endif

#endif

            break;
        }
        case PXUIElementTypeToggle:
        {

            break;
        }
        case PXUIElementTypeColorPicker:
        {

            break;
        }
        case PXUIElementTypeSlider:
        {

            break;
        }
        case PXUIElementTypeCheckBox:
        {


            break;
        }
        case PXUIElementTypeComboBox:
        {

            break;
        }
        case PXUIElementTypeRenderFrame:
        {
#if 1
            PXUIElementSceneRenderInfo* const pxUIElementSceneRenderInfo = &pxGUIElementCreateInfo->Data.SceneRender;
            PXEngine* const pxEngine = pxUIElementSceneRenderInfo->Engine;
            PXEngineStartInfo* const pxEngineStartInfo = pxUIElementSceneRenderInfo->StartInfo;

            pxEngineStartInfo->Mode = PXGraphicInitializeModeOSGUIElement;
            pxEngineStartInfo->Width = pxWindowCurrent->Position.Width;
            pxEngineStartInfo->Height = pxWindowCurrent->Position.Height;
            pxEngineStartInfo->UIElement = pxWindowCurrent;
            pxEngineStartInfo->Name = "UIElement-RenderFrame";
            pxEngineStartInfo->UseMouseInput = 1;

            PXEngineStart(pxEngine, pxEngineStartInfo);

#endif

            break;
        }

        case PXUIElementTypeWindow:
        {
            PXWindowCreateWindowInfo* const pxGUIElementCreateWindowInfo = &pxGUIElementCreateInfo->Data.Window;

            PXWindowTitleBarColorSet(pxWindowCurrent->Info.Handle.WindowID);

            // UpdateWindow(pxGUIElement->Info.Handle.WindowID);

            // ShowWindow(pxWindow->ID, SW_NORMAL)

#if OSWindows
            BOOL aaxx = EnableWindow(pxWindowCurrent->Info.Handle.WindowID, PXTrue);
#endif

          //  HWND qssa = SetCapture(pxWindowCurrent->Info.Handle.WindowID);

#if 0
#if OSUnix

            int numberOfDevices = 0;

            const XDeviceInfo* deviceInfoList = XListInputDevices(display, &numberOfDevices);

            for(int i = 0; i < numberOfDevices; ++i)
            {
                const XDeviceInfo& xDeviceInfo = deviceInfoList[i];

                printf
                (
                    "[Device] ID:%i Name:%s Use:%i\n",
                    xDeviceInfo.id,
                    xDeviceInfo.name,
                    xDeviceInfo.use
                );

                // XOpenDevice(display, i); // Cannot open mouse or keyboard??  invalid call

                // XEventClass eventList[8]={0,0,0,0,0,0,0,0};
                //  int listSize = xDeviceInfo.num_classes;

                //  int resultE = XSelectExtensionEvent(display, PXWindowID, eventList, listSize);

                // printf("");
            }
#endif



            if(info)
            {
                const PXActionResult actionResult = PXThreadRun(&window->MessageThread, "PXWindowMessageLoop", PXWindowMessageLoop, window);
                const PXBool sucessful = PXActionSuccessful == actionResult;

                if(!sucessful)
                {
                    return; // TODO: return something?
                }

                PXBool expected = PXTrue;
                PXAwaitInfo pxAwaitInfo;
                pxAwaitInfo.DataTarget = &window->IsRunning;
                pxAwaitInfo.DataExpect = &expected;
                pxAwaitInfo.DataSize = sizeof(PXBool);

                PXAwaitChange(&pxAwaitInfo);
            }

#endif

            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowFetch(PXGUISystem* const pxGUISystem, PXWindowPropertyInfo* const pxGUIElementUpdateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXWindowPropertyInfo* const pxGUIElementUpdateInfo = &pxGUIElementUpdateInfoList[i];
        PXWindow* const pxGUIElement = pxGUIElementUpdateInfo->UIElement;

        switch(pxGUIElementUpdateInfo->Property)
        {
            case PXUIElementPropertySizeParent:
            {
                PXWindowSizeInfo* pxWindowSizeInfo = &pxGUIElementUpdateInfo->Data.Size;

                // const PXBool hasParent = pxGUIElement ? pxGUIElement->Parent : PXFalse;

                const PXBool hasParent = PXNull != pxGUIElementUpdateInfoList->WindowReference;

                if(!hasParent) // Special behaviour, if ID is null, get the screensize
                {
                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;

                    PXGUIScreenGetSize(&pxWindowSizeInfo->Width, &pxWindowSizeInfo->Height);

                    return PXActionSuccessful;
                }

                PXWindowPropertyInfo pxGUIElementUpdateInfoSub;
                PXClear(PXWindowPropertyInfo, &pxGUIElementUpdateInfoSub);
                pxGUIElementUpdateInfoSub.Property = PXUIElementPropertySize;
                pxGUIElementUpdateInfoSub.UIElement = pxGUIElementUpdateInfoList->WindowReference;

                PXWindowFetch(pxGUISystem, &pxGUIElementUpdateInfoSub, 1);

                PXCopy(PXWindowSizeInfo, &pxGUIElementUpdateInfoSub.Data.Size, &pxGUIElementUpdateInfo->Data.Size);

                break;
            }
            case PXUIElementPropertySize:
            {
                PXWindowSizeInfo* pxWindowSizeInfo = &pxGUIElementUpdateInfo->Data.Size;

#if OSUnix
                // ScreenCount();

                // ScreenOfDisplay();

                // if "display" is null, DefaultScreenOfDisplay will SEGFAULT

                if(pxGUISystem->DisplayCurrent.DisplayHandle)
                {
                    Screen* const xScreen = DefaultScreenOfDisplay(pxGUISystem->DisplayCurrent.DisplayHandle); // X11

                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;
                    pxWindowSizeInfo->Width = WidthOfScreen(xScreen);
                    pxWindowSizeInfo->Height = HeightOfScreen(xScreen);
                }
                else
                {
                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;
                    pxWindowSizeInfo->Width = 800;
                    pxWindowSizeInfo->Height = 600;
                }


#elif PXOSWindowsDestop

                RECT rect;

                //const PXBool result = GetWindowRect(pxGUIElement->ID, &rect); // Windows 2000, User32.dll, winuser.h
                const PXBool result = GetClientRect(pxGUIElement->Info.Handle.WindowID, &rect); // Windows 2000, User32.dll, winuser.h

                if(!result)
                {
                    pxWindowSizeInfo->X = 0;
                    pxWindowSizeInfo->Y = 0;
                    pxWindowSizeInfo->Width = 0;
                    pxWindowSizeInfo->Height = 0;

                    return PXActionRefusedNotFound;
                }

                // Get Last Error

                pxWindowSizeInfo->X = rect.left;
                pxWindowSizeInfo->Y = rect.top;
                pxWindowSizeInfo->Width = rect.right - rect.left;
                pxWindowSizeInfo->Height = rect.bottom - rect.top;
#endif

                break;
            }

            default:
                break;
        }
    }



    return PXActionSuccessful;
}

PXActionResult PXAPI PXWindowRelease(PXWindow* const pxGUIElement)
{
#if 0

#if OSUnix
    glXMakeCurrent(window->DisplayCurrent, None, NULL);
    //    glXDestroyContext(DisplayCurrent, PXOpenGLConextID);
    XDestroyWindow(window->DisplayCurrent, window->ID);
    XCloseDisplay(window->DisplayCurrent);

    window->ID = PXNull;

#elif PXOSWindowsDestop

    // Minimizes but does not destroy the specified window.
    CloseWindow(window->ID); // Windows 2000, User32.dll, winuser.h

    // Finally destroy the window
    DestroyWindow(window->ID); // Windows 2000, User32.dll, winuser.h

    window->ID = PXNull;

#else
    return; // Not supported on this OS
#endif

#endif

    return PXActionSuccessful;
}


void PXAPI PXWindowhSizeRefresAll(PXGUISystem* const pxGUISystem)
{
    if(!(pxGUISystem))
    {
        return;
    }

    PXDictionary* const uiElementLookup = &pxGUISystem->ResourceManager->GUIElementLookup;

    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

    for(PXSize i = 0; i < uiElementLookup->EntryAmountCurrent; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;
        PXWindow* uiElement = PXNull;

        PXDictionaryIndex(uiElementLookup, i, &pxDictionaryEntry);

        uiElement = *(PXWindow**)pxDictionaryEntry.Value;

        if(uiElement->Type == PXUIElementTypeWindow)
        {
            continue;
        }

        PXWindowPropertyInfo pxGUIElementUpdateInfo;
        PXClear(PXWindowPropertyInfo, &pxGUIElementUpdateInfo);
        pxGUIElementUpdateInfo.UIElement = uiElement;
        pxGUIElementUpdateInfo.WindowReference = PXNull;
        pxGUIElementUpdateInfo.Property = PXUIElementPropertySizeParent;

        PXNativDrawWindowProperty(pxGUISystem, &pxGUIElementUpdateInfo, 1);
    }
}

PXActionResult PXAPI PXWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Window",
        "PixelSystem",
        "Setting info"
    );
#endif

#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows

    if(!pxWindowPixelSystemInfo->HandleDeviceContext) // If we dont have a prefered GPU
    {
        pxWindowPixelSystemInfo->HandleDeviceContext = GetDC(pxWindowPixelSystemInfo->HandleWindow); // Get the "default" device
    }

    const WORD  nVersion = 1;
    DWORD dwFlags =
        //PFD_DRAW_TO_WINDOW |
        //PFD_DIRECT3D_ACCELERATED |
        PFD_DOUBLEBUFFER |
        0;



    dwFlags |= pxWindowPixelSystemInfo->OpenGL * PFD_SUPPORT_OPENGL;
    dwFlags |= pxWindowPixelSystemInfo->DirectX * PFD_SUPPORT_DIRECTDRAW;
    dwFlags |= pxWindowPixelSystemInfo->GDI * PFD_SUPPORT_GDI;


    const BYTE  iPixelType = PFD_TYPE_RGBA; // The kind of framebuffer. RGBA or palette.
    const BYTE  cColorBits = pxWindowPixelSystemInfo->BitPerPixel;   // Colordepth of the framebuffer.
    const BYTE  cRedBits = 0;
    const BYTE  cRedShift = 0;
    const BYTE  cGreenBits = 0;
    const BYTE  cGreenShift = 0;
    const BYTE  cBlueBits = 0;
    const BYTE  cBlueShift = 0;
    const BYTE  cAlphaBits = 0;
    const BYTE  cAlphaShift = 0;
    const BYTE  cAccumBits = 0;
    const BYTE  cAccumRedBits = 0;
    const BYTE  cAccumGreenBits = 0;
    const BYTE  cAccumBlueBits = 0;
    const BYTE  cAccumAlphaBits = 0;
    const BYTE  cDepthBits = 24; // Number of bits for the depthbuffer
    const BYTE  cStencilBits = 8;  // Number of bits for the stencilbuffer
    const BYTE  cAuxBuffers = 0;  // Number of Aux buffers in the framebuffer.
    const BYTE  iLayerType = PFD_MAIN_PLANE;
    const BYTE  bReserved = 0;
    const DWORD dwLayerMask = 0;
    const DWORD dwVisibleMask = 0;
    const DWORD dwDamageMask = 0;
    const PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        nVersion,
        dwFlags,
        iPixelType,
        cColorBits,
        cRedBits,cRedShift,
        cGreenBits, cGreenShift,
        cBlueBits, cBlueShift,
        cAlphaBits, cAlphaShift,
        cAccumBits,
        cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits,
        cDepthBits,
        cStencilBits,
        cAuxBuffers,
        iLayerType,
        bReserved,
        dwLayerMask, dwVisibleMask, dwDamageMask
    };
    const int letWindowsChooseThisPixelFormat = ChoosePixelFormat(pxWindowPixelSystemInfo->HandleDeviceContext, &pfd);
    const PXBool successul = SetPixelFormat(pxWindowPixelSystemInfo->HandleDeviceContext, letWindowsChooseThisPixelFormat, &pfd);
    const PXActionResult pxActionResult = PXErrorCurrent(successul);

    if(PXActionSuccessful != pxActionResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "Window",
            "PixelSystem",
            "Setting failed"
        );
#endif

        return pxActionResult;
    }

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif
}


PXNativDrawWindowHandle PXAPI PXWindowFindViaTitle(const PXText* const windowTitle)
{
    switch(windowTitle->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
#if OSUnix
            return PXNull;
#elif PXOSWindowsDestop
            return FindWindowA(0, windowTitle->TextA); // Windows 2000, User32.dll, winuser.h
#else
            return PXFalse; // Not supported by OS
#endif
        }
        case TextFormatUNICODE:
        {
#if OSUnix
            return PXNull;
#elif PXOSWindowsDestop
            return FindWindowW(0, windowTitle->TextW); // Windows 2000, User32.dll, winuser.h
#else
            return PXNull; // Not supported by OS
#endif
        }
    }

    return PXNull;
}


PXActionResult PXAPI PXWindowTitleBarColorSet(const PXNativDrawWindowHandle pxWindowID)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    PXLibrary pyLibrary;

    // Open lib
    {
        const PXActionResult libOpenResult = PXLibraryOpenA(&pyLibrary, "DWMAPI.DLL");

        if(PXActionSuccessful != libOpenResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

    typedef HRESULT(WINAPI* PXDwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, _In_reads_bytes_(cbAttribute) LPCVOID pvAttribute, DWORD cbAttribute);

    PXDwmSetWindowAttribute pxDwmSetWindowAttribute;

    PXBool hasFunction = PXLibraryGetSymbolA(&pyLibrary, &pxDwmSetWindowAttribute, "DwmSetWindowAttribute", PXTrue);

    if(!hasFunction)
    {
        PXLibraryClose(&pyLibrary);

        return PXActionRefusedNotSupportedByOperatingSystem;
    }

    const BOOL useDarkMode = PXTrue;
    const BOOL setAttributeSuccess = SUCCEEDED(pxDwmSetWindowAttribute(pxWindowID, Windows10DarkModeID, &useDarkMode, sizeof(BOOL))); // Windows Vista, Dwmapi.dll;Uxtheme.dll, dwmapi.h

    PXLibraryClose(&pyLibrary);

    if(!setAttributeSuccess)
    {
        return PXActionCancelled;
    }

    ShowWindow(pxWindowID, SW_HIDE);
    ShowWindow(pxWindowID, SW_SHOW);

    return PXActionSuccessful;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXActionResult PXAPI PXWindowMouseMovementEnable(const PXNativDrawWindowHandle pxWindow)
{
#if OSUnix
    return PXActionRefusedNotImplemented;

#elif OSWindows
#if PXOSWindowsDestop && WindowsAtleastXP
    RAWINPUTDEVICE rawInputDeviceList[2];
    rawInputDeviceList[0].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    rawInputDeviceList[0].usUsage = 0x03;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[0].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rawInputDeviceList[0].hwndTarget = pxWindow;

    rawInputDeviceList[1].usUsagePage = 0x01;//HID_USAGE_PAGE_GENERIC;
    rawInputDeviceList[1].usUsage = 0x02;// HID_USAGE_GENERIC_MOUSE;
    rawInputDeviceList[1].dwFlags = RIDEV_INPUTSINK | RIDEV_DEVNOTIFY;
    rawInputDeviceList[1].hwndTarget = pxWindow;

    // WARNING
    // RegisterRawInputDevices should not be used from a library!
    // As it may interfere with any raw input processing logic already present in applications that load it.
    const BOOL regsiterRawImputSuccess = RegisterRawInputDevices(rawInputDeviceList, 2, sizeof(RAWINPUTDEVICE)); // Windows XP, User32.dll, winuser.h
    const PXActionResult pxActionResult = PXErrorCurrent(regsiterRawImputSuccess);

    if(PXActionSuccessful != pxActionResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "GUI",
            "Input-Mouse",
            "Failed to registerd device for <0x%p>",
            pxWindow
        );
#endif

        return pxActionResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI",
        "Input-Mouse",
        "Registerd device for <0x%p>",
        pxWindow
    );
#endif

    return PXActionSuccessful;

#else
    return PXActionRefusedNotSupported;
#endif

    return PXActionRefusedNotSupportedByOperatingSystem;

#endif
}

/*
PXActionResult PXAPI PXWindowSizeSet(const PXWindowID pxWindow, PXWindowSizeInfo* const pxWindowSizeInfo)
{
#if OSUnix
#elif PXOSWindowsDestop
    RECT rect;

    rect.left = pxWindowSizeInfo->X;
    rect.top = pxWindowSizeInfo->Y;
    rect.right = pxWindowSizeInfo->X + pxWindowSizeInfo->Width;
    rect.bottom = pxWindowSizeInfo->Y + pxWindowSizeInfo->Height;

    DWORD style = 0;
    DWORD exStyle = 0;

    //AdjustWindowRectEx();

    const PXBool result = AdjustWindowRectEx(&rect, style, FALSE, exStyle); // Windows 2000, User32.dll, winuser.h
    const PXBool success = result != 0;

    // Get Last Error

    return PXActionSuccessful;
#endif
}
*/

void PXAPI PXWindowCursorCaptureMode(const PXNativDrawWindowHandle pxWindowID, const PXWindowCursorMode cursorMode)
{
    PXInt32S horizontal = 0;
    PXInt32S vertical = 0;

#if OSUnix
#elif PXOSWindowsDestop

    PXGUIScreenGetSize(&horizontal, &vertical);

    switch(cursorMode)
    {
        case PXWindowCursorShow:
        {
            //printf("[Cursor] Show\n");

            while(ShowCursor(1) < 0);

            const PXBool clipResult = ClipCursor(NULL);
            const HCURSOR cursorSet = SetCursor(pxWindowID);

            break;
        }
        case PXWindowCursorLock:
        {
            // printf("[Cursor] Lock\n");

            // Capture cursor
            {
                RECT clipRect;
                GetClientRect(pxWindowID, &clipRect);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.left);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.right);
                ClipCursor(&clipRect);
            }

            break;
        }
        case PXWindowCursorLockAndHide:
        {
            //printf("[Cursor] Lock and hide\n");

            while(ShowCursor(0) >= 0);

            {
                RECT clipRect;
                GetClientRect(pxWindowID, &clipRect);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.left);
                ClientToScreen(pxWindowID, (POINT*)&clipRect.right);

                int xOff = (clipRect.right - clipRect.left) / 2;
                int yoFf = (clipRect.bottom - clipRect.top) / 2;

                clipRect.left += xOff;
                clipRect.top += yoFf;
                clipRect.right -= xOff;
                clipRect.bottom -= yoFf;

                unsigned char sucessClip = ClipCursor(&clipRect);
            }


            CURSORINFO cursorInfo;
            cursorInfo.cbSize = sizeof(CURSORINFO);

            const unsigned char sucessfulInfoGet = GetCursorInfo(&cursorInfo);

            HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

            CONSOLE_CURSOR_INFO lpCursor;
            lpCursor.bVisible = 0;
            lpCursor.dwSize = sizeof(CONSOLE_CURSOR_INFO);
            unsigned char y = SetConsoleCursorInfo(console, &lpCursor);

            break;
        }
    }

    //window->CursorModeCurrent = cursorMode;
#endif
}

PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXNativDrawWindowHandle pxWindowID, PXInt32S* const x, PXInt32S* const y)
{
    PXInt32S xPos = 0;
    PXInt32S yPos = 0;
    const PXBool sucessfulA = PXWindowCursorPositionInDestopGet(pxWindowID, &xPos, &yPos);

#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    POINT point;
    point.x = xPos;
    point.y = yPos;

    const PXBool sucessful = ScreenToClient(pxWindowID, &point);  // are now relative to hwnd's client area

    if(sucessful)
    {
        *x = point.x;
        *y = point.y;
    }
    else
    {
        *x = 0;
        *y = 0;
    }

    return sucessful;
#endif
}

PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXNativDrawWindowHandle pxWindowID, PXInt32S* const x, PXInt32S* const y)
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop

#if WindowsAtleastVista
    POINT point;
    point.x = 0;
    point.y = 0;

    const PXBool successful = GetPhysicalCursorPos(&point); // Windows Vista, User32.dll, winuser.h

    if(successful)
    {
        *x = point.x;
        *y = point.y;
    }
    else
    {
        *x = 0;
        *y = 0;
    }

    return successful;
#else

    const PXSize mouseMovePointSize = sizeof(MOUSEMOVEPOINT);
    const int nVirtualWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    const int nVirtualHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    const int nVirtualLeft = GetSystemMetrics(SM_XVIRTUALSCREEN);
    const int nVirtualTop = GetSystemMetrics(SM_YVIRTUALSCREEN);
    const int mode = GMMP_USE_DISPLAY_POINTS;
    MOUSEMOVEPOINT mp_in;
    MOUSEMOVEPOINT mp_out[64];

    PXMemoryClear(&mp_in, mouseMovePointSize);

    mp_in.x = 0x0000FFFF;//Ensure that this number will pass through.
    mp_in.y = 0x0000FFFF;
    const int numberOfPoints = GetMouseMovePointsEx(mouseMovePointSize, &mp_in, &mp_out, 64, mode); // Windows 2000, User32.dll, winuser.h
    const PXBool success = numberOfPoints > 0;

    if(!success)
    {
        return PXFalse;
    }

    /*
    for (PXSize i = 0; i < (PXSize)numberOfPoints; ++i)
    {
        switch (mode)
        {
        case GMMP_USE_DISPLAY_POINTS:
        {

            if (mp_out[i].x > 32767)
                mp_out[i].x -= 65536;
            if (mp_out[i].y > 32767)
                mp_out[i].y -= 65536;
            break;
        }
        case GMMP_USE_HIGH_RESOLUTION_POINTS:
        {
            mp_out[i].x = ((mp_out[i].x * (nVirtualWidth - 1)) - (nVirtualLeft * 65536)) / nVirtualWidth;
            mp_out[i].y = ((mp_out[i].y * (nVirtualHeight - 1)) - (nVirtualTop * 65536)) / nVirtualHeight;
            break;
        }
        }

        *x = mp_out[i].x;
        *y = mp_out[i].y;

        break;
    }*/

    return PXTrue;

#endif


#endif
}

PXBool PXAPI PXWindowIsInFocus(const PXNativDrawWindowHandle pxWindowID)
{
#if OSUnix
    return PXFalse;
#elif PXOSWindowsDestop
    const HWND windowIDInFocus = GetForegroundWindow(); // Windows 2000, User32.dll,
    const PXBool isInFocus = pxWindowID == windowIDInFocus;

    return isInFocus;
#endif
}

/*
PXActionResult PXAPI PXWindowDrawText(PXGUISystem* const pxGUISystem, PXWindow* const pxGUIElement, PXText* const pxText)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
            return PXWindowDrawTextA(pxGUISystem, pxGUIElement, 0, pxText->TextA, pxText->SizeUsed);

        case TextFormatUNICODE:
            return PXWindowDrawTextW(pxGUISystem, pxGUIElement, pxText->TextW, pxText->SizeUsed);

        default:
            return TextFormatInvalid;
    }
}*/





PXActionResult PXAPI PXGUIFontListFetch(PXGUISystem* const pxGUISystem, PXSize* const amount)
{
#if OSUnix
    int fontNameListSizeMax = 0;
    int fontNameListSizeCurrnet = 0;
    char** fontNameList = XListFonts
    (
        pxGUISystem->DisplayCurrent.DisplayHandle,
        PXNull,
        fontNameListSizeMax,
        &fontNameListSizeCurrnet
    );


    // Can also have additional info
    // char **XListFontsWithInfo(Display *display, char *pattern, int maxnames, int *count_return, XFontStruct

#elif OSWindows

    // Microsoft says: EnumFontFamiliesA() is deprecated and is only provided for compatibility with 16-bit versions
    // EnumFontFamiliesEx shall be used instead.

    const int result = 0;// EnumFontFamiliesExA(HDC hdc, LPLOGFONTA lpLogfont, FONTENUMPROCA lpProc, LPARAM lParam, 0);


#else
    return PXNotsuport;
#endif
}









#if OSUnix
PXActionResult PXAPI PXWindowErrorFromXSystem(const int xSysstemErrorID)
{
    switch(xSysstemErrorID)
    {
        case BadValue:
            return PXActionRefusedArgumentInvalid; // input is not valid
        case BadWindow:
            return PXActionRefusedNotFound; // object id invalid
        case BadAlloc:
            return PXActionFailedMemoryAllocation;
        case BadColor:
            return PXActionRefusedArgumentInvalid;
        case BadCursor:
            return PXActionRefusedArgumentInvalid;
        case BadMatch:
            return PXActionRefusedArgumentInvalid;
        case BadPixmap:
            return PXActionRefusedArgumentInvalid;

        default:
            return PXActionInvalid;
    }
}
#endif





typedef struct PXGUISystemContextMenuEntry_
{
    char* FileTypeExtension;
    char* FileTypeName;
    char* FileTypeDefaultIconPath;
}
PXGUISystemContextMenuEntry;

PXActionResult PXAPI PXGUISystemContextMenuEntryAdd(PXGUISystemContextMenuEntry* const pxGUISystemContextMenuEntry)
{
    // Go to HKEY_CLASSES_ROOT

    // Create sub folder for your file extension (dont forget the dot)
    // Name the default value to the name if rge type.
    // add additional info via keys

    // Create a sub-folder in that new file-extension
    // + file extension folder
    // +---+ shell
    //     +---+ *Name of what shows in menu*
    //         +--- command (Add a CMD command as default value)
}



void PXAPI PXGUIIconFetch()
{
    // https://learn.microsoft.com/en-us/windows/win32/api/shellapi/nf-shellapi-shgetstockiconinfo

    /*

     // Get icon from system
        HRESULT result = PInvoke.SHGetStockIconInfo(
            (SHSTOCKICONID)stockIcon,
            SHGSI_FLAGS.SHGSI_ICONLOCATION,
            &info);



    HICON ExtractIconA(
    [in] HINSTANCE hInst,
    [in] LPCSTR    pszExeFileName,
       UINT      nIconIndex
    */




}

#if OXWindows
void PXAPI PXGUIStartMenuEntryCreate(LPCSTR lpszPathObj, LPCSTR lpszPathLink, LPCSTR lpszDesc)
{
    // If atleast Vista
    // https://learn.microsoft.com/en-us/windows/win32/api/shobjidl/nf-shobjidl-istartmenupinnedlist-removefromlist

    /*

    HRESULT hres;
    IShellLink* psl;

    // Initialize COM library
    CoInitialize(NULL);

    // Create an IShellLink object
    hres = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (LPVOID*)&psl);
    if (SUCCEEDED(hres)) {
        IPersistFile* ppf;

        // Set the path to the shortcut target
        psl->lpVtbl->SetPath(psl, lpszPathObj);

        // Set the description of the shortcut
        psl->lpVtbl->SetDescription(psl, lpszDesc);

        // Query IShellLink for the IPersistFile interface
        hres = psl->lpVtbl->QueryInterface(psl, &IID_IPersistFile, (LPVOID*)&ppf);
        if (SUCCEEDED(hres)) {
            WCHAR wsz[MAX_PATH];

            // Ensure the string is Unicode
            MultiByteToWideChar(CP_ACP, 0, lpszPathLink, -1, wsz, MAX_PATH);

            // Save the link by calling IPersistFile::Save
            hres = ppf->lpVtbl->Save(ppf, wsz, TRUE);
            ppf->lpVtbl->Release(ppf);
        }
        psl->lpVtbl->Release(psl);
    }
    CoUninitialize();


    // Emabple use
      char szPath[MAX_PATH];

    // Get the path to the Programs folder
    SHGetSpecialFolderPath(NULL, szPath, CSIDL_PROGRAMS, FALSE);

    // Append the name of the shortcut
    strcat(szPath, "\\MyApp.lnk");

    // Create the shortcut
    PXGUIStartMenuEntryCreate("C:\\Path\\To\\YourApp.exe", szPath, "My Application");
    */
}
#endif



PXInt32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon)
{
    switch(cursorIcon)
    {
        default:
        case PXCursorIconInvalid:
            return -1;

        case PXCursorIconNormal:
            return CursorIconNormalID;

        case PXCursorIconIBeam:
            return CursorIconIBeamID;

        case PXCursorIconWait:
            return CursorIconWaitID;

        case PXCursorIconCross:
            return CursorIconCrossID;

        case PXCursorIconUp:
            return CursorIconUpID;

        case PXCursorIconHand:
            return CursorIconHandID;

        case PXCursorIconNotAllowed:
            return CursorIconNoAllowedID;

        case PXCursorIconAppStarting:
            return CursorIconAppStartingID;

        case PXCursorIconResizeHorizontal:
            return CursorIconResizeHorizontalID;

        case PXCursorIconResizeVertical:
            return CursorIconResizeVerticalID;

        case PXCursorIconResizeClockwise:
            return CursorIconResizeClockwiseID;

        case PXCursorIconResizeClockwiseCounter:
            return CursorIconResizeClockwiseCounterID;

        case PXCursorIconResizeAll:
            return CursorIconResizeAllID;
    }
}
