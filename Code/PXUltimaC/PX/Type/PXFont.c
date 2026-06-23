#include "PXFont.h"
#include <PX/Engine/PXResource.h>
#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

#if OSUnix
#include <fontconfig/fontconfig.h> // More modern than X11
#elif OSWindows
#endif


const char PXFontText[] = "Font";
const PXI8U PXFontTextLength = sizeof(PXFontText);
const PXECSRegisterInfoStatic PXFontRegisterInfoStatic =
{
    {sizeof(PXFontTextLength), sizeof(PXFontTextLength), PXFontText, TextFormatASCII},
    sizeof(PXFont),
    __alignof(PXFont),
    PXECSTypeResource,
    PXFontCreate,
    PXNull,
    PXNull
};
PXECSRegisterInfoDynamic PXFontRegisterInfoDynamic;



#if OSWindows
int CALLBACK PXFontEnumCallBack(const LOGFONT* lpelfe, const TEXTMETRIC* lpntme, DWORD FontType, LPARAM lParam)
{
    PXFont* pxFont;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXFontText,
        "Fetch",
        "detected : %s",
        lpelfe->lfFaceName
    );
#endif

    return PXTrue;
}
#endif

void PXAPI PXFontRegisterToECS(PXECSRegisterInfo PXREF pxECSRegisterInfo)
{
    pxECSRegisterInfo->InfoStatic = &PXFontRegisterInfoStatic;
    pxECSRegisterInfo->InfoDynamic = &PXFontRegisterInfoDynamic;
}

PXFontPage* PXAPI PXFontPageGet(PXFont PXREF pxFont, const PXSize index)
{
    if(!pxFont)
    {
        return PXNull; // PXResultRefusedParameterNull
    }

    const PXBool isInRange = index <= pxFont->PageListAmount;

    if(!isInRange)
    {
        return PXNull;
    }


    // Special case
    if(pxFont->PageListAmount == 1)
    {
        return &pxFont->PagePrime;
    }

    return &pxFont->PageList[index];
}

PXResult PXAPI PXFontPageCreate(PXFont PXREF pxFont, const PXSize amount)
{
    if(amount > 1)
    {
        pxFont->PageListAmount = amount - 1;
        pxFont->PageList = PXMemoryHeapCallocT(PXFontPage, pxFont->PageListAmount);
    }
    else
    {
        pxFont->PageListAmount = amount;
    }

    return PXResultOK;
}

PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage PXREF pxFontPage, const PXI32U characterID)
{
    PXFontPageCharacter* lastMatch = PXNull;
    PXBool match = PXFalse;

    if(!pxFontPage)
    {
        return PXNull;
    }

    for(PXSize i = 0; i < pxFontPage->CharacteListEntrys && !match; ++i)
    {
        const PXFontPageCharacter PXREF currentCharacter = &pxFontPage->CharacteList[i];

        match = currentCharacter->ID == characterID;

        lastMatch = (PXFontPageCharacter*)((PXSize)match * (PXSize)currentCharacter);
    }

    return lastMatch;
}

PXResult PXAPI PXFontListFetch()
{
#if OSUnix

    //-----------------------------------------------------
    // Modern way, old path is not maintained!
    FcInit();

    FcPattern* pat = FcPatternCreate();
    FcObjectSet* os = FcObjectSetBuild(FC_FAMILY, FC_STYLE, FC_FILE, NULL);
    FcFontSet* fs = FcFontList(NULL, pat, os);

    for(PXSize i = 0; i < fs->nfont; i++) 
    {
        FcPattern* font = fs->fonts[i];

        FcChar8* file;
        FcChar8* family;

        PXBool isOK =
            FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch &&
            FcPatternGetString(font, FC_FAMILY, 0, &family) == FcResultMatch;

        if(!isOK)
        {
            continue;
        }

        printf("Font: %s (%s)\n", family, file);
    }

    FcFontSetDestroy(fs);
    FcObjectSetDestroy(os);
    FcPatternDestroy(pat);
    FcFini();
    //-----------------------------------------------------







    //-----------------------------------------------------
    // Legacy X-System pathway
    //-----------------------------------------------------
    Display* display = 0;
    char* searchPattern = PXNull;
    int fontNameListSizeMax = 0;
    int fontNameListSizeCurrnet = 0;
    XFontStruct* info = PXNull;
    char** fontNameList = XListFontsWithInfo
    (
        display,
        searchPattern,
        fontNameListSizeMax,
        &fontNameListSizeCurrnet,
        &info
    );
    PXBool isOK = fontNameList > 0;

    for(PXSize i = 0; i < fontNameListSizeCurrnet; ++i)
    {
        const char* fontName = names[i];
        XFontStruct* f = &info[i];
    }

    XFreeFontNames(fontNameList);
    //-----------------------------------------------------


    // Can also have additional info
    // char **XListFontsWithInfo(Display *display, char *pattern, int maxnames, int *count_return, XFontStruct


    /*
     TODO: implement

    char **XListFonts(display, pattern, maxnames, actual_count_return)
      Display *display;
      char *pattern;
      int maxnames;
      int *actual_count_return;
*/

#elif OSWindows
    const HDC defaultDeviceContext = GetDC(NULL);

    LOGFONT logfont = { 0 };
    logfont.lfCharSet = DEFAULT_CHARSET;

    int resultEnum = EnumFontFamiliesExA(defaultDeviceContext, &logfont, PXFontEnumCallBack, 0, 0);

    int resultRelease = ReleaseDC(PXNull, defaultDeviceContext);
#else
    return PXActionRefusedNotSupportedByLibrary;
#endif
}

PXResult PXAPI PXFontCreate(PXFont** pxFontREF, PXFontCreateInfo PXREF pxFontCreateInfo)
{
    if(!(pxFontREF && pxFontCreateInfo))
    {
        return PXResultRefusedParameterNull;
    }

    PXFont* pxFont = PXNull;
    PXText* fontName = &pxFontCreateInfo->RegisteredName;

    pxFontCreateInfo->Info.Static = &PXFontRegisterInfoStatic;
    pxFontCreateInfo->Info.Dynamic = &PXFontRegisterInfoDynamic;
    PXResult pxResult = PXECSCreate(pxFontREF, pxFontCreateInfo);
    
    if(PXResultOK != pxResult)
    {
        return pxResult;
    }

    pxFont = *pxFontREF;

    // if this is a system font, we dont load it from disk but from the system, duh.
    if(fontName->Data4)
    {
#if OSUnix
        // "9x15" <--- linux font? Sometimes not found
        pxFont->Info.Handle.FontHandle = XLoadQueryFont //  XFontStruct*
        (
            pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle,
            name
        );

#elif OSWindows

        // "Consolas"
        int height = 0;
        int width = 0;
        int escapement = 0;
        int orientation = 0;
        int weight = FW_HEAVY;
        DWORD italic = (PXFontItalic & pxFont->Info.FlagListSettings) > 0;
        DWORD underline = (PXFontUnderline & pxFont->Info.FlagListSettings) > 0;
        DWORD strikeOut = (PXFontStrikeOut & pxFont->Info.FlagListSettings) > 0;
        DWORD charSet = DEFAULT_CHARSET;
        DWORD outPrecision = OUT_DEFAULT_PRECIS;
        DWORD clipPrecision = CLIP_DEFAULT_PRECIS;
        DWORD quality = (PXFontAntialiased & pxFont->Info.FlagListSettings) > 0 ? ANTIALIASED_QUALITY : NONANTIALIASED_QUALITY;
        DWORD pitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

        switch(fontName->Format)
        {
            case TextFormatASCII:
            {
                pxFont->FontHandle = CreateFontA
                (
                    height,
                    width,
                    escapement,
                    orientation,
                    weight,
                    italic,
                    underline,
                    strikeOut,
                    charSet,
                    outPrecision,
                    clipPrecision,
                    quality,
                    pitchAndFamily,
                    fontName->A
                );
                break;
            }
            case TextFormatUNICODE:
            {
                pxFont->FontHandle = CreateFontW
                (
                    height,
                    width,
                    escapement,
                    orientation,
                    weight,
                    italic,
                    underline,
                    strikeOut,
                    charSet,
                    outPrecision,
                    clipPrecision,
                    quality,
                    pitchAndFamily,
                    fontName->W
                );
                break;
            }
            default:
            {
                // ToDO: ??
                return PXResultRefusedParameterInvalid;
            }             
        }

        PXResult pxResult = PXErrorCurrent(PXNull != pxFont->FontHandle);

#endif

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXFontText,
            "Load",
            "PXID:<%4i>, Name:<%s>",
            pxFont->Info.ID,
            fontName->A
        );
#endif
    }
    else
    {
        // We want to load by file

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXFontText,
            "Load",
            "PXID:<%4i>, <%s>, Path:<%s>, from file",
            pxFont->Info.ID,
            fontName->A,
            PXNull // FilePath            
        );
#endif

        // Load font
        {
            const PXResult loadResult = PXECSLoad(pxFont, &pxFontCreateInfo->Info);

            if(PXResultOK != loadResult)
                return loadResult;
        }
    }

    return PXResultOK;
}

PXResult PXAPI PXFontRelease(PXFont PXREF pxFont)
{
#if OSUnix
    const int resultID = XFreeFont(pxNativDraw->GUISystem->DisplayCurrent.DisplayHandle, 0);
#elif OSWindows

#endif

    return PXActionRefusedNotImplemented;
}