#include "PXUI.h"

#define WindowsEnableModernColorSceme 1

#if OSUnix
#elif OSWindows

#include <commctrl.h>
#include <commdlg.h>
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

#include <OS/Memory/PXMemory.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>
#include <OS/Window/PXWindow.h>
#include <OS/Console/PXConsole.h>
#include <Engine/PXEngine.h>
#include <stdio.h>
#include <WindowsX.h>

PXActionResult PXAPI PXUIElementCreateOSStyle(PXUIElement* const pxUIElement, struct PXUIElementCreateData_* const pxUIElementCreateData)
{
    PXInt32U styleFlags = 0;
    wchar_t* className = PXNull;

    if(!pxUIElement)
    {
        return PXActionRefusedArgumentNull;
    }

    switch(pxUIElement->Type)
    {
        case PXUIElementTypePanel:
        {
            className = WC_STATICW;
            styleFlags = WS_VISIBLE | WS_CHILD;
            break;
        }
        case PXUIElementTypeText:
        {
            className = WC_STATICW;
            styleFlags = WS_VISIBLE | WS_CHILD | SS_LEFT | WS_BORDER;
            break;
        }
        case PXUIElementTypeButton:
        {// BS_USERBUTTON 
            className = WC_BUTTONW;
            styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD |  WS_BORDER; // BS_DEFPUSHBUTTON 
            break;
        }
        case PXUIElementTypeImage:
        {
           // className = WC_IMAGE;
            //styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeDropDown:
        {
            className = WC_COMBOBOX;
            styleFlags = CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE;
            break;
        }
        case PXUIElementTypeListBox:
        { 
            className = WC_LISTBOXW;
            styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeTextEdit:
        {
            className = WC_EDITW;
            styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeRichEdit:
        {
            className = L"RICHEDIT_CLASS";
            styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeScrollBar:
        {
            className = WC_SCROLLBARW;
            styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeTrackBar:
        {
            className = TRACKBAR_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS | WS_BORDER;
            break;
        }
        case PXUIElementTypeStatusBar:
        {
            className = STATUSCLASSNAMEW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeUpDown:
        {
            className = UPDOWN_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeProgressBar:
        {
            className = PROGRESS_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeHotKey:
        {
            className = HOTKEY_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeCalender:
        {
            className = MONTHCAL_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeToolTip:
        {
            className = TOOLTIPS_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeAnimate:
        {
            className = ANIMATE_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeDatePicker:
        {
            className = DATETIMEPICK_CLASSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeGroupBox:
        {
            className = WC_BUTTONW;
            styleFlags = WS_VISIBLE | WS_CHILD | BS_GROUPBOX | WS_BORDER;
            break;
        }
        case PXUIElementTypeRadioButton:
        {
            className = WC_BUTTONW;
            styleFlags = WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON | WS_BORDER;
            break;
        }
        case PXUIElementTypeGroupRadioButton:
        {
            className = WC_BUTTONW;
            styleFlags = WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_BORDER;
            break;
        }
        case PXUIElementTypeTreeView:
        {
            className = WC_TREEVIEWW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeIPInput:
        {
            className = WC_IPADDRESSW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeLink:
        {
            className = WC_LINK;
            styleFlags = WS_VISIBLE | WS_CHILD;
            break;
        }
        case PXUIElementTypeHeader:
        {
            className = WC_HEADERW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeFontSelector:
        {
            className = WC_NATIVEFONTCTLW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER; // NFS_USEFONTASSOC
            break;
        }
        case PXUIElementTypePageScroll:
        {
            className = WC_PAGESCROLLERW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeTabControll:
        {
            className = WC_TABCONTROLW;
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        case PXUIElementTypeToggle:
        {
            className = 00000000000000000000;
            styleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeColorPicker:
        {
            className = 00000000000000000000;
            styleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeSlider:
        {
            className = 00000000000000000000;
            styleFlags = 000000000000000000000000;
            break;
        }
        case PXUIElementTypeCheckBox:
        {
            className = WC_BUTTONW;
            styleFlags = WS_VISIBLE | WS_CHILD | BS_CHECKBOX | WS_BORDER;

            //CheckDlgButton(uiCheckBox->ID, 1, BST_CHECKED); // BST_UNCHECKED

            break;
        }
        case PXUIElementTypeComboBox:
        {
            className = WC_COMBOBOXEXW;
            styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
            break;
        }
        default:
            return PXActionRefusedArgumentInvalid;
    }



    DWORD dwExStyle = 0;
    HWND windowHandle = pxUIElementCreateData->WindowReference->ID;
    const HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(windowHandle, GWLP_HINSTANCE);


    PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
    PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);
    PXInt32S width = 0;
    PXInt32S height = 0;
    PXWindowSize(pxUIElementCreateData->WindowReference, PXNull, PXNull, &width, &height);

    pxUIElementPositionCalulcateInfo.WindowWidth = width;
    pxUIElementPositionCalulcateInfo.WindowHeight = height;

    PXUIElementPositionCalculcate(pxUIElement, &pxUIElementPositionCalulcateInfo);


    pxUIElement->ID = CreateWindowExW // Windows 2000, User32.dll, winuser.h
    (
        dwExStyle,
        className,
        PXNull, // Text content
        styleFlags,
        pxUIElementPositionCalulcateInfo.X,
        pxUIElementPositionCalulcateInfo.Y,
        pxUIElementPositionCalulcateInfo.Width,
        pxUIElementPositionCalulcateInfo.Height,
        windowHandle,
        PXNull,  // No menu.
        hInstance,
        NULL // Pointer not needed.
    );
    const PXBool success = -1 != pxUIElement->ID;


    
    char buffer[255];
    PXClearList(char, buffer, 255);

    PXSize* size = buffer;
    *size = 255;

    COLORREF colorAA = RGB(255, 0, 0);

    HDC xx = GetDC(pxUIElement->ID);

    HBRUSH brush = SendMessageA(windowHandle, WM_CTLCOLORBTN, xx, pxUIElement->ID); // RB_SETBKCOLOR
    SetTextColor(xx, RGB(255, 0, 0));
    SetBkColor(xx, RGB(0, 255, 0));



   // SetTextColor(xx, colorAA);


    //LRESULT ww = SendMessageA(pxUIElement->ID, EM_GETLINE, 0, buffer); // RB_SETBKCOLOR


  //  PXConsoleWriteA("\n\n%i     %i %s", brush, ww, buffer);


   // SendMessageA(pxUIElement->ID, RB_SETTEXTCOLOR, 0, &colorAA);
    




    Pager_SetBkColor(pxUIElement->ID, &colorAA);

    if(!success)
    {
        const PXActionResult pxActionResult = PXErrorCurrent();

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            "UI",
            "Element-Create",
            "Failed: Name:%ls, X:%4.2f, Y:%4.2f, Width:%4.2f, Height:%4.2f",
            className,
            pxUIElementPositionCalulcateInfo.X,
            pxUIElementPositionCalulcateInfo.Y,
            pxUIElementPositionCalulcateInfo.Width,
            pxUIElementPositionCalulcateInfo.Height
        );
#endif

        return pxActionResult;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "UI",
        "Element-Create",
        "Successful: Name:%ls, X:%4.2f, Y:%4.2f, Width:%4.2f, Height:%4.2f",
        className,
        pxUIElementPositionCalulcateInfo.X,
        pxUIElementPositionCalulcateInfo.Y,
        pxUIElementPositionCalulcateInfo.Width,
        pxUIElementPositionCalulcateInfo.Height
    );
#endif



    switch(pxUIElement->Type)
    {
        case PXUIElementTypePanel:
        {
            break;
        }
        case PXUIElementTypeText:
        {
            PXUIElementUpdateInfo pxUIElementUpdateInfo[2];
            pxUIElementUpdateInfo[0].UIElementReference = pxUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxUIElementCreateData->WindowReference;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;
            
            PXUIElementUpdateOSStyleV(pxUIElementUpdateInfo, 1);

            break;
        }
        case PXUIElementTypeButton:
        {
            PXUIElementUpdateInfo pxUIElementUpdateInfo[2];
            pxUIElementUpdateInfo[0].UIElementReference = pxUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxUIElementCreateData->WindowReference;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElementReference = pxUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxUIElementCreateData->WindowReference;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;

            PXTextCopyA(pxUIElement->Button.Text, PXTextLengthUnkown, pxUIElement->TextInfo.Content, 32);

            PXUIElementUpdateOSStyleV(pxUIElementUpdateInfo, 2);

            // SetWindowTextA(pxUIElement->ID, buttonInfo->Text); // ButtonTextSet()

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
        case PXUIElementTypeTextEdit:
        {
       
            break;
        }
        case PXUIElementTypeRichEdit:
        {
         
            break;
        }
        case PXUIElementTypeScrollBar:
        {
        
            break;
        }
        case PXUIElementTypeTrackBar:
        {
           
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
            PXUIElementUpdateInfo pxUIElementUpdateInfo[2];
            pxUIElementUpdateInfo[0].UIElementReference = pxUIElement;
            pxUIElementUpdateInfo[0].WindowReference = pxUIElementCreateData->WindowReference;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyProgressbarPercentage;
            pxUIElementUpdateInfo[1].UIElementReference = pxUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxUIElementCreateData->WindowReference;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyProgressbarBarColor;

            PXUIElementUpdateOSStyleV(pxUIElementUpdateInfo, 2);
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
        default:
            return PXActionRefusedArgumentInvalid;
    }













    return PXActionSuccessful;
}

PXActionResult PXAPI PXUIElementUpdateOSStyle(PXUIElementUpdateInfo* const pxUIElementUpdateInfo)
{
    PXUIElement* const pxUIElement = pxUIElementUpdateInfo->UIElementReference;

    switch(pxUIElementUpdateInfo->Property)
    {
        case PXUIElementPropertyTextContent:
        {
            PXUIElementTextInfo* const pxUIElementTextInfo = &pxUIElement->TextInfo;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "UI",
                "Text-Update",
                "[%i] %s",
                (int)pxUIElement->ID,
                pxUIElementTextInfo->Content
            );
#endif        

            SetWindowTextA(pxUIElement->ID, pxUIElementTextInfo->Content);

            break;
        }
        case PXUIElementPropertySize:
        {
            PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
            PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);


            PXInt32S width = 0;
            PXInt32S height = 0;
            PXWindowSize(pxUIElementUpdateInfo->WindowReference, PXNull, PXNull, &width, &height);

            pxUIElementPositionCalulcateInfo.WindowWidth = width;
            pxUIElementPositionCalulcateInfo.WindowHeight = height;

            PXUIElementPositionCalculcate(pxUIElement, &pxUIElementPositionCalulcateInfo);

            const PXBool success = MoveWindow
            (
                pxUIElement->ID,
                pxUIElementPositionCalulcateInfo.X,
                pxUIElementPositionCalulcateInfo.Y,
                pxUIElementPositionCalulcateInfo.Width,
                pxUIElementPositionCalulcateInfo.Height,
                PXTrue
            );

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "UI",
                "Element-Update",
                "Size X:%4.2f, Y:%4.2f, Width:%4.2f, Height:%4.2f",
                pxUIElementPositionCalulcateInfo.X,
                pxUIElementPositionCalulcateInfo.Y,
                pxUIElementPositionCalulcateInfo.Width,
                pxUIElementPositionCalulcateInfo.Height
            );
#endif

            break;
        }
        case PXUIElementPropertyProgressbarPercentage:
        {
            PXUIElementProgressBarInfo* const progressBar = &pxUIElement->ProgressBar;

            PXInt32U stepsConverted = progressBar->Percentage * 100;
            SendMessageA(pxUIElement->ID, PBM_SETPOS, stepsConverted, 0);

            break;
        }
        case PXUIElementPropertyProgressbarBarColor:
        {
            PXUIElementProgressBarInfo* const progressBar = &pxUIElement->ProgressBar;

            COLORREF color = RGB(progressBar->BarColor.Red, progressBar->BarColor.Green, progressBar->BarColor.Blue);
            SendMessageA(pxUIElement->ID, PBM_SETBARCOLOR, 0, color);

            break;
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXUIElementUpdateOSStyleV(PXUIElementUpdateInfo* const pxUIElementUpdateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXUIElementUpdateInfo* pxUIElementUpdateInfo = &pxUIElementUpdateInfoList[i];

        PXUIElementUpdateOSStyle(pxUIElementUpdateInfo);
    }

    return PXActionSuccessful;
}


/*

void PXOSUIElementTextGet(const UIElementID uiElementID, const wchar_t* buffer, const PXSize bufferSize, PXSize* bufferRead)
{
    const int textLength = GetWindowTextLengthW(uiElementID) + 1; // Windows 2000, User32.dll, winuser.h
    const int maximum = PXMathMinimum(textLength, bufferSize);
    const int lengthRead = GetWindowTextW(uiElementID, buffer, maximum); // Windows 2000, User32.dll, winuser.h

    *bufferRead = lengthRead;
}

void PXOSUIElementTextSet(const UIElementID uiElementID, const wchar_t* buffer, const PXSize bufferSize, PXSize* bufferWritten)
{
    //const int lengthWrittem = SetWindowTextW(uiElementID, buffer, bufferSize);

    //*bufferWritten = lengthWrittem;
}




void PXUIElementEnable()
{
    /*
    #if OSWindows
    INITCOMMONCONTROLSEX icex;

    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icex.dwICC =
        ICC_LISTVIEW_CLASSES | // listview, header
        ICC_TREEVIEW_CLASSES | // treeview, tooltips
        ICC_BAR_CLASSES | // toolbar, statusbar, trackbar, tooltips
        ICC_TAB_CLASSES | // tab, tooltips
        ICC_UPDOWN_CLASS | // updown
        ICC_PROGRESS_CLASS | // progress
        ICC_HOTKEY_CLASS | // hotkey
        ICC_ANIMATE_CLASS | // animate
        ICC_WIN95_CLASSES |
        ICC_DATE_CLASSES | // month picker, date picker, time picker, updown
        ICC_USEREX_CLASSES | // comboex
        ICC_COOL_CLASSES | // rebar (coolbar) control
        ICC_INTERNET_CLASSES |
        ICC_PAGESCROLLER_CLASS |   // page scroller

#if (NTDDI_VERSION >= NTDDI_WINXP)
        ICC_STANDARD_CLASSES |
        ICC_LINK_CLASS |
#endif
        ICC_NATIVEPXSpriteFontCTL_CLASS;   // native font control



    InitCommonControlsEx(&icex);
#endif

*/