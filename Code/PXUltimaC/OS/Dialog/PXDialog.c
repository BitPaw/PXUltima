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
        #include "PXDialog.h"
// #include <shobjidl.h> // [This is an internal header. Do not use as such]         
    #endif

typedef BOOL(APIENTRY* PXGetOpenFileNameA)(LPOPENFILENAMEA);
typedef BOOL(APIENTRY* PXGetOpenFileNameW)(LPOPENFILENAMEW);
typedef BOOL(APIENTRY* PXGetSaveFileNameA)(LPOPENFILENAMEA);
typedef BOOL(APIENTRY* PXGetSaveFileNameW)(LPOPENFILENAMEW);
typedef BOOL(APIENTRY* PXChooseColorA)(LPCHOOSECOLORA);
typedef BOOL(APIENTRY* PXChooseColorW)(LPCHOOSECOLORW);
typedef BOOL(APIENTRY* PXChooseFontA)(LPCHOOSEFONTA);
typedef BOOL(APIENTRY* PXChooseFontW)(LPCHOOSEFONTW);
typedef BOOL(APIENTRY* PXPrintDlgA)(LPPRINTDLGA pPD);
typedef BOOL(APIENTRY* PXPrintDlgW)(LPPRINTDLGW pPD);

#endif


PXActionResult PXAPI PXDialogSystemInitialize(PXDialogSystem* const pxDialogSystem)
{
#if OSUnix
#elif OSWindows

    PXClear(PXDialogSystem, pxDialogSystem);

    // Load library
    {
        const PXActionResult pxActionResult = PXLibraryOpenA(&pxDialogSystem->CommonDialogLibrary, "COMDLG32.DLL");

        if(PXActionSuccessful != pxActionResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }
    }

    // Fetch functions
    {
        PXLibraryFuntionEntry pxLibraryFuntionEntry[] =
        {
            { &pxDialogSystem->DialogColorChooseA, "ChooseColorA"},
            { &pxDialogSystem->DialogColorChooseW , "ChooseColorW"},
            { &pxDialogSystem->DialogFontChooseA , "ChooseFontA"},
            { &pxDialogSystem->DialogFontChooseW , "ChooseFontW"},
            { &pxDialogSystem->DialogFileNameOpenGetA , "GetOpenFileNameA"},
            { &pxDialogSystem->DialogFileNameOpenGetW , "GetOpenFileNameW"},
            { &pxDialogSystem->DialogFileNameSaveGetA , "GetSaveFileNameA"},
            { &pxDialogSystem->DialogFileNameSaveGetW , "GetSaveFileNameW"},
            { &pxDialogSystem->DialogPrintA , "PrintDlgA"},
            { &pxDialogSystem->DialogPrintW , "PrintDlgW"}
        };

        const PXSize amount = sizeof(pxLibraryFuntionEntry) / sizeof(PXLibraryFuntionEntry);

        PXLibraryGetSymbolListA(&pxDialogSystem->CommonDialogLibrary, pxLibraryFuntionEntry, amount);
    }

#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDialogSystemRelease(PXDialogSystem* const pxDialogSystem)
{
    PXLibraryClose(&pxDialogSystem->CommonDialogLibrary);

    PXClear(PXDialogSystem, pxDialogSystem);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXDialogFileOpen(PXDialogSystem* const pxDialogSystem, PXText* const filePath)
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

    switch(filePath->Format)
    {
        case TextFormatASCII:
        case TextFormatUTF8:
        {
            const char filter[] = "All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEA openFileName;
            PXClear(OPENFILENAMEA, &openFileName);

            openFileName.lStructSize = sizeof(OPENFILENAMEA);
            openFileName.hwndOwner = owner;
            openFileName.lpstrFilter = filter;
            openFileName.lpstrFile = filePath->TextA;
            openFileName.nMaxFile = filePath->SizeAllocated;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = "";

            {
                const PXGetOpenFileNameA pxGetOpenFileNameA = (PXGetOpenFileNameA)pxDialogSystem->DialogFileNameOpenGetA;
                const PXBool wasSuccesful = pxGetOpenFileNameA(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

                PXActionOnErrorFetchAndReturn(!wasSuccesful);
            }

            return PXActionSuccessful;
        }
        case TextFormatUNICODE:
        {
            const wchar_t filter[] = L"All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEW openFileName;
            PXClear(OPENFILENAMEW , &openFileName);

            openFileName.lStructSize = sizeof(OPENFILENAMEW);
            openFileName.hwndOwner = owner;
            openFileName.lpstrFilter = filter;
            openFileName.lpstrFile = filePath->TextW;
            openFileName.nMaxFile = filePath->SizeAllocated;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = L"";

            const PXGetOpenFileNameW pxGetOpenFileNameW = (PXGetOpenFileNameW)pxDialogSystem->DialogFileNameOpenGetW;
            const PXBool wasSuccesful = pxGetOpenFileNameW(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            return PXActionSuccessful;
        }
    }

    return PXActionInvalid;

#else
    return PXActionRefusedNotSupported;
#endif
#endif
}

PXActionResult PXAPI PXDialogFileSave(PXDialogSystem* const pxDialogSystem, PXText* const filePath)
{
#if OSUnix
    return 0;

#elif PXOSWindowsDestop
    switch(filePath->Format)
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

            const PXGetSaveFileNameA pxGetSaveFileNameA = (PXGetSaveFileNameA)pxDialogSystem->DialogFileNameSaveGetA;
            const PXBool wasSuccesful = pxGetSaveFileNameA(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            PXActionOnErrorFetchAndReturn(!wasSuccesful);

            return PXActionSuccessful;
        }
        case TextFormatUNICODE:
        {
            const wchar_t filter[] = L"All Files (*.*)\0*.*\0";
            HWND owner = NULL;

            OPENFILENAMEW openFileName;
            PXClear(OPENFILENAMEW, &openFileName);

            openFileName.lStructSize = sizeof(OPENFILENAMEW);
            openFileName.hwndOwner = owner;
            openFileName.lpstrFilter = filter;
            openFileName.lpstrFile = filePath->TextW;
            openFileName.nMaxFile = MAX_PATH;
            openFileName.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
            openFileName.lpstrDefExt = L"";

            const PXGetSaveFileNameW pxGetSaveFileNameW = (PXGetSaveFileNameW)pxDialogSystem->DialogFileNameSaveGetW;
            const PXBool wasSuccesful = pxGetSaveFileNameW(&openFileName); // Windows 2000, Comdlg32.dll, commdlg.h

            PXActionOnErrorFetchAndReturn(!wasSuccesful);

            return PXActionSuccessful;
        }
}

    return PXActionInvalid;
#else
    return PXFalse;
#endif
}

PXBool PXAPI PXDialogColorSelect(PXDialogSystem* const pxDialogSystem, PXColorRGBI8* const color)
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    CHOOSECOLORW chooseColor;

    PXClear(CHOOSECOLORW, &chooseColor);
    chooseColor.lStructSize = sizeof(CHOOSECOLORW);

    COLORREF colorReference = 0;
    chooseColor.lpCustColors = &colorReference;
    chooseColor.Flags = CC_FULLOPEN | CC_SHOWHELP | CC_ANYCOLOR;

    const PXChooseColorW pxChooseColorW = (PXChooseColorW)pxDialogSystem->DialogColorChooseA;
    const PXBool success = pxChooseColorW(&chooseColor); // Windows 2000, Comdlg32.dll, commdlg.h

    if(success)
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

PXBool PXAPI PXDialogFontSelect(PXDialogSystem* const pxDialogSystem)
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    CHOOSEFONTW choosefonta;
    PXClear(CHOOSEFONTW, &choosefonta);
    choosefonta.lStructSize = sizeof(CHOOSEFONTW);
    const PXChooseFontW pxChooseFontW = (PXChooseFontW)pxDialogSystem->DialogFontChooseW;
    const PXBool success = pxChooseFontW(&choosefonta); // Windows 2000, Comdlg32.dll, commdlg.h

    return success;
#else
    return PXFalse;
#endif
}

PXBool PXAPI PXDialogPrint(PXDialogSystem* const pxDialogSystem)
{
#if OSUnix
    return PXFalse;

#elif PXOSWindowsDestop
    PRINTDLGW printdlgw;
    PXClear(PRINTDLGW, &printdlgw);
    printdlgw.lStructSize = sizeof(PRINTDLGW);

    const PXPrintDlgW pxPrintDlgW = (PXPrintDlgW)pxDialogSystem->DialogPrintW;
    const PXBool success = pxPrintDlgW(&printdlgw); // Windows 2000, Comdlg32.dll, commdlg.h

    return success;
#else
    return PXFalse;
#endif
}