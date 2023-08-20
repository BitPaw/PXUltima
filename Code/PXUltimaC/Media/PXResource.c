#include "PXResource.h"

#include <Media/3DS/PX3DS.h>
#include <Media/AAC/PXAAC.h>
#include <Media/AVI/PXAVI.h>
#include <Media/Bitmap/PXBitmap.h>
#include <Media/FilmBox/PXFilmBox.h>
#include <Media/FLAC/PXFLAC.h>
#include <Media/GIF/PXGIF.h>
#include <Media/JPEG/PXJPEG.h>
#include <Media/MIDI/PXMIDI.h>
#include <Media/MP3/PXMP3.h>
#include <Media/MP4/PXMP4.h>
#include <Media/OGG/PXOGG.h>
#include <Media/PLY/PXPLY.h>
#include <Media/PNG/PXPNG.h>
#include <Media/SpriteFont/PXSpriteFont.h>
#include <Media/STEP/PXSTEP.h>
#include <Media/STL/PXSTL.h>
#include <Media/SVG/PXSVG.h>
#include <Media/WEBM/PXWEBM.h>
#include <Media/WEBP/PXWEBP.h>
#include <Media/Wavefront/PXWavefront.h>
#include <Media/PDF/PXPDF.h>
#include <Media/TTF/PXTTF.h>
#include <Media/VRML/PXVRML.h>
#include <Media/WAV/PXWAV.h>
#include <Media/WMA/PXWMA.h>
#include <Media/XML/PXXML.h>
#include <Media/TIFF/PXTIFF.h>
#include <Media/TGA/PXTGA.h>
#include <Media/YAML/PXYAML.h>

PXInt8U PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch (pxVertexBufferFormat)
    {
        case PXVertexBufferFormatT2F_XYZ: return 2u + 3u;
        case PXVertexBufferFormatT2F_N3F_XYZ: return 2u + 3u + 3u;
        case PXVertexBufferFormatXYZ: return 3u;

        default:
            return 0u;
    }
}

void PXVertexStructureConstruct(PXVertexStructure* const pxVertexStructure)
{
    PXObjectClear(PXVertexStructure, pxVertexStructure);

    PXMatrix4x4FIdentity(&pxVertexStructure->ModelMatrix);
    
    PXMarginSet(&pxVertexStructure->Margin, 1, 1, 1, 1);
}

void PXVertexStructureDestruct(PXVertexStructure* const pxVertexStructure)
{
    
}

PXFontPageCharacter* PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID)
{
    PXFontPageCharacter* lastMatch = PXNull;
    PXBool match = PXFalse;

    if (!pxFontPage)
    {
        return PXNull;
    }

    for (PXSize i = 0; i < pxFontPage->CharacteListSize && !match; ++i)
    {
        const PXFontPageCharacter* const currentCharacter = &pxFontPage->CharacteList[i];

        match = currentCharacter->ID == characterID;

        lastMatch = (PXFontPageCharacter*)((PXSize)match * (PXSize)currentCharacter);
    }

    return lastMatch;
}

PXActionResult PXResourceLoad(void* resource, const PXText* const filePath)
{
    printf("[PX] Resource load <%s>\n", filePath->TextA);

    PXFile pxFile;

    // Loading file
    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.Text = *filePath;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        const PXActionResult fileLoadingResult = PXFileOpenFromPath(&pxFile, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(fileLoadingResult);
    }

    // Try to load assumed format
    {
        if (pxFile.TypeInfo.FormatExpected == PXFileFormatUnkown)
        {
            return PXActionRefusedNotSupported;
        }

        if (pxFile.TypeInfo.ResourceLoadFunction == PXNull)
        {
            return PXActionRefusedNotImplemented;
        }

        const PXActionResult fileParsingResult = pxFile.TypeInfo.ResourceLoadFunction(resource, &pxFile);

        PXActionReturnOnSuccess(fileParsingResult); // Exit if this has worked first-try

        return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }

#if 0
    {

        PXActionResult fileGuessResult = PXActionInvalid;
        unsigned int fileFormatID = 1;

        do
        {
            const PXFileFormat imageFileFormat = fileGuessResult + fileFormatID;

            fileGuessResult = PXImageLoadD(image, &pxFile, imageFileFormat);

            fileFormatID++;
        }
        while (fileGuessResult == PXActionRefusedInvalidHeaderSignature);

        PXFileDestruct(&pxFile);

        return fileGuessResult;
    }
#endif
}

PXActionResult PXResourceLoadA(void* resource, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextUnkownLength);

    return PXResourceLoad(resource, &pxText);
}


PXActionResult PXFileTypeInfoProbe(PXFileTypeInfo* const pxFileTypeInfo, const PXText* const pxText)
{
    // Probe for file extension
    {
        pxFileTypeInfo->FormatExpected = PXFilePathExtensionDetectTry(pxText);
    }


    switch (pxFileTypeInfo->FormatExpected)
    {
        case PXFileFormatA3DS:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVertexStructure;
            pxFileTypeInfo->ResourceLoadFunction = PXAutodesk3DSLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXAutodesk3DSSafeFromFile;
            break;

        case PXFileFormatAAC:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXAACLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXAACSaveToFile;
            break;

        case PXFileFormatAVI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoadFunction = PXAVILoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXAVISaveToFile;
            break;

        case PXFileFormatBitMap:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXBitmapLoadToImage;
            pxFileTypeInfo->ResourceSaveFunction = PXBitmapSaveFromImage;
            break;

        case PXFileFormatC:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatCSharp:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatCSS:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatCPP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatWindowsDynamicLinkedLibrary:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeLibrary;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatLinuxExecutableAndLinkable:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeExecutable;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatEML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatWindowsExecutable:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeExecutable;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatFilmBox:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVertexStructure;
            pxFileTypeInfo->ResourceLoadFunction = PXFilmBoxLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXFilmBoxSaveToFile;
            break;

        case PXFileFormatFLAC:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXFLACLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXFLACSaveToFile;
            break;

        case PXFileFormatSpriteFont:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeFont;
            pxFileTypeInfo->ResourceLoadFunction = PXSpriteFontLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXSpriteFontSaveToFile;
            break;

        case PXFileFormatGIF:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatHTML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatINI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatJPEG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXJPEGLoadFromImage;
            pxFileTypeInfo->ResourceSaveFunction = PXJPEGSaveToImage;
            break;

        case PXFileFormatJSON:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeExecutable;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatM4A:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeExecutable;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatMIDI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXMIDILoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXMIDISaveToFile;
            break;

        case PXFileFormatMP3:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXMP3LoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXMP3SaveToFile;
            break;

        case PXFileFormatMP4:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;

        case PXFileFormatMSI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeUnkown;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;

        case PXFileFormatMTL:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeExecutable;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatWavefront:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVertexStructure;
            pxFileTypeInfo->ResourceLoadFunction = PXWavefrontLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXWavefrontSaveFromFile;
            break;

        case PXFileFormatOGG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXOGGLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXOGGSaveToFile;
            break;

        case PXFileFormatPDF:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXPDFCompile;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatPHP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatPLY:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXPLYParseFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatPNG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXPNGLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXPNGSaveToFile;
            break;

        case PXFileFormatQOI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatSTEP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXSTEPParse;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatSTL:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXSTLParseFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatSVG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXSVGLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXSVGSaveToFile;
            break;

        case PXFileFormatTGA:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXTGALoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXTGASaveToFile;
            break;

        case PXFileFormatTagImage:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXTIFFLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXTIFFSaveToFile;
            break;

        case PXFileFormatTrueTypeFont:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeFont;
            pxFileTypeInfo->ResourceLoadFunction = PXTTFLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXTTFSaveToFile;
            break;

        case PXFileFormatVRML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVertexStructure;
            pxFileTypeInfo->ResourceLoadFunction = PXVRMLLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXVRMLSaveToFile;
            break;

        case PXFileFormatWave:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXWaveLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXWaveSaveToFile;
            break;

        case PXFileFormatWEBM:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXWEBPParse;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatWEBP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXWEBPParse;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatWMA:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXWMAParse;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatXML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXXMLFileCompile;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatYAML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXYAMLLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;


        default:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeUnkown;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;
    }
}