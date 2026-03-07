#include "PXFileDirectory.h"
#include <PX/OS/Console/PXConsole.h>
#include <CommCtrl.h>

#include <PX/Engine/ECS/Resource/Icon/PXIcon.h>
#include <PX/Engine/PXGUI.h>




const char PXFileDirectoryName[] = "FileDirectory";
const PXI8U PXFileDirectoryNameLength = sizeof(PXFileDirectoryName);
const PXECSRegisterInfoStatic PXFileDirectoryInfoStatic =
{
    {sizeof(PXFileDirectoryName), sizeof(PXFileDirectoryName), PXFileDirectoryName, TextFormatASCII},
    sizeof(PXFileDirectory),
    __alignof(PXFileDirectory),
    PXECSTypeEntity
};
PXECSRegisterInfoDynamic PXFileDirectoryInfoDynamic;

PXResult PXAPI PXFileDirectoryRegisterToECS()
{
    PXECSRegister(&PXFileDirectoryInfoStatic, &PXFileDirectoryInfoDynamic);

    return PXResultOK;
}

const char PXFileNone[] = "<**No files**>";
const PXI8U PXFileNoneLength = sizeof(PXFileNone);

const char PXLocalPath[] = "./";
const PXI8U PXLocalPathLength = sizeof(PXLocalPath);


void PXAPI PXDrawEntry(PXFileDirectory PXREF pxFileDirectory, PXWindow PXREF pxWindow, PXFileEntry PXREF pxFileEntry, PXTextDrawInfo PXREF pxTextDrawInfo)
{
    PXText PXREF pxText = pxTextDrawInfo->Text;

    //PXClear(PXTextDrawInfo, &pxTextDrawInfo);

       //PXFileEntry PXREF pxFileEntry = PXListItemAtIndexGetT(PXFileEntry, &pxDirectorySearchCache->EntryList, i);

#if 1
       // PXIcon pxIcon;

       // const PXResult iconResult = PXIconGetViaFilePath(&pxIcon, &pxFileEntry->FilePath);

       // PXI32S rowY = (pxDrawInfo->RectangleXYWH.Height * (i));

    PXRectangleXYWHI32 old = pxTextDrawInfo->WindowDrawInfo->RectangleXYWH;

    pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.X = pxTextDrawInfo->Position.X;
    pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.Y = pxTextDrawInfo->Position.Y;
    pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.Width = 16;
    pxTextDrawInfo->WindowDrawInfo->RectangleXYWH.Height = 16;


    // Let's try to get the icon?



    // Draw icon
    PXWindowDrawIcon
    (
        pxWindow,
        pxTextDrawInfo->WindowDrawInfo,
        &pxFileEntry->Icon
    );

    pxTextDrawInfo->WindowDrawInfo->RectangleXYWH = old;
#endif

    int x = PXFileElementInfoTypeFile;

    pxTextDrawInfo->Position.X += 20;

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

    pxTextDrawInfo->Position.X -= 20;


    // pxWindowDrawInfoSub.RectangleXYWH.X = pxDrawInfo->RectangleXYWH.X + iconSize + 8;
    // pxWindowDrawInfoSub.RectangleXYWH.Y = pxDrawInfo->RectangleXYWH.Y + rowY;


    // pxWindowDrawInfoSub.Content = pxFileEntry->FilePath;
    // pxWindowDrawInfoSub.Behaviour = PXWindowAllignLeft;

     /*
     PXTextDrawInfo pxTextDrawInfo;
     pxTextDrawInfo.WindowDrawInfo = pxDrawInfo;
     pxTextDrawInfo.Text = &pxWindowDrawInfoSub;
     pxTextDrawInfo.Behaviour = PXWindowAllignLeft | PXWindowAllignTop;
     pxTextDrawInfo.X = 0;
     pxTextDrawInfo.Y = 0;

             PXWindowDrawText(pxWindow, &pxWindowDrawInfoSub, PXNull);
     */
}


PXResult PXAPI PXFileDirectoryDraw(PXFileDirectory PXREF pxFileDirectory, PXDrawInfo PXREF pxDrawInfo)
{
    PXDirectorySearchCache PXREF pxDirectorySearchCache = &pxFileDirectory->DirectorySearchCache;
    
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();
    PXWindow PXREF pxWindow = pxFileDirectory->WindowBase;

#if PXLogEnable
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



    if(pxFileDirectory->IsDirty)
    {
        PXText pxTExt;
        PXTextFromAdressA(&pxTExt, PXLocalPath, PXLocalPathLength, PXLocalPathLength);

        PXDirectorySearch(pxDirectorySearchCache, &pxTExt);

        pxFileDirectory->IsDirty = PXFalse;
    }


#if 0
    PXWindow pxWindowSub;
    PXCopy(PXWindow, pxWindow, &pxWindowSub);
    pxWindowSub.Info.Behaviour = PXECSInfoOK;
    pxWindowSub.Info.Setting = PXWindowAllignTop | PXWindowKeepHeight | PXWindowAllignLeft;
    pxWindowSub.Position.Form.Height = 20;
    pxWindowSub.Position.Form.X = 20;
#endif


#if 0

    PXWindowBrushSet
    (
        pxWindow,
        pxGUITheme->BrushMainPrimary,
        PXGUIDrawModeBack
    );
    PXWindowBrushSet
    (
        pxWindow,
        pxGUITheme->BrushTextWhite,
        PXGUIDrawModeFront
    );
#endif

    const PXSize iconSize = 16;
    char textBuffer[128];

    PXText pxText;
    PXTextFromAdressA(&pxText, textBuffer, sizeof(textBuffer), sizeof(textBuffer));
    PXTextPrint(&pxText, "FilePath");

    PXTextDrawInfo pxTextDrawInfo;
    PXClear(PXTextDrawInfo, &pxTextDrawInfo);
    pxTextDrawInfo.WindowDrawInfo = pxDrawInfo;
    pxTextDrawInfo.Position.Y -= 25;
    pxTextDrawInfo.Position.X = pxDrawInfo->RectangleXYWH.X+5;
    pxTextDrawInfo.Text = &pxText;

    PXWindowDrawText(pxWindow, &pxTextDrawInfo);



    if(0 == pxDirectorySearchCache->FilePathCache.EntryAmount)
    {   
        PXTextFromAdressA(&pxText, PXFileNone, sizeof(PXFileNone), sizeof(PXFileNone));

        pxTextDrawInfo.Position.Y -= 20;
      
        PXWindowDrawText(pxWindow, &pxTextDrawInfo);
    }

    for(PXSize i = 0; i < pxDirectorySearchCache->FilePathCache.EntryAmount; ++i)
    {
        pxTextDrawInfo.Position.Y -= 20;

        PXI32U id = i+100;

        PXListDynamicGet
        (
            &pxFileDirectory->DirectorySearchCache.FilePathCache,
            &id,
            &pxText.A,
            &pxText.SizeUsed
        );

        PXFileEntry* pxFileEntry = PXListItemAtIndexGet(&pxFileDirectory->DirectorySearchCache.EntryList, i);

        PXDrawEntry(pxFileDirectory, pxWindow, pxFileEntry, &pxTextDrawInfo);
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

   // pxWindow->Info.Behaviour |= PXECSInfoUseByEngine;


    //pxWindowCreateInfo->WindowClassName.A = WC_NATIVEFONTCTL;
    // NFS_USEFONTASSOC
    //pxWindowCreateInfo->WindowClassName.A = WC_PAGESCROLLER;
    //pxWindowCreateInfo->StyleFlags |= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    return PXResultOK;
}