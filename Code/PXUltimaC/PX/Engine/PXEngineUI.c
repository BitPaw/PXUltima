#include "PXEngineUI.h"

//---------------------------
#include <PX/OS/Console/PXConsole.h>
#include <PX/Compiler/PXCompiler.h>
#include <PX/Media/YAML/PXYAML.h>
#include <PX/OS/PXOS.h>
#include <PX/Type/PXWindow.h>
#include <PX/Container/List/PXList.h>
#include <PX/Type/PXText.h>
#include <PX/Math/PXMath.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Async/PXAwait.h>
#include <PX/OS/File/PXDirectory.h>
#include <PX/Type/PXCPU.h>
#include <PX/Engine/PXEngineModule.h>
#include <PX/Type/PXWindow.h>
//---------------------------


//---------------------------
// OS
//---------------------------
#if OSUnix

//---------------------------
// Xlib / X-System
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/Xrandr.h> // NEED: sudo apt install libxrandr-dev
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
#include <Richedit.h>   // ???
#include <windowsx.h>   // ???
#include <PX/Type/PXBrush.h>
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
//---------------------------




//---------------------------------------------------------
// Copy&Paste things needed to make some UIs and buttons modern
//---------------------------------------------------------
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
//---------------------------------------------------------

typedef struct PXEngineUI_
{
    PXEngineModule ModuleInfo;

    PXMonitor* MonitorList;
    PXSize MonitorCount;

    PXEngineUITheme ThemeDefault;
    PXEngineUITheme* ThemeSelected;

    PXWindow* WindowRoot;

#if OSUnix
    Display* DisplayHandle;
    Window WindowRootHandle;
    GC GraphicContextHandle;

    int ScreenCount;

    char* Data;
    char* Name;
    char* ServerVendor;
    int ProtocolVersion;
    int ProtocolRevision;
    int VendorRelease;

#elif OSWindows
    HINSTANCE hInstance;
    // no window needed
#endif


    UIWindow* g_floating_windows[32];
    int g_floating_count;
}
PXEngineUI;

const char PXEngineUIEventText[] = "Event";

const char PXEngineUIText[] = "UI-PX";
const PXI8U PXEngineUITextLength = sizeof(PXEngineUIText);
const PXECSRegisterInfoStatic PXEngineUIInfoStatic =
{
    {sizeof(PXEngineUIText), sizeof(PXEngineUIText), PXEngineUIText, TextFormatASCII},
    sizeof(PXEngineUI),
    __alignof(PXEngineUI),
    PXECSTypeSystem,
    PXEngineUICreate,
    PXEngineUIRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXEngineUIInfoDynamic;


PXEngineUI* _GlobalUIREF = 0; // Bad solution to fix X11 garbage.




#define MIN_MARGIN 16   // Ensure margin never drops below 16 virtual pixels
#define MAX_MARGIN 120  // Ensure margin never grows past 120 virtual pixels


// 128 bytes forming a repeating 32x32 diamond crosshatch pattern
static const GLubyte DiamondHatchPattern[128] =
{
    0x81, 0x81, 0x81, 0x81, // Row 0
    0x42, 0x42, 0x42, 0x42, // Row 1
    0x24, 0x24, 0x24, 0x24, // Row 2
    0x18, 0x18, 0x18, 0x18, // Row 3
    0x18, 0x18, 0x18, 0x18, // Row 4
    0x24, 0x24, 0x24, 0x24, // Row 5
    0x42, 0x42, 0x42, 0x42, // Row 6
    0x81, 0x81, 0x81, 0x81, // Row 7
    // Repeating the 8x8 pattern module down through all 32 rows:
    0x81, 0x81, 0x81, 0x81, 0x42, 0x42, 0x42, 0x42,
    0x24, 0x24, 0x24, 0x24, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24, 0x24,
    0x42, 0x42, 0x42, 0x42, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x81, 0x81, 0x42, 0x42, 0x42, 0x42,
    0x24, 0x24, 0x24, 0x24, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24, 0x24,
    0x42, 0x42, 0x42, 0x42, 0x81, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x81, 0x81, 0x42, 0x42, 0x42, 0x42,
    0x24, 0x24, 0x24, 0x24, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18, 0x18, 0x24, 0x24, 0x24, 0x24,
    0x42, 0x42, 0x42, 0x42, 0x81, 0x81, 0x81, 0x81
};






const char PXGUIName[] = "GUI";
const char PXPixelSystemName[] = "PixelSystem";

const char PXWindowName[] = "PXWindowMain";
const PXI8U PXWindowNameLength = sizeof(PXWindowName);


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


const char* PXBackGroundMatches[] =
{
    "Red",
    "Green",
    "Blue",
};
const PXI8U PXBackGroundMatchesAmount = sizeof(PXBackGroundMatches) / sizeof(char*);

#define PXBackGround_Red   0
#define PXBackGround_Green 1
#define PXBackGround_Blue  2


const char* PXPaddingMatches[] =
{
    "Left",
    "Top",
    "Right",
    "Bottom",
};
const PXI8U PXPaddingMatchesAmount = sizeof(PXPaddingMatches) / sizeof(char*);

#define PXPadding_Left      0
#define PXPadding_Top       1
#define PXPadding_Right     2
#define PXPadding_Bottom    3



const char* PXKeyMatchesLV1[] =
{
    "Window"
};
const PXI8U PXKeyMatchesLV1Amount = sizeof(PXKeyMatchesLV1) / sizeof(char*);

#define UI_Window          0

const char* PXWindowPropertyMatchesLV2[] =
{
    "Name",
    "Parent",
    "DockSide",
};
const PXI8U PXWindowPropertyLV2Amount = sizeof(PXWindowPropertyMatchesLV2) / sizeof(char*);

#define UI_Window_Name          0
#define UI_Window_Parent        1
#define UI_Window_DockSide      2


const char* PXWindowClassesMatchesLV2[] =
{
    "BackGround",
    "Padding"
};
const PXI8U PXWindowClassesMatchesLV2Amount = sizeof(PXWindowClassesMatchesLV2) / sizeof(char*);

#define UI_Window_BackGround    0
#define UI_Window_Padding       1


PXResult PXAPI PXEngineUILoadFromFile(PXEngineUI PXREF pxUI, PXText PXREF pxFilePath)
{
    PXResult pxResult = PXResultInvalid;

    if(!(pxUI && pxFilePath))
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxFilePath->Data)
    {
        return PXResultOK;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Load",
        "Path:%s",
        pxFilePath->A
    );
#endif

    PXDOM* pxDOM = 0;
    pxResult = PXDOMCreate(&pxDOM);

    // Load expected UI file
    {
        PXECSCreateInfo pxECSCreateInfo;
        PXClear(PXECSCreateInfo, &pxECSCreateInfo);
        pxECSCreateInfo.FilePath = *pxFilePath;

        pxResult = PXECSLoad(pxDOM, &pxECSCreateInfo);
    }



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Load",
        "DONE"
    );
#endif


    char buffer[128];

    PXText pxText;
    pxResult = PXTextFromAddressA(&pxText, buffer, 0, sizeof(buffer));



    PXSize amount = PXDOMKeyAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Keys]", amount
    );
#endif

#if 0
    for(size_t i = 0; i < amount; i++)
    {
        PXDOMKey* pxDOMKey = PXDOMKeyGet(pxDOM, i);

        PXTextClear(&pxText);
        PXTextAppendA(&pxText, pxDOMKey->Symbol.Source, pxDOMKey->Symbol.Size);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineUIText,
            "Load",
            "%-18s : %s",
            "DOM-Key", pxText.A
        );
#endif
    }
#endif


    amount = PXDOMPropertyAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Propertys]",amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMProperty PXREF pxDOMClassValue = PXDOMPropertyGet(pxDOM, i);

        char bufferValue[256];
        PXText pxTextValue;
        pxResult = PXTextFromAddressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

        PXTextClear(&pxText);
        pxResult = PXDOMPropertyToString(pxDOM, pxDOMClassValue, &pxText, &pxTextValue, PXTrue);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineUIText,
            "Load",
            "%-18s : [%2.2i] (P:%i) %-30s ~> <%s>",
            "DOM-Property", i, pxDOMClassValue->ParentObjectIndex, pxText.A,
            pxTextValue.A
        );
#endif
    }


    amount = PXDOMClassAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Classes]", amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMClass PXREF pxDOMClass = PXDOMClassGet(pxDOM, i);

        PXTextClear(&pxText);
        pxResult = PXDOMClassToString(pxDOM, pxDOMClass, &pxText, PXTrue);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineUIText,
            "Load",
            "%-18s : %-30s",
            "DOM-Class", pxText.A,
            i
        );
#endif
    }




    amount = PXDOMObjectAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Object]",amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMObject PXREF pxDOMObject = PXDOMObjectGet(pxDOM, i);

        PXTextClear(&pxText);
        pxResult = PXDOMObjectToString(pxDOM, pxDOMObject, &pxText, PXTrue);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXEngineUIText,
            "Load",
            "%-18s : [%2i][P:%5i][S:%5i][C:%5i] C:[%i] P:[%2.2i]_%-30s",
            "DOM-Object",
            i,
            pxDOMObject->ObjectIndexParent,
            pxDOMObject->ObjectIndexSibling,
            pxDOMObject->ObjectIndexChild,
            pxDOMObject->ClassIndex,
            pxDOMObject->PropertyIndexCount,
            pxText.A
        );
#endif

        for(PXSize i = 0; i < pxDOMObject->PropertyIndexCount; i++)
        {
            PXDOMIndex propertyStart = pxDOMObject->PropertyIndexStart + i;
            PXDOMProperty PXREF pxDOMClassValue = PXDOMPropertyGet(pxDOM, propertyStart);

            char bufferValue[256];
            PXText pxTextValue;
            PXTextFromAddressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

            PXTextClear(&pxText);
            pxResult = PXDOMPropertyToString(pxDOM, pxDOMClassValue, &pxText, &pxTextValue, PXTrue);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXEngineUIText,
                "Load",
                "%-18s : [%2.2i] (P:%i) %-30s ~> <%s>",
                "DOM-Property", i, pxDOMClassValue->ParentObjectIndex, pxText.A,
                pxTextValue.A
            );
#endif
        }
    }



    //PXDOMPrintTreeIterative(pxDOM);



    PXDOMCursor pxDOMCursor;
    PXDOMCursorStart(pxDOM, &pxDOMCursor);


    PXDOMCursorUpdate(pxDOM, &pxDOMCursor);

    PXDOMCursorNext(pxDOM, &pxDOMCursor);

    PXI8U match = 0;
    PXBool yeet = 0;

    while(PXDOMCursorUpdate(pxDOM, &pxDOMCursor))
    {
        match = PXDOMCursorKeyCompare(pxDOM, &pxDOMCursor, PXKeyMatchesLV1, PXKeyMatchesLV1Amount);

        switch(match)
        {
            default:
            {
                yeet = 1;
                break;
            }
            case UI_Window:
            {
                PXWindowCreateInfo pxWindowCreateInfo;
                PXClear(PXWindowCreateInfo, &pxWindowCreateInfo);
                pxWindowCreateInfo.EngineUI = pxUI;

#if 1 // Spawn all the windows as physical ones
                pxWindowCreateInfo.SkipHandleCreation = PXTrue;
#endif

                while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
                {
                    match = PXDOMCursorPropertyCompare(pxDOM, &pxDOMCursor, PXWindowPropertyMatchesLV2, PXWindowPropertyLV2Amount);

                    PXCompilerSymbolEntry* symbol = &pxDOMCursor.prop->Symbol;

                    switch(match)
                    {
                        case UI_Window_Name:
                        {
                            PXTextFromAddressA
                            (
                                &pxWindowCreateInfo.Info.Name,
                                symbol->Source,
                                symbol->Size,
                                symbol->Size
                            );
                            break;
                        }
                        case UI_Window_Parent:
                        {
                            PXTextFromAddressA
                            (
                                &pxWindowCreateInfo.WindowParentName,
                                symbol->Source,
                                symbol->Size,
                                symbol->Size
                            );
                            break;
                        }
                        case UI_Window_DockSide:
                        {
                            break;
                        }
                        default:
                        {
#if PXLogEnable
                            PXTextClear(&pxText);
                            // PXTextAppendA(&pxText, symbol->Source, symbol->Size);

                            PXLogPrint
                            (
                                PXLoggingWarning,
                                PXEngineUIText,
                                "TR",
                                "unexpected propertydetected : <%s>",
                                "---"
                            );
#endif

                            break;
                        }
                    }

                    PXDOMCursorPropertyNext(pxDOM, &pxDOMCursor);
                }

                PXDOMCursorNext(pxDOM, &pxDOMCursor);

                // Sub classes
                while(PXDOMCursorUpdate(pxDOM, &pxDOMCursor))
                {
                    match = PXDOMCursorKeyCompare(pxDOM, &pxDOMCursor, PXWindowClassesMatchesLV2, PXWindowClassesMatchesLV2Amount);

                    int height = pxDOMCursor.indent;

                    switch(match)
                    {
                        default:
                        {
                            yeet = 1;
                            break;
                        }
                        case UI_Window_BackGround:
                        {
                            PXColorRGB_U8 PXREF pxXColorRGBI8 = &pxWindowCreateInfo.BackGroundColor;

                            while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
                            {
                                match = PXDOMCursorPropertyCompare(pxDOM, &pxDOMCursor, PXBackGroundMatches, PXBackGroundMatchesAmount);

                                PXCompilerSymbolEntry* symbol = &pxDOMCursor.prop->Symbol;

                                switch(match)
                                {
                                    case PXBackGround_Red:
                                    {
                                        pxXColorRGBI8->Red = symbol->I32U;
                                        break;
                                    }
                                    case PXBackGround_Green:
                                    {
                                        pxXColorRGBI8->Green = symbol->I32U; // 255.0f;
                                        break;
                                    }
                                    case PXBackGround_Blue:
                                    {
                                        pxXColorRGBI8->Blue = symbol->I32U;
                                        break;
                                    }
                                }

                                PXDOMCursorPropertyNext(pxDOM, &pxDOMCursor);
                            }

                            break;
                        }
                        case UI_Window_Padding:
                        {
                            PXRectangleLTRBI32 PXREF padding = &pxWindowCreateInfo.Layout.Padding;

                            while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
                            {
                                match = PXDOMCursorPropertyCompare(pxDOM, &pxDOMCursor, PXPaddingMatches, PXPaddingMatchesAmount);

                                PXCompilerSymbolEntry* symbol = &pxDOMCursor.prop->Symbol;

                                switch(match)
                                {
                                    case PXPadding_Left:
                                    {
                                        padding->Left = symbol->I32U;
                                        break;
                                    }
                                    case PXPadding_Top:
                                    {
                                        padding->Top = symbol->I32U;
                                        break;
                                    }
                                    case PXPadding_Right:
                                    {
                                        padding->Right = symbol->I32U;
                                        break;
                                    }
                                    case PXPadding_Bottom:
                                    {
                                        padding->Bottom = symbol->I32U;
                                        break;
                                    }
                                }

                                PXDOMCursorPropertyNext(pxDOM, &pxDOMCursor);
                            }

                            break;
                        }
                    }

                    PXDOMCursorNext(pxDOM, &pxDOMCursor);

                    if(height > pxDOMCursor.indent)
                    {
                        break;
                    }

                    if(yeet)
                    {
                        break;
                    }
                }

                // ToDo: We need to store this, object is still valid but cant be deleted!
                PXWindow* pxWindow = PXNull;

                PXWindowCreate(&pxWindow, &pxWindowCreateInfo);

                break;
            }
        }

        if(yeet)
        {
            break;
        }

        //PXDOMCursorNext(pxDOM, &pxDOMCursor);
    }

#if 0
    for(;;)
    {
        // print
        pxDOMObjectCurrent = PXDOMObjectGet(pxDOM, pxDOMObjectCurrent->FirstChild);



        pxDOMObjectChild = PXDOMObjectGet(pxDOM, pxDOMObjectCurrent->FirstChild);
        pxDOMObjectSibling = PXDOMObjectGet(pxDOM, pxDOMObjectCurrent->NextSibling);

        // Go nested
        for(PXSize i = 0; i < pxDOMObject->; i++)
        {

        }

        // Go properties
        for(PXSize i = 0; i < length; i++)
        {

        }


    }
#endif




#if 0
    PXSlice pxSlice;
    pxSlice.Address = "UI.Window";
    pxSlice.Length = 9;

    PXDOMKey PXREF pxDOMClass = PXDOMKeyFindFirst(pxDOM, &pxSlice);

    if(!pxDOMClass)
    {
        // No UI windows to load?
        return PXResultOK;
    }


    for(PXSize i = 0; i < pxDOMClass->objectCount; i++)
    {
        ObjectRange r = windows->objects[i];
        for(int k = r.start; k <= r.end; k++) {
            // process KV entries for this object
        }
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingWarning,
        PXEngineUIText,
        "Load",
        "DONE!"
    );
#endif

    return PXResultOK;
}

#if OSWindows

typedef struct PXWindowsMonitorEnumInfo_
{
    PXEngineUI* UI;
    PXSize CountCurrent;
    PXBool DoStore;
}
PXWindowsMonitorEnumInfo;

BOOL CALLBACK PXEngineUIMonitorEnumCallBack(HMONITOR monitorHandle, HDC hdc, LPRECT rect, LPARAM data)
{
    PXWindowsMonitorEnumInfo* pxEnumInfo = (PXWindowsMonitorEnumInfo*)data;
    PXEngineUI PXREF pxUI = pxEnumInfo->UI;

    if(!pxEnumInfo->DoStore)
    {
        ++pxUI->MonitorCount;
        return PXTrue;
    }

    PXMonitor PXREF pxMonitor = &pxUI->MonitorList[pxUI->MonitorCount];
    pxMonitor->Handle.Data = monitorHandle;
    pxMonitor->DeviceContext = hdc;

    PXRectangleLTRBI32ToXYWHI32(rect, &pxMonitor->Form);

    MONITORINFOEX  monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFOEX);

    const BOOL result = GetMonitorInfoA(monitorHandle, (LPMONITORINFO)&monitorInfo);

    PXTextCopyA(monitorInfo.szDevice, CCHDEVICENAME, pxMonitor->Name, 32);

    ++pxUI->MonitorCount;

    return PXTrue;
}
#endif

PXResult PXAPI PXEngineUICreate(PXEngineUI** pxUIREF, PXEngineUIInfo PXREF pxUIInfo)
{
    PXEngineUI* pxEngineUI = PXNull;

    if(!(pxUIREF && pxUIInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxUIInfo->Info.Static = &PXEngineUIInfoStatic;
    pxUIInfo->Info.Dynamic = &PXEngineUIInfoDynamic;
    PXResult pxResult = PXECSCreate(pxUIREF, pxUIInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxEngineUI = *pxUIREF;

    // Window event functions HAVE to be in the main thread
    PXEngineModuleCallbackUpdateSerialSet(&pxEngineUI->ModuleInfo, PXEngineUIUpdate);


    _GlobalUIREF = pxEngineUI;

    // Init
    PXEngineUICreateContext(pxEngineUI);

    pxEngineUI->ThemeSelected = &pxEngineUI->ThemeDefault;
      

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


#if OSWindows

    // Ensures that the common control DLL (Comctl32.dll) is loaded
    INITCOMMONCONTROLSEX initCommonControls;
    initCommonControls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    initCommonControls.dwICC = ICC_TAB_CLASSES;
    BOOL initResult = InitCommonControlsEx(&initCommonControls);

#endif



    PXEngineUITheme PXREF pxTheme = &pxEngineUI->ThemeDefault;

    // Create brushes
    {
        PXBrushCreateInfo pxBrushCreateInfo;
        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);

        pxBrushCreateInfo.Info.Name.A = "MainPrimary";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 100 * 0.40f; // 30-30-30, 160, 40, 40
        pxBrushCreateInfo.Color.Green = 40 * 0.40f;
        pxBrushCreateInfo.Color.Blue = 40 * 0.40f;
        PXBrushCreate(&pxTheme->BrushMainPrimary, &pxBrushCreateInfo);

        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);
        pxBrushCreateInfo.Info.Name.A = "MainSecoundary";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 35 * 0.40f;
        pxBrushCreateInfo.Color.Green = 69 * 0.40f;
        pxBrushCreateInfo.Color.Blue = 100 * 0.40f;
        PXBrushCreate(&pxTheme->BrushMainSecoundary, &pxBrushCreateInfo);

        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);
        pxBrushCreateInfo.Info.Name.A = "TextWhite";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 0xff; // 200-200-200
        pxBrushCreateInfo.Color.Green = 200;
        pxBrushCreateInfo.Color.Blue = 200;
        PXBrushCreate(&pxTheme->BrushTextWhite, &pxBrushCreateInfo);

        PXClear(PXBrushCreateInfo, &pxBrushCreateInfo);
        pxBrushCreateInfo.Info.Name.A = "DarkBackground";
        pxBrushCreateInfo.Info.Type = PXResourceTypeBrush;
        pxBrushCreateInfo.Color.Red = 40;
        pxBrushCreateInfo.Color.Green = 40;
        pxBrushCreateInfo.Color.Blue = 40;
        PXBrushCreate(&pxTheme->BrushDarkBackground, &pxBrushCreateInfo);
    }


    // Font
    {
        PXFontCreateInfo pxFontCreateInfo;
        PXClear(PXFontCreateInfo, &pxFontCreateInfo);
        pxFontCreateInfo.Info.Name.A = "FontTitle";
        pxFontCreateInfo.Info.Type = PXResourceTypeFont;
        PXTextFromAddressA(&pxFontCreateInfo.RegisteredName, PXFontNamePri, sizeof(PXFontNamePri), sizeof(PXFontNamePri));
        PXFontCreate(&pxTheme->FontTitle, &pxFontCreateInfo);

        PXClear(PXFontCreateInfo, &pxFontCreateInfo);
        pxFontCreateInfo.Info.Name.A = "FontContent";
        pxFontCreateInfo.Info.Type = PXResourceTypeFont;
        PXTextFromAddressA(&pxFontCreateInfo.RegisteredName, PXFontNameSec, sizeof(PXFontNameSec), sizeof(PXFontNameSec));
        PXFontCreate(&pxTheme->FontContent, &pxFontCreateInfo);
    }

    // Load UI from path
    PXEngineUILoadFromFile(pxEngineUI, &pxUIInfo->Path);



    return PXResultOK;
}

PXResult PXAPI PXEngineUIRelease(PXEngineUI PXREF pxUI)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIUpdate(PXEngineUI PXREF pxEngineUI, PXEngineUpdateInfo PXREF pxEngineUpdateInfo)
{
    PXResult pxResult = PXResultInvalid;

    pxResult = PXEngineUIEventPoll();

    return pxResult;
}

PXWindow* PXAPI PXEngineUIWindowRootGet(const PXEngineUI PXREF pxEngineUI)
{
    if(!pxEngineUI)
    {
        return PXNull;
    }

    return pxEngineUI->WindowRoot;
}

PXResult PXAPI PXEngineUIWindowRootSet(PXEngineUI PXREF pxEngineUI, PXWindow PXREF pxWindow)
{
    pxEngineUI->WindowRoot = pxWindow;

    return PXResultOK;
}

void PXAPI PXEngineUIWindowAdd(PXEngineUI PXREF pxEngineUI, UIWindow PXREF pxWindow)
{
    pxEngineUI->g_floating_windows[pxEngineUI->g_floating_count] = pxWindow;
    pxEngineUI->g_floating_count++;

    return PXResultOK;
}

PXResult PXAPI PXEngineUIWindowResolve(PXEngineUI PXREF pxUI, PXECSInfo** pxECSInfo, PXText PXREF pxTextName)
{
    *pxECSInfo = PXECSEntryGetViaName(pxTextName);

    return PXResultOK;
}

PXResult PXAPI PXEngineUICreateContext(PXEngineUI PXREF pxUI)
{
    PXResult pxResult = PXResultInvalid;

#if OSUnix

    // open a connection to the x-server. NULL here uses the default display.
    pxUI->DisplayHandle = XOpenDisplay(PXNull); // X11/Xlib.h, Open connection to the X server

    if(!pxUI->DisplayHandle)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXEngineUIText,
            "Display-Open",
            "<%s> X11 failed to open display server!"
        );
#endif

        return PXResultFailedInitialization;
    }

    // Attach error callback
    XSetErrorHandler(PXEngineUICallBackHandler);

    // Fetch propertys
    pxUI->Data = XDisplayString(pxUI->DisplayHandle);
    pxUI->Name = XDisplayName(pxUI->DisplayHandle); // if NULL, this is the attempted name what XOpen would use

    pxUI->ProtocolVersion = XProtocolVersion(pxUI->DisplayHandle); // for X11, it is 11
    pxUI->ProtocolRevision = XProtocolRevision(pxUI->DisplayHandle);
    pxUI->ServerVendor = XServerVendor(pxUI->DisplayHandle);
    pxUI->VendorRelease = XVendorRelease(pxUI->DisplayHandle);

    pxUI->WindowRootHandle = DefaultRootWindow(pxUI->DisplayHandle);


    pxUI->GraphicContextHandle = XCreateGC(pxUI->DisplayHandle, pxUI->WindowRootHandle, 0, 0);


    if(pxUI->WindowRootHandle)
    {
        XRRScreenResources* res = XRRGetScreenResources(pxUI->DisplayHandle, pxUI->WindowRootHandle);
        pxUI->MonitorCount = res->ncrtc;
        pxUI->MonitorList = PXMemoryHeapCallocT(PXMonitor, pxUI->MonitorCount);

        for(PXSize i = 0; i < res->ncrtc; ++i)
        {
            PXMonitor PXREF pxMonitor = &pxUI->MonitorList[i];
            XRRCrtcInfo* info = XRRGetCrtcInfo(pxUI->DisplayHandle, res, res->crtcs[i]);
            pxMonitor->Form.X = info->x;
            pxMonitor->Form.Y = info->y;
            pxMonitor->Form.Width = info->width;
            pxMonitor->Form.Height = info->height;
            XRRFreeCrtcInfo(info);
        }

        XRRFreeScreenResources(res);
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Display-Open",
        "OK (0x%p)\n"
        "%10s: %i\n"
        "%10s: %s, Data: %s\n"
        "%10s: v.%i.%i\n"
        "%10s: %s (Relase %i)",
        pxUI->DisplayHandle,
        "Root", pxUI->WindowRootHandle,
        "Name", pxUI->Name, pxUI->Data,
        "Protocol", pxUI->ProtocolVersion, pxUI->ProtocolRevision,
        "Server", pxUI->ServerVendor, pxUI->VendorRelease
    );
#endif

    // Get amount of screens

   // Fetch additional data of your display

   /*


           // Default values
           Colormap XDefaultColormap(Display *display, int screen_number);
           int XDefaultDepth(Display *display, int screen_number);
           int *XListDepths(Display *display, int screen_number, int *count_return);
           GC XDefaultGC(Display *display, int screen_number);
           Visual *XDefaultVisual(Display *display, int screen_number);





           // UI Element needs function to override drawing by OS
           // Linux does not even have drawing
           PXWindowDrawFunction(GUISystem, PXWindow);


           PXWindowDrawRectangleFill();



           // Drawing routines?


           #if OSUnix
               const int resultID = XFillRectangles(Display *display, Drawable d, GC gc, XRectangle *rectangles, int nrectangles);
           #elif OSWindows
               const int resultID = FillRect();
           #endif


   Window XCreateSimpleWindow(Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, unsigned long border, unsigned long background);


           XGetErrorText();
           XGetErrorDatabaseText();

           XSetIOErrorHandler():
           int(int(*handler)(Display *));


    */




#elif OSWindows

    pxUI->hInstance = GetModuleHandle(NULL);

    PXWindowsMonitorEnumInfo pxWindowsMonitorEnumInfo;
    PXClear(PXWindowsMonitorEnumInfo, &pxWindowsMonitorEnumInfo);
    pxWindowsMonitorEnumInfo.UI = pxUI;

    BOOL result = EnumDisplayMonitors(NULL, NULL, PXEngineUIMonitorEnumCallBack, (LPARAM)&pxWindowsMonitorEnumInfo);

    pxUI->MonitorList = PXMemoryHeapCallocT(PXMonitor, pxUI->MonitorCount);
    pxUI->MonitorCount = 0;

    pxWindowsMonitorEnumInfo.DoStore = 1;
    result = EnumDisplayMonitors(NULL, NULL, PXEngineUIMonitorEnumCallBack, (LPARAM)&pxWindowsMonitorEnumInfo);
#endif
}


#if OSUnix
int PXAPI PXEngineUICallBackHandler(Display PXREF displayHandle, XErrorEvent PXREF xErrorEvent)
{
    // convert err->error_code to your own enum
    PXResult pxResult = PXEngineUIErrorFromX(xErrorEvent->error_code);


#if PXLogEnable
    char errorBuffer[256];

    XGetErrorText(displayHandle, xErrorEvent->error_code, errorBuffer, 256);

    PXLogPrint
    (
        PXLoggingError,
        PXEngineUIText,
        "X11",
        "(%i) %s",
        xErrorEvent->error_code,
        errorBuffer
    );
#endif

    return 0;
}
PXEngineUI* PXAPI PXEngineUIDisplayInstance(void)
{
    PXAssert(_GlobalUIREF, "You forgot to init this");

    return _GlobalUIREF;
}
Display* PXAPI PXEngineUIDisplayGet(PXEngineUI PXREF pxUI)
{
    if(!pxUI)
    {
        return PXNull;
    }

    PXAssert(pxUI->DisplayHandle, "You forgot to init this");

    return pxUI->DisplayHandle;
}
int PXAPI PXEngineUIScreenIDDefaultGet(PXEngineUI PXREF pxUI)
{
    if(!pxUI)
    {
        return 0;
    }

    Display* xDisplay = PXEngineUIDisplayGet(pxUI);

    if(!xDisplay)
    {
        return 0;
    }

    int screenID = DefaultScreen(xDisplay);

    return screenID;

   // return pxUI->ScreenIDHandle;
}
#elif OSWindows

#endif

void PXAPI PXEngineUIScreenSizeGet(PXRectangleXYWHI32 PXREF pxRectangleXYWH)
{
    if(!pxRectangleXYWH)
    {
        return;
    }

#if OSUnix

    const PXEngineUI PXREF pxUI = PXEngineUIDisplayInstance();
    const Display PXREF xDisplayHandle = PXEngineUIDisplayGet(pxUI);
    const int screenIDHandle = PXEngineUIScreenIDDefaultGet(xDisplayHandle);

    pxRectangleXYWH->X = 0;
    pxRectangleXYWH->Y = 0;
    pxRectangleXYWH->Width = XDisplayWidth(xDisplayHandle, screenIDHandle);
    pxRectangleXYWH->Height = XDisplayHeight(xDisplayHandle, screenIDHandle);

#elif PXOSWindowsDestop
    RECT desktop;

    // Get a handle to the desktop window
    const HWND desktopHandle = GetDesktopWindow();

    if(!desktopHandle)
    {
        return;
    }

    // Get the size of screen to the variable desktop
    const BOOL rectResult = GetWindowRect(desktopHandle, &desktop);

    if(!rectResult)
    {
        return;
    }

    // The top left corner will have coordinates (0,0)
    // and the bottom right corner will have coordinates
    // (horizontal, vertical)

    pxRectangleXYWH->X = desktop.left;
    pxRectangleXYWH->Y = desktop.top;
    pxRectangleXYWH->Width = desktop.right;
    pxRectangleXYWH->Height = desktop.bottom;
#else
    pxRectangleXYWH->X = 0;
    pxRectangleXYWH->Y = 0;
    pxRectangleXYWH->Width = 0;
    pxRectangleXYWH->Height = 0;
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Size-Get",
        "Desktop : X:%4i Y:%4i H:%4i W:%4i",
        pxRectangleXYWH->X,
        pxRectangleXYWH->Y,
        pxRectangleXYWH->Width,
        pxRectangleXYWH->Height
    );
#endif
}





PXResult PXAPI PXEngineUICursorPosition(PXVector2I32S PXREF position, const PXBool isWrite)
{
#if OSUnix
    return PXResultRefusedNotImplemented;
#elif OSWindows

    BOOL success = 0;

    if(!isWrite)
    {
        POINT point;

        success = GetCursorPos(&point); // Windows 2000, user32.dll, winuser.h

        position->X = point.x;
        position->Y = point.y;
    }
    else
    {
        success = SetCursorPos(position->X, position->Y); // Windows 2000, user32.dll, winuser.h
    }

    const PXResult result = PXErrorCurrent(success);

    return result;

#else
    return PXResultRefusedTypeNotSupported;
#endif
}










PXSize PXAPI PXEngineUIEventPendingAmount()
{
    PXSize amount = 0;

#if OSUnix
    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);

    amount = XPending(xDisplay); // X11
#elif OSWindows
    amount = GetQueueStatus(QS_ALLINPUT); // Windows 2000, User32.dll
#endif

    return amount;
}

PXResult PXAPI PXEngineUIEventPoll(void)
{
#if OSUnix
    XEvent windowEvent;

    PXEngineUI PXREF pxUI = PXEngineUIDisplayInstance();
    Display PXREF xDisplayHandle = PXEngineUIDisplayGet(pxUI);

    /*
        Window root_return, child_return;
        int root_x_return, root_y_return;
        int win_x_return, win_y_return;
        unsigned int mask_return;
        /*
         * We need:
         *     child_return - the active window under the cursor
         *     win_{x,y}_return - pointer coordinate with respect to root window
         * /
        int retval = XQueryPointer
        (
            display,
            root_window,
            &root_return,
            &child_return,
            &root_x_return,
            &root_y_return,
            &win_x_return,
            &win_y_return,
            &mask_return
        );*/



        // Multithread lock
    XLockDisplay(xDisplayHandle);

    // poll all events
    for(;;)
    {
        // Check if we have events
        const PXI32U amountOfEventsPending = XPending(xDisplayHandle);

        if(0 == amountOfEventsPending)
        {
            // Done
            break;
        }

        for(PXI32U i = 0; i < amountOfEventsPending; ++i)
        {
            // Fetch event form the display
            XNextEvent(xDisplayHandle, &windowEvent);

            // Translate event
            PXWindowEventTranslator(&windowEvent);
        }
    }

    // Multithread unlock
    XUnlockDisplay(xDisplayHandle);

#elif PXOSWindowsDestop
    MSG message;
    LRESULT dispatchResult;
    BOOL isOK;

    for(;;)
    {
        isOK = PeekMessage(&message, PXNull, 0, 0, PM_NOREMOVE); // Windows 2000, User32.dll, winuser.h

        if(!isOK)
        {
            break; // Stop, no more messages
        }

        isOK = GetMessage(&message, PXNull, 0, 0); // Windows 2000, User32.dll, winuser.h

        if(!isOK)
        {
            continue; // Skip, message fetch failed
        }

        isOK = TranslateMessage(&message); // Windows 2000, User32.dll, winuser.h
        dispatchResult = DispatchMessage(&message); // Windows 2000, User32.dll, winuser.h
    }
#endif

    return PXResultOK;
}





#if 0
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

        PXColorRGBA_U8 PXColorRGBA_U8;

        PXColorHSVToRGBAI8(&hsv, &PXColorRGBA_U8);

        vertex->Red = PXColorRGBA_U8.Red;
        vertex->Green = PXColorRGBA_U8.Green;
        vertex->Blue = PXColorRGBA_U8.Blue;
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
#endif

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

#if 0
PXBool PXAPI PXWindowValueFetch
(
    PXWindow PXREF pxUIElementList, 
    const PXSize dataListAmount,
    const PXWindowPropertyType pxUIElementProperty,
    void PXREF dataList
)
{

    for(size_t i = 0; i < dataListAmount; ++i)
    {
        PXWindow PXREF pxWindow = &pxUIElementList[i];

        switch(pxUIElementProperty)
        {
            case PXWindowPropertyTypeSliderPercentage:
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
#endif

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


#if OSUnix && 0
PXResult PXAPI PXErrorCurrent(const int xSysstemErrorID)
{
    switch(xSysstemErrorID)
    {
        case BadValue:
            return PXResultRefusedParameterInvalid; // input is not valid
        case BadWindow:
            return PXResultRefusedNotFound; // object id invalid
        case BadAlloc:
            return PXResultFailedMemoryAllocation;
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

PXResult PXAPI PXEngineUIOnWindowCreate(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnWindowSizeChanged(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXWindowEventResizeData PXREF resizeData = &pxWindowEvent->Resize;

    UIWindow PXREF g_root_workspace = PXWindowUIDataGet(pxWindow);
    UIState PXREF g_ui_state = PXWindowUIStateGet(pxWindow);

#if PXLogEnable
    PXECSInfo PXREF pxECSInfo = (PXECSInfo*)pxWindow;

    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineUIText,
        PXEngineUIEventText,
        "Resize, <%ix%i> (PXID:%4i)",
        resizeData->Width,
        resizeData->Height,
        pxECSInfo->ID
    );
#endif

    //PXWindowLayoutCompute(pxWindow);

    {
        glViewport(0, 0, resizeData->Width, resizeData->Height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, resizeData->Width, resizeData->Height, 0.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

    UI_ComputeLayout
    (
        pxEngineUI,
        g_root_workspace, 
        0,
        0, 
        resizeData->Width,
        resizeData->Height, 
        g_ui_state
    );

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnWindowSizeChanging(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    UIWindow PXREF g_root_workspace = PXWindowUIDataGet(pxWindow);
    UIState PXREF g_ui_state = PXWindowUIStateGet(pxWindow);

    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);
    HDC hdc = PXWindowDCGet(pxWindow);

    RECT rc;
    GetClientRect(pxWindowHandle.Data, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Force dynamic layout calculations & repaint while dragging
    UI_ComputeLayout(pxEngineUI, g_root_workspace, 0, 0, (float)width, (float)height, &g_ui_state);

    // Re-execute your drawing pass manually while Win32 is frozen
    glClearColor(0.12f, 0.12f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    UI_RenderWindow(pxEngineUI, g_root_workspace, g_ui_state);
    SwapBuffers(hdc);

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnWindowMoved(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

#if PXLogEnable
    PXECSInfo PXREF pxECSInfo = (PXECSInfo*)pxWindow;

    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineUIText,
        PXEngineUIEventText,
        "Moved PXID:<%i>",
        pxECSInfo->ID
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnWindowClosing(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnWindowClosed(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseMoved(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXWindowEventMouseMoveData PXREF mouseMoveData = &pxWindowEvent->MouseMove;

    UIWindow PXREF g_root_workspace = PXWindowUIDataGet(pxWindow);
    UIState PXREF g_ui_state = PXWindowUIStateGet(pxWindow);
    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    UI_HandleMouseMove
    (
        pxEngineUI,
        g_root_workspace, 
        g_root_workspace,
        mouseMoveData->Position.X,
        mouseMoveData->Position.Y,
        g_ui_state
    );
    InvalidateRect(pxWindowHandle.Data, NULL, FALSE);


    //wantToIgnore = PXTrue;

#if 0
    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineUIText,
        "Event",
        "PXID:<%i>, MouseMove, Px:%4i Py:%4i Dx:%4i Dy:%4i",
        pxECSInfo->ID,
        inputMouseMove->Position.X,
        inputMouseMove->Position.Y,
        inputMouseMove->Delta.X,
        inputMouseMove->Delta.Y
    );
#endif


    PXWindow* pxWindowHovered = PXNull;

    pxWindowHandle = PXWindowHandleGetAtPosition(mouseMoveData->Position.X, mouseMoveData->Position.Y);

#if 0
    if(pxWindow)
    {
        if(!pxWindow->State.IsInFocus)
        {
            pxWindow->State.IsInFocus = PXTrue;

            //-------------------------------------
            // Trigger enter event
            pxWindowEvent->Type = PXWindowEventTypeElementFocusEnter;
            PXWindowEventConsumer(pxWindowEvent);
            //-------------------------------------

            // Request WM_MOUSELEAVE for later, WM_MOUSEMOVE cant detect if we TAB out
            TRACKMOUSEEVENT tme = { 0 };
            tme.cbSize = sizeof(tme);
            tme.dwFlags = TME_LEAVE;
            tme.hwndTrack = pxWindowEvent->WindowHandle;
            TrackMouseEvent(&tme);
        }
    }
#endif

    // Overrride
#if 0
    pxMouseMove->IsOwnedByProcess = PXWindowHandleOwnedByThisProcess(pxMouseMove->WindowHandleHovered);

    if(pxMouseMove->IsOwnedByProcess)
    {
        pxWindowHovered = PXWindowFromHandle(pxMouseMove->WindowHandleHovered.Data);
    }
#endif


    // Hit-Check
    PXWindow PXREF windowHit = PXWindowHitTest(pxWindow, mouseMoveData->Position.X, mouseMoveData->Position.Y);

    // Set ALL hovers invalid.
    PXWindowHitReset(pxWindow);

    // Set our current hit
    if(windowHit)
    {
        //windowHit->State.Hover = PXTrue;
    }

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseEnter(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

#if PXLogEnable
    PXECSInfo PXREF pxECSInfo = (PXECSInfo*)pxWindow;

    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineUIText,
        PXEngineUIEventText,
        "Focus-Enter, (PXID:%4i)",
        pxECSInfo->ID
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXWindowEventMouseButtonData PXREF mouseMoveData = &pxWindowEvent->MouseButton;

    PXWindow PXREF windowHit = PXWindowHitTest(pxWindow, mouseMoveData->Position.X, mouseMoveData->Position.Y);

    //wantToIgnore = PXTrue;

#if PXLogEnable
    PXECSInfo PXREF pxECSInfo = (PXECSInfo*)pxWindow;

    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineUIText,
        PXEngineUIEventText,
        "Click, <PXID:%4i>",
        pxECSInfo->ID
    );

    if(!windowHit)
    {
        return;
    }
#endif

#if 0
    windowHit->State.Active = PXTrue;

    if(windowHit->OnClick)
    {
        windowHit->OnClick(windowHit);
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseLeave(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;

    /*   
    if(pxWindow->State.IsInFocus)
    {
        pxWindow->State.IsInFocus = PXFalse;

    
    }
    */

#if PXLogEnable
    PXECSInfo PXREF pxECSInfo = (PXECSInfo*)pxWindow;

    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineUIText,
        PXEngineUIEventText,
        "Focus-Leave (PXID:%4i)",
        pxECSInfo->ID
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonRightDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonRightUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonRightDoubleClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonLeftDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXWindowEventMouseButtonData PXREF mouseButtonData = &pxWindowEvent->MouseButton;

    UIWindow PXREF g_root_workspace = PXWindowUIDataGet(pxWindow);
    UIState PXREF g_ui_state = PXWindowUIStateGet(pxWindow);

    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    UI_HandleMouseClick
    (
        pxEngineUI, 
        g_root_workspace,
        g_root_workspace,
        mouseButtonData->Position.X,
        mouseButtonData->Position.Y, 
        PXFalse, 
        PXFalse,
        g_ui_state
    );
    InvalidateRect(pxWindowHandle.Data, NULL, FALSE);

    /*
    if(pxWindow && !pxWindow->State.Floating) {

        if(c.y < pxWindow->TitleBarHeight)
        {
            POINT s = c;
            ClientToScreen(pxWindowEvent->WindowHandle, &s);
            //PXWindowTearOffToFloating(pxWindow, s);
            return 0;
        }
    }
    */

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonLeftUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    UIWindow PXREF g_root_workspace = PXWindowUIDataGet(pxWindow);
    UIState PXREF g_ui_state = PXWindowUIStateGet(pxWindow);

    PXWindowEventMouseButtonData PXREF mouseButtonData = &pxWindowEvent->MouseButton;

    PXWindowHandle pxWindowHandle = PXWindowHandleGet(pxWindow);

    UI_HandleMouseClick
    (
        pxEngineUI,
        g_root_workspace,
        g_root_workspace,
        mouseButtonData->Position.X, 
        mouseButtonData->Position.Y,
        PXFalse, 
        PXFalse,
        g_ui_state
    );
    RECT rc;
    GetClientRect(pxWindowHandle.Data, &rc);
    UI_ComputeLayout(pxEngineUI, g_root_workspace, 0, 0, (float)rc.right, (float)rc.bottom, g_ui_state);
    InvalidateRect(pxWindowHandle.Data, NULL, FALSE);

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonLeftDoubleClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonMiddleDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonMiddleUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnMouseButtonMiddleDoubleClick(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnKeyboardKeyUp(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnKeyboardKeyDown(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnKeyboardInput(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXWindowEventKeyboardData* inputKeyboard = &pxWindowEvent->Keyboard;
    const char* buttonState = PXKeyPressStateToString(inputKeyboard->PressState);

    // wantToIgnore = PXTrue;

#if PXLogEnable
    PXECSInfo PXREF pxECSInfo = (PXECSInfo*)pxWindow;

    PXLogPrint
    (
        PXLoggingEvent,
        PXEngineUIText,
        PXEngineUIEventText,
        "KeyBoard, PXID:<%i>, KeyBoard, VKey:%-3i (%lc), %s",
        pxECSInfo->ID,
        inputKeyboard->VirtualKey,
        inputKeyboard->CharacterW,
        buttonState
    );
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineUIOnDraw(PXEngineUI PXREF pxEngineUI, PXEngineUIEvent PXREF pxWindowEvent)
{
    PXResult pxResult = PXResultOK;

    if(!(pxEngineUI && pxWindowEvent))
    {
        return PXResultRefusedParameterNull;
    }

    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXEngineUIDrawInfo PXREF pxUIDrawInfo = &pxWindowEvent->Draw;

    if(!pxWindow)
    {
        return PXResultRefusedWindowHandleNull;
    }

    const PXBool isPhysical = PXWindowIsPhysical(pxWindow);
    HWND windowHandle = PXNull;
    RECT clientRect;

    UIWindow PXREF uiWindow =  PXWindowUIDataGet(pxWindow);
    UIState PXREF uiState = PXWindowUIStateGet(pxWindow);


    if(isPhysical)
    {
        PXBool isMinimized = PXWindowIsMinimized(pxWindow);

        if(isMinimized)
        {
            // The window is minimized. Skip the entire frame immediately.
            return PXResultRefusedWindowSurfaceHidden;
        }
                
        PXBool isLegal = PXWindowIsSurfaceValid(pxWindow);  

        if(!isLegal)
        {
            // The window has no visible surface area. Reject early.
            return PXResultRefusedWindowSurfaceSizeInvalid;
        }

        PXWindowClientArea(pxWindow, &pxUIDrawInfo->ClientArea);
        PXWindowCursorPosition(pxWindow, &pxUIDrawInfo->CursorPositionGlobal, &pxUIDrawInfo->CursorPositionLocal);
    }

    /*
    if(!pxWindow->isVirtual)
    {
        // bind OS framebuffer
        // clear, set viewport, etc.
    }*/

    // Render root element

#if 0
    PXWindowEvent pxWindowEvent;
    PXClear(PXWindowEvent, &pxWindowEvent);
    pxWindowEvent.WindowSender = pxWindow;

#if OSUnix
    PXEngineUI PXREF pxUI = PXEngineUIDisplayInstance();
    pxWindowEvent.XDisplayHandle = PXEngineUIDisplayGet(pxUI);
#elif OSWindows
    pxWindowEvent.WindowHandle = pxWindow->Handle.Data;
    pxWindowEvent.EventID = WM_PAINT;
#endif
#endif

    /*
    PXDrawInfo* pxDrawInfo = &pxWindowEvent.Draw;
    pxDrawInfo->Window = pxWindow;
    pxDrawInfo->Engine = pxWindow->Engine;
    pxDrawInfo->Render = PXEngineRenderSystemGet(pxWindow->Engine);
    pxDrawInfo->Color.Red = pxWindow->BackGroundColor.Red;
    pxDrawInfo->Color.Green = pxWindow->BackGroundColor.Green;
    pxDrawInfo->Color.Blue = pxWindow->BackGroundColor.Blue;
    */


#if 0
    //PXConsoleGoToXY(0, 10);
    PXLogPrint
    (
        PXLoggingInfo,
        "Window",
        "Darw",
        "ID:%4i, %4i,%4i,%4i,%4i",
        pxWindow->Info.ID,
        pxDrawInfo->RectangleXYWH.X,
        pxDrawInfo->RectangleXYWH.Y,
        pxDrawInfo->RectangleXYWH.Width,
        pxDrawInfo->RectangleXYWH.Height
    );
#endif





    //-----------------------------------------------------
    // Actual render
    //-----------------------------------------------------
    if(isPhysical)
    {
        PXColorRGBA_F32 PXREF pxColorRGBA = &uiWindow->ColorBackGround;
        const GLbitfield clearFlags =
            //GL_POLYGON_BIT |
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT |
            GL_STENCIL_BUFFER_BIT |
            GL_ACCUM_BUFFER_BIT;

        glClearColor
        (
            pxColorRGBA->Red, 
            pxColorRGBA->Green,
            pxColorRGBA->Blue,
            pxColorRGBA->Alpha
        );
        glClear(clearFlags);
    }

    // Select the rectangle to render uinto
    PXWindowViewPortSet(pxWindow, &pxUIDrawInfo->ClientArea, PXFalse);


    //PXWindowEventRender(&pxWindowEvent);
    //-----------------------------------------------------


  

    //-----------------------------------------------------
    UIWindow* g_root_workspace = PXWindowUIDataGet(pxWindow);
    UIState* g_ui_state = PXWindowUIStateGet(pxWindow);

    UI_RenderWindow(pxEngineUI, &g_root_workspace, &g_ui_state);

    for(int i = 0; i < pxEngineUI->g_floating_count; i++)
    {
        if(pxEngineUI->g_floating_windows[i]->is_popped_out)
        {
            if(pxEngineUI->g_floating_windows[i]->computed_rect.Width == 0 || !pxEngineUI->g_floating_windows[i]->parent) {
                pxEngineUI->g_floating_windows[i]->computed_rect.Width = 250;
                pxEngineUI->g_floating_windows[i]->computed_rect.Height = 180;
            }
           
            UI_ComputeLayout
            (
                pxEngineUI,
                pxEngineUI->g_floating_windows[i],
                pxEngineUI->g_floating_windows[i]->floating_x,
                pxEngineUI->g_floating_windows[i]->floating_y, 
                pxEngineUI->g_floating_windows[i]->computed_rect.Width,
                pxEngineUI->g_floating_windows[i]->computed_rect.Height,
                g_ui_state
            );
           
            UI_RenderWindow(pxEngineUI, pxEngineUI->g_floating_windows[i], g_ui_state);
        }
    }

    if(g_ui_state->mouse_pressed && g_ui_state->active_dragging_window && g_ui_state->potential_drop_target)
    {
        PXRectangleXYWHF32 target_rect = g_ui_state->potential_drop_target->computed_rect;
        PXWindowDrawRectangleWireGL2(target_rect.X, target_rect.Y, target_rect.Width, target_rect.Height, 0.1f, 0.6f, 1.0f, 3.0f);
    }
    //-----------------------------------------------------




    //-----------------------------------------------------

    //-----------------------------------------------------
    // Render child virtual windows
#if 1
    if(uiWindow->DoRenderingChildren)
    {
        PXSize siblingAmount = PXWindowSiblingAmount(pxWindow);

        for(PXSize i = 0; i < siblingAmount; ++i)
        {
            PXWindow PXREF sibling = PXWindowSiblingGet(pxWindow, i);

            PXEngineUIOnDraw(pxEngineUI, sibling);
        }

        // Draw children
        PXSize childAmount = PXWindowChildAmount(pxWindow);

        for(PXSize i = 0; i < childAmount; ++i)
        {
            PXWindow PXREF child = PXWindowChildGet(pxWindow, i);

            PXEngineUIOnDraw(pxEngineUI, child);
        }
    }
#endif
    //-----------------------------------------------------

    if(isPhysical)
    {
        PXWindowBufferSwap(pxWindow);
       // ValidateRect(pxWindow, NULL);
    }

    // NO!
    // BOOL openglInstance = wglMakeCurrent(hdc, PXNull);


#if 0
    PXResult pxResult = PXResultOK;
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXEngineUIDrawInfo PXREF pxDrawInfo = &pxWindowEvent->Draw;

    PXWindowDrawClear_GL(pxWindow, pxDrawInfo);

    PXECSInfo* target = pxWindow->EventList.CallBackOwner;
    PXECSDrawFunction pxECSDrawFunction = pxWindow->EventList.CallBackDraw;

    if(pxECSDrawFunction && target)
    {
        //pxDrawInfo->DrawCall = pxECSDrawFunction;
    }
    else
    {
        target = pxWindow;
    }

    if(pxDrawInfo->DrawCall)
    {
        pxResult = pxDrawInfo->DrawCall(target, pxDrawInfo);
    }
#endif



#if 0
    // Draw
    drawBox_MBP
    (
        pxWindow->Layout.abs,
        pxWindow->Layout.Margin,
        pxWindow->Layout.Padding,
        pxWindow->Layout.BorderWidth
    );
#endif

#if 0
    if((PXGraphicAPIOpenGL == pxWindow->GraphicSystem) && 1)
    {
        if(pxWindow->State.IsInFocus)
        {
            glColor3f(0.0f, 1.0f, 0.0f);
        }
        else
        {
            glColor3f(1.0f, 0.0f, 0.0f);
        }

        // Optional: set line width
        glLineWidth(2.0f);

        // glScalef(0.9, 0.9, 1);

         // Draw rectangle as a line loop
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glBegin(GL_QUADS);
        glVertex2f(pxDrawInfo->RectangleXYWH.X, pxDrawInfo->RectangleXYWH.Y); // bottom-left
        glVertex2f(pxDrawInfo->RectangleXYWH.X + pxDrawInfo->RectangleXYWH.Width, pxDrawInfo->RectangleXYWH.Y); // bottom-right
        glVertex2f(pxDrawInfo->RectangleXYWH.X + pxDrawInfo->RectangleXYWH.Width, pxDrawInfo->RectangleXYWH.Y + pxDrawInfo->RectangleXYWH.Height); // top-right
        glVertex2f(pxDrawInfo->RectangleXYWH.X, pxDrawInfo->RectangleXYWH.Y + pxDrawInfo->RectangleXYWH.Height); // top-left
        glEnd();

        DrawCursorMarker2D(pxDrawInfo->CursorPosition.X, -pxDrawInfo->CursorPosition.Y, 5, 5);
    }
#endif

    return PXResultOK;
}



void PXAPI PXEngineUIDrawPanel(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo)
{   
    if(!(pxWindow && pxDrawInfo))
    {
        return;
    }

    UIWindow PXREF uiWindow = PXWindowUIDataGet(pxWindow);



#if 0
    DrawTitlebar(win->total_bounds, win->title);
    DrawWindowBorders(win->total_bounds);
    DrawControlButtons(win->total_bounds);

    // ---------------------------------------------------------
    // STEP 2: Calculate Bottom-Left OpenGL Coordinates
    // ---------------------------------------------------------
    int gl_x = win->client_bounds.x;
    int gl_y = g_HostWindowHeight - (win->client_bounds.y + win->client_bounds.h);
    int gl_w = win->client_bounds.w;
    int gl_h = win->client_bounds.h;

    // ---------------------------------------------------------
    // STEP 3: Sandbox the Client Area (Scissor + Viewport)
    // ---------------------------------------------------------
    // 1. Hardware clip the drawing space so it cannot leak outside the window borders
    glEnable(GL_SCISSOR_TEST);
    glScissor(gl_x, gl_y, gl_w, gl_h);

    // 2. Change the Viewport. (0,0) inside the user's callback now maps directly 
    //    to the bottom-left corner of the sub-window's client area!
    glViewport(gl_x, gl_y, gl_w, gl_h);

    // ---------------------------------------------------------
    // STEP 4: Fire the Clean Slate Callback
    // ---------------------------------------------------------


    if(pxWindow->OnDraw)
    {
        PXDrawInfo pxDrawInfo;
        PXClear();

        // The user can render objects from (0,0) to (gl_w, gl_h) safely
        pxWindow->OnDraw(win->user_data, gl_w, gl_h);
    }

    // ---------------------------------------------------------
    // STEP 5: Clean Up and Reset State
    // ---------------------------------------------------------
    glDisable(GL_SCISSOR_TEST);

    // Reset viewport back to full window scale for the next frame elements
    PXWindowViewPortSet(pxWindow, &pxWindow->Layout.SpaceDraw, PXFalse);

#endif






#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXEngineUIText,
        "Layout",
        "\n"
        "%20s : %4i\n"
        "%20s : (LTRB) %4i %4i %4i %4i\n"
        "%20s : (LTRB) %4i %4i %4i %4i\n"
        "%20s : (XYWH) %4i %4i %4i %4i\n"
        "%20s : (XYWH) %4i %4i %4i %4i\n"
        "%20s : (XYWH) %4i %4i %4i %4i\n"
        "%20s : %6.2f\n",
        "PXID", pxWindow->Info.ID,
        "Padding", pxWindow->Layout.Padding.Left, pxWindow->Layout.Padding.Top, pxWindow->Layout.Padding.Right, pxWindow->Layout.Padding.Bottom,
        "Margin", pxWindow->Layout.Margin.Left, pxWindow->Layout.Margin.Top, pxWindow->Layout.Margin.Right, pxWindow->Layout.Margin.Bottom,
        "SpaceDraw", pxWindow->Layout.SpaceDraw.X, pxWindow->Layout.SpaceDraw.Y, pxWindow->Layout.SpaceDraw.Width, pxWindow->Layout.SpaceDraw.Height,
        "SpaceBox", pxWindow->Layout.SpaceBox.X, pxWindow->Layout.SpaceBox.Y, pxWindow->Layout.SpaceBox.Width, pxWindow->Layout.SpaceBox.Height,
        "SpaceContent", pxWindow->Layout.SpaceContent.X, pxWindow->Layout.SpaceContent.Y, pxWindow->Layout.SpaceContent.Width, pxWindow->Layout.SpaceContent.Height,
        "AspectRatio", PXWindowAspectRatio(pxWindow)
    );
#endif


    // Draw debug lines
    //PXRectangleXYWHI32 PXREF spaceDraw = &pxWindow->Layout.SpaceDraw;
    //PXRectangleXYWHI32 PXREF spaceBox = &pxWindow->Layout.SpaceBox;
    //PXRectangleXYWHI32 PXREF spaceContent = &pxWindow->Layout.SpaceContent;


    PXRectangleLTRBI32 pxRectangleMargin;
    pxRectangleMargin.Left = 3;
    pxRectangleMargin.Top = 3;
    pxRectangleMargin.Right = 3;
    pxRectangleMargin.Bottom = 3;

    uiWindow->TitleBarHeight = 25;
    uiWindow->title_text = "PX-Window";    // NULL = uses text placeholder
    uiWindow->icon_texture = 0; // 0 = no texture, draws color placeholder
    uiWindow->button_count = 3;          // e.g., 1 for just [X], 3 for [ _ ][ [] ][ X ]
    uiWindow->is_window_focused = 1;     // Uses a different theme color if active

    PXWindowDrawPanel
    (
        pxWindow,
        &pxDrawInfo->ClientArea,
        &pxRectangleMargin
    );


    return;

#if 0
    glLineWidth(3);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glColor3f(0.0f, 0.0f, 0.6f);
    PXWindowDrawRect2(pxWindow, spaceDraw);

    glColor3f(0.6f, 0.0f, 0.0f);
    PXWindowDrawRect2(pxWindow, spaceBox);

    glColor3f(0.6f, 0.8f, 0.0f);
    PXWindowDrawRect2(pxWindow, spaceContent);




    PXLineSegment pxLineSegments[4];

    glColor3f(0.6f, 1.0f, 0.6f);
    ComputeConnectorLines(spaceBox, spaceDraw, pxLineSegments);
    RenderConnectorLines(pxLineSegments);

    ComputeConnectorLines(spaceContent, spaceBox, pxLineSegments);
    RenderConnectorLines(pxLineSegments);

    glColor3f(1.0f, 1.0f, 1.0f);

#if 0
    PXWindowDrawHelloWorldTriangle
    (
        pxWindow,
        pxWindow->Layout.SpaceDraw.X,
        pxWindow->Layout.SpaceDraw.Y,
        pxWindow->Layout.SpaceDraw.Width,
        pxWindow->Layout.SpaceDraw.Height
    );
#endif


    //PXWindowViewPortSet(pxWindow, &pxWindow->Layout.SpaceContent, PXTrue);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#endif

    /*

#if 1
    PXWindowDrawHelloWorldTriangle
    (
        pxWindow,
        -1,
        -1,
        2,
        2
    );
#else
    PXWindowDrawHelloWorldTriangle
    (
        pxWindow,
        pxWindow->Layout.SpaceContent.X,
        pxWindow->Layout.SpaceContent.Y,
        pxWindow->Layout.SpaceContent.Width,
        pxWindow->Layout.SpaceContent.Height
    );
#endif
*/

// PXWindowDrawHelloWorldTriangle(pxWindow, -1, -1, 2, 2);




 //PXCamera pxCamera;
 //pxCamera.Rectangle = pxDrawInfo->RectangleXYWH;

 //PXCameraGLFPP(&pxCamera, PXCameraPerspective2D);

 //glDrawBuffer(GL_BACK);
 //glEnableClientState(GL_COLOR_ARRAY);
}




#if 0
PXResult PXAPI PXWindowEventConsumer(PXWindowEvent PXREF pxWindowEvent)
{
    // Invoke
    PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    PXECSInfo* pxECSInfo = (PXECSInfo*)pxWindow;
    PXBool wantToIgnore = PXFalse;

    if(!pxWindow)
    {
        // Window is not registerd, this means we cant send a message back
        return PXResultOK;
    }

    if(pxWindowEvent->CallBack)
    {
        pxWindowEvent->CallBack(pxWindowEvent);
    }

#if 0
    if(!pxWindow->IsInFocus && wantToIgnore)
    {
        return PXResultCancelled;
    }
#endif

    PXWindowEventFunction pxWindowEventFunction = pxWindow->EventList.CallBackEvent;

    if(pxWindowEventFunction)
    {
        pxWindowEvent->Owner = pxWindow->EventList.CallBackOwner;
        pxWindowEventFunction(pxWindowEvent);
    }

    return PXResultOK;
}
#endif








PXResult PXAPI PXEngineUIDrawRectangle2D(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    PXEngineUITheme PXREF pxGUITheme = pxDrawInfo->UITheme;

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        PXRectangle2DText,
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i",
        pxWindow->Info.ID,
        pxWindowDrawRectangleInfo->RectangleXYWH.X,
        pxWindowDrawRectangleInfo->RectangleXYWH.Y,
        pxWindowDrawRectangleInfo->RectangleXYWH.Width,
        pxWindowDrawRectangleInfo->RectangleXYWH.Height
    );
#endif
    const PXBool isHovered = 1;// PXECSInfoFlagStateCheck(&pxWindow->Info, PXECSFlagBehaviourSelected);

    //  const BOOL bbbbb = SelectObject(pxWindow->DeviceContextHandle, GetStockObject(GRAY_BRUSH));

    // const COLORREF color = RGB(255, 0, 200);
    // const HBRUSH brushAA = CreateSolidBrush(color);

    PXBrush* pxWindowBrush = PXNull;

    if(isHovered)
    {
        pxWindowBrush = pxGUITheme->BrushTextWhite;
    }
    else
    {
        pxWindowBrush = pxGUITheme->BrushMainPrimary;
    }

    PXEngineUIDrawInfo pxWindowDrawRectangleInfoSub;
    PXClear(PXEngineUIDrawInfo, &pxWindowDrawRectangleInfoSub);
    pxWindowDrawRectangleInfoSub.Brush = pxWindowBrush;
    pxWindowDrawRectangleInfoSub.Rounded = PXFalse;

    PXResult pxResult = PXWindowDrawRectangle(pxWindow, &pxWindowDrawRectangleInfoSub);

    return pxResult;
}

PXResult PXAPI PXEngineUIDrawRectangle3D_GL(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxDrawInfo)
{
    //PXAssert(width > 0, "Width is 0");
      //PXAssert(height > 0, "height is 0");

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXWindowTextText,
        PXRectangle3DText,
        "PXID:%-4i X:%-4i Y:%-4i W:%-4i H:%-4i",
        pxWindow->Info.ID,
        pxWindowDrawRectangleInfo->RectangleXYWH.X,
        pxWindowDrawRectangleInfo->RectangleXYWH.Y,
        pxWindowDrawRectangleInfo->RectangleXYWH.Width,
        pxWindowDrawRectangleInfo->RectangleXYWH.Height
    );
#endif

    const PXBool isHovered = 1;// PXECSInfoFlagStateCheck(&pxWindow->Info, PXECSFlagBehaviourSelected);
    PXEngineUITheme PXREF pxGUITheme = pxDrawInfo->UITheme;
    PXBrush* brushFront = pxGUITheme->BrushTextWhite;
    PXBrush* brushBackground = pxGUITheme->BrushMainPrimary;


    const PXF32 colorTint = isHovered * 0.4f;
    const PXF32 highFactor = 1.35f;
    const PXF32 lowFactor = 0.65f;


    PXColorRGB_U8 highColor;
    highColor.Red = colorTint + highFactor * brushBackground->ColorDate.Red;
    highColor.Green = colorTint + highFactor * brushBackground->ColorDate.Green;
    highColor.Blue = colorTint + highFactor * brushBackground->ColorDate.Blue;

    PXColorRGB_U8 lowColor;
    lowColor.Red = colorTint + lowFactor * brushBackground->ColorDate.Red;
    lowColor.Green = colorTint + lowFactor * brushBackground->ColorDate.Green;
    lowColor.Blue = colorTint + lowFactor * brushBackground->ColorDate.Blue;



    float diff = 0.1;
    float primeColor = 0.7f;// -diff / 2 + (cosf(time * 20)) * diff;

    // Optional shake effect
    float offset = 0;

    if(1)
    {
        //offset = PXMathSinusRADF32(xt * 200.8f) * 0.01f;
        float x = offset;

        // xt += 0.01;
    }
    PXOpenGL PXREF pxOpenGL = 0;// &pxGraphic->APIOpenGL;

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    pxOpenGL->Binding.Enable(GL_LINE_SMOOTH);
    pxOpenGL->Binding.Hint(GL_LINE_SMOOTH_HINT, GL_NICEST);


    /*
    // Draw gradient background
    pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    pxOpenGL->Binding.Begin(GL_QUADS);
    pxOpenGL->Binding.Color3fv(colorBottom);
    pxOpenGL->Binding.Vertex2f(-1.0f+ offset, -1.0f);
    pxOpenGL->Binding.Vertex2f(+1.0f - offset, -1.0f);
    pxOpenGL->Binding.Color3fv(colorTop);
    pxOpenGL->Binding.Vertex2f(+1.0f - offset, 1.0f);
    pxOpenGL->Binding.Vertex2f(-1.0f + offset, 1.0f);
    pxOpenGL->Binding.End();
    */

    pxOpenGL->Binding.LineWidth(1);

#if 0
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_QUADS);
    glColor3fv(borderDark); glVertex2f(x, y);
    glColor3fv(borderLight); glVertex2f(x + width, y);
    glColor3fv(borderDark); glVertex2f(x + width, y + height);
    glColor3fv(borderLight); glVertex2f(x, y + height);
    glEnd();
#else


    // Draw border (light top-left, dark bottom-right)

    PXI32S x = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.X;
    PXI32S y = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Y;
    PXI32S width = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Width;
    PXI32S height = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Height;

    if(0)
    {
        pxOpenGL->Binding.Begin(GL_LINES);

        //pxOpenGL->Binding.Color3bv(&highColor);

        // pxOpenGL->Binding.Vertex2f(-1.0f, -1.0f);
        // pxOpenGL->Binding.Vertex2f(+1.0f, -1.0f); // Top
        // pxOpenGL->Binding.Vertex2f(-1.0f, +1.0f);
        // pxOpenGL->Binding.Vertex2f(+1.0f, +1.0f); // Left


        pxOpenGL->Binding.Color3ubv(lowColor.Data);
        pxOpenGL->Binding.Vertex2f(x + width, y);
        pxOpenGL->Binding.Vertex2f(x + width, y + height); // Right
        pxOpenGL->Binding.Vertex2f(x, y + height);
        pxOpenGL->Binding.Vertex2f(x + width, y + height); // Bottom

        pxOpenGL->Binding.End();
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXEngineUIDrawRectangle3D_GDI(PXWindow PXREF pxWindow, PXEngineUIDrawInfo PXREF pxWindowDrawRectangleInfo)
{
#if 0
    PXWindowBrushSet
    (
        pxWindow,
        brushBackground,
        PXGUIDrawModeBack
    );
    PXWindowDrawRectangle
    (
        pxWindow,
        pxDrawInfo
    );


#if OSWindows

    const COLORREF highColorWIN = RGB
    (
        highColor.Red,
        highColor.Green,
        highColor.Blue
    );
    const COLORREF lowColorWIN = RGB
    (
        lowColor.Red,
        lowColor.Green,
        lowColor.Blue
    );

    const DWORD penStyle = PS_ENDCAP_SQUARE | PS_GEOMETRIC | PS_SOLID | PS_JOIN_MITER;
    const DWORD penSize = 2;
    const PXI16U borderInwardOffset = penSize - 1;

    if(penSize == 0)
    {
        return PXResultOK;
    }

    const HPEN penLight = CreatePen(penStyle, penSize, highColorWIN);
    const HPEN penDark = CreatePen(penStyle, penSize, lowColorWIN);

    HDC hdc = PXWindowDCGet(pxWindow);

    PXI32S x = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.X;
    PXI32S y = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Y;
    PXI32S width = 0;//pxWindowDrawRectangleInfo->RectangleXYWH.Width;
    PXI32S height = 0;// pxWindowDrawRectangleInfo->RectangleXYWH.Height;

    // Bottom, right, dark line
    MoveToEx(hdc, x + width - borderInwardOffset, y + borderInwardOffset, PXNull);
    SelectObject(hdc, penDark); // Windows 2000, Gdi32.dll, wingdi.h,
    LineTo(hdc, x + width - borderInwardOffset, y + height - borderInwardOffset);
    LineTo(hdc, x + borderInwardOffset, y + height - borderInwardOffset);

    SelectObject(hdc, penLight);
    LineTo(hdc, x + borderInwardOffset, y + borderInwardOffset);
    LineTo(hdc, width - borderInwardOffset, y + borderInwardOffset);

    DeleteObject(penLight);
    DeleteObject(penDark);
#endif
#endif

    return PXResultOK;
}




typedef struct PXLineSegment_
{
    float x1, y1;
    float x2, y2;
}
PXLineSegment;

void ComputeConnectorLines
(
    const PXRectangleXYWHI32* inner,
    const PXRectangleXYWHI32* outer,
    PXLineSegment lines[4])
{
    // Inner centers
    float innerCenterX = inner->X + inner->Width * 0.5f;
    float innerCenterY = inner->Y + inner->Height * 0.5f;

    // Outer centers
    float outerCenterX = outer->X + outer->Width * 0.5f;
    float outerCenterY = outer->Y + outer->Height * 0.5f;

    // TOP
    lines[0].x1 = innerCenterX;
    lines[0].y1 = inner->Y;
    lines[0].x2 = outerCenterX;
    lines[0].y2 = outer->Y;

    // BOTTOM
    lines[1].x1 = innerCenterX;
    lines[1].y1 = inner->Y + inner->Height;
    lines[1].x2 = outerCenterX;
    lines[1].y2 = outer->Y + outer->Height;

    // LEFT
    lines[2].x1 = inner->X;
    lines[2].y1 = innerCenterY;
    lines[2].x2 = outer->X;
    lines[2].y2 = outerCenterY;

    // RIGHT
    lines[3].x1 = inner->X + inner->Width;
    lines[3].y1 = innerCenterY;
    lines[3].x2 = outer->X + outer->Width;
    lines[3].y2 = outerCenterY;
}


void RenderConnectorLines(const PXLineSegment lines[4])
{
    glBegin(GL_LINES);

    for(int i = 0; i < 4; ++i)
    {
        glVertex2f(lines[i].x1, lines[i].y1);
        glVertex2f(lines[i].x2, lines[i].y2);
    }

    glEnd();
}



// Helper: Draws a solid flat-colored rectangle
void GL11_DrawSolidRect(int x, int y, int w, int h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}

// Helper: Draws an empty wireframe rectangle outline
void GL11_DrawWireRect(int x, int y, int w, int h, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + w, y);
    glVertex2i(x + w, y + h);
    glVertex2i(x, y + h);
    glEnd();
}

// -------------------------------------------------------------------
// Component Function 1: Draws a standard window control button (like X)
// -------------------------------------------------------------------
void DrawTitleBarButton(int x, int y, int size, const char* type) {
    // Draw the button background box (Classic light gray)
    GL11_DrawSolidRect(x, y, size, size, 0.75f, 0.75f, 0.75f);
    GL11_DrawWireRect(x, y, size, size, 0.40f, 0.40f, 0.40f); // dark outline

    // Draw the structural symbols inside the button using simple OpenGL vector lines
    glColor3f(0.0f, 0.0f, 0.0f); // Black symbols
    glLineWidth(2.0f);

    if(type[0] == 'X' || type[0] == 'x') {
        // Draw the "X" Close Cross
        int offset = size / 4;
        glBegin(GL_LINES);
        glVertex2i(x + offset, y + offset);
        glVertex2i(x + size - offset, y + size - offset);
        glVertex2i(x + size - offset, y + offset);
        glVertex2i(x + offset, y + size - offset);
        glEnd();
    }
    else if(type[0] == '_') {
        // Draw the "_" Minimize Dash
        int padding_x = size / 4;
        int bottom_y = y + (size * 3) / 4;
        glBegin(GL_LINES);
        glVertex2i(x + padding_x, bottom_y);
        glVertex2i(x + size - padding_x, bottom_y);
        glEnd();
    }

    glLineWidth(1.0f); // Reset line thickness state
}

// -------------------------------------------------------------------
// Component Function 2: Draws a text string using era-safe Win32 fonts
// -------------------------------------------------------------------
void DrawTitleBarTextFallback(int x, int y, const char* text) {
    // Native Win32 GDI Font mapping system works flawlessly on OpenGL 1.1
    // Position the raster position pointer where text should emit
    glRasterPos2i(x, y + 16);

    if(!text || text[0] == '\0') {
        text = "[Untitled Window]"; // Safe hardcoded placeholder fallback
    }

    // Loop through characters and print them out onto the display canvas
    while(*text) {
        // Uses the standard Win32 default system font list bound to OpenGL
        glCallList(1000 + (unsigned char)*text);
        text++;
    }
}

PXResult PXAPI PXWindowDrawPanel
(
    PXWindow PXREF pxWindow,
    PXRectangleXYWHI32 PXREF rectangle,
    PXRectangleLTRBI32 PXREF margin
)
{
    UIWindow PXREF uiWindow = PXWindowUIDataGet(pxWindow);

    float dpi_scale = 1;

    // 1. Convert physical screen coordinates to Virtual Coordinates for layout math
    int virtual_screen_w = (int)((float)rectangle->Width / dpi_scale);
    int virtual_screen_h = (int)((float)rectangle->Height / dpi_scale);

    // 2. Calculate the 10% margin in virtual space
    int virtual_margin_x = PercentToPixels(margin->Left, virtual_screen_w);
    int virtual_margin_y = PercentToPixels(margin->Top, virtual_screen_h);

    // 3. High/Low Resolution Guard: Clamp to safe structural thresholds
    if(virtual_margin_x < MIN_MARGIN) virtual_margin_x = MIN_MARGIN;
    if(virtual_margin_x > MAX_MARGIN) virtual_margin_x = MAX_MARGIN;

    if(virtual_margin_y < MIN_MARGIN) virtual_margin_y = MIN_MARGIN;
    if(virtual_margin_y > MAX_MARGIN) virtual_margin_y = MAX_MARGIN;

    // 4. Formulate the UI bounds in Virtual Space
    int v_rect_x = virtual_margin_x;
    int v_rect_y = virtual_margin_y;
    int v_rect_w = virtual_screen_w - (virtual_margin_x * 2);
    int v_rect_h = virtual_screen_h - (virtual_margin_y * 2);

    // 5. Output raw Physical Pixels optimized for OpenGL consumption
    PXRectangleXYWHI32 physical_rect;
    physical_rect.X = ApplyDPIScale(v_rect_x, dpi_scale);
    physical_rect.Y = ApplyDPIScale(v_rect_y, dpi_scale);
    physical_rect.Width = ApplyDPIScale(v_rect_w, dpi_scale);
    physical_rect.Height = ApplyDPIScale(v_rect_h, dpi_scale);




    PXColorRGBA_U8 titlebar =
        //{ 85, 155, 215, 0xFF };
    { 32, 32, 32, 0xFF };



    PXWindowDrawRectangleGL(&physical_rect, &titlebar);




    int bx = physical_rect.X;
    int by = physical_rect.Y;
    int bw = physical_rect.Width;
    int bh = uiWindow->TitleBarHeight;

    // 1. Draw Title Bar Background (Blue if focused/active, Dark Gray if inactive)
    if(uiWindow->is_window_focused)
    {
        GL11_DrawSolidRect(bx, by, bw, bh, 0.05f, 0.05f, 0.05f); // Classic Blue Theme
    }
    else {
        GL11_DrawSolidRect(bx, by, bw, bh, 0.22f, 0.22f, 0.22f);  // Inactive Gray Theme
    }

    // -------------------------------------------------------------------
    // LEFT SIDE ELEMENT POSITIONING: ICON & TEXT
    // -------------------------------------------------------------------
    int current_left_x = bx + 6; // Initial layout offset padding from the frame edge
    int padding_y = (bh - 16) / 2; // Center elements vertically

    // Handle Icon layout
    if(uiWindow->icon_texture != 0)
    {
        // Map actual texture coordinates if an asset exists
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, uiWindow->icon_texture);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(current_left_x, by + padding_y);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(current_left_x + 16, by + padding_y);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(current_left_x + 16, by + padding_y + 16);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(current_left_x, by + padding_y + 16);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }
    else 
    {
        // Draw an explicit Color Placeholder Box if no image texture asset is found
        GL11_DrawSolidRect(current_left_x, by + padding_y, 16, 16, 0.9f, 0.6f, 0.1f); // Orange placeholder box
        GL11_DrawWireRect(current_left_x, by + padding_y, 16, 16, 1.0f, 1.0f, 1.0f);
    }

    // Shift cursor position right to allocate space for the text block entry
    current_left_x += 16 + 8;

    // Render Text Layer
    glColor3f(1.0f, 1.0f, 1.0f); // White text colors
    DrawTitleBarTextFallback(current_left_x, by + padding_y, uiWindow->title_text);

    // -------------------------------------------------------------------
    // RIGHT SIDE ELEMENT POSITIONING: WINDOW CONTROL BUTTONS
    // -------------------------------------------------------------------
    int button_size = 16;
    int current_right_x = (bx + bw) - button_size - 6; // Place first button right-aligned
    int btn_padding_y = (bh - button_size) / 2;

    // Always draw the Close "X" button on the absolute far right edge
    DrawTitleBarButton(current_right_x, by + btn_padding_y, button_size, "X");

    // Shift left to allocate layout tracks for more structural action elements if requested
    if(uiWindow->button_count >= 2) {
        current_right_x -= (button_size + 4);
        DrawTitleBarButton(current_right_x, by + btn_padding_y, button_size, "_"); // Minimize
    }

    return PXResultOK;
}

void PXAPI PXWindowDrawRectangleGL(PXRectangleXYWHI32 PXREF rectangle, PXColorRGBA_F32 PXREF pxColor)
{
    PXColorRGBA_F32 bottom =
        // { 72, 200, 177, 0xFF };
    { 86 / 255.0f, 73 / 255.0f, 197 / 255.0f, 1.0f };

    PXColorRGBA_F32 top =
        //{ 85, 155, 215, 0xFF };
    { 139 / 255.0f, 85 / 255.0f, 214 / 255.0f, 1.0f };

    if(pxColor)
    {
        bottom = *pxColor;
        top = *pxColor;
    }

    glBegin(GL_QUADS);
    glColor3fv(bottom.Data);
    glVertex2i(rectangle->X, rectangle->Y);
    glVertex2i(rectangle->X + rectangle->Width, rectangle->Y);
    glColor3fv(top.Data);
    glVertex2i(rectangle->X + rectangle->Width, rectangle->Y + rectangle->Height);
    glVertex2i(rectangle->X, rectangle->Y + rectangle->Height);
    glEnd();

    if(1)
    {
        // PASS 2: Overlay the stippled diamond grid pattern lines
        glEnable(GL_POLYGON_STIPPLE);
        glPolygonStipple(DiamondHatchPattern);

        // Set line color (a slightly lighter pinkish-white accent line)
        //glColor4f(1.0f, 1.0f, 1.0f, 0.15f); // Transparent white matches the image feel smoothly

        float factor = 0.8;

        glBegin(GL_QUADS);
        glColor3f(bottom.Red * factor, bottom.Green * factor, bottom.Blue * factor);
        glVertex2i(rectangle->X, rectangle->Y);
        glVertex2i(rectangle->X + rectangle->Width, rectangle->Y);
        glColor3f(top.Red * factor, top.Green * factor, top.Blue * factor);
        glVertex2i(rectangle->X + rectangle->Width, rectangle->Y + rectangle->Height);
        glVertex2i(rectangle->X, rectangle->Y + rectangle->Height);
        glEnd();

        // Reset state so subsequent UI windows render normally
        glDisable(GL_POLYGON_STIPPLE);
    }
}

void PXAPI PXWindowDrawRectangleGL2
(
    const PXF32 x,
    const PXF32 y,
    const PXF32 w,
    const PXF32 h,
    const PXF32 r,
    const PXF32 g,
    const PXF32 b
)
{
    PXRectangleXYWHI32 rect = { x, y, w, h };

    PXColorRGBA_F32 PXColorRGBA_U8 = { r, g, b };

    PXWindowDrawRectangleGL(&rect, &PXColorRGBA_U8);
}

void PXAPI PXWindowDrawRectangleWireGL2(const PXF32 x, const PXF32 y, const PXF32 w, const PXF32 h, const PXF32 r, const PXF32 g, const PXF32 b, const PXF32 thickness)
{
    glColor3f(r, g, b);
    glLineWidth(thickness);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    glLineWidth(1.0f);

    //const PXRectangleXYWHI32 pxRectangle = {x, y, w, h};

    //PXWindowDrawRectangleWireGL(&pxRectangle, r, g, b, thickness);
}

void PXAPI PXWindowDrawRectangleWireGL(PXRectangleXYWHI32 PXREF rectangle, PXF32 r, PXF32 g, PXF32 b, PXF32 thickness)
{
    glColor3f(r, g, b);
    glLineWidth(thickness);
    glBegin(GL_LINE_LOOP);
    glVertex2i(rectangle->X, rectangle->Y);
    glVertex2i(rectangle->X + rectangle->Width, rectangle->Y);
    glVertex2i(rectangle->X + rectangle->Width, rectangle->Y + rectangle->Height);
    glVertex2i(rectangle->X, rectangle->Y + rectangle->Height);
    glEnd();
    glLineWidth(1.0f);
}



PXResult PXAPI PXWindowDrawPoint(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y)
{
#if OSUnix
    const int resultID = 0;//XDrawPoint(Display *display, Drawable d, GC gc, int x, int y);
    const PXResult result = PXErrorCurrent(resultID);
    return result;
#elif OSWindows
    return PXResultRefusedNotImplemented;
#else
    return PXResultRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXWindowDrawPoints(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
#if OSUnix
    const int resultID = 0;//XDrawPoints(Display *display, Drawable d, GC gc, XPoint *points, int npoints, int mode);
    const PXResult result = PXErrorCurrent(resultID);
    return result;
#elif OSWindows
    return PXResultRefusedNotImplemented;
#else
    return PXResultRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXWindowDrawLine(PXWindow PXREF pxWindow, const PXI32S x1, const PXI32S y1, const PXI32S x2, const PXI32S y2)
{
#if OSUnix
    const resultID = 0;//XDrawLine(Display *display, Drawable d, GC gc, x1, y1, x2, y2);
    const PXResult result = PXErrorCurrent(resultID);
    return result;
#elif OSWindows

    POINT point;
    HDC deviceContextHandle = PXWindowDCGet(pxWindow);

    // Move to a start point, no drawing happens here
    const BOOL successMove = MoveToEx(deviceContextHandle, x1, y1, &point);

    // use the previous step to draw a line from there to here
    const BOOL successDraw = LineTo(deviceContextHandle, x2, y2);

    return PXResultOK;

#else
    return PXNotSupport;
#endif
}

PXResult PXAPI PXWindowDrawLines(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
    PXResult pxResult = PXResultInvalid;

    if(!pxWindow)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    PXEngineUI* pxUI = PXEngineUIDisplayInstance();
    Display* xDisplay = PXEngineUIDisplayGet(pxUI);
    GC xGraphicContext = pxWindow->PixelBuffer.Data;

    if(!xDisplay)
    {
        return PXResultRefusedHandleNull;
    }

    if(!xGraphicContext)
    {
        return PXResultRefusedHandleNull;
    }

    XPoint points;
    int npoints = 0;
    int mode = 0;

    const resultID = XDrawLines
    (
        xDisplay,
        pxWindow->Handle.Data,
        xGraphicContext,
        &points,
        npoints,
        mode
    );
    pxResult = PXErrorCurrent(resultID);

#elif OSWindows
    HDC deviceContextHandle = PXWindowDCGet(pxWindow);

    const BOOL success = PolylineTo
    (
        deviceContextHandle,
        0,
        0
    );

    pxResult = PXResultOK;
#else
    pxResult = PXNotSupport;
#endif

    return pxResult;
}



PXResult PXAPI PXWindowDrawHelloWorldTriangle(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
    glDisable(GL_CULL_FACE);

    glBegin(GL_TRIANGLES);

#if 1
    glColor3f(1.0f, 0.0f, 0.0f); // Rot
    glVertex2f(0.0f, 1.0f);      // Oben

    glColor3f(0.0f, 1.0f, 0.0f); // Grün
    glVertex2f(-1.0f, -1.0f);    // Unten links

    glColor3f(0.0f, 0.0f, 1.0f); // Blau
    glVertex2f(1.0f, -1.0f);     // Unten rechts
#else
    PXF32 amount = x + (width / 2);

    glColor3f(1.0f, 0.0f, 0.0f); // Rot
    glVertex2f(x, y); // Rot

    glColor3f(0.0f, 1.0f, 0.0f); // Grün
    glVertex2f(x + width, y); // Grün

    glColor3f(0.0f, 0.0f, 1.0f); // Blau
    glVertex2f(amount, y + height); // Blau
#endif

    glEnd();

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawRect(PXWindow PXREF pxWindow, const PXI32S x, const PXI32S y, const PXI32S width, const PXI32S height)
{
    glBegin(GL_QUADS);
    glColor3ub(72, 200, 177);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glColor3ub(85, 155, 215);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();

    return PXResultOK;
}

PXResult PXAPI PXWindowDrawRect2(PXWindow PXREF pxWindow, const PXRectangleXYWHI32 PXREF pxRectangle)
{
    glBegin(GL_QUADS);
    glVertex2i(pxRectangle->X, pxRectangle->Y);
    glVertex2i(pxRectangle->X + pxRectangle->Width, pxRectangle->Y);
    glVertex2i(pxRectangle->X + pxRectangle->Width, pxRectangle->Y + pxRectangle->Height);
    glVertex2i(pxRectangle->X, pxRectangle->Y + pxRectangle->Height);
    glEnd();
}

// Core scaling math functions
int PercentToPixels(float percent, int parent_size) {
    return (int)((percent / 100.0f) * parent_size);
}

int ApplyDPIScale(int virtual_pixels, float dpi_scale_factor) 
{
    return (int)((float)virtual_pixels * dpi_scale_factor);
}


PXResult PXAPI PXEngineUIDrawIcon_GDI(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle, PXIcon PXREF pxIcon)
{
    const HDC deviceContextHandle = PXWindowDCGet(pxWindow);
    const BOOL resultID = DrawIconEx
    (
        deviceContextHandle,
        pxRectangle->X,
        pxRectangle->Y,
        pxIcon->IconHandle,
        pxRectangle->Width,
        pxRectangle->Height,
        0,
        0,
        DI_NORMAL
    );
    PXResult pxResult = PXErrorCurrent(resultID);

    return pxResult;
}

PXResult PXAPI PXEngineUIDrawIcon_GL(PXWindow PXREF pxWindow, PXRectangleXYWHI32 PXREF pxRectangle, PXIcon PXREF pxIcon)
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, pxIcon->Texture->OpenGLID);

    glColor3f(1.0f, 1.0f, 1.0f); // full brightness

    glBegin(GL_QUADS);

    // Bottom-left
    glTexCoord2f(0.0f, 1.0f);
    glVertex2i(pxRectangle->X, pxRectangle->Y);

    // Bottom-right
    glTexCoord2f(1.0f, 1.0f);
    glVertex2i(pxRectangle->X + pxRectangle->Width, pxRectangle->Y);

    // Top-right
    glTexCoord2f(1.0f, 0.0f);
    glVertex2i(pxRectangle->X + pxRectangle->Width, pxRectangle->Y + pxRectangle->Height);

    // Top-left
    glTexCoord2f(0.0f, 0.0f);
    glVertex2i(pxRectangle->X, pxRectangle->Y + pxRectangle->Height);

    glEnd();

    glDisable(GL_TEXTURE_2D);

    return PXResultOK;
}






















PXBool UI_PointInRect(int px, int py, PXRectangleXYWHF32 PXREF rect)
{
    return (px >= rect->X && px <= (rect->X + rect->Width) &&
            py >= rect->Y && py <= (rect->Y + rect->Height));
}

void UI_FindPotentialDropTarget(PXEngineUI PXREF pxEngineUI, UIWindow* root, int mx, int my, UIState* state)
{
    if(!root || !root->is_enabled || root == state->active_dragging_window)
        return;

    if(UI_PointInRect(mx, my, &root->computed_rect))
    {
        if(root->accepts_children)
        {
            state->potential_drop_target = root;
        }
        for(int i = 0; i < root->child_count; i++)
        {
            UI_FindPotentialDropTarget(pxEngineUI, root->children[i], mx, my, state);
        }
    }
}

void UI_CheckSplitterHover(PXEngineUI PXREF pxEngineUI, UIWindow* win, int mx, int my, UIState* state)
{
    if(!win || !win->is_enabled || win->is_popped_out || win->child_count < 2)
        return;

    for(int i = 0; i < win->child_count - 1; i++)
    {
        UIWindow* c1 = win->children[i];
        if(!c1->is_enabled || c1->is_popped_out)
            continue;

        if(win->pack_method == PACK_COLUMN)
        {
            float edge_x = c1->computed_rect.X + c1->computed_rect.Width;
            if(mx >= edge_x - 4 && mx <= edge_x + 4 && my >= win->content_rect.Y && my <= win->content_rect.Y + win->content_rect.Height) {
                SetCursor(LoadCursor(NULL, IDC_SIZEWE));
                if(state->mouse_pressed && !state->is_resizing_splitter)
                {
                    state->is_resizing_splitter = PXTrue;
                    state->resizing_splitter_parent = win;
                    state->resizing_splitter_index = i;
                    state->drag_start_x = mx;
                }
                return;
            }
        }
        else if(win->pack_method == PACK_ROW) {
            float edge_y = c1->computed_rect.Y + c1->computed_rect.Height;
            if(my >= edge_y - 4 && my <= edge_y + 4 && mx >= win->content_rect.X && mx <= win->content_rect.X + win->content_rect.Width) {
                SetCursor(LoadCursor(NULL, IDC_SIZENS));
                if(state->mouse_pressed && !state->is_resizing_splitter) {
                    state->is_resizing_splitter = PXTrue;
                    state->resizing_splitter_parent = win;
                    state->resizing_splitter_index = i;
                    state->drag_start_y = my;
                }
                return;
            }
        }
    }

    for(int i = 0; i < win->child_count; i++)
    {
        UI_CheckSplitterHover(pxEngineUI, win->children[i], mx, my, state);
    }
}

void UI_RemoveChild(UIWindow* parent, UIWindow* child)
{
    if(!parent || !child)
        return;

    int index = -1;

    for(int i = 0; i < parent->child_count; i++)
    {
        if(parent->children[i] == child)
        {
            index = i;
            break;
        }
    }

    if(index != -1)
    {
        for(int i = index; i < parent->child_count - 1; i++)
        {
            parent->children[i] = parent->children[i + 1];
        }
        parent->child_count--;
        child->parent = NULL;
    }
}

void UI_AddChild(UIWindow* parent, UIWindow* child)
{
    if(!parent || !child || !parent->accepts_children)
        return;

    for(int i = 0; i < parent->child_count; i++)
    {
        if(parent->children[i] == child)
            return;
    }
    parent->children[parent->child_count] = child;
    parent->child_count++;
    child->parent = parent;
    child->is_popped_out = PXFalse;
    child->is_dragging = PXFalse;
}






void UI_InitState(PXEngineUI PXREF pxEngineUI, UIState* state)
{
    state->mouse_x = 0;
    state->mouse_y = 0;
    state->drag_start_x = 0;
    state->drag_start_y = 0;
    state->mouse_pressed = PXFalse;
    state->hovered_window = NULL;
    state->focused_window = NULL;
    state->active_dragging_window = NULL;
    state->potential_drop_target = NULL;
    state->resizing_splitter_parent = NULL;
    state->resizing_splitter_index = -1;
    state->is_resizing_splitter = PXFalse;
    state->selected_count = 0;
    state->debug_show_hitboxes = PXTrue;
    state->global_zoom = 1.0f;
    state->menu_active = PXFalse;
    state->menu_target = NULL;
}

void UI_ComputeLayout(PXEngineUI PXREF pxEngineUI, UIWindow* win, float x, float y, float w, float h, UIState* state)
{
    if(!win || !win->is_enabled)
        return;

    float zoom = state->global_zoom;

    if(win->is_popped_out && !win->parent) {
        win->computed_rect.X = win->floating_x;
        win->computed_rect.Y = win->floating_y;
    }
    else {
        win->computed_rect.X = x;
        win->computed_rect.Y = y;
    }

    win->computed_rect.Width = w;
    win->computed_rect.Height = h;

    float title_height = (win->type == WIDGET_CONTAINER) ? (25.0f * zoom) : 0.0f;
    win->content_rect.X = win->computed_rect.X;
    win->content_rect.Y = win->computed_rect.Y + title_height;
    win->content_rect.Width = w;
    win->content_rect.Height = (h - title_height > 0) ? (h - title_height) : 0;

    int active_children = 0;
    for(int i = 0; i < win->child_count; i++) {
        if(win->children[i]->is_enabled && !win->children[i]->is_popped_out) {
            active_children++;
        }
    }
    if(active_children <= 0) return;

    float allocated_w = 0, allocated_h = 0;
    int auto_count_w = 0, auto_count_h = 0;

    for(int i = 0; i < win->child_count; i++) {
        UIWindow* child = win->children[i];
        if(!child->is_enabled || child->is_popped_out) continue;
        child->parent = win;

        if(child->size_rule_width == SIZE_FIXED) {
            allocated_w += child->requested_width * zoom;
        }
        else if(child->size_rule_width == SIZE_PERCENT) {
            allocated_w += win->content_rect.Width * child->requested_width;
        }
        else {
            auto_count_w++;
        }

        if(child->size_rule_height == SIZE_FIXED) {
            allocated_h += child->requested_height * zoom;
        }
        else if(child->size_rule_height == SIZE_PERCENT) {
            allocated_h += win->content_rect.Height * child->requested_height;
        }
        else {
            auto_count_h++;
        }
    }

    float remaining_w = win->content_rect.Width - allocated_w;
    float remaining_h = win->content_rect.Height - allocated_h;

    if(remaining_w < 0)
        remaining_w = 0;
    if(remaining_h < 0)
        remaining_h = 0;

    float auto_w_slice = (auto_count_w > 0) ? (remaining_w / auto_count_w) : 0;
    float auto_h_slice = (auto_count_h > 0) ? (remaining_h / auto_count_h) : 0;

    float current_cx = win->content_rect.X;
    float current_cy = win->content_rect.Y;

    if(win->pack_method == PACK_COLUMN) {
        for(int i = 0; i < win->child_count; i++) {
            UIWindow* child = win->children[i];
            if(!child->is_enabled || child->is_popped_out) continue;
            float cw = (child->size_rule_width == SIZE_FIXED) ? (child->requested_width * zoom) :
                (child->size_rule_width == SIZE_PERCENT) ? (win->content_rect.Width * child->requested_width) : auto_w_slice;
            float ch = win->content_rect.Height;

            UI_ComputeLayout(pxEngineUI, child, current_cx, current_cy, cw, ch, state);
            current_cx += cw;
        }
    }
    else if(win->pack_method == PACK_ROW) {
        for(int i = 0; i < win->child_count; i++) {
            UIWindow* child = win->children[i];
            if(!child->is_enabled || child->is_popped_out) continue;
            float cw = win->content_rect.Width;
            float ch = (child->size_rule_height == SIZE_FIXED) ? (child->requested_height * zoom) :
                (child->size_rule_height == SIZE_PERCENT) ? (win->content_rect.Height * child->requested_height) : auto_h_slice;

            UI_ComputeLayout(pxEngineUI, child, current_cx, current_cy, cw, ch, state);
            current_cy += ch;
        }
    }
}

void UI_HandleMouseMove(PXEngineUI PXREF pxEngineUI, UIWindow PXREF g_root_workspace, UIWindow* root, int mx, int my, UIState* state)
{
    if(!root || !root->is_enabled)
        return;

    state->mouse_x = mx;
    state->mouse_y = my;

    if(state->mouse_pressed && state->is_resizing_splitter && state->resizing_splitter_parent) {
        UIWindow* parent = state->resizing_splitter_parent;
        UIWindow* left_child = parent->children[state->resizing_splitter_index];

        if(parent->pack_method == PACK_COLUMN) {
            int delta_x = mx - state->drag_start_x;
            if(left_child->size_rule_width == SIZE_PERCENT) {
                float fraction_delta = (float)delta_x / parent->content_rect.Width;
                left_child->requested_width += fraction_delta;

                if(left_child->requested_width < 0.05f)
                    left_child->requested_width = 0.05f;
                if(left_child->requested_width > 0.95f)
                    left_child->requested_width = 0.95f;
            }
            else {
                left_child->requested_width += delta_x;
                if(left_child->requested_width < 20.0f) left_child->requested_width = 20.0f;
            }
            state->drag_start_x = mx;
        }
        else if(parent->pack_method == PACK_ROW) {
            int delta_y = my - state->drag_start_y;
            if(left_child->size_rule_height == SIZE_PERCENT) {
                float fraction_delta = (float)delta_y / parent->content_rect.Height;
                left_child->requested_height += fraction_delta;
                if(left_child->requested_height < 0.05f) left_child->requested_height = 0.05f;
                if(left_child->requested_height > 0.95f) left_child->requested_height = 0.95f;
            }
            else {
                left_child->requested_height += delta_y;
                if(left_child->requested_height < 20.0f) left_child->requested_height = 20.0f;
            }
            state->drag_start_y = my;
        }
        return;
    }

    if(state->mouse_pressed && state->active_dragging_window) {
        UIWindow* active = state->active_dragging_window;

        if(!active->is_popped_out) {
            int dx = mx - state->drag_start_x;
            int dy = my - state->drag_start_y;
            if(sqrt((double)(dx * dx + dy * dy)) > 15.0) {
                active->is_popped_out = PXTrue;
                active->floating_x = active->computed_rect.X;
                active->floating_y = active->computed_rect.Y;
                if(active->parent) {
                    UI_RemoveChild(active->parent, active);
                }
            }
        }
        else {
            active->floating_x += (float)(mx - state->drag_start_x);
            active->floating_y += (float)(my - state->drag_start_y);
            state->drag_start_x = mx;
            state->drag_start_y = my;
        }

        state->potential_drop_target = NULL;
        UI_FindPotentialDropTarget(pxEngineUI, &g_root_workspace, mx, my, state);
        return;
    }

    UI_CheckSplitterHover(pxEngineUI, &g_root_workspace, mx, my, state);

    root->is_hovered = UI_PointInRect(mx, my, &root->computed_rect);

    for(int i = 0; i < root->child_count; i++)
    {
        UI_HandleMouseMove(pxEngineUI, g_root_workspace, root->children[i], mx, my, state);
    }
}

void UI_HandleMouseClick(PXEngineUI PXREF pxEngineUI, UIWindow PXREF g_root_workspace, UIWindow* root, int mx, int my, PXBool is_down, PXBool ctrl_pressed, UIState* state)
{
    //PXWindow PXREF pxWindow = pxWindowEvent->WindowSender;
    //UIWindow PXREF g_root_workspace = PXWindowUIDataGet(pxWindow);
    //UIState PXREF g_ui_state = PXWindowUIStateGet(pxWindow);

    if(!root || !root->is_enabled) 
        return;

    if(is_down) {
        state->mouse_pressed = PXTrue;

        UI_CheckSplitterHover(pxEngineUI, g_root_workspace, mx, my, state);
        if(state->is_resizing_splitter)
            return;

        PXRectangleXYWHF32 title_bar_rect = root->computed_rect;
        title_bar_rect.Height = (root->type == WIDGET_CONTAINER) ? (25.0f * state->global_zoom) : 0.0f;

        if(title_bar_rect.Height > 0 && UI_PointInRect(mx, my, &title_bar_rect) && root != &g_root_workspace) {
            state->active_dragging_window = root;
            state->drag_start_x = mx;
            state->drag_start_y = my;
            root->is_dragging = PXTrue;
            return;
        }

        if(UI_PointInRect(mx, my, &root->computed_rect)) {
            if(state->focused_window) state->focused_window->is_focused = PXFalse;
            root->is_focused = PXTrue;
            state->focused_window = root;
        }
    }
    else {
        state->mouse_pressed = PXFalse;
        state->is_resizing_splitter = PXFalse;
        state->resizing_splitter_parent = NULL;
        state->resizing_splitter_index = -1;

        if(state->active_dragging_window) {
            UIWindow* active = state->active_dragging_window;
            active->is_dragging = PXFalse;

            if(state->potential_drop_target) {
                UI_AddChild(state->potential_drop_target, active);
            }
            else if(active->is_popped_out) {
                active->parent = NULL;
            }
        }
        state->active_dragging_window = NULL;
        state->potential_drop_target = NULL;
    }

    for(int i = 0; i < root->child_count; i++)
    {
        UI_HandleMouseClick(pxEngineUI, g_root_workspace, root->children[i], mx, my, is_down, ctrl_pressed, state);
    }
}

void UI_RenderWindow(PXEngineUI PXREF pxEngineUI, UIWindow* win, UIState* state)
{
    if(!win || !win->is_enabled)
        return;

    float x = win->computed_rect.X;
    float y = win->computed_rect.Y;
    float w = win->computed_rect.Width;
    float h = win->computed_rect.Height;

    float base_r = 0.18f, base_g = 0.18f, base_b = 0.19f;

    if(win->is_focused)
    {
        base_r = 0.22f;
        base_g = 0.24f;
        base_b = 0.28f;
    }

    if(win->type == WIDGET_BUTTON)
    {
        base_r = win->is_hovered ? 0.35f : 0.28f;
        base_g = win->is_hovered ? 0.45f : 0.38f;
        base_b = win->is_hovered ? 0.70f : 0.60f;
    }
    else if(win->type == WIDGET_INPUT_FIELD)
    {
        base_r = 0.12f; base_g = 0.12f; base_b = 0.13f;
    }

    PXWindowDrawRectangleGL2(x, y, w, h, base_r, base_g, base_b);

    if(win->type == WIDGET_CONTAINER)
    {
        float title_h = 25.0f * state->global_zoom;
        float bar_r = win->is_focused ? 0.3f : 0.23f;
        float bar_g = win->is_focused ? 0.35f : 0.23f;
        PXWindowDrawRectangleGL2(x, y, w, title_h, bar_r, bar_g, bar_r + 0.05f);
    }

    if(state->debug_show_hitboxes)
    {
        if(win->is_hovered)
            PXWindowDrawRectangleWireGL2(x, y, w, h, 0.0f, 1.0f, 0.0f, 3.0f);
        else
            PXWindowDrawRectangleWireGL2(x, y, w, h, 0.3f, 0.3f, 0.3f, 1.5f);
    }

    for(int i = 0; i < win->child_count; i++) {
        if(!win->children[i]->is_popped_out) {
            UI_RenderWindow(pxEngineUI, win->children[i], state);
        }
    }
}