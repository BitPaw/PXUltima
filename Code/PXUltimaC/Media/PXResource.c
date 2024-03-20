#include "PXResource.h"

#include <OS/Console/PXConsole.h>
#include <OS/Time/PXStopWatch.h>
#include <OS/Hardware/PXProcessor.h>
#include <Media/PXText.h>

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
#include "PHP/PXPHP.h"
#include "M4A/PXM4A.h"
#include "MP4/PXMP4.h"
#include "MSI/PXMSI.h"
#include "Wave/PXWave.h"
#include "WMA/PXWMA.h"
#include "XML/PXXML.h"
#include "TIFF/PXTIFF.h"
#include "TGA/PXTGA.h"
#include "YAML/PXYAML.h"
#include "BinaryWindows/PXBinaryWindows.h"
#include "BinaryLinux/PXBinaryLinux.h"
#include "CanonRaw3/PXCanonRaw3.h"
#include "RAR/PXRAR.h"
#include "C/PXC.h"
#include "CS/PXCS.h"
#include "CPP/PXCPP.h"
#include "CSS/PXCSS.h"
#include "DDS/PXDDS.h"

PXInt8U PXAPI PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat)
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

void PXAPI PXRectangleOffsetSet(PXRectangleOffset* const pxRectangleOffset, float left, float top, float right, float bottom)
{
    pxRectangleOffset->Left = left;
    pxRectangleOffset->Top = top;
    pxRectangleOffset->Right = right;
    pxRectangleOffset->Bottom = bottom;
}

PXMaterial* PXAPI PXMaterialContainerFind(const PXMaterialContainer* const pxMaterialContainer, struct PXText_* const pxMaterialName)
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

void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferDataType pxVertexBufferDataType, const PXSize index)
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

void PXAPI PXModelConstruct(PXModel* const pxModel)
{
    PXClear(PXModel, pxModel);

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);    
    
    PXRectangleOffsetSet(&pxModel->Margin, 1, 1, 1, 1);
}

void PXAPI PXModelDestruct(PXModel* const pxModel)
{
    
}

PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID)
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

const char* PXAPI PXUIElementTypeToString(const PXUIElementType pxUIElementType)
{
    switch(pxUIElementType)
    {
        case PXUIElementTypeCustom: return "Custom";
        case PXUIElementTypeWindow: return "Window";
        case PXUIElementTypePanel: return "Panel";
        case PXUIElementTypeText: return "Text";
        case PXUIElementTypeButton: return "Button";
        case PXUIElementTypeButtonText: return "ButtonText";
        case PXUIElementTypeImage: return "Image";
        case PXUIElementTypeDropDown: return "DropDown";
        case PXUIElementTypeListBox: return "ListBox";
        case PXUIElementTypeTextEdit: return "TextEdit";
        case PXUIElementTypeRichEdit: return "RichText";
        case PXUIElementTypeScrollBar: return "ScrollBar";
        case PXUIElementTypeTrackBar: return "TrackBar";
        case PXUIElementTypeStatusBar: return "StatusBar";
        case PXUIElementTypeUpDown: return "UpDown";
        case PXUIElementTypeProgressBar: return "";
        case PXUIElementTypeHotKey: return "";
        case PXUIElementTypeCalender: return "";
        case PXUIElementTypeToolTip: return "";
        case PXUIElementTypeAnimate: return "";
        case PXUIElementTypeDatePicker: return "";
        case PXUIElementTypeGroupBox: return "GroupBox";
        case PXUIElementTypeRadioButton: return "";
        case PXUIElementTypeGroupRadioButton: return "";
        case PXUIElementTypeTreeView: return "TreeView";
        case PXUIElementTypeTreeViewItem: return "TreeViewItem";
        case PXUIElementTypeIPInput: return "";
        case PXUIElementTypeLink: return "";
        case PXUIElementTypeHeader: return "";
        case PXUIElementTypeFontSelector: return "";
        case PXUIElementTypePageScroll: return "";
        case PXUIElementTypeTabControll: return "TabControll";
        case PXUIElementTypeToggle: return "Toggle";
        case PXUIElementTypeCheckBox: return "CheckBox";
        case PXUIElementTypeComboBox: return "ComboBox";
        case PXUIElementTypeColorPicker: return "ColorPicker";
        case PXUIElementTypeSlider: return "Slider";
        case PXUIElementTypeRenderFrame: return "FrameRender";

        default:
            return PXNull;
    }
}

void PXAPI PXUIElementPositionCalculcate(PXUIElement* const pxUIElement, PXUIElementPositionCalulcateInfo* const pxUIElementPositionCalulcateInfo)
{
    for(PXUIElement* pxUIElementParent = pxUIElement->Parent; pxUIElementParent; pxUIElementParent = pxUIElementParent->Parent)
    {
        pxUIElementPositionCalulcateInfo->MarginLeft += pxUIElementParent->Position.MarginLeft;
        pxUIElementPositionCalulcateInfo->MarginTop += pxUIElementParent->Position.MarginTop;
        pxUIElementPositionCalulcateInfo->MarginRight += pxUIElementParent->Position.MarginRight;
        pxUIElementPositionCalulcateInfo->MarginBottom += pxUIElementParent->Position.MarginBottom;
    }

    pxUIElementPositionCalulcateInfo->MarginLeft += pxUIElement->Position.MarginLeft;
    pxUIElementPositionCalulcateInfo->MarginTop += pxUIElement->Position.MarginTop;
    pxUIElementPositionCalulcateInfo->MarginRight += pxUIElement->Position.MarginRight;
    pxUIElementPositionCalulcateInfo->MarginBottom += pxUIElement->Position.MarginBottom;

    // Normalizied space for OpenGL
    pxUIElementPositionCalulcateInfo->AA = -1 + pxUIElementPositionCalulcateInfo->MarginLeft; 
    pxUIElementPositionCalulcateInfo->BA = -1 + pxUIElementPositionCalulcateInfo->MarginTop;
    pxUIElementPositionCalulcateInfo->BB = +1 - pxUIElementPositionCalulcateInfo->MarginRight,
    pxUIElementPositionCalulcateInfo->AB = +1 - pxUIElementPositionCalulcateInfo->MarginBottom;

    float remLeft = (pxUIElementPositionCalulcateInfo->MarginLeft)* pxUIElementPositionCalulcateInfo->WindowWidth / 2.0f;
    float remTop = (pxUIElementPositionCalulcateInfo->MarginTop) * pxUIElementPositionCalulcateInfo->WindowHeight / 2.0f;
    float remRight = (pxUIElementPositionCalulcateInfo->MarginRight) * pxUIElementPositionCalulcateInfo->WindowWidth / 2.0f;
    float remBottom = (pxUIElementPositionCalulcateInfo->MarginBottom) * pxUIElementPositionCalulcateInfo->WindowHeight / 2.0f;


    // Add perspective scaling
    {
        float factorX = pxUIElementPositionCalulcateInfo->WindowWidth / (float)pxUIElementPositionCalulcateInfo->WindowHeight;
        float factorY = pxUIElementPositionCalulcateInfo->WindowHeight / (float)pxUIElementPositionCalulcateInfo->WindowWidth;

        remLeft += factorX;
        remTop += factorY;
        remRight -= factorX;
        remBottom -= factorY;
    }


    float mathWithScaling = pxUIElementPositionCalulcateInfo->WindowWidth - remRight - remLeft;
    float heightWithScaling = pxUIElementPositionCalulcateInfo->WindowHeight - remBottom - remTop;












    // XYWH for WindowsAPI stuff0


    if(pxUIElement->Position.FlagListKeep & PXUIElementKeepWidth)
    {
        pxUIElementPositionCalulcateInfo->Width = pxUIElement->Position.Width;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Width = mathWithScaling;
    }

    if(pxUIElement->Position.FlagListKeep & PXUIElementKeepHeight)
    {
        pxUIElementPositionCalulcateInfo->Height = pxUIElement->Position.Height;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Height = heightWithScaling;
    }

 

    switch(pxUIElement->Position.FlagListKeep & PXUIElementAllignFlags)
    {
        default:
        case PXUIElementAllignLeft:
        {
            pxUIElementPositionCalulcateInfo->X = remLeft;
            pxUIElementPositionCalulcateInfo->Y = remTop;
            break;
        }
        case PXUIElementAllignTop:
        {
            pxUIElementPositionCalulcateInfo->X = remLeft;
            pxUIElementPositionCalulcateInfo->Y = remTop;
            break;
        }
        case PXUIElementAllignRight:
        {
            pxUIElementPositionCalulcateInfo->X = pxUIElementPositionCalulcateInfo->WindowWidth - (remRight + pxUIElementPositionCalulcateInfo->Width);
            pxUIElementPositionCalulcateInfo->Y = pxUIElementPositionCalulcateInfo->WindowHeight - (remBottom + pxUIElementPositionCalulcateInfo->Height);
            break;

        }
        case PXUIElementAllignBottom:
        {
            pxUIElementPositionCalulcateInfo->X = remLeft;
            pxUIElementPositionCalulcateInfo->Y = pxUIElementPositionCalulcateInfo->WindowHeight - (remBottom + pxUIElementPositionCalulcateInfo->Height);
            break;
        }
    }

}

PXActionResult PXAPI PXFileTypeInfoProbe(PXFileTypeInfo* const pxFileTypeInfo, const PXText* const pxText)
{
    // Probe for file extension
    {
        pxFileTypeInfo->FormatExpected = PXFilePathExtensionDetectTry(pxText);
    }

    switch (pxFileTypeInfo->FormatExpected)
    {
        case PXFileFormatA3DS:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXAutodesk3DSLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXAutodesk3DSSafeFromFile;
            break;

        case PXFileFormatAAC:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXAACLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXAACSaveToFile;
            break;

        case PXFileFormatAVI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXAVILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXAVISaveToFile;
            break;

        case PXFileFormatBitMap:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXBitmapLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXBitmapSaveToFile;
            break;

        case PXFileFormatCanonRaw3:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXCanonRaw3LoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCanonRaw3SaveToFile;
            break;

        case PXFileFormatC:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoad = PXCLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCSaveToFile;
            break;

        case PXFileFormatCSharp:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoad = PXCSLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCSLoadFromFile;
            break;

        case PXFileFormatCSS:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXCSSLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCSSLoadFromFile;
            break;

        case PXFileFormatCPP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoad = PXCPPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCPPLoadFromFile;
            break;

        case PXFileFormatBinaryWindows:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeBinary;
            pxFileTypeInfo->ResourceLoad = PXBinaryWindowsLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXBinaryWindowsSaveToFile;
            break;

        case PXFileFormatBinaryLinux:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeBinary;
            pxFileTypeInfo->ResourceLoad = PXBinaryLinuxLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXBinaryLinuxSaveToFile;
            break;

        case PXFileFormatDirectDrawSurfaceTexture:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXDDSLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXDDSSaveToFile;
            break;

        case PXFileFormatEML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;

        case PXFileFormatFilmBox:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXFilmBoxLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXFilmBoxSaveToFile;
            break;

        case PXFileFormatFLAC:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXFLACLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXFLACSaveToFile;
            break;

        case PXFileFormatSpriteFont:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeFont;
            pxFileTypeInfo->ResourceLoad = PXSpriteFontLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSpriteFontSaveToFile;
            break;

        case PXFileFormatGIF:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXGIFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXGIFSaveToFile;
            break;

        case PXFileFormatHighEfficiencyImageFile:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXHEIFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXHEIFSaveToFile;
            break;

        case PXFileFormatHTML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXHTMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXHTMLSaveToFile;
            break;

        case PXFileFormatINI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXINILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXINISaveToFile;
            break;

        case PXFileFormatJPEG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXJPEGLoadFromImage;
            pxFileTypeInfo->ResourceSave = PXJPEGSaveToImage;
            break;

        case PXFileFormatJSON:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXJSONLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXJSONSaveToFile;
            break;

        case PXFileFormatM4A:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;

        case PXFileFormatMIDI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXMIDILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMIDISaveToFile;
            break;

        case PXFileFormatMP3:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXMP3LoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMP3SaveToFile;
            break;

        case PXFileFormatMP4:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXMP4LoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMP4SaveToFile;

        case PXFileFormatMSI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeInstaller;
            pxFileTypeInfo->ResourceLoad = PXMSILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMSISaveToFile;

        case PXFileFormatMTL:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeRenderMaterial;
            pxFileTypeInfo->ResourceLoad = PXMTLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMTLSaveToFile;
            break;

        case PXFileFormatWavefront:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXWavefrontLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWavefrontSaveFromFile;
            break;

        case PXFileFormatMatroska:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXMatroskaLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMatroskaSaveToFile;
            break;            

        case PXFileFormatOGG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXOGGLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXOGGSaveToFile;
            break;

        case PXFileFormatEugeneRoshalArchive:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeArchiv;
            pxFileTypeInfo->ResourceLoad = PXRARLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXRARSaveToFile;
            break;            

        case PXFileFormatPDF:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeDocument;
            pxFileTypeInfo->ResourceLoad = PXPDFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPDFSaveToFile;
            break;

        case PXFileFormatPHP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeCode;
            pxFileTypeInfo->ResourceLoad = PXPHPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPHPSaveToFile;
            break;

        case PXFileFormatPLY:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXPLYLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPLYSaveToFile;
            break;

        case PXFileFormatPNG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXPNGLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPNGSaveToFile;
            break;

        case PXFileFormatQOI:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;

        case PXFileFormatSTEP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXSTEPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSTEPSaveToFile;
            break;

        case PXFileFormatSTL:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXSTLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSTLSaveToFile;
            break;

        case PXFileFormatSVG:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXSVGLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSVGSaveToFile;
            break;

        case PXFileFormatTGA:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXTGALoadFromFile;
            pxFileTypeInfo->ResourceSave = PXTGASaveToFile;
            break;

        case PXFileFormatTagImage:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXTIFFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXTIFFSaveToFile;
            break;

        case PXFileFormatTrueTypeFont:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeFont;
            pxFileTypeInfo->ResourceLoad = PXTTFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXTTFSaveToFile;
            break;

        case PXFileFormatVRML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXVRMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXVRMLSaveToFile;
            break;

        case PXFileFormatWave:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXWaveLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWaveSaveToFile;
            break;

        case PXFileFormatWEBM:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXWEBMLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWEBMSaveToFile;
            break;

        case PXFileFormatWEBP:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXWEBPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWEBPSaveToFile;
            break;

        case PXFileFormatWMA:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXWMALoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWMASaveToFile;
            break;

        case PXFileFormatXML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXXMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXXMLSaveToFile;
            break;

        case PXFileFormatYAML:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXYAMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXYAMLSaveToFile;
            break;


        default:
            pxFileTypeInfo->ResourceType = PXFileResourceTypeUnkown;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceLoad(PXResourceLoadInfo* const pxResourceLoadInfo, const PXText* const filePath)
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
        if(pxFile.TypeInfo.FormatExpected == PXFileFormatUnkown)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "Resource",
                "Load",
                "Refused : Format not detected"
            );
#endif

            return PXActionRefusedNotSupported;
        }

        if(pxFile.TypeInfo.ResourceLoad == PXNull)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "Resource",
                "Load",
                "Refused : Not implemented"
            );
#endif

            return PXActionRefusedNotImplemented;
        }

        PXInt32U time = PXProcessorTimeReal();


        pxResourceLoadInfo->FileReference = &pxFile;


        const PXActionResult fileParsingResult = pxFile.TypeInfo.ResourceLoad(pxResourceLoadInfo);

#if PXLogEnable
        PXInt32U timeDelat = PXProcessorTimeReal() - time;

        PXText pxText;
        PXTextConstructBufferA(&pxText, 32);
        PXTextFormatTime(&pxText, timeDelat);

        PXLogPrint
        (
            PXLoggingInfo,
            "Resource",
            "Load",
            "%6s  ROPs:%-7i <%s>",
            pxText.TextA,
            pxFile.CounterOperationsRead,
            filePath->TextA
        );
#endif

        PXFileDestruct(&pxFile);

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
        } while(fileGuessResult == PXActionRefusedInvalidHeaderSignature);

        PXFileDestruct(&pxFile);

        return fileGuessResult;
    }
#endif
}

PXActionResult PXAPI PXResourceLoadA(PXResourceLoadInfo* const pxResourceLoadInfo, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceLoad(pxResourceLoadInfo, &pxText);

    return loadResult;
}

PXActionResult PXAPI PXResourceSave(PXResourceSaveInfo* const pxResourceSaveInfo, const PXText* const filePath)
{
    PXFile pxFile;

    // Loading file
    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.Text = *filePath;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeWriteOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXTrue;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        const PXActionResult fileLoadingResult = PXFileOpenFromPath(&pxFile, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(fileLoadingResult);
    }

    // Try to load assumed format
    {
        if(pxFile.TypeInfo.FormatExpected == PXFileFormatUnkown)
        {
            PXLogPrint
            (
                PXLoggingError,
                "Resource",
                "Save",
                "Refused : Format not known"
            );

            return PXActionRefusedNotSupported;
        }

        if(pxFile.TypeInfo.ResourceSave == PXNull)
        {
            PXLogPrint
            (
                PXLoggingError,
                "Resource",
                "Save",
                "Refused : Not implemented"
            );

            return PXActionRefusedNotImplemented;
        }

        PXInt32U time = PXProcessorTimeReal();


        pxResourceSaveInfo->FileReference = &pxFile;  

        const PXActionResult fileParsingResult = pxFile.TypeInfo.ResourceSave(pxResourceSaveInfo);

        PXInt32U timeDelat = PXProcessorTimeReal() - time;

        PXLogPrint
        (
            PXLoggingInfo,
            "Resource",
            "Save",
            "%6.3fms SOPs:%-7i <%s>",
            timeDelat / 1000.0f,
            pxFile.CounterOperationsWrite,
            filePath->TextA
        );

        // PXActionReturnOnSuccess(fileParsingResult); // Exit if this has worked first-try 

        // return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }

    PXFileClose(&pxFile);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceSaveA(PXResourceSaveInfo* const pxResourceSaveInfo, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceSave(pxResourceSaveInfo, &pxText);

    return loadResult;
}