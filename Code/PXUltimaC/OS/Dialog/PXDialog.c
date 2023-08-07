#include "PXDialog.h"

#if PXDialogUSE

#include <OS/File/PXFile.h>


#include <stdio.h>

#if OSUnix
	#define _MAX_PATH 260
#elif OSWindows
	#include <windows.h>

    #include <CommDlg.h>

	#if WindowsAtleastVista
        #include <Shlobj.h>
        #include <combaseapi.h>
		// #include <shobjidl.h> // [This is an internal header. Do not use as such] 		
	#endif
#endif





PXBool PXDialogFileOpen(PXText* const filePath)
{

#if OSUnix

    // Do stuff

#elif PXOSWindowsDestop

#if WindowsAtleastVista




    //HRESULT BasicFileOpen()
    
        // CoCreate the File Open Dialog object.
        char bufferVV[1024]; PXMemoryClear(bufferVV, 1024);
        char bufferAAA[1024]; PXMemoryClear(bufferAAA, 1024);

        IFileDialog* fileDialog = bufferVV;
        //MemoryClear(&pfd, sizeof(IFileDialog));

       // fileDialog->lpVtbl-> = bufferAAA;

        // hr = CoCreateInstance(&CLSID_IExample, 0, CLSCTX_INPROC_SERVER, &IID_IExample, & exampleObj)))

       // HRESULT hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

        // IID_IFolderView
        // IID_IFileDialogEvents
        // IID_IFileDialog
        //                 

        const HRESULT resss = CoInitialize(0);

        const HRESULT hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, &IID_IFileOpenDialog, &fileDialog); // Windows 2000, Ole32.dll, combaseapi.h


        switch (hr)
        {

            case S_OK:// An instance of the specified object class was successfully created.
            {
                printf("An instance of the specified object class was successfully created.\n");
                break;
            }

            case REGDB_E_CLASSNOTREG: // A specified class is not registered in the registration database.Also can indicate that the type of server you requested in the CLSCTX enumeration is not registered or the values for the server types in the registry are corrupt.
            {
                printf("A specified class is not registered in the registration database.Also can indicate that the type of server you requested in the CLSCTX enumeration is not registered or the values for the server types in the registry are corrupt.\n");
                break;
            }
            case CLASS_E_NOAGGREGATION: // This class cannot be created as part of an aggregate.
            {
                printf("This class cannot be created as part of an aggregate.\n");
                break;
            }
            case E_NOINTERFACE: // The specified class does not implement the requested interface, or the controlling IUnknown does not expose the requested interface.
            {
                printf("The specified class does not implement the requested interface, or the controlling IUnknown does not expose the requested interface.\n");
                break;
            }
            case E_POINTER: // The ppv parameter is NULL.
            {
                printf("The ppv parameter is NULL.\n");
                break;
            }
            default:
                break;
        }


        printf("");


        const HRESULT showResult = fileDialog->lpVtbl->Show(fileDialog, NULL);


        fileDialog->lpVtbl->Release(&fileDialog);

        return PXActionInvalid;


#else // OSWindowsXP

    switch (filePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            const char filter[] = "All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEA openFileName;        
            PXMemoryClear(&openFileName, sizeof(OPENFILENAMEA));

            openFileName.lStructSize = sizeof(OPENFILENAMEA);
            openFileName.hwndOwner = owner;
            openFileName.lpstrFilter = filter;
            openFileName.lpstrFile = filePath->TextA;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = "";

            const PXBool wasSuccesful = GetOpenFileNameA(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            return wasSuccesful;
        }
        case TextFormatUNICODE:
        {
            const wchar_t filter[] = L"All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEW openFileName;
            PXMemoryClear(&openFileName, sizeof(OPENFILENAMEW));

            openFileName.lStructSize = sizeof(OPENFILENAMEW);
            openFileName.hwndOwner = owner;
            openFileName.lpstrFilter = filter;
            openFileName.lpstrFile = filePath->TextW;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = L"";

            const PXBool wasSuccesful = GetOpenFileNameW(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            return wasSuccesful;
        }
    }

    return PXActionInvalid;


#endif
#else
return PXFalse;
#endif
}

PXBool PXDialogFileSave(PXText* const filePath)
{
#if OSUnix
    return 0;

#elif PXOSWindowsDestop
    switch (filePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            const char filter[] = "All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEA openFileName;
            PXMemoryClear(&openFileName, sizeof(OPENFILENAMEA));

            openFileName.lStructSize = sizeof(OPENFILENAMEA);
            openFileName.hwndOwner = owner;
            openFileName.lpstrFilter = filter;
            openFileName.lpstrFile = filePath->TextA;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = "";

            const PXBool wasSuccesful = GetSaveFileNameA(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            return wasSuccesful;
        }
        case TextFormatUNICODE:
        {
            const wchar_t filter[] = L"All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEW openFileName;
            PXMemoryClear(&openFileName, sizeof(OPENFILENAMEW));

            openFileName.lStructSize = sizeof(OPENFILENAMEW);
            openFileName.hwndOwner = owner;
            openFileName.lpstrFilter = filter;
            openFileName.lpstrFile = filePath->TextW;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = L"";

            const PXBool wasSuccesful = GetSaveFileNameW(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            return wasSuccesful;
        }
    }

    return PXActionInvalid;
#else
    return PXFalse;
#endif
}

PXBool PXDialogColorSelect(PXColorRGBI8* const color)
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    CHOOSECOLORW chooseColor;
   
    PXMemoryClear(&chooseColor, sizeof(CHOOSECOLORW));
    chooseColor.lStructSize = sizeof(CHOOSECOLORW);

    COLORREF colorReference = 0;
    chooseColor.lpCustColors = &colorReference;
    chooseColor.Flags = CC_FULLOPEN | CC_SHOWHELP | CC_ANYCOLOR;

    const PXBool success = ChooseColorW(&chooseColor); // Windows 2000, Comdlg32.dll, commdlg.h

    if (success)
    {
        color->Blue = (colorReference & 0x00FF0000) >> 16u;
        color->Green = (colorReference & 0x0000FF00) >> 8u;
        color->Red = (colorReference & 0x000000FF);
    }
    else
    {
        color->Blue = 0;
        color->Green = 0;
        color->Red = 0;
    }    

    return success;
#else
    return PXFalse;
#endif
}

PXBool PXDialogFontSelect()
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    CHOOSEFONTW choosefonta;
    PXMemoryClear(&choosefonta, sizeof(CHOOSEFONTW));
    choosefonta.lStructSize = sizeof(CHOOSEFONTW);

    const PXBool success = ChooseFontW(&choosefonta); // Windows 2000, Comdlg32.dll, commdlg.h

    return success;
#else
    return PXFalse;
#endif
}

PXBool PXDialogPrint()
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    PRINTDLGW printdlgw;
    PXMemoryClear(&printdlgw, sizeof(PRINTDLGW));
    printdlgw.lStructSize = sizeof(PRINTDLGW);

    const PXBool success = PrintDlgW(&printdlgw); // Windows 2000, Comdlg32.dll, commdlg.h

    return success;
#else
    return PXFalse;
#endif
}

#endif