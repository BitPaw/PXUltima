#include "PXResource.h"

#include <PX/Math/PXMath.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Time/PXStopWatch.h>
#include <PX/OS/Hardware/PXProcessor.h>
#include <PX/OS/File/PXFile.h>

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
#include "N64/PXN64.h"
#include "PHP/PXPHP.h"
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
#include "CSharp/PXCSharp.h"
#include "CPP/PXCPP.h"
#include "CSS/PXCSS.h"
#include "DDS/PXDDS.h"
#include "FastFile/PXFastFile.h"
#include "ADLER/PXAdler32.h"
#include "Java/PXJava.h"
#include "ZIP/PXZIP.h"
#include "USD/PXUSD.h"
#include "VOB/PXVOB.h"
#include "JavaScript/PXJavaScript.h"
#include "TAR/PXTAR.h"
#include "GLSL/PXGLSL.h"
#include "HLSL/PXHLSL.h"

#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/Graphic/NativDraw/PXNativDraw.h>

const char PXResourceManagerText[] = "PX-Resource";

PXResourceManager _GLOBALResourceManager;

//void _chkstk(size_t s) {};
extern int _fltused = 0;

const PXF32 PXVertexDataTriangle[] =
{
    -1.0f, -1.0f,
        1.0f, -1.0f,
        0.5f,  1.0f
};

const PXInt8S PXIndexDataTriangle[] =
{
    0,1,2
};


const PXInt8S PXVertexDataRectangle[] =
{
    -1, -1,
        1, -1,
        1,  1,
        -1,  1
};

const PXInt8U PXIndexDataRectangle[] =
{
    0,1,2,
    2,3,0
};

const PXInt8S PXVertexDataCube[] =
{
    -1, -1, -1,
        1, -1, -1,
        -1,  1, -1,
        1,  1, -1,
        -1, -1,  1,
        1, -1,  1,
        -1,  1,  1,
        1,  1,  1,
};

const PXInt8U PXIndexDataCube[] =
{
#if 0
    0,1,2,3, // Left
    0,2,4,6, // Back
    2,3,7,6, // Top
    4,5,6,7, // Right
    0,1,4,5, // Bottom
    1,3,5,7  // Font
#else

    // Left
    0,1,2,
    2,3,1,

    // Right
    4,5,6,
    6,7,5,

    // Back
    0,2,4,
    4,6,2,

    // Font
    1,3,5,
    5,7,3,

    // Bottom
    0,1,4,
    4,5,1,

    // Top
    2,3,7,
    7,6,2

#endif
};


const char* PXAPI PXGraphicShaderTypeToString(const PXGraphicShaderType pxGraphicShaderType)
{
    switch(pxGraphicShaderType)
    {
        case PXShaderTypeVertex:     // .vert - a vertex shader
            return "VertexShader";

        case PXShaderTypePixel:   // .frag - a fragment shader
            return "PixelShader";

        case PXShaderTypeTessellationControl:    // .tesc - a tessellation control shader
            return "TessellationControlShader";

        case PXShaderTypeTessellationEvaluation:     // .tese - a tessellation evaluation shader
            return "TessellationEvaluationShader";

        case PXShaderTypeGeometry:      // .geom - a geometry shader
            return "GeometryShader";

        case PXShaderTypeCompute:   // .comp - a compute shader
            return "ComputeShader";

        case PXShaderTypeInvalid:
        default:
            return PXNull;
    }
}

const char* PXAPI PXVertexBufferFormatToString(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
        case PXVertexBufferFormatP2I8: return "P2 8-Bit";
        case PXVertexBufferFormatP3I8: return "P3 8-Bit";

        case PXVertexBufferFormatP3F16: return "P3 F16";
        case PXVertexBufferFormatN3F16: return "N3 F16";
        case PXVertexBufferFormatT2F16: return "T2 F16";

        case PXVertexBufferFormatP3F32: return "P3 F32";
        case PXVertexBufferFormatN3F32: return "N3 F32";
        case PXVertexBufferFormatT2F32: return "T2 F32";

        case PXVertexBufferFormatXYPXF32:
            return "P2 PXF32";
        case PXVertexBufferFormatXYZW:
            return "P4 PXF32";
        case PXVertexBufferFormatC4UB_XY:
            return "C4UB_XY";
        case PXVertexBufferFormatC4UB_XYZ:
            return "C4UB_XYZ";
        case PXVertexBufferFormatRGBXYZ:
            return "T2 P3 PXF32";
        case PXVertexBufferFormatN3F_XYZ:
            return "N3 P3 PXF32";
        case PXVertexBufferFormatC4F_N3F_XYZ:
            return "C4 N3 P3 PXF32";
        case PXVertexBufferFormatT2F_XYZ:
            return "T2 XYZ PXF32";
        case PXVertexBufferFormatT4F_XYZW:
            return "T4 P3 - PXF32";
        case PXVertexBufferFormatT2F_C4UB_XYZ:
            return "T4 C4 P3 - PXF32";
        case PXVertexBufferFormatT2F_C3F_XYZ:
            return "T2 C4 P3 - PXF32";
        case PXVertexBufferFormatT2F_N3F_XYZ:
            return "T2 N3 P3 - PXF32";
        case PXVertexBufferFormatT2F_C4F_N3F_XYZ:
            return "T2 C4 N3 P3 - PXF32";
        case PXVertexBufferFormatT4F_C4F_N3F_XYZW:
            return "T4 C4 N3 P4 - PXF32";
        case PXVertexBufferFormatXYZC:
            return "XYZC";
        case PXVertexBufferFormatXYZRHW:
            return "XYZRHW";
        case PXVertexBufferFormatXYZHWC:
            return "XYZHWC";
        case PXVertexBufferFormatXYZB1:
            return "XYZB1";
        case PXVertexBufferFormatXYZB2:
            return "XYZB2";
        case PXVertexBufferFormatXYZB3:
            return "XYZB3";
        case PXVertexBufferFormatXYZB4:
            return "XYZB4";
        case PXVertexBufferFormatXYZB5:
            return "XYZB5";

        default:
            return "Invalid";
    }
}

PXInt8U PXAPI PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
        case PXVertexBufferFormatP2I8:
            return 2;
        case PXVertexBufferFormatP3I8:
            return 3;
        case PXVertexBufferFormatT2F_XYZ:
            return 2u + 3u;
        case PXVertexBufferFormatT2F_N3F_XYZ:
            return 2u + 3u + 3u;
        case PXVertexBufferFormatP3F32:
            return 3u;
        case PXVertexBufferFormatN3F_XYZ:
            return 3u + 3u;

        default:
            return 0u;
    }
}

PXInt8U PXAPI PXVertexBufferFormatSizePerVertex(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
        case PXVertexBufferFormatP2I8:
        case PXVertexBufferFormatT2F16:
        case PXVertexBufferFormatT2F32:
            return 2;

        case PXVertexBufferFormatN3F16:
        case PXVertexBufferFormatP3F16:
        case PXVertexBufferFormatN3F32:
        case PXVertexBufferFormatP3F32:
        case PXVertexBufferFormatP3I8:
            return 3;

        default:
            return 0u;
    }
}

const char* PXFileLocationModeToString(const PXFileLocationMode pxFileLocationMode)
{
    switch(pxFileLocationMode)
    {
        case PXFileLocationModeInternal:        return "Internal";
        case PXFileLocationModeExternal:        return "External";
        case PXFileLocationModeMappedVirtual:   return "MappedVirtual";
        case PXFileLocationModeMappedFromDisk:  return "MappedFromDisk";
        case PXFileLocationModeDirectCached:    return "DirectCached";
        case PXFileLocationModeDirectUncached:  return "DirectUncached";
        default:                                return "Invalid";
    }
}

PXBool PXAPI PXFileDataAvailable(const PXFile* const pxFile)
{
    switch(pxFile->LocationMode)
    {
        case PXFileLocationModeInternal:
        case PXFileLocationModeExternal:
        case PXFileLocationModeMappedVirtual:
        case PXFileLocationModeMappedFromDisk:
        {
            return pxFile->Data && (pxFile->DataCursor <= pxFile->DataUsed);
        }
        case PXFileLocationModeDirectCached:
        case PXFileLocationModeDirectUncached:
            break;

        default:
            return PXFalse;
    }

    return PXTrue;
}

void PXAPI PXResourceManagerRelease(PXResourceManager* const pxResourceManager)
{
    // assert(pxResourceManager);
}

PXResourceManager* PXAPI PXResourceManagerInstanceFetch(void)
{
    if(PXResourceManagerInit & _GLOBALResourceManager.Flags)
    {
        return &_GLOBALResourceManager;
    }

    // PXClear(PXResourceManager, pxResourceManager); // Not needed? global mem is alway zero?

    PXLockCreate(&_GLOBALResourceManager.CreateLock, PXLockTypeProcessOnly);

    PXListDynamicInit(&_GLOBALResourceManager.NameCache, sizeof(PXInt32U), PXListDynamicSizeObject2Byte);
    PXListDynamicInit(&_GLOBALResourceManager.SourcePathCache, sizeof(PXInt32U), PXListDynamicSizeObject2Byte);

    PXDictionaryConstruct(&_GLOBALResourceManager.MaterialLookUp, sizeof(PXInt32U), sizeof(PXMaterial), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.SpritelLookUp, sizeof(PXInt32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.TextLookUp, sizeof(PXInt32U), sizeof(PXEngineText), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.TimerLookUp, sizeof(PXInt32U), sizeof(PXEngineTimer), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.SoundLookUp, sizeof(PXInt32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.HitBoxLookUp, sizeof(PXInt32U), sizeof(PXHitBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture2D), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.ModelLookUp, sizeof(PXInt32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.ShaderProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.SkyBoxLookUp, sizeof(PXInt32U), sizeof(PXSkyBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.ImageLookUp, sizeof(PXInt32U), sizeof(PXImage), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.BrushLookUp, sizeof(PXInt32U), sizeof(PXWindowBrush), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.GUIElementLookup, sizeof(PXInt32U), sizeof(PXWindow), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.SpriteAnimator, sizeof(PXInt32U), sizeof(PXSpriteAnimator), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.IconLookUp, sizeof(PXInt32U), sizeof(PXIcon), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.IconAtlasLookUp, sizeof(PXInt32U), sizeof(PXIconAtlas), PXDictionaryValueLocalityExternalReference);


    _GLOBALResourceManager.Flags |= PXResourceManagerInit;

    PXThreadPoolCreate(PXNull);

    return &_GLOBALResourceManager;
}

PXInt32U PXAPI PXResourceManagerGenerateUniqeID()
{
    // assert(pxResourceManager);

    return 1000 + ++_GLOBALResourceManager.UniqeIDGeneratorCounter;
}


#if OSWindows
HBITMAP PXAPI PXBitMapFromImage(int width, int height, int amountofchannels, void* data)
{
    BITMAPINFO bmi;
    PXClear(BITMAPINFO, &bmi);
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Top-down DIB 
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 8 * amountofchannels;
    bmi.bmiHeader.biCompression = BI_RGB;
    void* bitmapData;
    HBITMAP hBitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, &bitmapData, NULL, 0);

    if(!hBitmap)
    {
        return PXNull;
    }

    PXSize size = amountofchannels * width * height;



    PXMemoryCopy(data, size, bitmapData, size);

    return hBitmap;
}

HICON PXAPI PXIconFromBitMap(PXImage* const pxImage)
{

}
#endif

#include <PX/OS/Async/PXThreadPool.h>

typedef PXActionResult (PXAPI* PXResourceEntryCreateFunction)(PXResourceCreateInfo* const pxResourceCreateInfo, void* const objectRef);

typedef struct PXResourceEntry_
{
    PXDictionary* LookupTable;
    PXResourceEntryCreateFunction CreateFunction;
    const char* Name;
    PXInt32U TypeID;
    PXInt32U TypeSize;
}
PXResourceEntry;

// Resource lookup for directCall
const PXResourceEntry _GlobalResourceEntryList[] =
{
    {PXNull,                                PXNull,                                 "Invalid", PXResourceTypeInvalid, 0 },
    {PXNull,                                PXNull,                                 "Custom", PXResourceTypeCustom, 0 },
    {&_GLOBALResourceManager.ImageLookUp,   PXResourceCreateImage,                  "Image", PXResourceTypeImage, sizeof(PXImage)},
    {&_GLOBALResourceManager.SoundLookUp,   PXResourceCreateSound,                  "Sound",PXResourceTypeSound, sizeof(PXSound) },
    {PXNull,                                PXNull,                                 "Video", PXResourceTypeVideo,sizeof(PXVideo) },
    {&_GLOBALResourceManager.ModelLookUp,   PXResourceCreateModel,                  "Model",PXResourceTypeModel, sizeof(PXModel) },
    {&_GLOBALResourceManager.FontLookUp,    PXResourceCreateFont,                   "Font",PXResourceTypeFont, sizeof(PXFont) },
    {&_GLOBALResourceManager.MaterialLookUp, PXResourceCreateMaterial,              "Material", PXResourceTypeMaterial, sizeof(PXMaterial) },
    {&_GLOBALResourceManager.BrushLookUp,   PXResourceCreateBrush,                  "Brush",PXResourceTypeBrush, sizeof(PXWindowBrush) },
    {&_GLOBALResourceManager.TextureLookUp, PXResourceCreateTexture2D,              "Texture2D",PXResourceTypeTexture2D, sizeof(PXTexture2D) },
    {&_GLOBALResourceManager.TextureLookUp, PXResourceCreateTextureCube,            "TextureCube", PXResourceTypeTextureCube, sizeof(PXTextureCube) },
    {&_GLOBALResourceManager.ShaderProgramLookup, PXResourceCreateShaderProgram,    "ShaderProgram",PXResourceTypeShaderProgram, sizeof(PXShaderProgram) },
    {&_GLOBALResourceManager.SkyBoxLookUp,  PXResourceCreateSkybox,                 "SkyBox",PXResourceTypeSkybox, sizeof(PXSkyBox) },
    {&_GLOBALResourceManager.SpritelLookUp, PXResourceCreateSprite,                 "Sprite",PXResourceTypeSprite, sizeof(PXSprite) },
    {&_GLOBALResourceManager.IconLookUp,    PXResourceCreateIcon,                   "Icon",PXResourceTypeIcon, sizeof(PXIcon) },
    {&_GLOBALResourceManager.IconAtlasLookUp, PXResourceCreateIconAtlas,              "IconAtlas",PXResourceTypeIconAtlas, sizeof(PXIconAtlas) },
    {&_GLOBALResourceManager.SpriteAnimator, PXResourceCreateSpriteAnimator,        "SpriteAnimator",PXResourceTypeSpriteAnimator, sizeof(PXSpriteAnimator) },
    {PXNull,                                PXNull,                                 "Text",PXResourceTypeText, sizeof(PXText) },
    {&_GLOBALResourceManager.TimerLookUp,   PXResourceCreateTimer,                  "Timer",PXResourceTypeTimer, sizeof(PXEngineTimer) },
    {PXNull,                                PXNull,                                 "---",PXResourceTypeEngineSound, 0 },
    {&_GLOBALResourceManager.GUIElementLookup, PXResourceCreateWindow,              "Window", PXResourceTypeGUIElement, sizeof(PXWindow) },
    {&_GLOBALResourceManager.HitBoxLookUp,  PXResourceCreateHitBox,                 "---",PXResourceTypeHitBox, sizeof(PXHitBox) },
    {&_GLOBALResourceManager.MaterialLookUp, PXNull,                                "Material", PXResourceTypeMaterialList, PXNull},
    {PXNull, PXNull, "---", PXResourceTypeCodeDocument, sizeof(PXWindowBrush) },
    {PXNull, PXNull, "---", PXResourceTypeDocument, sizeof(PXWindowBrush) },
    {PXNull, PXNull, "---", PXResourceTypeBinary, sizeof(PXWindowBrush) },
    {PXNull, PXNull, "---", PXResourceTypeStructuredText, sizeof(PXWindowBrush) },
    {PXNull, PXNull, "---", PXResourceTypeInstaller, sizeof(PXWindowBrush) },
    {PXNull, PXNull, "---", PXResourceTypeArchiv, sizeof(PXWindowBrush) },
    {PXNull, PXNull, "---", PXResourceTypeDialogBox, sizeof(PXWindowBrush) }
};


PXActionResult PXAPI PXResourceCreateSkybox(PXResourceCreateInfo* const pxResourceCreateInfo, PXSkyBox* const pxSkyBox)
{
    PXSkyBoxCreateEventInfo* const pxSkyBoxCreateEventData = &pxResourceCreateInfo->SkyBox;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Create",
        "SkyBox ID:%i\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s\n"
        "%9s : %s",
        pxSkyBox->Info.ID,
        "Top", pxSkyBoxCreateEventData->SkyBoxTextureA,
        "Left", pxSkyBoxCreateEventData->SkyBoxTextureB,
        "Right", pxSkyBoxCreateEventData->SkyBoxTextureC,
        "Back", pxSkyBoxCreateEventData->SkyBoxTextureD,
        "Bottom", pxSkyBoxCreateEventData->SkyBoxTextureE,
        "Forward", pxSkyBoxCreateEventData->SkyBoxTextureF,
        "Vertex Shader", pxSkyBoxCreateEventData->ShaderProgramCreateInfo.ShaderVertexFilePath,
        "Pixel Shader", pxSkyBoxCreateEventData->ShaderProgramCreateInfo.ShaderPixelFilePath
    );
#endif

    // Load Textures
    {
        PXResourceCreateInfo pxResourceCreateInfoList[2];
        PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoList, 2);

        // Skybox CubeTexture
        pxResourceCreateInfoList[0].Type = PXResourceTypeTextureCube;
        pxResourceCreateInfoList[0].Flags = PXResourceCreateBehaviourSpawnInScene | PXResourceCreateBehaviourLoadASYNC;
        pxResourceCreateInfoList[0].ObjectReference = (void**)&pxSkyBox->TextureCube; // Request object
        pxResourceCreateInfoList[0].TextureCube.FilePathA = pxSkyBoxCreateEventData->SkyBoxTextureA;
        pxResourceCreateInfoList[0].TextureCube.FilePathB = pxSkyBoxCreateEventData->SkyBoxTextureB;
        pxResourceCreateInfoList[0].TextureCube.FilePathC = pxSkyBoxCreateEventData->SkyBoxTextureC;
        pxResourceCreateInfoList[0].TextureCube.FilePathD = pxSkyBoxCreateEventData->SkyBoxTextureD;
        pxResourceCreateInfoList[0].TextureCube.FilePathE = pxSkyBoxCreateEventData->SkyBoxTextureE;
        pxResourceCreateInfoList[0].TextureCube.FilePathF = pxSkyBoxCreateEventData->SkyBoxTextureF;

        // Skybox Shader
        pxResourceCreateInfoList[1].Type = PXResourceTypeShaderProgram;
        pxResourceCreateInfoList[1].ObjectReference = (void**)&pxSkyBox->ShaderProgramReference; // Request object

        PXCopy(PXShaderProgramCreateInfo, pxSkyBoxCreateEventData, &pxResourceCreateInfoList[1].ShaderProgram);

        // Skybox model
        //pxEngineResourceActionInfoList[1].Type = PXEngineResourceActionTypeCreate;
        //pxEngineResourceActionInfoList[1].Create.CreateType = PXEngineCreateTypeModel;
        //pxEngineResourceActionInfoList[1].Create.SpawnEnabled = PXTrue;
        //pxEngineResourceActionInfoList[1].Create.Model.ModelReference = &pxSkyBox->Model; // Request object
        //pxEngineResourceActionInfoList[1].Create.Model.ModelFilePath = pxSkyBoxCreateEventData->;

        PXResourceManagerAddV(pxResourceCreateInfoList, 2);

        PXCopy(PXShaderProgramCreateInfo, &pxResourceCreateInfoList[1].ShaderProgram, pxSkyBoxCreateEventData);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateBrush(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindowBrush* const pxWindowBrush)
{
    PXBrushCreateInfo* const pxBrushCreateInfo = &pxResourceCreateInfo->Brush;

    PXNativDrawBrushCreate
    (
        PXNativDrawInstantance(),
        pxWindowBrush,
        &pxBrushCreateInfo->Color
    );


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Brush",
        "ID:%i, Color-RGB:%2.2x%2.2x%2.2x, <%s>",
        pxWindowBrush->Info.ID,
        pxBrushCreateInfo->Color.Red,
        pxBrushCreateInfo->Color.Green,
        pxBrushCreateInfo->Color.Blue,
        pxResourceCreateInfo->Name
    );
#endif

    pxWindowBrush->Info.Behaviour |= PXResourceInfoExist;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateImage(PXResourceCreateInfo* const pxResourceCreateInfo, PXImage* const pxImage)
{
    PXImageCreateInfo* const pxImageCreateInfo = &pxResourceCreateInfo->Image;


    //  pxImage->Info.ID = resourceID;

    PXText pxText;
    PXTextConstructFromAdressA(&pxText, pxResourceCreateInfo->FilePath, PXTextLengthUnkown, PXTextLengthUnkown);
    const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePath;
    PXInt32U checkSum = 0;


    // Load texture
    if(hasFilePath)
    {
        PXResourceTransphereInfo pxResourceLoadInfo;
        PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
        pxResourceLoadInfo.ResourceTarget = pxImage;
        pxResourceLoadInfo.ResourceType = PXResourceTypeImage;
        pxResourceLoadInfo.Manager = &_GLOBALResourceManager;

        const PXActionResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxText);


        if(PXActionSuccessful != loadResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Image-Create",
                "Failed <%s>!",
                pxText.TextA
            );
#endif

            return loadResult;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Image-Create",
            "Successful <%s>.",
            pxText.TextA
        );
#endif
    }
    else
    {
        PXImageCopyAsNew(pxImage, &pxImageCreateInfo->Image);

        checkSum = 0;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateShaderProgram(PXResourceCreateInfo* const pxResourceCreateInfo, PXShaderProgram* const pxShaderProgram)
{
    PXShaderProgramCreateInfo* const pxShaderProgramCreateData = &pxResourceCreateInfo->ShaderProgram;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Create",
        "ShaderProgram [PXID:%i]",
        pxShaderProgram->Info.ID
    );
#endif

    // is the data already present?
    const PXBool isDataPrensent = pxShaderProgramCreateData->ShaderVertexText && pxShaderProgramCreateData->ShaderPixelText;

    if(isDataPrensent)
    {
        PXFileOpenInfo pxFileOpenInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
        pxFileOpenInfo.BufferData = pxShaderProgramCreateData->ShaderVertexText;
        pxFileOpenInfo.BufferSize = pxShaderProgramCreateData->ShaderVertexTextSize;

        PXFileOpen(&pxShaderProgramCreateData->ShaderVertexFile, &pxFileOpenInfo);

        pxFileOpenInfo.BufferData = pxShaderProgramCreateData->ShaderPixelText;
        pxFileOpenInfo.BufferSize = pxShaderProgramCreateData->ShaderPixelTextSize;

        PXFileOpen(&pxShaderProgramCreateData->ShaderPixelFile, &pxFileOpenInfo);

        pxShaderProgram->Info.Behaviour |= PXResourceInfoStorageMemory;

        PXResourceStorePath(&pxShaderProgram->Info, "<Internal>", -1);
    }
    else // load file
    {
        PXFileOpenInfo pxFileOpenFromPathInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenFromPathInfo);
        pxFileOpenFromPathInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.FlagList = PXFileIOInfoAllowMapping | PXFileIOInfoFilePhysical;

        // TODO: we can make it a loop?

        pxFileOpenFromPathInfo.FilePathAdress = pxShaderProgramCreateData->ShaderVertexFilePath;

        const PXActionResult vertexLoadResult = PXFileOpen(&pxShaderProgramCreateData->ShaderVertexFile, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(vertexLoadResult);


        pxFileOpenFromPathInfo.FilePathAdress = pxShaderProgramCreateData->ShaderPixelFilePath;

        const PXActionResult fragmentLoadResult = PXFileOpen(&pxShaderProgramCreateData->ShaderPixelFile, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(fragmentLoadResult);

        pxShaderProgram->Info.Behaviour |= PXResourceInfoStorageMemory;

        PXResourceStorePath(&pxShaderProgram->Info, pxShaderProgramCreateData->ShaderVertexFilePath, -1);
    }

#if 0
    // Create job to register shader, register might need to be sync. 
    {
        PXResourceCreateInfo* pxResourceCreateInfoASYNC = PXMemoryHeapCallocT(PXResourceCreateInfo, 1);
        PXCopy(PXResourceCreateInfo, pxResourceCreateInfo, pxResourceCreateInfoASYNC);
        pxResourceCreateInfoASYNC->Flags |= PXResourceCreateBehaviourIsASYNCCall;

        PXThreadPoolQueueWork
        (
            PXNull,
            PXResourceManagerAdd,
            pxResourceCreateInfoASYNC,
            PXNull,
            PXTaskParameterRelease | PXTaskExecuteASYNC
        );
    }
#endif

 

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateIcon(PXResourceCreateInfo* const pxResourceCreateInfo, PXIcon* const pxIcon)
{
    PXIconCreateInfo* const pxIconCreateInfo = &pxResourceCreateInfo->Icon;

    PXResourceStoreName(&pxIcon->Info, pxResourceCreateInfo->Name, -1);

    // Check if texture is present
    if(!pxIconCreateInfo->IconImage)
    {
        PXResourceCreateInfo pxResourceCreateInfoSub;

        PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.ObjectReference = (void**)&pxIconCreateInfo->IconImage;
        pxResourceCreateInfoSub.ObjectAmount = 1;
        pxResourceCreateInfoSub.FilePath = pxResourceCreateInfo->FilePath;
        pxResourceCreateInfoSub.FilePathSize = -1;
        pxResourceCreateInfoSub.Type = PXResourceTypeImage;

        PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Icon-Create",
        "ID:%i - WHXY : %3i,%3i,%3i,%3i",
        pxIcon->Info.ID,
        pxIconCreateInfo->Width,
        pxIconCreateInfo->Height,
        pxIconCreateInfo->OffsetX,
        pxIconCreateInfo->OffsetY
    );
#endif

    PXNativDrawIconFromImage(PXNativDrawInstantance(), pxIcon, pxIconCreateInfo->IconImage);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateFont(PXResourceCreateInfo* const pxResourceCreateInfo, PXFont* const pxFont)
{
    PXEngineFontCreateInfo* const pxEngineFontCreateData = &pxResourceCreateInfo->Font;

    // if this is a system font, we dont load it from disk but from the system, duh.
    if(pxEngineFontCreateData->RegisteredName)
    {
        PXNativDrawFontLoadA
        (
            PXNativDrawInstantance(),
            pxFont,
            pxEngineFontCreateData->RegisteredName,
            pxEngineFontCreateData->RegisteredNameLength
        );

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Font-Load",
            "ID:%-4i <%s>, Name:<%s>, load from OS",
            pxFont->Info.ID,
            pxResourceCreateInfo->Name,
            pxEngineFontCreateData->RegisteredName

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
            PXResourceManagerText,
            "Font-Load",
            "ID:%4i <%s>, Path:<%s>, from file",
            pxFont->Info.ID,
            pxResourceCreateInfo->Name,
            pxResourceCreateInfo->FilePath

        );
#endif

        // Load font
        {
            PXResourceTransphereInfo pxResourceLoadInfo;
            PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
            pxResourceLoadInfo.ResourceTarget = pxFont;
            pxResourceLoadInfo.ResourceType = PXResourceTypeFont;
            pxResourceLoadInfo.Manager = &_GLOBALResourceManager;

            const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePath);

            PXActionReturnOnError(loadResult);
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateMaterial(PXResourceCreateInfo* const pxResourceCreateInfo, PXMaterial* const pxMaterial)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Material-Create",
        "Allocating x%i",
        pxResourceCreateInfo->ObjectAmount
    );
#endif

    void* keyList[64];

    for(PXSize materialIndex = 0; materialIndex < pxResourceCreateInfo->ObjectAmount; ++materialIndex)
    {
        PXMaterial* const pxMaterialCurrent = &pxMaterial[materialIndex];

        pxMaterialCurrent->Info.ID = PXResourceManagerGenerateUniqeID();
        pxMaterialCurrent->Info.Behaviour |= PXResourceInfoRender;

        keyList[materialIndex] = &pxMaterialCurrent->Info.ID;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateIconAtlas(PXResourceCreateInfo* const pxResourceCreateInfo, PXIconAtlas* const pxIconAtlas)
{
    PXIconAtlasCreateInfo* const pxIconAtlasCreateInfo = &pxResourceCreateInfo->IconAtlas;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "IconAtlas-Create",
        "ID:%i <%s> - CallSize:%i",
        pxIconAtlas->Info.ID,
        pxResourceCreateInfo->FilePath,
        pxIconAtlasCreateInfo->CellSize
    );
#endif


    // Load image
    {
        PXResourceCreateInfo pxResourceCreateInfoSub;

        PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.ObjectReference = &pxIconAtlas->IconTexture2D;
        pxResourceCreateInfoSub.ObjectAmount = 1;
        pxResourceCreateInfoSub.FilePath = pxResourceCreateInfo->FilePath;
        pxResourceCreateInfoSub.FilePathSize = -1;
        pxResourceCreateInfoSub.Type = PXResourceTypeTexture2D;

        PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

    pxIconAtlasCreateInfo->CellAmountX = pxIconAtlas->IconTexture2D->Image->Width / pxIconAtlasCreateInfo->CellSize;
    pxIconAtlasCreateInfo->CellAmountY = pxIconAtlas->IconTexture2D->Image->Height / pxIconAtlasCreateInfo->CellSize;
    pxIconAtlasCreateInfo->CellAmountTotal = pxIconAtlasCreateInfo->CellAmountX * pxIconAtlasCreateInfo->CellAmountY;


    // Preallocate icons
    pxIconAtlas->IconListAmount = pxIconAtlasCreateInfo->CellAmountTotal;
    pxIconAtlas->IconList = PXMemoryHeapCallocT(PXIcon, pxIconAtlasCreateInfo->CellAmountTotal);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "IconAtlas",
        "PXID:%i, CellMap:<%i/%i> from Texture:<%i/%i>. Total:%i icons",
        pxIconAtlas->Info.ID,
        pxIconAtlasCreateInfo->CellAmountX,
        pxIconAtlasCreateInfo->CellAmountY,
        pxIconAtlas->IconTexture2D->Image->Width,
        pxIconAtlas->IconTexture2D->Image->Height,
        pxIconAtlasCreateInfo->CellAmountTotal
    );
#endif

    if(!pxIconAtlas->IconList)
    {
        return PXActionInvalid;
    }

    // Register icons
    for(PXSize y = 0; y < pxIconAtlasCreateInfo->CellAmountY; ++y)
    {
        const PXSize pixelPositionY = y * pxIconAtlasCreateInfo->CellSize;

        for(PXSize x = 0; x < pxIconAtlasCreateInfo->CellAmountX; ++x)
        {
            const PXSize pixelPositionX = x * pxIconAtlasCreateInfo->CellSize;

            PXIcon* const pxIcon = &pxIconAtlas->IconList[x + y * pxIconAtlasCreateInfo->CellAmountX];

            PXResourceCreateInfo pxResourceCreateInfoSub;

            PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);
            pxResourceCreateInfoSub.ObjectReference = &pxIcon;
            pxResourceCreateInfoSub.ObjectAmount = 1;
            pxResourceCreateInfoSub.Type = PXResourceTypeIcon;
            pxResourceCreateInfoSub.Icon.IconImage = pxIconAtlas->IconTexture2D->Image;
            pxResourceCreateInfoSub.Icon.OffsetX = pixelPositionX;
            pxResourceCreateInfoSub.Icon.OffsetY = pixelPositionY;
            pxResourceCreateInfoSub.Icon.Width = pxIconAtlasCreateInfo->CellSize;
            pxResourceCreateInfoSub.Icon.Height = pxIconAtlasCreateInfo->CellSize;
            pxResourceCreateInfoSub.Icon.RowSize = pxIconAtlasCreateInfo->CellSize * 4;
            pxResourceCreateInfoSub.Icon.BitPerPixel = 8 * 4;

            PXResourceManagerAdd(&pxResourceCreateInfoSub);
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateTextureCube(PXResourceCreateInfo* const pxResourceCreateInfo, PXTextureCube* const pxTextureCube)
{
    PXTextureCubeCreateInfo* const pxTextureCubeCreateData = &pxResourceCreateInfo->TextureCube;

    pxTextureCube->Format = PXColorFormatRGBI8;

    PXResourceCreateInfo pxResourceCreateInfoList[6];
    PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoList, 6);

    pxResourceCreateInfoList[0].Type = PXResourceTypeImage;
    pxResourceCreateInfoList[0].ObjectReference = (void**)&pxTextureCube->ImageA;
    pxResourceCreateInfoList[0].FilePath = pxTextureCubeCreateData->FilePathA;

    pxResourceCreateInfoList[1].Type = PXResourceTypeImage;
    pxResourceCreateInfoList[1].ObjectReference = (void**)&pxTextureCube->ImageB;
    pxResourceCreateInfoList[1].FilePath = pxTextureCubeCreateData->FilePathB;

    pxResourceCreateInfoList[2].Type = PXResourceTypeImage;
    pxResourceCreateInfoList[2].ObjectReference = (void**)&pxTextureCube->ImageC;
    pxResourceCreateInfoList[2].FilePath = pxTextureCubeCreateData->FilePathC;

    pxResourceCreateInfoList[3].Type = PXResourceTypeImage;
    pxResourceCreateInfoList[3].ObjectReference = (void**)&pxTextureCube->ImageD;
    pxResourceCreateInfoList[3].FilePath = pxTextureCubeCreateData->FilePathD;

    pxResourceCreateInfoList[4].Type = PXResourceTypeImage;
    pxResourceCreateInfoList[4].ObjectReference = (void**)&pxTextureCube->ImageE;
    pxResourceCreateInfoList[4].FilePath = pxTextureCubeCreateData->FilePathE;

    pxResourceCreateInfoList[5].Type = PXResourceTypeImage;
    pxResourceCreateInfoList[5].ObjectReference = (void**)&pxTextureCube->ImageF;
    pxResourceCreateInfoList[5].FilePath = pxTextureCubeCreateData->FilePathF;

    PXResourceManagerAddV(pxResourceCreateInfoList, 6);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateTexture2D(PXResourceCreateInfo* const pxResourceCreateInfo, PXTexture2D* const pxTexture2D)
{
    const PXBool hasImageData = pxResourceCreateInfo->Texture2D.Image.Image.PixelData && pxResourceCreateInfo->Texture2D.Image.Image.PixelDataSize;

    //PXEngineTexture2DCreateData* const pxEngineTexture2DCreateData = &pxEngineResourceCreateInfo->Texture2D;

    // If we dont have a texture file path, instead of loading nothing, we
    // can pass the fail-back texture back. This prevents redundant materials and missing texture material
#if 0
    {
        const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePath;

        if(!hasFilePath)
        {
            // Do we have any other data to load?
            if(!hasImageData)
            {
                // Load failback texture
                *pxResourceCreateInfo->ObjectReference = _GLOBALResourceManager.Texture2DFailBack;

                return PXActionSuccessful;
            }
        }
    }
#endif

    pxTexture2D->Filter = PXGraphicRenderFilterNoFilter;
    pxTexture2D->LayoutNear = PXGraphicImageLayoutNearest;
    pxTexture2D->LayoutFar = PXGraphicImageLayoutNearest;
    pxTexture2D->WrapHeight = PXGraphicImageWrapRepeat;
    pxTexture2D->WrapWidth = PXGraphicImageWrapRepeat;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Create",
        "Texture2D PXID:%i <%s>.",
        pxTexture2D->Info.ID,
        pxResourceCreateInfo->FilePath
    );
#endif

    {
        PXResourceCreateInfo pxResourceCreateInfoSub;
        PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.Type = PXResourceTypeImage;
        pxResourceCreateInfoSub.ObjectReference = (void**)&pxTexture2D->Image;
        pxResourceCreateInfoSub.FilePath = pxResourceCreateInfo->FilePath;
        pxResourceCreateInfoSub.Image = pxResourceCreateInfo->Texture2D.Image; // What is this?

        PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateModel(PXResourceCreateInfo* const pxResourceCreateInfo, PXModel* const pxModel)
{
    PXModelCreateInfo* const pxModelCreateInfo = &pxResourceCreateInfo->Model;

    PXModelConstruct(pxModel);

    PXMesh* const pxMesh = &pxModel->Mesh;
    pxMesh->VertexBufferListAmount = 1;
    PXVertexBuffer* const pxVertexBuffer = &pxMesh->VertexBufferPrime;
    PXIndexBuffer* const pxIndexBuffer = &pxMesh->IndexBuffer;

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);
    PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);

    const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePath;

    if(hasFilePath)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Create",
            "Model ID:%i <%s>.",
            pxModel->Info.ID,
            pxResourceCreateInfo->FilePath
        );
#endif

        PXResourceStorePath(&pxModel->Info, pxResourceCreateInfo->FilePath, pxResourceCreateInfo->FilePathSize);

        // Load model
        {
            PXResourceTransphereInfo pxResourceLoadInfo;
            PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
            pxResourceLoadInfo.ResourceTarget = pxModel;
            pxResourceLoadInfo.ResourceType = PXResourceTypeModel;
            pxResourceLoadInfo.Manager = &_GLOBALResourceManager;

            const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePath);
            const PXBool success = PXActionSuccessful == loadResult;

            if(!success)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXResourceManagerText,
                    "Model-Load",
                    "ID:%i Failed",
                    pxModel->Info.ID
                );
#endif
                return loadResult;
            }
        }
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Model-Create",
            "ID:%i internal",
            pxModel->Info.ID
        );
#endif


        switch(pxModelCreateInfo->Form)
        {
            case PXModelFormCustom:
            {
                // Can the data be used form a const source? If so, we dont need to copy
                const PXBool isDataConst =
                    PXResourceInfoPermissionREAD & pxModel->Info.Behaviour &&
                    PXResourceInfoStorageMemory & pxModel->Info.Behaviour;

                if(!isDataConst)
                {
                    PXCopy(PXVertexBuffer, &pxModelCreateInfo->VertexBuffer, pxVertexBuffer);
                    PXCopy(PXIndexBuffer, &pxModelCreateInfo->IndexBuffer, pxIndexBuffer);

                    // Allocate memory and copy
                    pxVertexBuffer->VertexData = 0;
                    pxVertexBuffer->VertexDataSize = 0;
                    pxIndexBuffer->DataIndexPosition = 0;
                    pxIndexBuffer->DataIndexSizeSegment = 0;

                    pxVertexBuffer->VertexDataSize = pxModelCreateInfo->VertexBuffer.VertexDataSize;
                    pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXByte, pxModelCreateInfo->VertexBuffer.VertexDataSize, &pxModel->Mesh.VertexBuffer.VertexData, &pxModel->Mesh.VertexBuffer.VertexDataSize);
                  
                    pxModel->Mesh.IndexBuffer.DataIndexSizeSegment = pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment;
                    pxModel->Mesh.IndexBuffer.DataIndexPosition = PXMemoryHeapCallocT(PXByte, pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment, &pxModel->Mesh.IndexBuffer.DataIndexPosition, &pxModel->Mesh.IndexBuffer.DataIndexSizeSegment);

                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->VertexBuffer.VertexDataSize,
                        pxModelCreateInfo->VertexBuffer.VertexData,
                        pxVertexBuffer->VertexData
                    );
                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment,
                        pxModelCreateInfo->IndexBuffer.DataIndexPosition,
                        pxModel->Mesh.IndexBuffer.DataIndexPosition
                    );
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Engine",
                    "Model-Create",
                    "From: Custom"
                );
#endif

                break;
            }
            case PXModelFormTriangle:
            {
                pxVertexBuffer->Format = PXVertexBufferFormatXYPXF32;
                pxVertexBuffer->VertexData = (void*)PXVertexDataTriangle;
                pxVertexBuffer->VertexDataSize = sizeof(PXVertexDataTriangle);

                pxIndexBuffer->IndexDataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;
                pxIndexBuffer->DataIndexPosition = (void*)PXIndexDataTriangle;
                pxIndexBuffer->DataIndexSizeSegment = sizeof(PXIndexDataTriangle);



#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Engine",
                    "Model-Create",
                    "From: Triangle"
                );
#endif

                break;
            }
            case PXModelFormRectangle:
            {
                pxVertexBuffer->Format = PXVertexBufferFormatP2I8;
                pxVertexBuffer->VertexData = (void*)PXVertexDataRectangle;
                pxVertexBuffer->VertexDataSize = sizeof(PXVertexDataRectangle);

                pxIndexBuffer->IndexDataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;// PXDrawModeIDPoint | PXDrawModeIDLineLoop;
                pxIndexBuffer->DataIndexPosition = (void*)PXIndexDataRectangle;
                pxIndexBuffer->DataIndexSizeSegment = sizeof(PXIndexDataRectangle);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Engine",
                    "Model-Create",
                    "From: Rectangle"
                );
#endif

                break;
            }
            case PXModelFormCircle:
            {
                PXF32 cx = 0;
                PXF32 cy = 0;
                PXF32 radius = 1;
                int segmentAmount = 16;

                pxVertexBuffer->Format = PXVertexBufferFormatXYPXF32;
                pxVertexBuffer->VertexDataSize = segmentAmount * 2;
                pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXF32, pxVertexBuffer->VertexDataSize);

                PXF32* vertexData = (PXF32*)pxVertexBuffer->VertexData;

                for(PXSize i = 0; i < segmentAmount; ++i)
                {
                    const PXF32 theta = 2.0f * 3.14f * i / (PXF32)segmentAmount;//get the current angle
                    const PXF32 x = radius * PXMathCosinusRADF32(theta);//calculate the x component
                    const PXF32 y = radius * PXMathSinusRADF32(theta);//calculate the y component

                    vertexData[i++] = x + cx;
                    vertexData[i++] = x + cy; //output vertex
                }

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Engine",
                    "Model-Create",
                    "From: Circle"
                );
#endif

                break;
            }
            case PXModelFormCube:
            {
                pxVertexBuffer->Format = PXVertexBufferFormatP3I8;
                pxVertexBuffer->VertexData = (void*)PXVertexDataCube;
                pxVertexBuffer->VertexDataSize = sizeof(PXVertexDataCube);

                pxIndexBuffer->IndexDataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;
                pxIndexBuffer->DataIndexPosition = (void*)PXIndexDataCube;
                pxIndexBuffer->DataIndexSizeSegment = sizeof(PXIndexDataCube);


#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Engine",
                    "Model-Create",
                    "From: Cube"
                );
#endif

                /*

                PXF32* input = 0;

                PXF32* output = 0;
                PXSize outINdex = 0;

                // QUAD to TRIANGLE
                for(size_t i = 0; i < indexLength; i+=4)
                {
                    output[outINdex++] = input[i + 0];
                    output[outINdex++] = input[i + 1];
                    output[outINdex++] = input[i + 2];
                    output[outINdex++] = input[i + 2];
                    output[outINdex++] = input[i + 3];
                    output[outINdex++] = input[i + 1];
                }
                */

                break;
            }
            default:
                break;
        }


        // if we dont have an index array, create one
        // TODO: ???
    }

    // Setup
    // PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);

    pxModel->ShaderProgramReference = pxModelCreateInfo->ShaderProgramReference;

    pxModel->Info.Behaviour |= PXResourceInfoRender;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateSprite(PXResourceCreateInfo* const pxResourceCreateInfo, PXSprite* const pxSprite)
{
    PXSpriteCreateInfo* const pxSpriteCreateEventData = &pxResourceCreateInfo->Sprite;

    pxSprite->Info.Behaviour |= PXResourceInfoRender;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Create",
        "Sprite ID:%i, Use <%s>",
        pxSprite->Info.ID,
        pxResourceCreateInfo->FilePath
    );
#endif


    // Create hitbox if requested
    if(pxSpriteCreateEventData->HitboxBehaviour > 0)
    {
        PXResourceCreateInfo pxResourceCreateInfoList[2];
        PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoList, 2);

        // Skybox CubeTexture
        pxResourceCreateInfoList[0].Type = PXResourceTypeHitBox;
        pxResourceCreateInfoList[0].Flags = PXResourceCreateBehaviourSpawnInScene | PXResourceCreateBehaviourLoadASYNC;
        pxResourceCreateInfoList[0].ObjectReference = (void**)&pxSprite->HitBox;
        pxResourceCreateInfoList[0].HitBox.Model = pxSprite->Model;
        pxResourceCreateInfoList[0].HitBox.Behaviour = 0;

        PXResourceManagerAdd(pxResourceCreateInfoList);
    }



    // Scaling?













    // pxSprite->Model = pxResourceManager->ModelFailback;


#if 0
            // Clear sprite //     PXGraphicSpriteConstruct(&pxEngine->Graphic, pxSprite);
    {
        PXModelConstruct(&pxSprite->Model);

        //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
        //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

        PXVector2FSetXY(&pxSprite->TextureScalePositionOffset, 1, 1);
        PXVector2FSetXY(&pxSprite->TextureScalePointOffset, 1, 1);

        //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
    }

    // PXTextCopyA(pxEngineResourceCreateInfo->FilePath, 20, pxSprite->Name, 50);

    pxSprite->TextureScalePositionOffset.X = pxSpriteCreateEventData->TextureScalingPoints[0].X;
    pxSprite->TextureScalePositionOffset.Y = pxSpriteCreateEventData->TextureScalingPoints[0].Y;
    pxSprite->TextureScalePointOffset.X = pxSpriteCreateEventData->TextureScalingPoints[1].X;
    pxSprite->TextureScalePointOffset.Y = pxSpriteCreateEventData->TextureScalingPoints[1].Y;


    pxSprite->Model.ShaderProgramReference = pxSpriteCreateEventData->ShaderProgramCurrent;
    pxSprite->Model.IgnoreViewRotation = pxSpriteCreateEventData->ViewRotationIgnore;
    pxSprite->Model.IgnoreViewPosition = pxSpriteCreateEventData->ViewPositionIgnore;
    //pxSprite->Model.
    pxSprite->Model.RenderBothSides = PXTrue;



    pxSprite->Model.MaterialContaierList = PXNew(PXMaterialContainer);
    pxSprite->Model.MaterialContaierListSize = 1u;

    pxSprite->Model.MaterialContaierList->MaterialList = PXNew(PXMaterial);
    pxSprite->Model.MaterialContaierList->MaterialListSize = 1u;

    PXMaterial* materiial = &pxSprite->Model.MaterialContaierList->MaterialList[0];

    PXClear(PXMaterial, materiial);

    materiial->DiffuseTexture = pxSprite->Texture;
#endif


#if 0



    const PXBool hasScaling = pxSprite->TextureScalePositionOffset.X != 0 || pxSprite->TextureScalePositionOffset.Y != 0;

    if(hasScaling)
    {
        if(pxEngine->SpriteScaled.ResourceID.OpenGLID == 0)
        {
            PXOpenGLSpriteRegister(&pxEngine->Graphic.OpenGLInstance, pxSprite);
        }
        else
        {
            pxSprite->Model.StructureOverride = &pxEngine->SpriteScaled;
        }
    }
    else
    {
        if(pxEngine->SpriteUnScaled.ResourceID.OpenGLID == 0)
        {
            PXOpenGLSpriteRegister(&pxEngine->Graphic.OpenGLInstance, pxSprite);
        }
        else
        {
            pxSprite->Model.StructureOverride = &pxEngine->SpriteUnScaled;
        }
    }

    if(pxSpriteCreateEventData->Scaling.X == 0)
    {
        pxSpriteCreateEventData->Scaling.X = 1;
    }

    if(pxSpriteCreateEventData->Scaling.Y == 0)
    {
        pxSpriteCreateEventData->Scaling.Y = 1;
    }



    PXF32 aspectRationX = 1;

    if(pxSprite->Texture)
    {
        if(pxSprite->Texture->Image->Width && pxSprite->Texture->Image->Height)
        {
            aspectRationX = (PXF32)pxSprite->Texture->Image->Width / (PXF32)pxSprite->Texture->Image->Height;
        }
    }



    PXMatrix4x4FScaleSetXY(&pxSprite->Model.ModelMatrix, aspectRationX, 1);

    PXMatrix4x4FScaleByXY
    (
        &pxSprite->Model.ModelMatrix,
        pxSpriteCreateEventData->Scaling.X,
        pxSpriteCreateEventData->Scaling.Y
    );


    PXMatrix4x4FPositionSet(&pxSprite->Model.ModelMatrix, &pxSpriteCreateEventData->Position);




#endif




    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateSpriteAnimator(PXResourceCreateInfo* const pxResourceCreateInfo, PXSpriteAnimator* const pxSpriteAnimator)
{
    PXSpriteAnimatorInfo* const pxSpriteAnimatorInfo = &pxResourceCreateInfo->SpriteAnimator;

    pxSpriteAnimator->Info.Behaviour |= PXResourceInfoActive;

    pxSpriteAnimator->Info.Behaviour = pxSpriteAnimatorInfo->Behaviour;
    pxSpriteAnimator->SpriteTarget = pxSpriteAnimatorInfo->SpriteTarget;
    pxSpriteAnimator->RateUpdate = pxSpriteAnimatorInfo->UpdateRate;
    pxSpriteAnimator->TimeStampAmount = pxSpriteAnimatorInfo->TimeStampAmount;

    pxSpriteAnimator->TimeStampList = PXMemoryHeapCallocT(PXSpriteAnimatorTimeStamp, pxSpriteAnimatorInfo->TimeStampAmount);
    PXCopyList(PXSpriteAnimatorTimeStamp, pxSpriteAnimatorInfo->TimeStampAmount, pxSpriteAnimatorInfo->TimeStampList, pxSpriteAnimator->TimeStampList);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Create",
        "SpriteAnimator"
    );
#endif


    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateHitBox(PXResourceCreateInfo* const pxResourceCreateInfo, PXHitBox* const pxHitBox)
{
    pxHitBox->Info.Behaviour |= PXResourceInfoActive;
    pxHitBox->Info.Behaviour = pxResourceCreateInfo->HitBox.Behaviour;
    pxHitBox->Model = pxResourceCreateInfo->HitBox.Model;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "HitBox-Create",
        "PXID:%i",
        pxHitBox->Info.ID
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateSound(PXResourceCreateInfo* const pxResourceCreateInfo, PXSound* const pxSound)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Sound-Create",
        "PXID:%i, %s",
        pxSound->Info.ID,
        pxResourceCreateInfo->FilePath
    );
#endif

    PXResourceTransphereInfo pxResourceLoadInfo;
    PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
    pxResourceLoadInfo.ResourceTarget = pxSound;
    pxResourceLoadInfo.ResourceType = PXResourceTypeSound;

    const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePath);

    if(PXActionSuccessful != loadResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXResourceManagerText,
            "Sound-Load",
            "failed <%s>!",
            pxResourceCreateInfo->FilePath
        );
#endif
    }

    PXActionReturnOnError(loadResult);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Sound-Load",
        "successful <%s>.",
        pxResourceCreateInfo->FilePath
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateTimer(PXResourceCreateInfo* const pxResourceCreateInfo, PXEngineTimer* const pxEngineTimer)
{
    pxEngineTimer->Owner = pxEngineTimer->Owner;
    pxEngineTimer->CallBack = pxEngineTimer->CallBack;
    pxEngineTimer->TimeDeltaTarget = pxEngineTimer->TimeDeltaTarget;



#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Timer-Create",
        "PXID:%i",
        pxEngineTimer->Info.ID
    );
#endif

    pxEngineTimer->TimeStampStart = PXTimeCounterStampGet();

    //---------------------------------------------
    // Trigger enable
    //---------------------------------------------
    if(PXResourceCreateBehaviourSpawnInScene & pxResourceCreateInfo->Flags)
    {
#if 0
        PXEngineResourceActionInfo pxEngineResourceActionInfo;
        PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

        pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
        pxEngineResourceActionInfo.ChangeInfo.Enable = PXTrue;
        pxEngineResourceActionInfo.ChangeInfo.Type = PXEngineCreateTypeTimer;
        pxEngineResourceActionInfo.ChangeInfo.Object = pxEngineTimer;

        PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
#endif
    }
    //---------------------------------------------

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceCreateWindow(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindow* const pxWindow)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceManagerAdd(PXResourceCreateInfo* const pxResourceCreateInfo)
{
    if(!pxResourceCreateInfo)
    {
        return PXActionRefusedArgumentNull;
    }

    //-----------------------------------------------------
    // Pre-create Object
    //-----------------------------------------------------
    const PXResourceEntry* const pxResourceEntry = &_GlobalResourceEntryList[pxResourceCreateInfo->Type];
    const PXBool isResourceAllocated = *pxResourceCreateInfo->ObjectReference > 0;

    if(!isResourceAllocated)
    {
        // Validation
        {
            if(!pxResourceEntry->LookupTable)
            {
                // TODO: Add behaviour to handle missing lookup

            }
        }


        PXLockEngage(&_GLOBALResourceManager.CreateLock);

        // Special behaviour if we have an object size of 0.
        // Creating 0 objects does not make sense but if we dont set this, it will yield problems
        // To reduce useless definition of "1 object size", we assume 0 means we want one item
        if(pxResourceCreateInfo->ObjectAmount == 0)
        {
            ++pxResourceCreateInfo->ObjectAmount; // Asumme we want one item it we didnt set it to it,
        }

        void* objectList = PXMemoryHeapCalloc(PXNull, pxResourceEntry->TypeSize, pxResourceCreateInfo->ObjectAmount);

        if(!objectList)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Register",
                "Failed to allocate space for : Size:%-4i (%i) <%s>",
                pxResourceEntry->TypeSize,
                pxResourceCreateInfo->ObjectAmount,
                pxResourceEntry->Name
            );
#endif

            return PXActionFailedMemoryAllocation;
        }

        *pxResourceCreateInfo->ObjectReference = objectList;

        for(PXSize i = 0; i < pxResourceCreateInfo->ObjectAmount; ++i)
        {
            const PXInt32U resourceID = PXResourceManagerGenerateUniqeID();

            // Get currect objects is we have multible
            void* object = (char*)objectList + (pxResourceEntry->TypeSize * i);

            // UNSTANBLE CAST?
            PXResourceInfo* const pxResourceInfo = (PXResourceInfo*)object;
            pxResourceInfo->ID = resourceID;
            pxResourceInfo->Behaviour |= PXResourceInfoExist;

            // Store myself, so we can cast back with hirachy
            pxResourceInfo->Hierarchy.Yourself = object;

            PXDictionaryAdd(pxResourceEntry->LookupTable, &resourceID, *pxResourceCreateInfo->ObjectReference);           
       

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXResourceManagerText,
                "Register",
                "PXID:%-4i Size:%-4i (%i/%i) <%s>",
                resourceID,
                pxResourceEntry->TypeSize,
                i+1,
                pxResourceCreateInfo->ObjectAmount,
                pxResourceEntry->Name
            );
#endif
        }

        PXLockRelease(&_GLOBALResourceManager.CreateLock);



        //-----------------------------------------------------
        // Create ASYNC pathway
        //-----------------------------------------------------
        const PXBool createSubCall =
            (PXResourceCreateBehaviourLoadASYNC & pxResourceCreateInfo->Flags) &&
            !(PXResourceCreateBehaviourIsASYNCCall & pxResourceCreateInfo->Flags);

        if(createSubCall && 0)
        {
            // We want to load the resouce in an async way.
            // Start a thread and call this function again?

            PXResourceCreateInfo* pxResourceCreateInfoASYNC = PXMemoryHeapCallocT(PXResourceCreateInfo, 1);
            PXCopy(PXResourceCreateInfo, pxResourceCreateInfo, pxResourceCreateInfoASYNC);
            pxResourceCreateInfoASYNC->Flags |= PXResourceCreateBehaviourIsASYNCCall;

            PXThreadPoolQueueWork
            (
                PXNull,
                PXResourceManagerAdd,
                pxResourceCreateInfoASYNC,
                PXNull,
                PXTaskParameterRelease | PXTaskExecuteASYNC
            );

            return PXActionSuccessful;
        }
    }
    //-----------------------------------------------------

    // Invoke creation
    if(!pxResourceEntry->CreateFunction)
    {
        // If there is no handle function, we cant do anything. 
        // We regard this as an error
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXResourceManagerText,
            "Register",
            "[PXID:%-4i] <%s> Has no function to handle creation.",
            -1,
            pxResourceEntry->Name
        );
#endif

        return PXActionRefusedObjectCreateCallbackMissing;
    }

    const PXActionResult pxActionCreate = pxResourceEntry->CreateFunction(pxResourceCreateInfo, *pxResourceCreateInfo->ObjectReference);

    if(PXActionSuccessful != pxActionCreate)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXResourceManagerText,
            "Create",
            "[PXID:%-4i] <%s> failed creation.",
            -1,
            pxResourceEntry->Name
        );
#endif
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceManagerAddV(PXResourceCreateInfo* const pxResourceCreateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXResourceCreateInfo* const pxResourceCreateInfo = &pxResourceCreateInfoList[i];

        PXResourceManagerAdd(pxResourceCreateInfo);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceStoreName(PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize)
{
    // NULL Check
    if(!pxResourceInfo)
    {
        return PXActionRefusedArgumentNull;
    }

    const PXBool callValid = name && !(0 == nameSize || (PXSize)-1 == nameSize);

    if(!callValid)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXResourceManagerText,
            "Store-Name",
            "PXID:%i, Name can't be stored!",
            pxResourceInfo->ID
        );
#endif

        return PXActionRefusedArgumentInvalid;
    }


    PXSize length = nameSize;

    if(-1 == nameSize)
    {
        length = PXTextLengthA(name, nameSize);
    }

#if PXLogEnable
    char buffer[256];

    PXTextCopyA(name, nameSize, buffer, length);

    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Store-Name",
        "PXID:%i, %s (%i)",
        pxResourceInfo->ID,
        buffer,
        length
    );
#endif

    PXListDynamicAdd(&_GLOBALResourceManager.NameCache, &pxResourceInfo->ID, name, length);

    pxResourceInfo->Behaviour |= PXResourceInfoHasName;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceStorePath(PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize)
{
    PXSize length = nameSize;

    if(-1 == nameSize)
    {
        length = PXTextLengthA(name, nameSize);
    }

#if PXLogEnable
    char buffer[256];

    PXTextCopyA(name, nameSize, buffer, nameSize);

    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Store-Path",
        "%s",
        buffer
    );
#endif

    PXListDynamicAdd(&_GLOBALResourceManager.NameCache, &pxResourceInfo->ID, name, length);

    pxResourceInfo->Behaviour |= PXResourceInfoHasSource;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceFetchName(PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize)
{
    PXListDynamicGet(&_GLOBALResourceManager.NameCache, &pxResourceInfo->ID, name, nameSize);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceFetchPath(PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize)
{
    PXListDynamicGet(&_GLOBALResourceManager.SourcePathCache, &pxResourceInfo->ID, name, nameSize);

    return PXActionSuccessful;
}

void PXAPI PXIconLoad(PXIcon* const pxIcon)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    pxIcon->Info.Handle.IconHandle = LoadIcon(NULL, IDI_EXCLAMATION);
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXRectangleLTRBI32ToXYWHI32(const PXRectangleLTRBI32* const pxRectangleLTRBI32, PXRectangleXYWHI32* const pxRectangleXYWHI32)
{
    pxRectangleXYWHI32->X = pxRectangleLTRBI32->Left;
    pxRectangleXYWHI32->Y = pxRectangleLTRBI32->Top;
    pxRectangleXYWHI32->Width = pxRectangleLTRBI32->Right - pxRectangleLTRBI32->Left;
    pxRectangleXYWHI32->Height = pxRectangleLTRBI32->Bottom - pxRectangleLTRBI32->Top;
}

void PXAPI PXRectangleXYWHI32ToLTRBI32(const PXRectangleXYWHI32* const pxRectangleXYWHI32, PXRectangleLTRBI32* const pxRectangleLTRBI32)
{
    pxRectangleLTRBI32->Left = pxRectangleXYWHI32->X;
    pxRectangleLTRBI32->Top = pxRectangleXYWHI32->Y;
    pxRectangleLTRBI32->Right = pxRectangleXYWHI32->X + pxRectangleXYWHI32->Width;
    pxRectangleLTRBI32->Bottom = pxRectangleXYWHI32->Y + pxRectangleXYWHI32->Height;
}

PXMaterial* PXAPI PXMaterialContainerFind(const PXMaterialContainer* const pxMaterialContainer, struct PXText_* const pxMaterialName)
{
    if(!pxMaterialContainer)
    {
        return PXNull;
    }

    //for (PXSize materialContainerID = 0; materialContainerID < pxMaterialContainer->MaterialContaierListSize; ++materialContainerID)
    //{
    // const PXMaterialContainer* const pxMaterialContaier = &pxModel->MaterialContaierList[materialContainerID];

    for(PXSize materialID = 0; materialID < pxMaterialContainer->MaterialListAmount; ++materialID)
    {
        PXMaterial* const pxMaterial = &pxMaterialContainer->MaterialList[materialID];
        char* name = 0;
        PXSize nameSize = 0;

        PXResourceFetchName(&pxMaterial->Info, &name, &nameSize);

        const PXBool isMatch = PXTextCompareA(pxMaterialName->TextA, pxMaterialName->SizeUsed, name, nameSize);

        if(isMatch)
        {
            return pxMaterial;
        }
    }
    // }

    return PXNull;
}

void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferFormat pxVertexBufferFormat, const PXSize index)
{
    const PXInt8U stride = PXVertexBufferFormatStrideSize(pxVertexBuffer->Format);
    const PXSize rowEntiry = stride * index;

    return 0;

#if 0
    switch(pxVertexBuffer->Format)
    {
        case PXVertexBufferFormatT2F_XYZ:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 2];

                case PXVertexBufferDataTypeTexture:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatT2F_N3F_XYZ:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 2 + 3];

                case PXVertexBufferDataTypeTexture:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                case PXVertexBufferDataTypeNormal:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 2];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatXYZPXF32:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }
        case PXVertexBufferFormatN3F_XYZ:
        {
            switch(pxVertexBufferDataType)
            {
                case PXVertexBufferDataTypeVertex:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 3];

                case PXVertexBufferDataTypeNormal:
                    return &((PXF32*)pxVertexBuffer->VertexData)[rowEntiry + 0];

                default:
                    return PXNull;
            }
        }

        default:
            return PXNull;
    }
#endif
}

void PXAPI PXIndexBufferPrepare(PXIndexBuffer* const pxIndexBuffer, const PXSize amountVertex, const PXSize amountMaterials)
{
    // Index array data Type
    {
        if(amountVertex <= 0xFFu)
        {
            // 8-Bit
            pxIndexBuffer->IndexDataType = PXTypeInt08U;
        }
        else if(amountVertex <= 0xFFFFu)
        {
            // 16-Bit
            pxIndexBuffer->IndexDataType = PXTypeInt16U;
        }
        else if(amountVertex <= 0xFFFFFFFFu)
        {
            // 32-Bit
            pxIndexBuffer->IndexDataType = PXTypeInt32U;
        }
        else
        {
            // 64-Bit
            pxIndexBuffer->IndexDataType = PXTypeInt64U;
        }

        const PXSize dataSize = PXTypeSizeGet(pxIndexBuffer->IndexDataType);

        pxIndexBuffer->DataIndexAmount = amountVertex;
        pxIndexBuffer->DataIndexSizeSegment = dataSize * amountVertex;
        pxIndexBuffer->DataIndexSizeTotal = dataSize * amountVertex * 3;
        pxIndexBuffer->Data = PXMemoryHeapCalloc(PXNull, amountVertex * 3, dataSize);
  
        pxIndexBuffer->DataIndexPosition = pxIndexBuffer->Data;
        pxIndexBuffer->DataIndexNormal = (char*)pxIndexBuffer->DataIndexPosition + pxIndexBuffer->DataIndexSizeSegment;
        pxIndexBuffer->DataIndexTexturePos = (char*)pxIndexBuffer->DataIndexNormal + pxIndexBuffer->DataIndexSizeSegment;
    }

    pxIndexBuffer->SegmentListAmount = amountMaterials;
    pxIndexBuffer->SegmentList = PXMemoryHeapCallocT(PXIndexSegment, amountMaterials);


#if PXLogEnable
    const PXSize dataSize = PXTypeSizeGet(pxIndexBuffer->IndexDataType);

    PXLogPrint
    (
        PXLoggingInfo,
        "IndeBuffer",
        "prepare",
        "PXID:%i, TypeSize:%i, VertexAmount:%i, Materials:%i",
        pxIndexBuffer->Info.Handle.OpenGLID,
        dataSize,
        amountVertex,
        amountMaterials
    );
#endif
}

PXBool PXAPI PXIndexBufferIsUsed(const PXIndexBuffer* const pxIndexBuffer)
{
    return pxIndexBuffer->Info.Handle.OpenGLID != 0;
}

PXSize PXAPI PXMeshTriangleAmount(PXMesh* const pxMesh)
{
    /*
    const PXInt8U stride = PXVertexBufferFormatStrideSize(pxMesh->VertexBuffer.Format);
    PXSize amount = (pxMesh->VertexBuffer.VertexDataSize / stride) / 3;

    return amount;
    */

    return 0;
}

PXF32* PXAPI PXMeshTriangleIndex(PXMesh* const pxMesh, const PXSize index)
{
    //PXVertexBufferInsertionPoint();

    return 0;// PXPublic PXF32* PXAPI();
}

void PXAPI PXModelConstruct(PXModel* const pxModel)
{
    PXClear(PXModel, pxModel);

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);

    // PXRectangleOffsetSet(&pxModel->Margin, 1, 1, 1, 1);
}

void PXAPI PXModelDestruct(PXModel* const pxModel)
{

}

void PXAPI PXModelFormatTransmute(PXModel* const pxModel, PXModelFormatTransmuteInfo* const pxModelFormatTransmuteInfo)
{
#if 1
    //-----------------------------------------------------
    // Vertex
    //-----------------------------------------------------
    
    PXMesh* pxMesh = &pxModel->Mesh;
    const PXVertexBufferFormat oldFormat = pxMesh->VertexBufferPrime.Format;


    if(1 == pxMesh->VertexBufferListAmount)
    {
        PXVertexBuffer* const pxVertexBuffer = &pxMesh->VertexBufferPrime;

        switch(pxVertexBuffer->Format)
        {
            case PXVertexBufferFormatP2I8:
            {
                PXSize amountCurrent = PXVertexBufferFormatStrideSize(PXVertexBufferFormatP2I8);
                PXSize amountFuture = PXVertexBufferFormatStrideSize(PXVertexBufferFormatT2F_XYZ);
                PXSize sizeBefore = pxVertexBuffer->VertexDataSize;
                PXSize sizeCurrent = (pxVertexBuffer->VertexDataSize / 2) * amountFuture;

                // Store old data
                PXInt8S* dataOld = (PXInt8S*)pxVertexBuffer->VertexData;

                pxVertexBuffer->Format = PXVertexBufferFormatT2F_XYZ;
                pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXF32, sizeCurrent);;
                pxVertexBuffer->VertexDataSize = sizeof(PXF32) * sizeCurrent;

                PXF32* dataNew = (PXF32*)pxVertexBuffer->VertexData;

                PXSize newOffset = 0;

                for(PXSize i = 0; i < sizeBefore; i += 2)
                {
                    dataNew[newOffset++] = (dataOld[i + 0] + 1) / 2.0f;
                    dataNew[newOffset++] = -(dataOld[i + 1] + 1) / 2.0f;
                    dataNew[newOffset++] = dataOld[i + 0];
                    dataNew[newOffset++] = dataOld[i + 1];
                    dataNew[newOffset++] = 0.0f;
                }

                // Memory leak? dataOld needs to be deleted? But what if its read only?         

                break;
            }
            case PXVertexBufferFormatP3I8:
            {
                const PXSize amountCurrent = PXVertexBufferFormatStrideSize(PXVertexBufferFormatP3I8);
                const PXSize amountFuture = PXVertexBufferFormatStrideSize(PXVertexBufferFormatP3F32);
                PXSize sizeCurrent = pxVertexBuffer->VertexDataSize / 1;

                const PXSize newVertexArraySize = sizeof(PXF32) * sizeCurrent;
                PXF32* newVertexArray = PXMemoryHeapCallocT(PXF32, sizeCurrent);

                PXInt8S* dataSource = (PXInt8S*)pxVertexBuffer->VertexData;

                for(size_t i = 0; i < sizeCurrent; i++)
                {
                    newVertexArray[i] = dataSource[i];
                }

                // Memory leak?

                pxVertexBuffer->Format = PXVertexBufferFormatP3F32;
                pxVertexBuffer->VertexData = newVertexArray;
                pxVertexBuffer->VertexDataSize = newVertexArraySize;


                break;
            }
            default:
            {
                break;
            }
        }

        PXVertexBufferFormat newFormat = pxVertexBuffer->Format;

        const char* oldFomatText = PXVertexBufferFormatToString(oldFormat);
        const char* newFomatText = PXVertexBufferFormatToString(newFormat);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Model-Format",
            "Transmute <%s> to <%s>",
            oldFomatText,
            newFomatText
        );
#endif

    }

#endif

}

void PXAPI PXModelNormalDataGenerate(PXModel* const pxModel)
{
    /*
    for(PXSize i = 0; i < counterVertex; ++i)
    {
        PXF32* const positionData = (PXF32*)PXVertexBufferInsertionPoint(&pxModel->Mesh.VertexBuffer, PXVertexBufferDataTypeNormal, i);
        PXF32* const normalData = (PXF32*)PXVertexBufferInsertionPoint(&pxModel->Mesh.VertexBuffer, PXVertexBufferDataTypeVertex, i);

        PXVector3F normalVector;
        PXVector3F positionVector =
        {
            positionData[0],
            positionData[1],
            positionData[2]
        };
        const PXVector3F positionVectorConst = { 1,1,1 };

        PXF32 normalFactor = PXVector3FDotProduct(&positionVectorConst, &positionVector);

        if(normalFactor != 0.0)
        {
            normalFactor = 1.0f / PXMathRootSquare(normalFactor);
        }
        else
        {
            normalFactor = 0.0;
        }

        normalData[0] = normalFactor;
        normalData[1] = normalFactor;
        normalData[2] = normalFactor;
    }
    */
}

PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXInt32U characterID)
{
    PXFontPageCharacter* lastMatch = PXNull;
    PXBool match = PXFalse;

    if(!pxFontPage)
    {
        return PXNull;
    }

    for(PXSize i = 0; i < pxFontPage->CharacteListEntrys && !match; ++i)
    {
        const PXFontPageCharacter* const currentCharacter = &pxFontPage->CharacteList[i];

        match = currentCharacter->ID == characterID;

        lastMatch = (PXFontPageCharacter*)((PXSize)match * (PXSize)currentCharacter);
    }

    return lastMatch;
}

PXFontPage* PXAPI PXFontPageGet(PXFont* const pxFont, const PXSize index)
{
    if(!pxFont)
    {
        return PXNull; // PXActionRefusedArgumentNull
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

const char* PXAPI PXUIElementTypeToString(const PXUIElementType pxUIElementType)
{
    switch(pxUIElementType)
    {
        case PXUIElementTypeCustom:            return "Custom";
        case PXUIElementTypeWindow:            return "Window";
        case PXUIElementTypePanel:            return "Panel";
        case PXUIElementTypeText:            return "Text";
        case PXUIElementTypeButton:               return "Button";
        case PXUIElementTypeImage:                return "Image";
        case PXUIElementTypeDropDown:             return "DropDown";
        case PXUIElementTypeListBox:              return "ListBox";
        case PXUIElementTypeTextEdit:             return "TextEdit";
        case PXUIElementTypeRichEdit:             return "RichText";
        case PXUIElementTypeScrollBar:            return "ScrollBar";
        case PXUIElementTypeTrackBar:             return "TrackBar";
        case PXUIElementTypeStatusBar:            return "StatusBar";
        case PXUIElementTypeUpDown:               return "UpDown";
        case PXUIElementTypeProgressBar:          return "ProgressBar";
        case PXUIElementTypeHotKey:               return "HotKey";
        case PXUIElementTypeCalender:             return "Calender";
        case PXUIElementTypeToolTip:              return "ToolTip";
        case PXUIElementTypeAnimate:              return "Animate";
        case PXUIElementTypeDatePicker:           return "DatePicker";
        case PXUIElementTypeGroupBox:             return "GroupBox";
        case PXUIElementTypeRadioButton:          return "RadioButton";
        case PXUIElementTypeGroupRadioButton:     return "GroupRadioButton";
        case PXUIElementTypeTreeView:             return "TreeView";
        case PXUIElementTypeTreeViewItem:         return "TreeViewItem";
        case PXUIElementTypeFileDirectyView:      return "FileDirectyView";
        case PXUIElementTypeFileDirectyViewEntry: return "FileDirectyViewEntry";
        case PXUIElementTypeIPInput:              return "IPInput";
        case PXUIElementTypeLink:                 return "Link";
        case PXUIElementTypeHeader:               return "Header";
        case PXUIElementTypeFontSelector:         return "FontSelector";
        case PXUIElementTypePageScroll:           return "PageScroll";
        case PXUIElementTypeTabControll:          return "TabControll";
        case PXUIElementTypeTabPage:              return "TAB-Page";
        case PXUIElementTypeToggle:               return "Toggle";
        case PXUIElementTypeCheckBox:             return "CheckBox";
        case PXUIElementTypeComboBox:             return "ComboBox";
        case PXUIElementTypeColorPicker:          return "ColorPicker";
        case PXUIElementTypeSlider:               return "Slider";
        case PXUIElementTypeRenderFrame:          return "FrameRender";
        case PXUIElementTypeMenuStrip:            return "MenuStrip";
        case PXUIElementTypeResourceManger:       return "ResourceManger";
        case PXUIElementTypeResourceEntry:        return "ResourceEntry";

        default:
            return PXNull;
    }
}

void PXAPI PXWindowBrushColorSet(PXWindowBrush* const pxGUIElementBrush, const PXByte red, const PXByte green, const PXByte blue)
{
    pxGUIElementBrush->Info.Behaviour |= PXWindowBrushBehaviourColorEmbeded;
    pxGUIElementBrush->ColorDate.Red = red;
    pxGUIElementBrush->ColorDate.Green = green;
    pxGUIElementBrush->ColorDate.Blue = blue;
}

PXActionResult PXAPI PXVersionFromString(PXVersion* const pxVersion, char* versioNString)
{
    int versionMajor = 0;
    int versionMinor = 0;
    int versionBuild = 0;
    int versionPatch = 0;

    PXText pxTextVersion;
    PXTextConstructFromAdressA(&pxTextVersion, versioNString, 0, PXTextUnkownLength);

    PXSize offset = 0;

    offset = PXTextToInt(&pxTextVersion, &versionMajor);

    if(offset != -1)
    {
        PXTextAdvance(&pxTextVersion, offset + 1u); // dot
        offset = PXTextToInt(&pxTextVersion, &versionMinor);

        if(offset != -1)
        {
            PXTextAdvance(&pxTextVersion, offset + 1u); // dot
            PXTextToInt(&pxTextVersion, &versionBuild);
        }
    }

    pxVersion->Major = versionMajor;
    pxVersion->Minor = versionMinor;
    pxVersion->Build = versionBuild;
    pxVersion->Patch = versionPatch;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXVersionToString(PXVersion* const pxVersion, char* versioNString)
{
    PXTextPrintA(versioNString, 64, "%i.%i.%i", pxVersion->Major, pxVersion->Minor, pxVersion->Build);

    return PXActionSuccessful;
}

void PXAPI PXUIElementPositionCalculcate(PXWindow* const pxWindow, PXUIElementPositionCalulcateInfo* const pxUIElementPositionCalulcateInfo)
{
    if(!((PXWindowAllignIgnoreParentMargin & pxWindow->Info.Setting) > 0))
    {
        for
            (
            PXWindow* pxUIElementParent = (PXWindow*)pxWindow->Info.Hierarchy.Parrent;
            pxUIElementParent;
            pxUIElementParent = (PXWindow*)pxUIElementParent->Info.Hierarchy.Parrent
            )
        {
            pxUIElementPositionCalulcateInfo->MarginLeft += pxUIElementParent->Position.Margin.Left;
            pxUIElementPositionCalulcateInfo->MarginTop += pxUIElementParent->Position.Margin.Top;
            pxUIElementPositionCalulcateInfo->MarginRight += pxUIElementParent->Position.Margin.Right;
            pxUIElementPositionCalulcateInfo->MarginBottom += pxUIElementParent->Position.Margin.Bottom;
        }
    }
    else
    {
       // pxUIElementPositionCalulcateInfo->MarginLeft += 0.2;
       // pxUIElementPositionCalulcateInfo->MarginTop += 0.2;
       // pxUIElementPositionCalulcateInfo->MarginRight += 0.2;
       // pxUIElementPositionCalulcateInfo->MarginBottom += 0.2;
    }

    pxUIElementPositionCalulcateInfo->MarginLeft += pxWindow->Position.Margin.Left;
    pxUIElementPositionCalulcateInfo->MarginTop += pxWindow->Position.Margin.Top;
    pxUIElementPositionCalulcateInfo->MarginRight += pxWindow->Position.Margin.Right;
    pxUIElementPositionCalulcateInfo->MarginBottom += pxWindow->Position.Margin.Bottom;

    // Normalizied space for OpenGL
    pxUIElementPositionCalulcateInfo->AA = -1 + pxUIElementPositionCalulcateInfo->MarginLeft;
    pxUIElementPositionCalulcateInfo->BA = -1 + pxUIElementPositionCalulcateInfo->MarginTop;
    pxUIElementPositionCalulcateInfo->BB = +1 - pxUIElementPositionCalulcateInfo->MarginRight,
        pxUIElementPositionCalulcateInfo->AB = +1 - pxUIElementPositionCalulcateInfo->MarginBottom;

    PXF32 remLeft = (pxUIElementPositionCalulcateInfo->MarginLeft) * pxUIElementPositionCalulcateInfo->WindowWidth / 2.0f;
    PXF32 remTop = (pxUIElementPositionCalulcateInfo->MarginTop) * pxUIElementPositionCalulcateInfo->WindowHeight / 2.0f;
    PXF32 remRight = (pxUIElementPositionCalulcateInfo->MarginRight) * pxUIElementPositionCalulcateInfo->WindowWidth / 2.0f;
    PXF32 remBottom = (pxUIElementPositionCalulcateInfo->MarginBottom) * pxUIElementPositionCalulcateInfo->WindowHeight / 2.0f;


    // Add perspective scaling
    {
        PXF32 factorX = pxUIElementPositionCalulcateInfo->WindowWidth / (PXF32)pxUIElementPositionCalulcateInfo->WindowHeight;
        PXF32 factorY = pxUIElementPositionCalulcateInfo->WindowHeight / (PXF32)pxUIElementPositionCalulcateInfo->WindowWidth;

        remLeft += factorX;
        remTop += factorY;
        remRight -= factorX;
        remBottom -= factorY;
    }


    PXF32 mathWithScaling = pxUIElementPositionCalulcateInfo->WindowWidth - remRight - remLeft;
    PXF32 heightWithScaling = pxUIElementPositionCalulcateInfo->WindowHeight - remBottom - remTop;












    // XYWH for WindowsAPI stuff0


    if(PXWindowKeepWidth & pxWindow->Info.Setting)
    {
        pxUIElementPositionCalulcateInfo->Width = pxWindow->Position.Form.Width;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Width = mathWithScaling;
    }

    if(PXWindowKeepHeight & pxWindow->Info.Setting)
    {
        pxUIElementPositionCalulcateInfo->Height = pxWindow->Position.Form.Height;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Height = heightWithScaling;
    }


    switch(PXWindowAllignFlags & pxWindow->Info.Setting)
    {
        default:
        case PXWindowAllignLeft:
        {
            pxUIElementPositionCalulcateInfo->X = remLeft;
            pxUIElementPositionCalulcateInfo->Y = remTop;
            break;
        }
        case PXWindowAllignTop:
        {
            pxUIElementPositionCalulcateInfo->X = remLeft;
            pxUIElementPositionCalulcateInfo->Y = remTop;
            break;
        }
        case PXWindowAllignRight:
        {
            pxUIElementPositionCalulcateInfo->X = pxUIElementPositionCalulcateInfo->WindowWidth - (remRight + pxUIElementPositionCalulcateInfo->Width);
            pxUIElementPositionCalulcateInfo->Y = pxUIElementPositionCalulcateInfo->WindowHeight - (remBottom + pxUIElementPositionCalulcateInfo->Height);
            break;

        }
        case PXWindowAllignBottom:
        {
            pxUIElementPositionCalulcateInfo->X = remLeft;
            pxUIElementPositionCalulcateInfo->Y = pxUIElementPositionCalulcateInfo->WindowHeight - (remBottom + pxUIElementPositionCalulcateInfo->Height);
            break;
        }
    }

}

PXActionResult PXAPI PXFileTypeInfoProbe(PXResourceTransphereInfo* const pxFileTypeInfo, const PXText* const pxText)
{
    // Probe for file extension
    {
        pxFileTypeInfo->FormatExpected = PXFilePathExtensionDetectTry(pxText);
    }

    switch(pxFileTypeInfo->FormatExpected)
    {
        case PXFileFormatA3DS:
            pxFileTypeInfo->ResourceType = PXResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXAutodesk3DSLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXAutodesk3DSSafeFromFile;
            break;

        case PXFileFormatAAC:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXAACLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXAACSaveToFile;
            break;

        case PXFileFormatAVI:
            pxFileTypeInfo->ResourceType = PXResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXAVILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXAVISaveToFile;
            break;

        case PXFileFormatBitMap:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourcePeek = PXBitmapPeekFromFile;
            pxFileTypeInfo->ResourceLoad = PXBitmapLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXBitmapSaveToFile;
            break;

        case PXFileFormatCanonRaw3:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXCanonRaw3LoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCanonRaw3SaveToFile;
            break;

        case PXFileFormatC:
            pxFileTypeInfo->ResourceType = PXResourceTypeCodeDocument;
            pxFileTypeInfo->ResourceLoad = PXCLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCSaveToFile;
            break;

        case PXFileFormatCSharp:
            pxFileTypeInfo->ResourceType = PXResourceTypeCodeDocument;
            pxFileTypeInfo->ResourceLoad = PXCSharpLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCSharpSaveToFile;
            break;

        case PXFileFormatCSS:
            pxFileTypeInfo->ResourceType = PXResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXCSSLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCSSSaveToFile;
            break;

        case PXFileFormatCPP:
            pxFileTypeInfo->ResourceType = PXResourceTypeCodeDocument;
            pxFileTypeInfo->ResourceLoad = PXCPPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXCPPSaveToFile;
            break;

        case PXFileFormatBinaryWindows:
            pxFileTypeInfo->ResourceType = PXResourceTypeBinary;
            pxFileTypeInfo->ResourceLoad = PXBinaryWindowsLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXBinaryWindowsSaveToFile;
            break;

        case PXFileFormatBinaryLinux:
            pxFileTypeInfo->ResourceType = PXResourceTypeBinary;
            pxFileTypeInfo->ResourceLoad = PXBinaryLinuxLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXBinaryLinuxSaveToFile;
            break;

        case PXFileFormatDirectDrawSurfaceTexture:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXDDSLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXDDSSaveToFile;
            break;

        case PXFileFormatEML:
            pxFileTypeInfo->ResourceType = PXResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;

        case PXFileFormatFastFile:
            pxFileTypeInfo->ResourceType = PXResourceTypeArchiv;
            pxFileTypeInfo->ResourceLoad = PXFastFileLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXFastFileSaveToFile;
            break;

        case PXFileFormatFilmBox:
            pxFileTypeInfo->ResourceType = PXResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXFilmBoxLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXFilmBoxSaveToFile;
            break;

        case PXFileFormatFLAC:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXFLACLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXFLACSaveToFile;
            break;

        case PXFileFormatSpriteFont:
            pxFileTypeInfo->ResourceType = PXResourceTypeFont;
            pxFileTypeInfo->ResourceLoad = PXSpriteFontLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSpriteFontSaveToFile;
            break;

        case PXFileFormatGIF:
            pxFileTypeInfo->ResourceType = PXResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXGIFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXGIFSaveToFile;
            break;

        case PXFileFormatOpenGLShader:
            pxFileTypeInfo->ResourceType = PXResourceTypeShaderProgram;
            pxFileTypeInfo->ResourceLoad = PXGLSLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXGLSLSaveToFile;
            break;

        case PXFileFormatDirectXShader:
            pxFileTypeInfo->ResourceType = PXResourceTypeShaderProgram;
            pxFileTypeInfo->ResourceLoad = PXHLSLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXHLSLSaveToFile;
            break;

        case PXFileFormatHighEfficiencyImageFile:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXHEIFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXHEIFSaveToFile;
            break;

        case PXFileFormatHTML:
            pxFileTypeInfo->ResourceType = PXResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXHTMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXHTMLSaveToFile;
            break;

        case PXFileFormatINI:
            pxFileTypeInfo->ResourceType = PXResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXINILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXINISaveToFile;
            break;

        case PXFileFormatJava:
            pxFileTypeInfo->ResourceType = PXResourceTypeCodeDocument;
            pxFileTypeInfo->ResourceLoad = PXJavaLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXJavaSaveToFile;
            break;

        case PXFileFormatJavaScript:
            pxFileTypeInfo->ResourceType = PXResourceTypeCodeDocument;
            pxFileTypeInfo->ResourceLoad = PXJavaScriptLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXJavaScriptSaveToFile;
            break;

        case PXFileFormatJPEG:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXJPEGLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXJPEGSaveToFile;
            break;

        case PXFileFormatJSON:
            pxFileTypeInfo->ResourceType = PXResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXJSONLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXJSONSaveToFile;
            break;

        case PXFileFormatM4A:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;

        case PXFileFormatMIDI:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXMIDILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMIDISaveToFile;
            break;

        case PXFileFormatMP3:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXMP3LoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMP3SaveToFile;
            break;

        case PXFileFormatMP4:
            pxFileTypeInfo->ResourceType = PXResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXMP4LoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMP4SaveToFile;

        case PXFileFormatMSI:
            pxFileTypeInfo->ResourceType = PXResourceTypeInstaller;
            pxFileTypeInfo->ResourceLoad = PXMSILoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMSISaveToFile;

        case PXFileFormatMTL:
            pxFileTypeInfo->ResourceType = PXResourceTypeMaterialList;
            pxFileTypeInfo->ResourceLoad = PXMTLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMTLSaveToFile;
            break;

        case PXFileFormatN64:
            pxFileTypeInfo->ResourceType = PXResourceTypeBinary;
            pxFileTypeInfo->ResourceLoad = PXN64LoadFromFile;
            pxFileTypeInfo->ResourceSave = PXN64SaveToFile;
            break;

        case PXFileFormatWavefront:
            pxFileTypeInfo->ResourceType = PXResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXWavefrontLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWavefrontSaveFromFile;
            break;

        case PXFileFormatMatroska:
            pxFileTypeInfo->ResourceType = PXResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXMatroskaLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXMatroskaSaveToFile;
            break;

        case PXFileFormatOGG:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXOGGLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXOGGSaveToFile;
            break;

        case PXFileFormatEugeneRoshalArchive:
            pxFileTypeInfo->ResourceType = PXResourceTypeArchiv;
            pxFileTypeInfo->ResourceLoad = PXRARLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXRARSaveToFile;
            break;

        case PXFileFormatPDF:
            pxFileTypeInfo->ResourceType = PXResourceTypeCodeDocument;
            pxFileTypeInfo->ResourceLoad = PXPDFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPDFSaveToFile;
            break;

        case PXFileFormatPHP:
            pxFileTypeInfo->ResourceType = PXResourceTypeCodeDocument;
            pxFileTypeInfo->ResourceLoad = PXPHPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPHPSaveToFile;
            break;

        case PXFileFormatPLY:
            pxFileTypeInfo->ResourceType = PXResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXPLYLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPLYSaveToFile;
            break;

        case PXFileFormatPNG:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->FileSizePredict = PXPNGFilePredictSize;
            pxFileTypeInfo->ResourcePeek = PXPNGPeekFromFile;
            pxFileTypeInfo->ResourceLoad = PXPNGLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXPNGSaveToFile;
            break;

        case PXFileFormatQOI:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;

        case PXFileFormatSTEP:
            pxFileTypeInfo->ResourceType = PXResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXSTEPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSTEPSaveToFile;
            break;

        case PXFileFormatSTL:
            pxFileTypeInfo->ResourceType = PXResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXSTLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSTLSaveToFile;
            break;

        case PXFileFormatSVG:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXSVGLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXSVGSaveToFile;
            break;

        case PXFileFormatTAR:
            pxFileTypeInfo->ResourceType = PXResourceTypeArchiv;
            pxFileTypeInfo->ResourceLoad = PXTARLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXTARSaveToFile;
            break;

        case PXFileFormatTGA:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXTGALoadFromFile;
            pxFileTypeInfo->ResourceSave = PXTGASaveToFile;
            break;

        case PXFileFormatTagImage:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXTIFFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXTIFFSaveToFile;
            break;

        case PXFileFormatTrueTypeFont:
            pxFileTypeInfo->ResourceType = PXResourceTypeFont;
            pxFileTypeInfo->ResourceLoad = PXTTFLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXTTFSaveToFile;
            break;

        case PXFileFormatUniversalSceneDescription:
            pxFileTypeInfo->ResourceType = PXResourceTypeArchiv;
            pxFileTypeInfo->ResourceLoad = PXUSDLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXUSDSaveToFile;
            break;

        case PXFileFormatVideoObject:
            pxFileTypeInfo->ResourceType = PXResourceTypeVideo;
            pxFileTypeInfo->ResourceLoad = PXVOBLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXVOBLSaveToFile;
            break;

        case PXFileFormatVRML:
            pxFileTypeInfo->ResourceType = PXResourceTypeModel;
            pxFileTypeInfo->ResourceLoad = PXVRMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXVRMLSaveToFile;
            break;

        case PXFileFormatWave:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXWaveLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWaveSaveToFile;
            break;

        case PXFileFormatWEBM:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXWEBMLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWEBMSaveToFile;
            break;

        case PXFileFormatWEBP:
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXWEBPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWEBPSaveToFile;
            break;

        case PXFileFormatWMA:
            pxFileTypeInfo->ResourceType = PXResourceTypeSound;
            pxFileTypeInfo->ResourceLoad = PXWMALoadFromFile;
            pxFileTypeInfo->ResourceSave = PXWMASaveToFile;
            break;

        case PXFileFormatXML:
            pxFileTypeInfo->ResourceType = PXResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXXMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXXMLSaveToFile;
            break;

        case PXFileFormatYAML:
            pxFileTypeInfo->ResourceType = PXResourceTypeStructuredText;
            pxFileTypeInfo->ResourceLoad = PXYAMLLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXYAMLSaveToFile;
            break;

        case PXFileFormatZIP:
            pxFileTypeInfo->ResourceType = PXResourceTypeArchiv;
            pxFileTypeInfo->ResourceLoad = PXZIPLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXZIPSaveToFile;
            break;


        default:
            pxFileTypeInfo->ResourceType = PXResourceTypeCustom;
            pxFileTypeInfo->ResourceLoad = PXNull;
            pxFileTypeInfo->ResourceSave = PXNull;
            break;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceManagerReferenceValidate(PXResourceReference* const pxResourceReference)
{
    if(!pxResourceReference)
    {
        return PXActionRefusedArgumentNull;
    }

    // is ID refereing to "no object"?
    {
        PXBool isResourceReferenceWanted = -1 == pxResourceReference->IDExpected;

        if(!isResourceReferenceWanted)
        {
            pxResourceReference->ResourceAdress = PXNull; // Remove dependency if one is present. it is unwanted

            return PXActionSuccessful;
        }
    }

    // Check if adress is in range. aka is this a pointer in a valid range?
    {
        PXBool isAdressInRange = 1;

        if(!isAdressInRange)
        {
            pxResourceReference->ResourceAdress = PXNull;

            return PXActionSuccessful;
        }
    }
}

PXActionResult PXAPI PXResourceLoad(PXResourceTransphereInfo* const pxResourceLoadInfo, const PXText* const filePath)
{
    // Parameter exist check
    {
        if(!(pxResourceLoadInfo && filePath))
        {
            return PXActionRefusedArgumentNull;
        }
    }
    // parameter valid check
    {
        PXBool textValid = filePath->TextA && filePath->SizeUsed;

        if(!textValid)
        {
            return PXActionRefusedArgumentInvalid;
        }
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "File",
        "Load",
        "Begin for <%s>",
        filePath->TextA
    );
#endif

    // try to detect format over file extension
    PXFileTypeInfoProbe(pxResourceLoadInfo, filePath);


    // if we dont even have a clue how to load it, let the OS try to load it.
    {
        if(pxResourceLoadInfo->FormatExpected == PXFileFormatUnkown)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Load",
                "Refused : Format not detected"
            );
#endif

            return PXActionRefusedNotSupportedByLibrary;
        }

        if(!pxResourceLoadInfo->ResourceLoad)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Load",
                "Refused : Not implemented"
            );
#endif

            return PXActionRefusedNotImplemented;
        }
    }


    PXFile pxFile;

    pxResourceLoadInfo->FileReference = &pxFile;

    // Loading and map file if possible
    {
        PXFileOpenInfo pxFileOpenInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.FilePathAdress = filePath->TextA;
        pxFileOpenInfo.FilePathSize = filePath->SizeUsed;
        pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenInfo.FlagList = PXFileIOInfoAllowMapping | PXFileIOInfoFilePhysical;

        const PXActionResult fileLoadingResult = PXFileOpen(&pxFile, &pxFileOpenInfo);

        PXActionReturnOnError(fileLoadingResult);
    }

    // If a peek method exists, execute it, if not, strait to loading
    if(pxResourceLoadInfo->ResourcePeek)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Load-Peek",
            "Start..."
        );
#endif

        const PXInt64U timeStampA = PXTimeCounterStampGet();
        const PXActionResult pxPeekResult = pxResourceLoadInfo->ResourcePeek(pxResourceLoadInfo);
        const PXBool success = PXActionSuccessful == pxPeekResult;

        if(!success)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "File-Peek",
                "Failed"
            );
#endif
            return pxPeekResult;
        }

        const PXInt64U timeStampB = PXTimeCounterStampGet() - timeStampA;
        pxResourceLoadInfo->TimePeek = PXTimeCounterStampToSecoundsF(timeStampB);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Load-Peek",
            "Done! Took:%6.3fs",
            pxResourceLoadInfo->TimePeek
        );
#endif
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Load-Peek",
            "No peek function implemented. procceed.."
        );
#endif
    }

    // Preallocate on device
    if(pxResourceLoadInfo->OnDeviceDataRegister)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Load-PreAlloc",
            "Preallocate resource on device"
        );
#endif

        const PXInt64U timeStampA = PXTimeCounterStampGet();

        pxResourceLoadInfo->OnDeviceDataRegister(pxResourceLoadInfo);

        const PXInt64U timeStampB = PXTimeCounterStampGet() - timeStampA;

        pxResourceLoadInfo->TimeDeviceDataRegister = PXTimeCounterStampToSecoundsF(timeStampB);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Load-PreAlloc",
            "Preallocate took:<%6.3f>",
            pxResourceLoadInfo->TimeDeviceDataRegister
        );
#endif
    }

    // Try to load assumed format
    if(pxResourceLoadInfo->ResourceLoad)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Load-Extract",
            "Start..."
        );
#endif

        pxResourceLoadInfo->FileReference = &pxFile;

        const PXInt64U timeStampA = PXTimeCounterStampGet();

        const PXActionResult fileParsingResult = pxResourceLoadInfo->ResourceLoad(pxResourceLoadInfo);

        const PXInt64U timeStampB = PXTimeCounterStampGet() - timeStampA;

        pxResourceLoadInfo->TimeTransphere = PXTimeCounterStampToSecoundsF(timeStampB);



        PXFileClose(&pxFile);

        if(PXActionSuccessful != fileParsingResult)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Load-Extract",
                "Failed. Took:%6.3f  ROPs:%-7i <%s>",
                pxResourceLoadInfo->TimeTransphere,
                pxFile.CounterOperationsRead,
                filePath->TextA
            );
#endif

            return fileParsingResult;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Load-Extract",
            "Done! Took:%6.3f  ROPs:%-7i <%s>",
            pxResourceLoadInfo->TimeTransphere,
            pxFile.CounterOperationsRead,
            filePath->TextA
        );
#endif

        return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }
}

PXActionResult PXAPI PXResourceLoadA(PXResourceTransphereInfo* const pxResourceLoadInfo, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceLoad(pxResourceLoadInfo, &pxText);

    return loadResult;
}

PXActionResult PXAPI PXResourceSave(PXResourceTransphereInfo* const pxResourceSaveInfo, const PXText* const filePath)
{
    PXFile pxFile;

    // Loading file
    {
        PXFileOpenInfo pxFileIOInfo;
        pxFileIOInfo.FilePathAdress = filePath->TextA;
        pxFileIOInfo.FilePathSize = filePath->SizeUsed;
        pxFileIOInfo.AccessMode = PXAccessModeWriteOnly;
        pxFileIOInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileIOInfo.FlagList = PXFileIOInfoAllowMapping | PXFileIOInfoCreateIfNotExist;

        const PXActionResult fileLoadingResult = PXFileOpen(&pxFile, &pxFileIOInfo);

        PXActionReturnOnError(fileLoadingResult);
    }

    // Try to load assumed format
    {
        if(PXFileFormatUnkown == pxResourceSaveInfo->FormatExpected)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Save",
                "Refused : Format not known"
            );
#endif

            return PXActionRefusedNotSupportedByLibrary;
        }

        if(PXNull == pxResourceSaveInfo->ResourceSave)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceManagerText,
                "Save",
                "Refused : Not implemented"
            );
#endif

            return PXActionRefusedNotImplemented;
        }

#if PXLogEnable
        const PXInt64U timeStampA = PXTimeCounterStampGet();
#endif

        pxResourceSaveInfo->FileReference = &pxFile;

        const PXActionResult fileParsingResult = pxResourceSaveInfo->ResourceSave(pxResourceSaveInfo);

#if PXLogEnable
        const PXInt64U timeStampB = PXTimeCounterStampGet() - timeStampA;
        const PXF32 timeDelta = PXTimeCounterStampToSecoundsF(timeStampB);

        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Save",
            "%6.3fms SOPs:%-7i <%s>",
            timeDelta,
            pxFile.CounterOperationsWrite,
            filePath->TextA
        );
#endif

        // PXActionReturnOnSuccess(fileParsingResult); // Exit if this has worked first-try

        // return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }

    PXFileClose(&pxFile);

    pxResourceSaveInfo->FileReference = PXNull;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceSaveA(PXResourceTransphereInfo* const pxResourceSaveInfo, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceSave(pxResourceSaveInfo, &pxText);

    return loadResult;
}