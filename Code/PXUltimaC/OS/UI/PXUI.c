#include "PXUI.h"

#define WindowsEnableModernColorSceme 1

#if OSUnix
#elif OSWindows

#include <commctrl.h>
#include <commdlg.h>
//#include <ShObjIdl.h>

#if WindowsEnableModernColorSceme

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
#include <stdio.h>


#if OSWindows
void PXUIElementTextGet(const UIElementID uiElementID, const wchar_t* buffer, const PXSize bufferSize, PXSize* bufferRead)
{
    const int textLength = GetWindowTextLengthW(uiElementID) + 1;
    const int maximum = PXMathMinimum(textLength, bufferSize);
    const int lengthRead = GetWindowTextW(uiElementID, buffer, maximum);

    *bufferRead = lengthRead;
}

void PXUIElementTextSet(const UIElementID uiElementID, const wchar_t* buffer, const PXSize bufferSize, PXSize* bufferWritten)
{
    //const int lengthWrittem = SetWindowTextW(uiElementID, buffer, bufferSize);

    //*bufferWritten = lengthWrittem;
}
#endif

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
#endif*/
}


void UIElementConstruct(const PXWindowID window, UIElementID* ID, UILayout* Layout, const wchar_t* className, const wchar_t* name)
{
#if OSUnix
#elif OSWindows
    DWORD dwExStyle = 0;
    wchar_t* lpClassName = className;// Predefined class; Unicode assumed ;
    wchar_t* lpWindowName = name;
    HWND hWndParent = window;
    HMENU hMenu = 0; // No menu.
    HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(window, GWLP_HINSTANCE);
    LPVOID lpParam = NULL; // Pointer not needed.

    *ID = CreateWindowExW
    (
        dwExStyle,
        lpClassName,
        lpWindowName,
        Layout->Style,
        Layout->X,
        Layout->Y,
        Layout->Width,
        Layout->Height,
        hWndParent,
        hMenu,
        hInstance,
        lpParam
    );
#endif

    if(*ID)
    {
        printf("\n[i][UI] %ls create : %p\n", className,*ID);
    }
    else
    {
        printf("\n[x][UI] %ls create <FAILED> : %p\n", className, *ID);
    }


}

void ButtonConstruct(const PXWindowID window, OSButton* button, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(button, sizeof(OSButton));

#if OSUnix
#elif OSWindows
    button->Layout.Style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_USERBUTTON | WS_BORDER;
    button->Layout.X = x;
    button->Layout.Y = y;
    button->Layout.Width = width;
    button->Layout.Height = height;

    UIElementConstruct(window, &button->ID, &button->Layout, WC_BUTTONW, buttonText);
#endif
}

void ComboBoxConstruct(const PXWindowID window, OSComboBox* comboBox, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(comboBox, sizeof(OSComboBox));

#if OSUnix
#elif OSWindows

    comboBox->Layout.Style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
    comboBox->Layout.X = x;
    comboBox->Layout.Y = y;
    comboBox->Layout.Width = width;
    comboBox->Layout.Height = height;

    // WC_COMBOBOXEXW or WC_COMBOBOXW ??

    UIElementConstruct(window, &comboBox->ID, &comboBox->Layout, WC_COMBOBOXEXW, buttonText);
#endif
}

void ListBoxConstruct(const PXWindowID window, OSListBox* listBox, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(listBox, sizeof(OSListBox));

#if OSUnix
#elif OSWindows

    listBox->Layout.Style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
    listBox->Layout.X = x;
    listBox->Layout.Y = y;
    listBox->Layout.Width = width;
    listBox->Layout.Height = height;

    UIElementConstruct(window, &listBox->ID, &listBox->Layout, WC_LISTBOXW, buttonText);
#endif
}

void TextEditConstruct(const PXWindowID window, OSTextEdit* textEdit, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(textEdit, sizeof(OSTextEdit));

#if OSUnix
#elif OSWindows

    textEdit->Layout.Style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
    textEdit->Layout.X = x;
    textEdit->Layout.Y = y;
    textEdit->Layout.Width = width;
    textEdit->Layout.Height = height;

    UIElementConstruct(window, &textEdit->ID, &textEdit->Layout, WC_EDITW, buttonText);
#endif
}

void TextEditContentGet(OSTextEdit* textEdit, const wchar_t* buffer, const PXSize bufferSize, PXSize* bufferWritten)
{
#if OSUnix
#elif OSWindows
    PXUIElementTextGet(textEdit->ID, buffer, bufferSize, bufferWritten);
#endif
}

void RichEditConstruct(const PXWindowID window, OSRichEdit* richEdit, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(richEdit, sizeof(OSRichEdit));

#if OSUnix
#elif OSWindows

    richEdit->Layout.Style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
    richEdit->Layout.X = x;
    richEdit->Layout.Y = y;
    richEdit->Layout.Width = width;
    richEdit->Layout.Height = height;

    UIElementConstruct(window, &richEdit->ID, &richEdit->Layout, L"RICHEDIT_CLASS", buttonText);
#endif
}

void ScrollBarConstruct(const PXWindowID window, OSScrollBar* scrollBar, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(scrollBar, sizeof(OSScrollBar));

#if OSUnix
#elif OSWindows

    scrollBar->Layout.Style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER;
    scrollBar->Layout.X = x;
    scrollBar->Layout.Y = y;
    scrollBar->Layout.Width = width;
    scrollBar->Layout.Height = height;

    UIElementConstruct(window, &scrollBar->ID, &scrollBar->Layout, WC_SCROLLBARW, buttonText);
#endif
}

void PXUITextConstruct(const PXWindowID window, PXOSUIText* uiText, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(uiText, sizeof(PXOSUIText));

#if OSUnix
#elif OSWindows

    uiText->Layout.Style = WS_VISIBLE | WS_CHILD | SS_LEFT | WS_BORDER;
    uiText->Layout.X = x;
    uiText->Layout.Y = y;
    uiText->Layout.Width = width;
    uiText->Layout.Height = height;

    UIElementConstruct(window, &uiText->ID, &uiText->Layout, WC_STATICW, buttonText);
#endif

}

void OSCheckBoxConstruct(const PXWindowID window, PXUICheckBox* uiCheckBox, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
    PXMemoryClear(uiCheckBox, sizeof(PXUICheckBox));

#if OSUnix
#elif OSWindows

    uiCheckBox->Layout.Style = WS_VISIBLE | WS_CHILD | BS_CHECKBOX | WS_BORDER;
    uiCheckBox->Layout.X = x;
    uiCheckBox->Layout.Y = y;
    uiCheckBox->Layout.Width = width;
    uiCheckBox->Layout.Height = height;

    UIElementConstruct(window, &uiCheckBox->ID, &uiCheckBox->Layout, WC_BUTTONW, buttonText);

    CheckDlgButton(uiCheckBox->ID, 1, BST_CHECKED); // BST_UNCHECKED
#endif
}

void PXUITrackbarCreate(const PXWindowID window, PXUITrackbar* PXUITrackbar, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUITrackbar->Layout.Style = WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS | WS_BORDER;
    PXUITrackbar->Layout.X = x;
    PXUITrackbar->Layout.Y = y;
    PXUITrackbar->Layout.Width = width;
    PXUITrackbar->Layout.Height = height;

    UIElementConstruct(window, &PXUITrackbar->ID, &PXUITrackbar->Layout, TRACKBAR_CLASSW, buttonText);
#endif
}

void PXUIStatusbarCreate(const PXWindowID window, PXUIStatusbar* PXUIStatusbar, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIStatusbar->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIStatusbar->Layout.X = x;
    PXUIStatusbar->Layout.Y = y;
    PXUIStatusbar->Layout.Width = width;
    PXUIStatusbar->Layout.Height = height;

    UIElementConstruct(window, &PXUIStatusbar->ID, &PXUIStatusbar->Layout, STATUSCLASSNAMEW, buttonText);
#endif
}

void PXUIUpDownCreate(const PXWindowID window, PXUIUpDown* PXUIUpDown, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIUpDown->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIUpDown->Layout.X = x;
    PXUIUpDown->Layout.Y = y;
    PXUIUpDown->Layout.Width = width;
    PXUIUpDown->Layout.Height = height;

    UIElementConstruct(window, &PXUIUpDown->ID, &PXUIUpDown->Layout, UPDOWN_CLASSW, buttonText);
#endif
}

void PXUIProgressbarCreate(const PXWindowID window, PXUIProgressbar* PXUIProgressbar, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIProgressbar->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIProgressbar->Layout.X = x;
    PXUIProgressbar->Layout.Y = y;
    PXUIProgressbar->Layout.Width = width;
    PXUIProgressbar->Layout.Height = height;

    UIElementConstruct(window, &PXUIProgressbar->ID, &PXUIProgressbar->Layout, PROGRESS_CLASSW, buttonText); // "Progress bar"
#endif
}

void PXUIHotKeyCreate(const PXWindowID window, PXUIHotKey* PXUIHotKey, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIHotKey->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIHotKey->Layout.X = x;
    PXUIHotKey->Layout.Y = y;
    PXUIHotKey->Layout.Width = width;
    PXUIHotKey->Layout.Height = height;

    UIElementConstruct(window, &PXUIHotKey->ID, &PXUIHotKey->Layout, HOTKEY_CLASSW, buttonText);
#endif
}

void PXUICalenderCreate(const PXWindowID window, PXUICalender* PXUICalender, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUICalender->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUICalender->Layout.X = x;
    PXUICalender->Layout.Y = y;
    PXUICalender->Layout.Width = width;
    PXUICalender->Layout.Height = height;

    UIElementConstruct(window, &PXUICalender->ID, &PXUICalender->Layout, MONTHCAL_CLASSW, buttonText);
#endif
}

void PXUIToolTipCreate(const PXWindowID window, PXUIToolTip* PXUIToolTip, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIToolTip->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIToolTip->Layout.X = x;
    PXUIToolTip->Layout.Y = y;
    PXUIToolTip->Layout.Width = width;
    PXUIToolTip->Layout.Height = height;

    UIElementConstruct(window, &PXUIToolTip->ID, &PXUIToolTip->Layout, TOOLTIPS_CLASSW, buttonText);
#endif
}

void PXUIAnimateCreate(const PXWindowID window, PXUIAnimate* PXUIAnimate, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIAnimate->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIAnimate->Layout.X = x;
    PXUIAnimate->Layout.Y = y;
    PXUIAnimate->Layout.Width = width;
    PXUIAnimate->Layout.Height = height;

    UIElementConstruct(window, &PXUIAnimate->ID, &PXUIAnimate->Layout, ANIMATE_CLASSW, buttonText);
#endif
}

void PXUIDateTimePickerCreate(const PXWindowID window, PXUIDateTimePicker* PXUIDateTimePicker, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIDateTimePicker->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIDateTimePicker->Layout.X = x;
    PXUIDateTimePicker->Layout.Y = y;
    PXUIDateTimePicker->Layout.Width = width;
    PXUIDateTimePicker->Layout.Height = height;

    UIElementConstruct(window, &PXUIDateTimePicker->ID, &PXUIDateTimePicker->Layout, DATETIMEPICK_CLASSW, buttonText);
#endif
}

void PXUIGroupBoxCreate(const PXWindowID window, PXUIGroupBox* PXUIGroupBox, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIGroupBox->Layout.Style = WS_VISIBLE | WS_CHILD | BS_GROUPBOX | WS_BORDER;
    PXUIGroupBox->Layout.X = x;
    PXUIGroupBox->Layout.Y = y;
    PXUIGroupBox->Layout.Width = width;
    PXUIGroupBox->Layout.Height = height;

    UIElementConstruct(window, &PXUIGroupBox->ID, &PXUIGroupBox->Layout, WC_BUTTONW, buttonText);
#endif
}

void PXUIRadioButtonCreate(const PXWindowID window, PXUIRadioButton* PXUIRadioButton, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIRadioButton->Layout.Style = WS_VISIBLE | WS_CHILD | BS_RADIOBUTTON | WS_BORDER;
    PXUIRadioButton->Layout.X = x;
    PXUIRadioButton->Layout.Y = y;
    PXUIRadioButton->Layout.Width = width;
    PXUIRadioButton->Layout.Height = height;

    UIElementConstruct(window, &PXUIRadioButton->ID, &PXUIRadioButton->Layout, WC_BUTTONW, buttonText);
#endif
}

void PXUIGroupRadioButtonCreate(const PXWindowID window, PXUIGroupRadioButton* PXUIGroupRadioButton, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIGroupRadioButton->Layout.Style = WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | WS_BORDER;
    PXUIGroupRadioButton->Layout.X = x;
    PXUIGroupRadioButton->Layout.Y = y;
    PXUIGroupRadioButton->Layout.Width = width;
    PXUIGroupRadioButton->Layout.Height = height;

    UIElementConstruct(window, &PXUIGroupRadioButton->ID, &PXUIGroupRadioButton->Layout, WC_BUTTONW, buttonText);
#endif
}

void PXUIListBoxCreate(const PXWindowID window, PXUIListBox* PXUIListBox, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIListBox->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIListBox->Layout.X = x;
    PXUIListBox->Layout.Y = y;
    PXUIListBox->Layout.Width = width;
    PXUIListBox->Layout.Height = height;

    UIElementConstruct(window, &PXUIListBox->ID, &PXUIListBox->Layout, WC_LISTBOXW, buttonText);
#endif
}

void PXUITreeViewCreate(const PXWindowID window, PXUITreeView* PXUITreeView, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUITreeView->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUITreeView->Layout.X = x;
    PXUITreeView->Layout.Y = y;
    PXUITreeView->Layout.Width = width;
    PXUITreeView->Layout.Height = height;

    UIElementConstruct(window, &PXUITreeView->ID, &PXUITreeView->Layout, WC_TREEVIEWW, buttonText);
#endif
}

void PXUIIPInputCreate(const PXWindowID window, PXUIIPInput* PXUIIPInput, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIIPInput->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIIPInput->Layout.X = x;
    PXUIIPInput->Layout.Y = y;
    PXUIIPInput->Layout.Width = width;
    PXUIIPInput->Layout.Height = height;

    UIElementConstruct(window, &PXUIIPInput->ID, &PXUIIPInput->Layout, WC_IPADDRESSW, buttonText);
#endif
}

void PXUITabControlCreate(const PXWindowID window, PXUITabControl* PXUITabControl, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUITabControl->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUITabControl->Layout.X = x;
    PXUITabControl->Layout.Y = y;
    PXUITabControl->Layout.Width = width;
    PXUITabControl->Layout.Height = height;

    UIElementConstruct(window, &PXUITabControl->ID, &PXUITabControl->Layout, WC_TABCONTROLW, buttonText);
#endif
}

void PXUIPageScrollerCreate(const PXWindowID window, PXUIPageScroller* PXUIPageScroller, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIPageScroller->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIPageScroller->Layout.X = x;
    PXUIPageScroller->Layout.Y = y;
    PXUIPageScroller->Layout.Width = width;
    PXUIPageScroller->Layout.Height = height;

    UIElementConstruct(window, &PXUIPageScroller->ID, &PXUIPageScroller->Layout, WC_PAGESCROLLERW, buttonText);
#endif
}

void PXUIFontSelectorCreate(const PXWindowID window, PXUIFontSelector* PXUIFontSelector, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIFontSelector->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;// NFS_USEFONTASSOC;
    PXUIFontSelector->Layout.X = x;
    PXUIFontSelector->Layout.Y = y;
    PXUIFontSelector->Layout.Width = width;
    PXUIFontSelector->Layout.Height = height;

    UIElementConstruct(window, &PXUIFontSelector->ID, &PXUIFontSelector->Layout, WC_NATIVEFONTCTLW, buttonText);
#endif
}

void PXUIHeaderCreate(const PXWindowID window, PXUIHeader* PXUIHeader, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUIHeader->Layout.Style = WS_VISIBLE | WS_CHILD | WS_BORDER;
    PXUIHeader->Layout.X = x;
    PXUIHeader->Layout.Y = y;
    PXUIHeader->Layout.Width = width;
    PXUIHeader->Layout.Height = height;

    UIElementConstruct(window, &PXUIHeader->ID, &PXUIHeader->Layout, WC_HEADERW, buttonText);
#endif
}

void PXUILinkCreate(const PXWindowID window, PXUILink* PXUILink, const unsigned int x, const unsigned int y, const unsigned int width, const unsigned int height, const wchar_t* buttonText)
{
#if OSUnix
#elif OSWindows
    PXUILink->Layout.Style = WS_VISIBLE | WS_CHILD;
    PXUILink->Layout.X = x;
    PXUILink->Layout.Y = y;
    PXUILink->Layout.Width = width;
    PXUILink->Layout.Height = height;

    UIElementConstruct(window, &PXUILink->ID, &PXUILink->Layout, WC_LINK, buttonText);
#endif
}
