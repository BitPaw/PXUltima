#include "PXUI.h"

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
#include <PX/Engine/ECS/PXECS.h>
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





typedef struct PXUI_
{
    PXECSInfo Info;

    PXMonitor* MonitorList;
    PXSize MonitorCount;

    PXUITheme ThemeDefault;
    PXUITheme* ThemeSelected;

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
}
PXUI;

const char PXUIText[] = "UI-PX";
const PXI8U PXUITextLength = sizeof(PXUIText);
const PXECSRegisterInfoStatic PXUIInfoStatic =
{
    {sizeof(PXUIText), sizeof(PXUIText), PXUIText, TextFormatASCII},
    sizeof(PXUI),
    __alignof(PXUI),
    PXECSTypeSystem,
    PXUICreate,
    PXUIRelease,
    PXNull
};
PXECSRegisterInfoDynamic PXUIInfoDynamic;


PXUI* _GlobalUIREF = 0; // Bad solution to fix X11 garbage.






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





PXResult PXAPI PXUILoadFromFile(PXUI PXREF pxUI, PXText PXREF pxFilePath)
{
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
        PXUIText,
        "Load",
        "Path:%s",
        pxFilePath->A
    );
#endif

    PXDOM* pxDOM = 0;
    PXDOMCreate(&pxDOM);

    // Load expected UI file
    {
        PXECSCreateInfo pxECSCreateInfo;
        PXClear(PXECSCreateInfo, &pxECSCreateInfo);
        pxECSCreateInfo.FilePath = *pxFilePath;

        PXECSLoad(pxDOM, &pxECSCreateInfo);
    }



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
        "Load",
        "DONE"
    );
#endif


    char buffer[128];

    PXText pxText;
    PXTextFromAdressA(&pxText, buffer, 0, sizeof(buffer));



    PXSize amount = PXDOMKeyAmount(pxDOM);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXUIText,
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
            PXUIText,
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
        PXUIText,
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
        PXTextFromAdressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

        PXTextClear(&pxText);
        PXDOMPropertyToString(pxDOM, pxDOMClassValue, &pxText, &pxTextValue, PXTrue);


#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUIText,
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
        PXUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Classes]", amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMClass PXREF pxDOMClass = PXDOMClassGet(pxDOM, i);

        PXTextClear(&pxText);
        PXDOMClassToString(pxDOM, pxDOMClass, &pxText, PXTrue);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUIText,
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
        PXUIText,
        "Load",
        "%-18s : %i",
        "[DOM-Object]",amount
    );
#endif

    for(PXSize i = 0; i < amount; i++)
    {
        PXDOMObject PXREF pxDOMObject = PXDOMObjectGet(pxDOM, i);

        PXTextClear(&pxText);
        PXDOMObjectToString(pxDOM, pxDOMObject, &pxText, PXTrue);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXUIText,
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

        for(size_t i = 0; i < pxDOMObject->PropertyIndexCount; i++)
        {
            PXDOMIndex propertyStart = pxDOMObject->PropertyIndexStart + i;
            PXDOMProperty PXREF pxDOMClassValue = PXDOMPropertyGet(pxDOM, propertyStart);

            char bufferValue[256];
            PXText pxTextValue;
            PXTextFromAdressA(&pxTextValue, bufferValue, 0, sizeof(bufferValue));

            PXTextClear(&pxText);
            PXDOMPropertyToString(pxDOM, pxDOMClassValue, &pxText, &pxTextValue, PXTrue);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXUIText,
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


                while(PXDOMCursorPropertyUpdate(pxDOM, &pxDOMCursor))
                {
                    match = PXDOMCursorPropertyCompare(pxDOM, &pxDOMCursor, PXWindowPropertyMatchesLV2, PXWindowPropertyLV2Amount);

                    PXCompilerSymbolEntry* symbol = &pxDOMCursor.prop->Symbol;

                    switch(match)
                    {
                        case UI_Window_Name:
                        {
                            PXTextFromAdressA
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
                            PXTextFromAdressA
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
                                PXUIText,
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
                            PXColorRGBI8 PXREF pxXColorRGBI8 = &pxWindowCreateInfo.BackGroundColor;

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
        PXUIText,
        "Load",
        "DONE!"
    );
#endif


    return PXResultOK;
}

#if OSWindows

typedef struct PXWindowsMonitorEnumInfo_
{
    PXUI* UI;
    PXSize CountCurrent;
    PXBool DoStore;
}
PXWindowsMonitorEnumInfo;

BOOL CALLBACK PXUIMonitorEnumCallBack(HMONITOR monitorHandle, HDC hdc, LPRECT rect, LPARAM data)
{
    PXWindowsMonitorEnumInfo* pxEnumInfo = (PXWindowsMonitorEnumInfo*)data;
    PXUI PXREF pxUI = pxEnumInfo->UI;

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

PXResult PXAPI PXUICreate(PXUI** pxUIREF, PXUIInfo PXREF pxUIInfo)
{
    PXUI* pxUI = PXNull;

    if(!(pxUIREF && pxUIInfo))
    {
        return PXResultRefusedParameterNull;
    }

    pxUIInfo->Info.Static = &PXUIInfoStatic;
    pxUIInfo->Info.Dynamic = &PXUIInfoDynamic;
    PXResult pxResult = PXECSCreate(pxUIREF, pxUIInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxUI = *pxUIREF;
    _GlobalUIREF = pxUI;


    // Init
    PXUICreateContext(pxUI);





    pxUI->ThemeSelected = &pxUI->ThemeDefault;
      

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

    PXUITheme PXREF pxTheme = &pxUI->ThemeDefault;

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
        PXTextFromAdressA(&pxFontCreateInfo.RegisteredName, PXFontNamePri, sizeof(PXFontNamePri), sizeof(PXFontNamePri));
        PXFontCreate(&pxTheme->FontTitle, &pxFontCreateInfo);

        PXClear(PXFontCreateInfo, &pxFontCreateInfo);
        pxFontCreateInfo.Info.Name.A = "FontContent";
        pxFontCreateInfo.Info.Type = PXResourceTypeFont;
        PXTextFromAdressA(&pxFontCreateInfo.RegisteredName, PXFontNameSec, sizeof(PXFontNameSec), sizeof(PXFontNameSec));
        PXFontCreate(&pxTheme->FontContent, &pxFontCreateInfo);
    }





    // Load UI from path
    PXUILoadFromFile(pxUI, &pxUIInfo->Path);

    return PXResultOK;
}

PXResult PXAPI PXUIRelease(PXUI PXREF pxUI)
{
    return PXResultOK;
}

PXResult PXAPI PXUIWindowResolve(PXUI PXREF pxUI, PXECSInfo** pxECSInfo, PXText PXREF pxTextName)
{
    *pxECSInfo = PXECSEntryGetViaName(pxTextName);

    return PXResultOK;
}

PXResult PXAPI PXUICreateContext(PXUI PXREF pxUI)
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
            PXUIText,
            "Display-Open",
            "<%s> X11 failed to open display server!"
        );
#endif

        return PXActionFailedInitialization;
    }

    // Attach error callback
    XSetErrorHandler(PXUICallBackHandler);

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
        PXUIText,
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

    EnumDisplayMonitors(NULL, NULL, PXUIMonitorEnumCallBack, (LPARAM)&pxWindowsMonitorEnumInfo);

    pxUI->MonitorList = PXMemoryHeapCallocT(PXMonitor, pxUI->MonitorCount);

    pxWindowsMonitorEnumInfo.DoStore = 1;
    EnumDisplayMonitors(NULL, NULL, PXUIMonitorEnumCallBack, (LPARAM)&pxWindowsMonitorEnumInfo);
#endif
}


#if OSUnix
PXResult PXAPI PXUIErrorFromX(const int xErrorCode)
{
    switch(xErrorCode)
    {
        case BadRequest:        return PXResultRefusedRequestInvalid;
        case BadValue:          return PXResultRefusedValueInvalid;
        case BadWindow:         return PXResultRefusedWindowInvalid;
        case BadPixmap:         return PXResultRefusedPixmapInvalid;
        case BadAtom:           return PXResultRefusedAtomInvalid;
        case BadCursor:         return PXResultRefusedCursorInvalid;
        case BadFont:           return PXResultRefusedFontInvalid;
        case BadMatch:          return PXResultRefusedIncompatible;
        case BadDrawable:       return PXResultRefusedDrawableInvalid;
        case BadAccess:         return PXResultRefusedAccessDenied;
        case BadAlloc:          return PXActionFailedMemoryAllocation;
        case BadColor:          return PXResultRefusedColorInvalid;
        case BadGC:             return PXResultRefusedGraphicsContextInvalid;
        case BadIDChoice:       return PXResultRefusedResourceIDInvalid;
        case BadName:           return PXResultRefusedNameInvalid;
        case BadLength:         return PXResultRefusedLengthInvalid;
        case BadImplementation: return PXResultRefusedOperationUnsupported;
        default:                return PXResultUnknown;
    }
}

int PXAPI PXUICallBackHandler(Display PXREF displayHandle, XErrorEvent PXREF xErrorEvent)
{
    // convert err->error_code to your own enum
    PXResult pxResult = PXUIErrorFromX(xErrorEvent->error_code);


#if PXLogEnable
    char errorBuffer[256];

    XGetErrorText(displayHandle, xErrorEvent->error_code, errorBuffer, 256);

    PXLogPrint
    (
        PXLoggingError,
        PXUIText,
        "X11",
        "(%i) %s",
        xErrorEvent->error_code,
        errorBuffer
    );
#endif

    return 0;
}
PXUI* PXAPI PXUIDisplayInstance(void)
{
    PXAssert(_GlobalUIREF, "You forgot to init this");

    return _GlobalUIREF;
}
Display* PXAPI PXUIDisplayGet(PXUI PXREF pxUI)
{
    if(!pxUI)
    {
        return PXNull;
    }

    PXAssert(pxUI->DisplayHandle, "You forgot to init this");

    return pxUI->DisplayHandle;
}
int PXAPI PXUIScreenIDDefaultGet(PXUI PXREF pxUI)
{
    if(!pxUI)
    {
        return 0;
    }

    Display* xDisplay = PXUIDisplayGet(pxUI);

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

void PXAPI PXUIScreenSizeGet(PXRectangleXYWHI32 PXREF pxRectangleXYWH)
{
    if(!pxRectangleXYWH)
    {
        return;
    }

#if OSUnix

    const PXUI PXREF pxUI = PXUIDisplayInstance();
    const Display PXREF xDisplayHandle = PXUIDisplayGet(pxUI);
    const int screenIDHandle = PXUIScreenIDDefaultGet(xDisplayHandle);

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
        PXUIText,
        "Size-Get",
        "Desktop : X:%4i Y:%4i H:%4i W:%4i",
        pxRectangleXYWH->X,
        pxRectangleXYWH->Y,
        pxRectangleXYWH->Width,
        pxRectangleXYWH->Height
    );
#endif
}





PXResult PXAPI PXUICursorPosition(PXVector2I32S PXREF position, const PXBool isWrite)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
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
    return PXActionRefusedTypeNotSupported;
#endif
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
