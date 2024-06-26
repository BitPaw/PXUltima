#include "PXResource.h"

#include <OS/Console/PXConsole.h>
#include <OS/Time/PXStopWatch.h>
#include <OS/Hardware/PXProcessor.h>
#include <Media/PXText.h>
#include <Math/PXMath.h>

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

extern void _chkstk(size_t s) {};
extern int _fltused = 0;



const float PXVertexDataTriangle[] =
{
    -1.0f, -1.0f,
    1.0f, -1.0f,
    0.5f,  1.0f
};

const PXInt8S PXVertexDatarectangle[] =
{
    -1, -1,
     1, -1,
    -1,  1,
     1,  1
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
        case PXVertexBufferFormatXYI8: return "XY 8-Bit";
        case PXVertexBufferFormatXYZI8: return "XYZ 8-Bit";
        case PXVertexBufferFormatXYFloat: return "XY float";
        case PXVertexBufferFormatXYZFloat: return "XYZ float";
        case PXVertexBufferFormatXYZW: return "XYZW float";
        case PXVertexBufferFormatC4UB_XY: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatC4UB_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatRGBXYZ: return "RGBXYZ float";
        case PXVertexBufferFormatN3F_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatC4F_N3F_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatT2F_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatT4F_XYZW: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatT2F_C4UB_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatT2F_C3F_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatT2F_N3F_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatT2F_C4F_N3F_XYZ: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatT4F_C4F_N3F_XYZW: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZC: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZRHW: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZHWC: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZB1: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZB2: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZB3: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZB4: return "xxxxxxxxxxxxxxx";
        case PXVertexBufferFormatXYZB5: return "xxxxxxxxxxxxxxx";  

        default:
            return "Invalid";
    }
}

PXInt8U PXAPI PXVertexBufferFormatStrideSize(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch (pxVertexBufferFormat)
    {
        case PXVertexBufferFormatXYI8: return 2;
        case PXVertexBufferFormatXYZI8: return 3;
        case PXVertexBufferFormatT2F_XYZ: return 2u + 3u;
        case PXVertexBufferFormatT2F_N3F_XYZ: return 2u + 3u + 3u;
        case PXVertexBufferFormatXYZFloat: return 3u;
        case PXVertexBufferFormatN3F_XYZ: return 3u + 3u;

        default:
            return 0u;
    }
}

const char* PXAPI PXResourceTypeToString(const PXResourceType pxResourceType)
{
    switch(pxResourceType)
    {
        case PXResourceTypeCustom: return "Custom";
        case PXResourceTypeModel: return "Model";
        case PXResourceTypeFont: return "Font";
        case PXResourceTypeTexture2D: return "Texture2D";
        case PXResourceTypeImage: return "Image";
        case PXResourceTypeTextureCube: return "TetxureCube";
        case PXResourceTypeShaderProgram: return "ShaderProgram";
        case PXResourceTypeSkybox: return "SkyBox";
        case PXResourceTypeSprite: return "Sprite";
        case PXResourceTypeText: return "Text";
        case PXResourceTypeTimer: return "Timer";
        case PXResourceTypeSound: return "Sound";
        case PXResourceTypeEngineSound: return "PXSound";
        case PXResourceTypeGUIElement: return "UI";
        case PXResourceTypeHitBox: return "HitBox";
        case PXResourceTypeDialogBox: return "DialogBox";

        default:
            return PXNull;
    }
}

void PXAPI PXResourceManagerInit(PXResourceManager* const pxResourceManager)
{
    PXClear(PXResourceManager, pxResourceManager);

    PXFlexDataCacheInit(&pxResourceManager->NameCache, sizeof(PXInt32U));
    PXFlexDataCacheInit(&pxResourceManager->SourcePathCache, sizeof(PXInt32U));

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
}

void PXAPI PXResourceManagerRelease(PXResourceManager* const pxResourceManager)
{

}

PXInt32U PXAPI PXResourceManagerGenerateUniqeID(PXResourceManager* const pxResourceManager)
{
    return 1000 + ++pxResourceManager->UniqeIDGeneratorCounter;
}

PXActionResult PXAPI PXResourceManagerAdd(PXResourceManager* const pxResourceManager, PXResourceCreateInfo* const pxResourceCreateInfoList, const PXSize amount)
{
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

               // Color xx = Color(255, 0, 0, 255);
               // SolidBrush ww = opaqueBrush();

                pxGUIElementBrush->Info.BrushHandle = brushHandle;
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
                    PXResourceLoadInfo pxResourceLoadInfo;
                    PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
                    pxResourceLoadInfo.Target = pxImage;
                    pxResourceLoadInfo.Type = PXResourceTypeImage;

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

                    // Load model
                    {
                        PXResourceLoadInfo pxResourceLoadInfo;
                        PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
                        pxResourceLoadInfo.Target = pxModel;
                        pxResourceLoadInfo.Type = PXResourceTypeModel;

                        PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePath);

                        //  PXFilePathStructure pxFilePathStructure;

                        //  PXFilePathSplitt(&pxText, &pxFilePathStructure);

                       //   PXTextCopyA(pxFilePathStructure.FileName.TextA, pxFilePathStructure.FileName.SizeUsed, pxModel->ResourceID.Name, ResourceIDNameLength);

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
                            pxVertexBuffer->VertexData = (void*)PXVertexDatarectangle;
                            pxVertexBuffer->VertexDataSize = sizeof(PXVertexDatarectangle);

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
                }


                // Load additional resources
                {
                    // Load textures

                    for(PXSize i = 0; i < pxModel->MaterialContaierListAmount; i++)
                    {
                        PXMaterialContainer* const pxMaterialContainer = &pxModel->MaterialContaierList[i];

                        for(PXSize i = 0; i < pxMaterialContainer->MaterialListAmount; i++)
                        {
                            PXMaterial* const pxMaterial = &pxMaterialContainer->MaterialList[i];

                            PXResourceCreateInfo pxResourceCreateInfo;
                            PXClear(PXResourceCreateInfo, &pxResourceCreateInfo);
                            pxResourceCreateInfo.Type = PXResourceTypeTexture2D;
                            pxResourceCreateInfo.ObjectReference = (void**)&pxMaterial->DiffuseTexture;
                            pxResourceCreateInfo.FilePath = pxMaterial->DiffuseTextureFilePath;

                            const PXActionResult resultA = PXResourceManagerAdd(pxResourceManager, &pxResourceCreateInfo, 1);
                        }
                    }
                }

                // Setup    
               // PXMatrix4x4FScaleBy(&pxModel->ModelMatrix, pxModelCreateInfo->Scale);

                pxModel->ShaderProgramReference = pxModelCreateInfo->ShaderProgramReference;

                pxModel->Info.Flags |= PXEngineResourceInfoVisble;

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
                PXDictionaryAdd(&pxResourceManager->FontLookUp, &pxTexture2D->Info.ID, pxTexture2D);


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
                    pxResourceCreateInfoSub.Image = pxResourceCreateInfo->Texture2D.Image;

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
                    PXResourceLoadInfo pxResourceLoadInfo;
                    PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
                    pxResourceLoadInfo.Target = pxFont;
                    pxResourceLoadInfo.Type = PXResourceTypeFont;

                    const PXActionResult loadResult = PXResourceLoadA(&pxResourceLoadInfo, pxResourceCreateInfo->FilePath);

                    PXActionReturnOnError(loadResult);
                }

                // Register as normal
                {
                    // Load Textures
                    {
                        PXFontPage* const pxFontPage = &pxFont->MainPage;

                        PXResourceCreateInfo pxResourceCreateInfoList;
                        PXClear(PXResourceCreateInfo, &pxResourceCreateInfoList);

                        pxResourceCreateInfoList.Type = PXResourceTypeTexture2D;
                        pxResourceCreateInfoList.ObjectReference = (void**)&pxFontPage->Texture;
                        pxResourceCreateInfoList.FilePath = pxFontPage->TextureFilePath;

                        PXResourceManagerAdd(pxResourceManager, &pxResourceCreateInfoList, 1);
                    }

                    for(PXSize i = 0; i < pxFont->AdditionalPageListSize; ++i)
                    {
                        PXFontPage* const pxFontPage = &pxFont->AdditionalPageList[i];
                    }
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
                pxSkyBox->Info.Flags |= PXEngineResourceInfoExist;

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
                pxSprite->Info.Flags |= PXEngineResourceInfoVisble;

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

                // Load texture & sprite model
                {
                    PXResourceCreateInfo pxResourceCreateInfoSub[3];
                    PXClearList(PXResourceCreateInfo, &pxResourceCreateInfoSub, 3);
                    PXSize amount = 2;

                    pxResourceCreateInfoSub[0].Type = PXResourceTypeTexture2D;
                    pxResourceCreateInfoSub[0].ObjectReference = (void**)&pxSprite->Texture;
                    pxResourceCreateInfoSub[0].FilePath = pxResourceCreateInfo->FilePath;

                    pxResourceCreateInfoSub[1].Type = PXResourceTypeModel;
                    pxResourceCreateInfoSub[1].ObjectReference = (void**)&pxSprite->Model;
                    pxResourceCreateInfoSub[1].Model.Form = PXModelFormRectangle;

                    // Add hibox if needed
                    if(pxSpriteCreateEventData->HitBoxCreate)
                    {
                        pxResourceCreateInfoSub[2].Type = PXResourceTypeHitBox;
                        pxResourceCreateInfoSub[2].ObjectReference = (void**)&pxSprite->HitBox;
                        pxResourceCreateInfoSub[2].HitBox.Flags = 0;
                        pxResourceCreateInfoSub[2].HitBox.Model = pxSprite->Model;

                       // pxResourceCreateInfo->HitBox.HitBox = pxSprite->HitBox;

                        amount++;
                    }

                    PXResourceManagerAdd(pxResourceManager, &pxResourceCreateInfoSub, amount);
                }

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

                PXResourceLoadInfo pxResourceLoadInfo;
                PXClear(PXResourceLoadInfo, &pxResourceLoadInfo);
                pxResourceLoadInfo.Target = pxSound;
                pxResourceLoadInfo.Type = PXResourceTypeSound;

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

              //  pxEngineSound->Info.ID = PXResourceManagerGenerateUniqeID(pxResourceManager);
               // PXDictionaryAdd(&pxResourceManager->SoundLookUp, &pxEngineSound->Info.ID, pxSound);

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
                    PXFileBufferExternal(&pxShaderProgramCreateData->ShaderVertexFile, pxShaderProgramCreateData->ShaderVertexText, pxShaderProgramCreateData->ShaderVertexTextSize);
                    PXFileBufferExternal(&pxShaderProgramCreateData->ShaderPixelFile, pxShaderProgramCreateData->ShaderPixelText, pxShaderProgramCreateData->ShaderPixelTextSize);

                    pxShaderProgram->Info.Flags |= PXEngineResourceInfoStorageMemory;
                }
                else // load file
                {
                    PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
                    PXClear(PXFileOpenFromPathInfo, &pxFileOpenFromPathInfo);
                    pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
                    pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
                    pxFileOpenFromPathInfo.AllowMapping = PXTrue;

                    // TODO: we can make it a loop?

                    PXTextConstructFromAdressA(&pxFileOpenFromPathInfo.Text, pxShaderProgramCreateData->ShaderVertexFilePath, PXTextLengthUnkown, PXTextLengthUnkown);

                    const PXActionResult vertexLoadResult = PXFileOpenFromPath(&pxShaderProgramCreateData->ShaderVertexFile, &pxFileOpenFromPathInfo);

                    PXActionReturnOnError(vertexLoadResult);

                    PXTextConstructFromAdressA(&pxFileOpenFromPathInfo.Text, pxShaderProgramCreateData->ShaderPixelFilePath, PXTextLengthUnkown, PXTextLengthUnkown);

                    const PXActionResult fragmentLoadResult = PXFileOpenFromPath(&pxShaderProgramCreateData->ShaderPixelFile, &pxFileOpenFromPathInfo);

                    PXActionReturnOnError(fragmentLoadResult);

                    pxShaderProgram->Info.Flags |= PXEngineResourceInfoStorageMemory;
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

                pxHitBox->Info.Flags |= PXEngineResourceInfoEnabled;
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

                pxGUIElement->Info.Flags |= PXEngineResourceInfoEnabled;

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
        return PXNull;
    }

    //for (PXSize materialContainerID = 0; materialContainerID < pxMaterialContainer->MaterialContaierListSize; ++materialContainerID)
    //{
       // const PXMaterialContainer* const pxMaterialContaier = &pxModel->MaterialContaierList[materialContainerID];

        for (PXSize materialID = 0; materialID < pxMaterialContainer->MaterialListAmount; ++materialID)
        {
            PXMaterial* const pxMaterial = &pxMaterialContainer->MaterialList[materialID];

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
    PXVertexBufferFormat newFormat = PXVertexBufferFormatInvalid;

    switch(pxModel->Mesh.VertexBuffer.Format)
    {
        case PXVertexBufferFormatXYI8:
        {
            float* newVertexArray = 0;
            PXSize newVertexArraySize = 0;
            PXSize amountFuture = PXVertexBufferFormatStrideSize(PXVertexBufferFormatXYFloat);
            PXSize amountCurrent = PXVertexBufferFormatStrideSize(PXVertexBufferFormatXYI8);
            PXSize sizeCurrent = pxModel->Mesh.VertexBuffer.VertexDataSize / 1;

            PXNewList(float, sizeCurrent, &newVertexArray, &newVertexArraySize);

            PXInt8S* dataSource = (PXInt8S*)pxModel->Mesh.VertexBuffer.VertexData;

            for(size_t i = 0; i < sizeCurrent; i++)
            {
                newVertexArray[i] = dataSource[i];
            }

            // Memory leak?

            pxModel->Mesh.VertexBuffer.Format = PXVertexBufferFormatXYFloat;
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

    newFormat = pxModel->Mesh.VertexBuffer.Format;

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

void PXAPI PXUIElementPositionCalculcate(PXGUIElement* const pxGUIElement, PXUIElementPositionCalulcateInfo* const pxUIElementPositionCalulcateInfo)
{
    for(PXGUIElement* pxUIElementParent = pxGUIElement->Parent; pxUIElementParent; pxUIElementParent = pxUIElementParent->Parent)
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


    if(pxGUIElement->Position.FlagListKeep & PXUIElementKeepWidth)
    {
        pxUIElementPositionCalulcateInfo->Width = pxGUIElement->Position.Width;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Width = mathWithScaling;
    }

    if(pxGUIElement->Position.FlagListKeep & PXUIElementKeepHeight)
    {
        pxUIElementPositionCalulcateInfo->Height = pxGUIElement->Position.Height;
    }
    else
    {
        pxUIElementPositionCalulcateInfo->Height = heightWithScaling;
    }

 

    switch(pxGUIElement->Position.FlagListKeep & PXUIElementAllignFlags)
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
            pxFileTypeInfo->ResourceType = PXResourceTypeImage;
            pxFileTypeInfo->ResourceLoad = PXJavaLoadFromFile;
            pxFileTypeInfo->ResourceSave = PXJavaSaveToFile;
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

PXActionResult PXAPI PXResourceLoad(PXResourceLoadInfo* const pxResourceLoadInfo, const PXText* const filePath)
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

            return PXActionRefusedNotSupportedByLibrary;
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
        if(PXFileFormatUnkown == pxFile.TypeInfo.FormatExpected)
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

        if(PXNull == pxFile.TypeInfo.ResourceSave)
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
        PXInt32U time = PXProcessorTimeReal();
#endif

        pxResourceSaveInfo->FileReference = &pxFile;  

        const PXActionResult fileParsingResult = pxFile.TypeInfo.ResourceSave(pxResourceSaveInfo);

#if PXLogEnable
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
#endif

        // PXActionReturnOnSuccess(fileParsingResult); // Exit if this has worked first-try 

        // return fileParsingResult; // TEMP-FIX: if the file extension is wrong, how can we still load?

    }

    PXFileClose(&pxFile);

    pxResourceSaveInfo->FileReference = PXNull;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXResourceSaveA(PXResourceSaveInfo* const pxResourceSaveInfo, const char* const filePath)
{
    PXText pxText;

    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown, PXTextUnkownLength);

    const PXActionResult loadResult = PXResourceSave(pxResourceSaveInfo, &pxText);

    return loadResult;
}