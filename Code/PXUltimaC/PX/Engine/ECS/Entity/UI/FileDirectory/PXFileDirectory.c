#include "PXFileDirectory.h"
#include <PX/OS/Console/PXConsole.h>
#include <CommCtrl.h>
#include <PX/OS/File/PXDirectory.h>
#include <PX/Engine/ECS/Resource/Icon/PXIcon.h>
#include <PX/Engine/PXGUI.h>

const char PXFileDirectoryName[] = "FileDirectory";
const char PXFileNone[] = "<**No files**>";
const PXI8U PXFileNoneLength = sizeof(PXFileNone);

const char PXLocalPath[] = "./";
const PXI8U PXLocalPathLength = sizeof(PXLocalPath);

PXResult PXAPI PXFileDirectoryDraw(PXFileDirectory PXREF pxFileDirectory, PXWindowDrawInfo PXREF pxWindowDrawInfo)
{
    PXDirectorySearchCache pxDirectorySearchCache;// = (PXDirectorySearchCache*)pxWindow->ExtendedData;
    
    PXGUITheme PXREF pxGUITheme = PXGUIThemeGet();
    PXWindow* pxWindow = pxFileDirectory;

    PXWindowDrawInfo pxWindowDrawInfoSub = *pxWindowDrawInfo;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXFileDirectoryName,
        "Draw",
        "FileDirectoryView"
    );
#endif

    pxWindowDrawInfo->Brush = pxGUITheme->BrushMainPrimary;

    PXWindowDrawRectangle3D(pxWindow, pxWindowDrawInfo);


    PXText pxTExt;
    PXTextFromAdressA(&pxTExt, PXLocalPath, PXLocalPathLength, PXLocalPathLength);

    PXDirectorySearch(&pxDirectorySearchCache, &pxTExt);


#if 0
    PXWindow pxWindowSub;
    PXCopy(PXWindow, pxWindow, &pxWindowSub);
    pxWindowSub.Info.Behaviour = PXECSInfoOK;
    pxWindowSub.Info.Setting = PXWindowAllignTop | PXWindowKeepHeight | PXWindowAllignLeft;
    pxWindowSub.Position.Form.Height = 20;
    pxWindowSub.Position.Form.X = 20;
#endif

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

    const PXSize iconSize = 16;

    if(0 == pxDirectorySearchCache.EntryList.EntryAmountUsed)
    {
        /*
            pxWindow.Position.Form.X + 16,
            pxWindow->Position.Form.Y + 16,
            pxWindow->Position.Form.Width,
            pxWindow->Position.Form.Height,
            buffer,
            bufferSize,
            PXWindowAllignLeft
            */
       // PXTextFromAdressA(&pxWindowDrawInfoSub.Content, PXFileNone, PXFileNoneLength, PXFileNoneLength);

        PXWindowDrawText(pxWindow, &pxWindowDrawInfoSub, &pxTExt);
    }

    for(PXSize i = 0; i < pxDirectorySearchCache.EntryList.EntryAmountUsed; ++i)
    {
        PXFileEntry PXREF pxFileEntry = PXListItemAtIndexGetT(PXFileEntry, &pxDirectorySearchCache.EntryList, i);

        PXIcon pxIcon;

        const PXResult iconResult = PXIconGetViaFilePath(&pxIcon, &pxFileEntry->FilePath);

        PXI32S rowY = (pxWindowDrawInfo->RectangleXYWH.Height * (i));

        PXWindowDrawIcon
        (
            pxWindow,
            &pxIcon,
            2,
            pxWindowDrawInfo->RectangleXYWH.Y + rowY + 2,
            iconSize,
            iconSize
        );

        switch(pxFileEntry->Type)
        {
            default:
            case PXFileElementInfoTypeFile:
            {
               // icon = fileIcon;

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

        pxWindowDrawInfoSub.RectangleXYWH.X = pxWindowDrawInfo->RectangleXYWH.X + iconSize + 8;
        pxWindowDrawInfoSub.RectangleXYWH.Y = pxWindowDrawInfo->RectangleXYWH.Y + rowY;
       // pxWindowDrawInfoSub.Content = pxFileEntry->FilePath;
        pxWindowDrawInfoSub.Behaviour = PXWindowAllignLeft;

        PXWindowDrawText(pxWindow, &pxWindowDrawInfoSub, PXNull);
    }  

    return PXActionSuccessful;
}

PXResult PXAPI PXFileDirectoryCreate(PXFileDirectory** pxFileDirectory, PXWindowCreateInfo PXREF pxWindowCreateInfo)
{
#if OSWindows
    pxWindowCreateInfo->WindowClassName.A = WC_STATIC;
    pxWindowCreateInfo->DrawFunctionEngine = PXFileDirectoryDraw;
#endif

   // pxWindow->Info.Behaviour |= PXECSInfoUseByEngine;


    pxWindowCreateInfo->WindowClassName.A = WC_NATIVEFONTCTL;
    // NFS_USEFONTASSOC
    pxWindowCreateInfo->WindowClassName.A = WC_PAGESCROLLER;
    pxWindowCreateInfo->StyleFlags |= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

    return PXActionSuccessful;
}