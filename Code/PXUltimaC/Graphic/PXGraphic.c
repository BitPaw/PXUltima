#include "PXGraphic.h"

#if PXGraphicUSE

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Graphic/OpenGL/PXOpenGL.h>
#include <OS/Window/PXWindow.h>

PXActionResult PXGraphicTextureScreenShot(PXGraphicContext* const graphicContext, PXImage* const image)
{
  //  PXOpenGLPixelDataRead(graphicContext, 0, 0, image->Width, image->Height, PXOpenGLImageFormatRGB, PXOpenGLTypeByteUnsigned, image->PixelData);

	return PXActionSuccessful;
}

PXActionResult PXGraphicTextureUse(PXGraphicContext* const graphicContext, PXTexture* const texture)
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

PXActionResult PXGraphicTextureLoad(PXGraphicContext* const graphicContext, PXTexture* const texture, const PXText* filePath)
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
        const PXActionResult loadResult = PXImageLoad(&texture->Image, filePath);

        PXActionReturnOnError(loadResult);
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

        PXTexture pxTexture;
        pxTexture.ID = -1;
        pxTexture.Type = PXGraphicImageTypeTexture2D;
        pxTexture.Filter = PXGraphicRenderFilterNoFilter;
        pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
        pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
        pxTexture.WrapHeight = PXGraphicImageWrapStrechEdges;
        pxTexture.WrapWidth = PXGraphicImageWrapStrechEdges;
        pxTexture.Image = pxSpriteFont->FontPageList->FontTextureMap;


        PXImageRemoveColor(&pxTexture.Image, 0, 0, 0);

        PXGraphicTextureRegister(graphicContext, &pxTexture);

        printf("Texture ID : %i\n", pxTexture.ID);
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

PXActionResult PXGraphicTextureRegister(PXGraphicContext* const graphicContext, PXTexture* const texture)
{
    PXLockEngage(&graphicContext->_resourceLock);
    texture->ID = PXGraphicGenerateUniqeID(graphicContext);
    PXDictionaryAdd(&graphicContext->TextureLookUp, &texture->ID, texture);
    PXLockRelease(&graphicContext->_resourceLock);

#if PXOpenGLUSE
    const PXOpenGLTextureType openGLTextureType = ImageTypeGraphicToPXOpenGL(texture->Type);
    const unsigned int openGLTextureTypeID = PXOpenGLTextureTypeToID(openGLTextureType);

    PXImage* const image = &texture->Image;

    if (!image->PixelData)
    {
        return PXActionRefuedInputBufferEmpty; // No image data
    }

    // Register and select
    {
        PXOpenGLID textureID = 0;

        PXOpenGLTextureCreate(&graphicContext->OpenGLInstance, 1u, &textureID);

        const PXBool registerSuccess = textureID != -1;

        if (!registerSuccess)
        {
            return PXActionFailedResourceRegister;
        }

        texture->ID = textureID;
    }

    PXOpenGLTextureBind(&graphicContext->OpenGLInstance, openGLTextureType, texture->ID);

    // Texture Style
    {
        //const int textureWrapWidth = PXOpenGLToImageWrap(texture->WrapWidth);
        //onst int textureWrapHeight = PXOpenGLToImageWrap(texture->WrapHeight);
        //const int textueFilterNear = PXOpenGLToImageLayout(texture->LayoutNear);
       // const int textueFilterFar = PXOpenGLToImageLayout(texture->LayoutFar);

        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Remember! This stuff is required for some reason, its not optional!
        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // if not done, textures might be black.
        //glTexParameteri(openGLTextureTypeID, GL_GENERATE_MIPMAP, GL_FALSE);
    }

    //glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    // ToDO: erro?
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


    const PXOpenGLImageFormat openGLImageFormat  = PXGraphicImageFormatToPXOpenGL(image->Format);

    PXOpenGLTextureData2D(&graphicContext->OpenGLInstance, openGLTextureType, 0, PXOpenGLImageFormatRGBA, image->Width, image->Height, openGLImageFormat, PXOpenGLTypeByteUnsigned, image->PixelData);

    // glTexImage2D(textureType, 0, GL_RGBA, image->Width, image->Height, 0, format, PXOpenGLTypeByteUnsigned, image->PixelData);

    //glGenerateMipmap(openGLTextureTypeID);

    PXOpenGLTextureUnbind(&graphicContext->OpenGLInstance, openGLTextureType);
#endif

    return PXActionSuccessful;
}

PXActionResult PXGraphicTextureRelease(PXGraphicContext* const graphicContext, PXTexture* const texture)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicTextureCubeRegister(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
#if PXOpenGLUSE
    PXOpenGLID textureID = -1;

    // Check
    {
        const unsigned char isValid = 1;// textureCube.HasTextures();

        if (!isValid)
        {
            return PXActionInvalid;
        }
    }

    PXOpenGLTextureCreate(&graphicContext->OpenGLInstance, 1u, &textureID);
    PXOpenGLTextureBind(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, textureID);

    PXOpenGLTextureParameter(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, PXOpenGLTextureWRAP_S, PXOpenGLTextureParameterValueClampToEdge);
    PXOpenGLTextureParameter(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, PXOpenGLTextureWRAP_T, PXOpenGLTextureParameterValueClampToEdge);
    PXOpenGLTextureParameter(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, PXOpenGLTextureWRAP_R, PXOpenGLTextureParameterValueClampToEdge);
    PXOpenGLTextureParameter(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, PXOpenGLTextureMAG_FILTER, PXOpenGLTextureParameterValueLINEAR);
    PXOpenGLTextureParameter(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, PXOpenGLTextureMIN_FILTER, PXOpenGLTextureParameterValueLINEAR);
    PXOpenGLTextureParameterI(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, PXOpenGLTextureBASE_LEVEL, 0);
    PXOpenGLTextureParameterI(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap, PXOpenGLTextureMAX_LEVEL, 0);


    PXOpenGLSettingChange(&graphicContext->OpenGLInstance, PXOpenGLToggleTextureCubeMapSeamless, PXTrue);

    const PXOpenGLTextureType openGLTextureTypeList[6] =
    {
        PXOpenGLTextureTypeCubeMapTop,
        PXOpenGLTextureTypeCubeMapBottom,
        PXOpenGLTextureTypeCubeMapFront,
        PXOpenGLTextureTypeCubeMapBack,
        PXOpenGLTextureTypeCubeMapLeft,
        PXOpenGLTextureTypeCubeMapRight
    };

    for (PXSize i = 0; i < 6u; ++i)
    {
        const PXImage* const image = &textureCube->ImageList[i];
        const PXOpenGLTextureType textureTypeID = openGLTextureTypeList[i];
        const PXOpenGLImageFormat openGLImageFormat = PXGraphicImageFormatToPXOpenGL(image->Format);
        const PXOpenGLDataType openGLDataType = PXGraphicDataTypeToPXOpenGL(image->Format);
        const int levelOfDetail = 0;

        PXOpenGLTextureData2D
        (
            &graphicContext->OpenGLInstance,
            textureTypeID,
            levelOfDetail,
            PXOpenGLImageFormatRGB,
            image->Width,
            image->Height,
            openGLImageFormat,
            openGLDataType,
            image->PixelData
        );
    }

    PXOpenGLTextureUnbind(&graphicContext->OpenGLInstance, PXOpenGLTextureTypeCubeMap);

    textureCube->ID = textureID;
#endif

    return PXActionSuccessful;
}

PXActionResult PXGraphicTextureCubeRegisterUse(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicTextureCubeRelease(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    return PXActionInvalid;
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
        PXByte bufferData[32];
        PXModel pxModel;

        PXModelConstruct(&pxModel);

        pxModel.Data = bufferData;

        PXModelSegmentsAdd(&pxModel, 3u, vertexDataSize, -1);

        pxModel.DataVertexList = (void*)vertexData;
        pxModel.DataVertexListSize = vertexDataSize;

        pxModel.DataVertexWidth = 3u;
        pxModel.DataVertexSize = vertexDataSize;

        pxModel.IndexList = (void*)indexList;
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
            const PXActionResult textureRightResult = PXImageLoad(image, filePath);

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
        shaderPXProgram.ID = -1;

        const PXActionResult shaderResult = PXGraphicShaderProgramLoadGLSL(graphicContext, &shaderPXProgram, shaderVertex, shaderFragment);

        skyBox->Renderable.MeshSegmentList[0].ShaderID = shaderPXProgram.ID;
    }

    return PXActionSuccessful;
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

        pxRenderableMeshSegment->ShaderID = shaderPXProgram->ID;
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

    const PXSize stride = PXModelVertexDataStride(model);
    const PXSize vertexListSize = model->DataVertexListSize * stride;

    PXOpenGLBufferBind(openGLContext, PXOpenGLBufferArray, renderable->VBO);
    PXOpenGLBufferData(openGLContext, PXOpenGLBufferArray, vertexListSize, model->DataVertexList, PXOpenGLStoreStaticDraw);

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
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataVertexWidth, PXOpenGLTypeFloat, 0, stride, offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataVertexWidth * sizeof(float);
    }

    if (model->DataNormalWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataNormalWidth, PXOpenGLTypeFloat, 0, stride, offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataNormalWidth * sizeof(float);
    }

    if (model->DataTextureWidth)
    {
        PXOpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataTextureWidth, PXOpenGLTypeFloat, 0, stride, offset);
        PXOpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataTextureWidth * sizeof(float);
    }

    // Color?

    PXOpenGLBufferUnbind(openGLContext, PXOpenGLBufferArray);

    if (model->DataIndexWidth)
    {
        PXOpenGLBufferGenerate(openGLContext, 1u, &renderable->IBO);

        PXOpenGLBufferBind(openGLContext, PXOpenGLBufferElementArray, renderable->IBO);

        PXOpenGLBufferData(openGLContext, PXOpenGLBufferElementArray, sizeof(unsigned int) * model->DataIndexSize, model->IndexList, PXOpenGLStoreStaticDraw);

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
            PXTexture pxTexture;

            PXTextureConstruct(&pxTexture);

            pxTexture.Type = PXGraphicImageTypeTexture2D;
            pxTexture.Filter = PXGraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = PXGraphicImageLayoutNearest;
            pxTexture.LayoutFar = PXGraphicImageLayoutNearest;
            pxTexture.WrapHeight = PXGraphicImageWrapRepeat;
            pxTexture.WrapWidth = PXGraphicImageWrapRepeat;

            PXGraphicTextureLoad(graphicContext, &pxTexture, &material.DiffuseTextureFilePath);

            pxRenderableMeshSegment->TextureID = pxTexture.ID;
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

    shader->ID = PXShaderNotRegisterd;
}

void PXShaderDataSet(PXShader* const shader, const PXShaderType type, const char* data, const PXSize size)
{
    shader->ID = PXShaderNotRegisterd;
    shader->Type = type;
    shader->ContentSize = size;
    shader->Content = data;
}

void PXTextureConstruct(PXTexture* const texture)
{
    PXMemoryClear(texture, sizeof(PXTexture));
}

void PXTextureDestruct(PXTexture* const texture)
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

void PXUIElementConstruct(PXUIElement* const pxUIElement)
{
    PXMemoryClear(pxUIElement, sizeof(PXUIElement));

    pxUIElement->ID = -1;
    pxUIElement->TextureID = -1;
    pxUIElement->ShaderID = -1;

    PXUIElementColorSet4F(pxUIElement, 1, 0, 1, 1);

    PXTextCopyA("[N/A]", 5, pxUIElement->Name, 32);
}

void PXUIElementColorSet4F(PXUIElement* const pxUIElement, const float red, const float green, const float blue, const float alpha)
{
    pxUIElement->BackGroundColor.Red = red;
    pxUIElement->BackGroundColor.Green = green;
    pxUIElement->BackGroundColor.Blue = blue;
    pxUIElement->BackGroundColor.Alpha = alpha;
}

void PXUIElementPositionSetXYWH(PXUIElement* const pxUIElement, const float x, const float y, const float width, const float height)
{
    pxUIElement->X = x;
    pxUIElement->Y = y;
    pxUIElement->Width = width;
    pxUIElement->Height = height;
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

#if PXOpenGLUSE
PXOpenGLDataType PXGraphicDataTypeToPXOpenGL(const PXColorFormat imageDataFormat)
{
    switch (imageDataFormat)
    {
        case PXColorFormatAlphaBinary:
        case PXColorFormatAlphaByte:
            return PXOpenGLTypeInvalid;

        case PXColorFormatRGBI8:
        case PXColorFormatRGBAI8:
        case PXColorFormatBGRI8:
        case PXColorFormatBGRAI8:
            return PXOpenGLTypeByteUnsigned;

        case PXColorFormatRGBI16:
        case PXColorFormatRGBAI16:
            return PXOpenGLTypeShortUnsigned;

        case PXColorFormatRGBF:
        case PXColorFormatRGBAF:
            return PXOpenGLTypeFloat;

        case PXColorFormatInvalid:
        default:
            return PXOpenGLTypeInvalid;
    }
}

PXOpenGLImageFormat PXGraphicImageFormatToPXOpenGL(const PXColorFormat imageDataFormat)
{
    switch (imageDataFormat)
    {
        case PXColorFormatRGBI8:
        case PXColorFormatRGBI16:
        case PXColorFormatRGBF:
            return PXOpenGLImageFormatRGB;

        case PXColorFormatRGBAI8:
        case PXColorFormatRGBAI16:
        case PXColorFormatRGBAF:
            return PXOpenGLImageFormatRGBA;

        case PXColorFormatBGRI8:
            return PXOpenGLImageFormatBGR;

        case PXColorFormatBGRAI8:
            return PXOpenGLImageFormatBGRA;

        case PXColorFormatInvalid:
        default:
            return PXOpenGLImageFormatInvalid;
    }
}

PXOpenGLShaderType PXGraphicShaderFromPXOpenGL(const PXShaderType shaderType)
{
    switch (shaderType)
    {
    default:
    case PXShaderTypeInvalid:
        return PXOpenGLShaderInvalid;

    case PXShaderTypeVertex:
        return PXOpenGLShaderVertex;

    case PXShaderTypeFragment:
        return PXOpenGLShaderFragment;

    case PXShaderTypeTessellationControl:
        return PXOpenGLShaderTessellationControl;

    case PXShaderTypeTessellationEvaluation:
        return PXOpenGLShaderTessellationEvaluation;

    case PXShaderTypeGeometry:
        return PXOpenGLShaderGeometry;

    case PXShaderTypeCompute:
        return PXOpenGLShaderCompute;
    }
}

/*

        PXOpenGLTextureTypeInvalid,
        PXOpenGLTextureType1D,
        PXOpenGLTextureType2D,
        PXOpenGLTextureType3D,
        PXOpenGLTextureType1DArray,
        PXOpenGLTextureType2DArray,
        PXOpenGLTextureTypeRectangle,
        PXOpenGLTextureTypeCubeMap,
        PXOpenGLTextureTypeCubleMapArray,
        PXOpenGLTextureTypeBuffer,
        PXOpenGLTextureType2DMultiSample,
        PXOpenGLTextureType2DMultiSampleArray,
*/

PXOpenGLTextureType ImageTypeGraphicToPXOpenGL(const PXGraphicImageType graphicImageType)
{
    switch (graphicImageType)
    {
        default:
        case PXGraphicImageTypeInvalid: return PXOpenGLTextureTypeInvalid;
        case PXGraphicImageTypeTexture2D: return PXOpenGLTextureType2D;
        case PXGraphicImageTypeTexture3D: return PXOpenGLTextureType3D;
        case PXGraphicImageTypeTextureCubeContainer: return PXOpenGLTextureTypeCubeMap;
        case PXGraphicImageTypeTextureCubeRight: return PXOpenGLTextureTypeCubeMapRight;
        case PXGraphicImageTypeTextureCubeLeft: return PXOpenGLTextureTypeCubeMapLeft;
        case PXGraphicImageTypeTextureCubeTop: return PXOpenGLTextureTypeCubeMapTop;
        case PXGraphicImageTypeTextureCubeDown: return PXOpenGLTextureTypeCubeMapBottom;
        case PXGraphicImageTypeTextureCubeBack: return PXOpenGLTextureTypeCubeMapBack;
        case PXGraphicImageTypeTextureCubeFront: return PXOpenGLTextureTypeCubeMapFront;
    }
}

PXOpenGLRenderMode PXGraphicRenderModeToPXOpenGL(const PXGraphicRenderMode graphicRenderMode)
{
      //  PXOpenGLRenderQuadStrip,
     //   PXOpenGLRenderPolygon

    switch (graphicRenderMode)
    {
        case PXGraphicRenderModePoint: return PXOpenGLRenderPoints;
        case PXGraphicRenderModeLine: return PXOpenGLRenderLines;
        case PXGraphicRenderModeLineLoop: return PXOpenGLRenderLineLoop;
        case PXGraphicRenderModeLineStrip: return PXOpenGLRenderLineStrip;
        //case PXGraphicRenderModeLineStripAdjacency: return xxxxx;
        //case PXGraphicRenderModeLineAdjacency: return xxxxx;
        case PXGraphicRenderModeTriangle: return PXOpenGLRenderTriangles;
        //case PXGraphicRenderModeTriangleAdjacency: return xxxxx;
        case PXGraphicRenderModeTriangleFAN: return PXOpenGLRenderTriangleFan;
        case PXGraphicRenderModeTriangleStrip: return PXOpenGLRenderTriangleStrip;
        //case PXGraphicRenderModeTriangleStripAdjacency: return xxxxx;
        //case PXGraphicRenderModeWireFrame: return xxxxx;
        case PXGraphicRenderModeSquare: return PXOpenGLRenderQuads;
        //case PXGraphicRenderModePatches: return xxxxx;
        default:
            return PXOpenGLRenderInvalid;
    }
}
#endif

void PXGraphicInstantiate(PXGraphicContext* const graphicContext)
{
    PXLockCreate(&graphicContext->_resourceLock, PXLockTypeGlobal);

    PXDictionaryConstruct(&graphicContext->UIElementLookUp, sizeof(PXInt32U), sizeof(PXUIElement), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->SpritelLookUp, sizeof(PXInt32U), sizeof(PXSprite), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->ModelLookUp, sizeof(PXInt32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->SoundLookup, sizeof(PXInt32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->ShaderPXProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);

#if PXWindowUSE
    PXWindow* const pxWindow = (PXWindow*)graphicContext->AttachedWindow;

#if PXOpenGLUSE
    graphicContext->OpenGLInstance.AttachedWindow = pxWindow;

    PXOpenGLCreateForWindow(&graphicContext->OpenGLInstance);

    if (1)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
    }

    if (1) // X-RAY
    {
        glEnable(GL_DEPTH_TEST);
    }

    if (1)
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_COLOR, GL_DST_COLOR);

         //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
         //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    }

    glViewport(0, 0, pxWindow->Width, pxWindow->Height);

    PXOpenGLDeselect(&graphicContext->OpenGLInstance);
#endif
#endif   
}

void PXGraphicResourceRegister(PXGraphicContext* const graphicContext, PXGraphicResourceInfo* const pxGraphicResourceInfo)
{
 
}

PXBool PXGraphicImageBufferSwap(PXGraphicContext* const graphicContext)
{
    PXWindow* window = (PXWindow*)graphicContext->AttachedWindow;

   // PXOpenGLContextFlush(&graphicContext->OpenGLInstance);

    return PXWindowFrameBufferSwap(window);
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

PXActionResult PXGraphicShaderProgramLoadGLSL(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderPXProgram, const PXText* const vertexShaderFilePath, const PXText* const fragmentShaderFilePath)
{
    PXShader vertexShader; PXShaderConstruct(&vertexShader);
    PXShader fragmentShader; PXShaderConstruct(&fragmentShader);
    PXFile vertexShaderFile; PXFileConstruct(&vertexShaderFile);
    PXFile fragmentFile; PXFileConstruct(&fragmentFile);


    //-----------------------------------------------------
    // Load shader Files
    //-----------------------------------------------------
    {
        PXFileOpenFromPathInfo pxFileOpenFromPathInfo;
        pxFileOpenFromPathInfo.AccessMode = PXMemoryAccessModeReadOnly;
        pxFileOpenFromPathInfo.MemoryCachingMode = PXMemoryCachingModeSequential;
        pxFileOpenFromPathInfo.AllowMapping = PXTrue;
        pxFileOpenFromPathInfo.CreateIfNotExist = PXFalse;
        pxFileOpenFromPathInfo.AllowOverrideOnCreate = PXFalse;

        {
            const PXBool isAlreadyLoaded = shaderPXProgram->ID != -1;
            const PXBool hasEmptyPaths = !vertexShaderFilePath || !fragmentShaderFilePath;

            if (isAlreadyLoaded)
            {
                return PXActionInvalid;
            }

            if (hasEmptyPaths)
            {
                return PXActionInvalid;
            }
        }

        {
            pxFileOpenFromPathInfo.Text = *vertexShaderFilePath;

            const PXActionResult actionResult = PXFileOpenFromPath(&vertexShaderFile, &pxFileOpenFromPathInfo);
            const PXBool sucessful = PXActionSuccessful == actionResult;

            if (!sucessful)
            {
                return actionResult;
            }

            PXShaderDataSet(&vertexShader, PXShaderTypeVertex, (const char*)vertexShaderFile.Data, vertexShaderFile.DataSize);
        }


        {
            pxFileOpenFromPathInfo.Text = *fragmentShaderFilePath;

            const PXActionResult actionResult = PXFileOpenFromPath(&fragmentFile, &pxFileOpenFromPathInfo);
            const PXBool successful = PXActionSuccessful == actionResult;

            if (!successful)
            {
                return actionResult;
            }

            PXShaderDataSet(&fragmentShader, PXShaderTypeFragment, (const char*)fragmentFile.Data, fragmentFile.DataSize);
        }
        //-----
    }

#if PXOpenGLUSE

    const PXSize shaderListSize = 2;
    PXShader* const shaderList[2] = { &vertexShader, &fragmentShader };
    const PXOpenGLID shaderPXProgrammID = PXOpenGLShaderProgramCreate(&graphicContext->OpenGLInstance);
    unsigned int  sucessfulCounter = 0;
    PXBool isValidShader = 1;

    for (PXSize i = 0; i < shaderListSize; ++i)
    {
        PXShader* const shader = shaderList[i];
        const PXOpenGLShaderType openGLShaderType = PXGraphicShaderFromPXOpenGL(shader->Type);
        const PXOpenGLShaderID shaderID = PXOpenGLShaderCreate(&graphicContext->OpenGLInstance, openGLShaderType);

        PXOpenGLShaderSource(&graphicContext->OpenGLInstance, shaderID, 1u, &shader->Content, &shader->ContentSize);

        const unsigned char compileSuccessful = PXOpenGLShaderCompile(&graphicContext->OpenGLInstance, shaderID);

        if (!compileSuccessful)
        {
            isValidShader = 0;
            break;
        }

        PXOpenGLShaderProgramAttach(&graphicContext->OpenGLInstance, shaderPXProgrammID, shaderID);

        shader->ID = shaderID;
    }

    if (isValidShader)
    {
        PXOpenGLShaderProgramLink(&graphicContext->OpenGLInstance, shaderPXProgrammID);
        PXOpenGLShaderProgramValidate(&graphicContext->OpenGLInstance, shaderPXProgrammID);

        shaderPXProgram->ID = shaderPXProgrammID;
    }

    // We used the Shaders above to compile, these elements are not used anymore.
    for (PXSize i = 0; i < shaderListSize; ++i)
    {
        PXShader* const shader = shaderList[i];
        const PXBool isLoaded = shader->ID != -1;

        if (isLoaded)
        {
            PXOpenGLShaderDelete(&graphicContext->OpenGLInstance, shader->ID);
        }
    }

    if (!isValidShader)
    {
        PXOpenGLShaderProgramDelete(&graphicContext->OpenGLInstance, shaderPXProgrammID);
    }

    if (!isValidShader)
    {
        return PXActionInvalid;
    }

    PXLockEngage(&graphicContext->_resourceLock);
    //shaderPXProgram->ID = PXGraphicGenerateUniqeID(graphicContext);
    PXDictionaryAdd(&graphicContext->ShaderPXProgramLookup, &shaderPXProgram->ID, shaderPXProgram);
    PXLockRelease(&graphicContext->_resourceLock);
#endif

    printf("[PX] Shader program ID:%i\n", shaderPXProgram->ID);

    PXFileDestruct(&vertexShaderFile);
    PXFileDestruct(&fragmentFile);
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