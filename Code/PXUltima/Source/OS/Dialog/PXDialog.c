#include "PXDialog.h"

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

#elif OSWindows

#if WindowsAtleastVista




    //HRESULT BasicFileOpen()
    {
        // CoCreate the File Open Dialog object.
        char bufferVV[1024]; MemoryClear(bufferVV, 1024);
        char bufferAAA[1024]; MemoryClear(bufferAAA, 1024);

        IFileDialog* fileDialog = bufferVV;
        //MemoryClear(&pfd, sizeof(IFileDialog));

        fileDialog->lpVtbl = bufferAAA;

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


        /*
        if (SUCCEEDED(hr))
        {
            // Create an event handling object, and hook it up to the dialog.
            IFileDialogEvents* pfde = NULL;

            pfd.lpVtbl.

            hr = CDialogEventHandler_CreateInstance(IID_PPV_ARGS(&pfde));
            if (SUCCEEDED(hr))
            {
                // Hook up the event handler.
                DWORD dwCookie;
                hr = pfd->Advise(pfde, &dwCookie);
                if (SUCCEEDED(hr))
                {
                    // Set the options on the dialog.
                    DWORD dwFlags;

                    // Before setting, always get the options first in order
                    // not to override existing options.
                    hr = pfd->GetOptions(&dwFlags);
                    if (SUCCEEDED(hr))
                    {
                        // In this case, get shell items only for file system items.
                        hr = pfd->SetOptions(dwFlags | FOS_FORCEFILESYSTEM);
                        if (SUCCEEDED(hr))
                        {
                            // Set the file types to display only.
                            // Notice that this is a 1-based array.
                            hr = pfd->SetFileTypes(ARRAYSIZE(c_rgSaveTypes), c_rgSaveTypes);
                            if (SUCCEEDED(hr))
                            {
                                // Set the selected file type index to Word Docs for this example.
                                hr = pfd->SetFileTypeIndex(INDEX_WORDDOC);
                                if (SUCCEEDED(hr))
                                {
                                    // Set the default extension to be ".doc" file.
                                    hr = pfd->SetDefaultExtension(L"doc;docx");
                                    if (SUCCEEDED(hr))
                                    {
                                        // Show the dialog
                                        hr = pfd->Show(NULL);
                                        if (SUCCEEDED(hr))
                                        {
                                            // Obtain the result once the user clicks
                                            // the 'Open' button.
                                            // The result is an IShellItem object.
                                            IShellItem* psiResult;
                                            hr = pfd->GetResult(&psiResult);
                                            if (SUCCEEDED(hr))
                                            {
                                                // We are just going to print out the
                                                // name of the file for sample sake.
                                                PWSTR pszFilePath = NULL;
                                                hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH,
                                                    &pszFilePath);
                                                if (SUCCEEDED(hr))
                                                {
                                                    TaskDialog(NULL,
                                                        NULL,
                                                        L"CommonFileDialogApp",
                                                        pszFilePath,
                                                        NULL,
                                                        TDCBF_OK_BUTTON,
                                                        TD_INFORMATION_ICON,
                                                        NULL);
                                                    CoTaskMemFree(pszFilePath);
                                                }
                                                psiResult->Release();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                    // Unhook the event handler.
                    pfd->Unadvise(dwCookie);
                }
                pfde->Release();
            }
            pfd->Release();
}
        return hr;
}







*/










#else // OSWindowsXP

    switch (filePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            const char filter[] = "All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEA openFileName;
            MemoryClear(&openFileName, sizeof(OPENFILENAMEA));

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
            MemoryClear(&openFileName, sizeof(OPENFILENAMEW));

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
#endif
}

PXBool PXDialogFileSave(PXText* const filePath)
{

}

PXBool PXDialogColorSelect(PXColorRGBI8* const color)
{
#if OSUnix
    return PXFalse;

#elif OSWindows
    CHOOSECOLORW chooseColor;
   
    MemoryClear(&chooseColor, sizeof(CHOOSECOLORW));
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
#endif
}

PXBool PXDialogFontSelect()
{
#if OSUnix
    return PXFalse;

#elif OSWindows
    CHOOSEFONTW choosefonta;
    MemoryClear(&choosefonta, sizeof(CHOOSEFONTW));
    choosefonta.lStructSize = sizeof(CHOOSEFONTW);

    const PXBool success = ChooseFontW(&choosefonta); // Windows 2000, Comdlg32.dll, commdlg.h

    return success;
#endif
}

PXBool PXDialogPrint()
{
#if OSUnix
    return PXFalse;

#elif OSWindows
    PRINTDLGW printdlgw;
    MemoryClear(&printdlgw, sizeof(PRINTDLGW));
    printdlgw.lStructSize = sizeof(PRINTDLGW);

    const PXBool success = PrintDlgW(&printdlgw); // Windows 2000, Comdlg32.dll, commdlg.h

    return success;
#endif
}