#include "PXGUI.h"


#if OSWindows

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


#if OSUnix

//---------------------------
// Xlib / X-System
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include <X11/Xmd.h>
//---------------------------

//---------------------------
// Wayland
//#include <wayland-client.h>
//---------------------------

//---------------------------
// GTK
//#include <gtk/gtk.h>
//---------------------------


#elif OSWindows

#include <Windows.h>    // Has to be included before any else
#include <shellapi.h>   // SHGetStockIconInfo
#include <CommCtrl.h>   // InitCommonControlsEx
#include <commdlg.h>    // ???
//#include <hidusage.h> // ???
#include <Dbt.h>        // ???
#include <ole2.h>       // Object Linking and Embedding
#include <dwmapi.h>     // Set tilebar color
#include <shellapi.h>   // ???
#include <Richedit.h>   // ???
#include <windowsx.h>   // ???
//#include <ShObjIdl.h> // ???

//#include <gdiplusgraphics.h>// ???
//#include <ddrawgdi.h>// ???

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 20
#endif

#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Msimg32.lib")

#endif


#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/Container/List/PXList.h>
#include <PX/Engine/PXEngine.h>
#include <PX/Media/PXText.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXAwait.h>
#include <PX/OS/Graphic/PXGraphic.h>
#include <PX/OS/File/PXDirectory.h>
#include <PX/OS/Graphic/NativDraw/PXNativDraw.h>
#include <PX/OS/Hardware/PXProcessor.h>


PXGUIManager _pxGUIManager;

const char PXGUIName[] = "GUI";
PXF32 colorTemp = 20;






















PXGUISystem* PXAPI PXGUIInitialize(void)
{
    PXClear(PXGUIManager, &_pxGUIManager);

    _pxGUIManager.ThemeSelected = &_pxGUIManager.ThemeDefault;
    



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
    //PXNativDrawDisplayOpen(&_pxGUIManager.NativDraw, &_pxGUIManager.DisplayCurrent, PXNull);



#if OSWindows

    // Ensures that the common control DLL (Comctl32.dll) is loaded
    INITCOMMONCONTROLSEX initCommonControls;
    initCommonControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    initCommonControls.dwICC = ICC_TAB_CLASSES;
    BOOL initResult = InitCommonControlsEx(&initCommonControls);

#endif


    //  PXGUIDisplayScreenListRefresh(pxGUISystem);



    // Create brushes

    PXResourceCreateInfo pxResourceCreateInfoList[4];
    PXClearList(PXResourceCreateInfo, pxResourceCreateInfoList, 4);
    pxResourceCreateInfoList[0].ObjectReference = &_pxGUIManager.ThemeDefault.BrushBackgroundDark;
    pxResourceCreateInfoList[0].NameAdress = "BackgroundDark";
    pxResourceCreateInfoList[0].Type = PXResourceTypeBrush;
    pxResourceCreateInfoList[0].Brush.Color.Red = 100; // 30-30-30, 160, 40, 40
    pxResourceCreateInfoList[0].Brush.Color.Green = 40;
    pxResourceCreateInfoList[0].Brush.Color.Blue = 40;



    pxResourceCreateInfoList[1].ObjectReference = &_pxGUIManager.ThemeDefault.BrushWhite;
    pxResourceCreateInfoList[1].NameAdress = "White";
    pxResourceCreateInfoList[1].Type = PXResourceTypeBrush;
    pxResourceCreateInfoList[1].Brush.Color.Red = 0xff; // 200-200-200
    pxResourceCreateInfoList[1].Brush.Color.Green = 200;
    pxResourceCreateInfoList[1].Brush.Color.Blue = 200;


#if 0
    pxResourceCreateInfoList[0].Brush.Color.Red = 0xFF;
    pxResourceCreateInfoList[0].Brush.Color.Green = 0x00;
    pxResourceCreateInfoList[0].Brush.Color.Blue = 0xFF;
    pxResourceCreateInfoList[1].Brush.Color.Red = 0x00;
    pxResourceCreateInfoList[1].Brush.Color.Green = 0xFF;
    pxResourceCreateInfoList[1].Brush.Color.Blue = 0x00;
#endif


    pxResourceCreateInfoList[2].ObjectReference = &_pxGUIManager.ThemeDefault.FontTitle;
    pxResourceCreateInfoList[2].NameAdress = "FontTitle";
    pxResourceCreateInfoList[2].Type = PXResourceTypeFont;

#if OSUnix
    pxResourceCreateInfoList[2].Font.RegisteredName = "fixed";
#elif OSWindows
    pxResourceCreateInfoList[2].Font.RegisteredName = "UniSpace";
#endif

    pxResourceCreateInfoList[3].ObjectReference = &_pxGUIManager.ThemeDefault.FontContent;
    pxResourceCreateInfoList[3].NameAdress = "FontContent";
    pxResourceCreateInfoList[3].Type = PXResourceTypeFont;
#if OSUnix
    pxResourceCreateInfoList[3].Font.RegisteredName = "fixed";    // fixed, Samanata(does not work on X11)
#elif OSWindows
    pxResourceCreateInfoList[3].Font.RegisteredName = "Eras Medium ITC";  // "Eras Medium ITC"; // Bradley Hand ITC, UniSpace,OCR A, Cascadia Mono,         "UniSpace";
#endif

    PXResourceManagerAddV(pxResourceCreateInfoList, 4);

    return PXActionSuccessful;
}

PXResult PXAPI  PXGUIShutdown(void)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
    const int resultID = XCloseDisplay(_pxGUIManager.DisplayCurrent.DisplayHandle);
    result = PXActionRefusedNotImplemented;
#elif OSWindows
    // TODO: ???
    result = PXActionRefusedNotImplemented;
#else
    result = PXnursupported;
#endif

    return result;
}

PXResult PXAPI  PXGUICreatePhysical(PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXGUICreateVirtual(PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXGUICreate(PXWindow* const pxWindow, PXWindowCreateInfo* const pxWindowCreateInfo)
{
    PXActionResult result = PXActionInvalid;

    switch(pxWindowCreateInfo->FLags)
    {
        case PXWindowCreatePhysical:
        {
            result = PXGUICreatePhysical(pxWindow, pxWindowCreateInfo);
            break;
        }
        case PXWindowCreateVirtual:
        {
            result = PXGUICreateVirtual(pxWindow, pxWindowCreateInfo);
            break;
        }
        default:
        {
            return PXActionRefusedArgumentInvalid;
        }
    }

    return result;
}

PXResult PXAPI  PXGUIDestroy(PXWindow* const pxWindow)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
    const int resultID = XDestroyWindow(_pxGUIManager.DisplayCurrent.DisplayHandle, pxWindow->Info.Handle.WindowHandle);
    result = PXWindowErrorFromXSystem(resultID);
#elif OSWindows
    const PXBool success = DestroyWindow(pxWindow->Info.Handle.WindowHandle);
    result = PXErrorCurrent(success);
#else
    result PXActionRefusedNotSupportedByLibrary;
#endif

    if(PXActionSuccessful == result)
    {
        pxWindow->Info.Handle.WindowHandle = PXNull;
    }





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

   // return PXActionSuccessful;

    return result;
}






PXResult PXAPI  PXGUIPropertyChange(PXWindow* const pxWindow, PXGUIProperty* const pxGUIProperty)
{
    return PXActionInvalid;
}

const char WindowsLibraryDWMAPI[] = "DWMAPI.DLL";
const char WindowsLibraryDWMAPISET[] = "DwmSetWindowAttribute";


PXResult PXAPI  PXGUIPropertyTitleBarColor(PXWindow* const pxWindow)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    PXLibrary pyLibrary;

    // Open lib
    {
        const PXActionResult libOpenResult = PXLibraryOpenA(&pyLibrary, WindowsLibraryDWMAPI);

        if(PXActionSuccessful != libOpenResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

    typedef HRESULT(WINAPI* PXDwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, _In_reads_bytes_(cbAttribute) LPCVOID pvAttribute, DWORD cbAttribute);

    PXDwmSetWindowAttribute pxDwmSetWindowAttribute;

    const PXBool hasFunction = PXLibraryGetSymbolA(&pyLibrary, &pxDwmSetWindowAttribute, WindowsLibraryDWMAPISET, PXTrue);

    if(!hasFunction)
    {
        PXLibraryClose(&pyLibrary);

        return PXActionRefusedNotSupportedByOperatingSystem;
    }

    const HWND windowHandle = pxWindow->Info.Handle.WindowHandle;
    const BOOL useDarkMode = PXTrue;
    const HRESULT resultID = pxDwmSetWindowAttribute(windowHandle, DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(BOOL)); // Windows Vista, Dwmapi.dll;Uxtheme.dll, dwmapi.h
    const PXResult setResult = PXErrorFromHRESULT(resultID);

    PXLibraryClose(&pyLibrary);

    if(PXActionSuccessful != setResult)
    {
        return setResult;
    }

    ShowWindow(windowHandle, SW_HIDE);
    ShowWindow(windowHandle, SW_SHOW);

    return setResult;
#else
    return PXActionNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXGUIFindViaTitleA(PXWindow* const pxWindow, const char* text)
{
    const HWND windowHandle = FindWindowA(0, text); // Windows 2000, User32.dll, winuser.h

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIFindViaTitleW(PXWindow* const pxWindow, const wchar_t* text)
{
    const HWND windowHandle = FindWindowW(0, text); // Windows 2000, User32.dll, winuser.h

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIIconGetSystem(PXIcon* const pxIcon, const int iconID)
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

    const HRESULT resultFile = SHGetStockIconInfo
    (
        SIID_DOCNOASSOC, 
        SHGSI_ICON, 
        &stockIconInfo
    ); // Windows Vista, Shell32.dll, shellapi.h
    const PXActionResult pxActionResult = PXErrorFromHRESULT(resultFile);

    pxIcon->Info.Handle.IconHandle = 0;

    return pxActionResult;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXGUIIconGetViaFilePath(PXIcon* const pxIcon, const char* fileName)
{
    if(!(pxIcon && fileName))
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    SHFILEINFOA shFileInfo;
    PXClear(SHFILEINFOA, &shFileInfo);

    const PXBool fetch = SHGetFileInfoA
    (
        fileName,
        0,
        &shFileInfo,
        sizeof(SHFILEINFOA),
        SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_SMALLICON | SHGFI_LARGEICON
    ); // Windows 2000, Shell32.dll 4.0, shellapi.h

    pxIcon->Info.Handle.IconHandle = shFileInfo.hIcon;

    return PXActionSuccessful;


    /*




    char moduleName[MAX_PATH];
    PXI16U moduleNameSize = 0;
    PXI16U iconID = 0;

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
        PXGUIName,
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

PXResult PXAPI PXGUIDragStart(PXWindow* const pxWindow)
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

PXResult PXAPI PXGUIDrawRectangle2D(PXWindow* const pxWindow, const int x, const int y, const int width, const int height)
{
    PXGUITheme* const pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw-Rectangle",
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i",
        pxWindow->Info.ID,
        x,
        y,
        width,
        height
    );
#endif

    const PXBool isHovered = (PXResourceInfoSelected & pxWindow->Info.Behaviour) > 0;

    PXActionResult pxActionResult = PXActionInvalid;

#if OSUnix
    PXNativDrawColorSetBrush(pxNativDraw, pxWindow, brushBackGround, PXGUIDrawModeBack);

    const resultID = XFillRectangle
    (
        pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
        pxWindow->Info.Handle.WindowHandle,
        pxNativDraw->GUISystem->DisplayCurrent.GraphicContent,
        x,
        y,
        width,
        height
    );
    // pxActionResultresult = PXWindowErrorFromXSystem(resultID);
#elif OSWindows
    //  const BOOL bbbbb = SelectObject(pxWindow->DeviceContextHandle, GetStockObject(GRAY_BRUSH));



    RECT rect;
    PXRectangleXYWHI32 pxRectangleXYWHI32 = { x, y, width, height };
    PXRectangleXYWHI32ToLTRBI32(&pxRectangleXYWHI32, (PXRectangleLTRBI32*)&rect);

    // const COLORREF color = RGB(255, 0, 200);
    // const HBRUSH brushAA = CreateSolidBrush(color);

    PXWindowBrush* pxWindowBrush = PXNull;

    if(isHovered)
    {
        pxWindowBrush = pxGUITheme->BrushWhite;
    }
    else
    {
        pxWindowBrush = pxGUITheme->BrushBackgroundDark;
    }

    PXNativDrawRectangle
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        x,
        y,
        width, 
        height,
        pxWindowBrush
    );

    // DeleteObject();


    const BOOL success = 1;// Rectangle(pxWindow->DeviceContextHandle, x, y, width, height);

    if(!success)
    {
        pxActionResult = PXActionInvalid;
    }

#else
    pxActionResult = PXNotSupport;
#endif


#if PXLogEnable && 0
    if(PXActionSuccessful == pxActionResult)
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXNativDraw,
            "Draw-Button",
            "%i, X:%i,Y:%i,W:%i,H:%i",
            pxWindow->Info.ID,
            x, y, width, height
        );
    }
    else
    {
        PXLogPrint
        (
            PXLoggingInfo,
            PXNativDraw,
            "Draw-Rectangle",
            "%i, X:%i,Y:%i,W:%i,H:%i",
            pxWindow->Info.ID,
            x, y, width, height
        );
    }
#endif

    return pxActionResult; 
}

PXResult PXAPI PXGUIDrawRectangle3D(PXWindow* const pxWindow, const int x, const int y, const int width, const int height)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Rectangle3D",
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i",
        pxWindow->Info.ID,
        x,
        y,
        width,
        height
    );
#endif

    const PXBool isHovered = (PXResourceInfoSelected & pxWindow->Info.Behaviour) > 0;

    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

    PXWindowBrush* brushFront = pxGUITheme->BrushWhite;
    PXWindowBrush* brushBackground = pxGUITheme->BrushBackgroundDark;

    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        brushFront,
        PXGUIDrawModeFront
    );

    if(isHovered)
    {
        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            brushFront,
            PXGUIDrawModeBack
        );
        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            brushFront,
            PXGUIDrawModeFront
        );
    }
    else
    {
        PXNativDrawColorSetBrush
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
            brushBackground,
            PXGUIDrawModeFront
        );
    }

    PXNativDrawRectangle
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        x,
        y,
        width,
        height,
        brushBackground
    );

    const PXF32 colorTint = isHovered * 0.4f;
    const PXF32 highFactor = 1.35f;
    const PXF32 lowFactor = 0.65f;


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
    const PXI16U borderInwardOffset = penSize - 1;

    if(penSize == 0)
    {
        return PXActionSuccessful;
    }

    const HPEN penLight = CreatePen(penStyle, penSize, highColor);
    const HPEN penDark = CreatePen(penStyle, penSize, lowColor);

    // Bottom, right, dark line
    MoveToEx(pxWindow->DeviceContextHandle, width - borderInwardOffset, y + borderInwardOffset, PXNull);
    SelectPen(pxWindow->DeviceContextHandle, penDark);
    LineTo(pxWindow->DeviceContextHandle, width - borderInwardOffset, height - borderInwardOffset);
    LineTo(pxWindow->DeviceContextHandle, x + borderInwardOffset, height - borderInwardOffset);

    SelectPen(pxWindow->DeviceContextHandle, penLight);
    LineTo(pxWindow->DeviceContextHandle, x + borderInwardOffset, y + borderInwardOffset);
    LineTo(pxWindow->DeviceContextHandle, width - borderInwardOffset, y + borderInwardOffset);

    DeletePen(penLight);
    DeletePen(penDark);
#endif

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawHeader(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

    const PXBool isHovered = (PXResourceInfoSelected & pxWindow->Info.Behaviour) > 0;

    PXWindowExtendedMenuItem* const pxWindowExtendedMenuItem = (PXWindowExtendedMenuItem*)pxWindow->ExtendedData;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI-Draw",
        "Header",
        "Header"
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
    */




    int y = 0;
    int height = 16;

    PXRectangleXYWHI32 block = pxWindow->Position.Form;

    for(PXSize i = 0; i < pxWindowExtendedMenuItem->MenuItemAmount; ++i)
    {
        PXWindowMenuItem* const pxWindowMenuItem = &pxWindowExtendedMenuItem->MenuItemList[i];


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
            pxGUITheme->BrushBackgroundDark,
            PXGUIDrawModeBack
        );
        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxGUITheme->BrushWhite,
            PXGUIDrawModeFront
        );


#if 1
        PXGUIDrawRectangle3D
        (
            pxWindow,
            block.X,
            block.Y,
            block.Width,
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
            pxGUITheme->BrushBackgroundDark,
            PXGUIDrawModeBack
        );
        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxGUITheme->BrushWhite,
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

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawFooter(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "Footer"
    );
#endif

    PXGUIDrawRectangle3D
    (
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height
    );

    PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);





    PXProcessor pxProcessor;
    PXProcessorFetchInfo(&pxProcessor);

    //pxWindowDrawInfo->Content = pxProcessor.BrandName;
    //pxWindowDrawInfo->ContentSize = pxProcessor.BrandNameSize;

    PXGUIDrawText
    (
        pxWindow,
        pxWindowDrawInfo
    );

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawViewTitle(PXWindow* const pxWindow, PXIcon* const pxIcon, PXRectangleXYWHI32* const pxRectangleXYWHI32, const char* const title, const PXSize titleSize)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawViewEntry(PXWindow* const pxWindow, PXIcon* const pxIcon, PXRectangleXYWHI32* const pxRectangleXYWHI32, const char* const title, const PXSize titleSize)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawResourceInfo(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "PXID:%i, ResourceInfo",
        pxWindow->Info.ID
    );
#endif

    PXResourceInfo* pxResourceInfo = 0;

    PXDictionaryEntry pxDictionaryEntry;

    PXDictionary* pxDictionary = &PXResourceManagerGet()->GUIElementLookup;

    PXDictionaryIndex(pxDictionary, 0, &pxDictionaryEntry);

    pxResourceInfo = &(*(PXWindow**)pxDictionaryEntry.Value)->Info;

    PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);
    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushWhite,
        PXGUIDrawModeFront
    );
    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushBackgroundDark,
        PXGUIDrawModeBack
    );


    char nameID[32];

    PXSize textSize = PXTextPrintA(nameID, 32, "ID: %i", pxResourceInfo->ID);

    PXNativDrawA
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height,
        nameID,
        textSize,
        PXWindowAllignLeft
    );

    PXIcon pxIcon;

    PXGUIIconLoad(&pxIcon);
    
    

    int left = pxWindow->Position.Form.X;
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
        PXUIElementPosition pxUIElementPositionPrev = pxWindow->Position;

        pxWindow->Position.Form.X = left;
        pxWindow->Position.Form.Y = y;
        // pxWindow->Position.Right = pxWindow->Position.Right;
        pxWindow->Position.Form.Height = y + height;
        pxWindow->Info.Setting &= ~PXWindowAllignFlags;
        pxWindow->Info.Setting |= PXWindowAllignLeft;


        PXNativDrawIconDraw
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            &pxIcon,
            pxWindow->Position.Form.X + 2,
            pxWindow->Position.Form.Y,
            16,
            16
        );

        pxWindow->Position.Form.X += 16 + 4;


        PXSize len = PXTextLengthA(table[i], 30);

        PXNativDrawA
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxWindow->Position.Form.X,
            pxWindow->Position.Form.Y,
            pxWindow->Position.Form.Width,
            pxWindow->Position.Form.Height,
            table[i],
            len,
            PXWindowAllignLeft
        );



        pxWindow->Position.Form.X += 120;

        char* textxx = (pxResourceInfo->Behaviour & (1 << i)) != 0 ? "Yes" : "No";
        len = PXTextLengthA(textxx, 4);

        PXNativDrawA
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxWindow->Position.Form.X,
            pxWindow->Position.Form.Y,
            pxWindow->Position.Form.Width,
            pxWindow->Position.Form.Height,
            textxx,
            len,
            PXWindowAllignLeft
        );


        pxWindow->Position = pxUIElementPositionPrev;


        y += height + 3;
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawTabList(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
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

        return PXActionRefusedArgumentInvalid;
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
        PXWindow* const pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

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


        const PXBool isHovered = (PXResourceInfoSelected & pxWindowTABPage->Info.Behaviour) > 0;


        PXWindowBrush* brushFront = pxGUITheme->BrushWhite;
        PXWindowBrush* brushBackground = pxGUITheme->BrushBackgroundDark;

        if(isHovered)
        {
            brushBackground = pxGUITheme->BrushWhite;
            brushFront = pxGUITheme->BrushBackgroundDark;
        }

        PXNativDrawColorSetBrush
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

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawFailback(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawText(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return 0;

    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

    const char* text = pxWindowDrawInfo->Content ? pxWindowDrawInfo->Content : pxWindow->NameContent;
    const PXSize textSize = pxWindowDrawInfo->Content ? pxWindowDrawInfo->ContentSize : pxWindow->NameContentSize;

    const int shadowOffset = 1;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "GUI-Draw",
        "Text",
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i - %i %s",
        pxWindow->Info.ID,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height,
        pxWindow->NameContentSize,
        pxWindow->NameContent
    );
#endif

    // Draw background
    {
        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxGUITheme->BrushWhite,
            PXGUIDrawModeFront
        );

        PXNativDrawColorSetBrush
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            pxGUITheme->BrushBackgroundDark,
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
            pxGUITheme->BrushBackgroundDark
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
            pxGUITheme->BrushWhite->ColorDate.Red * 0.25f,
            pxGUITheme->BrushWhite->ColorDate.Green * 0.25f,
            pxGUITheme->BrushWhite->ColorDate.Blue * 0.25f,
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
            pxGUITheme->BrushWhite,
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

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawButton(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "Button"
    );
#endif

    PXGUIDrawRectangle3D
    (
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height
    );

    PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);

    PXNativDrawA
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height,
        pxWindow->NameContent,
        pxWindow->NameContentSize,
        PXWindowAllignLeft
    );

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawComboBox(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawColorPicker(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    HDC hdc = pxWindow->DeviceContextHandle;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "ColorPicker"
    );
#endif

    PXNativDrawRectangle
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height,
        _pxGUIManager.ThemeSelected->BrushBackgroundDark
    );


    PXF32 scaling = 0.80f;
    int width = (pxWindow->Position.Form.Width - pxWindow->Position.Form.X) / 2;
    int height = (pxWindow->Position.Form.Height - pxWindow->Position.Form.Y) / 2;
    int widthS = (pxWindow->Position.Form.Width * scaling - pxWindow->Position.Form.X * scaling) / 2;
    int heightS = (pxWindow->Position.Form.Height * scaling - pxWindow->Position.Form.Y * scaling) / 2;

    int precision = 8;

    PXF32 r = 250;

#if OSUnix
#elif OSWindows
    HBRUSH brush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, brush);

    {
        TRIVERTEX vertexList[120];
        GRADIENT_TRIANGLE gRect[120];

        for(size_t i = 0; i < precision; ++i)
        {
            TRIVERTEX* const vertex = &vertexList[i];
            GRADIENT_TRIANGLE* const index = &gRect[i];

            const PXF32 steps = 360.f / (PXF32)precision;
            PXF32 degree = (steps * (i + 1)) * (3.14f / 180.f);

            PXF32 x = PXMathCosinusRADF32(degree);
            PXF32 y = PXMathSinusRADF32(degree);

            vertex->x = r * x + width;
            vertex->y = r * y + height;
            vertex->Red = 0xFF;
            vertex->Green = ((i) / (PXF32)precision) * 0xFF;
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

        for(PXSize i = 0; i < pxColorCircle.Precision; ++i)
        {
            int indexA = i;
            int indexB = (i + 1) % pxColorCircle.Precision;

            PXColorCircleVertex* dataA = &pxColorCircleVertexList[indexA];
            PXColorCircleVertex* dataB = &pxColorCircleVertexList[indexB];

            // TRIVERTEX* vertexA = &vertexList[i];
            // TRIVERTEX* vertexB = &vertexList[indexB];
            const COLORREF color = RGB(dataA->Red, dataA->Green, dataA->Blue);
            const HPEN hPen = CreatePen(PS_SOLID, 20, color);
            SelectObject(hdc, hPen);

            MoveToEx(hdc, dataA->X, dataA->Y, NULL);
            LineTo(hdc, dataB->X, dataB->Y);

        }

        GradientFill
        (
            hdc,
            vertexList,
            pxColorCircle.Precision * 3, 
            &gRect,
            1, 
            GRADIENT_FILL_TRIANGLE
        );














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
        vertices[1].Red = (pxColorRGBAI8.Red / (PXF32)0xFF) * 0xFFFF;
        vertices[1].Green = (pxColorRGBAI8.Green / (PXF32)0xFF) * 0xFFFF;
        vertices[1].Blue = (pxColorRGBAI8.Blue / (PXF32)0xFF) * 0xFFFF;
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
        GradientFill(hdc, vertices, 3, &gRectaaa, 1, GRADIENT_FILL_TRIANGLE);



    }


#endif


    /*

    int width = 128;
    int height = 128;
    PXF32 scale = 80.5;

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

            rect.left = pxWindow->Position.Left - rect.left;
            rect.top = pxWindow->Position.Top - rect.top;
            rect.right = pxWindow->Position.Right - rect.right;
            rect.bottom = pxWindow->Position.Bottom - rect.bottom;

            FillRect(pxWindow->DeviceContextHandle, &rect, brush);
            DeleteObject(brush);
        }
    }
    */




    //   PXWindowDrawA(pxGUISystem, pxWindow, pxWindow->NameContent, pxWindow->NameContentSize);

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXGUIDrawHexView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawFileDirectoryView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    PXDirectorySearchCache* const pxDirectorySearchCache = (PXDirectorySearchCache*)pxWindow->ExtendedData;
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "FileDirectoryView"
    );
#endif

    PXText pxTExt;
    PXTextMakeFixedA(&pxTExt, "./");

    PXDirectorySearch(pxDirectorySearchCache, &pxTExt);

    PXWindow pxWindowSub;
    PXCopy(PXWindow, pxWindow, &pxWindowSub);
    pxWindowSub.Info.Behaviour = PXResourceInfoOK;
    pxWindowSub.Info.Setting = PXWindowAllignTop | PXWindowKeepHeight | PXWindowAllignLeft;
    pxWindowSub.Position.Form.Height = 20;
    pxWindowSub.Position.Form.X = 20;

    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushBackgroundDark,
        PXGUIDrawModeBack
    );
    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushWhite,
        PXGUIDrawModeFront
    );

    const PXSize iconSize = 16;

    if(0 == pxDirectorySearchCache->EntryList.EntryAmountUsed)
    {
        char buffer[] = "**No files**";
        PXSize bufferSize = sizeof(buffer);

        PXNativDrawA
        (
            &_pxGUIManager.NativDraw,
            &pxWindowSub,
            pxWindow->Position.Form.X + 16,
            pxWindow->Position.Form.Y + 16,
            pxWindow->Position.Form.Width,
            pxWindow->Position.Form.Height,
            buffer,
            bufferSize,
            PXWindowAllignLeft
        );
    }


    for(PXSize i = 0; i < pxDirectorySearchCache->EntryList.EntryAmountUsed; ++i)
    {
        PXFileEntry* const pxFileEntry = PXListItemAtIndexGetT(PXFileEntry, &pxDirectorySearchCache->EntryList, i);

        PXIcon pxIcon;

        const PXActionResult iconResult = PXGUIIconGetViaFilePath(&pxIcon, pxFileEntry->FilePathData);

        PXI32S rowY = (pxWindowSub.Position.Form.Height * (i));

        PXNativDrawIcon
        (
            &_pxGUIManager.NativDraw,
            pxWindow,
            &pxIcon,
            2,
            pxWindowSub.Position.Form.Y + rowY + 2,
            iconSize
        );

        /*

        switch(pxFileEntry->Type)
        {
            default:
            case PXFileElementInfoTypeFile:
            {
                icon = fileIcon;

        #if 0

                PXText pxA;
                PXTextConstructFromAdressA(&pxA, pxFileEntry->FilePathData, pxFileEntry->FilePathSize, pxFileEntry->FilePathSize);

                PXText pxTextB;
                PXTextConstructBufferA(&pxTextB, 64);

                PXSize extensionSize = PXFilePathExtensionGet(&pxA, &pxTextB);

                if(extensionSize != 0)
                {
                    HICON smsm = PXGUIIconGetViaExtension(pxTextB.A);

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

        PXNativDrawA
        (
            &_pxGUIManager.NativDraw,
            &pxWindowSub,
            pxWindow->Position.Form.X + iconSize + 8,
            pxWindow->Position.Form.Y + rowY,
            pxWindow->Position.Form.Width,
            pxWindow->Position.Form.Height,
            pxFileEntry->FilePathData,
            pxFileEntry->FilePathSize,
            PXWindowAllignLeft
        );
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawCode(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawHexEditor(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawGraphBehaviour(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "GraphBehaviour\n\n"
    );
#endif

    PXNativDrawClear(&_pxGUIManager.NativDraw, pxWindow);

    PXGUIDrawRectangle3D
    (
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height
    );

    PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);


    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushBackgroundDark,
        PXGUIDrawModeBack
    );
    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushWhite,
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
        pxWindow->NameContent,
        pxWindow->NameContentSize,
        PXWindowAllignCenter
    );

    return PXActionSuccessful;
}

PXResult PXAPI PXGUIDrawGraphTime(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    PXGUITheme* pxGUITheme = _pxGUIManager.ThemeSelected;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Draw",
        "GraphTime"
    );
#endif

    PXGUIDrawRectangle3D
    (
        pxWindow,
        pxWindow->Position.Form.X,
        pxWindow->Position.Form.Y,
        pxWindow->Position.Form.Width,
        pxWindow->Position.Form.Height
    );

    PXNativDrawFontSelect(&_pxGUIManager.NativDraw, pxWindow, pxGUITheme->FontContent);


    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushBackgroundDark,
        PXGUIDrawModeBack
    );
    PXNativDrawColorSetBrush
    (
        &_pxGUIManager.NativDraw,
        pxWindow,
        pxGUITheme->BrushWhite,
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
        pxWindow->NameContent,
        pxWindow->NameContentSize,
        PXWindowAllignLeft
    );

    return PXActionRefusedNotImplemented;
}

PXResult PXAPI PXGUIDrawSoundPlayerMixer(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawVideoCutter(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawDataBase(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawNetwork(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawInputView(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}

PXResult PXAPI PXGUIDrawHardwareInfo(PXWindow* const pxWindow, PXWindowDrawInfo* const pxWindowDrawInfo)
{
    return PXActionInvalid;
}




















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
#include <PX/Engine/PXEngine.h>
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






void PXMathCircle(PXColorCircle* const pxColorCircle)
{
    for(size_t i = 0; i < pxColorCircle->Precision; ++i)
    {
        PXColorCircleVertex* vertex = &pxColorCircle->VertexList[i];

        const PXF32 steps = 360.f / (PXF32)pxColorCircle->Precision;
        const PXF32 degree = (steps * (i + 0));
        const PXF32 rad = degree * (3.14f / 180.f);

        vertex->X = pxColorCircle->StartX + pxColorCircle->Size * PXMathCosinusRADF32(rad);
        vertex->Y = pxColorCircle->StartY + pxColorCircle->Size * PXMathSinusRADF32(rad);


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

        const PXF32 steps = 360.f / (PXF32)3;
        const PXF32 degree = (int)((steps * (i + 0)) + pxColorCircle->ColorSelected.Hue) % 360;
        const PXF32 rad = degree * (3.14f / 180.f);

        vertex->X = pxColorCircle->StartX + pxColorCircle->Size * 0.85f * PXMathCosinusRADF32(rad);
        vertex->Y = pxColorCircle->StartY + pxColorCircle->Size * 0.85f * PXMathSinusRADF32(rad);
    }
}

void PXWindowsChildShow(PXWindow* const pxWindow)
{
    for(PXWindow* child = pxWindow->Info.Hierarchy.ChildFirstborn; child; child = child->Info.Hierarchy.ChildFirstborn)
    {
        PXWindowsChildShow(child);

        child->Info.Behaviour |= PXResourceInfoSelected | PXResourceInfoRender;

        ShowWindow(child->Info.Handle.WindowHandle, SW_SHOW);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXGUIName,
            "SHOW",
            "PXID:%i, %s",
            child->Info.ID,
            child->NameContent
        );
#endif
    }


    pxWindow->Info.Behaviour |= PXResourceInfoSelected | PXResourceInfoRender;

    ShowWindow(pxWindow->Info.Handle.WindowHandle, SW_SHOW);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "SHOW",
        "PXID:%i, %s, %p",
        pxWindow->Info.ID,
        pxWindow->NameContent,
        pxWindow->DrawFunction
    );
#endif

#if 1
    if(pxWindow->DrawFunction)
    {
        pxWindow->DrawFunction(pxWindow, PXNull);
    }
#endif
}

void PXWindowsChildHide(PXWindow* const pxWindow)
{
    for(PXWindow* child = pxWindow->Info.Hierarchy.ChildFirstborn; child; child = child->Info.Hierarchy.ChildFirstborn)
    {
        PXWindowsChildHide(child);

        child->Info.Behaviour &= ~PXResourceInfoSelected;
        child->Info.Behaviour &= ~PXResourceInfoRender;

        ShowWindow(child->Info.Handle.WindowHandle, SW_HIDE);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXGUIName,
            "HIDE",
            "PXID:%i, %s",
            child->Info.ID,
            child->NameContent
        );
#endif
    }

    pxWindow->Info.Behaviour &= ~PXResourceInfoSelected;
    pxWindow->Info.Behaviour &= ~PXResourceInfoRender;

    ShowWindow(pxWindow->Info.Handle.WindowHandle, SW_HIDE);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "HIDE",
        "PXID:%i, %s",
        pxWindow->Info.ID,
        pxWindow->NameContent
    );
#endif
}

PXResult PXAPI  PXWindowTabListSwapPage(PXWindow* const pxWindow)
{
    PXWindowExtendedBehaviourTab* pxWindowExtendedBehaviourTab = (PXWindowExtendedBehaviourTab*)pxWindow->ExtendedData;

    ++pxWindowExtendedBehaviourTab->TABPageIndexCurrent;

    pxWindowExtendedBehaviourTab->TABPageIndexCurrent %= pxWindowExtendedBehaviourTab->TABPageAmount;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "TAB",
        "Index swap : %i/%i",
        pxWindowExtendedBehaviourTab->TABPageIndexCurrent + 1,
        pxWindowExtendedBehaviourTab->TABPageAmount
    );
#endif

    PXWindow* shownPage = 0;

    for(PXSize i = 0; i < pxWindowExtendedBehaviourTab->TABPageAmount; ++i)
    {
        PXWindow* const pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

        if(pxWindowExtendedBehaviourTab->TABPageIndexCurrent == i)
        {
            shownPage = pxWindowTABPage;
        }
        else
        {
            PXWindowsChildHide(pxWindowTABPage);
        }

        // InvalidateRect(pxWindowTABPage->Info.Handle.WindowHandle, 0, TRUE);
    }

    PXWindowsChildShow(shownPage);

    //  pxWindow->DrawFunction(PXNativDrawInstantance()->GUISystem, pxWindow, PXNull);

      //InvalidateRect(pxWindow->Info.Handle.WindowHandle, 0, TRUE);

    return PXActionSuccessful;
}


PXBool PXAPI PXWindowValueFetch(PXWindow* const pxUIElementList, const PXSize dataListAmount, const PXUIElementProperty pxUIElementProperty, void* const dataList)
{
    for(size_t i = 0; i < dataListAmount; ++i)
    {
        PXWindow* const pxWindow = &pxUIElementList[i];

        switch(pxUIElementProperty)
        {
            case PXUIElementPropertySliderPercentage:
            {
                PXF32* target = &((PXF32*)dataList)[i];

#if OSUnix

#elif OSWindows
                * target = SendMessageA(pxWindow->Info.Handle.WindowHandle, TBM_GETPOS, 0, 0) / 100.f;
#endif

                break;
            }

            default:
                break;
        }
    }

    return PXTrue;
}

PXResult PXAPI  PXWindowStyleUpdate(PXGUISystem* const pxGUISystem, PXWindow* const pxWindow)
{
    PXActionResult result = PXActionInvalid;

#if OSUnix
    result = PXActionRefusedNotImplemented;
#elif OSWindows
    SetWindowLongPtr(pxWindow->Info.Handle.WindowHandle, GWL_STYLE, WS_SYSMENU); //3d argument=style
    const PXBool setSuccess = SetWindowPos
    (
        pxWindow->Info.Handle.WindowHandle,
        HWND_TOPMOST,
        100,
        100,
        100,
        100,
        SWP_SHOWWINDOW
    );
#else
    result = PXActionRefusedNotSupportedByLibrary;
#endif

    return result;
}

PXResult PXAPI  PXWindowCreate(PXGUISystem* const pxGUISystem, PXResourceCreateInfo* const pxResourceCreateInfo, const PXSize amount)
{
    if(!(pxGUISystem && pxResourceCreateInfo))
    {
        return PXActionRefusedArgumentNull;
    }

    PXWindowCreateInfo* pxWindowCreateInfo = &pxResourceCreateInfo->UIElement;
    PXWindow* const pxWindowCurrent = *(PXWindow**)pxResourceCreateInfo->ObjectReference;
    PXWindow* const pxWindowParent = pxWindowCreateInfo->WindowParent;

    pxWindowCurrent->Type = pxWindowCreateInfo->Type;
    pxWindowCurrent->InteractCallBack = pxWindowCreateInfo->InteractCallBack;
    pxWindowCurrent->InteractOwner = pxWindowCreateInfo->InteractOwner;
    pxWindowCurrent->Info.Hierarchy.Parrent = pxWindowParent;
    pxWindowCurrent->Info.Behaviour |= pxWindowCreateInfo->Behaviour;
    pxWindowCurrent->Info.Setting |= pxWindowCreateInfo->Setting;
    //pxWindowCurrent->BrushFront = _pxGUIManager.BrushWhite;
    //pxWindowCurrent->BrushBackground = _pxGUIManager.BrushBackgroundDark;
    //pxWindowCurrent->NameContent = pxResourceCreateInfo->Name; // This breaks current names!

    PXCopy(PXUIElementPosition, &pxWindowCreateInfo->Position, &pxWindowCurrent->Position);


    // Resize

    PXWindowPropertyInfo pxWindowSizeInfo;
    PXClear(PXWindowPropertyInfo, &pxWindowSizeInfo);
    //sizeInfoAA.UIElement = *pxWindowCreateInfo->UIElement;
    pxWindowSizeInfo.WindowParentREF = pxWindowCreateInfo->WindowCurrent;
    pxWindowSizeInfo.Property = PXUIElementPropertySizeParent;

    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

    // PXWindowSizeInfo pxWindowSizeInfo;

    // PXWindowID windowID = pxWindowCreateInfo->UIElementWindow ? pxWindowCreateInfo->UIElementWindow->ID : PXNull;

    //  PXWindowSizeGet(windowID, &pxWindowSizeInfo);

    PXWindowFetch(pxGUISystem, &pxWindowSizeInfo, 1);

    pxUIElementPositionCalulcateInfo.WindowWidth = pxWindowSizeInfo.Data.Size.Width;
    pxUIElementPositionCalulcateInfo.WindowHeight = pxWindowSizeInfo.Data.Size.Height;

    if(pxUIElementPositionCalulcateInfo.WindowWidth == 0)
    {
        pxUIElementPositionCalulcateInfo.WindowWidth = 400;
        pxUIElementPositionCalulcateInfo.WindowHeight = 500;
    }

    PXUIElementPositionCalculcate(pxWindowCurrent, &pxUIElementPositionCalulcateInfo);


    if(pxWindowParent)
    {
        // Validate, if a parent is set, if it has a valid creation handle
        const PXActionResult validResult = PXNativDrawWindowIDValid(&_pxGUIManager.NativDraw, pxWindowParent->Info.Handle.WindowHandle);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXGUIName,
            "Window",
            "Attempting to create window with parent..\n"
            "%10s - [PX-ID:%i], X:%4i Y:%4i W:%4i H:%4i, Name:%s, NEW\n"
            "%10s - [PX-ID:%i], X:%4i Y:%4i W:%4i H:%4i, Name:%s, HANDLE:%s",
            "Self",
            pxWindowCurrent->Info.ID,
            (int)pxUIElementPositionCalulcateInfo.X,
            (int)pxUIElementPositionCalulcateInfo.Y,
            (int)pxUIElementPositionCalulcateInfo.Width,
            (int)pxUIElementPositionCalulcateInfo.Height,
            pxResourceCreateInfo->NameAdress,
            "Parent",
            pxWindowParent->Info.ID,
            pxWindowParent->Position.Form.X,
            pxWindowParent->Position.Form.Y,
            pxWindowParent->Position.Form.Width,
            pxWindowParent->Position.Form.Height,
            pxWindowParent->NameContent,
            validResult ? "OK" : "INVALID"
        );
#endif
    }
    else
    {
        // Calc size
        {
            const PXBool isDefaultSize = 1;// windowInfo->Width == 0 && windowInfo->Height == 0;

            if(isDefaultSize)
            {
                PXRectangleXYWHI32 pxRectangleXYWH;

                PXNativDrawScreenSizeGet(&pxRectangleXYWH);

                pxUIElementPositionCalulcateInfo.X = pxRectangleXYWH.Width * 0.125f;
                pxUIElementPositionCalulcateInfo.Y = pxRectangleXYWH.Height * 0.125f;
                pxUIElementPositionCalulcateInfo.Width = pxRectangleXYWH.Width * 0.75f;
                pxUIElementPositionCalulcateInfo.Height = pxRectangleXYWH.Height * 0.75f;
            }
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXGUIName,
            "Window",
            "Attempting to create global window..\n"
            "%10s PX-ID:%i, X:%4i Y:%4i W:%4i H:%4i, Name:%s",
            "",
            pxWindowCurrent->Info.ID,
            (int)pxUIElementPositionCalulcateInfo.X,
            (int)pxUIElementPositionCalulcateInfo.Y,
            (int)pxUIElementPositionCalulcateInfo.Width,
            (int)pxUIElementPositionCalulcateInfo.Height,
            pxResourceCreateInfo->NameAdress
        );
#endif
    }



    //-----------------------------------------------------
    // Setup flags and draw function callbacks
    //-----------------------------------------------------
#if OSWindows
    pxWindowCreateInfo->WindowsStyleFlags = WS_CLIPCHILDREN | SS_NOTIFY; // WS_CLIPSIBLINGS

    if(!pxWindowCreateInfo->Invisible)
    {
        pxWindowCreateInfo->WindowsStyleFlags |= WS_VISIBLE;
    }

    if(PXWindowBehaviourBorder & pxWindowCreateInfo->Setting)
    {
        pxWindowCreateInfo->WindowsStyleFlags |= WS_BORDER;
    }

    if(pxWindowCreateInfo->WindowParent)
    {
        pxWindowCreateInfo->WindowsStyleFlags |= WS_CHILD;
    }

    if(PXWindowBehaviourSelectable & pxWindowCreateInfo->Setting)
    {
        pxWindowCreateInfo->WindowsStyleFlags |= WS_TABSTOP;
    }
#endif

    // Default
#if OSWindows
    pxWindowCreateInfo->WindowsClassName = WC_STATIC;
#endif

    switch(pxWindowCurrent->Type)
    {
        case PXUIElementTypePanel:
        {
#if OSWindows

            // pxWindowCreateInfo->WindowsStyleFlags |= SS_BLACKFRAME;
#endif

            break;
        }
        case PXUIElementTypeText:
        {
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawText;

            /*
            PXUIElementTextInfo* const pxUIElementTextInfo = &pxWindowCreateInfo->Data.Text;

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

            break;
        }
        case PXUIElementTypeButton:
        {
            // BS_DEFPUSHBUTTON
            // BS_USERBUTTON
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_BUTTON;
            pxWindowCreateInfo->WindowsTextContent = pxWindowCreateInfo->Data.Button.TextInfo.Content;
#endif
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawButton;

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
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_COMBOBOX;
            pxWindowCreateInfo->WindowsStyleFlags |= CBS_HASSTRINGS | CBS_DROPDOWNLIST | WS_OVERLAPPED;
#endif
            break;
        }
        case PXUIElementTypeListBox:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_LISTBOX;
#endif
            break;
        }
        case PXUIElementTypeTextEdit:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_EDIT;
            pxWindowCreateInfo->WindowsStyleFlags |= ES_MULTILINE;
            pxWindowCreateInfo->WindowsTextContent = pxWindowCreateInfo->Data.Button.TextInfo.Content;
#endif
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawText;

            break;
        }
        case PXUIElementTypeRichEdit:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = "RICHEDIT_CLASS";
#endif
            break;
        }
        case PXUIElementTypeScrollBar:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_SCROLLBAR;
#endif
            break;
        }
        case PXUIElementTypeTrackBar:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = TRACKBAR_CLASS;
            pxWindowCreateInfo->WindowsStyleFlags |= TBS_AUTOTICKS | TBS_TOOLTIPS;
#endif
            break;
        }
        case PXUIElementTypeStatusBar:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = STATUSCLASSNAMEA;
            //pxWindowCreateInfo->WindowsStyleFlags |= SBARS_SIZEGRIP;


            pxWindowCreateInfo->WindowsClassName = WC_STATIC;
#endif
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawFooter;

            pxWindowCurrent->Info.Behaviour &= ~PXResourceInfoUseByMask;
            pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;

            break;
        }
        case PXUIElementTypeUpDown:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = UPDOWN_CLASS;
#endif
            break;
        }
        case PXUIElementTypeProgressBar:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = PROGRESS_CLASS;
#endif
            break;
        }
        case PXUIElementTypeHotKey:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = HOTKEY_CLASS;
#endif
            break;
        }
        case PXUIElementTypeCalender:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = MONTHCAL_CLASS;
#endif
            break;
        }
        case PXUIElementTypeToolTip:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = TOOLTIPS_CLASS;
#endif
            break;
        }
        case PXUIElementTypeAnimate:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = ANIMATE_CLASS;
#endif
            break;
        }
        case PXUIElementTypeDatePicker:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = DATETIMEPICK_CLASS;
#endif
            break;
        }
        case PXUIElementTypeGroupBox:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_BUTTON;
            pxWindowCreateInfo->WindowsStyleFlags |= BS_GROUPBOX;
#endif
            break;
        }
        case PXUIElementTypeRadioButton:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_BUTTON;
            pxWindowCreateInfo->WindowsStyleFlags |= BS_RADIOBUTTON;
#endif
            break;
        }
        case PXUIElementTypeGroupRadioButton:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_BUTTON;
            pxWindowCreateInfo->WindowsStyleFlags |= BS_AUTORADIOBUTTON;
#endif
            break;
        }
        case PXUIElementTypeTreeView:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_TREEVIEW;
            pxWindowCreateInfo->WindowsStyleFlags |=
                TVIF_TEXT |
                TVIF_IMAGE |
                TVIF_SELECTEDIMAGE |
                TVIF_PARAM | // Required to get the a selected item, otherwise its just NULL.
                TVS_HASBUTTONS |
                TVS_HASLINES |
                TVS_LINESATROOT;
#endif
            break;
        }
        case PXUIElementTypeFileDirectyView:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_STATIC;
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawFileDirectoryView;
#endif

            pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;
            break;
        }
        case PXUIElementTypeIPInput:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_IPADDRESS;
#endif
            break;
        }
        case PXUIElementTypeLink:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = "SysLink"; // WC_LINK
#endif
            break;
        }
        case PXUIElementTypeHeader:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_HEADER;
#endif
            break;
        }
        case PXUIElementTypeFontSelector:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_NATIVEFONTCTL;
            // NFS_USEFONTASSOC
#endif
            break;
        }
        case PXUIElementTypePageScroll:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_PAGESCROLLER;
#endif
            break;
        }
        case PXUIElementTypeTabControll:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_TABCONTROL;
            //pxWindowCreateInfo->WindowsStyleFlags |= WS_CLIPSIBLINGS | TCS_BUTTONS;

            pxWindowCreateInfo->WindowsClassName = WC_STATIC;
#endif
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawTabList;
            break;
        }
        case PXUIElementTypeTabPage:
        {
            break;
        }
        case PXUIElementTypeToggle:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = 00000000000000000000;
            pxWindowCreateInfo->WindowsStyleFlags = 000000000000000000000000;
#endif
            break;
        }
        case PXUIElementTypeColorPicker:
        {
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawColorPicker;
            break;
        }
        case PXUIElementTypeSlider:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = 00000000000000000000;
            pxWindowCreateInfo->WindowsStyleFlags = 000000000000000000000000;
#endif
            break;
        }
        case PXUIElementTypeCheckBox:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_BUTTON;
            pxWindowCreateInfo->WindowsStyleFlags |= BS_CHECKBOX;

            //CheckDlgButton(uiCheckBox->ID, 1, BST_CHECKED); // BST_UNCHECKED
#endif

            break;
        }
        case PXUIElementTypeComboBox:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsClassName = WC_COMBOBOXEX;
#endif
            break;
        }
        case PXUIElementTypeRenderFrame:
        {
#if OSWindows
            pxWindowCreateInfo->WindowsStyleFlags |= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
#endif
            break;
        }
        case PXUIElementTypeResourceManger:
        {
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawView;
            break;
        }
        case PXUIElementTypeResourceInfo:
        {
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawResourceInfo;
            break;
        }
        case PXUIElementTypeWindow:
        {
            PXWindowCreateWindowInfo* const windowInfo = &pxWindowCreateInfo->Data.Window;

#if OSWindows
            pxWindowCreateInfo->WindowsClassName = "PXWindow";


            pxWindowCreateInfo->WindowsWindowsStyleFlagsExtended =
                WS_EX_APPWINDOW |
                WS_EX_DLGMODALFRAME |
                WS_EX_CONTEXTHELP;

            pxWindowCreateInfo->WindowsStyleFlags |=
                WS_OVERLAPPEDWINDOW |
                CS_OWNDC |
                CS_HREDRAW |
                CS_VREDRAW;

            PXWindowHandle windowID = 0;

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
                wndclass.lpfnWndProc = PXNativDrawEventTranslator;
                wndclass.cbClsExtra = 0;
                wndclass.cbWndExtra = 0;
                wndclass.hInstance = GetModuleHandleA(NULL);
                wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
                wndclass.hCursor = PXNull;
                wndclass.hbrBackground = CreateSolidBrush(RGB(windowInfo->BackGroundColor.Red, windowInfo->BackGroundColor.Green, windowInfo->BackGroundColor.Blue));
                wndclass.lpszMenuName = 0;
                wndclass.lpszClassName = "PXWindow";


                const ATOM classID = RegisterClassA(&wndclass);
                const PXActionResult result = PXErrorCurrent(0 != classID);

                pxWindowCreateInfo->WindowsClassName = (char*)classID;
            }
#endif

            break;
        }
        case PXUIElementTypeMenuStrip:
        {
            if(1)
            {
                pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawHeader;
            }
            else
            {
                pxWindowCreateInfo->AvoidCreation = PXTrue;
            }

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            pxWindowCreateInfo->AvoidCreation = PXTrue;
            break;
        }
        case PXUIElementTypeScene:
        {

            break;
        }
        case PXUIElementTypeCode:
        {
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawCode;
            break;
        }
        case PXUIElementTypeHexEditor:
        {
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawHexEditor;
            break;
        }
        case PXUIElementTypeGraphBehaviour:
        {
            pxWindowCreateInfo->DrawFunctionEngine = PXGUIDrawGraphBehaviour;
            break;
        }
        case PXUIElementTypeGraphTime:
        {
            // pxWindowCreateInfo->DrawFunctionEngine = PXWindowDrawCustomGraphTime;
            break;
        }
        case PXUIElementTypeSoundPlayerMixer:
        {
            // pxWindowCreateInfo->DrawFunctionEngine = PXWindowDrawCustomSoundPlayerMixer;
            break;
        }
        case PXUIElementTypeVideoCutter:
        {
            // pxWindowCreateInfo->DrawFunctionEngine = PXWindowDrawCustomVideoCutter;
            break;
        }
        case PXUIElementTypeDataBaseManager:
        {
            // pxWindowCreateInfo->DrawFunctionEngine = PXWindowDrawCustomDataBaseManager;
            break;
        }
        case PXUIElementTypeNetworkTester:
        {
            // pxWindowCreateInfo->DrawFunctionEngine = PXWindowDrawCustomNetworkTester;

            break;
        }
        case PXUIElementTypeInputView:
        {
            // pxWindowCreateInfo->DrawFunctionEngine = PXWindowDrawCustomInputView;
            break;
        }
        case PXUIElementTypeHardwareInfo:
        {
            //pxWindowCreateInfo->DrawFunctionEngine = PXWindowDrawCustomHardwareInfo;
            break;
        }

        default:
            return PXActionRefusedArgumentInvalid;
    }


    // If we hav
    {
        if(pxWindowCreateInfo->DrawFunctionEngine)
        {
            pxWindowCurrent->DrawFunction = pxWindowCreateInfo->DrawFunctionEngine;
        }
        else
        {
            //pxWindowCurrent->DrawFunction = PXWindowDrawCustomFailback;
        }

        pxWindowCurrent->Info.Behaviour &= ~PXResourceInfoUseByMask;
        pxWindowCurrent->Info.Behaviour |= PXResourceInfoUseByEngine;

#if OSWindows
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
                    magicID = SS_OWNERDRAW | SS_BLACKRECT;
                    break;
            }

            pxWindowCreateInfo->WindowsStyleFlags |= magicID;
        }
#endif // OSWindows
    }
















#if OSUnix

    pxWindowCreateInfo->Simple = PXTrue;
    pxWindowCreateInfo->X = pxUIElementPositionCalulcateInfo.X;
    pxWindowCreateInfo->Y = pxUIElementPositionCalulcateInfo.Y;
    pxWindowCreateInfo->Width = pxUIElementPositionCalulcateInfo.Width;
    pxWindowCreateInfo->Height = pxUIElementPositionCalulcateInfo.Height;
    pxWindowCreateInfo->BorderWidth = 1;
    pxWindowCreateInfo->Border = 1;

    PXNativDrawWindowCreate(PXNativDrawInstantance(), pxWindowCurrent, pxWindowCreateInfo);


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


    const int selectResultID = XISelectEvents(_pxGUIManager.DisplayCurrent.DisplayHandle, _pxGUIManager.DisplayCurrent.WindowRootHandle, &eventmask, 1u);
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
            _pxGUIManager.DisplayCurrent.DisplayHandle,
            _pxGUIManager.DisplayCurrent.WindowRootHandle,
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
            pxWindowCurrent->Info.Handle.WindowHandle
        );
#endif
    }










#elif OSWindows

    // If we a text that should be rendered on a button, the OS does not really intent to do this.
    // To avoid wierd graphical bugs, we will merge these into one object.
    // Still this object needs to be seperate for other render systems
    // pxWindow->Type == PXUIElementTypeText&&;



    if(!pxWindowCreateInfo->AvoidCreation)
    {
        HINSTANCE hInstance = PXNull;
        PXWindowHandle windowID = pxWindowCreateInfo->WindowCurrent ? pxWindowCreateInfo->WindowCurrent->Info.Handle.WindowHandle : PXNull;
        //  PXNativDrawWindowHandle windowID = pxWindowCreateInfo->WindowParent ? pxWindowCreateInfo->WindowParent->Info.Handle.WindowHandle : PXNull;

        if(pxWindowCreateInfo->WindowParent)
        {
            //windowID = pxWindowCreateInfo->WindowParent->Info.Handle.WindowHandle;
        }

        if(pxWindowCreateInfo->WindowCurrent)
        {
            hInstance = (HINSTANCE)GetWindowLongPtr(pxWindowCreateInfo->WindowCurrent->Info.Handle.WindowHandle, GWLP_HINSTANCE);
            //windowID = pxWindowCreateInfo->UIElementWindow->ID;
        }
        else
        {
            hInstance = GetModuleHandle(NULL);
        }

        // const HINSTANCE hInstance = GetModuleHandle(NULL);



        PXWindowCreateInfo pxWindowCreateInfoSub;
        PXClear(PXWindowCreateInfo, &pxWindowCreateInfoSub);
        pxWindowCreateInfoSub.ParentID = windowID;
        pxWindowCreateInfoSub.InstanceHandle = hInstance;
        pxWindowCreateInfoSub.X = pxUIElementPositionCalulcateInfo.X;
        pxWindowCreateInfoSub.Y = pxUIElementPositionCalulcateInfo.Y;
        pxWindowCreateInfoSub.Width = pxUIElementPositionCalulcateInfo.Width;
        pxWindowCreateInfoSub.Height = pxUIElementPositionCalulcateInfo.Height;
        pxWindowCreateInfoSub.BorderWidth = 2;
        pxWindowCreateInfoSub.Border = 2;
        pxWindowCreateInfoSub.WindowsWindowsStyleFlagsExtended = pxWindowCreateInfo->WindowsWindowsStyleFlagsExtended;
        pxWindowCreateInfoSub.WindowsStyleFlags = pxWindowCreateInfo->WindowsStyleFlags;
        pxWindowCreateInfoSub.WindowsTextContent = pxWindowCreateInfo->WindowsTextContent;
        //    pxNativDrawWindow.WindowsTextSize;
        pxWindowCreateInfoSub.WindowsClassName = pxWindowCreateInfo->WindowsClassName;

        const PXActionResult pxActionResult = PXNativDrawWindowCreate(PXNativDrawInstantance(), pxWindowCurrent, &pxWindowCreateInfoSub);


        if(PXActionSuccessful != pxActionResult)
        {
            // Failed to create window

            return pxActionResult;
        }



        // Gegister drag&Drop
        IDropTarget dropTarget;

        const HRESULT dragResult = RegisterDragDrop(pxWindowCurrent->Info.Handle.WindowHandle, &dropTarget); // Windows 2000, Ole32.dll, ole2.h






#if 0
        if(pxWindow->ID && pxWindowCreateInfo->UIElementParent)
        {
            PXWindow* parent = pxWindowCreateInfo->UIElementParent;

            HDWP resA = BeginDeferWindowPos(1);

            if(PXUIElementTypeButtonText == pxWindow->Type)
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    parent->ID,
                    pxWindow->ID,
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
                    pxWindow->ID,
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

    HDC xx = GetDC(pxWindow->ID);

    HBRUSH brush = SendMessageA(windowHandle, WM_CTLCOLORSTATIC, xx, pxWindow->ID); // RB_SETBKCOLOR
    SetTextColor(xx, RGB(255, 0, 0));
    SetBkColor(xx, RGB(0, 255, 0));


    Pager_SetBkColor(pxWindow->ID, &colorAA);
#endif


    // SetTextColor(xx, colorAA);


    //LRESULT ww = SendMessageA(pxWindow->ID, EM_GETLINE, 0, buffer); // RB_SETBKCOLOR


    //  PXConsoleWriteA("\n\n%i     %i %s", brush, ww, buffer);


    // SendMessageA(pxWindow->ID, RB_SETTEXTCOLOR, 0, &colorAA);




#endif












    //-------------------------------------------------------------------------
    // POST-Update
    //-------------------------------------------------------------------------
    switch(pxWindowCurrent->Type)
    {
        case PXUIElementTypeMenuStrip:
        {
            PXWindowMenuItemList* const menuItemListInput = &pxWindowCreateInfo->Data.MenuItem;

            PXWindowExtendedMenuItem* menuItemListOut = PXMemoryHeapCallocT(PXWindowExtendedMenuItem, 1);

            pxWindowCurrent->ExtendedData = menuItemListOut;

            // setup extended data
            {
                menuItemListOut->MenuItemAmount = menuItemListInput->MenuItemInfoListAmount;
                menuItemListOut->MenuItemList = PXMemoryHeapCallocT(PXWindowMenuItem, menuItemListInput->MenuItemInfoListAmount);

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
                        PXWindowMenuItemInfo* const pxWindowMenuItemInfo = &menuItemListInput->MenuItemInfoListData[i];

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
                        menuItemInfo.dwTypeData = pxWindowMenuItemInfo->TextData;    // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
                        menuItemInfo.cch = pxWindowMenuItemInfo->TextSize;           // used if MIIM_TYPE (4.0) or MIIM_STRING (>4.0)
                        menuItemInfo.hbmpItem = 0;      // used if MIIM_BITMAP

                        UINT newID = i;

                        const PXBool itemAddResult = AppendMenuA(pxWindowCurrent->Info.Handle.MenuHandle, menuItemInfo.fState, &newID, menuItemInfo.dwTypeData);
                        // const PXBool itemAddResult = AppendMenuA(pxWindow->Info.Handle.MenuHandle, MF_STRING | MF_POPUP, (UINT_PTR)hSubMenu, menuItemInfo.dwTypeData);


                        const PXActionResult sdfsdfghg = PXErrorCurrent(itemAddResult);

                        PXWindowMenuItemList* sub = pxWindowMenuItemInfo->ChildList;

                        if(sub)
                        {
                            for(size_t i = 0; i < sub->MenuItemInfoListAmount; i++)
                            {
                                UINT newIEED = newID;

                                PXWindowMenuItemInfo* const pxWindowMenuItemInfo = &sub->MenuItemInfoListData[i];

                                const PXBool asddassdad = AppendMenuA(hSubMenu, menuItemInfo.fState | MF_POPUP, &newIEED, pxWindowMenuItemInfo->TextData);
                                const PXActionResult wewrerew = PXErrorCurrent(asddassdad);
                            }
                        }




                        //  const PXBool itemAddResult = InsertMenuItemA(pxWindow->Info.Handle.MenuHandle, 0, PXTrue, &menuItemInfo);
                        const PXActionResult res = PXErrorCurrent(itemAddResult);

                        // DrawMenuBar(pxWindow->Parent->Info.Handle.WindowHandle);

                        menuItemInfo.wID += 0;
                    }

                    PXWindow* parentElement = (PXWindow*)pxWindowCurrent->Info.Hierarchy.Parrent;
                    const PXBool setResult = SetMenu(parentElement->Info.Handle.WindowHandle, pxWindowCurrent->Info.Handle.MenuHandle);
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
            PXDirectorySearchCache* pxDirectorySearchCache = PXMemoryHeapCallocT(PXDirectorySearchCache, 1);

            pxWindowCurrent->ExtendedData = pxDirectorySearchCache;

            break;
        }
        case PXUIElementTypeText:
        {
            PXWindowPropertyInfo pxUIElementUpdateInfo[2];
            PXClearList(PXWindowPropertyInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].WindowCurrent = pxWindowCurrent;
            pxUIElementUpdateInfo[0].WindowParentREF = pxWindowCreateInfo->WindowParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;

            pxUIElementUpdateInfo[1].WindowCurrent = pxWindowCurrent;
            pxUIElementUpdateInfo[1].WindowParentREF = pxWindowCreateInfo->WindowParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyAllign;

            // PXWindowUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeButton:
        {
            PXWindowPropertyInfo pxUIElementUpdateInfo[2];
            PXClearList(PXWindowPropertyInfo, pxUIElementUpdateInfo, 2);

            pxUIElementUpdateInfo[0].WindowCurrent = pxWindowCurrent;
            pxUIElementUpdateInfo[0].WindowParentREF = pxWindowCreateInfo->WindowParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].WindowCurrent = pxWindowCurrent;
            pxUIElementUpdateInfo[1].WindowParentREF = pxWindowCreateInfo->WindowParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;


            //  PXWindowUpdate(pxGUISystem, pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            PXUIElementTreeViewItemInfo* const pxUIElementTreeViewItemInfo = &pxWindowCreateInfo->Data.TreeViewItem;
            // Create ui item for a tree view

#if OSUnix
#elif OSWindows

            TVINSERTSTRUCT item;
            PXClear(TVINSERTSTRUCT, &item);

            // item.item.iImage = 1;


            const char text[] = "[N/A]";

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                item.hParent = 0;// pxUIElementTreeViewItemInfo->ItemParent->Info.Handle.WindowHandle;
            }

            if(pxUIElementTreeViewItemInfo->TextDataOverride)
            {
                item.item.pszText = pxUIElementTreeViewItemInfo->TextDataOverride;
                item.item.cchTextMax = pxUIElementTreeViewItemInfo->TextSizeOverride;
            }
            else
            {
                item.item.pszText = pxWindowCreateInfo->WindowsTextContent;
                item.item.cchTextMax = pxWindowCreateInfo->WindowsTextSize;
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

            HTREEITEM itemID = TreeView_InsertItem(pxUIElementTreeViewItemInfo->TreeView->Info.Handle.WindowHandle, &item);

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                TreeView_Expand
                (
                    pxUIElementTreeViewItemInfo->TreeView->Info.Handle.WindowHandle,
                    pxUIElementTreeViewItemInfo->ItemParent->Info.Handle.WindowHandle,
                    TVE_EXPAND
                );
            }

            pxWindowCurrent->Info.Handle.WindowHandle = 0;// itemID;
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
            pxWindowCurrent->ExtendedData = PXMemoryHeapCallocT(PXWindowExtendedBehaviourResourceView, 1);



            break;
        }
        case PXUIElementTypeTextEdit:
        {




            break;
        }
        case PXUIElementTypeRichEdit:
        {


            // EM_SETBKGNDCOLOR(pxWindow->ID, RGB(10, 10, 10));
            // EM_SETTEXTCOLOR(pxWindow->ID, RGB(200, 200, 200));

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

            HDC dc = GetDC(pxWindowCurrent->Info.Handle.WindowHandle);


            SetTextColor(dc, RGB(255, 0, 0));
            SetBkColor(dc, RGB(0, 0, 255));


            SendMessageA(pxWindowCurrent->Info.Handle.WindowHandle, TBM_SETTIPSIDE, TBTS_RIGHT, PXNull);


            // SendMessageA(pxWindow->ID, SET_BACKGROUND_COLOR, RGB(30, 30, 30), RGB(30, 30, 30));

            // SET_BACKGROUND_COLOR;

            // TreeView_SetBkColor(pxWindow->ID, RGB(30, 30, 30));

            //   TreeView_SetTextColor(pxWindow->ID, RGB(200, 200, 200));
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

            pxUIElementUpdateInfo[0].WindowCurrent = pxWindowCurrent;
            pxUIElementUpdateInfo[0].WindowParentREF = pxWindowCreateInfo->WindowParent;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].WindowCurrent = pxWindowCurrent;
            pxUIElementUpdateInfo[1].WindowParentREF = pxWindowCreateInfo->WindowParent;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;

            PXNativDrawWindowProperty(&_pxGUIManager.NativDraw, pxUIElementUpdateInfo, 2);

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

                TreeView_SetBkColor(pxWindowCurrent->Info.Handle.WindowHandle, RGB(30, 30, 30));

                TreeView_SetTextColor(pxWindowCurrent->Info.Handle.WindowHandle, RGB(200, 200, 200));


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
                        PXGUIName,
                        "ImageList-Add",
                        "New icon %i",
                        addedID
                    );
                }

                TreeView_SetImageList(pxWindowCurrent->Info.Handle.WindowHandle, imageListHandle, TVSIL_NORMAL);
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
            PXUIElementTabPageInfo* const pxUIElementTabPageInfo = &pxWindowCreateInfo->Data.TabPage;


            // Create space for extended data
            PXWindowExtendedBehaviourTab* pxWindowExtendedBehaviourTab = PXMemoryHeapCallocT(PXWindowExtendedBehaviourTab, 1);
            pxWindowCurrent->ExtendedData = pxWindowExtendedBehaviourTab;

            // Fill extended data
            pxWindowExtendedBehaviourTab->TABPageIndexCurrent = -1;
            pxWindowExtendedBehaviourTab->TABPageAmount = pxWindowCreateInfo->Data.TabPage.TabPageSingleInfoAmount;

            // NO!! NO ALLOC HERE
            //pxWindowExtendedBehaviourTab->TABPageList = PXMemoryHeapCallocT(PXWindow, pxWindowCreateInfo->Data.TabPage.TabPageSingleInfoAmount);


            PXIcon* pxIcon = PXMemoryHeapCallocT(PXIcon, 1);

            PXNativDrawIconLoad(&_pxGUIManager.NativDraw, pxIcon, 0);


            PXResourceCreateInfo pxResourceCreateInfo;
            PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);

            // Create
            {
                // Create a panel for each page, to contain all elements, so that we can hide and show all at once
                pxResourceCreateInfo.Type = PXResourceTypeGUIElement;
                pxResourceCreateInfo.ObjectAmount = pxWindowExtendedBehaviourTab->TABPageAmount;
                pxResourceCreateInfo.ObjectReference = &pxWindowExtendedBehaviourTab->TABPageList;
                //pxResourceCreateInfo.Parent = pxWindowCurrent;
                pxResourceCreateInfo.UIElement.Type = PXUIElementTypePanel;
                pxResourceCreateInfo.UIElement.Name = "TAB-Page";
                pxResourceCreateInfo.UIElement.Invisible = PXFalse;
                pxResourceCreateInfo.UIElement.WindowCurrent = pxWindowCurrent;
                pxResourceCreateInfo.UIElement.WindowParent = pxWindowParent;
                pxResourceCreateInfo.UIElement.Setting = PXWindowBehaviourBorder | PXWindowAllignCenter;// | PXWindowAllignIgnoreParentMargin;
                pxResourceCreateInfo.UIElement.Position.Margin.Left = -0.40;
                pxResourceCreateInfo.UIElement.Position.Margin.Top = 0.025;
                pxResourceCreateInfo.UIElement.Position.Margin.Right = -0.40;
                pxResourceCreateInfo.UIElement.Position.Margin.Bottom = -0.525;

                PXResourceManagerAdd(&pxResourceCreateInfo);
                //PXWindowCreate(pxGUISystem, &pxResourceCreateInfo, 1);
            }



            for(PXSize i = 0; i < pxUIElementTabPageInfo->TabPageSingleInfoAmount; ++i)
            {
                PXWindow* const pxWindowSub = &pxWindowExtendedBehaviourTab->TABPageList[i];

                pxResourceCreateInfo.ObjectAmount = 1;
                pxResourceCreateInfo.ObjectReference = &pxWindowSub;
                pxResourceCreateInfo.Parent = pxWindowCurrent;
                //pxResourceCreateInfo.UIElement.WindowCurrent = pxWindowSub;
                pxResourceCreateInfo.UIElement.WindowParent = pxWindowCurrent;

                PXWindowCreate(pxGUISystem, &pxResourceCreateInfo, 1);

                PXUIElementTabPageSingleInfo* const pxUIElementTabPageSingleInfo = &pxUIElementTabPageInfo->TabPageSingleInfoList[i];
                *pxUIElementTabPageSingleInfo->UIElement = pxWindowSub; // Store reference to have it for the caller

                pxWindowSub->NameContent = pxUIElementTabPageSingleInfo->PageName;
                pxWindowSub->NameContentSize = PXTextLengthA(pxWindowSub->NameContent, PXTextLengthUnkown);

                if(pxUIElementTabPageSingleInfo->TABIcon)
                {
                    pxWindowSub->Icon = pxUIElementTabPageSingleInfo->TABIcon;
                }
                else
                {
                    pxWindowSub->Icon = pxIcon;
                }

                pxWindowSub->Info.Hierarchy.Parrent = pxWindowCurrent;
                pxWindowSub->Type = pxUIElementTabPageSingleInfo->UIElementType;


                switch(pxUIElementTabPageSingleInfo->UIElementType)
                {
                    case PXUIElementTypeCode:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawCode;
                        break;
                    }
                    case PXUIElementTypeHexEditor:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawHexEditor;
                        break;
                    }
                    case PXUIElementTypeGraphBehaviour:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawGraphBehaviour;
                        break;
                    }
                    case PXUIElementTypeGraphTime:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawGraphTime;
                        break;
                    }
                    case PXUIElementTypeSoundPlayerMixer:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawSoundPlayerMixer;
                        break;
                    }
                    case PXUIElementTypeVideoCutter:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawVideoCutter;
                        break;
                    }
                    case PXUIElementTypeDataBaseManager:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawDataBase;
                        break;
                    }
                    case PXUIElementTypeNetworkTester:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawNetwork;
                        break;
                    }
                    case PXUIElementTypeInputView:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawInputView;
                        break;
                    }
                    case PXUIElementTypeHardwareInfo:
                    {
                        pxWindowSub->DrawFunction = PXGUIDrawHardwareInfo;
                        break;
                    }

                    default:
                        break;
                }

                pxWindowSub->DrawFunction = PXGUIDrawFailback;



                switch(PXResourceInfoUseByMask & pxWindowCurrent->Info.Behaviour)
                {
                    case PXResourceInfoUseByOS:
                    {
#if OSUnix
#elif OSWindows && 0
                        TCITEM tie;
                        tie.mask = TCIF_TEXT | TCIF_IMAGE;
                        tie.iImage = pxUIElementTabPageSingleInfo->ImageID;
                        tie.pszText = pxUIElementTabPageSingleInfo->PageName;

                        auto x = TabCtrl_InsertItem(pxWindow->Info.Handle.WindowHandle, i, &tie);
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
                    PXGUIName,
                    "TabControl-Add",
                    "Page added %s",
                    pxUIElementTabPageSingleInfo->PageName
                );
            }

#if OSUnix
#elif OSWindows && 0
            SendMessage(pxWindow->Info.Handle.WindowHandle, TCM_SETCURFOCUS, 0, 0);
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
            PXUIElementSceneRenderInfo* const pxUIElementSceneRenderInfo = &pxWindowCreateInfo->Data.SceneRender;
            PXEngine* const pxEngine = pxUIElementSceneRenderInfo->Engine;
            PXEngineStartInfo* const pxEngineStartInfo = pxUIElementSceneRenderInfo->StartInfo;

            // pxEngine->Window = pxWindowCurrent; // Maybe override?

            pxEngineStartInfo->Mode = PXGraphicInitializeModeOSGUIElement;
            pxEngineStartInfo->Width = pxWindowCurrent->Position.Form.Width;
            pxEngineStartInfo->Height = pxWindowCurrent->Position.Form.Height;
            pxEngineStartInfo->Name = "UIElement-RenderFrame";
            pxEngineStartInfo->UseMouseInput = 1;
            pxEngineStartInfo->WindowRenderTarget = pxWindowCurrent; // Maybe override?
            pxEngineStartInfo->WindowRenderParent = pxWindowCreateInfo->WindowParent;

            PXEngineStart(pxEngine, pxEngineStartInfo);

#endif

            break;
        }

        case PXUIElementTypeWindow:
        {
            PXWindowCreateWindowInfo* const pxWindowCreateWindowInfo = &pxWindowCreateInfo->Data.Window;

            PXGUIPropertyTitleBarColor(pxWindowCurrent);           

#if OSWindows
            BOOL aaxx = EnableWindow(pxWindowCurrent->Info.Handle.WindowHandle, PXTrue);
#endif

            //  HWND qssa = SetCapture(pxWindowCurrent->Info.Handle.WindowHandle);

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

PXResult PXAPI  PXWindowFetch(PXGUISystem* const pxGUISystem, PXWindowPropertyInfo* const pxWindowUpdateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXWindowPropertyInfo* const pxWindowUpdateInfo = &pxWindowUpdateInfoList[i];
        PXWindow* const pxWindow = pxWindowUpdateInfo->WindowCurrent;

        switch(pxWindowUpdateInfo->Property)
        {
            case PXUIElementPropertySizeParent:
            {
                PXRectangleXYWHI32* pxRectangleXYWH = &pxWindowUpdateInfo->Data.Size;

                // const PXBool hasParent = pxWindow ? pxWindow->Parent : PXFalse;

                const PXBool hasParent = PXNull != pxWindowUpdateInfoList->WindowParentREF;

                if(!hasParent) // Special behaviour, if ID is null, get the screensize
                {
                    PXNativDrawScreenSizeGet(pxRectangleXYWH);

                    return PXActionSuccessful;
                }

                PXWindowPropertyInfo pxWindowUpdateInfoSub;
                PXClear(PXWindowPropertyInfo, &pxWindowUpdateInfoSub);
                pxWindowUpdateInfoSub.Property = PXUIElementPropertySize;
                pxWindowUpdateInfoSub.WindowCurrent = pxWindowUpdateInfoList->WindowParentREF;

                PXWindowFetch(pxGUISystem, &pxWindowUpdateInfoSub, 1);

                PXCopy(PXRectangleXYWHI32, &pxWindowUpdateInfoSub.Data.Size, &pxWindowUpdateInfo->Data.Size);

                break;
            }
            case PXUIElementPropertySize:
            {
                PXNativDrawWindowXYWH(&_pxGUIManager.NativDraw, pxWindow, &pxWindowUpdateInfo->Data.Size, PXFalse);
                break;
            }

            default:
                break;
        }
    }

    return PXActionSuccessful;
}



BOOL CALLBACK PXWindowEnumChildProc(HWND hwnd, LPARAM lParam)
{
    int mode = *(int*)lParam;

    // Recursion
    {
        const BOOL success = EnumChildWindows
        (
            hwnd,
            PXWindowEnumChildProc,
            lParam
        );
    }

    ShowWindow(hwnd, mode);
}




void PXAPI PXWindowhSizeRefresAll(PXGUISystem* const pxGUISystem)
{
    if(!(pxGUISystem))
    {
        return;
    }

  //  PXDictionary* const uiElementLookup = &_pxGUIManager.ResourceManager->GUIElementLookup;

    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

    /*
    const BOOL enumResult = EnumChildWindows
    (
        pxWindow->Info.Handle.WindowHandle,
        [in]           WNDENUMPROC lpEnumFunc,
        [in]           LPARAM      lParam
    );
    */



    /*

    for(PXSize i = 0; i < uiElementLookup->EntryAmountCurrent; ++i)
    {
        PXDictionaryEntry pxDictionaryEntry;

        PXDictionaryIndex(uiElementLookup, i, &pxDictionaryEntry);

        PXWindow* uiElement = *(PXWindow**)pxDictionaryEntry.Value;

        if(uiElement->Type == PXUIElementTypeWindow)
        {
            //continue;
        }





        /*
        PXWindowPropertyInfo pxWindowUpdateInfo;
        PXClear(PXWindowPropertyInfo, &pxWindowUpdateInfo);
        pxWindowUpdateInfo.WindowCurrent = uiElement;
        pxWindowUpdateInfo.WindowReference = uiElement->Info.Hierarchy.Parrent;
        pxWindowUpdateInfo.Property = PXUIElementPropertySize;

        PXNativDrawWindowProperty(&_pxGUIManager.NativDraw, &pxWindowUpdateInfo, 1);


    }
       */
}

PXResult PXAPI PXGUIWindowPixelSystemSet(PXWindowPixelSystemInfo* const pxWindowPixelSystemInfo)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Window-PixelSystem",
        "Setting info"
    );
#endif

#if OSUnix
    // TODO: implement?

    return PXActionSuccessful;

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



PXResult PXAPI  PXWindowMouseMovementEnable(const PXWindowHandle pxWindow)
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
            PXGUIName,
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
        PXGUIName,
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
PXResult PXAPI  PXWindowSizeSet(const PXWindowID pxWindow, PXWindowSizeInfo* const pxWindowSizeInfo)
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

void PXAPI PXWindowCursorCaptureMode(const PXWindowHandle pxWindowID, const PXWindowCursorMode cursorMode)
{
#if OSUnix
#elif PXOSWindowsDestop

    switch(cursorMode)
    {
        case PXWindowCursorShow:
        {
            //printf("[Cursor] Show\n");

            while(ShowCursor(1) < 0);

            //const PXBool clipResult = ClipCursor(NULL);
            //const HCURSOR cursorSet = SetCursor(pxWindowID);

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

PXResult PXAPI  PXWindowReDrawEnable(const PXWindowHandle pxWindowID, const PXBool enable)
{
#if OSUnix
#elif OSWindows

    SendMessage(pxWindowID, WM_SETREDRAW, enable, 0);

    if(enable)
    {
        const BOOL result = InvalidateRect(pxWindowID, NULL, TRUE); // forces the window to redraw.
    }

#endif

    return PXActionSuccessful;
}

PXBool PXAPI PXWindowCursorPositionInWindowGet(const PXWindowHandle pxWindowID, PXI32S* const x, PXI32S* const y)
{
    PXI32S xPos = 0;
    PXI32S yPos = 0;
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

PXBool PXAPI PXWindowCursorPositionInDestopGet(const PXWindowHandle pxWindowID, PXI32S* const x, PXI32S* const y)
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

PXBool PXAPI PXWindowIsInFocus(const PXWindowHandle pxWindowID)
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
PXResult PXAPI  PXWindowDrawText(PXGUISystem* const pxGUISystem, PXWindow* const pxWindow, PXText* const pxText)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
            return PXWindowDrawA(pxGUISystem, pxWindow, 0, pxText->A, pxText->SizeUsed);

        case TextFormatUNICODE:
            return PXWindowDrawW(pxGUISystem, pxWindow, pxText->W, pxText->SizeUsed);

        default:
            return TextFormatInvalid;
    }
}*/





PXResult PXAPI  PXGUIFontListFetch(PXGUISystem* const pxGUISystem, PXSize* const amount)
{
#if OSUnix
    int fontNameListSizeMax = 0;
    int fontNameListSizeCurrnet = 0;
    char** fontNameList = XListFonts
    (
        _pxGUIManager.DisplayCurrent.DisplayHandle,
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
PXResult PXAPI  PXWindowErrorFromXSystem(const int xSysstemErrorID)
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

PXResult PXAPI  PXGUISystemContextMenuEntryAdd(PXGUISystemContextMenuEntry* const pxGUISystemContextMenuEntry)
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



PXI32U PXAPI PXWindowCursorIconToID(const PXCursorIcon cursorIcon)
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
