#include "PXUI.h"

#define WindowsEnableModernColorSceme 1

#if OSUnix
#elif OSWindows
#include <Windows.h>
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


#include <Media/PXText.h>
#include <Math/PXMath.h>
#include <OS/Window/PXWindow.h>
#include <OS/Memory/PXMemory.h>
#include <OS/Console/PXConsole.h>
#include <Engine/PXEngine.h>

#include <stdio.h>

#include <WindowsX.h>

PXActionResult PXAPI PXUIElementCreateOSStyle(PXUIElement* const pxUIElement, struct PXUIElementCreateData_* const pxUIElementCreateData)
{
    PXInt32U styleFlags = 0;
    wchar_t* className = PXNull;
    PXBool creationSkip = PXFalse;

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
            styleFlags = WS_VISIBLE | WS_CHILD | SS_CENTER | WS_BORDER;

            PXBool hasParenet = pxUIElement->Parent;

            if(hasParenet)
            {
                creationSkip = PXUIElementTypeButton == pxUIElement->Parent->Type;

                if(creationSkip)
                {
                    pxUIElement->Type = PXUIElementTypeButtonText;
                    pxUIElement->ID = 0;
                }
            }

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
            styleFlags = WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
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

        case PXUIElementTypeRenderFrame:
        {
            // HBRUSH hbrBackground = CreateSolidBrush(RGB(38, 38, 38));
            className = WC_STATICW;
            styleFlags = WS_VISIBLE | CS_OWNDC | CS_HREDRAW | CS_VREDRAW | WS_BORDER | WS_CHILD;
            break;
        }


        /*
    

    switch (pxWindow->Title.Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            char* windowClassName = 0;

            // Registering of class
            {
                WNDCLASSA wndclass;

                PXClear(WNDCLASSA, &wndclass);

                wndclass.style = style;
                wndclass.lpfnWndProc = lpfnWndProc;
                wndclass.cbClsExtra = cbClsExtra;
                wndclass.cbWndExtra = cbWndExtra;
                wndclass.hInstance = hInstance;
                wndclass.hIcon = hIcon;
                wndclass.hCursor = hCursor;
                wndclass.hbrBackground = hbrBackground;
                wndclass.lpszMenuName = 0;
                wndclass.lpszClassName = "PXUltima_WindowCreate";

                const WORD classID = RegisterClassA(&wndclass);

                windowClassName = (char*)classID;
            }

            windowID = CreateWindowExA // Windows 2000, User32.dll, winuser.h
            (
                windowStyle,
                windowClassName,
                pxWindow->Title.TextA,
                dwStyle,
                pxWindow->X,
                pxWindow->Y,
                pxWindow->Width,
                pxWindow->Height,
                hWndParent,
                hMenu,
                hInstance,
                lpParam
            );
        */

        case PXUIElementTypeTreeViewItem:
        {
            creationSkip = PXTrue;
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
   
    PXWindowSizeInfo pxWindowSizeInfo;
    PXWindowSizeGet(pxUIElementCreateData->WindowReference->ID, &pxWindowSizeInfo);

    pxUIElementPositionCalulcateInfo.WindowWidth = pxWindowSizeInfo.Width;
    pxUIElementPositionCalulcateInfo.WindowHeight = pxWindowSizeInfo.Height;

    PXUIElementPositionCalculcate(pxUIElement, &pxUIElementPositionCalulcateInfo);

    // If we a text that should be rendered on a button, the OS does not really intent to do this.
    // To avoid wierd graphical bugs, we will merge these into one object.
    // Still this object needs to be seperate for other render systems
     // pxUIElement->Type == PXUIElementTypeText&&;


    if(!creationSkip)
    {
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



        if(pxUIElement->ID && pxUIElement->Parent)
        {
            PXUIElement* parent = pxUIElement->Parent;

            HDWP resA = BeginDeferWindowPos(1);

            if(PXUIElementTypeButtonText == pxUIElement->Type)
            {
                HDWP resB = DeferWindowPos
                (
                    resA,
                    parent->ID,
                    pxUIElement->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );
            }
            else
            {              
                HDWP resB = DeferWindowPos
                (
                    resA,
                    pxUIElement->ID,
                    parent->ID,
                    0,
                    0,
                    0,
                    0,
                    SWP_NOMOVE | SWP_NOSIZE | SWP_NOCOPYBITS
                );         
            }

            EndDeferWindowPos(resA);
        }
               



    }

   


#if 0
    
    char buffer[255];
    PXClearList(char, buffer, 255);

    PXSize* size = buffer;
    *size = 255;

    COLORREF colorAA = RGB(255, 0, 0);

    HDC xx = GetDC(pxUIElement->ID);

    HBRUSH brush = SendMessageA(windowHandle, WM_CTLCOLORSTATIC, xx, pxUIElement->ID); // RB_SETBKCOLOR
    SetTextColor(xx, RGB(255, 0, 0));
    SetBkColor(xx, RGB(0, 255, 0));


    Pager_SetBkColor(pxUIElement->ID, &colorAA);
#endif


   // SetTextColor(xx, colorAA);


    //LRESULT ww = SendMessageA(pxUIElement->ID, EM_GETLINE, 0, buffer); // RB_SETBKCOLOR


  //  PXConsoleWriteA("\n\n%i     %i %s", brush, ww, buffer);


   // SendMessageA(pxUIElement->ID, RB_SETTEXTCOLOR, 0, &colorAA);
    







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
            
            pxUIElementUpdateInfo[1].UIElementReference = pxUIElement;
            pxUIElementUpdateInfo[1].WindowReference = pxUIElementCreateData->WindowReference;
            pxUIElementUpdateInfo[1].Property = PXUIElementPropertyTextAllign;

            PXUIElementUpdateOSStyleV(pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeButtonText: // Local override for OSStyle buttons
        {
            PXUIElement* pxButton = pxUIElement->Parent;

            PXUIElementUpdateInfo pxUIElementUpdateInfo[2];
            pxUIElementUpdateInfo[0].UIElementReference = pxButton;
            pxUIElementUpdateInfo[0].WindowReference = pxUIElementCreateData->WindowReference;
            pxUIElementUpdateInfo[0].Property = PXUIElementPropertyTextContent;

            PXCopy(PXUIElementTextInfo, &pxUIElement->TextInfo, &pxButton->TextInfo);

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

            PXUIElementUpdateOSStyleV(pxUIElementUpdateInfo, 2);

            break;
        }
        case PXUIElementTypeTreeViewItem:
        {
            PXUIElementTreeViewItemInfo* const pxUIElementTreeViewItemInfo = &pxUIElementCreateData->TreeViewItem;
            // Create ui item for a tree view

            TVINSERTSTRUCT item;
            PXClear(TVINSERTSTRUCT, &item);

            const char text[] = "[N/A]";

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                item.hParent = pxUIElementTreeViewItemInfo->ItemParent->ID;
            }         

            if(pxUIElementTreeViewItemInfo->TextDataOverride)
            {
                item.item.pszText = pxUIElementTreeViewItemInfo->TextDataOverride;
                item.item.cchTextMax = pxUIElementTreeViewItemInfo->TextSizeOverride;
            }
            else
            {
                item.item.pszText = pxUIElement->NameData;
                item.item.cchTextMax = pxUIElement->NameSize;
            }
          
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

            item.item.mask = TVIF_TEXT;

            HTREEITEM itemID = TreeView_InsertItem(pxUIElementTreeViewItemInfo->TreeView->ID, &item);

            if(pxUIElementTreeViewItemInfo->ItemParent)
            {
                TreeView_Expand
                (
                    pxUIElementTreeViewItemInfo->TreeView->ID,
                    pxUIElementTreeViewItemInfo->ItemParent->ID,
                    TVE_EXPAND
                );
            }           

            pxUIElement->ID = itemID;
            pxUIElementTreeViewItemInfo->ItemHandle = itemID;

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
            TreeView_SetBkColor(pxUIElement->ID, RGB(10, 10, 10));

            TreeView_SetTextColor(pxUIElement->ID, RGB(200, 200, 200));


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
        case PXUIElementTypeRenderFrame:
        {
#if 1
            PXUIElementSceneRenderInfo* const pxUIElementSceneRenderInfo = &pxUIElementCreateData->SceneRender;
            PXEngine* pxEngine = pxUIElementSceneRenderInfo->Engine;
  
            PXEngineStartInfo pxEngineStartInfo;
            pxEngineStartInfo.Mode = PXGraphicInitializeModeOSGUIElement;
            pxEngineStartInfo.System = PXGraphicSystemOpenGL;
            pxEngineStartInfo.Width = pxUIElement->Position.Width;
            pxEngineStartInfo.Height = pxUIElement->Position.Height;
            pxEngineStartInfo.UIElement = pxUIElement;

            PXEngineStart(pxEngine, &pxEngineStartInfo);
#endif

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

            const PXBool success = SetWindowTextA(pxUIElement->ID, pxUIElementTextInfo->Content);


         //   SendMessageA(pxUIElement->ID, PBM_SETPOS, stepsConverted, 0);

      
            


            break;
        }
        case PXUIElementPropertyTextAllign:
        {
            PXUIElementTextInfo* const pxUIElementTextInfo = &pxUIElement->TextInfo;

            HDC xx = GetDC(pxUIElement->ID);

            

            LONG ww = SetWindowLongA(pxUIElement->ID, GWL_EXSTYLE, WS_EX_LEFT | WS_EX_RIGHT);

           // const UINT allign = SetTextAlign(xx, TA_CENTER);
           // const PXBool success = GDI_ERROR == allign;

            if(!ww)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    "UI",
                    "Text-Update",
                    "Allign Failed"
                );
#endif   
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "UI",
                "Text-Update",
                "Allign successful"
            );
#endif   

            break;
        }
        case PXUIElementPropertySize:
        {
            PXUIElementPositionCalulcateInfo pxUIElementPositionCalulcateInfo;
            PXClear(PXUIElementPositionCalulcateInfo, &pxUIElementPositionCalulcateInfo);

            PXWindowSizeInfo pxWindowSizeInfo;

            PXWindowSizeGet(pxUIElementUpdateInfo->WindowReference->ID, &pxWindowSizeInfo);

            pxUIElementPositionCalulcateInfo.WindowWidth = pxWindowSizeInfo.Width;
            pxUIElementPositionCalulcateInfo.WindowHeight = pxWindowSizeInfo.Height;

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
    
        case PXUIElementPropertyItemAdd:
        {
            PXUIElementItemInfo* pxUIElementItemInfo = &pxUIElement->Item;

            TVINSERTSTRUCT item;
            PXClear(TVINSERTSTRUCT, &item);

            const char text[] = "[N/A]";

            item.item.pszText = text;
            item.item.cchTextMax = sizeof(text);
            item.hInsertAfter = TVI_ROOT;
            item.item.mask = TVIF_TEXT;

            TreeView_InsertItem(pxUIElement->ID, &item);

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