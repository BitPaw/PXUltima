#include "PXFileDirectory.h"
#include <PX/OS/Console/PXConsole.h>
#include <PX/Type/PXIcon.h>
#include <PX/Engine/PXGUI.h>


#if OSUnix
#elif OSWindows
#include <CommCtrl.h>
#endif

const char PXFileDirectoryName[] = "FileDirectory";
const PXI8U PXFileDirectoryNameLength = sizeof(PXFileDirectoryName);
const PXECSRegisterInfoStatic PXFileDirectoryInfoStatic =
{
    {sizeof(PXFileDirectoryName), sizeof(PXFileDirectoryName), PXFileDirectoryName, TextFormatASCII},
    sizeof(PXFileDirectory),
    __alignof(PXFileDirectory),
    PXECSTypeEntity,
    PXFileDirectoryCreate,
    PXFileDirectoryDestroy,
    PXFileDirectoryDraw
};
PXECSRegisterInfoDynamic PXFileDirectoryInfoDynamic;

void PXAPI PXFileDirectoryRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXFileDirectoryInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXFileDirectoryInfoDynamic;
}

const char PXFileNone[] = "<**No files**>";
const PXI8U PXFileNoneLength = sizeof(PXFileNone);

const char PXLocalPath[] = "./";
const PXI8U PXLocalPathLength = sizeof(PXLocalPath);


void PXAPI PXFileDirectoryEntryDraw(PXFileDirectory PXREF pxFileDirectory, PXWindow PXREF pxWindow, PXFileEntry PXREF pxFileEntry, PXDrawInfo PXREF pxTextDrawInfo)
{
    PXText PXREF pxText = pxTextDrawInfo->Text;

    //PXClear(PXTextDrawInfo, &pxTextDrawInfo);

       //PXFileEntry PXREF pxFileEntry = PXListItemAtIndexGetT(PXFileEntry, &pxDirectorySearchCache->EntryList, i);






#if 1
       // PXIcon pxIcon;

       // const PXResult iconResult = PXIconGetViaFilePath(&pxIcon, &pxFileEntry->FilePath);

       // PXI32S rowY = (pxDrawInfo->RectangleXYWH.Height * (i));

    PXDrawInfo old = *pxTextDrawInfo;

    // pxTextDrawInfo->RectangleXYWH.Height = 16;
    //pxTextDrawInfo->RectangleXYWH.Width = 16;
    PXWindowDrawRectangle(pxWindow, pxTextDrawInfo);

    // Let's try to get the icon?

    // Draw icon
    PXWindowDrawIcon
    (
        pxWindow,
        pxTextDrawInfo,
        &pxFileEntry->Icon
    );


#endif

    int x = PXFileElementInfoTypeFile;

    //pxTextDrawInfo->RectangleXYWH.X += 20;

    switch(x) // pxFileEntry->Type
    {
        default:
        case PXFileElementInfoTypeFile:
        {
            // icon = fileIcon;
          

            pxText->Format = TextFormatUNICODE;

            PXWindowDrawText(pxWindow, pxTextDrawInfo);

#if 0

            PXText pxA;
            PXTextConstructFromAdressA(&pxA, pxFileEntry->FilePathData, pxFileEntry->FilePathSize, pxFileEntry->FilePathSize);

            PXText pxTextB;
            PXTextConstructBufferA(&pxTextB, 64);

            PXSize extensionSize = PXFilePathExtensionGet(&pxA, &pxTextB);

            if(extensionSize != 0)
            {
                HICON smsm = PXGUIIconGetViaExtension(pxTextB.A);

                if(smsm)
                {
                    icon = smsm;
                }
            }
#else



#endif

            break;
        }
        case PXFileElementInfoTypeDictionary:
        {
            //   icon = folderIcon;
            break;
        }
    }

    *pxTextDrawInfo = old;
}

PXResult PXAPI PXFileDirectoryDraw(PXFileDirectory PXREF pxFileDirectory, PXDrawInfo PXREF pxDrawInfo)
{
    PXDirectorySearchCache PXREF pxDirectorySearchCache = &pxFileDirectory->DirectorySearchCache;
    
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();
    PXWindow PXREF pxWindow = pxFileDirectory->WindowBase;

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXFileDirectoryName,
        "Draw",
        "FileDirectoryView"
    );
#endif

    pxDrawInfo->Brush = pxGUITheme->BrushMainPrimary;

   // PXWindowDrawRectangle3D(pxWindow, pxDrawInfo);

    return PXResultOK;

    if(pxFileDirectory->IsDirty)
    {
        PXText pxTExt;
        PXTextFromAdressA(&pxTExt, PXLocalPath, PXLocalPathLength, PXLocalPathLength);

        PXDirectorySearch(pxDirectorySearchCache, &pxTExt);

        pxFileDirectory->IsDirty = PXFalse;
    }

    const PXSize iconSize = 16;
    char textBuffer[128];

    PXText pxText;
    PXTextFromAdressA(&pxText, textBuffer, sizeof(textBuffer), sizeof(textBuffer));
    PXTextPrint(&pxText, "FilePath");
    
    pxDrawInfo->Text = &pxText;
    //pxDrawInfo->RectangleXYWH.Width = 150;
    //pxDrawInfo->RectangleXYWH.Height = 20;

    PXWindowDrawText(pxWindow, pxDrawInfo);


    if(1)
    {
        // pxDrawInfo->RectangleXYWH.X = sizeof(textBuffer);
        //pxDrawInfo->RectangleXYWH.Width = 16;
        PXButtonDraw(pxFileDirectory->ButtonFileBrowser, pxDrawInfo);
        //pxDrawInfo->RectangleXYWH.X += 18;
                
        PXButtonDraw(pxFileDirectory->ButtonFolderUp, pxDrawInfo);
        //pxDrawInfo->RectangleXYWH.X += 18;

        PXButtonDraw(pxFileDirectory->ButtonScrollUp, pxDrawInfo);
        //pxDrawInfo->RectangleXYWH.X += 18;

        PXButtonDraw(pxFileDirectory->ButtonScrollDown, pxDrawInfo);
        //pxDrawInfo->RectangleXYWH.X += 18;

        PXButtonDraw(pxFileDirectory->ButtonListRefresh, pxDrawInfo);

        //pxDrawInfo->RectangleXYWH.X = 0;
    }

    pxDrawInfo->Text = &pxText;
    pxDrawInfo->Color.Red = 0;
    pxDrawInfo->Color.Green = 0;
    pxDrawInfo->Color.Blue = 0;

    PXTextDrawInfoRowNext(pxDrawInfo);

    if(0 == pxDirectorySearchCache->FilePathCache.EntryAmount)
    {   
        PXTextFromAdressA(&pxText, PXFileNone, sizeof(PXFileNone), sizeof(PXFileNone));

        PXTextDrawInfoRowNext(pxDrawInfo);
      
        PXWindowDrawText(pxWindow, pxDrawInfo);
    }

    for(PXSize i = 0; i < pxDirectorySearchCache->FilePathCache.EntryAmount; ++i)
    {
        PXI32U id = i+100;

        PXListDynamicGet
        (
            &pxFileDirectory->DirectorySearchCache.FilePathCache,
            &id,
            &pxText.A,
            &pxText.SizeUsed
        );

        PXFileEntry* pxFileEntry = PXListItemAtIndexGet(&pxFileDirectory->DirectorySearchCache.EntryList, i);

        PXFileDirectoryEntryDraw(pxFileDirectory, pxWindow, pxFileEntry, pxDrawInfo);

        PXTextDrawInfoRowNext(pxDrawInfo);
    }  

    return PXResultOK;
}

PXResult PXAPI PXFileDirectoryCreate(PXFileDirectory** pxFileDirectoryREF, PXFileDirectoryCreateInfo PXREF pxFileDirectoryCreateInfo)
{
    PXFileDirectory* pxFileDirectory = PXNull;

    pxFileDirectoryCreateInfo->Info.Static = &PXFileDirectoryInfoStatic;
    pxFileDirectoryCreateInfo->Info.Dynamic = &PXFileDirectoryInfoDynamic;
    PXResult pxResult = PXECSCreate(pxFileDirectoryREF, pxFileDirectoryCreateInfo);

    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxFileDirectory = *pxFileDirectoryREF;

#if OSWindows
    pxFileDirectoryCreateInfo->Window.WindowClassName.A = WC_STATIC;
   
#endif

    pxFileDirectoryCreateInfo->Window.EventList.CallBackOwner = pxFileDirectory;
    pxFileDirectoryCreateInfo->Window.EventList.CallBackDraw = PXFileDirectoryDraw;

    PXWindowCreate(&pxFileDirectory->WindowBase, &pxFileDirectoryCreateInfo->Window);

    pxFileDirectory->IsDirty = PXTrue;


    PXWindow PXREF mainWindow = pxFileDirectory->WindowBase;

    // Create buttons
    {
        PXButtonCreateInfo pxButtonCreateInfo;
        PXClear(PXButtonCreateInfo, &pxButtonCreateInfo);
        pxButtonCreateInfo.Window.WindowParent = mainWindow;
        PXButtonCreate(&pxFileDirectory->ButtonFileBrowser, &pxButtonCreateInfo);

        PXClear(PXButtonCreateInfo, &pxButtonCreateInfo);
        pxButtonCreateInfo.Window.WindowParent = mainWindow;
        PXButtonCreate(&pxFileDirectory->ButtonFolderUp, &pxButtonCreateInfo);

        PXClear(PXButtonCreateInfo, &pxButtonCreateInfo);
        pxButtonCreateInfo.Window.WindowParent = mainWindow;
        PXButtonCreate(&pxFileDirectory->ButtonScrollUp, &pxButtonCreateInfo);

        PXClear(PXButtonCreateInfo, &pxButtonCreateInfo);
        pxButtonCreateInfo.Window.WindowParent = mainWindow;
        PXButtonCreate(&pxFileDirectory->ButtonScrollDown, &pxButtonCreateInfo);

        PXClear(PXButtonCreateInfo, &pxButtonCreateInfo);
        pxButtonCreateInfo.Window.WindowParent = mainWindow;
        PXButtonCreate(&pxFileDirectory->ButtonListRefresh, &pxButtonCreateInfo);
    }







   // pxWindow->Info.Behaviour |= PXECSInfoUseByEngine;


    //pxWindowCreateInfo->WindowClassName.A = WC_NATIVEFONTCTL;
    // NFS_USEFONTASSOC
    //pxWindowCreateInfo->WindowClassName.A = WC_PAGESCROLLER;
    //pxWindowCreateInfo->StyleFlags |= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    return PXResultOK;
}

PXResult PXAPI PXFileDirectoryDestroy(PXFileDirectory PXREF pxFileDirectory)
{
    return PXResultOK;
}