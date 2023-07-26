#include "PXGraphic.h"

#if PXGraphicUSE

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Window/PXWindow.h>
#include <Media/ADLER/PXAdler32.h>

#include <OS/Graphic/OpenGL/PXOpenGL.h>
#include <OS/Graphic/DirectX/PXDirectX.h>
#include <OS/Graphic/Vulcan/PXVulcan.h>

#include <gl/GL.h>

PXActionResult PXGraphicLoadImage(PXGraphicContext* const graphicContext, PXImage* const pxImage, const PXText* const pxImageFilePath)
{
    // Create checksum
    const PXInt32U checkSum = PXAdler32Create(1, pxImageFilePath->TextA, pxImageFilePath->SizeUsed);


    // Check if already loaded
    {
        PXImage* pxImageFoundEntry = PXNull;

        const PXBool foundEntry = PXDictionaryFindEntry(&graphicContext->ResourceImageLookUp, &checkSum, &pxImageFoundEntry);
    
        if (foundEntry) // image is already loaded
        {          
            PXImageCopyAsIs(pxImage, pxImageFoundEntry);

            printf("[PXGraphic] Texture load <%s>... [REDUNDANT]\n", pxImageFilePath->TextA);

            return;
        }
    }

    // Load texture
    {
        printf("[PXGraphic] Texture load <%s>... ", pxImageFilePath->TextA);

        const PXActionResult loadResult = PXImageLoad(pxImage, pxImageFilePath);

        if (PXActionSuccessful != loadResult)
        {
            printf("[FAILED]\n");
        }

        PXActionReturnOnError(loadResult);

        printf("[OK]\n");

        PXDictionaryAdd(&graphicContext->ResourceImageLookUp, &checkSum, pxImage);
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicTextureScreenShot(PXGraphicContext* const graphicContext, PXImage* const image)
{
  //  PXOpenGLPixelDataRead(graphicContext, 0, 0, image->Width, image->Height, PXOpenGLImageFormatRGB, PXOpenGLTypeByteUnsigned, image->PixelData);

	return PXActionSuccessful;
}

PXActionResult PXGraphicTextureUse(PXGraphicContext* const graphicContext, PXTexture2D* const texture)
{
    /*
    const bool isValidTexture = textureID != -1 && imageType != ImageType::Invalid;

#if 1 // Ignore
    if (!isValidTexture)
    {
        return;
    }
#else
    assert(isValidTexture);
#endif

    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture

    const PXOpenGLID imageTypeID = ToImageType(imageType);

    glBindTexture(imageTypeID, textureID);
    */
    return PXActionInvalid;
}

PXActionResult PXGraphicTextureLoad(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const PXText* filePath)
{
    if (!graphicContext || !texture || !filePath)
    {
        return PXActionRefuedParameterNull;
    }

    texture->Type = PXGraphicImageTypeTexture2D;
    texture->Filter = PXGraphicRenderFilterNoFilter;
    texture->LayoutNear = PXGraphicImageLayoutNearest;
    texture->LayoutFar = PXGraphicImageLayoutNearest;
    texture->WrapHeight = PXGraphicImageWrapRepeat;
    texture->WrapWidth = PXGraphicImageWrapRepeat;

    // Load texture..
    {
        const PXActionResult registerResult = PXGraphicLoadImage(graphicContext, &texture->Image, filePath);

        PXActionReturnOnError(registerResult);
    }

    // Register as normal
    {
        const PXActionResult registerResult = PXGraphicTextureRegister(graphicContext, texture);

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

PXActionResult PXGraphicTextureLoadA(PXGraphicContext* const graphicContext, PXTexture2D* const texture, const char* const filePath)
{
    PXText pxText;
    PXTextConstructFromAdressA(&pxText, filePath, PXTextLengthUnkown);

    return PXGraphicTextureLoad(graphicContext, texture, &pxText);
}

PXActionResult PXGraphicFontLoad(PXGraphicContext* const graphicContext, PXFont* const pxFont, const PXText* const filePath)
{
    // Load texture
    { 
        const PXActionResult loadResult = PXFontLoad(pxFont, filePath);

        PXActionReturnOnError(loadResult);
    }

    // Register as normal
    {
        const PXActionResult registerResult = PXGraphicFontRegister(graphicContext, pxFont);

        PXActionReturnOnError(registerResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicFontRegister(PXGraphicContext* const graphicContext, PXFont* const pxFont)
{
    PXLockEngage(&graphicContext->_resourceLock);
    pxFont->ID = PXGraphicGenerateUniqeID(graphicContext);
    PXDictionaryAdd(&graphicContext->FontLookUp, &pxFont->ID, pxFont);
    PXLockRelease(&graphicContext->_resourceLock);

    for (PXSize i = 0; i < pxFont->FontElementSize; ++i)
    {
        PXSpriteFont* const pxSpriteFont = &pxFont->FontElement[i];

        PXTexture2D pxTexture;
        pxTexture.ResourceID.DirectXInterface = -1;
        pxTexture.Type = PXGraphicImageTypeTexture2D;
        pxTexture.Filter = PXGraphicRenderFilterNoFilter;
        pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
        pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
        pxTexture.WrapHeight = PXGraphicImageWrapStrechEdges;
        pxTexture.WrapWidth = PXGraphicImageWrapStrechEdges;
        pxTexture.Image = pxSpriteFont->FontPageList->FontTextureMap;    

        //PXImageRemoveColor(&pxTexture.Image, 0, 0, 0);

        PXGraphicTextureRegister(graphicContext, &pxTexture);

        pxSpriteFont->FontPageList[0].TextureID = pxTexture.ResourceID.OpenGLID;
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicFontRelease(PXGraphicContext* const graphicContext, PXFont* const pxFont)
{
    return PXActionSuccessful;
}

PXActionResult PXGraphicFontUse(PXGraphicContext* const graphicContext, PXFont* const pxFont)
{
    return PXActionSuccessful;
}

PXActionResult PXGraphicTextureRegister(PXGraphicContext* const graphicContext, PXTexture2D* const texture)
{
    PXLockEngage(&graphicContext->_resourceLock);
    texture->ResourceID.PXID = PXGraphicGenerateUniqeID(graphicContext);
    PXDictionaryAdd(&graphicContext->TextureLookUp, &texture->ResourceID.PXID, texture);
    PXLockRelease(&graphicContext->_resourceLock);

    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
        {
            return PXOpenGLTexture2DCreate(&graphicContext->OpenGLInstance, texture);
        }
        case PXGraphicSystemDirectX:
        {
            return PXDirectXTexture2DCreate(&graphicContext->DirectXInstance, texture);
        }

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXGraphicTextureRelease(PXGraphicContext* const graphicContext, PXTexture2D* const texture)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicTextureCubeRegister(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
        {
            return PXOpenGLTextureCubeCreate(&graphicContext->OpenGLInstance, textureCube);
        }
        case PXGraphicSystemDirectX:
        {
            return PXDirectXTextureCubeCreate(&graphicContext->DirectXInstance, textureCube);
        }

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXGraphicTextureCubeRegisterUse(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicTextureCubeRelease(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    return PXActionInvalid;
}

PXActionResult PXSpriteConstruct(PXSprite* const pxSprite)
{
    PXObjectClear(PXSprite, pxSprite);

    PXMatrix4x4FIdentity(&pxSprite->Position);
    
    PXVector2FSetXY(&pxSprite->TextureScaleOffset, 1, 1);

    PXMarginSet(&pxSprite->Margin, 1, 1, 1, 1);
}

PXActionResult PXSpriteTextureScaleBorder(PXSprite* const pxSprite, const float x, const float y)
{
    PXVector2FSetXY(&pxSprite->TextureScaleOffset, x, y);
}

PXActionResult PXGraphicSpriteRegister(PXGraphicContext* const graphicContext, PXSprite* const pxSprite)
{
    pxSprite->ID = PXGraphicGenerateUniqeID(graphicContext);

    PXDictionaryAdd(&graphicContext->SpritelLookUp, &pxSprite->ID, pxSprite);
    
    return PXActionSuccessful;
}

PXActionResult PXGraphicSkyboxRegister(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox)
{
    const float vertexData[] =
    {
         1,  1,  1,
        -1,  1,  1,
         1, -1,  1,
        -1, -1,  1,
         1,  1, -1,
        -1,  1, -1,
         1, -1, -1,
        -1, -1, -1,
    };
    const unsigned int indexList[] =
    {
        0,1,3,2, // Left OK
        6,7,5,4, // Right  OK
        4,5,1,0, // Top OK
        2,3,7,6, // Bot OK
        0,2,6,4, // Front OK
        5,7,3,1 // Back OK
    };
    const PXSize vertexDataSize = sizeof(vertexData) / sizeof(float);
    const PXSize indexListSize = sizeof(indexList) / sizeof(unsigned int);

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

        PXGraphicModelRegisterFromModel(graphicContext, &skyBox->Renderable, &pxModel);
    }

    // Register Cube Texture
    PXGraphicTextureCubeRegister(graphicContext, &skyBox->TextureCube);

    graphicContext->_currentSkyBox = skyBox;

    return PXActionSuccessful;
}

PXActionResult PXGraphicSkyboxRegisterD
(
    PXGraphicContext* const graphicContext,
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
            const PXActionResult textureRightResult = PXGraphicLoadImage(graphicContext, image, filePath);

            resultList[i] = textureRightResult;
        }
    }

    // Register skybox
    {
        const PXActionResult registerResult = PXGraphicSkyboxRegister(graphicContext, skyBox);

    }

    // Register Shaders
    {
        PXShaderProgram shaderPXProgram;
        shaderPXProgram.ResourceID.OpenGLID = -1;

        const PXActionResult shaderResult = PXGraphicShaderProgramLoad(graphicContext, &shaderPXProgram, shaderVertex, shaderFragment);

        skyBox->Renderable.MeshSegmentList[0].ShaderID = shaderPXProgram.ResourceID.OpenGLID;
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicSkyboxRegisterA
(
    PXGraphicContext* const graphicContext,
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
        graphicContext,
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

PXActionResult PXGraphicSkyboxUse(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox)
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

PXSize PXGraphicModelListSize(const PXGraphicContext* const graphicContext)
{


    return PXYes;
}

PXBool PXGraphicModelListGetFromIndex(const PXGraphicContext* const graphicContext, PXModel** pxModel, const PXSize index)
{
    return PXYes;
}

PXSize PXGraphicRenderableListSize(const PXGraphicContext* const graphicContext)
{
    return graphicContext->_renderList.NodeListSizeCurrent;
}

PXBool PXGraphicRenderableListGetFromIndex(const PXGraphicContext* const graphicContext, PXRenderable** pxRenderable, const PXSize index)
{
#if 1 //
    PXLinkedListNodeFixed currentModel;

    const PXBool successful = PXLinkedListFixedNodeAt(&graphicContext->_renderList, &currentModel, index);

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

    PXLinkedListFixedNodeAt(&graphicContext->_renderList, &currentModel, 0);

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
    while (PXLinkedListFixedNodeNext(&graphicContext->_renderList, &currentModel));

    return PXNo;
#endif
}

PXActionResult PXGraphicModelCreate(PXGraphicContext* const graphicContext, PXModel** const pxModel)
{
    PXModel* const model = PXMemoryAllocateType(PXModel, 1u);

    if (!model)
    {
        return PXActionFailedMemoryAllocation;
    }

    PXModelConstruct(model);

    PXGraphicModelRegister(graphicContext, model);

    *pxModel = model;

    return PXActionSuccessful;
}

PXBool PXGraphicModelRegister(PXGraphicContext* const graphicContext, PXModel* const pxModel)
{
    return PXTrue;
}

PXActionResult PXGraphicRenderableCreate(PXGraphicContext* const graphicContext, PXRenderable** const pxRenderable)
{
    PXRenderable* const renderable = PXMemoryAllocateType(PXRenderable, 1u);

    if (!renderable)
    {
        return PXActionFailedMemoryAllocation;
    }

    PXMemoryClear(renderable, sizeof(PXRenderable));

    PXGraphicRenderableRegister(graphicContext, *pxRenderable);

    *pxRenderable = renderable; // TODO: Is this right?

    return PXActionSuccessful;
}

PXBool PXGraphicRenderableRegister(PXGraphicContext* const graphicContext, PXRenderable* const pxRenderable)
{
    PXLockEngage(&graphicContext->_resourceLock);
    PXLinkedListFixedNodeAdd(&graphicContext->_renderList, pxRenderable);
    PXLockRelease(&graphicContext->_resourceLock);

    return PXTrue;
}

void PXGraphicModelShaderSet(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXShaderProgram* const shaderPXProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = shaderPXProgram->ResourceID.OpenGLID;
    }
}

PXActionResult PXGraphicModelLoad(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXText* const filePath)
{
    PXModel* model = 0;

    // Create model
    {
        const PXActionResult createResult = PXGraphicModelCreate(graphicContext, &model);

        PXActionReturnOnError(createResult);
    }

    // create PXRenderable
    {
        PXLockEngage(&graphicContext->_resourceLock);
        PXLinkedListFixedNodeAdd(&graphicContext->_renderList, renderable);
        PXLockRelease(&graphicContext->_resourceLock);

        /*

        if (renderable) // Owner is caller
        {
            pxRenderable = *renderable;


        }
        else // No model, make one
        {
            const PXActionResult createResult = PXGraphicRenderableCreate(graphicContext, &pxRenderable);

            PXActionExitOnError(createResult);

            *renderable = pxRenderable; // Write to return back to caller
        }
        */
    }

    // Load model
    {
        const PXActionResult loadResult = PXModelLoad(model, filePath);

        PXActionReturnOnError(loadResult);
    }

    // Register model into renderable
    {
        const PXActionResult loadResult = PXGraphicModelRegisterFromModel(graphicContext, renderable, model);

        PXActionReturnOnError(loadResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicModelRegisterFromModel(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXModel* const model)
{
#if PXOpenGLUSE
    PXOpenGL* const openGLContext = &graphicContext->OpenGLInstance;

    PXRenderableConstruct(renderable);

    PXOpenGLVertexArrayGenerate(openGLContext, 1u, &renderable->VAO);

    PXOpenGLID vbo = 0;

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
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataVertexWidth, PXOpenGLTypeFloat, 0, model->DataVertexWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataVertexWidth * sizeof(float);
    }

    if (model->DataNormalWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataNormalWidth, PXOpenGLTypeFloat, 0, model->DataNormalWidth * sizeof(float), offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataNormalWidth * sizeof(float);
    }

    if (model->DataTextureWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataTextureWidth, PXOpenGLTypeFloat, 0, model->DataTextureWidth * sizeof(float), offset);
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

        pxRenderableMeshSegment->RenderMode = PXGraphicRenderModeTriangle;
        pxRenderableMeshSegment->NumberOfVertices = meshSegment.DrawClusterSize;

        PXMaterial material;
        const PXBool fetchMaterialSuccess = PXModelMaterialGet(model, meshSegment.TextureID, &material);

        if (fetchMaterialSuccess)
        {
            PXTexture2D pxTexture;

            PXTextureConstruct(&pxTexture);

            pxTexture.Type = PXGraphicImageTypeTexture2D;
            pxTexture.Filter = PXGraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
            pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
            pxTexture.WrapHeight = PXGraphicImageWrapRepeat;
            pxTexture.WrapWidth = PXGraphicImageWrapRepeat;

            //PXGraphicTextureLoad(graphicContext, &pxTexture, &material.DiffuseTextureFilePath);

            pxRenderableMeshSegment->TextureID = pxTexture.ResourceID.OpenGLID;
        }

        pxRenderableMeshSegment->DoRendering = PXYes;
    }
    //-------------------------------------------------------------------------

    // Model is not fully registered and ready to be rendered
    renderable->DoRendering = 1u;
#endif

    return PXActionSuccessful;
}

PXActionResult PXGraphicModelRegisterFromData(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize)
{
    /*
    // Check
    {
        const bool hasData = vertexData && vertexDataSize && indexList && indexListSize;

        if (!hasData)
        {
            return;
        }
    }

    PXOpenGLID id[3] = { (unsigned int)-1,(unsigned int)-1,(unsigned int)-1 };

    glGenVertexArrays(1, &id[0]);

    glBindVertexArray(id[0]);

    glGenBuffers(2, &id[1]);

    glBindBuffer(GL_ARRAY_BUFFER, id[1]);
    glBufferData(GL_ARRAY_BUFFER, vertexDataSize * sizeof(float), vertexData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexListSize * sizeof(unsigned int), indexList, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    renderable.ID = id[0];
    renderable.ChunkListSize = 1;
    renderable.ChunkList = new RenderableChunk();
    renderable.ChunkList->ID = id[1];
    renderable.ChunkList->SegmentListSize = 1;
    renderable.ChunkList->SegmentList = new RenderableSegment();
    renderable.ChunkList->SegmentList[0].ID = id[2];
    renderable.ChunkList->SegmentList[0].Size = indexListSize;
    renderable.ChunkList->SegmentList[0].MaterialRange = new SegmentMaterialRange();
    renderable.ChunkList->SegmentList[0].MaterialRangeSize = 1;
    renderable.ChunkList->SegmentList[0].MaterialRange[0].Size = indexListSize;
    */

    return PXActionInvalid;
}

PXActionResult PXGraphicUIRectangleCreate(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXSize x, const PXSize y, const PXSize sidth, const PXSize height)
{
    PXMatrix4x4FMoveXY(&renderable->MatrixModel, x, y, &renderable->MatrixModel);
    PXMatrix4x4FScaleSet(sidth, height, 1, &renderable->MatrixModel);

    // Register rectangle
    {
        //float

        //GraphicModelRegisterFromData(graphicContext, renderable, , );

    }

    return PXActionSuccessful;
}

void PXShaderConstruct(PXShader* const shader)
{
    PXMemoryClear(shader, sizeof(PXShader));

    shader->ResourceID.PXID = PXShaderNotRegisterd;
}

void PXShaderDataSet(PXShader* const shader, const PXShaderType type, const char* data, const PXSize size)
{
    shader->ResourceID.PXID = PXShaderNotRegisterd;
    shader->Type = type;
    shader->ContentSize = size;
    shader->Content = data;
}

void PXTextureConstruct(PXTexture2D* const texture)
{
    PXMemoryClear(texture, sizeof(PXTexture2D));
}

void PXTextureDestruct(PXTexture2D* const texture)
{

}

void PXRenderableConstruct(PXRenderable* const pxRenderable)
{
    PXMemoryClear(pxRenderable, sizeof(PXRenderable));

    PXMatrix4x4FIdentity(&pxRenderable->MatrixModel);

    pxRenderable->VAO = -1;
    pxRenderable->VBO = -1;
    pxRenderable->IBO = -1;
}

void PXUIElementConstruct(PXUIElement* const pxUIElement, const PXUIElementType pxUIElementType)
{
    PXMemoryClear(pxUIElement, sizeof(PXUIElement));

    pxUIElement->ID = -1;
    pxUIElement->TextureID = -1;
    pxUIElement->ShaderID = -1;
    pxUIElement->Type = pxUIElementType;
    pxUIElement->NameTextScale = 1;

    PXUIElementColorSet4F(pxUIElement, 1, 1, 1, 1);

    PXTextCopyA("[N/A]", 5, pxUIElement->Name, 32);
}

void PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha)
{
    pxUIElement->BackGroundColor.Red = red;
    pxUIElement->BackGroundColor.Green = green;
    pxUIElement->BackGroundColor.Blue = blue;
    pxUIElement->BackGroundColor.Alpha = alpha;
}

void PXUIElementPositionSetXYWH(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height, const PXUIElementPositionMode pxUIElementPositionMode)
{
    pxUIElement->X = x;
    pxUIElement->Y = y;
    pxUIElement->Width = width;
    pxUIElement->Height = height;
    pxUIElement->PositionMode = pxUIElementPositionMode;
}

void PXUIElementTextSet(PXUIElement* const pxUIElement, PXText* const pxText)
{
    PXText nameBuffer;
    PXTextConstructFromAdressA(&nameBuffer, pxUIElement->Name, 32);

    PXTextCopy(pxText, &nameBuffer);
}

void PXUIElementTextSetA(PXUIElement* const pxUIElement, const char* const text)
{
    PXTextCopyA(text, -1, pxUIElement->Name, 32);
}

void PXUIElementTextSetAV(PXUIElement* const pxUIElement, const char* const format, ...)
{
    va_list args;
    va_start(args, format);

    PXTextPrint(pxUIElement, format, args);

    va_end(args);
}

void PXUIElementFontSet(PXUIElement* const pxUIElement, const PXFont* const pxFont)
{
    pxUIElement->FontID = pxFont;
}

void PXUIElementHoverable(PXUIElement* const pxUIElement, const PXBool isHoverable)
{
    pxUIElement->IsHoverable = isHoverable;
}

void PXUIElementParentSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent)
{
    pxUIElement->Parent = pxUIElementParent;

    PXUIElementChildSet(pxUIElementParent, pxUIElement);
}

void PXUIElementChildSet(PXUIElement* const pxUIElement, PXUIElement* const pxUIElementParent)
{
    pxUIElement->Child = pxUIElementParent;
}

PXInt32U PXGraphicGenerateUniqeID(PXGraphicContext* const graphicContext)
{
    return ++graphicContext->UniqeIDGeneratorCounter;
}

PXActionResult PXGraphicUIElementRegister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement)
{
    // Is Registerd
    {
        const PXBool isRegisterd = pxUIElement->ID != -1;

        if (!isRegisterd)
        {
            return PXActionInvalidRedundandInteraction;
        }

        // Create
        pxUIElement->ID = PXGraphicGenerateUniqeID(graphicContext);

        PXDictionaryAdd(&graphicContext->UIElementLookUp, &pxUIElement->ID, pxUIElement);
    }



    switch (pxUIElement->Type)
    {
        case PXUIElementTypeRenderFrame:
        {
            PXOpenGL* const pxOpenGL = &graphicContext->OpenGLInstance;

            // Texture
            PXOpenGLTextureCreate(pxOpenGL, 1, &pxUIElement->FrameRenderTextureID);
            PXOpenGLTextureBind(pxOpenGL, PXOpenGLTextureType2D, pxUIElement->FrameRenderTextureID);

            PXOpenGLTextureData2D(pxOpenGL, PXOpenGLTextureType2D, 0, PXOpenGLImageFormatRGB, pxUIElement->FrameRenderWidth, pxUIElement->Height, PXOpenGLImageFormatRGB, PXOpenGLTypeByteSigned, PXNull);

            PXOpenGLTextureParameter(pxOpenGL, PXOpenGLTextureType2D, PXOpenGLTextureMIN_FILTER, PXOpenGLTextureParameterValueNEAREST);
            PXOpenGLTextureParameter(pxOpenGL, PXOpenGLTextureType2D, PXOpenGLTextureMAG_FILTER, PXOpenGLTextureParameterValueNEAREST);

            // Framebuffer
            PXOpenGLFrameBufferCreate(pxOpenGL, 1, &pxUIElement->FrameBufferID);
            PXOpenGLFrameBufferBind(pxOpenGL, PXOpenGLFrameBufferModeDrawAndRead, pxUIElement->FrameBufferID);

            // Renderbuffer as depthbuffer
            PXOpenGLRenderBufferCreate(pxOpenGL, 1, &pxUIElement->FrameRenderID);
            PXOpenGLRenderBufferBind(pxOpenGL, pxUIElement->FrameRenderID);
            PXOpenGLRenderBufferStorage(pxOpenGL, PXOpenGLRenderBufferFormatDepthComponent, pxUIElement->FrameRenderWidth, pxUIElement->Height);
           
            // Link buffer
            PXOpenGLFrameBufferLinkRenderBuffer(pxOpenGL, PXOpenGLRenderBufferAttachmentPointDepth, pxUIElement->FrameRenderID);



            PXOpenGLFrameBufferLinkTexture2D(pxOpenGL, PXOpenGLRenderBufferAttachmentPointColor, PXOpenGLTextureType2D, pxUIElement->FrameRenderTextureID, 0);

            // Set the list of draw buffers.
           // GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
            //glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

           // // Always check that our framebuffer is ok
            //if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
             //   return false;


            PXOpenGLFrameBufferBind(pxOpenGL, PXOpenGLFrameBufferModeDrawAndRead, 0);
            PXOpenGLRenderBufferBind(pxOpenGL, 0);

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
                const PXActionResult actionResult = PXGraphicModelRegisterFromModel(graphicContext, &pxUIPanel->UIElement.Renderable, &model);

                PXActionExitOnError(actionResult);
            }

            PXGraphicRenderableRegister(graphicContext, &pxUIPanel->UIElement.Renderable);

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
                const PXActionResult actionResult = PXGraphicModelRegisterFromModel(graphicContext, &pxGraphicUIText->UIElement.Renderable, &model);

                PXActionExitOnError(actionResult);
            }

            PXGraphicRenderableRegister(graphicContext, &pxGraphicUIText->UIElement.Renderable);



            PXTexture pxTexture;

            PXTextureConstruct(&pxTexture);

            MemoryCopy(&pxGraphicUIText->TextFont->FontElement[0].FontPageList[0].FontTextureMap, sizeof(Image), &pxTexture.Image, sizeof(Image));

            pxTexture.Type = PXGraphicImageTypeTexture2D;
            pxTexture.Filter = PXGraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
            pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
            pxTexture.WrapHeight = PXGraphicImageWrapStrechEdges;
            pxTexture.WrapWidth = PXGraphicImageWrapStrechEdges;

            PXGraphicTextureRegister(graphicContext, &pxTexture);

            pxGraphicUIText->UIElement.Renderable.MeshSegmentList[0].TextureID = pxTexture.ID;

            break;
        }
        case PXUIElementTypeButton:
        {
            PXRenderable* renderable = &pxButton->UIElement.Renderable;

            pxButton->TextFont = pxFont;
            PXGraphicUITextRegister(graphicContext, renderable, 0, 0, 1, 1, text);
            PXGraphicModelShaderSet(graphicContext, renderable, shader);
            PXMatrix4x4FScaleSet(0.0017, 0.002, 1, &renderable->MatrixModel);
            PXMatrix4x4FMoveToScaleXY(&renderable->MatrixModel, -0.9, -0.9, &renderable->MatrixModel);
            renderable->MeshSegmentList[0].RenderMode = PXGraphicRenderModeSquare;

            PXLockEngage(&graphicContext->_pxUIElements);
            PXLinkedListFixedNodeAdd(&graphicContext->_pxUIElements, &pxButton->UIElement);
            PXLockRelease(&graphicContext->_pxUIElements);

            break;
        }
        case PXUIElementTypeImage:

        case PXUIElementTypeCustom:

        default:
            break;
    }*/
}
PXActionResult PXGraphicUIElementUpdate(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement)
{

}
PXActionResult PXGraphicUIElementUnregister(PXGraphicContext* const graphicContext, PXUIElement* const pxUIElement)
{

}

void PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment)
{
    pxRenderableMeshSegment->NumberOfVertices = 0;
    pxRenderableMeshSegment->TextureID = (unsigned int)-1;
    pxRenderableMeshSegment->ShaderID = (unsigned int)-1;
    pxRenderableMeshSegment->RenderMode = PXGraphicRenderModeInvalid;
    pxRenderableMeshSegment->DoRendering = PXNo;
}

void PXGraphicInstantiate(PXGraphicContext* const graphicContext)
{
    PXLockCreate(&graphicContext->_resourceLock, PXLockTypeGlobal);

    graphicContext->GraphicSystem = PXGraphicSystemPXOpenGL;

    PXDictionaryConstruct(&graphicContext->ResourceImageLookUp, sizeof(PXInt32U), sizeof(PXImage), PXDictionaryValueLocalityExternalReference);

    PXDictionaryConstruct(&graphicContext->UIElementLookUp, sizeof(PXInt32U), sizeof(PXUIElement), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture2D), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->SpritelLookUp, sizeof(PXInt32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->ModelLookUp, sizeof(PXInt32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->SoundLookup, sizeof(PXInt32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->ShaderPXProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);

#if PXWindowUSE
    PXWindow* const pxWindow = (PXWindow*)graphicContext->AttachedWindow;


    PXDirectXContextCreate(&graphicContext->DirectXInstance, pxWindow->ID, PXDirectXVersion9, PXDirectXDriverTypeHardwareDevice);

#if PXOpenGLUSE
    graphicContext->OpenGLInstance.AttachedWindow = pxWindow;

    PXOpenGLCreateForWindow(&graphicContext->OpenGLInstance);

    if (1)
    {
       // glEnable(GL_CULL_FACE);
      //  glCullFace(GL_BACK);
    }

    if (1) // X-RAY
    {
        //glEnable(GL_DEPTH_TEST);
    }

    if (1)
    {
       // glEnable(GL_BLEND);
       // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_COLOR, GL_DST_COLOR);

         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    }

    PXOpenGLDeselect(&graphicContext->OpenGLInstance);
#endif
#endif

    PXViewPort pxViewPort;
    pxViewPort.X = 0;
    pxViewPort.Y = 0;
    pxViewPort.Width = pxWindow->Width;
    pxViewPort.Height = pxWindow->Height;
    pxViewPort.ClippingMinimum = 0;
    pxViewPort.ClippingMaximum = 1;

    PXGraphicViewPortSet(graphicContext, &pxViewPort);
}


PXActionResult PXGraphicViewPortSet(PXGraphicContext* const graphicContext, const PXViewPort* const pxViewPort)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
            PXOpenGLViewPortSet(&graphicContext->OpenGLInstance, pxViewPort);
            break;

        case PXGraphicSystemDirectX:
            PXDirectXViewportSet(&graphicContext->DirectXInstance, pxViewPort);
            break;

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXGraphicViewPortGet(PXGraphicContext* const graphicContext, PXViewPort* const pxViewPort)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
            PXOpenGLViewPortGet(&graphicContext->OpenGLInstance, pxViewPort);
            break;

        case PXGraphicSystemDirectX:
            PXDirectXViewportGet(&graphicContext->DirectXInstance, pxViewPort);
            break;

        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXGraphicSelect(PXGraphicContext* const graphicContext)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
            PXOpenGLSelect(&graphicContext->OpenGLInstance);
            break;

        case PXGraphicSystemDirectX:
            break;

        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXGraphicDeselect(PXGraphicContext* const graphicContext)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
            PXOpenGLDeselect(&graphicContext->OpenGLInstance);
            break;

        case PXGraphicSystemDirectX:
            break;

        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXGraphicSwapIntervalSet(PXGraphicContext* const graphicContext, const PXInt32U interval)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
            PXOpenGLSwapIntervalSet(&graphicContext->OpenGLInstance, interval);
            break;

        case PXGraphicSystemDirectX:
            break;

        default:
            return PXActionNotSupportedByLibrary;
    }
}
PXInt32U PXGraphicSwapIntervalGet(PXGraphicContext* const graphicContext)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
            return PXOpenGLSwapIntervalGet(&graphicContext->OpenGLInstance);

        case PXGraphicSystemDirectX:
            break;

        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXMarginSet(PXMargin* const pxMargin, const float left, const float bottom, const float right, const float top)
{
    pxMargin->Left = left;
    pxMargin->Bottom = bottom;
    pxMargin->Right = right;
    pxMargin->Top = top;
}

void PXGraphicResourceRegister(PXGraphicContext* const graphicContext, PXGraphicResourceInfo* const pxGraphicResourceInfo)
{
 
}

void PXGraphicClear(PXGraphicContext* const graphicContext, const PXColorRGBAF* const backgroundColor)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
            PXOpenGLClearColor(&graphicContext->OpenGLInstance, backgroundColor);
            PXOpenGLClear(&graphicContext->OpenGLInstance, 0);
            break;

        case PXGraphicSystemDirectX:
            PXDirectXClear(&graphicContext->DirectXInstance, 0, 0, D3DCLEAR_TARGET, backgroundColor, 1.0f, 0);
            break;

        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXBool PXGraphicSceneDeploy(PXGraphicContext* const graphicContext)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
        {
            PXWindow* window = (PXWindow*)graphicContext->AttachedWindow;

            window->MouseCurrentInput.Delta[0] = 0;
            window->MouseCurrentInput.Delta[1] = 0;
            window->MouseCurrentInput.DeltaNormalisized[0] = 0;
            window->MouseCurrentInput.DeltaNormalisized[1] = 0;

            // PXOpenGLContextFlush(&graphicContext->OpenGLInstance);

            return PXWindowFrameBufferSwap(window);
        }

        case PXGraphicSystemDirectX:
        {
            PXDirectXPresent(&graphicContext->DirectXInstance, 0, 0, 0, 0);
            break;
        }
        default:
            return PXActionNotSupportedByLibrary;
    }
}


void PXGraphicSceneBegin(PXGraphicContext* const graphicContext)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
        {
            return PXActionNotSupportedByLibrary;
        }
        case PXGraphicSystemDirectX:
        {
            return PXDirectXSceneBegin(&graphicContext->DirectXInstance);
        }
        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXGraphicSceneEnd(PXGraphicContext* const graphicContext)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
        {
            return PXActionNotSupportedByLibrary;
        }
        case PXGraphicSystemDirectX:
        {
            return PXDirectXSceneEnd(&graphicContext->DirectXInstance);
        }
        default:
            return PXActionNotSupportedByLibrary;
    }
}

void PXGraphicVertexStructureDraw(PXGraphicContext* const graphicContext, PXVertexStructure* const pxVertexStructure)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
        {
            //glEnable(GL_TEXTURE_2D);
            //glBindTexture(GL_TEXTURE_2D, pxSprite->Texture.ResourceID.OpenGLID);

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_INDEX_ARRAY);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);

          //  glInterleavedArrays(GL_T2F_V3F, 5 * sizeof(float), vertexData);

            //glIndexPointer(GL_UNSIGNED_BYTE, 0, indexData);

            //glDrawElements(GL_QUADS, indexAmount, GL_UNSIGNED_BYTE, indexData);
            //glDrawElements(GL_LINE_LOOP, indexAmount, GL_UNSIGNED_BYTE, indexData);
            //glDrawArrays(GL_LINE_LOOP, 0, indexAmount);

           // glBindTexture(GL_TEXTURE_2D, 0);
           // glDisable(GL_TEXTURE_2D);

            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_TEXTURE_COORD_ARRAY);
            glDisableClientState(GL_INDEX_ARRAY);

            break;
        }
        case PXGraphicSystemDirectX:
        {
            PXDirectXStreamSourceSet(&graphicContext->DirectXInstance, 0, &pxVertexStructure->VertexBuffer, 0, pxVertexStructure->VertexBuffer.VertexDataRowSize);
            PXDirectXVertexFixedFunctionSet(&graphicContext->DirectXInstance, pxVertexStructure->VertexBuffer.Format);
            PXDirectXPrimitiveDraw(&graphicContext->DirectXInstance, PXGraphicRenderModeTriangle, 0, 1);

            break;
        }
        default:
            return PXActionNotSupportedByLibrary;
    }
}

PXActionResult PXGraphicShaderProgramCreate(PXGraphicContext* const graphicContext)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicShaderCompile(PXGraphicContext* const graphicContext)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicShaderUse(PXGraphicContext* const graphicContext, const unsigned int shaderID)
{
#if PXOpenGLUSE
    PXOpenGLShaderProgramUse(&graphicContext->OpenGLInstance, shaderID);
#endif

    return PXActionSuccessful;
}

PXActionResult PXGraphicShaderProgramLoad(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderPXProgram, const PXText* const vertexShaderFilePath, const PXText* const fragmentShaderFilePath)
{
    switch (graphicContext->GraphicSystem)
    {
        case PXGraphicSystemPXOpenGL:
             return PXOpenGLShaderProgramCompileVP(&graphicContext->OpenGLInstance, shaderPXProgram, vertexShaderFilePath, fragmentShaderFilePath);

        case PXGraphicSystemDirectX:
            return PXOpenGLShaderProgramCompileVP(&graphicContext->DirectXInstance, shaderPXProgram, vertexShaderFilePath, fragmentShaderFilePath);

        default:
            return PXActionNotSupportedByLibrary;
    }

    PXLockEngage(&graphicContext->_resourceLock);
    //shaderPXProgram->ID = PXGraphicGenerateUniqeID(graphicContext);
    //PXDictionaryAdd(&graphicContext->ShaderPXProgramLookup, &shaderPXProgram->ID, shaderPXProgram);
    PXLockRelease(&graphicContext->_resourceLock);

    //printf("[PX] Shader program ID:%i\n", shaderPXProgram->ID);
  
}

PXActionResult PXGraphicShaderProgramLoadA(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderPXProgram, const char* const vertexShaderFilePath, const char* const fragmentShaderFilePath)
{
    PXText vertexShader;
    PXTextConstructFromAdressA(&vertexShader, vertexShaderFilePath, PXTextLengthUnkown);

    PXText fragmentShader;
    PXTextConstructFromAdressA(&fragmentShader, fragmentShaderFilePath, PXTextLengthUnkown);

    PXGraphicShaderProgramLoad(graphicContext, shaderPXProgram, &vertexShader, &fragmentShader);
}

PXActionResult PXGraphicRender(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}

void PXGraphicShaderUpdateMatrix4x4F(PXGraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4)
{
#if PXOpenGLUSE
    PXOpenGLShaderVariableMatrix4fv(&graphicContext->OpenGLInstance, locationID, 1, 0, matrix4x4);
#endif
}

unsigned int PXGraphicShaderVariableIDFetch(PXGraphicContext* const graphicContext, const unsigned int shaderID, const char* const name)
{
#if PXOpenGLUSE
    const unsigned int locationID = PXOpenGLShaderVariableIDGet(&graphicContext->OpenGLInstance, shaderID, name);

    return locationID;
#else
    return -1;
#endif  
}

void PXGraphicShaderPXProgramUse(PXGraphicContext* const graphicContext, const unsigned int shaderID)
{
#if PXOpenGLUSE
    PXOpenGLShaderProgramUse(&graphicContext->OpenGLInstance, shaderID);
#endif
}

PXActionResult PXGraphicRenderElement(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicRenderList(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}
#endif