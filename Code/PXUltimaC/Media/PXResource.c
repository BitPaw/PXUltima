#include "PXResource.h"

#include <Math/PXMath.h>
#include <Media/PXText.h>
#include <OS/Console/PXConsole.h>
#include <OS/Time/PXStopWatch.h>
#include <OS/Hardware/PXProcessor.h>
#include <OS/File/PXFile.h>

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

#include <assert.h>

extern void _chkstk(size_t s) {};
extern int _fltused = 0;



const float PXVertexDataTriangle[] =
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
    case PXVertexBufferFormatXYI8:
        return "P2 8-Bit";
    case PXVertexBufferFormatXYZI8:
        return "P3 8-Bit";
    case PXVertexBufferFormatXYFloat:
        return "P2 float";
    case PXVertexBufferFormatXYZFloat:
        return "P3 float";
    case PXVertexBufferFormatXYZW:
        return "P4 float";
    case PXVertexBufferFormatC4UB_XY:
        return "C4UB_XY";
    case PXVertexBufferFormatC4UB_XYZ:
        return "C4UB_XYZ";
    case PXVertexBufferFormatRGBXYZ:
        return "T2 P3 float";
    case PXVertexBufferFormatN3F_XYZ:
        return "N3 P3 float";
    case PXVertexBufferFormatC4F_N3F_XYZ:
        return "C4 N3 P3 float";
    case PXVertexBufferFormatT2F_XYZ:
        return "T2 XYZ float";
    case PXVertexBufferFormatT4F_XYZW:
        return "T4 P3 - float";
    case PXVertexBufferFormatT2F_C4UB_XYZ:
        return "T4 C4 P3 - float";
    case PXVertexBufferFormatT2F_C3F_XYZ:
        return "T2 C4 P3 - float";
    case PXVertexBufferFormatT2F_N3F_XYZ:
        return "T2 N3 P3 - float";
    case PXVertexBufferFormatT2F_C4F_N3F_XYZ:
        return "T2 C4 N3 P3 - float";
    case PXVertexBufferFormatT4F_C4F_N3F_XYZW:
        return "T4 C4 N3 P4 - float";
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
    switch (pxVertexBufferFormat)
    {
    case PXVertexBufferFormatXYI8:
        return 2;
    case PXVertexBufferFormatXYZI8:
        return 3;
    case PXVertexBufferFormatT2F_XYZ:
        return 2u + 3u;
    case PXVertexBufferFormatT2F_N3F_XYZ:
        return 2u + 3u + 3u;
    case PXVertexBufferFormatXYZFloat:
        return 3u;
    case PXVertexBufferFormatN3F_XYZ:
        return 3u + 3u;

    default:
        return 0u;
    }
}

const char* PXAPI PXResourceTypeToString(const PXResourceType pxResourceType)
{
    switch(pxResourceType)
    {
    case PXResourceTypeCustom:
        return "Custom";
    case PXResourceTypeModel:
        return "Model";
    case PXResourceTypeFont:
        return "Font";
    case PXResourceTypeTexture2D:
        return "Texture2D";
    case PXResourceTypeImage:
        return "Image";
    case PXResourceTypeTextureCube:
        return "TetxureCube";
    case PXResourceTypeShaderProgram:
        return "ShaderProgram";
    case PXResourceTypeSkybox:
        return "SkyBox";
    case PXResourceTypeSprite:
        return "Sprite";
    case PXResourceTypeText:
        return "Text";
    case PXResourceTypeTimer:
        return "Timer";
    case PXResourceTypeSound:
        return "Sound";
    case PXResourceTypeEngineSound:
        return "PXSound";
    case PXResourceTypeGUIElement:
        return "UI";
    case PXResourceTypeHitBox:
        return "HitBox";
    case PXResourceTypeDialogBox:
        return "DialogBox";

    default:
        return PXNull;
    }
}

void PXAPI PXResourceManagerInit(PXResourceManager* const pxResourceManager)
{
    assert(pxResourceManager);

    PXClear(PXResourceManager, pxResourceManager);

    PXListDynamicInit(&pxResourceManager->NameCache, sizeof(PXInt32U), PXListDynamicSizeObject2Byte);
    PXListDynamicInit(&pxResourceManager->SourcePathCache, sizeof(PXInt32U), PXListDynamicSizeObject2Byte);

    PXDictionaryConstruct(&pxResourceManager->MaterialLookUp, sizeof(PXInt32U), sizeof(PXMaterial), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->SpritelLookUp, sizeof(PXInt32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->TextLookUp, sizeof(PXInt32U), sizeof(PXEngineText), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->TimerLookUp, sizeof(PXInt32U), sizeof(PXEngineTimer), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->SoundLookUp, sizeof(PXInt32U), sizeof(PXEngineSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->HitBoxLookUp, sizeof(PXInt32U), sizeof(PXHitBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture2D), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->ModelLookUp, sizeof(PXInt32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->ShaderProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->SkyBoxLookUp, sizeof(PXInt32U), sizeof(PXSkyBox), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->ImageLookUp, sizeof(PXInt32U), sizeof(PXImage), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->BrushLookUp, sizeof(PXInt32U), sizeof(PXGUIElementBrush), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->GUIElementLookup, sizeof(PXInt32U), sizeof(PXGUIElement), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxResourceManager->SpriteAnimator, sizeof(PXInt32U), sizeof(PXSpriteAnimator), PXDictionaryValueLocalityExternalReference);
}

void PXAPI PXResourceManagerRelease(PXResourceManager* const pxResourceManager)
{
    assert(pxResourceManager);
}

PXInt32U PXAPI PXResourceManagerGenerateUniqeID(PXResourceManager* const pxResourceManager)
{
    assert(pxResourceManager);

    return 1000 + ++pxResourceManager->UniqeIDGeneratorCounter;
}

PXActionResult PXAPI PXResourceManagerAdd(PXResourceManager* const pxResourceManager, PXResourceCreateInfo* const pxResourceCreateInfoList, const PXSize amount)
{
    assert(pxResourceManager);
    assert(pxResourceCreateInfoList);
    assert(0 <= amount);

    for(PXSize i = 0; i < amount; ++i)
    {
        PXResourceCreateInfo* const pxResourceCreateInfo = &pxResourceCreateInfoList[i];

        switch(pxResourceCreateInfo->Type)
        {
        case PXResourceTypeBrush:
        {
            PXBrushCreateInfo* const pxBrushCreateInfo = &pxResourceCreateInfo->Brush;
            PXGUIElementBrush* pxGUIElementBrush = *(PXGUIElementBrush**)pxResourceCreateInfo->ObjectReference;

            if(!pxGUIElementBrush)
            {
                PXNewZerod(PXGUIElementBrush, &pxGUIElementBrush);
                *pxResourceCreateInfo->ObjectReference = pxGUIElementBrush;
            }

            pxGUIElementBrush->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);


#if OSUnix
#elif OSWindows



            COLORREF brushColor = RGB(pxBrushCreateInfo->Color.Red, pxBrushCreateInfo->Color.Green, pxBrushCreateInfo->Color.Blue);
            HBRUSH brushHandle = CreateSolidBrush(brushColor);


            pxGUIElementBrush->Info.Handle.BrushHandle = brushHandle;
            PXGUIElementBrushColorSet(pxGUIElementBrush, pxBrushCreateInfo->Color.Red, pxBrushCreateInfo->Color.Green, pxBrushCreateInfo->Color.Blue);

            // Color xx = Color(255, 0, 0, 255);
            // SolidBrush ww = opaqueBrush();


#endif

            PXDictionaryAdd(&pxResourceManager->BrushLookUp, &pxGUIElementBrush->Info.ID, pxGUIElementBrush);

            break;
        }
        case PXResourceTypeImage:
        {
            PXImageCreateInfo* const pxImageCreateInfo = &pxResourceCreateInfo->Image;
            PXImage* pxImage = *(PXImage**)pxResourceCreateInfo->ObjectReference;

            PXText pxText;
            PXTextConstructFromAdressA(&pxText, pxResourceCreateInfo->FilePath, PXTextLengthUnkown, PXTextLengthUnkown);
            const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePath;
            PXInt32U checkSum = 0;

            if(hasFilePath)
            {
                // Create checksum
                const PXInt32U checkSum = PXAdler32Create(1, pxText.TextA, pxText.SizeUsed);

                // Check if already loaded
                {
                    PXImage* pxImageFoundEntry = PXNull;

                    const PXBool foundEntry = PXDictionaryFindEntry(&pxResourceManager->ImageLookUp, &checkSum, (void**)&pxImageFoundEntry);

                    if(foundEntry) // image is already loaded
                    {
                        pxImage = pxImageFoundEntry;
                        *pxResourceCreateInfo->ObjectReference = pxImage;

#if PXLogEnable
                        PXLogPrint
                        (
                            PXLoggingWarning,
                            "Resource",
                            "Image-Create",
                            "Skipped <%s> (Redundant)",
                            pxText.TextA
                        );
#endif

                        continue; // Rendundand load, skipt to next
                    }
                }
            }

            if(!pxImage)
            {
                PXNewZerod(PXImage, &pxImage);
                *pxResourceCreateInfo->ObjectReference = pxImage;
            }

            PXImageConstruct(pxImage);

            // Load texture
            if(hasFilePath)
            {
                PXResourceTransphereInfo pxResourceLoadInfo;
                PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.ResourceTarget = pxImage;
                pxResourceLoadInfo.ResourceType = PXResourceTypeImage;
                pxResourceLoadInfo.Manager = pxResourceManager;

                const PXActionResult loadResult = PXResourceLoad(&pxResourceLoadInfo, &pxText);


                if(PXActionSuccessful != loadResult)
                {
#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingError,
                        "Resource",
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
                    "Resource",
                    "Image-Create",
                    "Successful <%s>.",
                    pxText.TextA
                );
#endif

                PXDictionaryAdd(&pxResourceManager->ImageLookUp, &checkSum, pxImage);
            }
            else
            {
                PXImageCopyAsNew(pxImage, &pxImageCreateInfo->Image);

                checkSum = 0;

                PXDictionaryAdd(&pxResourceManager->ImageLookUp, &checkSum, pxImage);
            }

            break;
        }
        case PXResourceTypeTextureCube:
        {
            PXTextureCubeCreateInfo* const pxTextureCubeCreateData = &pxResourceCreateInfo->TextureCube;
            PXTextureCube* pxTextureCube = *(PXTextureCube**)pxResourceCreateInfo->ObjectReference;

            if(!pxTextureCube)
            {
                PXNewZerod(PXTextureCube, &pxTextureCube);
                *pxResourceCreateInfo->ObjectReference = pxTextureCube;
            }

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

            PXResourceManagerAdd(pxResourceManager, pxResourceCreateInfoList, 6);

            break;
        }
        case PXResourceTypeModel:
        {
            PXModelCreateInfo* const pxModelCreateInfo = &pxResourceCreateInfo->Model;
            PXModel* pxModel = *(PXModel**)pxResourceCreateInfo->ObjectReference;

            if(!pxModel)
            {
                PXNewZerod(PXModel, &pxModel);
                *pxResourceCreateInfo->ObjectReference = pxModel;
            }

            PXMesh* const pxMesh = &pxModel->Mesh;
            PXVertexBuffer* const pxVertexBuffer = &pxMesh->VertexBuffer;
            PXIndexBuffer* const pxIndexBuffer = &pxMesh->IndexBuffer;


            // Init
            PXModelConstruct(pxModel);

            // Register
            pxModel->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->ModelLookUp, &pxModel->Info.ID, pxModel);

            // Add scaling
            PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);


            const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePath;

            if(hasFilePath)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Resource",
                    "Model-Create",
                    "ID:%i <%s>.",
                    pxModel->Info.ID,
                    pxResourceCreateInfo->FilePath
                );
#endif

                PXResourceStorePath(&pxResourceManager->SourcePathCache, &pxModel->Info, pxResourceCreateInfo->FilePath, pxResourceCreateInfo->FilePathSize);

                // Load model
                {
                    PXResourceTransphereInfo pxResourceLoadInfo;
                    PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
                    pxResourceLoadInfo.ResourceTarget = pxModel;
                    pxResourceLoadInfo.ResourceType = PXResourceTypeModel;
                    pxResourceLoadInfo.Manager = pxResourceManager;

                    const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePath);
                    const PXBool success = PXActionSuccessful == loadResult;

                    if(!success)
                    {
#if PXLogEnable
                        PXLogPrint
                        (
                            PXLoggingInfo,
                            "Resource",
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
                    "Resource",
                    "Model-Create",
                    "ID:%i internal",
                    pxModel->Info.ID
                );
#endif



                switch(pxModelCreateInfo->Form)
                {
                case PXModelFormCustom:
                {
                    PXCopy(PXVertexBuffer, &pxModelCreateInfo->VertexBuffer, pxVertexBuffer);
                    PXCopy(PXIndexBuffer, &pxModelCreateInfo->IndexBuffer, pxIndexBuffer);

                    // Allocate memory and copy
                    pxVertexBuffer->VertexData = 0;
                    pxVertexBuffer->VertexDataSize = 0;
                    pxIndexBuffer->IndexData = 0;
                    pxIndexBuffer->IndexDataSize = 0;

                    PXNewList(PXByte, pxModelCreateInfo->VertexBuffer.VertexDataSize, &pxModel->Mesh.VertexBuffer.VertexData, &pxModel->Mesh.VertexBuffer.VertexDataSize);
                    PXNewList(PXByte, pxModelCreateInfo->IndexBuffer.IndexDataSize, &pxModel->Mesh.IndexBuffer.IndexData, &pxModel->Mesh.IndexBuffer.IndexDataSize);

                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->VertexBuffer.VertexDataSize,
                        pxModelCreateInfo->VertexBuffer.VertexData,
                        pxModel->Mesh.VertexBuffer.VertexData
                    );
                    PXCopyList
                    (
                        PXByte,
                        pxModelCreateInfo->IndexBuffer.IndexDataSize,
                        pxModelCreateInfo->IndexBuffer.IndexData,
                        pxModel->Mesh.IndexBuffer.IndexData
                    );

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
                    pxVertexBuffer->Format = PXVertexBufferFormatXYFloat;
                    pxVertexBuffer->VertexData = (void*)PXVertexDataTriangle;
                    pxVertexBuffer->VertexDataSize = sizeof(PXVertexDataTriangle);

                    pxIndexBuffer->IndexDataType = PXDataTypeInt08U;
                    pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;
                    pxIndexBuffer->IndexData = (void*)PXIndexDataTriangle;
                    pxIndexBuffer->IndexDataSize = sizeof(PXIndexDataTriangle);



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
                    pxVertexBuffer->Format = PXVertexBufferFormatXYI8;
                    pxVertexBuffer->VertexData = (void*)PXVertexDataRectangle;
                    pxVertexBuffer->VertexDataSize = sizeof(PXVertexDataRectangle);

                    pxIndexBuffer->IndexDataType = PXDataTypeInt08U;
                    pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;// PXDrawModeIDPoint | PXDrawModeIDLineLoop;
                    pxIndexBuffer->IndexData = (void*)PXIndexDataRectangle;
                    pxIndexBuffer->IndexDataSize = sizeof(PXIndexDataRectangle);

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
                    float cx = 0;
                    float cy = 0;
                    float radius = 1;
                    int segmentAmount = 16;

                    pxVertexBuffer->Format = PXVertexBufferFormatXYFloat;
                    PXNewList(float, segmentAmount * 2, &pxVertexBuffer->VertexData, &pxVertexBuffer->VertexDataSize);
                    float* vertexData = (float*)pxVertexBuffer->VertexData;

                    for(PXSize i = 0; i < segmentAmount; ++i)
                    {
                        const float theta = 2.0f * 3.14f * i / (float)segmentAmount;//get the current angle
                        const float x = radius * PXMathCosinus(theta);//calculate the x component
                        const float y = radius * PXMathSinus(theta);//calculate the y component

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
                    pxVertexBuffer->Format = PXVertexBufferFormatXYZI8;
                    pxVertexBuffer->VertexData = (void*)PXVertexDataCube;
                    pxVertexBuffer->VertexDataSize = sizeof(PXVertexDataCube);

                    pxIndexBuffer->IndexDataType = PXDataTypeInt08U;
                    pxIndexBuffer->DrawModeID = PXDrawModeIDTriangle;
                    pxIndexBuffer->IndexData = (void*)PXIndexDataCube;
                    pxIndexBuffer->IndexDataSize = sizeof(PXIndexDataCube);


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

                    float* input = 0;

                    float* output = 0;
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

            pxModel->Info.Flags |= PXResourceInfoRender;

            break;
        }
        case PXResourceTypeMaterial:
        {
            PXMaterial* pxMaterial = *(PXMaterial**)pxResourceCreateInfo->ObjectReference;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "Material-Create",
                "Allocating x%i",
                pxResourceCreateInfo->ObjectAmount
            );
#endif

            if(!pxMaterial)
            {
                PXNewListZerod(PXMaterial, pxResourceCreateInfo->ObjectAmount, &pxMaterial, PXNull);
                *pxResourceCreateInfo->ObjectReference = pxMaterial;
            }

            void* keyList[64];

            for(PXSize materialIndex = 0; materialIndex < pxResourceCreateInfo->ObjectAmount; ++materialIndex)
            {
                PXMaterial* const pxMaterialCurrent = &pxMaterial[materialIndex];

                pxMaterialCurrent->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
                pxMaterialCurrent->Info.Flags |= PXResourceInfoRender;

                keyList[materialIndex] = &pxMaterialCurrent->Info.ID;
            }

            PXDictionaryAddMultible(&pxResourceManager->MaterialLookUp, keyList, pxMaterial, pxResourceCreateInfo->ObjectAmount);

            break;
        }
        case PXResourceTypeTexture2D:
        {
            const PXBool hasImageData = pxResourceCreateInfo->Texture2D.Image.Image.PixelData && pxResourceCreateInfo->Texture2D.Image.Image.PixelDataSize;

            //PXEngineTexture2DCreateData* const pxEngineTexture2DCreateData = &pxEngineResourceCreateInfo->Texture2D;
            PXTexture2D* pxTexture2D = *(PXTexture2D**)pxResourceCreateInfo->ObjectReference;

            // If we dont have a texture file path, instead of loading nothing, we
            // can pass the fail-back texture back. This prevents redundant materials and missing texture material
            {
                const PXBool hasFilePath = PXNull != pxResourceCreateInfo->FilePath;

                if(!hasFilePath)
                {
                    // Do we have any other data to load?
                    if(!hasImageData)
                    {
                        // Load failback texture
                        *pxResourceCreateInfo->ObjectReference = pxResourceManager->Texture2DFailBack;

                        return PXActionSuccessful;
                    }
                }
            }

            // We have a texture to load, but we need a new one
            if(!pxTexture2D)
            {
                PXNewZerod(PXTexture2D, &pxTexture2D);
                *pxResourceCreateInfo->ObjectReference = pxTexture2D;
            }

            pxTexture2D->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->TextureLookUp, &pxTexture2D->Info.ID, pxTexture2D);


            pxTexture2D->Filter = PXGraphicRenderFilterNoFilter;
            pxTexture2D->LayoutNear = PXGraphicImageLayoutNearest;
            pxTexture2D->LayoutFar = PXGraphicImageLayoutNearest;
            pxTexture2D->WrapHeight = PXGraphicImageWrapRepeat;
            pxTexture2D->WrapWidth = PXGraphicImageWrapRepeat;


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "Texture2D-Create",
                "ID:%i <%s>.",
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

                PXResourceManagerAdd(pxResourceManager, &pxResourceCreateInfoSub, 1);
            }

            break;
        }
        case PXResourceTypeFont:
        {
            PXEngineFontCreateInfo* const pxEngineFontCreateData = &pxResourceCreateInfo->Font;
            PXFont* pxFont = *(PXFont**)pxResourceCreateInfo->ObjectReference;

            // create if not exist
            if(!pxFont)
            {
                PXNewZerod(PXFont, &pxFont);
                *pxResourceCreateInfo->ObjectReference = pxFont;
            }

            pxFont->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->FontLookUp, &pxFont->Info.ID, pxFont);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "Font-Create",
                "ID:%i <%s>",
                pxFont->Info.ID,
                pxResourceCreateInfo->FilePath

            );
#endif

            // Load font
            {
                PXResourceTransphereInfo pxResourceLoadInfo;
                PXClear(PXResourceTransphereInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.ResourceTarget = pxFont;
                pxResourceLoadInfo.ResourceType = PXResourceTypeFont;
                pxResourceLoadInfo.Manager = pxResourceManager;

                const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePath);

                PXActionReturnOnError(loadResult);
            }

            break;
        }
        case PXResourceTypeSkybox:
        {
            PXSkyBoxCreateEventInfo* const pxSkyBoxCreateEventData = &pxResourceCreateInfo->SkyBox;
            PXSkyBox* pxSkyBox = *(PXSkyBox**)pxResourceCreateInfo->ObjectReference;

            if(!pxSkyBox)
            {
                PXNewZerod(PXSkyBox, &pxSkyBox);
                *pxResourceCreateInfo->ObjectReference = pxSkyBox;
            }

            pxSkyBox->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->SkyBoxLookUp, &pxSkyBox->Info.ID, pxSkyBox);
            pxSkyBox->Info.Flags |= PXResourceInfoExist;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "SkyBox-Create",
                "ID:%i\n"
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
                "Forward", pxSkyBoxCreateEventData->SkyBoxTextureF
            );
#endif

            // Load Textures
            {
                PXResourceCreateInfo pxResourceCreateInfoList[6];
                PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoList, 6);

                // Skybox CubeTexture
                pxResourceCreateInfoList[0].Type = PXResourceTypeTextureCube;
                pxResourceCreateInfoList[0].SpawnEnabled = PXTrue;
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

                PXResourceManagerAdd(pxResourceManager, pxResourceCreateInfoList, 2);

                PXCopy(PXShaderProgramCreateInfo, &pxResourceCreateInfoList[1].ShaderProgram, pxSkyBoxCreateEventData);
            }

            break;
        }
        case PXResourceTypeSprite:
        {
            PXSpriteCreateInfo* const pxSpriteCreateEventData = &pxResourceCreateInfo->Sprite;
            PXSprite* pxSprite = *(PXSprite**)pxResourceCreateInfo->ObjectReference;

            if(!pxSprite)
            {
                PXNewZerod(PXSprite, &pxSprite);
                *pxResourceCreateInfo->ObjectReference = pxSprite;
            }

            pxSprite->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            pxSprite->Info.Flags |= PXResourceInfoRender;

            PXDictionaryAdd(&pxResourceManager->SpritelLookUp, &pxSprite->Info.ID, pxSprite);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "Sprite-Create",
                "ID:%i, Use <%s>",
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
                pxResourceCreateInfoList[0].SpawnEnabled = PXTrue;
                pxResourceCreateInfoList[0].ObjectReference = (void**)&pxSprite->HitBox;
                pxResourceCreateInfoList[0].HitBox.Model = pxSprite->Model;
                pxResourceCreateInfoList[0].HitBox.Behaviour = 0;

                PXResourceManagerAdd(pxResourceManager, pxResourceCreateInfoList, 1);
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



            float aspectRationX = 1;

            if(pxSprite->Texture)
            {
                if(pxSprite->Texture->Image->Width && pxSprite->Texture->Image->Height)
                {
                    aspectRationX = (float)pxSprite->Texture->Image->Width / (float)pxSprite->Texture->Image->Height;
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





            break;
        }
        case PXResourceTypeText:
        {
            //PXEngineTextCreateData* const pxEngineTextCreateData = &pxEngineResourceCreateInfo->Text;
            PXEngineText* pxEngineText = *(PXEngineText**)pxResourceCreateInfo->ObjectReference;

            if(!pxEngineText)
            {
                PXNewZerod(PXEngineText, &pxEngineText);
                *pxResourceCreateInfo->ObjectReference = pxEngineText;
            }

            pxEngineText->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->TextLookUp, &pxEngineText->Info.ID, pxEngineText);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "Text-Create",
                "ID:%i",
                pxEngineText->Info.ID
            );
#endif


#if 0
            //---------------------------------------------
            // Trigger enable
            //---------------------------------------------
            if(pxEngineResourceCreateInfo->SpawnEnabled)
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfo;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

                pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeStateChange;
                pxEngineResourceActionInfo.ChangeInfo.Enable = PXTrue;
                pxEngineResourceActionInfo.ChangeInfo.Type = PXEngineCreateTypeText;
                pxEngineResourceActionInfo.ChangeInfo.Object = pxEngineText;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
            }
            //---------------------------------------------
#endif
            break;
        }
        case PXResourceTypeTimer:
        {
            PXEngineTimer* pxEngineTimer = *(PXEngineTimer**)pxResourceCreateInfo->ObjectReference;

            if(!pxEngineTimer)
            {
                PXNewZerod(PXEngineTimer, &pxEngineTimer);
                *pxResourceCreateInfo->ObjectReference = pxEngineTimer;
            }

            pxEngineTimer->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->TimerLookUp, &pxEngineTimer->Info.ID, pxEngineTimer);


            pxEngineTimer->Owner = pxEngineTimer->Owner;
            pxEngineTimer->CallBack = pxEngineTimer->CallBack;
            pxEngineTimer->TimeDeltaTarget = pxEngineTimer->TimeDeltaTarget;



#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "Timer-Create",
                "ID:%i",
                pxEngineTimer->Info.ID
            );
#endif

            pxEngineTimer->TimeStampStart = PXTimeCounterStampGet();

            //---------------------------------------------
            // Trigger enable
            //---------------------------------------------
            if(pxResourceCreateInfo->SpawnEnabled)
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

            break;
        }
        case PXResourceTypeSound:
        {
            PXSound* pxSound = *(PXSound**)pxResourceCreateInfo->ObjectReference;

            if(!pxSound)
            {
                PXNewZerod(PXSound, &pxSound);
                *pxResourceCreateInfo->ObjectReference = pxSound;
            }


            pxSound->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->SoundLookUp, &pxSound->Info.ID, pxSound);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "Sound-Create",
                "ID:%i, %s",
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
                    "Resource",
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
                "Resource",
                "Sound-Load",
                "successful <%s>.",
                pxResourceCreateInfo->FilePath
            );
#endif

            break;
        }
        case PXResourceTypeEngineSound:
        {
            PXEngineSoundCreateInfo* const pxEngineSoundCreateInfo = &pxResourceCreateInfo->Sound;
            PXEngineSound* pxEngineSound = *(PXEngineSound**)pxResourceCreateInfo->ObjectReference;

            // Create if not exists
            if(!pxEngineSound)
            {
                PXNewZerod(PXEngineSound, &pxEngineSound);
                *pxResourceCreateInfo->ObjectReference = pxEngineSound;
            }

            //  pxEngineSound->Info.Handle.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            // PXDictionaryAdd(&pxResourceManager->SoundLookUp, &pxEngineSound->Info.Handle.ID, pxSound);

            // Register
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    "Resource",
                    "Sound",
                    "Register",
                    PXNull
                );
#endif



            }

#if 0
            // Load
            {
                PXEngineResourceActionInfo pxEngineResourceActionInfo;
                PXClear(PXEngineResourceActionInfo, &pxEngineResourceActionInfo);

                pxEngineResourceActionInfo.Type = PXEngineResourceActionTypeCreate;
                pxEngineResourceActionInfo.Create.CreateType = PXEngineCreateTypeSound;
                pxEngineResourceActionInfo.Create.ObjectReference = &pxEngineSound->Sound;
                pxEngineResourceActionInfo.Create.FilePath = pxEngineResourceCreateInfo->FilePath;

                PXEngineResourceAction(pxEngine, &pxEngineResourceActionInfo);
            }
#endif

            break;
        }
        case PXResourceTypeShaderProgram:
        {
            PXShaderProgramCreateInfo* const pxShaderProgramCreateData = &pxResourceCreateInfo->ShaderProgram;
            PXShaderProgram* pxShaderProgram = *(PXShaderProgram**)pxResourceCreateInfo->ObjectReference;

            // Make sure object exists
            {
                if(!pxShaderProgram)
                {
                    PXNewZerod(PXShaderProgram, &pxShaderProgram);
                    *pxResourceCreateInfo->ObjectReference = pxShaderProgram;
                }
            }

            pxShaderProgram->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->ShaderProgramLookup, &pxShaderProgram->Info.ID, pxShaderProgram);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "ShaderProgram-Create",
                "ID:%i",
                pxShaderProgram->Info.ID
            );
#endif

            // is the data already present?
            const PXBool isDataPrensent = pxShaderProgramCreateData->ShaderVertexText && pxShaderProgramCreateData->ShaderPixelText;

            if(isDataPrensent)
            {
                PXFileOpenInfo pxFileOpenInfo;
                PXClear(PXFileOpenInfo, &pxFileOpenInfo);
                pxFileOpenInfo.FlagList = PXFileIOInfoFileMemory;
                pxFileOpenInfo.BufferData = pxShaderProgramCreateData->ShaderVertexText;
                pxFileOpenInfo.BufferSize = pxShaderProgramCreateData->ShaderVertexTextSize;

                PXFileOpen(&pxShaderProgramCreateData->ShaderVertexFile, &pxFileOpenInfo);

                pxFileOpenInfo.BufferData = pxShaderProgramCreateData->ShaderPixelText;
                pxFileOpenInfo.BufferSize = pxShaderProgramCreateData->ShaderPixelTextSize;

                PXFileOpen(&pxShaderProgramCreateData->ShaderPixelFile, &pxFileOpenInfo);

                pxShaderProgram->Info.Flags |= PXResourceInfoStorageMemory;
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

                pxShaderProgram->Info.Flags |= PXResourceInfoStorageMemory;
            }

            break;
        }
        case PXResourceTypeHitBox:
        {
            PXHitBox* pxHitBox = *(PXHitBox**)pxResourceCreateInfo->ObjectReference;

            if(!pxHitBox)
            {
                PXNewZerod(PXHitBox, &pxHitBox);
                *pxResourceCreateInfo->ObjectReference = pxHitBox;
            }

            pxHitBox->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->HitBoxLookUp, &pxHitBox->Info.ID, pxHitBox);

            pxHitBox->Info.Flags |= PXResourceInfoActive;
            pxHitBox->Info.Behaviour = pxResourceCreateInfo->HitBox.Behaviour;
            pxHitBox->Model = pxResourceCreateInfo->HitBox.Model;

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "HitBox-Create",
                "ID:%i",
                pxHitBox->Info.ID
            );
#endif

            break;
        }
        case PXResourceTypeGUIElement:
        {
            PXGUIElementCreateInfo* const pxGUIElementCreateInfo = &pxResourceCreateInfo->UIElement;
            PXGUIElement* pxGUIElement = *(PXGUIElement**)pxResourceCreateInfo->ObjectReference;

            if(!pxGUIElement)
            {
                PXNewZerod(PXGUIElement, &pxGUIElement);
                *pxResourceCreateInfo->ObjectReference = pxGUIElement;
            }

            pxGUIElement->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->GUIElementLookup, &pxGUIElement->Info.ID, pxGUIElement);

            pxGUIElement->Info.Flags |= PXResourceInfoActive;

            break;
        }
        case PXResourceTypeSpriteAnimator:
        {
            PXSpriteAnimatorInfo* const pxSpriteAnimatorInfo = &pxResourceCreateInfo->SpriteAnimator;
            PXSpriteAnimator* pxSpriteAnimator = *(PXSpriteAnimator**)pxResourceCreateInfo->ObjectReference;

            if(!pxSpriteAnimator)
            {
                PXNewZerod(PXSpriteAnimator, &pxSpriteAnimator);
                *pxResourceCreateInfo->ObjectReference = pxSpriteAnimator;
            }

            pxSpriteAnimator->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
            PXDictionaryAdd(&pxResourceManager->SpriteAnimator, &pxSpriteAnimator->Info.ID, pxSpriteAnimator);

            pxSpriteAnimator->Info.Flags |= PXResourceInfoActive;

            pxSpriteAnimator->Info.Behaviour = pxSpriteAnimatorInfo->Behaviour;
            pxSpriteAnimator->SpriteTarget = pxSpriteAnimatorInfo->SpriteTarget;
            pxSpriteAnimator->RateUpdate = pxSpriteAnimatorInfo->UpdateRate;
            pxSpriteAnimator->TimeStampAmount = pxSpriteAnimatorInfo->TimeStampAmount;

            PXNewList(PXSpriteAnimatorTimeStamp, pxSpriteAnimatorInfo->TimeStampAmount, &pxSpriteAnimator->TimeStampList, PXNull);
            PXCopyList(PXSpriteAnimatorTimeStamp, pxSpriteAnimatorInfo->TimeStampAmount, pxSpriteAnimatorInfo->TimeStampList, pxSpriteAnimator->TimeStampList);


#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                "Resource",
                "SpriteAnimator",
                "OK"
            );
#endif

            break;
        }
        default:
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                "Resource",
                "Add",
                "Type is invalid"
            );
#endif

            return PXActionRefusedArgumentInvalid;
        }
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceStoreName(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize)
{
#if PXLogEnable
    char buffer[256];

    PXTextCopyA(name, nameSize, buffer, nameSize);

    PXLogPrint
    (
        PXLoggingInfo,
        "Resource",
        "Store-Name",
        "%s",
        buffer
    );
#endif

    PXListDynamicAdd(&pxResourceManager->NameCache, &pxResourceInfo->ID, name, nameSize);

    pxResourceInfo->Flags |= PXResourceInfoHasName;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceStorePath(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, const char* const name, const PXSize nameSize)
{
#if PXLogEnable
    char buffer[256];

    PXTextCopyA(name, nameSize, buffer, nameSize);

    PXLogPrint
    (
        PXLoggingInfo,
        "Resource",
        "Store-Path",
        "%s",
        buffer
    );
#endif

    PXListDynamicAdd(&pxResourceManager->NameCache, &pxResourceInfo->ID, name, nameSize);

    pxResourceInfo->Flags |= PXResourceInfoHasSource;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceFetchName(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize)
{
    PXListDynamicGet(&pxResourceManager->NameCache, &pxResourceInfo->ID, name, nameSize);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceFetchPath(PXResourceManager* const pxResourceManager, PXResourceInfo* const pxResourceInfo, char** name, PXSize* nameSize)
{
    PXListDynamicGet(&pxResourceManager->SourcePathCache, pxResourceInfo->ID, name, nameSize);

    return PXActionSuccessful;
}

void PXAPI PXIconLoad(PXIcon* const pxIcon)
{
#if OSUnix
    return PXActionRefusedNotImplemented;
#elif OSWindows
    pxIcon->Info.Handle.IconHandle = LoadIcon(NULL, IDI_QUESTION);
#else
    return PXActionRefusedNotSupportedByOperatingSystem;
#endif
}

void PXAPI PXRectangleOffsetSet(PXRectangleOffset* const pxRectangleOffset, float left, float top, float right, float bottom)
{
    pxRectangleOffset->Left = left;
    pxRectangleOffset->Top = top;
    pxRectangleOffset->Right = right;
    pxRectangleOffset->Bottom = bottom;
}

PXMaterial* PXAPI PXMaterialContainerFind(PXResourceManager* const pxResourceManager, const PXMaterialContainer* const pxMaterialContainer, struct PXText_* const pxMaterialName)
{
    if (!pxMaterialContainer)
    {
        return PXNull;
    }

    //for (PXSize materialContainerID = 0; materialContainerID < pxMaterialContainer->MaterialContaierListSize; ++materialContainerID)
    //{
    // const PXMaterialContainer* const pxMaterialContaier = &pxModel->MaterialContaierList[materialContainerID];

    for (PXSize materialID = 0; materialID < pxMaterialContainer->MaterialListAmount; ++materialID)
    {
        PXMaterial* const pxMaterial = &pxMaterialContainer->MaterialList[materialID];
        char* name = 0;
        PXSize nameSize = 0;

        PXResourceFetchName(pxResourceManager, &pxMaterial->Info, &name, &nameSize);

        const PXBool isMatch = PXTextCompareA(pxMaterialName->TextA, pxMaterialName->SizeUsed, name, nameSize);

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
    const PXInt8U stride = PXVertexBufferFormatStrideSize(pxVertexBuffer->Format);
    const PXSize rowEntiry = stride * index;

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
    case PXVertexBufferFormatXYZFloat:
    {
        switch (pxVertexBufferDataType)
        {
        case PXVertexBufferDataTypeVertex:
            return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 0];

        default:
            return PXNull;
        }
    }
    case PXVertexBufferFormatN3F_XYZ:
    {
        switch(pxVertexBufferDataType)
        {
        case PXVertexBufferDataTypeVertex:
            return &((float*)pxVertexBuffer->VertexData)[rowEntiry + 3];

        case PXVertexBufferDataTypeNormal:
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

void PXAPI PXModelFormatTransmute(PXModel* const pxModel, PXModelFormatTransmuteInfo* const pxModelFormatTransmuteInfo)
{



    //-----------------------------------------------------
    // Vertex
    //-----------------------------------------------------
    PXVertexBufferFormat oldFormat = pxModel->Mesh.VertexBuffer.Format;

    switch(pxModel->Mesh.VertexBuffer.Format)
    {
    case PXVertexBufferFormatXYI8:
    {
        float* newVertexArray = 0;
        PXSize newVertexArraySize = 0;
        PXSize amountFuture = PXVertexBufferFormatStrideSize(PXVertexBufferFormatT2F_XYZ);
        PXSize amountCurrent = PXVertexBufferFormatStrideSize(PXVertexBufferFormatXYI8);
        PXSize sizeBefore = pxModel->Mesh.VertexBuffer.VertexDataSize;
        PXSize sizeCurrent = (pxModel->Mesh.VertexBuffer.VertexDataSize / 2) * amountFuture;

        PXNewList(float, sizeCurrent, &newVertexArray, &newVertexArraySize);

        PXInt8S* dataSource = (PXInt8S*)pxModel->Mesh.VertexBuffer.VertexData;

        PXSize newOffset = 0;

        for(PXSize i = 0; i < sizeBefore; i+=2)
        {
            newVertexArray[newOffset++] = (dataSource[i + 0] + 1) / 2.0f;
            newVertexArray[newOffset++] = -(dataSource[i + 1] + 1) / 2.0f;
            newVertexArray[newOffset++] = dataSource[i + 0];
            newVertexArray[newOffset++] = dataSource[i + 1];
            newVertexArray[newOffset++] = 0.0f;
        }

        // Memory leak?

        pxModel->Mesh.VertexBuffer.Format = PXVertexBufferFormatT2F_XYZ;
        pxModel->Mesh.VertexBuffer.VertexData = newVertexArray;
        pxModel->Mesh.VertexBuffer.VertexDataSize = newVertexArraySize;

        break;
    }
    case PXVertexBufferFormatXYZI8:
    {
        float* newVertexArray = 0;
        PXSize newVertexArraySize = 0;
        PXSize amountFuture = PXVertexBufferFormatStrideSize(PXVertexBufferFormatXYZFloat);
        PXSize amountCurrent = PXVertexBufferFormatStrideSize(PXVertexBufferFormatXYZI8);
        PXSize sizeCurrent = pxModel->Mesh.VertexBuffer.VertexDataSize / 1;

        PXNewList(float, sizeCurrent, &newVertexArray, &newVertexArraySize);

        PXInt8S* dataSource = (PXInt8S*)pxModel->Mesh.VertexBuffer.VertexData;

        for(size_t i = 0; i < sizeCurrent; i++)
        {
            newVertexArray[i] = dataSource[i];
        }

        // Memory leak?

        pxModel->Mesh.VertexBuffer.Format = PXVertexBufferFormatXYZFloat;
        pxModel->Mesh.VertexBuffer.VertexData = newVertexArray;
        pxModel->Mesh.VertexBuffer.VertexDataSize = newVertexArraySize;


        break;
    }
    default:
    {
        break;
    }
    }

    PXVertexBufferFormat newFormat = pxModel->Mesh.VertexBuffer.Format;

    const char* oldFomatText = PXVertexBufferFormatToString(oldFormat);
    const char* newFomatText = PXVertexBufferFormatToString(newFormat);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        "Resource",
        "Model-Format",
        "Transmute <%s> to <%s>",
        oldFomatText,
        newFomatText
    );
#endif


    //-----------------------------------------------------
    // Index
    //-----------------------------------------------------

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
    case PXUIElementTypeCustom:
        return "Custom";
    case PXUIElementTypeWindow:
        return "Window";
    case PXUIElementTypePanel:
        return "Panel";
    case PXUIElementTypeText:
        return "Text";
    case PXUIElementTypeButton:
        return "Button";
    case PXUIElementTypeImage:
        return "Image";
    case PXUIElementTypeDropDown:
        return "DropDown";
    case PXUIElementTypeListBox:
        return "ListBox";
    case PXUIElementTypeTextEdit:
        return "TextEdit";
    case PXUIElementTypeRichEdit:
        return "RichText";
    case PXUIElementTypeScrollBar:
        return "ScrollBar";
    case PXUIElementTypeTrackBar:
        return "TrackBar";
    case PXUIElementTypeStatusBar:
        return "StatusBar";
    case PXUIElementTypeUpDown:
        return "UpDown";
    case PXUIElementTypeProgressBar:
        return "ProgressBar";
    case PXUIElementTypeHotKey:
        return "HotKey";
    case PXUIElementTypeCalender:
        return "Calender";
    case PXUIElementTypeToolTip:
        return "ToolTip";
    case PXUIElementTypeAnimate:
        return "Animate";
    case PXUIElementTypeDatePicker:
        return "DatePicker";
    case PXUIElementTypeGroupBox:
        return "GroupBox";
    case PXUIElementTypeRadioButton:
        return "RadioButton";
    case PXUIElementTypeGroupRadioButton:
        return "GroupRadioButton";
    case PXUIElementTypeTreeView:
        return "TreeView";
    case PXUIElementTypeTreeViewItem:
        return "TreeViewItem";
    case PXUIElementTypeFileDirectyView:
        return "FileDirectyView";
    case PXUIElementTypeFileDirectyViewEntry:
        return "FileDirectyViewEntry";
    case PXUIElementTypeIPInput:
        return "IPInput";
    case PXUIElementTypeLink:
        return "Link";
    case PXUIElementTypeHeader:
        return "Header";
    case PXUIElementTypeFontSelector:
        return "FontSelector";
    case PXUIElementTypePageScroll:
        return "PageScroll";
    case PXUIElementTypeTabControll:
        return "TabControll";
    case PXUIElementTypeToggle:
        return "Toggle";
    case PXUIElementTypeCheckBox:
        return "CheckBox";
    case PXUIElementTypeComboBox:
        return "ComboBox";
    case PXUIElementTypeColorPicker:
        return "ColorPicker";
    case PXUIElementTypeSlider:
        return "Slider";
    case PXUIElementTypeRenderFrame:
        return "FrameRender";

    default:
        return PXNull;
    }
}

void PXAPI PXGUIElementBrushColorSet(PXGUIElementBrush* const pxGUIElementBrush, const PXByte red, const PXByte green, const PXByte blue)
{
    pxGUIElementBrush->Info.Behaviour |= PXGUIElementBrushBehaviourColorEmbeded;
    pxGUIElementBrush->ColorDate.Red = red;
    pxGUIElementBrush->ColorDate.Green = green;
    pxGUIElementBrush->ColorDate.Blue = blue;
}

void PXAPI PXUIElementPositionCalculcate(PXGUIElement* const pxGUIElement, PXUIElementPositionCalulcateInfo* const pxUIElementPositionCalulcateInfo)
{
    for(
        PXGUIElement* pxUIElementParent = (PXGUIElement*)pxGUIElement->Info.Hierarchy.Parrent;
        pxUIElementParent;
        pxUIElementParent = (PXGUIElement*)pxUIElementParent->Info.Hierarchy.Parrent
    )
    {
        pxUIElementPositionCalulcateInfo->MarginLeft += pxUIElementParent->Position.MarginLeft;
        pxUIElementPositionCalulcateInfo->MarginTop += pxUIElementParent->Position.MarginTop;
        pxUIElementPositionCalulcateInfo->MarginRight += pxUIElementParent->Position.MarginRight;
        pxUIElementPositionCalulcateInfo->MarginBottom += pxUIElementParent->Position.MarginBottom;
    }

    pxUIElementPositionCalulcateInfo->MarginLeft += pxGUIElement->Position.MarginLeft;
    pxUIElementPositionCalulcateInfo->MarginTop += pxGUIElement->Position.MarginTop;
    pxUIElementPositionCalulcateInfo->MarginRight += pxGUIElement->Position.MarginRight;
    pxUIElementPositionCalulcateInfo->MarginBottom += pxGUIElement->Position.MarginBottom;

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


    if(PXGUIElementKeepWidth & pxGUIElement->Info.Behaviour)
    {
        pxUIElementPositionCalulcateInfo->Width = pxGUIElement->Position.Width;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Width = mathWithScaling;
    }

    if(PXGUIElementKeepHeight & pxGUIElement->Info.Behaviour)
    {
        pxUIElementPositionCalulcateInfo->Height = pxGUIElement->Position.Height;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Height = heightWithScaling;
    }


    switch(PXGUIElementAllignFlags & pxGUIElement->Info.Behaviour)
    {
    default:
    case PXGUIElementAllignLeft:
    {
        pxUIElementPositionCalulcateInfo->X = remLeft;
        pxUIElementPositionCalulcateInfo->Y = remTop;
        break;
    }
    case PXGUIElementAllignTop:
    {
        pxUIElementPositionCalulcateInfo->X = remLeft;
        pxUIElementPositionCalulcateInfo->Y = remTop;
        break;
    }
    case PXGUIElementAllignRight:
    {
        pxUIElementPositionCalulcateInfo->X = pxUIElementPositionCalulcateInfo->WindowWidth - (remRight + pxUIElementPositionCalulcateInfo->Width);
        pxUIElementPositionCalulcateInfo->Y = pxUIElementPositionCalulcateInfo->WindowHeight - (remBottom + pxUIElementPositionCalulcateInfo->Height);
        break;

    }
    case PXGUIElementAllignBottom:
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

    switch (pxFileTypeInfo->FormatExpected)
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

PXActionResult PXAPI PXResourceManagerReferenceValidate(PXResourceManager* const pxResourceManager, PXResourceReference* const pxResourceReference)
{
    if(!(pxResourceManager && pxResourceReference))
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
                "Resource",
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
                "Resource",
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
            "Resource",
            "Load-Peek",
            "Start"
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
                "Resource",
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
            "Resource",
            "Load-Peek",
            "Took:%6.3fs",
            pxResourceLoadInfo->TimePeek
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
            "Resource",
            "Load",
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
            "Resource",
            "Load",
            "Preallocate took:<%6.3f>",
            pxResourceLoadInfo->TimeDeviceDataRegister
        );
#endif
    }

    // Try to load assumed format
    if(pxResourceLoadInfo->ResourceLoad)
    {
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
                "Resource",
                "Load",
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
            "Resource",
            "Load",
            "Success. Took:%6.3f  ROPs:%-7i <%s>",
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
                "Resource",
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
                "Resource",
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
        const float timeDelta = PXTimeCounterStampToSecoundsF(timeStampB);

        PXLogPrint
        (
            PXLoggingInfo,
            "Resource",
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
