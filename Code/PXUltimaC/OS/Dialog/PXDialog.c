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


PXActionResult PXAPI PXDialogFileOpen(PXText* const filePath)
{
#if OSUnix

    return PXActionRefusedNotImplemented;

#elif PXOSWindowsDestop

#if WindowsAtleastVista && 0




    //HRESULT BasicFileOpen()
    
        // CoCreate the File Open Dialog object.
        IFileDialog* fileDialog = PXNull;
        //MemoryClear(&pfd, sizeof(IFileDialog));

       // fileDialog->lpVtbl-> = bufferAAA;

        // hr = CoCreateInstance(&CLSID_IExample, 0, CLSCTX_INPROC_SERVER, &IID_IExample, & exampleObj)))

       // HRESULT hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

        // IID_IFolderView
        // IID_IFileDialogEvents
        // IID_IFileDialog
        //                 

        const HRESULT resss = CoInitialize(NULL);

        const HRESULT hr = CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, &IID_IFileOpenDialog, &fileDialog); // Windows 2000, Ole32.dll, combaseapi.h
        const PXActionResult result = PXWindowsHandleErrorFromID(hr);



        const HRESULT showResult = fileDialog->lpVtbl->Show(fileDialog, NULL);


        fileDialog->lpVtbl->Release(fileDialog);

        return PXActionSuccessful;


#elif WindowsAtleast2000

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
            openFileName.nMaxFile = filePath->SizeAllocated;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = "";

            {
                const PXBool wasSuccesful = GetOpenFileNameA(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

                PXActionOnErrorFetchAndReturn(!wasSuccesful);
            }

            return PXActionSuccessful;
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
            openFileName.nMaxFile = filePath->SizeAllocated;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = L"";

            const PXBool wasSuccesful = GetOpenFileNameW(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            return wasSuccesful;
        }
    }

    return PXActionInvalid;

#else
    return PXActionRefusedNotSupported;
#endif
#endif
}

PXActionResult PXAPI PXDialogFileSave(PXText* const filePath)
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

PXBool PXAPI PXDialogColorSelect(PXColorRGBI8* const color)
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

PXBool PXAPI PXDialogFontSelect()
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

PXBool PXAPI PXDialogPrint()
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