#include "PXGraphic.h"

#if PXGraphicUSE

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Window/PXWindow.h>
#include <Media/ADLER/PXAdler32.h>

#include <OS/Graphic/OpenGL/PXOpenGL.h>
#include <OS/Graphic/DirectX/PXDirectX.h>
#include <OS/Graphic/Vulcan/PXVulcan.h>

//#include <gl/GL.h> // Not found???
#include <GL/gl.h>


#include <Math/PXMath.h>
#include <OS/Window/PXWindow.h>

PXActionResult PXAPI PXGraphicLoadImage(PXGraphic* const pxGraphic, PXImage* const pxImage, const PXText* const pxImageFilePath)
{
    // Create checksum
    const PXInt32U checkSum = PXAdler32Create(1, pxImageFilePath->TextA, pxImageFilePath->SizeUsed);


    // Check if already loaded
    {
        PXImage* pxImageFoundEntry = PXNull;

        const PXBool foundEntry = PXDictionaryFindEntry(&pxGraphic->ResourceImageLookUp, &checkSum, &pxImageFoundEntry);

        if (foundEntry) // image is already loaded
        {
            PXImageCopyAsIs(pxImage, pxImageFoundEntry);

            printf("[PXGraphic] Texture load <%s>... [REDUNDANT]\n", pxImageFilePath->TextA);

            return PXActionSuccessful;
        }
    }

    // Load texture
    {
        printf("[PXGraphic] Texture load <%s>... ", pxImageFilePath->TextA);

        const PXActionResult loadResult = PXResourceLoad(pxImage, pxImageFilePath);

        if (PXActionSuccessful != loadResult)
        {
            printf("[FAILED]\n");
        }

        PXActionReturnOnError(loadResult);

        printf("[OK]\n");

        PXDictionaryAdd(&pxGraphic->ResourceImageLookUp, &checkSum, pxImage);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicTextureScreenShot(PXGraphic* const pxGraphic, PXImage* const image)
{
  //  PXOpenGLPixelDataRead(pxGraphic, 0, 0, image->Width, image->Height, PXOpenGLImageFormatRGB, PXOpenGLTypeByteUnsigned, image->PixelData);

	return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicTexture2DLoad(PXGraphic* const pxGraphic, PXTexture2D* const texture, const PXText* filePath)
{
    if (!pxGraphic || !texture || !filePath)
    {
        return PXActionRefuedParameterNull;
    }

    texture->Filter = PXGraphicRenderFilterNoFilter;
    texture->LayoutNear = PXGraphicImageLayoutNearest;
    texture->LayoutFar = PXGraphicImageLayoutNearest;
    texture->WrapHeight = PXGraphicImageWrapRepeat;
    texture->WrapWidth = PXGraphicImageWrapRepeat;

    // Load texture..
    {
        const PXActionResult registerResult = PXGraphicLoadImage(pxGraphic, &texture->Image, filePath);

        PXActionReturnOnError(registerResult);
    }

    // Register as normal
    {
        const PXActionResult registerResult = pxGraphic->Texture2DRegister(pxGraphic, texture);

        PXActionReturnOnError(registerResult);
    }



    /*

      if(loadAsynchronously)
    {
        PXThreadRun(0, LoadResourceAsync, &image);

        return PXActionSuccessful;
    }
    else
    {
        const PXActionResult imageLoadResult = ImageLoadW(image, filePath);
        const bool isSucessful = imageLoadResult == PXActionSuccessful;

        if(isSucessful)
        {


            PXGraphicTextureRegister(&_mainWindow.GraphicInstance, &texture);
        }

        return imageLoadResult;
    }

    */


    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicTexture2DLoadA(PXGraphic* const pxGraphic, PXTexture2D* const texture, const char* const filePath)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown);

    return PXGraphicTexture2DLoad(pxGraphic, texture, &pxText);
}

PXActionResult PXAPI PXGraphicFontLoad(PXGraphic* const pxGraphic, PXFont* const pxFont, const PXText* const filePath)
{
    // Load texture
    {
        const PXActionResult loadResult = PXResourceLoad(pxFont, filePath);

        PXActionReturnOnError(loadResult);
    }

    // Register as normal
    {
        const PXActionResult registerResult = PXGraphicFontRegister(pxGraphic, pxFont);

        PXActionReturnOnError(registerResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicFontRegister(PXGraphic* const pxGraphic, PXFont* const pxFont)
{
    PXLockEngage(&pxGraphic->_resourceLock);
    //pxFont->ID = PXGraphicGenerateUniqeID(pxGraphic);
    //PXDictionaryAdd(&pxGraphic->FontLookUp, &pxFont->ID, pxFont);
    PXLockRelease(&pxGraphic->_resourceLock);

    pxGraphic->Texture2DRegister(pxGraphic, &pxFont->MainPage.Texture);

    for (PXSize i = 0; i < pxFont->AdditionalPageListSize; ++i)
    {
        PXFontPage* const pxFontPage = &pxFont->AdditionalPageList[i];

        pxGraphic->Texture2DRegister(pxGraphic, &pxFontPage->Texture);
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicFontRelease(PXGraphic* const pxGraphic, PXFont* const pxFont)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicFontUse(PXGraphic* const pxGraphic, PXFont* const pxFont)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicSpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y)
{
    PXVector2FSetXY(&pxSprite->TextureScaleOffset, x, y);
}

PXActionResult PXAPI PXGraphicSpriteRegister(PXGraphic* const pxGraphic, PXSprite* const pxSprite)
{
    pxSprite->PXID = PXGraphicGenerateUniqeID(pxGraphic);

    PXDictionaryAdd(&pxGraphic->SpritelLookUp, &pxSprite->PXID, pxSprite);

    const PXBool hasScaling = pxSprite->TextureScaleOffset.X != 1 || pxSprite->TextureScaleOffset.Y != 1;

    if (hasScaling)
    {
        if (pxGraphic->SpriteScaled.ResourceID.OpenGLID == 0)
        {
            PXOpenGLSpriteRegister(&pxGraphic->OpenGLInstance, pxSprite);
        }
        else
        {
            pxSprite->VertexStructure.StructureOverride = &pxGraphic->SpriteScaled;
        }
    }
    else
    {
        if (pxGraphic->SpriteUnScaled.ResourceID.OpenGLID == 0)
        {
            PXOpenGLSpriteRegister(&pxGraphic->OpenGLInstance, pxSprite);
        }
        else
        {
            pxSprite->VertexStructure.StructureOverride = &pxGraphic->SpriteUnScaled;
        }
    }

    return PXActionSuccessful;
}

#define QuadSkybox 0

PXActionResult PXAPI PXGraphicSkyboxRegister(PXGraphic* const pxGraphic, PXSkyBox* const skyBox)
{
    const float vertexData[] =
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

    const PXInt8U indexList[] =
    {
        #if QuadSkybox
        0,1,2,3, // Left OK
        4,5,6,7, // Right  OK
        4,5,1,0, // Top OK
        2,3,7,6, // Bot OK
        0,2,6,4, // Front OK
        5,7,3,1 // Back OK
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


    PXObjectClear(PXVertexStructure, &skyBox->VertexStructure);
    skyBox->VertexStructure.VertexBuffer.VertexData = vertexData;
    skyBox->VertexStructure.VertexBuffer.VertexDataSize = sizeof(vertexData);
    skyBox->VertexStructure.VertexBuffer.VertexDataRowSize = 3;
    skyBox->VertexStructure.VertexBuffer.Format = PXVertexBufferFormatXYZ; // PXVertexBufferFormatXYZC  PXVertexBufferFormatXYZHWC

    skyBox->VertexStructure.IndexBuffer.DataType = PXDataTypeInt8U;
    skyBox->VertexStructure.IndexBuffer.IndexData = indexList;
    skyBox->VertexStructure.IndexBuffer.IndexDataSize = sizeof(indexList);
    skyBox->VertexStructure.IndexBuffer.IndexDataAmount = sizeof(indexList) / sizeof(PXInt8U);
    skyBox->VertexStructure.IndexBuffer.DrawModeID = PXDrawModeIDPoint | PXDrawModeIDLineLoop |
#if QuadSkybox
        PXDrawModeIDSquare
#else
        PXDrawModeIDTriangle
#endif
        ;

    pxGraphic->VertexStructureRegister(pxGraphic, &skyBox->VertexStructure);

    //OLD
    /*

    {
        const PXSize bufferSize = sizeof(vertexData) + sizeof(indexList);
        PXByte dataBuffer[192];

        PXModel pxModel;
        PXModelConstruct(&pxModel);

        // Copy
        {
            PXSize offset = 0;

            offset += PXMemoryCopy(vertexData + offset, sizeof(vertexData), dataBuffer + offset, 192);
            offset += PXMemoryCopy(vertexData + offset, sizeof(indexList), dataBuffer + offset, 192 - offset);
        }

        pxModel.Data = dataBuffer;
        pxModel.DataSize = bufferSize;

        pxModel.DataVertexList = dataBuffer;
        pxModel.DataVertexWidth = 3;
        pxModel.DataVertexSize = vertexDataSize;

        pxModel.DataIndexList = dataBuffer + sizeof(vertexData);
        pxModel.DataIndexWidth = 4u;
        pxModel.DataIndexSize = indexListSize;

        PXGraphicModelRegisterFromModel(pxGraphic, &skyBox->Renderable, &pxModel);
    }*/

    // Register Cube Texture
    pxGraphic->TextureCubeRegister(pxGraphic, &skyBox->TextureCube);

    pxGraphic->_currentSkyBox = skyBox;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicSkyboxRegisterD
(
    PXGraphic* const pxGraphic,
    PXSkyBox* const skyBox,
    const PXText* const shaderVertex,
    const PXText* const shaderFragment,
    const PXText* const textureRight,
    const PXText* const textureLeft,
    const PXText* const textureTop,
    const PXText* const textureBottom,
    const PXText* const textureBack,
    const PXText* const textureFront
)
{
    // Load Textures
    {
        const PXText** const filePathList[6] = { textureRight, textureLeft, textureTop, textureBottom, textureBack, textureFront };
        PXActionResult resultList[6];

        for (PXSize i = 0; i < 6u; ++i)
        {
            PXImage* const image = &skyBox->TextureCube.ImageList[i];
            const PXText* const filePath = filePathList[i];
            const PXActionResult textureRightResult = PXGraphicLoadImage(pxGraphic, image, filePath);

            resultList[i] = textureRightResult;
        }
    }

    // Register skybox
    {
        const PXActionResult registerResult = PXGraphicSkyboxRegister(pxGraphic, skyBox);

    }

    // Register Shaders
    {
        PXShaderProgram* shaderPXProgram = malloc(sizeof(PXShaderProgram));

        const PXActionResult shaderResult = pxGraphic->ShaderProgramCreateFromFileVP(pxGraphic, shaderPXProgram, shaderVertex, shaderFragment);

        skyBox->ShaderProgramReference = shaderPXProgram;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicSkyboxRegisterA
(
    PXGraphic* const pxGraphic,
    PXSkyBox* const skyBox,
    const char* const shaderVertex,
    const char* const shaderFragment,
    const char* const textureRight,
    const char* const textureLeft,
    const char* const textureTop,
    const char* const textureBottom,
    const char* const textureBack,
    const char* const textureFront
)
{
    PXText pxTextShaderVertex;
    PXText pxTextShaderFragment;
    PXText pxTextTextureRight;
    PXText pxTextTextureLeft;
    PXText pxTextTextureTop;
    PXText pxTextTextureBottom;
    PXText pxTextTextureBack;
    PXText pxTextTextureFront;

    PXTextConstructFromAdressA(&pxTextShaderVertex, shaderVertex, PXTextLengthUnkown);
    PXTextConstructFromAdressA(&pxTextShaderFragment, shaderFragment, PXTextLengthUnkown);
    PXTextConstructFromAdressA(&pxTextTextureRight, textureRight, PXTextLengthUnkown);
    PXTextConstructFromAdressA(&pxTextTextureLeft, textureLeft, PXTextLengthUnkown);
    PXTextConstructFromAdressA(&pxTextTextureTop, textureTop, PXTextLengthUnkown);
    PXTextConstructFromAdressA(&pxTextTextureBottom, textureBottom, PXTextLengthUnkown);
    PXTextConstructFromAdressA(&pxTextTextureBack, textureBack, PXTextLengthUnkown);
    PXTextConstructFromAdressA(&pxTextTextureFront, textureFront, PXTextLengthUnkown);

    return PXGraphicSkyboxRegisterD
    (
        pxGraphic,
        skyBox,
        &pxTextShaderVertex,
        &pxTextShaderFragment,
        &pxTextTextureRight,
        &pxTextTextureLeft,
        &pxTextTextureTop,
        &pxTextTextureBottom,
        &pxTextTextureBack,
        &pxTextTextureFront
    );
}

PXActionResult PXAPI PXGraphicSkyboxUse(PXGraphic* const pxGraphic, PXSkyBox* const skyBox)
{
    /*
    // TODO:TEST REMOVAL !!!    PXOpenGL::VertexArrayBind(skyBox.RenderInfo.VAO);

    PXOpenGLID skyBoxTextureLocation = PXOpenGL::ShaderGetUniformLocationID(skyBox.Shader.ID, "SkyBoxTexture");

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_CUBE_MAP);

    glUniform1i(skyBoxTextureLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox.Texture.ID);
    // glBindTexture(GL_TEXTURE_2D, skyBox.Texture.ID);
    */

    return PXActionInvalid;
}

PXSize PXAPI PXGraphicRenderableListSize(const PXGraphic* const pxGraphic)
{
    return pxGraphic->_renderList.NodeListSizeCurrent;
}

PXBool PXAPI PXGraphicRenderableListGetFromIndex(const PXGraphic* const pxGraphic, PXRenderable** pxRenderable, const PXSize index)
{
#if 1 //
    PXLinkedListNodeFixed currentModel;

    const PXBool successful = PXLinkedListFixedNodeAt(&pxGraphic->_renderList, &currentModel, index);

    if (!successful)
    {
        return PXNo;
    }

    {
        PXRenderable* const renderableCurrent = (PXRenderable*)currentModel.BlockData;

        *pxRenderable = renderableCurrent;

        return PXYes;
    }

#else

    PXLinkedListNodeFixed currentModel;

    PXLinkedListFixedNodeAt(&pxGraphic->_renderList, &currentModel, 0);

    *pxRenderable = 0;

    PXSize counter = 0;

    do
    {
        const PXRenderable* const renderableCurrent = currentModel.BlockData;

        if (index == counter++)
        {
            *pxRenderable = renderableCurrent;
            return PXTrue;
        }

    }
    while (PXLinkedListFixedNodeNext(&pxGraphic->_renderList, &currentModel));

    return PXNo;
#endif
}

PXActionResult PXAPI PXGraphicRenderableCreate(PXGraphic* const pxGraphic, PXRenderable** const pxRenderable)
{
    PXRenderable* const renderable = PXMemoryAllocateType(PXRenderable, 1u);

    if (!renderable)
    {
        return PXActionFailedMemoryAllocation;
    }

    PXMemoryClear(renderable, sizeof(PXRenderable));

    PXGraphicRenderableRegister(pxGraphic, *pxRenderable);

    *pxRenderable = renderable; // TODO: Is this right?

    return PXActionSuccessful;
}

PXBool PXAPI PXGraphicRenderableRegister(PXGraphic* const pxGraphic, PXRenderable* const pxRenderable)
{
    PXLockEngage(&pxGraphic->_resourceLock);
    PXLinkedListFixedNodeAdd(&pxGraphic->_renderList, pxRenderable);
    PXLockRelease(&pxGraphic->_resourceLock);

    return PXTrue;
}

void PXAPI PXGraphicModelShaderSet(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = shaderPXProgram->ResourceID.OpenGLID;
    }
}

/*
PXActionResult PXGraphicModelLoad(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXText* const filePath)
{
    PXModel* model = 0;

    // Create model
    {
        const PXActionResult createResult = PXGraphicModelCreate(pxGraphic, &model);

        PXActionReturnOnError(createResult);
    }

    // create PXRenderable
    {
        PXLockEngage(&pxGraphic->_resourceLock);
        PXLinkedListFixedNodeAdd(&pxGraphic->_renderList, renderable);
        PXLockRelease(&pxGraphic->_resourceLock);

        /*

        if (renderable) // Owner is caller
        {
            pxRenderable = *renderable;


        }
        else // No model, make one
        {
            const PXActionResult createResult = PXGraphicRenderableCreate(pxGraphic, &pxRenderable);

            PXActionExitOnError(createResult);

            *renderable = pxRenderable; // Write to return back to caller
        }
        * /
    }

    // Load model
    {
        const PXActionResult loadResult = PXModelLoad(model, filePath);

        PXActionReturnOnError(loadResult);
    }

    // Register model into renderable
    {
        const PXActionResult loadResult = PXGraphicModelRegisterFromModel(pxGraphic, renderable, model);

        PXActionReturnOnError(loadResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicModelRegisterFromModel(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXModel* const model)
{
#if PXOpenGLUSE
    PXOpenGL* const openGLContext = &pxGraphic->OpenGLInstance;

    PXRenderableConstruct(renderable);

    PXOpenGLVertexArrayGenerate(openGLContext, 1u, &renderable->VAO);

    PXInt32U vbo = 0;

    // Create VBO Buffers
    PXOpenGLBufferGenerate(openGLContext, 1u, &vbo);
    renderable->VBO = vbo;



    PXOpenGLVertexArrayBind(openGLContext, renderable->VAO);

    PXOpenGLBufferBind(openGLContext, PXOpenGLBufferArray, renderable->VBO);
    PXOpenGLBufferData(openGLContext, PXOpenGLBufferArray, model->DataSize, model->Data, PXOpenGLStoreStaticDraw);

#if 0
    printf
    (
        "| %7s %7s %7s | %7s %7s %7s | %7s %7s |\n",
        "X",
        "Y",
        "Z",
        "Nx",
        "Ny",
        "Nz",
        "Tx",
        "Ty"
    );

    for (PXSize i = 0; i < model->DataIndexSize * stride; i+=8)
    {
        float* data = &(((float*)model->DataVertex)[i]);

        printf
        (
            "| %7.2f %7.2f %7.2f | %7.2f %7.2f %7.2f | %7.2f %7.2f |\n",
            data[0],
            data[1],
            data[2],

            data[3],
            data[4],
            data[5],

            data[6],
            data[7]
        );
    }
#endif

    // TODO
    unsigned int index = 0;
    unsigned int offset = 0;

    if (model->DataVertexWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataVertexWidth, PXDataTypeFloat, 0, model->DataVertexWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataVertexWidth * sizeof(float);
    }

    if (model->DataNormalWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataNormalWidth, PXDataTypeFloat, 0, model->DataNormalWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataNormalWidth * sizeof(float);
    }

    if (model->DataTextureWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataTextureWidth, PXDataTypeFloat, 0, model->DataTextureWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataTextureWidth * sizeof(float);
    }

    // Color?

    PXOpenGLBufferUnbind(openGLContext, PXOpenGLBufferArray);

    if (model->DataIndexWidth)
    {
        PXOpenGLBufferGenerate(openGLContext, 1u, &renderable->IBO);

        PXOpenGLBufferBind(openGLContext, PXOpenGLBufferElementArray, renderable->IBO);

        PXOpenGLBufferData(openGLContext, PXOpenGLBufferElementArray, sizeof(unsigned int) * model->DataIndexSize, model->DataIndexList, PXOpenGLStoreStaticDraw);

        PXOpenGLBufferUnbind(openGLContext, PXOpenGLBufferElementArray);
    }

    PXOpenGLVertexArrayUnbind(openGLContext);
    //-------------------------------------------------------------------------

    //---<Register all textures>-----------------------------------------------
    const PXSize segmentsListSize = 1;// PXModelSegmentsAmount(model);
    const PXSize modelListSize = 1;// PXModelMaterialAmount(model);

    renderable->MeshSegmentListSize = segmentsListSize;
    renderable->MeshSegmentList = PXMemoryAllocateType(PXRenderableMeshSegment, segmentsListSize);

    for (PXSize i = 0; i < segmentsListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];
        MeshSegment meshSegment;

        PXRenderableMeshSegmentConstruct(pxRenderableMeshSegment);
        PXModelSegmentsGet(model, i, &meshSegment);

        pxRenderableMeshSegment->RenderMode = PXGraphicDrawModeTriangle;
        pxRenderableMeshSegment->NumberOfVertices = meshSegment.DrawClusterSize;

        PXMaterial material;
        const PXBool fetchMaterialSuccess = PXModelMaterialGet(model, meshSegment.TextureID, &material);

        if (fetchMaterialSuccess)
        {
            PXTexture2D pxTexture;

            PXTextureConstruct(&pxTexture);

            pxTexture.Filter = PXGraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
            pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
            pxTexture.WrapHeight = PXGraphicImageWrapRepeat;
            pxTexture.WrapWidth = PXGraphicImageWrapRepeat;

            //PXGraphicTextureLoad(pxGraphic, &pxTexture, &material.DiffuseTextureFilePath);

            pxRenderableMeshSegment->TextureID = pxTexture.ResourceID.OpenGLID;
        }

        pxRenderableMeshSegment->DoRendering = PXYes;
    }
    //-------------------------------------------------------------------------

    // Model is not fully registered and ready to be rendered
    renderable->DoRendering = 1u;
#endif

    return PXActionSuccessful;
}*/


PXActionResult PXAPI PXGraphicUIRectangleCreate(PXGraphic* const pxGraphic, PXRenderable* const renderable, const PXSize x, const PXSize y, const PXSize sidth, const PXSize height)
{
    PXMatrix4x4FMoveXY(&renderable->MatrixModel, x, y, &renderable->MatrixModel);
   // PXMatrix4x4FScaleXYZSet(&renderable->MatrixModel, sidth, height, 1);

    // Register rectangle
    {
        //float

        //GraphicModelRegisterFromData(pxGraphic, renderable, , );

    }

    return PXActionSuccessful;
}

void PXAPI PXShaderConstruct(PXShader* const shader)
{
    PXMemoryClear(shader, sizeof(PXShader));

    shader->ResourceID.PXID = PXShaderNotRegisterd;
}

void PXAPI PXShaderDataSet(PXShader* const shader, const PXGraphicShaderType type, const char* data, const PXSize size)
{
    shader->ResourceID.PXID = PXShaderNotRegisterd;
    shader->Type = type;
    shader->ContentSize = size;
    shader->Content = data;
}

void PXAPI PXTextureConstruct(PXTexture2D* const texture)
{
    PXMemoryClear(texture, sizeof(PXTexture2D));
}

void PXAPI PXTextureDestruct(PXTexture2D* const texture)
{

}

PXActionResult PXAPI PXGraphicUIElementCreate(PXGraphic* const pxGraphic, PXUIElement** const pxUIElement, const PXSize amount, PXUIElement* const pxUIElementParrent)
{
    //PXClear(PXUIElement, *pxUIElement);

    (*pxUIElement) = PXNew(PXUIElement);

    (*pxUIElement)->ID = ++(pxGraphic->UIElementIDCounter);   


    //*pxUIElement->Type = pxUIElementType;
    //*pxUIElement->TextureReference = PXNull;
    //*pxUIElement->ShaderReference = PXNull;

    //*pxUIElement->NameTextScale = 1;

    PXUIElementColorSet4F(*pxUIElement, 1, 1, 1, 1);

   // PXTextCopyA("[N/A]", 5, pxUIElement->Name, 32);

    if (pxUIElementParrent)
    {
        // Impossible if already has child

        (*pxUIElement)->Parent = pxUIElementParrent; // "It's alway my parent"

        const PXBool alreadyHasAFirstBorn = pxUIElementParrent->Child != PXNull;

        if (alreadyHasAFirstBorn)
        {
            // Add as sibling for firstborn
            PXUIElement* targetSibling = (*pxUIElement)->Parent->Child;

            // Search for last sibling to be
            while (targetSibling->Sibling != PXNull)
            {
                targetSibling = targetSibling->Sibling;
            }

            targetSibling->Sibling = (*pxUIElement); // Add sibling
        }
        else
        {
            // Add as child
            pxUIElementParrent->Child = (*pxUIElement);
        }    
    }
    else
    { 
        if (pxGraphic->UIElementBase.Child) // if Has child, everyone will be sibling from child
        {
            PXUIElement* insertionNode = pxGraphic->UIElementBase.Child;

            while (insertionNode->Sibling != PXNull)
            {
                insertionNode = insertionNode->Sibling;
            }

            insertionNode->Sibling = (*pxUIElement); // I am your sibbling
        }
        else
        {
            (*pxUIElement)->Parent = &pxGraphic->UIElementBase; // You are my parent now
            pxGraphic->UIElementBase.Child = (*pxUIElement); // As I am your child
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicUIElementDelete(PXGraphic* const pxGraphic, PXUIElement** const pxUIElement)
{
    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicUIElementTypeSet(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement, const PXUIElementType pxUIElementType)
{
    pxUIElement->Type = pxUIElementType;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicUIElementPrintfNode(PXUIElement* const pxUIElement, int depth, void* sender, PXGraphicUIElementTrigger preFound, PXGraphicUIElementTrigger postFound)
{
    char* nothing = "---";
    char* offset = "    ";

    PXFunctionInvoke(preFound, sender, pxUIElement);
#if 0
    for (size_t i = 0; i < depth; i++) printf(offset);
    printf("+--------------------+\n");

    for (size_t i = 0; i < depth; i++) printf(offset);

    printf("| Node: <%i> |\n", pxUIElement->ID);  

    for (size_t i = 0; i < depth; i++) printf(offset);

    if (pxUIElement->Parent)
    {
        printf("| %-10s : <%i>\n", "Parent", pxUIElement->Parent->ID);
    }
    else
    {
        printf("| %-10s : %s\n", "Parent", nothing);
    }  

    for (size_t i = 0; i < depth; i++) printf(offset);
#endif

    if (pxUIElement->Child)
    {
#if 0
        printf("| %-10s : <%i>\n", "Child", pxUIElement->Child->ID);
#endif
        PXGraphicUIElementPrintfNode(pxUIElement->Child, depth+1, sender, preFound, postFound);
    }
#if 0
    else
    {
        printf("| %-10s : %s\n", "Child", nothing);
    }

    for (size_t i = 0; i < depth; i++) printf(offset);
#endif

    if (pxUIElement->Sibling)
    {
#if 0
        printf("| %-10s : %s\n", "Siblings", "-->");
        for (size_t i = 0; i < depth; i++) printf(offset);
        printf("+--------------------+\n");
#endif

        PXGraphicUIElementPrintfNode(pxUIElement->Sibling, depth, sender, preFound, postFound);
    }
#if 0
    else
    {
        printf("| %-10s : %s\n", "Siblings", nothing);

       for (size_t i = 0; i < depth; i++) printf(offset);
        printf("+--------------------+\n");
    }
#endif

    PXFunctionInvoke(postFound, sender, pxUIElement);
}

PXActionResult PXAPI PXGraphicUIElementIterator(PXGraphic* const pxGraphic, void* sender, PXGraphicUIElementTrigger preFound, PXGraphicUIElementTrigger postFound)
{
    PXGraphicUIElementPrintfNode(&pxGraphic->UIElementBase, 0, sender, preFound, postFound);
}

PXActionResult PXAPI PXGraphicUIElementPrint(PXGraphic* const pxGraphic)
{
   // printf("+---------------------------------------------------------+\n");

    PXGraphicUIElementIterator(pxGraphic, 0, 0, 0);

   // printf("+---------------------------------------------------------+\n");
}

void PXAPI PXRenderableConstruct(PXRenderable* const pxRenderable)
{
    PXMemoryClear(pxRenderable, sizeof(PXRenderable));

    PXMatrix4x4FIdentity(&pxRenderable->MatrixModel);

    pxRenderable->VAO = -1;
    pxRenderable->VBO = -1;
    pxRenderable->IBO = -1;
}

void PXAPI PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha)
{
    PXColorRGBAF* color = PXNew(PXColorRGBAF);

    color->Red = red;
    color->Green = green;
    color->Blue = blue;
    color->Alpha = alpha;

    pxUIElement->ColorTintReference = color;
}

void PXAPI PXUIElementSizeSet(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height, const PXUIElementPositionMode pxUIElementPositionMode)
{
    //pxUIElement->X = x;
    //pxUIElement->Y = y;
    // pxUIElement->Width = width;
   // pxUIElement->Height = height;

    PXRectangleOffsetSet(&pxUIElement->Margin, x, y, width, height);

   // pxUIElement->PositionMode = pxUIElementPositionMode;
}

void PXAPI PXGraphicPXUIElementTextSet(PXUIElement* const pxUIElement, PXText* const pxText)
{
   // PXText nameBuffer;
   // PXTextConstructFromAdressA(&nameBuffer, pxUIElement->Name, 32);

   // PXTextCopy(pxText, &nameBuffer);
}

void PXAPI PXGraphicPXUIElementTextSetA(PXUIElement* const pxUIElement, const char* const text)
{
    PXTextCopyA(text, -1, pxUIElement->TextInfo.Content, 32);

    pxUIElement->TextInfo.Scale = 0.5;
}

void PXAPI PXGraphicPXUIElementTextSetAV(PXUIElement* const pxUIElement, const char* const format, ...)
{
    va_list args;
    va_start(args, format);

    PXTextPrint(pxUIElement, format, args);

    va_end(args);
}

void PXAPI PXGraphicPXUIElementFontSet(PXUIElement* const pxUIElement, const PXFont* const pxFont)
{
    //pxUIElement->FontID = pxFont;
}

void PXAPI PXGraphicUIElementFlagSet(PXUIElement* const pxUIElement, const PXInt32U flagList)
{
    pxUIElement->FlagsList = flagList;
}

void PXAPI PXGraphicPXUIElementParentSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent)
{
    pxUIElement->Parent = pxUIElementParent;

    PXGraphicPXUIElementChildSet(pxUIElementParent, pxUIElement);
}

void PXAPI PXGraphicPXUIElementChildSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent)
{
    pxUIElement->Child = pxUIElementParent;
}

PXInt32U PXAPI PXGraphicGenerateUniqeID(PXGraphic* const pxGraphic)
{
    return ++pxGraphic->UniqeIDGeneratorCounter;
}

PXActionResult PXAPI PXGraphicUIElementRegister(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement)
{
    // Is Registerd
    {
        const PXBool isRegisterd = pxUIElement->ID != -1;

        if (!isRegisterd)
        {
            return PXActionInvalidRedundandInteraction;
        }

        // Create
        pxUIElement->ID = PXGraphicGenerateUniqeID(pxGraphic);

        PXDictionaryAdd(&pxGraphic->UIElementLookUp, &pxUIElement->ID, pxUIElement);
    }



    switch (pxUIElement->Type)
    {
        case PXUIElementTypeRenderFrame:
        {
            PXOpenGL* const pxOpenGL = &pxGraphic->OpenGLInstance;

#if 0
            // Texture
           // PXTexture2DSet(pxUIElement->FrameRenderTextureReference, PXColorFormatRGBI8, pxUIElement->FrameRenderWidth, pxUIElement->Height);
            PXOpenGLTexture2DCreate(pxOpenGL, pxUIElement->FrameRenderTextureReference);


            // Framebuffer
            PXOpenGLFrameBufferCreate(pxOpenGL, 1, &pxUIElement->FrameBufferID);
            PXOpenGLFrameBufferBind(pxOpenGL, PXOpenGLFrameBufferModeDrawAndRead, pxUIElement->FrameBufferID);

            // Renderbuffer as depthbuffer
            PXOpenGLRenderBufferCreate(pxOpenGL, 1, &pxUIElement->FrameRenderID);
            PXOpenGLRenderBufferBind(pxOpenGL, pxUIElement->FrameRenderID);
            PXOpenGLRenderBufferStorage(pxOpenGL, PXOpenGLRenderBufferFormatDepthComponent, pxUIElement->FrameRenderWidth, pxUIElement->FrameRenderHeight);

            // Link buffer
            PXOpenGLFrameBufferLinkRenderBuffer(pxOpenGL, PXOpenGLRenderBufferAttachmentPointDepth, pxUIElement->FrameRenderID);



            PXOpenGLFrameBufferLinkTexture2D(pxOpenGL, PXOpenGLRenderBufferAttachmentPointColor, PXGraphicTextureType2D, pxUIElement->FrameRenderTextureReference->ResourceID.OpenGLID, 0);

            // Set the list of draw buffers.
           // GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
            //glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

           // // Always check that our framebuffer is ok
            //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
             //   return false;


            PXOpenGLFrameBufferBind(pxOpenGL, PXOpenGLFrameBufferModeDrawAndRead, 0);
            PXOpenGLRenderBufferBind(pxOpenGL, 0);
#endif
            break;
        }

        default:
            break;
    }




    /*
    switch (pxUIElement->Type)
    {
        case PXUIElementTypePanel:
        {
            const float vertexData[] =
            {
                 -1,  -1,  0, 0,1,
                1,  -1,  0,  1,1,
                 1, 1,  0,   1,0,
                -1, 1,  0,   0,0
            };
            const PXSize vertexDataSize = sizeof(vertexData) / sizeof(float);

            PXByte bufferData[32];
            PXModel model;

            ModelConstruct(&model);

            model.Data = bufferData;

            MemoryClear(bufferData, sizeof(bufferData));
            ModelSegmentsAdd(&model, 4u, vertexDataSize, -1);

            model.DataVertexList = vertexData;
            model.DataVertexListSize = vertexDataSize;

            model.DataVertexWidth = 3u;
            model.DataVertexSize = vertexDataSize;
            model.DataTextureWidth = 2u;
            model.DataTextureSize = vertexDataSize;

            {
                const PXActionResult actionResult = PXGraphicModelRegisterFromModel(pxGraphic, &pxUIPanel->UIElement.Renderable, &model);

                PXActionExitOnError(actionResult);
            }

            PXGraphicRenderableRegister(pxGraphic, &pxUIPanel->UIElement.Renderable);

            break;
        }
        case PXUIElementTypeLabel:
        {
            const PXSize textSize = PXTextLengthA(text, 256);
            const PXSize vertexDataSize = textSize * 4u * (3u + 2u);

            float* vertexData = MemoryAllocate(vertexDataSize * sizeof(float));

            PXSize index = 0;
            float xoffset = 0;

            float imgwidth = pxGraphicUIText->TextFont->FontElement->FontPageList[0].FontTextureMap.Width;
            float imgheight = pxGraphicUIText->TextFont->FontElement->FontPageList[0].FontTextureMap.Height;

            for (size_t i = 0; i < textSize; i++)
            {
                char character = text[i];

                PXSpriteFontCharacter* PXSpriteFontChar = PXSpriteFontGetCharacter(pxGraphicUIText->TextFont->FontElement, character);

                float texturePositionX = PXSpriteFontChar->Position[0] / imgwidth;
                float texturePositionY = PXSpriteFontChar->Position[1] / imgheight;
                float texturePositionWidth = PXSpriteFontChar->Size[0] / imgwidth;
                float texturePositionHeight = PXSpriteFontChar->Size[1] / imgheight;

                vertexData[index++] = xoffset;
                vertexData[index++] = 0;
                vertexData[index++] = 0;

                vertexData[index++] = texturePositionX;
                vertexData[index++] = texturePositionY + texturePositionHeight;

                vertexData[index++] = PXSpriteFontChar->Size[0] + xoffset;
                vertexData[index++] = 0;
                vertexData[index++] = 0;

                vertexData[index++] = texturePositionX + texturePositionWidth;
                vertexData[index++] = texturePositionY + texturePositionHeight;

                vertexData[index++] = PXSpriteFontChar->Size[0] + xoffset;
                vertexData[index++] = PXSpriteFontChar->Size[1];
                vertexData[index++] = 0;

                vertexData[index++] = texturePositionX + texturePositionWidth;
                vertexData[index++] = texturePositionY;

                vertexData[index++] = xoffset;
                vertexData[index++] = PXSpriteFontChar->Size[1];
                vertexData[index++] = 0;

                vertexData[index++] = texturePositionX;
                vertexData[index++] = texturePositionY;



                xoffset += PXSpriteFontChar->XAdvance + 10;


                //vertexData[index++] = PXSpriteFontChar->Position[0];
                //vertexData[index++] = PXSpriteFontChar->Position[1];

                //vertexData[index++] = PXSpriteFontChar->Position[0];
                //vertexData[index++] = PXSpriteFontChar->Position[1];
                //vertexData[index++] = PXSpriteFontChar->Position[0];
                //vertexData[index++] = PXSpriteFontChar->Position[1];

            }

            PXByte bufferData[32];
            PXModel model;

            ModelConstruct(&model);

            model.Data = bufferData;

            MemoryClear(bufferData, sizeof(bufferData));
            ModelSegmentsAdd(&model, 4u, vertexDataSize, -1);

            model.DataVertexList = vertexData;
            model.DataVertexListSize = vertexDataSize;

            model.DataVertexWidth = 3u;
            model.DataVertexSize = vertexDataSize;
            model.DataTextureWidth = 2u;
            model.DataTextureSize = vertexDataSize;

            {
                const PXActionResult actionResult = PXGraphicModelRegisterFromModel(pxGraphic, &pxGraphicUIText->UIElement.Renderable, &model);

                PXActionExitOnError(actionResult);
            }

            PXGraphicRenderableRegister(pxGraphic, &pxGraphicUIText->UIElement.Renderable);



            PXTexture pxTexture;

            PXTextureConstruct(&pxTexture);

            MemoryCopy(&pxGraphicUIText->TextFont->FontElement[0].FontPageList[0].FontTextureMap, sizeof(Image), &pxTexture.Image, sizeof(Image));

            pxTexture.Type = PXGraphicTextureType2D;
            pxTexture.Filter = PXGraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
            pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
            pxTexture.WrapHeight = PXGraphicImageWrapStrechEdges;
            pxTexture.WrapWidth = PXGraphicImageWrapStrechEdges;

            PXGraphicTextureRegister(pxGraphic, &pxTexture);

            pxGraphicUIText->UIElement.Renderable.MeshSegmentList[0].TextureID = pxTexture.ID;

            break;
        }
        case PXUIElementTypeButton:
        {
            PXRenderable* renderable = &pxButton->UIElement.Renderable;

            pxButton->TextFont = pxFont;
            PXGraphicUITextRegister(pxGraphic, renderable, 0, 0, 1, 1, text);
            PXGraphicModelShaderSet(pxGraphic, renderable, shader);
            PXMatrix4x4FScaleSet(0.0017, 0.002, 1, &renderable->MatrixModel);
            PXMatrix4x4FMoveToScaleXY(&renderable->MatrixModel, -0.9, -0.9, &renderable->MatrixModel);
            renderable->MeshSegmentList[0].RenderMode = PXGraphicDrawModeSquare;

            PXLockEngage(&pxGraphic->_pxUIElements);
            PXLinkedListFixedNodeAdd(&pxGraphic->_pxUIElements, &pxButton->UIElement);
            PXLockRelease(&pxGraphic->_pxUIElements);

            break;
        }
        case PXUIElementTypeImage:

        case PXUIElementTypeCustom:

        default:
            break;
    }*/
}
PXActionResult PXAPI PXGraphicUIElementUpdate(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement)
{

}
PXActionResult PXAPI PXGraphicUIElementUnregister(PXGraphic* const pxGraphic, PXUIElement* const pxUIElement)
{

}

void PXAPI PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment)
{
    pxRenderableMeshSegment->NumberOfVertices = 0;
    pxRenderableMeshSegment->TextureID = (unsigned int)-1;
    pxRenderableMeshSegment->ShaderID = (unsigned int)-1;
    pxRenderableMeshSegment->RenderMode = PXGraphicDrawModeInvalid;
    pxRenderableMeshSegment->DoRendering = PXNo;
}

PXActionResult PXAPI PXGraphicInstantiate(PXGraphic* const pxGraphic, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
    pxGraphic->UIElementIDCounter = 0;

    pxGraphic->AttachedWindow = pxGraphicInitializeInfo->WindowReference;
    pxGraphic->GraphicSystem = pxGraphicInitializeInfo->GraphicSystem;
    pxGraphicInitializeInfo->Graphic = pxGraphic;

    
    /*
    for (size_t displayID = 0; displayID < EnumDisplayMonitors(PXNull, ); ++displayID)
    {

    }*/

    // EnumDisplayDevicesA

    // Fetch all graphical devices 
    {
        DISPLAY_DEVICEA displayDevice;
        displayDevice.cb = sizeof(DISPLAY_DEVICEA);

        // Count how many devices we have.
        for (pxGraphic->DevicePhysicalListSize = 0; EnumDisplayDevicesA(NULL, pxGraphic->DevicePhysicalListSize, &displayDevice, 0); ++(pxGraphic->DevicePhysicalListSize));

        // Allocate space for needed devices
        pxGraphic->DevicePhysicalList = PXNewList(PXGraphicDevicePhysical, pxGraphic->DevicePhysicalListSize);

        // Loop over all devices and store space
        for (PXSize deviceID = 0; EnumDisplayDevicesA(NULL, deviceID, &displayDevice, 0); ++deviceID)
        {
            PXGraphicDevicePhysical* const currentDevice = &pxGraphic->DevicePhysicalList[deviceID];

            PXTextCopyA(displayDevice.DeviceName, 32, currentDevice->DeviceDisplay, PXDeviceDisplaySize);
            PXTextCopyA(displayDevice.DeviceString, 128, currentDevice->DeviceName, PXDeviceNameSize);
            PXTextCopyA(displayDevice.DeviceID, 128, currentDevice->DeviceID, PXDeviceIDSize);
            PXTextCopyA(displayDevice.DeviceKey, 128, currentDevice->DeviceKey, PXDeviceKeySize);

            currentDevice->IsConnectedToMonitor = displayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP;
        }
    }  
 

#if 0
    // Create GPU affinity
    int selecvtDeviceID = 0;
    HDC dc = CreateDCA(graphicDevicePhysical[selecvtDeviceID].DeviceDisplay, graphicDevicePhysical[selecvtDeviceID].DeviceName, NULL, NULL); // "DISPLAY"

    auto xxxx = GetLastError();

    pxWindow->HandleDeviceContext = dc;
#endif

    PXWindowPixelSystemSet(pxGraphicInitializeInfo->WindowReference);



#if 0

   // void choose_ogl_vendor(const char* vendor_id)
   // {
        int idx;
   
        HDC dc;
        PIXELFORMATDESCRIPTOR pfd;

        dd.cb = sizeof(dd);
        idx = 0;
        while (1)
        {
            if (!EnumDisplayDevicesA(NULL, idx, &dd, 0))
                return; // not found!
            if (strstr(dd.DeviceID, vendor_id))
                break; // there we go
            idx += 1;
        }

        dc = CreateDCA(dd.DeviceName, NULL, NULL, NULL);
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        // those flags are not important, they just need to be valid (and nondemanding, just in case).
        // later you will use whatever flags you wish when you are creating your actual gl context
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DEPTH_DONTCARE;
        ChoosePixelFormat(dc, &pfd);
        DeleteDC(dc);
  //  }
#endif

    PXLockCreate(&pxGraphic->_resourceLock, PXLockTypeGlobal);

    PXDictionaryConstruct(&pxGraphic->ResourceImageLookUp, sizeof(PXInt32U), sizeof(PXImage), PXDictionaryValueLocalityExternalReference);

    PXDictionaryConstruct(&pxGraphic->UIElementLookUp, sizeof(PXInt32U), sizeof(PXUIElement), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxGraphic->TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture2D), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxGraphic->SpritelLookUp, sizeof(PXInt32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxGraphic->ModelLookUp, sizeof(PXInt32U), sizeof(PXVertexStructure), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxGraphic->FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxGraphic->SoundLookup, sizeof(PXInt32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&pxGraphic->ShaderPXProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);

    //-------------------------------------------------------------------------
    // Setup references
    //-------------------------------------------------------------------------
    switch (pxGraphic->GraphicSystem)
    {
        case PXGraphicSystemOpenGL:
        {
            pxGraphic->EventOwner = &pxGraphic->OpenGLInstance;

            pxGraphic->ShaderProgramCreateFromFileVP = PXOpenGLShaderProgramCreateFromFileVF;
            pxGraphic->ShaderProgramCreateFromFileVPA = PXOpenGLShaderProgramCreateFromFileVFA;
            pxGraphic->ShaderProgramCreateFromStringVP = PXOpenGLShaderProgramCreateFromStringVF;
            pxGraphic->ShaderProgramCreateFromStringVPA = PXOpenGLShaderProgramCreateFromStringVFA;

            pxGraphic->ShaderVariableFx1 = PXOpenGLShaderVariableFx1;
            pxGraphic->ShaderVariableFx1xN = PXOpenGLShaderVariableFx1xN;
            pxGraphic->ShaderVariableIx1 = PXOpenGLShaderVariableIx1;
            pxGraphic->ShaderVariableIx1xN = PXOpenGLShaderVariableIx1xN;
            pxGraphic->ShaderVariableFx2 = PXOpenGLShaderVariableFx2;
            pxGraphic->ShaderVariableFx2xN = PXOpenGLShaderVariableFx2xN;
            pxGraphic->ShaderVariableIx2 = PXOpenGLShaderVariableIx2;
            pxGraphic->ShaderVariableIx2xN = PXOpenGLShaderVariableIx2xN;
            pxGraphic->ShaderVariableFx3 = PXOpenGLShaderVariableFx3;
            pxGraphic->ShaderVariableFx3xN = PXOpenGLShaderVariableFx3xN;
            pxGraphic->ShaderVariableIx3 = PXOpenGLShaderVariableIx3;
            pxGraphic->ShaderVariableIx3xN = PXOpenGLShaderVariableIx3xN;
            pxGraphic->ShaderVariableFx4 = PXOpenGLShaderVariableFx4;
            pxGraphic->ShaderVariableFx4xN = PXOpenGLShaderVariableFx4xN;
            pxGraphic->ShaderVariableIx4 = PXOpenGLShaderVariableIx4;
            pxGraphic->ShaderVariableIx4xN = PXOpenGLShaderVariableIx4xN;
            pxGraphic->ShaderVariableMatrix2fv = PXOpenGLShaderVariableMatrix2fv;
            pxGraphic->ShaderVariableMatrix3fv = PXOpenGLShaderVariableMatrix3fv;
            pxGraphic->ShaderVariableMatrix4fv = PXOpenGLShaderVariableMatrix4fv;


            pxGraphic->ShaderVariableIDFetch = PXOpenGLShaderVariableIDFetch;
            pxGraphic->DrawModeSet = PXOpenGLDrawMode;

            pxGraphic->DrawColorRGBAF = PXOpenGLDrawColorRGBAF;

            pxGraphic->RectangleDraw = PXOpenGLRectangleDraw;
            pxGraphic->RectangleDrawTx = PXOpenGLRectangleDrawTx;

            pxGraphic->SwapIntervalSet = PXOpenGLSwapIntervalSet;
            pxGraphic->SwapIntervalGet = PXOpenGLSwapIntervalGet;

            pxGraphic->DevicePhysicalListAmount = PXOpenGLDevicePhysicalListAmount;
            pxGraphic->DevicePhysicalListFetch = PXOpenGLDevicePhysicalListFetch;

            pxGraphic->Initialize = PXOpenGLInitialize;
            pxGraphic->Release = PXOpenGLRelease;
            pxGraphic->Select = PXOpenGLSelect;
            pxGraphic->Deselect = PXOpenGLDeselect;
            pxGraphic->Clear = PXOpenGLClear;
            pxGraphic->SceneDeploy = PXOpenGLSceneDeploy;
            pxGraphic->ViewPortGet = PXOpenGLViewPortGet;
            pxGraphic->ViewPortSet = PXOpenGLViewPortSet;
            pxGraphic->ShaderProgramCreate = PXOpenGLShaderProgramCreate;
            pxGraphic->ShaderProgramSelect = PXOpenGLShaderProgramSelect;
            pxGraphic->ShaderProgramDelete = PXOpenGLShaderProgramDelete;
            pxGraphic->SceneBegin = PXNull;
            pxGraphic->SceneEnd = PXNull;
            pxGraphic->DrawScriptCreate = PXOpenGLDrawScriptCreate;
            pxGraphic->DrawScriptBegin = PXOpenGLDrawScriptBegin;
            pxGraphic->DrawScriptEnd = PXOpenGLDrawScriptEnd;
            pxGraphic->DrawScriptDelete = PXOpenGLDrawScriptDelete;
            pxGraphic->DrawScriptExecute = PXOpenGLDrawScriptExecute;
            pxGraphic->TextureCubeRegister = PXOpenGLTextureCubeCreate;
            pxGraphic->TextureCubeRegisterUse = PXNull;
            pxGraphic->TextureCubeRelease = PXNull;
            pxGraphic->VertexStructureRegister = PXOpenGLVertexStructureRegister;
            pxGraphic->VertexStructureDraw = PXOpenGLVertexStructureDraw;
            pxGraphic->VertexStructureSelect = PXNull;
            pxGraphic->VertexStructureRelease = PXNull;
            pxGraphic->LightSet = PXOpenGLLightSet;
            pxGraphic->LightGet = PXOpenGLLightGet;
            pxGraphic->LightEnableSet = PXNull;
            pxGraphic->LightEnableGet = PXNull;
            pxGraphic->Texture2DRegister = PXOpenGLTexture2DCreate;
            pxGraphic->Texture2DRelease = PXNull;
            pxGraphic->Texture2DSelect = PXOpenGLTexture2DBind;

            break;
        }
        case PXGraphicSystemDirectX:
        {
            pxGraphic->EventOwner = &pxGraphic->DirectXInstance;

            pxGraphic->ShaderProgramCreateFromFileVP = PXDirectXShaderProgramCreateFromFileVF;
            pxGraphic->ShaderProgramCreateFromFileVPA = PXDirectXShaderProgramCreateFromFileVFA;
            pxGraphic->ShaderProgramCreateFromStringVP = PXDirectXShaderProgramCreateFromStringVF;
            pxGraphic->ShaderProgramCreateFromStringVPA = PXDirectXShaderProgramCreateFromStringVFA;

            pxGraphic->ShaderVariableIDFetch = PXNull;

            pxGraphic->ShaderVariableFx1 = PXNull;
            pxGraphic->ShaderVariableFx1xN = PXNull;
            pxGraphic->ShaderVariableIx1 = PXNull;
            pxGraphic->ShaderVariableIx1xN = PXNull;
            pxGraphic->ShaderVariableFx2 = PXNull;
            pxGraphic->ShaderVariableFx2xN = PXNull;
            pxGraphic->ShaderVariableIx2 = PXNull;
            pxGraphic->ShaderVariableIx2xN = PXNull;
            pxGraphic->ShaderVariableFx3 = PXNull;
            pxGraphic->ShaderVariableFx3xN = PXNull;
            pxGraphic->ShaderVariableIx3 = PXNull;
            pxGraphic->ShaderVariableIx3xN = PXNull;
            pxGraphic->ShaderVariableFx4 = PXNull;
            pxGraphic->ShaderVariableFx4xN = PXNull;
            pxGraphic->ShaderVariableIx4 = PXNull;
            pxGraphic->ShaderVariableIx4xN = PXNull;
            pxGraphic->ShaderVariableMatrix2fv = PXNull;
            pxGraphic->ShaderVariableMatrix3fv = PXNull;
            pxGraphic->ShaderVariableMatrix4fv = PXNull;

            pxGraphic->DrawModeSet = PXNull;
            pxGraphic->DrawColorRGBAF = PXNull;

            pxGraphic->RectangleDraw = PXNull;
            pxGraphic->RectangleDrawTx = PXNull;

            pxGraphic->SwapIntervalSet = PXNull;
            pxGraphic->SwapIntervalGet = PXNull;

            pxGraphic->DevicePhysicalListAmount = PXNull;
            pxGraphic->DevicePhysicalListFetch = PXNull;

            pxGraphic->Initialize = PXDirectXInitialize; // PXDirectXVersion9, PXDirectXDriverTypeHardwareDevice);
            pxGraphic->Release = PXDirectXRelease;
            pxGraphic->Select = PXNull;
            pxGraphic->Deselect = PXNull;
            pxGraphic->Clear = PXDirectXClear;
            pxGraphic->SceneDeploy = PXDirectXSceneDeploy;
            pxGraphic->ViewPortGet = PXDirectXViewportGet;
            pxGraphic->ViewPortSet = PXDirectXViewportSet;
            pxGraphic->ShaderProgramCreate = PXDirectXShaderProgramCreate;
            pxGraphic->ShaderProgramSelect = PXDirectXShaderProgramSelect;
            pxGraphic->ShaderProgramDelete = PXDirectXShaderProgramDelete;
            pxGraphic->SceneBegin = PXDirectXSceneBegin;
            pxGraphic->SceneEnd = PXDirectXSceneEnd;
            pxGraphic->DrawScriptCreate = PXDirectXDrawScriptCreate;
            pxGraphic->DrawScriptBegin = PXDirectXDrawScriptBegin;
            pxGraphic->DrawScriptEnd = PXDirectXDrawScriptEnd;
            pxGraphic->DrawScriptDelete = PXDirectXDrawScriptDelete;
            pxGraphic->DrawScriptExecute = PXDirectXDrawScriptExecute;
            pxGraphic->TextureCubeRegister = PXDirectXTextureCubeCreate;
            pxGraphic->TextureCubeRegisterUse = PXNull;
            pxGraphic->TextureCubeRelease = PXNull;
            pxGraphic->VertexStructureRegister = PXDirectXVertexStructureRegister;
            pxGraphic->VertexStructureDraw = PXDirectXVertexStructureDraw;
            pxGraphic->VertexStructureSelect = PXNull;
            pxGraphic->VertexStructureRelease = PXNull;
            pxGraphic->LightSet = PXDirectXLightSet;
            pxGraphic->LightGet = PXDirectXLightGet;
            pxGraphic->LightEnableSet = PXNull;
            pxGraphic->LightEnableGet = PXNull;
            pxGraphic->Texture2DRegister = PXDirectXTexture2DCreate;
            pxGraphic->Texture2DRelease = PXNull;
            pxGraphic->Texture2DSelect = PXNull;     
            break;
        }
        case PXGraphicSystemVulcan:
        {
            pxGraphic->EventOwner = &pxGraphic->VulcanInstance;

            pxGraphic->Initialize = PXVulcanInitialize;
            pxGraphic->Release = PXVulcanRelease;

            break;
        }

        default:
            break;
    }

    // Graphic initialize
    {
        const PXActionResult pxActionResult = pxGraphic->Initialize(pxGraphic->EventOwner, pxGraphicInitializeInfo);

        PXActionReturnOnError(pxActionResult);
    }

    //-------------------------------------------------------------------------

    for (size_t i = 0; i < pxGraphic->DevicePhysicalListSize; i++)
    {
        PXGraphicDevicePhysical* const pxGraphicDevicePhysical = &pxGraphic->DevicePhysicalList[i];

        char targetBuffer[64];

        if (pxGraphicDevicePhysical->Driver[0] != '\0')
        {
            sprintf_s(targetBuffer, 64, "%s, %s (%s)", pxGraphicDevicePhysical->DeviceDisplay, pxGraphicDevicePhysical->Driver, pxGraphicDevicePhysical->IsConnectedToMonitor ? "Connected" : "Not Connected");
        }
        else
        {
            sprintf_s(targetBuffer, 64, "%s, (%s)", pxGraphicDevicePhysical->DeviceDisplay, pxGraphicDevicePhysical->IsConnectedToMonitor ? "Connected" : "Not Connected");
        }

      

        printf
        (
            "+---------------------------------------------------------+\n"
            "| Graphics Card (%2i/%-2i) - Information                     |\n"
            "+---------------------------------------------------------+\n"
            "| Model         : %-39s |\n"
            "| Target        : %-39s |\n"
            "| Vendor        : %-39s |\n"
            "| Renderer      : %-39s |\n"
            "| Shader-OpenGL : %-39s |\n"
            "+---------------------------------------------------------+\n\n",
            i+1,
            pxGraphic->DevicePhysicalListSize,
            pxGraphicDevicePhysical->DeviceName,          
            targetBuffer,
            pxGraphicDevicePhysical->Vendor,
            pxGraphicDevicePhysical->Renderer,
            pxGraphicDevicePhysical->Shader
        );
    }





    PXViewPort pxViewPort;
    pxViewPort.X = 0;
    pxViewPort.Y = 0;
    pxViewPort.Width = pxGraphic->AttachedWindow->Width;
    pxViewPort.Height = pxGraphic->AttachedWindow->Height;
    pxViewPort.ClippingMinimum = 0;
    pxViewPort.ClippingMaximum = 1;

    pxGraphic->ViewPortSet(pxGraphic->EventOwner, &pxViewPort);


    //PXMatrix4x4FIdentity(&pxGraphic->SpriteScaled.ModelMatrix);
   // PXMatrix4x4FIdentity(&pxGraphic->SpriteUnScaled.ModelMatrix);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXGraphicRelease(PXGraphic* const pxGraphic)
{
    return PXActionRefusedNotImplemented;
}

void PXAPI PXGraphicHotSwap(PXGraphic* const pxGraphic, const PXGraphicSystem pxGraphicSystem)
{
    //pxGraphic->Release();
}

void PXAPI PXGraphicResourceRegister(PXGraphic* const pxGraphic, PXGraphicResourceInfo* const pxGraphicResourceInfo)
{

}

PXActionResult PXAPI PXGraphicSpriteConstruct(PXGraphic* const pxGraphic, PXSprite* const pxSprite)
{
    PXObjectClear(PXSprite, pxSprite);

    PXVertexStructureConstruct(&pxSprite->VertexStructure);


    //PXMatrix4x4FIdentity(&pxSprite->ModelMatrix);
    //PXMatrix4x4FMoveXYZ(&pxSprite->ModelMatrix, 0,0,-0.5f, &pxSprite->ModelMatrix);

    PXVector2FSetXY(&pxSprite->TextureScaleOffset, 1, 1);

  //  PXRectangleOffsetSet(&pxSprite->Margin, 1, 1, 1, 1);
}

PXActionResult PXAPI PXGraphicSpriteTextureLoadA(PXGraphic* const pxGraphic, PXSprite* const pxSprite, const char* textureFilePath)
{
    PXTexture2D* pxTexture = PXNew(PXTexture2D);

    PXActionResult loadTextureResult = PXGraphicTexture2DLoadA(pxGraphic, pxTexture, textureFilePath);

    float aspectRationX = pxTexture->Image.Width / pxTexture->Image.Height;

    PXMatrix4x4FScaleSetXY(&pxSprite->VertexStructure.ModelMatrix, aspectRationX, 1);

    pxSprite->VertexStructure.IndexBuffer.Texture2D = pxTexture;

    return loadTextureResult;
}

PXActionResult PXAPI PXGraphicSpriteDraw(PXGraphic* const pxGraphic, const PXSprite* const pxSprite, const PXCamera* const pxCamera)
{
    switch (pxGraphic->GraphicSystem)
    {
        case PXGraphicSystemOpenGL:
        {
            return PXOpenGLSpriteDraw(&pxGraphic->OpenGLInstance, pxSprite, pxCamera);
        }
        case PXGraphicSystemDirectX:
        {
            return PXActionNotSupportedByLibrary;
        }
        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXAPI PXGraphicShaderUpdateMatrix4x4F(PXGraphic* const pxGraphic, const unsigned int locationID, const float* const matrix4x4)
{
#if PXOpenGLUSE
    PXOpenGLShaderVariableMatrix4fv(&pxGraphic->OpenGLInstance, locationID, 1, 0, matrix4x4);
#endif
}
#endif

void PXAPI PXCameraConstruct(PXCamera* const camera)
{
    PXMemoryClear(camera, sizeof(PXCamera));

    camera->WalkSpeed = 2;
    camera->ViewSpeed = 0.4;
    camera->FollowSpeed = 0.98f;
    camera->FieldOfView = 45;
    camera->Height = 1;
    camera->Width = 1;
    camera->Near = -0.001;
    camera->Far = 100000;

    PXMatrix4x4FIdentity(&camera->MatrixModel);
    PXMatrix4x4FIdentity(&camera->MatrixView);
    PXMatrix4x4FIdentity(&camera->MatrixProjection);

    PXCameraViewChange(camera, PXCameraPerspective3D);
}

void PXAPI PXCameraDestruct(PXCamera* const camera)
{

}

void PXAPI PXCameraViewChangeToOrthographic(PXCamera* const camera, const PXSize width, const PXSize height, const float nearPlane, const float farPlane)
{
    const float scaling = 0.005;
    const float left = -(width / 2.0f) * scaling;
    const float right = (width / 2.0f) * scaling;
    const float bottom = -(height / 2.0f) * scaling;
    const float top = (height / 2.0f) * scaling;

    camera->Perspective = PXCameraPerspective2D;
    camera->Width = width;
    camera->Height = height;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FOrthographic(&camera->MatrixProjection, left, right, bottom, top, nearPlane, farPlane);
}

void PXAPI PXCameraViewChangeToPerspective(PXCamera* const camera, const float fieldOfView, const float aspectRatio, const float nearPlane, const float farPlane)
{
    camera->Perspective = PXCameraPerspective3D;
    camera->FieldOfView = fieldOfView;
    camera->Near = nearPlane;
    camera->Far = farPlane;

    PXMatrix4x4FPerspective(&camera->MatrixProjection, fieldOfView, aspectRatio, nearPlane, farPlane);
}

void PXAPI PXCameraAspectRatioChange(PXCamera* const camera, const PXSize width, const PXSize height)
{
    camera->Width = width;
    camera->Height = height;

    PXCameraViewChange(camera, camera->Perspective);
}

float PXAPI PXCameraAspectRatio(const PXCamera* const camera)
{
    return (float)camera->Width / (float)camera->Height;
}

void PXAPI PXCameraViewChange(PXCamera* const camera, const PXCameraPerspective cameraPerspective)
{
    camera->Perspective = cameraPerspective;

    switch (cameraPerspective)
    {
        case PXCameraPerspective2D:
        {
            PXCameraViewChangeToOrthographic(camera, camera->Width, camera->Height, camera->Near, camera->Far);
            break;
        }

        case PXCameraPerspective3D:
        {
            const float aspectRatio = PXCameraAspectRatio(camera);

            PXCameraViewChangeToPerspective(camera, camera->FieldOfView, aspectRatio, camera->Near, camera->Far);

            break;
        }
    }
}

void PXAPI PXCameraRotate(PXCamera* const camera, const PXVector3F* const vector3F)
{
    const float maxValue = 85.0f;
    const float minValue = -85.0f;

    camera->CurrentRotation.X -= vector3F->X * camera->ViewSpeed;
    camera->CurrentRotation.Y -= vector3F->Y * camera->ViewSpeed;

    camera->CurrentRotation.Y = PXMathLimit(camera->CurrentRotation.Y, minValue, maxValue);

    const float pitchRAD = PXMathDegreeToRadians(camera->CurrentRotation.Y);
    const float yawRAD = PXMathDegreeToRadians(camera->CurrentRotation.X);
    const float rx = PXMathCosinus(pitchRAD) * PXMathCosinus(yawRAD);
    const float ry = PXMathSinus(pitchRAD);
    const float rz = PXMathCosinus(pitchRAD) * PXMathSinus(yawRAD);

    PXVector3FSetXYZ(&camera->LookAtPosition, rx, ry, rz);
    PXVector3FNormalize(&camera->LookAtPosition, &camera->LookAtPosition);
}

void PXAPI PXCameraRotateXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
    const PXVector3F vector = { x, y, z };

    PXCameraRotate(camera, &vector);
}

void PXAPI PXCameraMoveXYZ(PXCamera* const camera, const float x, const float y, const float z)
{
    const PXVector3F vector3F = { x, y, z };

    PXCameraMove(camera, &vector3F);
}

void PXAPI PXCameraMove(PXCamera* const camera, const PXVector3F* const vector3F)
{
    PXVector3F xAxis = { 0,0,0 };
    const PXVector3F yAxis = { 0, vector3F->Y, 0 };
    PXVector3F zAxis = { 0,0,0 };

    // ...
    {
        const PXVector3F up = { 0, 1, 0 };
        const PXVector3F lookAtPosition = { camera->LookAtPosition.X, camera->LookAtPosition.Y, camera->LookAtPosition.Z };

        PXVector3FCrossProduct(&lookAtPosition, &up, &xAxis);
        PXVector3FNormalize(&xAxis, &xAxis);
        PXVector3FMultiplyXYZ(&xAxis, vector3F->X, 0, vector3F->X, &xAxis);

        zAxis = lookAtPosition;

        PXVector3FNormalize(&zAxis, &zAxis);
        PXVector3FMultiplyXYZ(&zAxis, vector3F->Z, 0, vector3F->Z, &zAxis);
    }

    {
        PXVector3F targetedMovement = { 0,0,0 };

        PXVector3FAdd(&targetedMovement, &xAxis, &targetedMovement);
        PXVector3FAdd(&targetedMovement, &yAxis, &targetedMovement);
        PXVector3FAdd(&targetedMovement, &zAxis, &targetedMovement);
        PXVector3FMultiplyS(&targetedMovement, camera->WalkSpeed, &targetedMovement);

        PXMatrix4x4FMove3F(&camera->MatrixModel, &targetedMovement, &camera->MatrixModel);
    }
}

void PXAPI PXCameraFollow(PXCamera* const camera, const float deltaTime)
{
    PXVector3F cameraPositionCurrent;
    PXVector3F desiredPosition;

    if (!camera->Target)
    {
        return;
    }

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &cameraPositionCurrent); // Get current camera pos
    PXMatrix4x4FPositionGet(camera->Target, &desiredPosition); // get current target pos

    PXVector3FAdd(&desiredPosition, &camera->Offset, &desiredPosition); // add offset to target pos

    PXVector3FInterpolate(&cameraPositionCurrent, &desiredPosition, camera->FollowSpeed * deltaTime, &desiredPosition); // calculate delta movement

    PXMatrix4x4FPositionSet(&camera->MatrixModel, &desiredPosition, &camera->MatrixModel); // Set delte movement
}

void PXAPI PXCameraUpdate(PXCamera* const camera, const float deltaTime)
{
    const float walkSpeedSmoothed = camera->WalkSpeed * deltaTime;
    const float viewSpeedSmoothed = camera->ViewSpeed * deltaTime;
    const PXVector3F up = { 0,1,0 };
    PXVector3F currentPosition;
    PXVector3F centerPosition;

    PXMatrix4x4FPositionGet(&camera->MatrixModel, &currentPosition);
    PXVector3FAdd(&currentPosition, &camera->LookAtPosition, &centerPosition);

    PXMatrix4x4FLookAt(&camera->MatrixView, &currentPosition, &centerPosition, &up, &camera->MatrixView);
}
