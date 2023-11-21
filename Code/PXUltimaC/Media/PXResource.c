#include "PXResource.h"

#include <OS/Console/PXConsole.h>
#include <OS/Time/PXStopWatch.h>
#include <OS/Hardware/PXProcessor.h>

#include "Autodesk3DS/PXAutodesk3DS.h"
#include "AAC/PXAAC.h"
#include "AVI/PXAVI.h"
#include "Bitmap/PXBitmap.h"
#include "FilmBox/PXFilmBox.h"
#include "FLAC/PXFLAC.h"
#include "GIF/PXGIF.h"
#include "JPEG/PXJPEG.h"
#include "MIDI/PXMIDI.h"
#include "MP3/PXMP3.h"
#include "MP4/PXMP4.h"
#include "OGG/PXOGG.h"
#include "PLY/PXPLY.h"
#include "PNG/PXPNG.h"
#include "SpriteFont/PXSpriteFont.h"
#include "STEP/PXSTEP.h"
#include "STL/PXSTL.h"
#include "SVG/PXSVG.h"
#include "HEIF/PXHEIF.h"
#include "HTML/PXHTML.h"
#include "JSON/PXJSON.h"
#include "INI/PXINI.h"
#include "WEBM/PXWEBM.h"
#include "WEBP/PXWEBP.h"
#include "Wavefront/PXWavefront.h"
#include "Matroska/PXMatroska.h"
#include "PDF/PXPDF.h"
#include "TTF/PXTTF.h"
#include "VRML/PXVRML.h"
#include "Wave/PXWave.h"
#include "WMA/PXWMA.h"
#include "XML/PXXML.h"
#include "TIFF/PXTIFF.h"
#include "TGA/PXTGA.h"
#include "YAML/PXYAML.h"
#include "BinaryWindows/PXBinaryWindows.h"
#include "BinaryLinux/PXBinaryLinux.h"
#include "CanonRaw3/PXCanonRaw3.h"
#include "C/PXC.h"
#include "CS/PXCS.h"
#include "CPP/PXCPP.h"
#include "CSS/PXCSS.h"

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

PXMaterial* PXMaterialContainerFind(const PXMaterialContainer* const pxMaterialContainer, PXText* const pxMaterialName)
{
    if (!pxMaterialContainer)
    {
        return;
    }

    //for (PXSize materialContainerID = 0; materialContainerID < pxMaterialContainer->MaterialContaierListSize; ++materialContainerID)
    //{
       // const PXMaterialContainer* const pxMaterialContaier = &pxModel->MaterialContaierList[materialContainerID];

        for (PXSize materialID = 0; materialID < pxMaterialContainer->MaterialListSize; ++materialID)
        {
            const PXMaterial* const pxMaterial = &pxMaterialContainer->MaterialList[materialID];

            const PXBool isMatch = PXTextCompareA(pxMaterialName->TextA, pxMaterialName->SizeUsed, pxMaterial->Name, PXTextUnkownLength);

            if (isMatch)
            {
                return pxMaterial;
            }
        }
   // }

    return PXNull;
}

void* PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferDataType pxVertexBufferDataType, const PXSize index)
{
    const PXSize rowEntiry = pxVertexBuffer->VertexDataRowSize * index;

    switch (pxVertexBuffer->Format)
    {
        case PXVertexBufferFormatT2F_XYZ: 
        {
            switch (pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 2];

                case PXVertexBufferDataTypeTexture:
                    return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatT2F_N3F_XYZ:
        {
            switch (pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 2 + 3];

                case PXVertexBufferDataTypeTexture:
                    return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                case PXVertexBufferDataTypeNormal:
                    return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 2];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatXYZ:
        {
            switch (pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }

        default:
            return PXNull;
    }   
}

void PXModelConstruct(PXModel* const pxModel)
{
    PXClear(PXModel, pxModel);

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);
    
    PXRectangleOffsetSet(&pxModel->Margin, 1, 1, 1, 1);
}

void PXModelDestruct(PXModel* const pxModel)
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
            PXLogPrint
            (
                PXLoggingError,
                "Resource",
                "Load refused : Format not detected"
            );

            return PXActionRefusedNotSupported;
        }

        if (pxFile.TypeInfo.ResourceLoadFunction == PXNull)
        {
            PXLogPrint
            (
                PXLoggingError,
                "Resource",
                "Load refused : Not implemented"
            );

            return PXActionRefusedNotImplemented;
        }

        PXInt32U time = PXProcessorTimeReal();

        const PXActionResult fileParsingResult = pxFile.TypeInfo.ResourceLoadFunction(resource, &pxFile);

        PXInt32U timeDelat = PXProcessorTimeReal() - time;

        PXLogPrint
        (
            PXLoggingInfo,
            "Resource",
            "Loaded (%6.3fms, ROPs:%4i) <%s> ",      
            timeDelat/1000.0f,
            pxFile.CounterOperationsRead,
            filePath->TextA
        );

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

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceLoad(resource, &pxText);

    return loadResult;
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
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
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
            pxFileTypeInfo->ResourceLoadFunction = PXBitmapLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXBitmapSaveToFile;
            break;

        case PXFileFormatCanonRaw3:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXCanonRaw3LoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXCanonRaw3SaveToFile;
            break;

        case PXFileFormatC:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoadFunction = PXCLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXCSaveToFile;
            break;

        case PXFileFormatCSharp:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoadFunction = PXCSLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXCSLoadFromFile;
            break;

        case PXFileFormatCSS:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXCSSLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXCSSLoadFromFile;
            break;

        case PXFileFormatCPP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoadFunction = PXCPPLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXCPPLoadFromFile;
            break;

        case PXFileFormatBinaryWindows:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeBinary;
            pxFileTypeInfo->ResourceLoadFunction = PXBinaryWindowsLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXBinaryWindowsSaveToFile;
            break;

        case PXFileFormatBinaryLinux:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeBinary;
            pxFileTypeInfo->ResourceLoadFunction = PXBinaryLinuxLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXBinaryLinuxSaveToFile;
            break;

        case PXFileFormatEML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatFilmBox:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
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
            pxFileTypeInfo->ResourceLoadFunction = PXGIFLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXGIFSaveToFile;
            break;

        case PXFileFormatHighEfficiencyImageFile:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXHEIFLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXHEIFSaveToFile;
            break;

        case PXFileFormatHTML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXHTMLLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXHTMLSaveToFile;
            break;

        case PXFileFormatINI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXINILoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXINISaveToFile;
            break;

        case PXFileFormatJPEG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXJPEGLoadFromImage;
            pxFileTypeInfo->ResourceSaveFunction = PXJPEGSaveToImage;
            break;

        case PXFileFormatJSON:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXJSONLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXJSONSaveToFile;
            break;

        case PXFileFormatM4A:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
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
            pxFileTypeInfo->ResourceType = PXFileResourceTypeInstaller;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;

        case PXFileFormatMTL:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeRenderMaterial;
            pxFileTypeInfo->ResourceLoadFunction = PXMTLLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXMTLSaveToFile;
            break;

        case PXFileFormatWavefront:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoadFunction = PXWavefrontLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXWavefrontSaveFromFile;
            break;

        case PXFileFormatMatroska:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoadFunction = PXMatroskaLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXMatroskaSaveToFile;
            break;            

        case PXFileFormatOGG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXOGGLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXOGGSaveToFile;
            break;

        case PXFileFormatPDF:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeDocument;
            pxFileTypeInfo->ResourceLoadFunction = PXPDFLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXPDFSaveToFile;
            break;

        case PXFileFormatPHP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatPLY:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoadFunction = PXPLYLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXPLYSaveToFile;
            break;

        case PXFileFormatPNG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXPNGLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXPNGSaveToFile;
            break;

        case PXFileFormatQOI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;

        case PXFileFormatSTEP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoadFunction = PXSTEPLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXSTEPSaveToFile;
            break;

        case PXFileFormatSTL:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoadFunction = PXSTLLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXSTLSaveToFile;
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
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
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
            pxFileTypeInfo->ResourceLoadFunction = PXWEBPLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXWEBPLoadSaveToFile;
            break;

        case PXFileFormatWEBP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoadFunction = PXWEBPLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXWEBPLoadSaveToFile;
            break;

        case PXFileFormatWMA:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoadFunction = PXWMALoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXWMASaveToFile;
            break;

        case PXFileFormatXML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXXMLLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXXMLSaveToFile;
            break;

        case PXFileFormatYAML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoadFunction = PXYAMLLoadFromFile;
            pxFileTypeInfo->ResourceSaveFunction = PXYAMLSaveToFile;
            break;


        default:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeUnkown;
            pxFileTypeInfo->ResourceLoadFunction = PXNull;
            pxFileTypeInfo->ResourceSaveFunction = PXNull;
            break;
    }

    return PXActionSuccessful;
}