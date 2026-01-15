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
#include <PX/Media/PXText.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXAwait.h>
#include <PX/OS/Graphic/PXGraphic.h>
#include <PX/OS/File/PXDirectory.h>
#include <PX/OS/Graphic/NativDraw/PXNativDraw.h>
#include <PX/OS/Hardware/PXProcessor.h>
#include "ECS/PXECS.h"



PXGUIManager _pxGUIManager;

const char PXGUIName[] = "GUI";
const char PXPixelSystemName[] = "PixelSystem";

const char PXWindowName[] = "PXWindowMain";
const PXI8U PXWindowNameLength = sizeof(PXWindowName);

PXF32 colorTemp = 20;


// // "Eras Medium ITC"; // Bradley Hand ITC, UniSpace,OCR A, Cascadia Mono,         "UniSpace";
const char PXFontNamePri[] =
#if OSUnix
"fixed"
#elif OSWindows
"UniSpace"
#endif
;

// fixed, Samanata(does not work on X11)
const char PXFontNameSec[] =
#if OSUnix
"fixed"
#elif OSWindows
"Eras Medium ITC" // Eras Medium ITC
#endif
;






PXGUIManager* PXAPI PXGUIInitialize(void)
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


    //  PXGUIDisplayScreenListRefresh(pxGUIManager);



    // Create brushes
    {
        PXBrushCreateInfo pxBrushCreateInfo;
        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);

        pxBrushCreateInfo.Info.Name.A = "MainPrimary";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 100 * 0.40f; // 30-30-30, 160, 40, 40
        pxBrushCreateInfo.Color.Green = 40 * 0.40f;
        pxBrushCreateInfo.Color.Blue = 40 * 0.40f;
        PXBrushCreate(&_pxGUIManager.ThemeDefault.BrushMainPrimary, &pxBrushCreateInfo);

        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);
        pxBrushCreateInfo.Info.Name.A = "MainSecoundary";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 35 * 0.40f;
        pxBrushCreateInfo.Color.Green = 69 * 0.40f;
        pxBrushCreateInfo.Color.Blue = 100 * 0.40f;
        PXBrushCreate(&_pxGUIManager.ThemeDefault.BrushMainSecoundary, &pxBrushCreateInfo);

        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);
        pxBrushCreateInfo.Info.Name.A = "TextWhite";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 0xff; // 200-200-200
        pxBrushCreateInfo.Color.Green = 200;
        pxBrushCreateInfo.Color.Blue = 200;
        PXBrushCreate(&_pxGUIManager.ThemeDefault.BrushTextWhite, &pxBrushCreateInfo);

        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);
        pxBrushCreateInfo.Info.Name.A = "DarkBackground";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 40;
        pxBrushCreateInfo.Color.Green = 40;
        pxBrushCreateInfo.Color.Blue = 40;
        PXBrushCreate(&_pxGUIManager.ThemeDefault.BrushDarkBackground, &pxBrushCreateInfo);
    }


    // Font
    {
        PXFontCreateInfo pxFontCreateInfo;
        PXClear(PXFontCreateInfo, &pxFontCreateInfo); 
        pxFontCreateInfo.Info.Name.A = "FontTitle";
        pxFontCreateInfo.Info.Type = PXResourceTypeFont;
        PXTextFromAdressA(&pxFontCreateInfo.RegisteredName, PXFontNamePri, sizeof(PXFontNamePri), sizeof(PXFontNamePri));
        PXFontCreate(&_pxGUIManager.ThemeDefault.FontTitle, &pxFontCreateInfo);

        PXClear(PXFontCreateInfo, &pxFontCreateInfo);
        pxFontCreateInfo.Info.Name.A = "FontContent";
        pxFontCreateInfo.Info.Type = PXResourceTypeFont;
        PXTextFromAdressA(&pxFontCreateInfo.RegisteredName, PXFontNameSec, sizeof(PXFontNameSec), sizeof(PXFontNameSec));
        PXFontCreate(&_pxGUIManager.ThemeDefault.FontContent, &pxFontCreateInfo);
    }

    return &_pxGUIManager;
}

PXResult PXAPI PXGUIShutdown(void)
{
    PXResult result = PXResultInvalid;

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

PXGUITheme* PXAPI PXGUIThemeGet(void)
{
    return _pxGUIManager.ThemeSelected;
}


PXResult PXAPI PXGUIPropertyFetch(PXWindow PXREF pxWindow, PXGUIProperty PXREF pxGUIPropertyList, const PXSize amount, const PXBool doWrite)
{
    /*
    PXWindow* pxWindowParrent = (PXWindow*)pxWindow->Info.Hierarchy.Parrent;

    for(PXSize i = 0; i < amount; ++i)
    {
        PXGUIProperty PXREF pxGUIProperty = &pxGUIPropertyList[i];

        switch(pxGUIProperty->Property)
        {
            case PXUIElementPropertySizeParent:
            {         
                PXRectangleXYWHI32* pxRectangleXYWH = &pxGUIPropertyList->Size;

                if(!pxWindowParrent) // Special behaviour, if ID is null, get the screensize
                {
                    PXNativDrawScreenSizeGet(pxRectangleXYWH);

                    return PXResultOK;
                }

                // We want to get the parrents size here, so we dont get the screen size

                PXGUIProperty pxGUIPropertySub;
                PXClear(PXGUIProperty, &pxGUIPropertySub);
                pxGUIPropertySub.Property = PXUIElementPropertySize;
                //pxGUIPropertySub.WindowCurrent = pxWindow->Info.Hierarchy.Parrent;
                            
                PXGUIPropertyFetch(pxWindowParrent, &pxGUIPropertySub, 1, doWrite);

                PXCopy(PXRectangleXYWHI32, &pxGUIPropertySub.Size, &pxGUIProperty->Size);

                break;
            }
            case PXUIElementPropertySize:
            {
                PXNativDrawWindowXYWH(&_pxGUIManager.NativDraw, pxWindow, &pxGUIProperty->Size, PXFalse);
                break;
            }

            default:
                break;
        }
    }
    */
    return PXResultOK;
}

void PXMathCircle(PXColorCircle PXREF pxColorCircle)
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

void PXWindowsChildShow(PXWindow PXREF pxWindow)
{
#if 0
    for(PXWindow* child = pxWindow->Info.Hierarchy.ChildFirstborn; child; child = child->Info.Hierarchy.ChildFirstborn)
    {
        PXWindowsChildShow(child);

        child->Info.Behaviour |= PXECSInfoSelected | PXECSInfoRender;

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


    pxWindow->Info.Behaviour |= PXECSInfoSelected | PXECSInfoRender;

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
#endif
}

void PXWindowsChildHide(PXWindow PXREF pxWindow)
{
#if 0
    for(PXWindow* child = pxWindow->Info.Hierarchy.ChildFirstborn; child; child = child->Info.Hierarchy.ChildFirstborn)
    {
        PXWindowsChildHide(child);

        child->Info.Behaviour &= ~PXECSInfoSelected;
        child->Info.Behaviour &= ~PXECSInfoRender;

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

    pxWindow->Info.Behaviour &= ~PXECSInfoSelected;
    pxWindow->Info.Behaviour &= ~PXECSInfoRender;

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
#endif
}

PXResult PXAPI PXWindowTabListSwapPage(PXWindow PXREF pxWindow)
{
#if 0
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
        PXWindow PXREF pxWindowTABPage = &pxWindowExtendedBehaviourTab->TABPageList[i];

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
#endif
    return PXResultOK;
}


PXBool PXAPI PXWindowValueFetch(PXWindow PXREF pxUIElementList, const PXSize dataListAmount, const PXUIElementProperty pxUIElementProperty, void PXREF dataList)
{
#if 0
    for(size_t i = 0; i < dataListAmount; ++i)
    {
        PXWindow PXREF pxWindow = &pxUIElementList[i];

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
#endif
    return PXTrue;
}

/*
BOOL CALLBACK PXWindowEnumChildProc(const HWND windowHandle, LPARAM lParam)
{
    int mode = *(int*)lParam;
    BOOL success = FALSE;

    // Recursion
    {
        success = EnumChildWindows
        (
            windowHandle,
            PXWindowEnumChildProc,
            lParam
        );
    }

    success = ShowWindow(windowHandle, mode);
}*/

/*
PXResult PXAPI PXWindowSizeSet(const PXWindowID pxWindow, PXWindowSizeInfo PXREF pxWindowSizeInfo)
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

    return PXResultOK;
#endif
}
*/


/*
PXResult PXAPI PXWindowDrawText(PXGUIManager PXREF pxGUIManager, PXWindow PXREF pxWindow, PXText PXREF pxText)
{
    switch(pxText->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
            return PXWindowDrawA(pxGUIManager, pxWindow, 0, pxText->A, pxText->SizeUsed);

        case TextFormatUNICODE:
            return PXWindowDrawW(pxGUIManager, pxWindow, pxText->W, pxText->SizeUsed);

        default:
            return TextFormatInvalid;
    }
}*/





PXResult PXAPI PXGUIFontListFetch(PXGUIManager PXREF pxGUIManager, PXSize PXREF amount)
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
PXResult PXAPI PXWindowErrorFromXSystem(const int xSysstemErrorID)
{
    switch(xSysstemErrorID)
    {
        case BadValue:
            return PXResultRefusedParameterInvalid; // input is not valid
        case BadWindow:
            return PXActionRefusedNotFound; // object id invalid
        case BadAlloc:
            return PXActionFailedMemoryAllocation;
        case BadColor:
            return PXResultRefusedParameterInvalid;
        case BadCursor:
            return PXResultRefusedParameterInvalid;
        case BadMatch:
            return PXResultRefusedParameterInvalid;
        case BadPixmap:
            return PXResultRefusedParameterInvalid;

        default:
            return PXResultInvalid;
    }
}
#endif


typedef struct PXGUIManagerContextMenuEntry_
{
    char* FileTypeExtension;
    char* FileTypeName;
    char* FileTypeDefaultIconPath;
}
PXGUIManagerContextMenuEntry;

PXResult PXAPI PXGUIManagerContextMenuEntryAdd(PXGUIManagerContextMenuEntry PXREF pxGUIManagerContextMenuEntry)
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
