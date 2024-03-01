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
#include <stdio.h>

PXActionResult PXAPI PXUIElementCreateOSStyle(PXUIElement* const pxUIElement, struct PXWindow_* pxWindow)
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
            styleFlags = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON| WS_BORDER; // BS_DEFPUSHBUTTON 
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
    const wchar_t* lpWindowName = L"TEST";
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(pxWindow->ID, GWLP_HINSTANCE);

    pxUIElement->ID = CreateWindowExW // Windows 2000, User32.dll, winuser.h
    (
        dwExStyle,
        className,
        lpWindowName,
        styleFlags,
        pxUIElement->Poisition2D.X,
        pxUIElement->Poisition2D.Y,
        pxUIElement->Poisition2D.Width,
        pxUIElement->Poisition2D.Height,
        pxWindow->ID,
        PXNull,  // No menu.
        hInstance,
        NULL // Pointer not needed.
    );
    const PXBool success = -1 != pxUIElement->ID;

    if(!success)
    {
        const PXActionResult pxActionResult = PXErrorCurrent();

        return pxActionResult;
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