#include "FileOpenDialog.h"

#include <OS/System/OSVersion.h>
#include <Media/PXText.h>
#include <OS/Memory/PXMemory.h>
#include <OS/File/PXFile.h>

#include <stdio.h>

#if OSUnix
	#define _MAX_PATH 260
#elif OSWindows
	#include <windows.h>
	#if WindowsAtleastVista
		#include <shobjidl.h> 
	#else
		#include <CommDlg.h>
	#endif
#endif

PXBool FileOpenDialogA(char* filePathOutput)
{
    wchar_t pathW[PathMaxSize];

    PXTextCopyAW(filePathOutput, PathMaxSize, pathW, PathMaxSize);

    return FileOpenDialogW(pathW);
}

PXBool FileOpenDialogW(wchar_t* filePathOutput)
{
    PXBool wasSuccesful = 0;

#if OSWindows
#if OSWindowsXP
    const wchar_t* filter = L"All Files (*.*)\0*.*\0";
    HWND owner = NULL;

    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = owner;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = filePathOutput;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = L"";

    if(GetOpenFileName(&ofn))
    {
        wasSuccesful = PXTrue;
    }
#else

#if 0 // Can only be used in C++ context
    {
        const HRESULT hResult = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        const unsigned char sucessful = SUCCEEDED(hResult);

        if(!sucessful)
        {
            return 0;
        }
    }

    IFileOpenDialog* pFileOpen = 0;

    // Create the FileOpenDialog object.
    const HRESULT hResult = CoCreateInstance
    (
        CLSID_FileOpenDialog,
        NULL,
        CLSCTX_ALL,
        IID_IFileOpenDialog,
        &pFileOpen
    );

    if(SUCCEEDED(hResult))
    {
        // Show the Open dialog box.
        hResult = pFileOpen->Show(NULL);

        // Get the file name from the dialog box.
        if(SUCCEEDED(hResult))
        {
            IShellItem* pItem;
            hResult = pFileOpen->GetResult(&pItem);
            if(SUCCEEDED(hResult))
            {
                PWSTR pszFilePath; // wchar
                hResult = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                // Display the file name to the user.
                if(SUCCEEDED(hResult))
                {
                    lstrcpyW(filePathOutput, pszFilePath);

                    wasSuccesful = 1u;

                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }

        pFileOpen->Release();
    }

    CoUninitialize();
#endif
    
#endif
#elif OSUnix
#endif

    return wasSuccesful;
}
