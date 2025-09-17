#include "PXResource.h"

#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

PXResourceManager _pxResourceManager;

const char PXResourceText[] = "Resource";



const PXF32 PXVertexDataTriangle[] =
{
    -1.0f, -1.0f,
        1.0f, -1.0f,
        0.5f,  1.0f
};

const PXI8S PXIndexDataTriangle[] =
{
    0,1,2
};

// This only works if we dont use any texturepositions
const PXF32 PXVertexDataRectangle[] =
{
    -1, -1,
     1, -1,
     1,  1,
    -1,  1
};

const PXF32 PXVertexDataRectangleTX[] =
{
    -1, -1, 1, 1, // Bottom-left
     1, -1, 0, 1, // Bottom-right
     1,  1, 0, 0,  // Top-right
    -1,  1, 1, 0   // Top-left
};

const PXI8U PXIndexDataRectangle[] =
{
    0,1,2,
    2,3,0
};

const PXF32 PXVertexDataCube[] = // PXI8S
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

const PXI8U PXIndexDataCube[] =
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




#include <PX/Math/PXMath.h>
#include <PX/Media/PXText.h>
#include <PX/OS/Console/PXConsole.h>
#include <PX/OS/Time/PXStopWatch.h>
#include <PX/OS/Hardware/PXProcessor.h>
#include <PX/OS/File/PXFile.h>
#include <PX/OS/PXOS.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/Graphic/NativDraw/PXNativDraw.h>
#include <PX/OS/Async/PXThreadPool.h>
#include <PX/OS/Graphic/PXGraphic.h>

#include <PX/Media/Autodesk3DS/PXAutodesk3DS.h>
#include <PX/Media/AAC/PXAAC.h>
#include <PX/Media/AVI/PXAVI.h>
#include <PX/Media/Bitmap/PXBitmap.h>
#include <PX/Media/FilmBox/PXFilmBox.h>
#include <PX/Media/FLAC/PXFLAC.h>
#include <PX/Media/GIF/PXGIF.h>
#include <PX/Media/JPEG/PXJPEG.h>
#include <PX/Media/MIDI/PXMIDI.h>
#include <PX/Media/MP3/PXMP3.h>
#include <PX/Media/MP4/PXMP4.h>
#include <PX/Media/OGG/PXOGG.h>
#include <PX/Media/PLY/PXPLY.h>
#include <PX/Media/PNG/PXPNG.h>
#include <PX/Media/SpriteFont/PXSpriteFont.h>
#include <PX/Media/STEP/PXSTEP.h>
#include <PX/Media/STL/PXSTL.h>
#include <PX/Media/SVG/PXSVG.h>
#include <PX/Media/HEIF/PXHEIF.h>
#include <PX/Media/HTML/PXHTML.h>
#include <PX/Media/JSON/PXJSON.h>
#include <PX/Media/INI/PXINI.h>
#include <PX/Media/WEBM/PXWEBM.h>
#include <PX/Media/WEBP/PXWEBP.h>
#include <PX/Media/Wavefront/PXWavefront.h>
#include <PX/Media/Matroska/PXMatroska.h>
#include <PX/Media/PDF/PXPDF.h>
#include <PX/Media/TTF/PXTTF.h>
#include <PX/Media/VRML/PXVRML.h>
#include <PX/Media/N64/PXN64.h>
#include <PX/Media/PHP/PXPHP.h>
#include <PX/Media/MSI/PXMSI.h>
#include <PX/Media/Wave/PXWave.h>
#include <PX/Media/WMA/PXWMA.h>
#include <PX/Media/XML/PXXML.h>
#include <PX/Media/TIFF/PXTIFF.h>
#include <PX/Media/TGA/PXTGA.h>
#include <PX/Media/YAML/PXYAML.h>
#include <PX/Media/BinaryWindows/PXBinaryWindows.h>
#include <PX/Media/BinaryLinux/PXBinaryLinux.h>
#include <PX/Media/CanonRaw3/PXCanonRaw3.h>
#include <PX/Media/RAR/PXRAR.h>
#include <PX/Media/C/PXC.h>
#include <PX/Media/CSharp/PXCSharp.h>
#include <PX/Media/CPP/PXCPP.h>
#include <PX/Media/CSS/PXCSS.h>
#include <PX/Media/DDS/PXDDS.h>
#include <PX/Media/FastFile/PXFastFile.h>
#include <PX/Media/ADLER/PXAdler32.h>
#include <PX/Media/Java/PXJava.h>
#include <PX/Media/ZIP/PXZIP.h>
#include <PX/Media/USD/PXUSD.h>
#include <PX/Media/VOB/PXVOB.h>
#include <PX/Media/JavaScript/PXJavaScript.h>
#include <PX/Media/TAR/PXTAR.h>
#include <PX/Media/GLSL/PXGLSL.h>
#include <PX/Media/HLSL/PXHLSL.h>
#include <PX/Media/WAD/PXWAD.h>
#include <PX/Media/RTexture/PXRTexture.h>
#include <PX/Media/RGeometricMap/PXRGeometricMap.h>
#include <PX/Media/RGeometricWorld/PXRGeometricWorld.h>
#include <PX/Media/RSkin/PXRSkin.h>
#include <PX/Media/RedshiftMesh/PXRedshiftMesh.h>
#include <PX/Media/RedshiftSkeleton/PXRedshiftSkeleton.h>
#include <PX/Media/RedshiftAnimation/PXRedshiftAnimation.h>


const char PXResourceManagerText[] = "PX-Resource";

PXResourceManager _GLOBALResourceManager;





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

#if 0
void PXAPI PXVertexBufferFormatInfoExtract(PXVertexBufferFormatInfo* const pxVertexBufferFormatInfo, const PXVertexBufferFormat pxVertexBufferFormat)
{
    pxVertexBufferFormatInfo->VertexAttributesAmount = 0;

    pxVertexBufferFormatInfo->Format = PXVertexBufferFormatExtract(PXVertexBufferFormatFormat, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->Position = PXVertexBufferFormatExtract(PXVertexBufferFormatPosition, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->TexturePosition = PXVertexBufferFormatExtract(PXVertexBufferFormatTexturePosition, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->Normal = PXVertexBufferFormatExtract(PXVertexBufferFormatNormal, pxVertexBufferFormat);
    pxVertexBufferFormatInfo->Color = PXVertexBufferFormatExtract(PXVertexBufferFormatColor, pxVertexBufferFormat);

    char* buffer = pxVertexBufferFormatInfo->AsText;
    PXSize offset = 0;
    PXSize sizeLeft = 64;

    if(pxVertexBufferFormatInfo->Position > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "P%i ", pxVertexBufferFormatInfo->Position);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    if(pxVertexBufferFormatInfo->Normal > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "N%i ", pxVertexBufferFormatInfo->Normal);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    if(pxVertexBufferFormatInfo->Color > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "C%i ", pxVertexBufferFormatInfo->Color);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    if(pxVertexBufferFormatInfo->TexturePosition > 0)
    {
        offset += PXTextPrintA(&buffer[offset], sizeLeft, "T%i ", pxVertexBufferFormatInfo->TexturePosition);
        ++pxVertexBufferFormatInfo->VertexAttributesAmount;
    }

    switch(pxVertexBufferFormatInfo->Format)
    {
        case PXVertexBufferFormatI08:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "I8");
            pxVertexBufferFormatInfo->VertexElementSize = 1;
            break;

        case PXVertexBufferFormatI16:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "I16");
            pxVertexBufferFormatInfo->VertexElementSize = 2;
            break;

        case PXVertexBufferFormatI32:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "I32");
            pxVertexBufferFormatInfo->VertexElementSize = 4;
            break;

        case PXVertexBufferFormatF16:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "F16");
            pxVertexBufferFormatInfo->VertexElementSize = 2;
            break;

        case PXVertexBufferFormatF32:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "F32");
            pxVertexBufferFormatInfo->VertexElementSize = 4;
            break;

        default:
            offset += PXTextPrintA(&buffer[offset], sizeLeft, "ERROR");
            break;
    }


    // StrideSize
    pxVertexBufferFormatInfo->Stride =
        pxVertexBufferFormatInfo->Position +
        pxVertexBufferFormatInfo->TexturePosition +
        pxVertexBufferFormatInfo->Normal +
        pxVertexBufferFormatInfo->Color;
}
#endif

/*
PXI8U PXAPI PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat)
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

PXI8U PXAPI PXVertexBufferFormatSizePerVertex(const PXVertexBufferFormat pxVertexBufferFormat)
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
        {
            DebugBreak();
            return 0u;
        }
    }
}
*/

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



    PXMemoryCopy(data, bitmapData, size);

    return hBitmap;
}
#endif


// Resource lookup for directCall
const PXResourceEntry _GlobalResourceEntryList[] =
{
    {PXNull,                                PXNull,                                 "Invalid", PXResourceTypeInvalid, 0 },
    {PXNull,                                PXNull,                                 "Custom", PXResourceTypeCustom, 0 },
    {&_GLOBALResourceManager.SoundLookUp,   PXResourceCreateSound,                  "Sound",PXResourceTypeSound, sizeof(PXSound) },
    {PXNull,                                PXNull,                                 "Video", PXResourceTypeVideo,sizeof(PXVideo) },
    {&_GLOBALResourceManager.ModelLookUp,   PXResourceCreateModel,                  "Model",PXResourceTypeModel, sizeof(PXModel) },
    {&_GLOBALResourceManager.FontLookUp,    PXResourceCreateFont,                   "Font",PXResourceTypeFont, sizeof(PXFont) },
    {&_GLOBALResourceManager.MaterialLookUp, PXResourceCreateMaterial,              "Material", PXResourceTypeMaterial, sizeof(PXMaterial) },
    {&_GLOBALResourceManager.BrushLookUp,   PXResourceCreateBrush,                  "Brush",PXResourceTypeBrush, sizeof(PXWindowBrush) },
    {&_GLOBALResourceManager.TextureLookUp, PXResourceCreateTexture,              "Texture",PXResourceTypeTexture2D, sizeof(PXTexture) },
    {&_GLOBALResourceManager.ShaderProgramLookup, PXResourceCreateShaderProgram,    "ShaderProgram",PXResourceTypeShaderProgram, sizeof(PXShaderProgram) },
    {&_GLOBALResourceManager.SkyBoxLookUp,  PXResourceCreateSkybox,                 "SkyBox",PXResourceTypeSkybox, sizeof(PXSkyBox) },
    {&_GLOBALResourceManager.SpritelLookUp, PXResourceCreateSprite,                 "Sprite",PXResourceTypeSprite, sizeof(PXSprite) },
    {&_GLOBALResourceManager.IconLookUp,    PXResourceCreateIcon,                   "Icon",PXResourceTypeIcon, sizeof(PXIcon) },
    {&_GLOBALResourceManager.IconAtlasLookUp, PXResourceCreateIconAtlas,              "IconAtlas",PXResourceTypeIconAtlas, sizeof(PXIconAtlas) },
    {&_GLOBALResourceManager.SpriteAnimator, PXResourceCreateSpriteAnimator,        "SpriteAnimator",PXResourceTypeSpriteAnimator, sizeof(PXSpriteAnimator) },
    {PXNull,                                PXNull,                                 "Text",PXResourceTypeText, sizeof(PXText) },
    {&_GLOBALResourceManager.TimerLookUp,   PXResourceCreateTimer,                  "Timer",PXResourceTypeTimer, sizeof(PXTimer) },
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
    {PXNull, PXNull, "---", PXResourceTypeDialogBox, sizeof(PXWindowBrush) },
    {&_GLOBALResourceManager.SpriteMapAtlasLookUp, PXResourceCreateSpriteMap, "SpriteMap", PXResourceTypeSpriteMap, sizeof(PXSpriteMap) },
};
const PXSize _GlobalResourceEntryListAmoumt = sizeof(_GlobalResourceEntryList) / sizeof(PXResourceEntry);


PXResult PXAPI  PXResourceCreateSkybox(PXResourceCreateInfo* const pxResourceCreateInfo, PXSkyBox* const pxSkyBox)
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
        "Top", pxSkyBoxCreateEventData->TextureCube.Cube.Top.A,
        "Left", pxSkyBoxCreateEventData->TextureCube.Cube.Left.A,
        "Right", pxSkyBoxCreateEventData->TextureCube.Cube.Right.A,
        "Back", pxSkyBoxCreateEventData->TextureCube.Cube.Back.A,
        "Bottom", pxSkyBoxCreateEventData->TextureCube.Cube.Bottom.A,
        "Forward", pxSkyBoxCreateEventData->TextureCube.Cube.Front.A,
        "Vertex Shader", pxSkyBoxCreateEventData->ShaderProgram.ShaderVertexFilePath,
        "Pixel Shader", pxSkyBoxCreateEventData->ShaderProgram.ShaderPixelFilePath
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
        pxResourceCreateInfoList[0].Texture = pxSkyBoxCreateEventData->TextureCube;

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

PXResult PXAPI  PXResourceCreateBrush(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindowBrush* const pxWindowBrush)
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
        pxResourceCreateInfo->NameAdress
    );
#endif

    pxWindowBrush->Info.Behaviour |= PXResourceInfoExist;

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceCreateShaderProgram(PXResourceCreateInfo* const pxResourceCreateInfo, PXShaderProgram* const pxShaderProgram)
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
    const PXBool isDataPrensent = pxShaderProgramCreateData->ShaderVertex.A && pxShaderProgramCreateData->ShaderPixel.A;

    if(isDataPrensent)
    {
        PXFileOpenInfo pxFileOpenInfo;
        PXClear(PXFileOpenInfo, &pxFileOpenInfo);
        pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
        pxFileOpenInfo.BufferData = pxShaderProgramCreateData->ShaderVertex.A;
        pxFileOpenInfo.BufferSize = pxShaderProgramCreateData->ShaderVertex.SizeUsed;

        PXFileOpen(&pxShaderProgramCreateData->ShaderVertexFile, &pxFileOpenInfo);

        pxFileOpenInfo.BufferData = pxShaderProgramCreateData->ShaderPixel.A;
        pxFileOpenInfo.BufferSize = pxShaderProgramCreateData->ShaderPixel.SizeUsed;

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

        pxFileOpenFromPathInfo.FilePathAdress = pxShaderProgramCreateData->ShaderVertexFilePath.A;

        const PXActionResult vertexLoadResult = PXFileOpen(&pxShaderProgramCreateData->ShaderVertexFile, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(vertexLoadResult);


        pxFileOpenFromPathInfo.FilePathAdress = pxShaderProgramCreateData->ShaderPixelFilePath.A;

        const PXActionResult fragmentLoadResult = PXFileOpen(&pxShaderProgramCreateData->ShaderPixelFile, &pxFileOpenFromPathInfo);

        PXActionReturnOnError(fragmentLoadResult);

        pxShaderProgram->Info.Behaviour |= PXResourceInfoStorageMemory;

        PXResourceStorePath(&pxShaderProgram->Info, pxShaderProgramCreateData->ShaderVertexFilePath.A, -1);
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

PXResult PXAPI  PXResourceCreateIcon(PXResourceCreateInfo* const pxResourceCreateInfo, PXIcon* const pxIcon)
{
    PXIconCreateInfo* const pxIconCreateInfo = &pxResourceCreateInfo->Icon;

    PXResourceStoreName(&pxIcon->Info, pxResourceCreateInfo->NameAdress, -1);

    // Check if texture is present
    if(!pxIconCreateInfo->IconImage)
    {
        PXResourceCreateInfo pxResourceCreateInfoSub;

        PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.ObjectReference = (void**)&pxIconCreateInfo->IconImage;
        pxResourceCreateInfoSub.ObjectAmount = 1;
        pxResourceCreateInfoSub.FilePathAdress = pxResourceCreateInfo->FilePathAdress;
        pxResourceCreateInfoSub.FilePathSize = -1;
        pxResourceCreateInfoSub.Type = PXResourceTypeTexture;

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

PXResult PXAPI  PXResourceCreateFont(PXResourceCreateInfo* const pxResourceCreateInfo, PXFont* const pxFont)
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
            pxResourceCreateInfo->NameAdress,
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
            pxResourceCreateInfo->NameAdress,
            pxResourceCreateInfo->FilePathAdress

        );
#endif

        // Load font
        {
            PXResourceTransphereInfo pxResourceLoadInfo;
            PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
            pxResourceLoadInfo.ResourceTarget = pxFont;
            pxResourceLoadInfo.ResourceType = PXResourceTypeFont;
            pxResourceLoadInfo.Manager = &_GLOBALResourceManager;

            const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePathAdress);

            PXActionReturnOnError(loadResult);
        }
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceCreateMaterial(PXResourceCreateInfo* const pxResourceCreateInfo, PXMaterial* const pxMaterial)
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

PXResult PXAPI  PXResourceCreateIconAtlas(PXResourceCreateInfo* const pxResourceCreateInfo, PXIconAtlas* const pxIconAtlas)
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
        pxResourceCreateInfo->FilePathAdress,
        pxIconAtlasCreateInfo->CellSize
    );
#endif


    // Load image
    {
        PXResourceCreateInfo pxResourceCreateInfoSub;

        PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.ObjectReference = &pxIconAtlas->IconTexture2D;
        pxResourceCreateInfoSub.ObjectAmount = 1;
        pxResourceCreateInfoSub.FilePathAdress = pxResourceCreateInfo->FilePathAdress;
        pxResourceCreateInfoSub.FilePathSize = -1;
        pxResourceCreateInfoSub.Type = PXResourceTypeTexture2D;

        PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

    pxIconAtlasCreateInfo->CellAmountX = pxIconAtlas->IconTexture2D->Width / pxIconAtlasCreateInfo->CellSize;
    pxIconAtlasCreateInfo->CellAmountY = pxIconAtlas->IconTexture2D->Height / pxIconAtlasCreateInfo->CellSize;
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
        pxIconAtlas->IconTexture2D->Width,
        pxIconAtlas->IconTexture2D->Height,
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
            pxResourceCreateInfoSub.Icon.IconImage = pxIconAtlas->IconTexture2D;
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

PXResult PXAPI  PXResourceCreateTexture(PXResourceCreateInfo* const pxResourceCreateInfo, PXTexture* const pxTexture)
{
    switch(0)
    {
        case 0:
        {
            //PXTextureCreateInfo* const PXTextureCreateInfo = &pxResourceCreateInfo->Image;


            //  PXTexture->Info.ID = resourceID;

            PXText pxText;
            PXTextConstructFromAdressA(&pxText, pxResourceCreateInfo->FilePathAdress, PXTextLengthUnkown, PXTextLengthUnkown);
            const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePathAdress;
            PXI32U checkSum = 0;


            // Load texture
            if(hasFilePath)
            {
                PXResourceTransphereInfo pxResourceLoadInfo;
                PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.ResourceTarget = pxTexture;
                pxResourceLoadInfo.ResourceType = PXResourceTypeTexture;
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
                        pxText.A
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
                    pxText.A
                );
#endif
            }
            else
            {
                // PXTextureCopyAsNew(PXTexture, &PXTextureCreateInfo->Image);

                checkSum = 0;
            }
        }
        case 1:
        {
            PXTextureCreateInfo* const pxTextureCreateData = &pxResourceCreateInfo->Texture;

           // pxTexture->Format = PXColorFormatRGBI8;

            PXResourceCreateInfo pxResourceCreateInfoList[6];
            PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoList, 6);

            pxResourceCreateInfoList[0].Type = PXResourceTypeTexture;
            //pxResourceCreateInfoList[0].ObjectReference = (void**)&pxTexture->ImageA;
            pxResourceCreateInfoList[0].FilePathAdress = pxTextureCreateData->Cube.Top.A;

            pxResourceCreateInfoList[1].Type = PXResourceTypeTexture;
           // pxResourceCreateInfoList[1].ObjectReference = (void**)&pxTexture->ImageB;
            pxResourceCreateInfoList[1].FilePathAdress = pxTextureCreateData->Cube.Left.A;

            pxResourceCreateInfoList[2].Type = PXResourceTypeTexture;
           // pxResourceCreateInfoList[2].ObjectReference = (void**)&pxTexture->ImageC;
            pxResourceCreateInfoList[2].FilePathAdress = pxTextureCreateData->Cube.Right.A;

            pxResourceCreateInfoList[3].Type = PXResourceTypeTexture;
           // pxResourceCreateInfoList[3].ObjectReference = (void**)&pxTexture->ImageD;
            pxResourceCreateInfoList[3].FilePathAdress = pxTextureCreateData->Cube.Back.A;

            pxResourceCreateInfoList[4].Type = PXResourceTypeTexture;
           // pxResourceCreateInfoList[4].ObjectReference = (void**)&pxTexture->ImageE;
            pxResourceCreateInfoList[4].FilePathAdress = pxTextureCreateData->Cube.Bottom.A;

            pxResourceCreateInfoList[5].Type = PXResourceTypeTexture;
           // pxResourceCreateInfoList[5].ObjectReference = (void**)&pxTexture->ImageF;
            pxResourceCreateInfoList[5].FilePathAdress = pxTextureCreateData->Cube.Front.A;

            PXResourceManagerAddV(pxResourceCreateInfoList, 6);

            break;
        }
        case 2:
        {
            break;
        }

        default:
            break;
    }



    //const PXBool hasImageData = pxResourceCreateInfo->Texture.PixelData && pxResourceCreateInfo->Texture2D.Image.Image.PixelDataSize;

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

    pxTexture->Filter = PXRenderFilterNoFilter;
    pxTexture->LayoutNear = PXTextureLayoutNearest;
    pxTexture->LayoutFar = PXTextureLayoutNearest;
    pxTexture->WrapHeight = PXTextureWrapRepeat;
    pxTexture->WrapWidth = PXTextureWrapRepeat;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Create",
        "Texture2D PXID:%i <%s>.",
        pxTexture->Info.ID,
        pxResourceCreateInfo->FilePathAdress
    );
#endif

    {
        PXResourceCreateInfo pxResourceCreateInfoSub;
        PXClear(PXResourceCreateInfo, &pxResourceCreateInfoSub);
        pxResourceCreateInfoSub.Type = PXResourceTypeTexture;
       // pxResourceCreateInfoSub.ObjectReference = (void**)&pxTexture->Image;
        pxResourceCreateInfoSub.FilePathAdress = pxResourceCreateInfo->FilePathAdress;
       // pxResourceCreateInfoSub.Image = pxResourceCreateInfo->Texture2D.Image; // What is this?

        PXResourceManagerAdd(&pxResourceCreateInfoSub);
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceCreateModel(PXResourceCreateInfo* const pxResourceCreateInfo, PXModel* const pxModel)
{
    PXModelCreateInfo* const pxModelCreateInfo = &pxResourceCreateInfo->Model;

    PXModelConstruct(pxModel);

    PXMesh* const pxMesh = &pxModel->Mesh;
    pxMesh->VertexBufferListAmount = 1;
    PXVertexBuffer* const pxVertexBuffer = PXMeshVertexBufferListGET(pxMesh);
    PXIndexBuffer* const pxIndexBuffer = &pxMesh->IndexBuffer;

    PXMatrix4x4FIdentity(&pxModel->ModelMatrix);
    PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);

    const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePathAdress;

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
            pxResourceCreateInfo->FilePathAdress
        );
#endif

        PXResourceStorePath(&pxModel->Info, pxResourceCreateInfo->FilePathAdress, pxResourceCreateInfo->FilePathSize);

        // Load model
        {
            PXResourceTransphereInfo pxResourceLoadInfo;
            PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
            pxResourceLoadInfo.ResourceTarget = pxModel;
            pxResourceLoadInfo.ResourceType = PXResourceTypeModel;
            pxResourceLoadInfo.Manager = &_GLOBALResourceManager;

            const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePathAdress);
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
        // As this element is internal, we need to create an ID
        pxModel->Info.ID = PXResourceManagerGenerateUniqeID();

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
                /*
                if(!isDataConst)
                {
                    PXCopy(PXVertexBuffer, &pxModelCreateInfo->VertexBuffer, pxVertexBuffer);
                    PXCopy(PXIndexBuffer, &pxModelCreateInfo->IndexBuffer, pxIndexBuffer);

                    // Allocate memory and copy
                    pxIndexBuffer->DataIndexPosition = 0;
                    pxIndexBuffer->DataIndexSizeSegment = 0;
                    pxVertexBuffer->VertexData.Size = pxModelCreateInfo->VertexBuffer.VertexData.Size;
                    pxVertexBuffer->VertexData.Data = PXMemoryHeapCallocT(PXByte, pxModelCreateInfo->VertexBuffer.VertexData.Size, &pxModel->Mesh.VertexBuffer.VertexData, &pxModel->Mesh.VertexBuffer.VertexDataSize);

                    PXBufferAllocate(&pxModel->Mesh.IndexBuffer.Data, );

                    pxModel->Mesh.IndexBuffer.DataIndexSizeSegment = pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment;
                    pxModel->Mesh.IndexBuffer.DataIndexPosition = PXMemoryHeapCallocT
                    (
                        PXByte,
                        pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment,
                        &pxModel->Mesh.IndexBuffer.DataIndexPosition,
                        &pxModel->Mesh.IndexBuffer.DataIndexSizeSegment
                    );

                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->VertexBuffer.VertexData.Size,
                        pxModelCreateInfo->VertexBuffer.VertexData.Data,
                        pxVertexBuffer->VertexData.Data
                    );
                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->IndexBuffer.DataIndexSizeSegment,
                        pxModelCreateInfo->IndexBuffer.DataIndexPosition,
                        pxModel->Mesh.IndexBuffer.DataIndexPosition
                    );
                }
                */

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
                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;
                pxVertexBuffer->VertexData.Data = (void*)PXVertexDataTriangle;
                pxVertexBuffer->VertexData.Size = sizeof(PXVertexDataTriangle);

                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;
                pxIndexBuffer->Data.Data = (void*)PXIndexDataTriangle;
                pxIndexBuffer->Data.Size = sizeof(PXIndexDataTriangle);

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.Size;


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
                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;
                pxVertexBuffer->VertexData.Data = (void*)PXVertexDataRectangle;
                pxVertexBuffer->VertexData.Size = sizeof(PXVertexDataRectangle);

                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;
                pxIndexBuffer->Data.Data = (void*)PXIndexDataRectangle;
                pxIndexBuffer->Data.Size = sizeof(PXIndexDataRectangle);

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.Size;

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
            case PXModelFormRectangleTX:
            {
                pxVertexBuffer->LayoutAmount = 2;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32;
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;
                pxVertexBuffer->LayoutPrime[1].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[1].Format = PXTypeF32;
                pxVertexBuffer->LayoutPrime[1].Type = PXVertexBufferLayoutTypeTexturePos;
                pxVertexBuffer->VertexData.Data = (void*)PXVertexDataRectangleTX;
                pxVertexBuffer->VertexData.Size = sizeof(PXVertexDataRectangleTX);


                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;
                pxIndexBuffer->Data.Data = (void*)PXIndexDataRectangle;
                pxIndexBuffer->Data.Size = sizeof(PXIndexDataRectangle);

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.Size;

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Engine",
                    "Model-Create",
                    "From: RectangleTX"
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

                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 2;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeInt32S;
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;
                pxVertexBuffer->VertexData.Size = segmentAmount * 2;
                pxVertexBuffer->VertexData.Data = PXMemoryHeapCallocT(PXF32, pxVertexBuffer->VertexData.Size);

                PXF32* vertexData = (PXF32*)pxVertexBuffer->VertexData.Data;

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
                pxVertexBuffer->LayoutAmount = 1;
                pxVertexBuffer->LayoutPrime[0].AmountOfElements = 3;
                pxVertexBuffer->LayoutPrime[0].Format = PXTypeF32; // PXTypeInt08S
                pxVertexBuffer->LayoutPrime[0].Type = PXVertexBufferLayoutTypePosition;
                pxVertexBuffer->VertexData.Data = (void*)PXVertexDataCube;
                pxVertexBuffer->VertexData.Size = sizeof(PXVertexDataCube);

                pxIndexBuffer->DataType = PXTypeInt08U;
                pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle | PXDrawModeIDPoint | PXDrawModeIDLineLoop;;
                pxIndexBuffer->Data.Data = (void*)PXIndexDataCube;
                pxIndexBuffer->Data.Size = sizeof(PXIndexDataCube);
                pxIndexBuffer->LayoutListAmount = 1;
                pxIndexBuffer->LayoutPrime.AmountOfElements = 1;
                //pxIndexBuffer->LayoutPrime. = 1;
                pxIndexBuffer->LayoutPrime.AmountOfElements = 1;

                pxIndexBuffer->SegmentListAmount = 1;
                pxIndexBuffer->SegmentPrime.DataRange = pxIndexBuffer->Data.Size;




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


    // After loading, we need to transmute it 
    // into a format we can actually use
    PXMeshVertexLayoutTransmute(&pxModel->Mesh);


    pxModel->ShaderProgramReference = pxModelCreateInfo->ShaderProgramReference;

    pxModel->Info.Behaviour |= PXResourceInfoRender;

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceCreateSprite(PXResourceCreateInfo* const pxResourceCreateInfo, PXSprite* const pxSprite)
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
        pxResourceCreateInfo->FilePathAdress
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

        PXVector2F32SetXY(&pxSprite->TextureScalePositionOffset, 1, 1);
        PXVector2F32SetXY(&pxSprite->TextureScalePointOffset, 1, 1);

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

PXResult PXAPI  PXResourceCreateSpriteAnimator(PXResourceCreateInfo* const pxResourceCreateInfo, PXSpriteAnimator* const pxSpriteAnimator)
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

PXResult PXAPI  PXResourceCreateHitBox(PXResourceCreateInfo* const pxResourceCreateInfo, PXHitBox* const pxHitBox)
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

PXResult PXAPI  PXResourceCreateSound(PXResourceCreateInfo* const pxResourceCreateInfo, PXSound* const pxSound)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXResourceManagerText,
        "Sound-Create",
        "PXID:%i, %s",
        pxSound->Info.ID,
        pxResourceCreateInfo->FilePathAdress
    );
#endif

    PXResourceTransphereInfo pxResourceLoadInfo;
    PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
    pxResourceLoadInfo.ResourceTarget = pxSound;
    pxResourceLoadInfo.ResourceType = PXResourceTypeSound;

    const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePathAdress);

    if(PXActionSuccessful != loadResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXResourceManagerText,
            "Sound-Load",
            "failed <%s>!",
            pxResourceCreateInfo->FilePathAdress
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
        pxResourceCreateInfo->FilePathAdress
    );
#endif

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceCreateTimer(PXResourceCreateInfo* const pxResourceCreateInfo, PXTimer* const pxEngineTimer)
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

PXResult PXAPI  PXResourceCreateWindow(PXResourceCreateInfo* const pxResourceCreateInfo, PXWindow* const pxWindow)
{
    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceCreateSpriteMap(PXResourceCreateInfo* const pxResourceCreateInfo, PXSpriteMap* const pxSpriteMap)
{
    PXEngineSpriteMapInfo* const PXEngineSpriteMapInfo = &pxResourceCreateInfo->SpriteMap;

    // Create texture
    {
        PXResourceCreateInfo pxResourceCreateInfoList[2];
        PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoList, 2);

        // Skybox CubeTexture
        pxResourceCreateInfoList[0].Type = PXResourceTypeTexture2D;
        pxResourceCreateInfoList[0].ObjectReference = (void**)&pxSpriteMap->Texture;

        PXResourceManagerAdd(pxResourceCreateInfoList);
    }

    // Fetch model, a rectangle as a base.
    {
        PXResourceCreateInfo pxResourceCreateInfoList[2];
        PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoList, 2);

        // Skybox CubeTexture
        pxResourceCreateInfoList[0].Type = PXResourceTypeModel;
        pxResourceCreateInfoList[0].ObjectReference = (void**)&pxSpriteMap->Model;
        pxResourceCreateInfoList[0].Model.Form = PXModelFormRectangleTX;

        PXResourceManagerAdd(pxResourceCreateInfoList);
    }


    // Add a instanced vertex buffer.
    PXSize amountOfUniqeCells =
        pxSpriteMap->Texture->Width *
        pxSpriteMap->Texture->Height;

    PXSize amountOfLevelCells =
        PXEngineSpriteMapInfo->MapSize.X * PXEngineSpriteMapInfo->MapSize.Y;

    PXSize size = sizeof(PXSpriteMapEntity) * amountOfLevelCells;
    pxSpriteMap->SpriteMapEntityList = PXMemoryHeapCallocT(PXSpriteMapEntity, amountOfLevelCells);


    if(pxSpriteMap->Model)
    {
        PXModel* const pxModel = pxSpriteMap->Model;

        PXBufferLayout pxVertexBufferLayout[3];
        pxVertexBufferLayout[0].Format = PXTypeF32;
        pxVertexBufferLayout[0].AmountOfElements = 2;
        pxVertexBufferLayout[0].UpdatePerPrimitive = 1;

        pxVertexBufferLayout[1].Format = PXTypeF32;
        pxVertexBufferLayout[1].AmountOfElements = 2;
        pxVertexBufferLayout[1].UpdatePerPrimitive = 1;

        pxVertexBufferLayout[2].Format = PXTypeF32;
        pxVertexBufferLayout[2].AmountOfElements = 2;
        pxVertexBufferLayout[2].UpdatePerPrimitive = 1;

        PXMeshVertexArrayAdd
        (
            &pxModel->Mesh,
            pxSpriteMap->SpriteMapEntityList,
            size,
            pxVertexBufferLayout,
            3
        );

        // amountOfLevelCells
    }

    return PXActionSuccessful;
}

PXResourceEntry* PXResourceEntryGet(PXI32U type)
{
    for(size_t i = 0; i < _GlobalResourceEntryListAmoumt; i++)
    {
        const PXBool isTarget = _GlobalResourceEntryList[i].TypeID == type;

        if(isTarget)
        {
            return &_GlobalResourceEntryList[i];
        }
    }

    return PXNull;
}


PXResult PXAPI  PXResourceManagerAdd(PXResourceCreateInfo* const pxResourceCreateInfo)
{
    if(!pxResourceCreateInfo)
    {
        return PXActionRefusedArgumentNull;
    }

    //-----------------------------------------------------
    // Pre-create Object
    //-----------------------------------------------------

    // TODO: Bad provider funcion!
    const PXResourceEntry* const pxResourceEntry = PXResourceEntryGet(pxResourceCreateInfo->Type);

    if(!pxResourceEntry)
    {
        return PXActionRefusedArgumentInvalid;
    }

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


        PXLockEngage(&_GLOBALResourceManager.AsyncLock);

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
            const PXI32U resourceID = PXResourceManagerGenerateUniqeID();

            // Get currect objects is we have multible
            void* object = (char*)objectList + (pxResourceEntry->TypeSize * i);

            // UNSTANBLE CAST?
            PXResourceInfo* const pxResourceInfo = (PXResourceInfo*)object;
            pxResourceInfo->ID = resourceID;
            pxResourceInfo->Behaviour |= PXResourceInfoExist;

            // Store myself, so we can cast back with hirachy
            pxResourceInfo->Hierarchy.Yourself = object;

            PXDictionaryEntryAdd(pxResourceEntry->LookupTable, &resourceID, *pxResourceCreateInfo->ObjectReference);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXResourceManagerText,
                "Register",
                "PXID:<%-4i> Size:%-4i (%i/%i) <%s>",
                resourceID,
                pxResourceEntry->TypeSize,
                i + 1,
                pxResourceCreateInfo->ObjectAmount,
                pxResourceEntry->Name
            );
#endif
        }

        PXLockRelease(&_GLOBALResourceManager.AsyncLock);



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
                0,
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

PXResult PXAPI  PXResourceManagerAddV(PXResourceCreateInfo* const pxResourceCreateInfoList, const PXSize amount)
{
    for(PXSize i = 0; i < amount; ++i)
    {
        PXResourceCreateInfo* const pxResourceCreateInfo = &pxResourceCreateInfoList[i];

        PXResourceManagerAdd(pxResourceCreateInfo);
    }

    return PXActionSuccessful;
}

PXResult PXAPI PXResourceStoreName(PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize)
{
    return PXActionRefusedNotImplemented;
}

PXResult PXAPI  PXResourceStorePath(PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize)
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

PXResult PXAPI  PXResourceFetchName(PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize)
{
    PXListDynamicGet(&_GLOBALResourceManager.NameCache, &pxResourceInfo->ID, name, nameSize);

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceFetchPath(PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize)
{
    PXListDynamicGet(&_GLOBALResourceManager.SourcePathCache, &pxResourceInfo->ID, name, nameSize);

    return PXActionSuccessful;
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

        const PXBool isMatch = PXTextCompareA(pxMaterialName->A, pxMaterialName->SizeUsed, name, nameSize, PXTextCompareRequireSameLength);

        if(isMatch)
        {
            return pxMaterial;
        }
    }
    // }

    return PXNull;
}

PXBufferLayout* PXAPI PXVertexBufferLayoutGET(PXVertexBuffer* const pxVertexBuffer)
{
    if(PXEmbeddedArraySize >= pxVertexBuffer->LayoutAmount)
    {
        return &pxVertexBuffer->LayoutPrime;
    }
    else
    {
        return pxVertexBuffer->LayoutList;
    }
}

/*
void* PXAPI PXVertexBufferInsertionPoint(const PXVertexBuffer* const pxVertexBuffer, const PXVertexBufferFormat pxVertexBufferFormat, const PXSize index)
{
   // PXVertexBufferFormatInfo pxVertexBufferFormatInfo;
   // PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfo, pxVertexBuffer->Format);

    const PXI8U stride = 0;// pxVertexBufferFormatInfo.Stride;
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
*/

void PXAPI PXIndexBufferPrepare(PXIndexBuffer* const pxIndexBuffer, const PXSize amountVertex, const PXSize amountMaterials)
{
    // Index array data Type    
    if(amountVertex <= 0xFFu)
    {
        // 8-Bit
        pxIndexBuffer->DataType = PXTypeInt08U;
    }
    else if(amountVertex <= 0xFFFFu)
    {
        // 16-Bit
        pxIndexBuffer->DataType = PXTypeInt16U;
    }
    else if(amountVertex <= 0xFFFFFFFFu)
    {
        // 32-Bit
        pxIndexBuffer->DataType = PXTypeInt32U;
    }
    else
    {
        // 64-Bit
        pxIndexBuffer->DataType = PXTypeInt64U;
    }

    const PXSize dataSize = PXTypeSizeGet(pxIndexBuffer->DataType);

    PXBufferAllocate(&pxIndexBuffer->Data, amountVertex * 3 * dataSize);

    PXSize indexAmount = 3;

    pxIndexBuffer->LayoutListAmount = 3;
    pxIndexBuffer->LayoutList = PXMemoryHeapCallocT(PXBufferLayout, pxIndexBuffer->LayoutListAmount);

    pxIndexBuffer->LayoutList[0].Format = pxIndexBuffer->DataType;
    pxIndexBuffer->LayoutList[0].AmountOfElements = 1;
    pxIndexBuffer->LayoutList[0].Type = PXVertexBufferLayoutTypePosition;

    pxIndexBuffer->LayoutList[1].Format = pxIndexBuffer->DataType;
    pxIndexBuffer->LayoutList[1].AmountOfElements = 1;
    pxIndexBuffer->LayoutList[1].Type = PXVertexBufferLayoutTypeNormal;

    pxIndexBuffer->LayoutList[2].Format = pxIndexBuffer->DataType;
    pxIndexBuffer->LayoutList[2].AmountOfElements = 1;
    pxIndexBuffer->LayoutList[2].Type = PXVertexBufferLayoutTypeTexturePos;


    pxIndexBuffer->SegmentListAmount = amountMaterials;
    pxIndexBuffer->SegmentList = PXMemoryHeapCallocT(PXIndexSegment, amountMaterials);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "IndeBuffer",
        "prepare",
        "PXID:%i, TypeSize:%i, IndexAmount:%i, VertexAmount:%i, Materials:%i",
        pxIndexBuffer->Info.Handle.OpenGLID,
        dataSize,
        indexAmount,
        amountVertex,
        amountMaterials
    );
#endif
}

PXBool PXAPI PXIndexBufferIsUsed(const PXIndexBuffer* const pxIndexBuffer)
{
    return pxIndexBuffer->Info.Handle.OpenGLID != 0;
}

PXIndexSegment* PXAPI PXIndexBufferSegmentListGET(const PXIndexBuffer* const pxIndexBuffer)
{
    PXAssert(pxIndexBuffer->SegmentListAmount != 0, "This cant be 0");

    if(pxIndexBuffer->SegmentListAmount == 1)
    {
        return &pxIndexBuffer->SegmentPrime;
    }
    else
    {
        return pxIndexBuffer->SegmentList;
    }
}

PXBufferLayout* PXAPI PXIndexLayoutListGET(const PXIndexBuffer* const pxIndexBuffer)
{
    PXAssert(pxIndexBuffer->LayoutListAmount != 0, "This cant be 0");

    if(pxIndexBuffer->LayoutListAmount == 1)
    {
        return &pxIndexBuffer->LayoutPrime;
    }
    else
    {
        return pxIndexBuffer->LayoutList;
    }
}

PXSize PXAPI PXIndexIndexGET(const PXIndexBuffer* const pxIndexBuffer, const PXI8U type)
{
#if 1



#else


    PXBufferLayout* const pxBufferLayoutList = PXIndexLayoutListGET(pxIndexBuffer);

    for(size_t i = 0; i < pxIndexBuffer->LayoutListAmount; i++)
    {
        PXBufferLayout* const pxBufferLayout = &pxBufferLayoutList[i];

        if(type != pxBufferLayout->Type)
        {
            continue;
        }

        pxBufferLayout->
    }

    return 0;
#endif
}

PXResult PXAPI  PXMeshVertexLayoutPrint(PXMesh* const pxMesh)
{
    PXVertexBuffer* pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    char bufferLayout[64];

    for(size_t i = 0; i < pxMesh->VertexBufferListAmount; i++)
    {
        PXVertexBuffer* pxVertexBuffer = &pxVertexBufferList[i];

        PXBufferLayout* const pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            "Vertex",
            "Layout",
            "PXID:%i, GLID_VBO:%i, (%i/%i), Amount:%i",
            pxVertexBuffer->Info.ID,
            pxVertexBuffer->Info.Handle.OpenGLID,
            i + 1,
            pxMesh->VertexBufferListAmount,
            pxVertexBuffer->LayoutAmount
        );
#endif

        for(size_t w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);
            PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Vertex",
                "Layout",
                "- (%i/%i) Amount:%i, TypeSize:%i, Type:%i",
                w + 1,
                pxVertexBuffer->LayoutAmount,
                amountOfElement,
                sizeOfSingleElement,
                pxVertexBufferLayout->Type
            );
#endif
        }
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXMeshVertexLayout(PXMesh* const pxMesh, const PXSize index, PXBufferLayout* const pxVertexBufferLayoutList, const PXSize amount)
{
    PXVertexBuffer* pxVertexBufferList = PXNull;

    pxMesh->VertexBufferListAmount = amount;

    //-----------------------------------------------------
    // Calc total use
    PXSize totalVertexDataSize = 0;

    for(PXSize i = 0; i < amount; ++i)
    {
        PXBufferLayout* const pxVertexBufferLayout = &pxVertexBufferLayoutList[i];

        PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);
        PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

        totalVertexDataSize += sizeOfSingleElement * amountOfElement;
    }
    //-----------------------------------------------------

    //-----------------------------------------------------
    // Alloc, VBO
    pxVertexBufferList = PXMeshVertexBufferListSET(pxMesh, amount);

    //-----------------------------------------------------

    // Distructbute

    PXVertexBuffer* pxVertexBuffer = &pxVertexBufferList[index];

    pxVertexBuffer->LayoutAmount = amount;
    PXCopyList(PXBufferLayout, amount, pxVertexBufferLayoutList, pxVertexBuffer->LayoutPrime);

    PXBufferAllocate(&pxVertexBuffer->VertexData, totalVertexDataSize);

    // How many vertex arrays?
}

PXResult PXAPI  PXMeshIndexLayout(PXMesh* const pxMesh, const PXSize primitveAmount, const PXSize segmentAmount)
{
    PXIndexBufferPrepare(&pxMesh->IndexBuffer, primitveAmount, segmentAmount);

    pxMesh->IndexBuffer.DrawModeID = PXDrawModeIDTriangle;// | PXDrawModeIDPoint | PXDrawModeIDLineLoop;

    pxMesh->MaterialContaierListAmount = segmentAmount;
    pxMesh->MaterialContaierList = PXMemoryHeapCallocT(PXMaterialContainer, segmentAmount);

    return PXActionSuccessful;
}

PXBufferLayout* PXAPI PXMeshVertexBufferGET(PXMesh* const pxMesh, const PXI8U type)
{
    PXVertexBuffer* const pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    // Search every vertexBuffer
    for(PXSize i = 0; i < pxMesh->VertexBufferListAmount; ++i)
    {
        PXVertexBuffer* const pxVertexBuffer = &pxVertexBufferList[i];
        PXBufferLayout* const pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        // Search every layout
        for(PXSize w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            // if layout matches..
            if(pxVertexBufferLayout->Type == type)
            {
                return pxVertexBufferLayout;
            }
        }
    }

    return PXNull;
}

void* PXAPI PXMeshVertexInsert(PXMesh* const pxMesh, const PXI8U type)
{
    PXVertexBuffer* const pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    // Search every vertexBuffer
    for(PXSize i = 0; i < pxMesh->VertexBufferListAmount; ++i)
    {
        PXVertexBuffer* const pxVertexBuffer = &pxVertexBufferList[i];
        PXBufferLayout* const pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        PXSize offset = 0;

        // Search every layout
        for(PXSize w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);
            PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

            offset += sizeOfSingleElement * amountOfElement;

            // if layout matches..
            if(pxVertexBufferLayout->Type == type)
            {
                void* adress = (PXByte*)pxVertexBuffer->VertexData.Data + offset;

                PXAssert
                (
                    offset <=
                    pxVertexBuffer->VertexData.Size,
                    "Out of bounce"
                );

                return adress;
            }
        }
    }

    return PXNull;
}

void* PXAPI PXMeshIndexInsert(PXMesh* const pxMesh, const PXI8U type)
{
    PXByte* data = (PXByte*)pxMesh->IndexBuffer.Data.Data;
    const PXSize sizeOfElement = PXTypeSizeGet(pxMesh->IndexBuffer.DataType);

    PXBufferLayout* pxBufferLayoutList = PXIndexLayoutListGET(&pxMesh->IndexBuffer);

    for(PXSize i = 0; i < pxMesh->IndexBuffer.LayoutListAmount; ++i)
    {
        PXBufferLayout* pxBufferLayout = &pxBufferLayoutList[i];

        if(type == pxBufferLayout->Type)
        {
            return data + sizeOfElement * i;
        }
    }

    return PXNull;
}

PXSize PXAPI PXMeshVertexStrideGET(PXMesh* const pxMesh)
{
    return 0;
}

PXVertexBuffer* PXAPI PXMeshVertexBufferListGET(PXMesh* const pxMesh)
{
    if(pxMesh->VertexBufferListAmount <= 4)
    {
        return &pxMesh->VertexBufferPrime;
    }
    else
    {
        return pxMesh->VertexBufferList;
    }
}

PXVertexBuffer* PXAPI PXMeshVertexBufferListSET(PXMesh* const pxMesh, const PXSize amount)
{
    if(amount <= 4)
    {
        return &pxMesh->VertexBufferPrime;
    }
    else
    {
        pxMesh->VertexBufferList = PXMemoryHeapCallocT(PXVertexBuffer, pxMesh->VertexBufferListAmount);

        return pxMesh->VertexBufferList;
    }
}

PXSize PXAPI PXMeshIndexBufferLengthGET(PXMesh* const pxMesh)
{
    PXBufferLayout* const pxBufferLayoutList = PXIndexLayoutListGET(&pxMesh->IndexBuffer);

    PXI8U typeSize = 0;

    for(size_t i = 0; i < pxMesh->VertexBufferListAmount; i++)
    {
        PXBufferLayout* const pxBufferLayout = &pxBufferLayoutList[i];

        typeSize = PXTypeSizeGet(pxBufferLayout->Type);
    }

    if(typeSize == 0)
    {
        typeSize = 1;
    }

    PXSize res = (pxMesh->IndexBuffer.Data.Size / typeSize) / pxMesh->VertexBufferListAmount;

    return res;
}

void PXConvertAnyToFloat(void* a, PXType aType, float* b)
{
    const PXSize isFloat = (PXTypeF32 & aType) == PXTypeF32;

    if(aType == isFloat)
    {
        *b = *(float*)a;
        return;
    }

    switch(PXTypeSizeGet(aType))
    {
        case PXTypeInt08S:
        {
            *b = *(PXI8S*)a;
            return;
        }
        case PXTypeInt16S:
        {
            *b = *(PXI16S*)a;
            return;
        }
        case PXTypeInt32S:
        {
            *b = *(PXI32S*)a;
            return;
        }
        default:
        {
            DebugBreak();
            return;
        }
    }
}


PXResult PXAPI  PXMeshVertexLayoutTransmute(PXMesh* const pxMesh)
{
#if 0

    // If we have POS; NORM; TEX. Convert it into an interleaved array
    // As the GPU is to stupit to use seperate arrays.
    PXIndexBuffer* pxIndexBuffer = &pxMesh->IndexBuffer;

    PXVertexBuffer* const pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);
    PXBufferLayout* const pxIndexBufferLayoutList = PXIndexLayoutListGET(pxIndexBuffer);


    const PXSize layoutListAmount = pxIndexBuffer->LayoutListAmount;
    const PXSize vertexBufferAmount = pxMesh->VertexBufferListAmount;

    const PXBool hasMultiIndexArray = layoutListAmount > 1;
    const PXSize indexAmount = PXMeshIndexBufferLengthGET(pxIndexBuffer);


    // If the format is not already compatible, we need to translate it.
    // Allocate new vertex memory for this
    const PXSize stride = PXMeshVertexStrideGET(pxMesh);
    const PXSize fullData = sizeof(float) * stride * indexAmount;

    float* memBlock = PXMemoryVirtualAllocate(fullData, PXNull, PXAccessModeReadAndWrite);


    PXSize offset = 0;
    float* vertexDataNew[8];

    for(size_t i = 0; i < vertexBufferAmount; i++)
    {
        PXVertexBuffer* const pxVertexBuffer = &pxVertexBufferList[i];


        // Width of the whole buffer
        PXBufferLayout* const pxBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        PXSize width = 0;

        for(size_t i = 0; i < pxVertexBuffer->LayoutAmount; i++)
        {
            PXBufferLayout* const pxBufferLayout = &pxBufferLayoutList[i];

            width += PXTypeSizeGet(pxBufferLayout->Format);
        }

        vertexDataNew[i] = &((float*)memBlock)[offset];

        offset += indexAmount * width;
    }


    if(hasMultiIndexArray)
    {
        // Loop over all the arrays 
        for(PXSize x = 0; x < layoutListAmount; ++x)
        {
            PXBufferLayout* const pxBufferLayout = &pxIndexBufferLayoutList[x];

            // Lookup reference vertex array
            PXBufferLayout* vertexBufferLayout = PXMeshVertexBufferGET(pxMesh, pxBufferLayout->Type);

            void* source = PXMeshVertexInsert(pxMesh, pxBufferLayout->Type);
            float* target = vertexDataNew[x];


            const PXI8U vertexTypeSize = PXTypeSizeGet(vertexBufferLayout->Type);
            const PXI8U indexTypeSize = PXTypeSizeGet(pxBufferLayout->Type);


            void* indexData = pxIndexBuffer->Data.Data;



            for(size_t y = 0; y < indexAmount; ++y)
            {
                PXSize refIndex = x + y * layoutListAmount;
                PXI8U index = ((PXByte*)pxIndexBuffer->Data.Data)[refIndex];

                void* vertexDataANY = (PXByte*)source * vertexTypeSize * index;
                float* vertexDataF = &target[index];

                PXConvertAnyToFloat(vertexDataANY, pxBufferLayout->Type, vertexDataF);
            }

            switch(indexTypeSize)
            {
                case 1:
                {

                    break;
                }
                case 2:
                {
                    for(size_t y = 0; y < indexAmount; ++y)
                    {
                        PXSize refIndex = x + y * layoutListAmount;
                        PXI8U index = ((PXByte*)pxIndexBuffer->Data.Data)[refIndex];

                        void* vertexDataANY = index * (PXByte*)source * vertexTypeSize;
                        float* vertexDataF = &target[index];

                        PXConvertAnyToFloat(vertexDataANY, pxBufferLayout->Type, vertexDataF);
                    }

                    break;
                }
                case 4:
                {
                    for(size_t y = 0; y < indexAmount; ++y)
                    {
                        PXSize refIndex = x + y * layoutListAmount;
                        PXI8U index = ((PXByte*)pxIndexBuffer->Data.Data)[refIndex];

                        void* vertexDataANY = index * (PXByte*)source * vertexTypeSize;
                        float* vertexDataF = &target[index];

                        PXConvertAnyToFloat(vertexDataANY, pxBufferLayout->Type, vertexDataF);
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }
        }
    }
    else
    {
        // We prob. only need to promote vertex data
        // As the index data is only one type, there is no need to do anything

        for(PXSize i = 0; i < vertexBufferAmount; i++)
        {
            PXVertexBuffer* const pxVertexBuffer = &pxVertexBufferList[i];


        }



    }






#endif



#if 0


    PXSize vertexDataCacheOffset = 0;

    PXSize globalOffset = 0;
    PXSize indexOffset = 0;

    PXSize typeSize = PXTypeSizeGet(pxIndexBuffer->DataType);

    PXF32 cacheLine[3 + 3 + 2];


    float* inputPosition = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypePosition);
    float* inputNormal = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypeNormal);
    float* inputTexture = PXMeshVertexInsert(pxMesh, PXVertexBufferLayoutTypeTexturePos);

    void* inputIndexPosition = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypePosition);
    void* inputIndexNormal = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypeNormal);
    void* inputIndexTexture = PXMeshIndexInsert(pxMesh, PXVertexBufferLayoutTypeTexturePos);

    // Problem: Opengl does not work with multible index data 
    // things, so we need to change them
    const PXBool hasMultibleIndexData = pxMesh->IndexBuffer.LayoutListAmount > 1;




    const PXSize pxIndexSegmentListAmount = pxIndexBuffer->SegmentListAmount;
    PXIndexSegment* pxIndexSegmentList = PXIndexBufferSegmentListGET(pxIndexBuffer);

    for(PXSize segmentIndex = 0; segmentIndex < pxIndexSegmentListAmount; ++segmentIndex)
    {
        PXIndexSegment* const pxIndexSegment = &pxIndexSegmentList[segmentIndex];

        const PXSize range = pxIndexSegment->DataRange;

        for(PXSize i = 0; i < range; ++i)
        {
            PXI16U dataIndexVertex = *(PXI16U*)((PXByte*)inputIndexPosition + (typeSize * i));
            PXI16U dataIndexNormal = *(PXI16U*)((PXByte*)inputIndexNormal + (typeSize * i));
            PXI16U dataIndexTexture = *(PXI16U*)((PXByte*)inputIndexTexture + (typeSize * i));

            float* inVertex = &inputPosition[dataIndexVertex];
            float* inNormal = &inputNormal[dataIndexNormal];
            float* inTexture = &inputTexture[dataIndexTexture];

            float* outVertex = &cacheLine[0];
            float* outNormal = &cacheLine[3];
            float* outTexture = &cacheLine[3 + 3];

            vertexDataCacheOffset += PXMemoryCopy(inVertex, sizeof(float) * 3, outVertex, 100);
            vertexDataCacheOffset += PXMemoryCopy(inNormal, sizeof(float) * 3, outNormal, 100);
            vertexDataCacheOffset += PXMemoryCopy(inTexture, sizeof(float) * 2, outTexture, 100);

            pxOpenGL->Binding.BufferDataSub
            (
                GL_ARRAY_BUFFER,
                globalOffset,
                (GLsizeiptr)vertexDataCacheOffset,
                cacheLine
            );

            PXOpenGLErrorCurrent(pxOpenGL, 1);

            globalOffset += vertexDataCacheOffset;
            vertexDataCacheOffset = 0;
            ++indexOffset;

            // Mark as usable!                      
        }

#if PXLogEnable
        int progress = -1;// (globalOffset / (float)vertexDataInterleavedSize) * 100;

        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            PXOpenGLModelName,
            "Segment:<%2i/%-2i> ready! %7i/%7i (%3i)",
            segmentIndex + 1,
            pxIndexBuffer->SegmentListAmount,
            globalOffset,
            -1,
            progress
        );
#endif
    }
    //#endif


























    PXMeshVertexLayoutPrint(pxMesh);

    PXVertexBuffer* const pxVertexBufferList = PXMeshVertexBufferListGET(pxMesh);

    // Search every vertexBuffer
    for(PXSize i = 0; i < pxMesh->VertexBufferListAmount; ++i)
    {
        PXVertexBuffer* const pxVertexBuffer = &pxVertexBufferList[i];
        PXBufferLayout* const pxVertexBufferLayoutList = PXVertexBufferLayoutGET(pxVertexBuffer);

        PXSize offset = 0;

        // Search every layout
        for(PXSize w = 0; w < pxVertexBuffer->LayoutAmount; w++)
        {
            PXBufferLayout* pxVertexBufferLayout = &pxVertexBufferLayoutList[w];

            PXSize sizeOfSingleElement = PXTypeSizeGet(pxVertexBufferLayout->Format);

            if(sizeOfSingleElement == 1)
            {
                // We are using chars, we promite
            }

            PXSize amountOfElement = pxVertexBufferLayout->AmountOfElements;

            offset += sizeOfSingleElement * amountOfElement;


        }
    }



    //#if 0
        //-----------------------------------------------------
        // Vertex
        //-----------------------------------------------------

    PXMesh* pxMesh = &pxModel->Mesh;
    const PXVertexBufferFormat oldFormat = pxMesh->VertexBufferPrime.Format;


    if(1 == pxMesh->VertexBufferListAmount)
    {
        PXVertexBuffer* const pxVertexBuffer = &pxMesh->VertexBufferPrime;

        PXVertexBufferFormatInfo pxVertexBufferFormatInfoOLD;
        PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfoOLD, pxVertexBuffer->Format);

        PXVertexBufferFormat formatNEW = PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 2, 0, 0);
        PXVertexBufferFormatInfo pxVertexBufferFormatInfoNEW;
        PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfoNEW, formatNEW);


        const PXBool isP2I8 = pxVertexBufferFormatInfoOLD.Format == PXVertexBufferFormatI08 && pxVertexBufferFormatInfoOLD.Position == 2;
        const PXBool isP3I8 = pxVertexBufferFormatInfoOLD.Format == PXVertexBufferFormatI08 && pxVertexBufferFormatInfoOLD.Position == 3;
        const PXBool isT2P2I8 = pxVertexBufferFormatInfoOLD.Format == PXVertexBufferFormatI08 && pxVertexBufferFormatInfoOLD.Position == 2 && pxVertexBufferFormatInfoOLD.TexturePosition == 2;

        PXSize amountCurrent = pxVertexBufferFormatInfoOLD.Stride;
        PXSize amountFuture = pxVertexBufferFormatInfoNEW.Stride;

        if(isT2P2I8)
        {
            PXSize sizeBefore = pxVertexBuffer->VertexDataSize;
            PXSize sizeCurrent = (pxVertexBuffer->VertexDataSize / 4) * amountFuture;

            // Store old data
            PXI8S* dataOld = (PXI8S*)pxVertexBuffer->VertexData;

            pxVertexBuffer->Format = formatNEW;
            pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXF32, sizeCurrent);;
            pxVertexBuffer->VertexDataSize = sizeof(PXF32) * sizeCurrent;

            PXF32* dataNew = (PXF32*)pxVertexBuffer->VertexData;

            PXSize newOffset = 0;

            for(PXSize i = 0; i < sizeBefore; i += 4)
            {
                dataNew[newOffset++] = (PXF32)dataOld[i + 0];
                dataNew[newOffset++] = (PXF32)dataOld[i + 1];
                dataNew[newOffset++] = 0.0f;
                dataNew[newOffset++] = (PXF32)dataOld[i + 2];
                dataNew[newOffset++] = (PXF32)dataOld[i + 3];
            }

            //PXConsoleWriteTablePXF32(dataNew, sizeCurrent, 5);
        }
        else if(isP2I8)
        {
            PXSize sizeBefore = pxVertexBuffer->VertexDataSize;
            PXSize sizeCurrent = (pxVertexBuffer->VertexDataSize / 2) * amountFuture;

            // Store old data
            PXI8S* dataOld = (PXI8S*)pxVertexBuffer->VertexData;

            pxVertexBuffer->Format = formatNEW;
            pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXF32, sizeCurrent);;
            pxVertexBuffer->VertexDataSize = sizeof(PXF32) * sizeCurrent;

            PXF32* dataNew = (PXF32*)pxVertexBuffer->VertexData;

            PXSize newOffset = 0;

            const char texCoords[] =
            {
                0, 1,   // Top-left
                1, 1,   // Top-right
                1, 0,   // Bottom-right
                0, 0    // Bottom-left
            };

            for(PXSize i = 0; i < sizeBefore; i += 2)
            {
                dataNew[newOffset++] = (PXF32)dataOld[i + 0];
                dataNew[newOffset++] = (PXF32)dataOld[i + 1];
                dataNew[newOffset++] = 0.0f;

                dataNew[newOffset++] = (PXF32)texCoords[(i + 0) % 8];
                dataNew[newOffset++] = (PXF32)texCoords[(i + 1) % 8];

                //dataNew[newOffset++] = (dataOld[i + 0] + 1) / 2.0f;
                //dataNew[newOffset++] = -(dataOld[i + 1] + 1) / 2.0f;
            }

            //PXConsoleWriteTablePXF32(dataNew, sizeCurrent, 5);

            // Memory leak? dataOld needs to be deleted? But what if its read only?         
        }
        else if(isP3I8)
        {
            formatNEW = PXVertexBufferFormatMake(PXVertexBufferFormatF32, 3, 0, 0, 0);
            PXVertexBufferFormatInfoExtract(&pxVertexBufferFormatInfoNEW, formatNEW);
            amountFuture = pxVertexBufferFormatInfoNEW.Stride;


            PXSize sizeCurrent = pxVertexBuffer->VertexDataSize / 1;

            const PXSize newVertexArraySize = sizeof(PXF32) * sizeCurrent;
            PXF32* newVertexArray = PXMemoryHeapCallocT(PXF32, sizeCurrent);

            PXI8S* dataSource = (PXI8S*)pxVertexBuffer->VertexData;

            for(size_t i = 0; i < sizeCurrent; i++)
            {
                newVertexArray[i] = dataSource[i];
            }

            // Memory leak?

            pxVertexBuffer->Format = formatNEW;
            pxVertexBuffer->VertexData = newVertexArray;
            pxVertexBuffer->VertexDataSize = newVertexArraySize;
        }

        PXVertexBufferFormat newFormat = pxVertexBuffer->Format;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Model-Format",
            "Transmute <%s> to <%s>",
            pxVertexBufferFormatInfoOLD.AsText,
            pxVertexBufferFormatInfoNEW.AsText
        );
#endif

    }

#endif
}

PXResult PXAPI  PXMeshNormalDataGenerate(PXMesh* const pxMesh)
{
    // ...

    return PXActionSuccessful;
}

PXResult PXAPI  PXMeshVertexArrayAdd
(
    PXMesh* const pxMesh,
    void* data,
    const PXSize dataLength,
    PXBufferLayout* const pxVertexBufferLayoutList,
    const PXSize pxVertexBufferLayoutListAmount
)
{
    PXVertexBuffer* pxVertexBufferTarget = PXNull;

    // Do we need to allocate? Buffer
    const PXSize expectedIndex = pxMesh->VertexBufferListAmount;
    const PXBool createNew = PXEmbeddedArraySize < (expectedIndex + 1);

    if(createNew)
    {
        // Do then
        pxVertexBufferTarget = &pxMesh->VertexBufferList[expectedIndex];
    }
    else
    {
        pxVertexBufferTarget = &pxMesh->VertexBufferPrime[expectedIndex];
    }

    PXAssert(pxVertexBufferTarget, "Can't be NULL");

    pxVertexBufferTarget->LayoutAmount = pxVertexBufferLayoutListAmount;
    PXCopyList(PXBufferLayout, pxVertexBufferLayoutListAmount, pxVertexBufferLayoutList, pxVertexBufferTarget->LayoutPrime);

    // COPY??
    pxVertexBufferTarget->VertexData.Data = data;
    pxVertexBufferTarget->VertexData.Size = dataLength;



    //---------------------
   // Offset buffer
   //---------------------

   // GetGraphicsMode();
#if 0

   // Request API

    PXGraphic* const pxGraphic = PXGraphicInstantiateGET();
    PXOpenGL* const opengl = &pxGraphic->OpenGLInstance;

    PXVertexBuffer indexedVertexBuffer;

    PXSize attributeIndex = 0;

    // Create
    PXOpenGLBufferGenerate(opengl, 1, &indexedVertexBuffer.Info.Handle.OpenGLID);

    // Bind
    PXOpenGLBufferBind(opengl, PXOpenGLBufferArray, indexedVertexBuffer.Info.Handle.OpenGLID);

    // Upload
    PXOpenGLBufferData(opengl, PXOpenGLBufferArray, dataLength, data, PXOpenGLStoreStaticDraw);

    // Define
    PXOpenGLVertexArrayAttributeDefine(opengl, attributeIndex, 2, PXTypeF32, PXFalse, dataLength, (void*)0);
    PXOpenGLVertexArrayEnable(opengl, attributeIndex);

#endif



    // Attach to model...



    return PXActionSuccessful;
}

PXSize PXAPI PXMeshTriangleAmount(PXMesh* const pxMesh)
{
    /*
    const PXI8U stride = PXVertexBufferFormatStrideSize(pxMesh->VertexBuffer.Format);
    PXSize amount = (pxMesh->VertexBuffer.VertexDataSize / stride) / 3;

    return amount;
    */

    return 0;// (pxMesh->IndexBuffer.DataIndexSizeTotal[0];
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

PXFontPageCharacter* PXAPI PXFontPageCharacterFetch(PXFontPage* const pxFontPage, const PXI32U characterID)
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

void PXAPI PXWindowBrushColorSet(PXWindowBrush* const pxWindowBrush, const PXByte red, const PXByte green, const PXByte blue)
{
    pxWindowBrush->Info.Behaviour |= PXWindowBrushBehaviourColorEmbeded;
    pxWindowBrush->ColorDate.Red = red;
    pxWindowBrush->ColorDate.Green = green;
    pxWindowBrush->ColorDate.Blue = blue;
}

PXResult PXAPI  PXVersionFromString(PXVersion* const pxVersion, char* versioNString)
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
        PXAdvance(&pxTextVersion, offset + 1u); // dot
        offset = PXTextToInt(&pxTextVersion, &versionMinor);

        if(offset != -1)
        {
            PXAdvance(&pxTextVersion, offset + 1u); // dot
            PXTextToInt(&pxTextVersion, &versionBuild);
        }
    }

    pxVersion->Major = versionMajor;
    pxVersion->Minor = versionMinor;
    pxVersion->Build = versionBuild;
    pxVersion->Patch = versionPatch;

    return PXActionSuccessful;
}

PXResult PXAPI  PXVersionToString(PXVersion* const pxVersion, char* versioNString)
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

PXResult PXAPI  PXFileTypeInfoProbe(PXFileFormatInfo* const pxFileFormatInfo, const PXText* const pxText)
{
    // Probe for file extension
    if(pxText)
    {
        PXFileFormatInfoViaPath(pxFileFormatInfo, pxText);
    }

    switch(PXFileFormatMask & pxFileFormatInfo->Flags)
    {
        case PXFileFormatA3DS:
            pxFileFormatInfo->Flags = PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXAutodesk3DSLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXAutodesk3DSSafeFromFile;
            pxFileFormatInfo->ExtensionText = "3DS";
            break;

        case PXFileFormatAAC:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXAACLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXAACSaveToFile;
            pxFileFormatInfo->ExtensionText = "ACC";
            break;

        case PXFileFormatAVI:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXAVILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXAVISaveToFile;
            pxFileFormatInfo->ExtensionText = "AVI";
            break;

        case PXFileFormatBitMap:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourcePeek = PXBitmapPeekFromFile;
            pxFileFormatInfo->ResourceLoad = PXBitmapLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXBitmapSaveToFile;
            pxFileFormatInfo->ExtensionText = "BMP";
            break;

        case PXFileFormatCanonRaw3:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXCanonRaw3LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCanonRaw3SaveToFile;
            pxFileFormatInfo->ExtensionText = "CR3";
            break;

        case PXFileFormatC:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXCLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCSaveToFile;
            pxFileFormatInfo->ExtensionText = "C";
            break;

        case PXFileFormatCSharp:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXCSharpLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCSharpSaveToFile;
            pxFileFormatInfo->ExtensionText = "CS";
            break;

        case PXFileFormatCSS:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXCSSLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCSSSaveToFile;
            pxFileFormatInfo->ExtensionText = "CSS";
            break;

        case PXFileFormatCPP:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXCPPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXCPPSaveToFile;
            pxFileFormatInfo->ExtensionText = "CPP";
            break;

        case PXFileFormatBinaryWindows:
            pxFileFormatInfo->Flags = PXResourceTypeBinary;
            pxFileFormatInfo->ResourceLoad = PXBinaryWindowsLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXBinaryWindowsSaveToFile;
            pxFileFormatInfo->ExtensionText = "EXE";
            break;

        case PXFileFormatBinaryLinux:
            pxFileFormatInfo->Flags = PXResourceTypeBinary;
            pxFileFormatInfo->ResourceLoad = PXBinaryLinuxLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXBinaryLinuxSaveToFile;
            pxFileFormatInfo->ExtensionText = "ELF";
            break;

        case PXFileFormatDirectDrawSurfaceTexture:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXDDSLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXDDSSaveToFile;
            pxFileFormatInfo->ExtensionText = "DDS";
            break;

        case PXFileFormatEML:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            pxFileFormatInfo->ExtensionText = "EML";
            break;

        case PXFileFormatFastFile:
            pxFileFormatInfo->Flags |= PXFileFormatTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXFastFileLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXFastFileSaveToFile;
            pxFileFormatInfo->ExtensionText = "FF";
            break;

        case PXFileFormatFilmBox:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXFilmBoxLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXFilmBoxSaveToFile;
            pxFileFormatInfo->ExtensionText = "FBX";
            break;

        case PXFileFormatFLAC:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXFLACLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXFLACSaveToFile;
            pxFileFormatInfo->ExtensionText = "FLAC";
            break;

        case PXFileFormatSpriteFont:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXSpriteFontLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSpriteFontSaveToFile;
            pxFileFormatInfo->ExtensionText = "FNT";
            break;

        case PXFileFormatGIF:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXGIFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXGIFSaveToFile;
            pxFileFormatInfo->ExtensionText = "GIF";
            break;

        case PXFileFormatOpenGLShader:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXGLSLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXGLSLSaveToFile;
            pxFileFormatInfo->ExtensionText = "GLSL";
            break;

        case PXFileFormatDirectXShader:
            pxFileFormatInfo->Flags = PXResourceTypeShaderProgram;
            pxFileFormatInfo->ResourceLoad = PXHLSLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXHLSLSaveToFile;
            pxFileFormatInfo->ExtensionText = "HLSL";
            break;

        case PXFileFormatHighEfficiencyImageFile:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXHEIFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXHEIFSaveToFile;
            break;

        case PXFileFormatHTML:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXHTMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXHTMLSaveToFile;
            pxFileFormatInfo->ExtensionText = "HTML";
            break;

        case PXFileFormatINI:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXINILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXINISaveToFile;
            pxFileFormatInfo->ExtensionText = "INI";
            break;

        case PXFileFormatJava:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXJavaLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJavaSaveToFile;
            pxFileFormatInfo->ExtensionText = "JAR";
            break;

        case PXFileFormatJavaScript:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXJavaScriptLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJavaScriptSaveToFile;
            break;

        case PXFileFormatJPEG:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXJPEGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJPEGSaveToFile;
            pxFileFormatInfo->ExtensionText = "JPG";
            break;

        case PXFileFormatJSON:
            pxFileFormatInfo->Flags = PXResourceTypeStructuredText;
            pxFileFormatInfo->ResourceLoad = PXJSONLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXJSONSaveToFile;
            pxFileFormatInfo->ExtensionText = "JSON";
            break;

        case PXFileFormatM4A:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            pxFileFormatInfo->ExtensionText = "M4A";
            break;

        case PXFileFormatMIDI:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXMIDILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMIDISaveToFile;
            pxFileFormatInfo->ExtensionText = "MIDI";
            break;

        case PXFileFormatMP3:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXMP3LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMP3SaveToFile;
            pxFileFormatInfo->ExtensionText = "MP3";
            break;

        case PXFileFormatMP4:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXMP4LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMP4SaveToFile;
            pxFileFormatInfo->ExtensionText = "MP4";
            break;

        case PXFileFormatMSI:
            pxFileFormatInfo->Flags = PXResourceTypeInstaller;
            pxFileFormatInfo->ResourceLoad = PXMSILoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMSISaveToFile;
            pxFileFormatInfo->ExtensionText = "MSI";
            break;

        case PXFileFormatMTL:
            pxFileFormatInfo->Flags = PXResourceTypeMaterialList;
            pxFileFormatInfo->ResourceLoad = PXMTLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMTLSaveToFile;
            pxFileFormatInfo->ExtensionText = "MTL";
            break;

        case PXFileFormatN64:
            pxFileFormatInfo->Flags = PXResourceTypeBinary;
            pxFileFormatInfo->ResourceLoad = PXN64LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXN64SaveToFile;
            pxFileFormatInfo->ExtensionText = "N64";
            break;

        case PXFileFormatWavefront:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXWavefrontLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWavefrontSaveFromFile;
            pxFileFormatInfo->ExtensionText = "WAV";
            break;

        case PXFileFormatMatroska:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXMatroskaLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXMatroskaSaveToFile;
            pxFileFormatInfo->ExtensionText = "";
            break;

        case PXFileFormatOGG:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXOGGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXOGGSaveToFile;
            pxFileFormatInfo->ExtensionText = "OGG";
            break;

        case PXFileFormatEugeneRoshalArchive:
            pxFileFormatInfo->Flags = PXResourceTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXRARLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRARSaveToFile;
            pxFileFormatInfo->ExtensionText = "RAR";
            break;

        case PXFileFormatPDF:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXPDFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPDFSaveToFile;
            pxFileFormatInfo->ExtensionText = "PDF";
            break;

        case PXFileFormatPHP:
            pxFileFormatInfo->Flags = PXResourceTypeCodeDocument;
            pxFileFormatInfo->ResourceLoad = PXPHPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPHPSaveToFile;
            pxFileFormatInfo->ExtensionText = "PHP";
            break;

        case PXFileFormatPLY:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXPLYLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPLYSaveToFile;
            pxFileFormatInfo->ExtensionText = "PLY";
            break;

        case PXFileFormatPNG:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            //pxFileFormatInfo->FileSizePredict = PXPNGFilePredictSize;
            pxFileFormatInfo->ResourcePeek = PXPNGPeekFromFile;
            pxFileFormatInfo->ResourceLoad = PXPNGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXPNGSaveToFile;
            pxFileFormatInfo->ExtensionText = "PNG";
            break;

        case PXFileFormatQOI:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            pxFileFormatInfo->ExtensionText = "QOI";
            break;

        case PXFileFormatSTEP:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXSTEPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSTEPSaveToFile;
            pxFileFormatInfo->ExtensionText = "STEP";
            break;

        case PXFileFormatSTL:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXSTLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSTLSaveToFile;
            pxFileFormatInfo->ExtensionText = "STL";
            break;

        case PXFileFormatSVG:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXSVGLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXSVGSaveToFile;
            pxFileFormatInfo->ExtensionText = "SVG";
            break;

        case PXFileFormatTAR:
            pxFileFormatInfo->Flags = PXResourceTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXTARLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTARSaveToFile;
            pxFileFormatInfo->ExtensionText = "TAR";
            break;

        case PXFileFormatTGA:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXTGALoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTGASaveToFile;
            pxFileFormatInfo->ExtensionText = "TGA";
            break;

        case PXFileFormatTagImage:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXTIFFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTIFFSaveToFile;
            pxFileFormatInfo->ExtensionText = "TIFF";
            break;

        case PXFileFormatTrueTypeFont:
            pxFileFormatInfo->Flags = PXResourceTypeFont;
            pxFileFormatInfo->ResourceLoad = PXTTFLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXTTFSaveToFile;
            pxFileFormatInfo->ExtensionText = "TTF";
            break;

        case PXFileFormatUniversalSceneDescription:
            pxFileFormatInfo->Flags = PXResourceTypeArchiv;
            pxFileFormatInfo->ResourceLoad = PXUSDLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXUSDSaveToFile;
            pxFileFormatInfo->ExtensionText = "UCD";
            break;

        case PXFileFormatVideoObject:
            pxFileFormatInfo->Flags = PXResourceTypeVideo;
            pxFileFormatInfo->ResourceLoad = PXVOBLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXVOBLSaveToFile;
            pxFileFormatInfo->ExtensionText = "";
            break;

        case PXFileFormatVRML:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXVRMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXVRMLSaveToFile;
            pxFileFormatInfo->ExtensionText = "VRML";
            break;

        case PXFileFormatWAD:
            pxFileFormatInfo->Flags = PXResourceTypeModel;
            pxFileFormatInfo->ResourceLoad = PXWADLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWADSaveToFile;
            pxFileFormatInfo->ExtensionText = "WAD";
            break;

        case PXFileFormatWave:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXWaveLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWaveSaveToFile;
            pxFileFormatInfo->ExtensionText = "WAV";
            break;

        case PXFileFormatWEBM:
            pxFileFormatInfo->Flags = PXResourceTypeSound;
            pxFileFormatInfo->ResourceLoad = PXWEBMLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWEBMSaveToFile;
            pxFileFormatInfo->ExtensionText = "WEBM";
            break;

        case PXFileFormatWEBP:
            pxFileFormatInfo->Flags = PXResourceTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXWEBPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWEBPSaveToFile;
            pxFileFormatInfo->ExtensionText = "WEBP";
            break;

        case PXFileFormatWMA:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXWMALoadFromFile;
            pxFileFormatInfo->ResourceSave = PXWMASaveToFile;
            pxFileFormatInfo->ExtensionText = "WMA";
            break;

        case PXFileFormatXML:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXXMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXXMLSaveToFile;
            pxFileFormatInfo->ExtensionText = "XML";
            break;

        case PXFileFormatYAML:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXYAMLLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXYAMLSaveToFile;
            pxFileFormatInfo->ExtensionText = "YML";
            break;

        case PXFileFormatZIP:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXZIPLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXZIPSaveToFile;
            pxFileFormatInfo->ExtensionText = "ZIP";
            break;


            // NEW

        case PXFileFormatRedshiftMesh:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXR3D2LoadFromFile;
            pxFileFormatInfo->ResourceSave = PXR3D2SaveToFile;
            pxFileFormatInfo->ExtensionText = "R3D2";
            break;

        case PXFileFormatRedshiftSkeleton:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRedshiftSkeletonLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRedshiftSkeletonSaveToFile;
            pxFileFormatInfo->ExtensionText = "SKL";
            break;

        case PXFileFormatRedshiftAnimation:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRedshiftAnimationLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRedshiftAnimationSaveToFile;
            pxFileFormatInfo->ExtensionText = "ANM";
            break;

        case PXFileFormatRGeometryMap:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRGeometricMapLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRGeometricMapSaveToFile;
            pxFileFormatInfo->ExtensionText = "MAPGEO";
            break;

        case PXFileFormatRGeometryWorld:
            pxFileFormatInfo->Flags |= PXFileFormatTypeModel;
            pxFileFormatInfo->ResourceLoad = PXRGeometricWorldLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRGeometricWorldSaveToFile;
            pxFileFormatInfo->ExtensionText = "WGEO";
            break;

        case PXFileFormatRSkinSimple:
            pxFileFormatInfo->Flags |= PXFileFormatTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXRSkinLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRSkinSaveToFile;
            pxFileFormatInfo->ExtensionText = "SKN";
            break;

        case PXFileFormatRTexture:
            pxFileFormatInfo->Flags |= PXFileFormatTypeTexture;
            pxFileFormatInfo->ResourceLoad = PXRTextureLoadFromFile;
            pxFileFormatInfo->ResourceSave = PXRTextureSaveToFile;
            pxFileFormatInfo->ExtensionText = "TEX";
            break;

        default:
            pxFileFormatInfo->Flags |= PXFileFormatTypeInvalid;
            pxFileFormatInfo->ResourceLoad = PXNull;
            pxFileFormatInfo->ResourceSave = PXNull;
            break;
    }

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceManagerReferenceValidate(PXResourceReference* const pxResourceReference)
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

PXResult PXAPI  PXResourceLoad(PXResourceTransphereInfo* const pxResourceLoadInfo, const PXText* const filePath)
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
        PXBool textValid = filePath->A && filePath->SizeUsed;

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
        filePath->A
    );
#endif

    // try to detect format over file extension
    PXFileTypeInfoProbe(&pxResourceLoadInfo->FormatInfo, filePath);


    // if we dont even have a clue how to load it, let the OS try to load it.
    {
        if(pxResourceLoadInfo->FormatInfo.Flags == PXFileFormatUnkown)
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

        if(!pxResourceLoadInfo->FormatInfo.ResourceLoad)
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
        pxFileOpenInfo.FilePathAdress = filePath->A;
        pxFileOpenInfo.FilePathSize = filePath->SizeUsed;
        pxFileOpenInfo.AccessMode = PXAccessModeReadOnly;
        pxFileOpenInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenInfo.FlagList = PXFileIOInfoAllowMapping | PXFileIOInfoFilePhysical;

        const PXActionResult fileLoadingResult = PXFileOpen(&pxFile, &pxFileOpenInfo);

        PXActionReturnOnError(fileLoadingResult);
    }

    PXPerformanceInfo pxPerformanceInfo;
    pxPerformanceInfo.UpdateCounter = 0;

    // If a peek method exists, execute it, if not, strait to loading
    if(pxResourceLoadInfo->FormatInfo.ResourcePeek)
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

        PXPerformanceInfoGet(&pxPerformanceInfo);
        const PXActionResult pxPeekResult = pxResourceLoadInfo->FormatInfo.ResourcePeek(pxResourceLoadInfo);
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

        PXPerformanceInfoGet(&pxPerformanceInfo);
        pxResourceLoadInfo->TimePeek = pxPerformanceInfo.TimeDelta;

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

        PXPerformanceInfoGet(&pxPerformanceInfo);

        pxResourceLoadInfo->OnDeviceDataRegister(pxResourceLoadInfo);

        PXPerformanceInfoGet(&pxPerformanceInfo);

        pxResourceLoadInfo->TimeDeviceDataRegister = pxPerformanceInfo.TimeDelta;

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
    if(pxResourceLoadInfo->FormatInfo.ResourceLoad)
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

        PXPerformanceInfoGet(&pxPerformanceInfo);

        const PXActionResult fileParsingResult = pxResourceLoadInfo->FormatInfo.ResourceLoad(pxResourceLoadInfo);

        PXPerformanceInfoGet(&pxPerformanceInfo);

        pxResourceLoadInfo->TimeTransphere = pxPerformanceInfo.TimeDelta;



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
                filePath->A
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
            "OK! Took:%6.3f, ROPs:%-7i PageFaults:%-7i <%s>",
            pxResourceLoadInfo->TimeTransphere,
            pxFile.CounterOperationsRead,
            pxPerformanceInfo.PageFaultCount,
            filePath->A
        );
#endif

        return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }
}

PXResult PXAPI  PXResourceLoadA(PXResourceTransphereInfo* const pxResourceLoadInfo, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceLoad(pxResourceLoadInfo, &pxText);

    return loadResult;
}

PXResult PXAPI  PXResourceSave(PXResourceTransphereInfo* const pxResourceSaveInfo, const PXText* const filePath)
{
    PXFile pxFile;

    // Loading file
    {
        PXFileOpenInfo pxFileIOInfo;
        PXClear(PXFileOpenInfo, &pxFileIOInfo);
        pxFileIOInfo.FilePathAdress = filePath->A;
        pxFileIOInfo.FilePathSize = filePath->SizeUsed;
        pxFileIOInfo.AccessMode = PXAccessModeWriteOnly;
        pxFileIOInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileIOInfo.FlagList = PXFileIOInfoFilePhysical | PXFileIOInfoAllowMapping | PXFileIOInfoCreateIfNotExist;

        const PXActionResult fileLoadingResult = PXFileOpen(&pxFile, &pxFileIOInfo);

        PXActionReturnOnError(fileLoadingResult);
    }


    // try to detect format over file extension
    PXFileTypeInfoProbe(&pxResourceSaveInfo->FormatInfo, filePath);


    // Try to load assumed format
    {
        if(PXFileFormatUnkown == pxResourceSaveInfo->FormatInfo.Flags)
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

        if(PXNull == pxResourceSaveInfo->FormatInfo.ResourceSave)
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
        const PXI64U timeStampA = PXTimeCounterStampGet();
#endif

        pxResourceSaveInfo->FileReference = &pxFile;

        const PXActionResult fileParsingResult = pxResourceSaveInfo->FormatInfo.ResourceSave(pxResourceSaveInfo);

#if PXLogEnable
        const PXI64U timeStampB = PXTimeCounterStampGet() - timeStampA;
        const PXF32 timeDelta = PXTimeCounterStampToSecoundsF(timeStampB);

        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceManagerText,
            "Save",
            "%6.3fms SOPs:%-7i <%s>",
            timeDelta,
            pxFile.CounterOperationsWrite,
            filePath->A
        );
#endif

        // PXActionReturnOnSuccess(fileParsingResult); // Exit if this has worked first-try

        // return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }

    PXFileClose(&pxFile);

    pxResourceSaveInfo->FileReference = PXNull;

    return PXActionSuccessful;
}

PXResult PXAPI  PXResourceSaveA(PXResourceTransphereInfo* const pxResourceSaveInfo, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceSave(pxResourceSaveInfo, &pxText);

    return loadResult;
}
























PXResourceManager* PXAPI PXResourceManagerGet(void)
{
    if(PXResourceManagerFlagIsCreated & _GLOBALResourceManager.Flags)
    {
        return &_GLOBALResourceManager;
    }

    // Not needed? global mem is alway zero?
    // We do it anyway.. just in case
    PXClear(PXResourceManager, &_GLOBALResourceManager);

    PXLockCreate(&_GLOBALResourceManager.AsyncLock, PXLockTypeProcessOnly);

    PXListDynamicInit(&_GLOBALResourceManager.NameCache, sizeof(PXI32U), PXListDynamicSizeObject2Byte);
    PXListDynamicInit(&_GLOBALResourceManager.SourcePathCache, sizeof(PXI32U), PXListDynamicSizeObject2Byte);

   
    PXDictionaryConstruct(&_GLOBALResourceManager.SpritelLookUp, sizeof(PXI32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.FontLookUp, sizeof(PXI32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.TextLookUp, sizeof(PXI32U), sizeof(PXDialogText), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.TimerLookUp, sizeof(PXI32U), sizeof(PXTimer), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.SoundLookUp, sizeof(PXI32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.HitBoxLookUp, sizeof(PXI32U), sizeof(PXHitBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.TextureLookUp, sizeof(PXI32U), sizeof(PXTexture), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.ModelLookUp, sizeof(PXI32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.ShaderProgramLookup, sizeof(PXI32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.SkyBoxLookUp, sizeof(PXI32U), sizeof(PXSkyBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.BrushLookUp, sizeof(PXI32U), sizeof(PXWindowBrush), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.GUIElementLookup, sizeof(PXI32U), sizeof(PXWindow), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.SpriteAnimator, sizeof(PXI32U), sizeof(PXSpriteAnimator), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.IconLookUp, sizeof(PXI32U), sizeof(PXIcon), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&_GLOBALResourceManager.IconAtlasLookUp, sizeof(PXI32U), sizeof(PXIconAtlas), PXDictionaryValueLocalityExternalReference);


   // PXResourceComponentCreate();

    // Register these with additional lookups

#if 0
    // Resource lookup for directCall
    const PXResourceEntry _GlobalResourceEntryList[] =
    {
        {&_GLOBALResourceManager.SoundLookUp,   PXResourceCreateSound,                  "Sound",PXResourceTypeSound, sizeof(PXSound) },
        {&_GLOBALResourceManager.ModelLookUp,   PXResourceCreateModel,                  "Model",PXResourceTypeModel, sizeof(PXModel) },
        {&_GLOBALResourceManager.FontLookUp,    PXResourceCreateFont,                   "Font",PXResourceTypeFont, sizeof(PXFont) },
        {&_GLOBALResourceManager.MaterialLookUp, PXResourceCreateMaterial,              "Material", PXResourceTypeMaterial, sizeof(PXMaterial) },
        {&_GLOBALResourceManager.BrushLookUp,   PXResourceCreateBrush,                  "Brush",PXResourceTypeBrush, sizeof(PXWindowBrush) },
        {&_GLOBALResourceManager.TextureLookUp, PXResourceCreateTexture,              "Texture",PXResourceTypeTexture2D, sizeof(PXTexture) },
        {&_GLOBALResourceManager.ShaderProgramLookup, PXResourceCreateShaderProgram,    "ShaderProgram",PXResourceTypeShaderProgram, sizeof(PXShaderProgram) },
        {&_GLOBALResourceManager.SkyBoxLookUp,  PXResourceCreateSkybox,                 "SkyBox",PXResourceTypeSkybox, sizeof(PXSkyBox) },
        {&_GLOBALResourceManager.SpritelLookUp, PXResourceCreateSprite,                 "Sprite",PXResourceTypeSprite, sizeof(PXSprite) },
        {&_GLOBALResourceManager.IconLookUp,    PXResourceCreateIcon,                   "Icon",PXResourceTypeIcon, sizeof(PXIcon) },
        {&_GLOBALResourceManager.IconAtlasLookUp, PXResourceCreateIconAtlas,              "IconAtlas",PXResourceTypeIconAtlas, sizeof(PXIconAtlas) },
        {&_GLOBALResourceManager.SpriteAnimator, PXResourceCreateSpriteAnimator,        "SpriteAnimator",PXResourceTypeSpriteAnimator, sizeof(PXSpriteAnimator) },
        {&_GLOBALResourceManager.TimerLookUp,   PXResourceCreateTimer,                  "Timer",PXResourceTypeTimer, sizeof(PXTimer) },
        {&_GLOBALResourceManager.GUIElementLookup, PXResourceCreateWindow,              "Window", PXResourceTypeGUIElement, sizeof(PXWindow) },
        {&_GLOBALResourceManager.HitBoxLookUp,  PXResourceCreateHitBox,                 "HitBox",PXResourceTypeHitBox, sizeof(PXHitBox) },
        {&_GLOBALResourceManager.MaterialLookUp, PXNull,                                "Material", PXResourceTypeMaterialList, PXNull},
        {&_GLOBALResourceManager.SpriteMapAtlasLookUp, PXResourceCreateSpriteMap, "SpriteMap", PXResourceTypeSpriteMap, sizeof(PXSpriteMap) },
    };
    const PXSize amount = sizeof(_GlobalResourceEntryList) / sizeof(_GlobalResourceEntryList[0]);

    for(size_t i = 0; i < amount; i++)
    {

    }
#endif





    _GLOBALResourceManager.Flags |= PXResourceManagerFlagIsCreated;

    PXThreadPoolCreate(PXNull);

    return &_GLOBALResourceManager;
}

PXResult PXAPI PXResourceComponentCreate(PXComponentInfo* const pxComponentInfo)
{
    //PXDictionaryEntryCreate(&_GLOBALResourceManager.ComponentLookup.ComponentLookup);

   

    //PXDictionaryConstruct(&_GLOBALResourceManager.MaterialLookUp, sizeof(PXI32U), sizeof(PXMaterial), PXDictionaryValueLocalityExternalReference);

    return PXActionSuccessful;
}

PXI32U PXAPI PXResourceManagerGenerateUniqeID()
{
    // assert(pxResourceManager);

    return 1000 + ++_pxResourceManager.UniqeIDCounter;
}

PXResult PXAPI PXResourceAdd(PXResourceInfo* const pxResourceInfo, void* payload)
{
    return PXActionSuccessful;
}

PXResult PXAPI PXResourceRemove(PXResourceInfo* const pxResourceInfo)
{
    return PXActionSuccessful;
}

PXResult PXAPI  PXResourcePropertyE(PXResourceProperty* const pxResourceProperty, const PXBool doWrite)
{
    // NULL Check
    if(!pxResourceProperty)
    {
        return PXActionRefusedArgumentNull;
    }

    if(doWrite)
    {
        const PXBool callValid = 0;// name && !(0 == nameSize || (PXSize)-1 == nameSize);

        if(!callValid)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXResourceText,
                "Store-Name",
                "PXID:%i, Name can't be stored!",
                0//pxResourceInfo->ID
            );
#endif

            return PXActionRefusedArgumentInvalid;
        }


        PXSize length = 0;// nameSize;

        //if(-1 == nameSize)
        {
            length = 0;// PXTextLengthA(name, nameSize);
        }

#if PXLogEnable && 0
        char buffer[256];

      //  PXTextCopyA(name, nameSize, buffer, length);

        PXLogPrint
        (
            PXLoggingInfo,
            PXResourceText,
            "Store-Name",
            "PXID:%i, %s (%i)",
            0//pxResourceInfo->ID,
            buffer,
            length
        );
#endif

        //PXListDynamicAdd(&_pxResourceManager.NameCache, &pxResourceInfo->ID, name, length);

       // pxResourceInfo->Behaviour |= PXResourceInfoHasName;

        return PXActionSuccessful;
    }

    return PXActionInvalid;
}







PXResult PXAPI PXGUIIconLoad(PXIcon* const pxIcon)
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

PXResult PXAPI PXTextureRelease(PXTexture* const pxTexture)
{
    PXMemoryHeapFree(PXNull, pxTexture->PixelData);

    return PXActionInvalid;
}

PXResult PXAPI PXTextureCopyAsIs(PXTexture* const pxTexture, const PXTexture* const pxTextureSource)
{
  //  PXCopy(PXTexture, pxTexture, pxTextureSource);

    return PXActionInvalid;
}

PXResult PXAPI PXTextureCopyAsNew(PXTexture* const pxTexture, const PXTexture* const pxTextureSource)
{
    pxTexture->PixelData = PXMemoryHeapCallocT(PXByte, pxTextureSource->PixelDataSize);
    PXCopyList(PXByte, pxTexture->PixelDataSize, pxTexture->PixelData, pxTextureSource->PixelData);

    pxTexture->Width = pxTextureSource->Width;
    pxTexture->Height = pxTextureSource->Height;
    pxTexture->Depth = pxTextureSource->Depth;
    pxTexture->Format = pxTextureSource->Format;

    return PXActionInvalid;
}

PXResult PXAPI PXTextureResize(PXTexture* const pxTexture, const PXColorFormat format, const PXSize width, const PXSize height)
{
    const PXSize bbp = PXColorFormatBytePerPixel(format);
    const PXSize newSize = width * height * bbp;
    const PXSize oldSize = pxTexture->PixelDataSize;

    // Do we need to realloc?
    {
        const PXBool isSizeAlreadyOK = newSize == oldSize;

        if(isSizeAlreadyOK)
        {
            return PXYes;
        }
    }

    // reallocate
    {
        pxTexture->PixelDataSize = newSize;
        pxTexture->PixelData = PXMemoryHeapCallocT(PXByte, newSize);
        pxTexture->Format = format;
        pxTexture->Width = width;
        pxTexture->Height = height;

        /*

        // Set image data, so we dont have a random fully empty image with no alpha
        // Causes wierd problems if you dont expect that.
        if(pxMemoryHeapReallocateEventData.WasSizeIncreased)
        {
            switch (bbp)
            {
                case 3:
                {
                    for (PXSize i = 0; i < pxMemoryHeapReallocateEventData.PointOfNewDataSize; i += 3)
                    {
                        char* data = &((char*)pxMemoryHeapReallocateEventData.PointOfNewData)[i];

                        data[0] = 0xFF;
                        data[1] = 0xFF;
                        data[2] = 0xFF;
                    }
                    break;
                }

                case 4:
                {
                    for (PXSize i = 0; i < pxMemoryHeapReallocateEventData.PointOfNewDataSize; i += 4)
                    {
                        char* data = &((char*)pxMemoryHeapReallocateEventData.PointOfNewData)[i];

                        data[0] = 0xFF;
                        data[1] = 0xFF;
                        data[2] = 0xFF;
                        data[3] = 0xFF;
                    }
                    break;
                }
            }
        }
        */

    }

    return PXActionSuccessful;
}

PXResult PXAPI PXTextureFlipHorizontal(PXTexture* const pxTexture)
{
    const PXSize bbp = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize rowSize = (pxTexture->Width * bbp);
    const PXSize length = (pxTexture->Width * bbp) / 2;

    for(PXSize x = 0; x < length; x += bbp) //
    {
        const PXSize xB = rowSize - x - bbp;

        for(PXSize y = 0; y < pxTexture->Height; y++)
        {
            const PXSize indexA = x + (y * rowSize);
            const PXSize indexB = xB + (y * rowSize);
            PXByte tempByte[4] = { 0,0,0,0 };
            PXByte* pixelA = (PXByte*)pxTexture->PixelData + indexA;
            PXByte* pixelB = (PXByte*)pxTexture->PixelData + indexB;

            PXMemoryCopyX(pixelA, 4, tempByte, bbp);
            PXMemoryCopyX(pixelB, 4, pixelA, bbp);
            PXMemoryCopyX(tempByte, 4, pixelB, bbp);
        }
    }

    return PXActionInvalid;
}

PXResult PXAPI PXTextureFlipVertical(PXTexture* const pxTexture)
{
    const PXSize bbp = PXColorFormatBytePerPixel(pxTexture->Format);;
    const PXSize scanLineWidthSize = pxTexture->Width * bbp;
    const PXSize scanLinesToSwap = pxTexture->Height / 2u;
    PXByte* copyBufferRow = PXMemoryHeapCallocT(PXByte, scanLineWidthSize);

    if(!copyBufferRow)
    {
        return;
    }

    for(PXSize scanlineIndex = 0; scanlineIndex < scanLinesToSwap; scanlineIndex++)
    {
        PXByte* bufferA = (PXByte*)pxTexture->PixelData + (scanlineIndex * scanLineWidthSize);
        PXByte* bufferB = (PXByte*)pxTexture->PixelData + ((pxTexture->Height - scanlineIndex) * scanLineWidthSize) - scanLineWidthSize;

        PXMemoryCopy(bufferB,  copyBufferRow, scanLineWidthSize); // A -> Buffer 'Save A'
        PXMemoryCopy(bufferA,  bufferB, scanLineWidthSize); // B -> A 'Move B to A(override)'
        PXMemoryCopy(copyBufferRow,  bufferA, scanLineWidthSize); // Buffer -> B 'Move SaveCopy (A) to B'
    }

    PXMemoryHeapFree(PXNull, copyBufferRow);

    return PXActionInvalid;
}

PXResult PXAPI PXTextureRemoveColor(PXTexture* const pxTexture, const PXByte red, const PXByte green, const PXByte blue)
{
    // DO we have alpha? If not, make one

    const PXSize currentOffset = pxTexture->PixelDataSize;
    PXSize dataOffset = 0;

    PXTextureResize(pxTexture, PXColorFormatRGBAI8, pxTexture->Width, pxTexture->Height);

    dataOffset = pxTexture->PixelDataSize;

    const PXSize length = pxTexture->Width * pxTexture->Height;

    for(PXSize pixelIndex = 0; pixelIndex < length; ++pixelIndex)
    {
        PXByte* const oldData = (PXByte*)pxTexture->PixelData + currentOffset - ((pixelIndex + 1) * 3u);
        PXByte* const newData = (PXByte*)pxTexture->PixelData + dataOffset - ((pixelIndex + 1) * 4u);

        newData[0] = oldData[0];
        newData[1] = oldData[1];
        newData[2] = oldData[2];
        newData[3] = !(oldData[0] == red) && (oldData[1] == green) && (oldData[2] == blue);
    }

    return PXActionInvalid;
}

PXResult PXAPI PXTextureFillColorRGBA8(PXTexture* const pxTexture, const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha)
{
    for(PXSize i = 0; i < pxTexture->PixelDataSize; i += 4u)
    {
        PXByte* const data = (PXByte*)pxTexture->PixelData + i;

        data[0] = red;
        data[1] = green;
        data[2] = blue;
        data[3] = alpha;
    }

    return PXActionInvalid;
}

void* PXAPI PXTextureDataPoint(const PXTexture* const pxTexture, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize index = x * bytesPerPixel + y * pxTexture->Width;

    return (PXByte*)pxTexture->PixelData + index;
}

PXSize PXAPI PXTexturePixelPosition(const PXTexture* const pxTexture, const PXSize x, const PXSize y)
{
    const PXSize bytesPerPixel = PXColorFormatBytePerPixel(pxTexture->Format);
    const PXSize index = x * bytesPerPixel + y * bytesPerPixel * pxTexture->Width;

    return index;
}

void PXAPI PXTexturePixelSetRGB8(PXTexture* const pxTexture, const PXSize x, const PXSize y, const PXByte red, const PXByte green, const PXByte blue)
{
    const PXSize index = PXTexturePixelPosition(pxTexture, x, y);
    PXByte* pixelData = (PXByte*)pxTexture->PixelData + index;

    pixelData[0] = red;
    pixelData[1] = green;
    pixelData[2] = blue;
}
