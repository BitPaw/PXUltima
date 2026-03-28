#include "PXIcon.h"

#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/Graphic/OpenGL/PXOpenGL.h>

#if OSWindows
#include <Windows.h>
#include <shellapi.h>
#include <shlwapi.h>
#endif

const char PXIconText[] = "Icon";
const PXI8U PXIconTextLength = sizeof(PXIconText);
const PXECSRegisterInfoStatic PXIconRegisterInfoStatic =
{
    {sizeof(PXIconTextLength), sizeof(PXIconTextLength), PXIconText, TextFormatASCII},
    sizeof(PXIcon),
    __alignof(PXIcon),
    PXECSTypeResource,
    PXIconCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXIconRegisterInfoDynamic;

PXResult PXAPI PXIconRegisterToECS()
{
    PXECSRegister(&PXIconRegisterInfoStatic, &PXIconRegisterInfoDynamic);

    return PXResultOK;
}

PXResult PXAPI PXGUIIconLoad(PXIcon PXREF pxIcon)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    pxIcon->IconHandle = LoadIcon(NULL, IDI_EXCLAMATION);
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXIconGetSystem(PXIcon PXREF pxIcon, const int iconID)
{
    if(!pxIcon)
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    SHSTOCKICONINFO stockIconInfo;
    PXClear(SHSTOCKICONINFO, &stockIconInfo);
    stockIconInfo.cbSize = sizeof(SHSTOCKICONINFO);

    /*
    
Folder up	SIID_UP
Back	SIID_BACK
Forward	SIID_FORWARD
Refresh	SIID_REFRESH
Folder	SIID_FOLDER
Open folder	SIID_FOLDEROPEN
Drive	SIID_DRIVEFIXED
Desktop	SIID_DESKTOPPC
    */
    const HRESULT resultFile = SHGetStockIconInfo
    (
        SIID_DOCNOASSOC,
        SHGSI_ICON,
        &stockIconInfo
    ); // Windows Vista, Shell32.dll, shellapi.h
    const PXResult pxResult = PXErrorFromHRESULT(resultFile);

    pxIcon->IconHandle = PXNull;



    /*
    * // Note: This is not recommended, as IDs are not stable across versions!
    HMODULE hShell = LoadLibraryA("shell32.dll");
    if(!hShell)
        return NULL;

    return (HICON)LoadImageA(
        hShell,
        MAKEINTRESOURCEA(resource_id),
        IMAGE_ICON,
        size,
        size,
        LR_SHARED
    );
    */







    return pxResult;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXIconGetEmbedded(PXIcon PXREF pxIcon)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXIconText,
        "Load",
        "PXID:<%4i>, %s",
        pxIcon->Info.ID,
        "-"
    );
#endif

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    HINSTANCE instanceHandle = NULL; // predefined system icon if NULL

    switch(TextFormatASCII)
    {
        case TextFormatASCII:
        {
            pxIcon->IconHandle = LoadIconA
            (
                instanceHandle,
                IDI_EXCLAMATION
            );
            break;
        }
        case TextFormatUNICODE:
        {
            pxIcon->IconHandle = LoadIconW
            (
                instanceHandle,
                IDI_EXCLAMATION
            );
            break;
        }
        default:
        {
            return PXResultRefusedParameterInvalid;
        }
    }

    return PXResultOK;
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

PXResult PXAPI PXIconFromTexture(PXIcon PXREF pxIcon, PXTexture PXREF pxTexture)
{
    if(!(pxIcon && pxTexture))
    {
        return PXResultRefusedParameterNull;
    }

    if(!pxTexture->PixelData.Adress)
    {
        return PXActionRefusedObjectInternalDataMissing;
    }

#if OSUnix

#elif OSWindows
    BYTE* bitmapData[2];
    HBITMAP bitmapHandle[2];

    BITMAPINFO bitmapInfo[2];
    PXClearList(BITMAPINFO, bitmapInfo, 2);
    bitmapInfo[0].bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo[0].bmiHeader.biWidth = pxTexture->Width;
    bitmapInfo[0].bmiHeader.biHeight = -pxTexture->Height; // Top-down DIB 
    bitmapInfo[0].bmiHeader.biPlanes = 1;
    bitmapInfo[0].bmiHeader.biBitCount = 24;// PXColorFormatBitsPerPixel(pxIconCreateInfo->IconImage->Format);
    bitmapInfo[0].bmiHeader.biCompression = BI_RGB;

    bitmapInfo[1].bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bitmapInfo[1].bmiHeader.biWidth = pxTexture->Width;
    bitmapInfo[1].bmiHeader.biHeight = -pxTexture->Height; // Top-down DIB
    bitmapInfo[1].bmiHeader.biPlanes = 1;
    bitmapInfo[1].bmiHeader.biBitCount = 1; // Monochrome bitmap 
    bitmapInfo[1].bmiHeader.biCompression = BI_RGB;

    for(PXSize i = 0; i < 2; ++i)
    {
        bitmapHandle[i] = CreateDIBSection(NULL, &bitmapInfo[i], DIB_RGB_COLORS, &bitmapData[i], NULL, 0);
    }

    BYTE* pixelDataBGR = (BYTE*)bitmapData[0];
    BYTE* pxMaskAND = (BYTE*)bitmapData[1];
    BYTE* pxMaskXOR = &bitmapData[(pxTexture->Width * pxTexture->Height) / 8];

    const PXSize maskXOROffset = (pxTexture->Height - 1) * ((pxTexture->Width + 7) / 8) + ((pxTexture->Width - 1) / 8) + 1;

    for(PXSize y = 0; y < pxTexture->Height; y++)
    {
        for(PXSize x = 0; x < pxTexture->Width; x++)
        {
            const PXSize indexInsret = (x + y * pxTexture->Width) * 3;

            const PXSize sourceX = x;// +pxIconCreateInfo->OffsetX;
            const PXSize sourceY = y;// + pxIconCreateInfo->OffsetY;
            const PXSize indexSource = PXTexturePixelPosition(pxTexture, sourceX, sourceY);
            PXByte* pixelDataSource = pxTexture->PixelData.Data;
            const PXColorRGBAI8 PXREF source = (PXColorRGBAI8*)&pixelDataSource[indexSource];

            BYTE PXREF insert = &pixelDataBGR[indexInsret];


            insert[0] = source->Blue;
            insert[1] = source->Green;
            insert[2] = source->Red;
            // There is a *2 because the mask index is only used every 2nd line??
            const PXSize maskIndex = (y * ((pxTexture->Width + 7) / 8) * 2) + (x / 8);
            const PXBool isTransparent = source->Alpha == 0;
            const PXI8U bitData = 1 << (7 - (x % 8));

            if(isTransparent)
            {
                pxMaskAND[maskIndex] &= ~bitData;
                pxMaskXOR[maskIndex] |= bitData;
            }
            else
            {
                pxMaskAND[maskIndex] |= bitData;
                pxMaskXOR[maskIndex] &= ~bitData;
            }
        }
    }


    if(0) // Use binaryicon
    {
        pxIcon->IconHandle = CreateIcon
        (
            PXNull,
            pxTexture->Width,
            pxTexture->Height,
            1,
            1,
            pxMaskAND,
            pxMaskXOR
        );
    }
    else
    {
        // memset(pxMaskAND, 0x40, 0x1000);

        ICONINFO iconInfo;
        iconInfo.fIcon = TRUE;
        iconInfo.xHotspot = 0;
        iconInfo.yHotspot = 0;
        iconInfo.hbmMask = bitmapHandle[1]; // mask pxMaskXOR is following
        iconInfo.hbmColor = bitmapHandle[0];

        pxIcon->IconHandle = CreateIconIndirect(&iconInfo);

        // iconHandle = CreateIcon(PXNull, pxIconCreateInfo->Width, pxIconCreateInfo->Height, 1, 1, pxMaskAND, pxMaskXOR);
    }
#endif

    return PXResultOK;
}

PXResult PXAPI PXIconCreate(PXIcon** pxIcon, PXIconCreateInfo PXREF pxIconCreateInfo)
{
    // Check if texture is present
    if(!pxIconCreateInfo->IconImage)
    {
        PXECSCreateInfo pxResourceCreateInfoSub;

        PXClear(PXECSCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.ObjectReference = (PXECSInfo**)&pxIconCreateInfo->IconImage;
        pxResourceCreateInfoSub.ObjectAmount = 1;
        pxResourceCreateInfoSub.FilePath = pxIconCreateInfo->Info.FilePath;
      //  pxResourceCreateInfoSub.Type = PXResourceTypeTexture;

     //   //PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXIconText,
        "Create",
        "ID:%i - WHXY : %3i,%3i,%3i,%3i",
        0,//pxIcon->Info.ID,
        pxIconCreateInfo->Width,
        pxIconCreateInfo->Height,
        pxIconCreateInfo->OffsetX,
        pxIconCreateInfo->OffsetY
    );
#endif

    //PXNativDrawIconFromImage(PXNativDrawInstantance(), pxIcon, pxIconCreateInfo->IconImage);

    return PXResultOK;
}

PXResult PXAPI PXIconGetViaFilePath(PXIcon PXREF pxIcon, const PXText PXREF filePath)
{
    if(!(pxIcon && filePath))
    {
        return PXResultRefusedParameterNull;
    }

#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows

    union
    {
        SHFILEINFOA A;
        SHFILEINFOW W;
    }
    shFileInfo;
    const PXI8U shFileInfoSize = sizeof(shFileInfo);
    const UINT flags = SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_SMALLICON | SHGFI_LARGEICON;
    PXBool fetch = 0;

    PXMemoryClear(&shFileInfo, shFileInfoSize);

    switch(filePath->Format)
    {
        case TextFormatASCII:
        {
            fetch = SHGetFileInfoA
            (
                filePath->A,
                0,
                &shFileInfo.A,
                sizeof(SHFILEINFOA),
                flags
            ); // Windows 2000, Shell32.dll 4.0, shellapi.h
            pxIcon->IconHandle = shFileInfo.A.hIcon;
            break;
        }
        case TextFormatUNICODE:
        {
            fetch = SHGetFileInfoW
            (
                filePath->W,
                0,
                &shFileInfo.W,
                sizeof(SHFILEINFOW),
                flags
            ); // Windows 2000, Shell32.dll 4.0, shellapi.h
            pxIcon->IconHandle = shFileInfo.W.hIcon;
            break;
        }
        default:
            break;
    }

    int iconWidth = GetSystemMetrics(SM_CXICON);
    int iconHeight = GetSystemMetrics(SM_CYICON);
    PXSize totalSize = 0;

    // Step 2 - Convert handle to BITMAP
    {
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = iconWidth;
        bmi.bmiHeader.biHeight = -iconHeight; // top-down
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        totalSize = (bmi.bmiHeader.biBitCount / 8) * iconWidth * iconHeight;

        PXByte* pixels = NULL;
        HDC hdc = CreateCompatibleDC(NULL);
        HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &pixels, NULL, 0);
        SelectObject(hdc, hBmp);

        // Draw icon into the bitmap
        DrawIconEx
        (
            hdc,
            0,
            0, 
            pxIcon->IconHandle,
            iconWidth,
            iconHeight, 
            0,
            NULL, 
            DI_NORMAL
        );

        // Quicktransform
#if 0
        for(size_t i = 0; i < iconWidth * iconHeight * 4; i += 4)
        {
            PXByte temp = pixels[i + 0];

            pixels[i + 0] = pixels[i + 2];
           // pixels[i + 1] = pixels[i + 1];
            pixels[i + 2] = temp;
           // pixels[i + 0] = pixels[i + 0];
        }
#endif

        // COPY
        {
            PXTextureCreateInfo pxTextureCreateInfo;
            PXClear(PXTextureCreateInfo, &pxTextureCreateInfo);
            pxTextureCreateInfo.Type = PXTextureType2D;
            pxTextureCreateInfo.Format = PXColorFormatRGBAI8;
            pxTextureCreateInfo.Payload = pixels;
            pxTextureCreateInfo.PayloadSize = totalSize;
            pxTextureCreateInfo.T2D.Width = iconWidth;
            pxTextureCreateInfo.T2D.Height = iconHeight;

            PXTextureCreate(&pxIcon->Texture, &pxTextureCreateInfo);
        }

        PXTextureRegisterGL(pxIcon->Texture);
    }

    return PXResultOK;


    /*




    char moduleName[MAX_PATH];
    PXI16U moduleNameSize = 0;
    PXI16U iconID = 0;

    char nameFile[MAX_PATH];
    DWORD nameFileSize = MAX_PATH;
    char objectName[MAX_PATH];
    DWORD dataSize = MAX_PATH;


    // Search file extension
    {
        char fileExtensionKey[MAX_PATH];

        PXTextPrintA(fileExtensionKey, MAX_PATH, ".%s", fileExtension);

        LSTATUS res = RegGetValueA(HKEY_CLASSES_ROOT, fileExtensionKey, NULL, RRF_RT_REG_SZ, NULL, objectName, &dataSize);
        PXErrorCurrent(ERROR_SUCCESS == res);

        PXConsoleWriteF(0, "\n");
    }

    // Search redirect
    {
        // Most programs dont put the default icon under the extension, so we need to follow a redirect

        LSTATUS res = RegGetValueA(HKEY_CLASSES_ROOT, objectName, NULL, RRF_RT_REG_SZ, NULL, nameFile, &nameFileSize);
        PXErrorCurrent(ERROR_SUCCESS == res);

        // Probe for default icon


        char defaultIconPath[MAX_PATH];

        PXTextPrintA(defaultIconPath, MAX_PATH, "%s\\%s", objectName, "DefaultIcon");

        char iconPath[MAX_PATH];
        DWORD iconPathSize = MAX_PATH;

        LSTATUS ressss = RegGetValueA(HKEY_CLASSES_ROOT, defaultIconPath, NULL, RRF_RT_REG_SZ, NULL, iconPath, &iconPathSize);
        PXErrorCurrent(ERROR_SUCCESS == res);


        // Unravel default icon string, format ""xxx", 0"
        const PXBool isFormat = iconPath[0] == '\"';
        PXSize comma = 0;
        char* text = 0;
        PXSize textSize = 0;

        char* iconTextData = 0;
        PXSize iconTextSize = 0;

        if(isFormat)
        {
            comma = PXTextFindLastCharacterA(iconPath, iconPathSize, ',');
            text = &iconPath[1];
            textSize = comma - 2;

            iconTextData = &iconPath[comma+1];
            iconTextSize = comma - textSize;

            iconID = PXTextToIntA(iconTextData, iconTextSize, &iconID);
        }

        moduleNameSize = PXTextCopyA(text, textSize, moduleName, MAX_PATH);


        PXConsoleWriteF(0, "\n");





    }


#if 1
    // SHGetFileInfoA() would make sense to be used but can crash the whole thread on error.
    SHFILEINFOA shFileInfo;
    ZeroMemory(&shFileInfo, sizeof(shFileInfo));


    const PXBool fetch = SHGetFileInfoA(moduleName, 0, &shFileInfo, sizeof(SHFILEINFOA), SHGFI_ICON | SHGFI_DISPLAYNAME | SHGFI_SMALLICON | SHGFI_LARGEICON);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXGUIName,
        "Icon-Fetch",
        "%s",
        shFileInfo.szDisplayName
    );
#endif

    return shFileInfo.hIcon; // Return the handle to the icon

#else
    LoadModule();
#endif


    */


#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}