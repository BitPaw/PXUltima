#include "PXGraphic.h"

#include <OS/File/PXFile.h>
#include <Media/PXText.h>
#include <OS/Graphic/OpenGL/OpenGL.h>
#include <OS/Window/PXWindow.h>

PXActionResult PXGraphicTextureScreenShot(PXGraphicContext* const graphicContext, PXImage* const image)
{
  //  OpenGLPixelDataRead(graphicContext, 0, 0, image->Width, image->Height, OpenGLImageFormatRGB, OpenGLTypeByteUnsigned, image->PixelData);

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

    const OpenGLID imageTypeID = ToImageType(imageType);

    glBindTexture(imageTypeID, textureID);
    */
    return PXActionInvalid;
}

PXActionResult PXGraphicTextureLoadA(PXGraphicContext* const graphicContext, PXTexture* const texture, const PXTextASCII filePath)
{
    texture->Type = PXGraphicImageTypeTexture2D;
    texture->Filter = PXGraphicRenderFilterNoFilter;
    texture->LayoutNear = PXGraphicImageLayoutNearest;
    texture->LayoutFar = PXGraphicImageLayoutNearest;
    texture->WrapHeight = PXGraphicImageWrapRepeat;
    texture->WrapWidth = PXGraphicImageWrapRepeat;


    // Load texture..
    {
        const PXActionResult loadResult = PXImageLoadA(&texture->Image, filePath);

        PXActionExitOnError(loadResult);
    }

    // Register as normal
    {
        const PXActionResult registerResult = PXGraphicTextureRegister(graphicContext, texture);

        PXActionExitOnError(registerResult);
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

PXActionResult PXGraphicTextureLoadW(PXGraphicContext* const graphicContext, PXTexture* const texture, const PXTextUNICODE filePath)
{
    // Load texture..
    {
        const PXActionResult loadResult = PXImageLoadW(&texture->Image, filePath);

        PXActionExitOnError(loadResult);
    }

    // Register as normal
    {
        const PXActionResult registerResult = PXGraphicTextureRegister(graphicContext, texture);

        PXActionExitOnError(registerResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicFontLoadA(PXGraphicContext* const graphicContext, PXFont* const pxFont, const PXTextASCII filePath)
{
    // Load texture
    { 
        const PXActionResult loadResult = PXFontLoadA(pxFont, filePath);

        PXActionExitOnError(loadResult);
    }

    // Register as normal
    {
        const PXActionResult registerResult = PXGraphicFontRegister(graphicContext, pxFont);

        PXActionExitOnError(registerResult);
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

    const OpenGLTextureType openGLTextureType = ImageTypeGraphicToOpenGL(texture->Type);
    const unsigned int openGLTextureTypeID = OpenGLTextureTypeToID(openGLTextureType);

    PXImage* const image = &texture->Image;

    if (!image->PixelData)
    {
        return PXActionRefuedInputBufferEmpty; // No image data
    }

    // Register and select
    {
        OpenGLID textureID = 0;

        OpenGLTextureCreate(graphicContext, 1u, &textureID);

        const PXBool registerSuccess = textureID != -1;

        if (!registerSuccess)
        {
            return PXActionFailedResourceRegister;
        }

        texture->ID = textureID;
    }

    OpenGLTextureBind(graphicContext, openGLTextureType, texture->ID);

    // Texture Style
    {
        //const int textureWrapWidth = OpenGLToImageWrap(texture->WrapWidth);
        //onst int textureWrapHeight = OpenGLToImageWrap(texture->WrapHeight);
        //const int textueFilterNear = OpenGLToImageLayout(texture->LayoutNear);
       // const int textueFilterFar = OpenGLToImageLayout(texture->LayoutFar);

        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Remember! This stuff is required for some reason, its not optional!
        glTexParameteri(openGLTextureTypeID, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // if not done, textures might be black.
        //glTexParameteri(openGLTextureTypeID, GL_GENERATE_MIPMAP, GL_FALSE);
    }

    //glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    // ToDO: erro?
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    OpenGLTextureData2D(graphicContext, openGLTextureType, 0, OpenGLImageFormatRGBA, image->Width, image->Height, OpenGLImageFormatRGBA, OpenGLTypeByteUnsigned, image->PixelData);

    // glTexImage2D(textureType, 0, GL_RGBA, image->Width, image->Height, 0, format, OpenGLTypeByteUnsigned, image->PixelData);

    //glGenerateMipmap(openGLTextureTypeID);

    OpenGLTextureUnbind(graphicContext, openGLTextureType);

    return PXActionSuccessful;
}

PXActionResult PXGraphicTextureRelease(PXGraphicContext* const graphicContext, PXTexture* const texture)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicTextureCubeRegister(PXGraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    OpenGLID textureID = -1;

    // Check
    {
        const unsigned char isValid = 1;// textureCube.HasTextures();

        if (!isValid)
        {
            return PXActionInvalid;
        }
    }

    OpenGLTextureCreate(&graphicContext->OpenGLInstance, 1u, &textureID);
    OpenGLTextureBind(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, textureID);

    OpenGLTextureParameter(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureWRAP_S, OpenGLTextureParameterValueClampToEdge);
    OpenGLTextureParameter(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureWRAP_T, OpenGLTextureParameterValueClampToEdge);
    OpenGLTextureParameter(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureWRAP_R, OpenGLTextureParameterValueClampToEdge);
    OpenGLTextureParameter(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureMAG_FILTER, OpenGLTextureParameterValueLINEAR);
    OpenGLTextureParameter(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureMIN_FILTER, OpenGLTextureParameterValueLINEAR);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureBASE_LEVEL, 0);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureMAX_LEVEL, 0);


    OpenGLSettingChange(&graphicContext->OpenGLInstance, OpenGLToggleTextureCubeMapSeamless, PXTrue);

    const OpenGLTextureType openGLTextureTypeList[6] =
    {
        OpenGLTextureTypeCubeMapTop,
        OpenGLTextureTypeCubeMapBottom,
        OpenGLTextureTypeCubeMapFront,
        OpenGLTextureTypeCubeMapBack,
        OpenGLTextureTypeCubeMapLeft,
        OpenGLTextureTypeCubeMapRight
    };

    for (PXSize i = 0; i < 6u; ++i)
    {
        const PXImage* const image = &textureCube->ImageList[i];
        const OpenGLTextureType textureTypeID = openGLTextureTypeList[i];
        const OpenGLImageFormat openGLImageFormat = PXGraphicImageFormatToOpenGL(image->Format);
        const OpenGLDataType openGLDataType = PXGraphicDataTypeToOpenGL(image->Format);
        const int levelOfDetail = 0;

        OpenGLTextureData2D
        (
            &graphicContext->OpenGLInstance,
            textureTypeID,
            levelOfDetail,
            OpenGLImageFormatRGB,
            image->Width,
            image->Height,
            openGLImageFormat,
            openGLDataType,
            image->PixelData
        );
    }

    OpenGLTextureUnbind(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap);

    textureCube->ID = textureID;

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

        pxModel.DataVertexList = vertexData;
        pxModel.DataVertexListSize = vertexDataSize;

        pxModel.DataVertexWidth = 3u;
        pxModel.DataVertexSize = vertexDataSize;

        pxModel.IndexList = indexList;
        pxModel.DataIndexWidth = 4u;
        pxModel.DataIndexSize = indexListSize;

        PXGraphicModelRegisterFromModel(graphicContext, &skyBox->Renderable, &pxModel);
    }

    // Register Cube Texture
    PXGraphicTextureCubeRegister(graphicContext, &skyBox->TextureCube);

    graphicContext->_currentSkyBox = skyBox;

    return PXActionSuccessful;
}

PXActionResult PXGraphicSkyboxRegisterA
(
    PXGraphicContext* const graphicContext,
    PXSkyBox* const skyBox,
    const char* shaderVertex,
    const char* shaderFragment,
    const char* textureRight,
    const char* textureLeft,
    const char* textureTop,
    const char* textureBottom,
    const char* textureBack,
    const char* textureFront
)
{
    // Load Textures
    {
        const char* const filePathList[6] = { textureRight, textureLeft, textureTop, textureBottom, textureBack, textureFront };
        PXActionResult resultList[6];

        for (PXSize i = 0; i < 6u; ++i)
        {
            PXImage* const image = &skyBox->TextureCube.ImageList[i];
            const char* const filePath = filePathList[i];
            const PXActionResult textureRightResult = PXImageLoadA(image, filePath);

            resultList[i] = textureRightResult;
        }
    }

    // Register skybox
    {
        const PXActionResult registerResult = PXGraphicSkyboxRegister(graphicContext, skyBox);

    }

    // Register Shaders
    {
        PXShaderProgram shaderProgram;
        shaderProgram.ID = -1;

        const PXActionResult shaderResult = PXGraphicShaderProgramLoadGLSLA(graphicContext, &shaderProgram, shaderVertex, shaderFragment);

        skyBox->Renderable.MeshSegmentList[0].ShaderID = shaderProgram.ID;
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicSkyboxUse(PXGraphicContext* const graphicContext, PXSkyBox* const skyBox)
{
    /*
    // TODO:TEST REMOVAL !!!    OpenGL::VertexArrayBind(skyBox.RenderInfo.VAO);

    OpenGLID skyBoxTextureLocation = OpenGL::ShaderGetUniformLocationID(skyBox.Shader.ID, "SkyBoxTexture");

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
        const PXRenderable* const renderableCurrent = currentModel.BlockData;

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
    PXModel* const model = (PXModel* const)MemoryAllocate(sizeof(PXModel) * 1u);

    if (!model)
    {
        return PXActionFailedAllocation;
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
    PXRenderable* const renderable = (PXRenderable* const)MemoryAllocate(sizeof(PXRenderable) * 1u);

    if (!renderable)
    {
        return PXActionFailedAllocation;
    }

    MemoryClear(renderable, sizeof(PXRenderable));

    PXGraphicRenderableRegister(graphicContext, pxRenderable);

    *pxRenderable = renderable;

    return PXActionSuccessful;
}

PXBool PXGraphicRenderableRegister(PXGraphicContext* const graphicContext, PXRenderable* const pxRenderable)
{
    PXLockEngage(&graphicContext->_resourceLock);
    PXLinkedListFixedNodeAdd(&graphicContext->_renderList, pxRenderable);
    PXLockRelease(&graphicContext->_resourceLock);

    return PXTrue;
}

void PXGraphicModelShaderSet(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXShaderProgram* const shaderProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = shaderProgram->ID;
    }
}

PXActionResult PXGraphicModelLoadA(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXTextASCII filePath)
{
    PXModel* model = 0;

    // Create model
    {
        const PXActionResult createResult = PXGraphicModelCreate(graphicContext, &model);

        PXActionExitOnError(createResult);
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
        const PXActionResult loadResult = PXModelLoadA(model, filePath);

        PXActionExitOnError(loadResult);
    }

    // Register model into renderable
    {
        const PXActionResult loadResult = PXGraphicModelRegisterFromModel(graphicContext, renderable, model);

        PXActionExitOnError(loadResult);
    }

    return PXActionSuccessful;
}

PXActionResult PXGraphicModelRegisterFromModel(PXGraphicContext* const graphicContext, PXRenderable* const renderable, const PXModel* const model)
{
    OpenGLContext* const openGLContext = &graphicContext->OpenGLInstance;

    PXRenderableConstruct(renderable);

    OpenGLVertexArrayGenerate(openGLContext, 1u, &renderable->VAO);

    OpenGLID vbo = 0;

    // Create VBO Buffers
    OpenGLBufferGenerate(openGLContext, 1u, &vbo);
    renderable->VBO = vbo;



    OpenGLVertexArrayBind(openGLContext, renderable->VAO);

    const PXSize stride = PXModelVertexDataStride(model);
    const PXSize vertexListSize = model->DataVertexListSize * stride;

    OpenGLBufferBind(openGLContext, OpenGLBufferArray, renderable->VBO);
    OpenGLBufferData(openGLContext, OpenGLBufferArray, vertexListSize, model->DataVertexList, OpenGLStoreStaticDraw);

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
        OpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataVertexWidth, OpenGLTypeFloat, 0, stride, offset);
        OpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataVertexWidth * sizeof(float);
    }

    if (model->DataNormalWidth)
    {
        OpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataNormalWidth, OpenGLTypeFloat, 0, stride, offset);
        OpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataNormalWidth * sizeof(float);
    }

    if (model->DataTextureWidth)
    {
        OpenGLVertexArrayAttributeDefine(openGLContext, index, model->DataTextureWidth, OpenGLTypeFloat, 0, stride, offset);
        OpenGLVertexArrayEnable(openGLContext, index++);

        offset += model->DataTextureWidth * sizeof(float);
    }

    // Color?

    OpenGLBufferUnbind(openGLContext, OpenGLBufferArray);

    if (model->DataIndexWidth)
    {
        OpenGLBufferGenerate(openGLContext, 1u, &renderable->IBO);

        OpenGLBufferBind(openGLContext, OpenGLBufferElementArray, renderable->IBO);

        OpenGLBufferData(openGLContext, OpenGLBufferElementArray, sizeof(unsigned int) * model->DataIndexSize, model->IndexList, OpenGLStoreStaticDraw);

        OpenGLBufferUnbind(openGLContext, OpenGLBufferElementArray);
    }

    OpenGLVertexArrayUnbind(openGLContext);
    //-------------------------------------------------------------------------

    //---<Register all textures>-----------------------------------------------
    const PXSize segmentsListSize = PXModelSegmentsAmount(model);
    const PXSize modelListSize = PXModelMaterialAmount(model);

    renderable->MeshSegmentListSize = segmentsListSize;
    renderable->MeshSegmentList = (PXRenderableMeshSegment*)MemoryAllocate(sizeof(PXRenderableMeshSegment) * segmentsListSize);

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

            PXGraphicTextureLoadA(graphicContext, &pxTexture, material.DiffuseTextureFilePath);

            pxRenderableMeshSegment->TextureID = pxTexture.ID;
        }

        pxRenderableMeshSegment->DoRendering = PXYes;
    }
    //-------------------------------------------------------------------------

    // Model is not fully registered and ready to be rendered
    renderable->DoRendering = 1u;

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

    OpenGLID id[3] = { (unsigned int)-1,(unsigned int)-1,(unsigned int)-1 };

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
    MemoryClear(shader, sizeof(PXShader));

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
    MemoryClear(texture, sizeof(PXTexture));
}

void PXTextureDestruct(PXTexture* const texture)
{

}

void PXRenderableConstruct(PXRenderable* const pxRenderable)
{
    MemoryClear(pxRenderable, sizeof(PXRenderable));

    PXMatrix4x4FIdentity(&pxRenderable->MatrixModel);

    pxRenderable->VAO = -1;
    pxRenderable->VBO = -1;
    pxRenderable->IBO = -1;
}

void PXUIElementConstruct(PXUIElement* const pxUIElement)
{
    MemoryClear(pxUIElement, sizeof(PXUIElement));

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

OpenGLDataType PXGraphicDataTypeToOpenGL(const PXColorFormat imageDataFormat)
{
    switch (imageDataFormat)
    {
        case PXColorFormatAlphaBinary:
        case PXColorFormatAlphaByte:
            return OpenGLTypeInvalid;

        case PXColorFormatRGBI8:
        case PXColorFormatBGRI8:
            return OpenGLTypeByteUnsigned;

        case PXColorFormatRGBI16:
        case PXColorFormatRGBAI16:
            return OpenGLTypeShortUnsigned;

        case PXColorFormatRGBF:
        case PXColorFormatRGBAF:
            return OpenGLTypeFloat;

        case PXColorFormatInvalid:
        default:
            return OpenGLTypeInvalid;
    }
}

OpenGLImageFormat PXGraphicImageFormatToOpenGL(const PXColorFormat imageDataFormat)
{
    switch (imageDataFormat)
    {
        case PXColorFormatRGBI8:
        case PXColorFormatRGBI16:
        case PXColorFormatRGBF:
            return OpenGLImageFormatRGB;

        case PXColorFormatRGBAI8:
        case PXColorFormatRGBAI16:
        case PXColorFormatRGBAF:
            return OpenGLImageFormatRGBA;

        case PXColorFormatBGRI8:
            return OpenGLImageFormatBGR;

        case PXColorFormatBGRAI8:
            return OpenGLImageFormatBGRA;

        case PXColorFormatInvalid:
        default:
            return OpenGLImageFormatInvalid;
    }
}

OpenGLShaderType PXGraphicShaderFromOpenGL(const PXShaderType shaderType)
{
    switch (shaderType)
    {
    default:
    case PXShaderTypeInvalid:
        return OpenGLShaderInvalid;

    case PXShaderTypeVertex:
        return OpenGLShaderVertex;

    case PXShaderTypeFragment:
        return OpenGLShaderFragment;

    case PXShaderTypeTessellationControl:
        return OpenGLShaderTessellationControl;

    case PXShaderTypeTessellationEvaluation:
        return OpenGLShaderTessellationEvaluation;

    case PXShaderTypeGeometry:
        return OpenGLShaderGeometry;

    case PXShaderTypeCompute:
        return OpenGLShaderCompute;
    }
}

/*

        OpenGLTextureTypeInvalid,
        OpenGLTextureType1D,
        OpenGLTextureType2D,
        OpenGLTextureType3D,
        OpenGLTextureType1DArray,
        OpenGLTextureType2DArray,
        OpenGLTextureTypeRectangle,
        OpenGLTextureTypeCubeMap,
        OpenGLTextureTypeCubleMapArray,
        OpenGLTextureTypeBuffer,
        OpenGLTextureType2DMultiSample,
        OpenGLTextureType2DMultiSampleArray,
*/

OpenGLTextureType ImageTypeGraphicToOpenGL(const PXGraphicImageType graphicImageType)
{
    switch (graphicImageType)
    {
        default:
        case PXGraphicImageTypeInvalid: return OpenGLTextureTypeInvalid;
        case PXGraphicImageTypeTexture2D: return OpenGLTextureType2D;
        case PXGraphicImageTypeTexture3D: return OpenGLTextureType3D;
        case PXGraphicImageTypeTextureCubeContainer: return OpenGLTextureTypeCubeMap;
        case PXGraphicImageTypeTextureCubeRight: return OpenGLTextureTypeCubeMapRight;
        case PXGraphicImageTypeTextureCubeLeft: return OpenGLTextureTypeCubeMapLeft;
        case PXGraphicImageTypeTextureCubeTop: return OpenGLTextureTypeCubeMapTop;
        case PXGraphicImageTypeTextureCubeDown: return OpenGLTextureTypeCubeMapBottom;
        case PXGraphicImageTypeTextureCubeBack: return OpenGLTextureTypeCubeMapBack;
        case PXGraphicImageTypeTextureCubeFront: return OpenGLTextureTypeCubeMapFront;
    }
}

OpenGLRenderMode PXGraphicRenderModeToOpenGL(const PXGraphicRenderMode graphicRenderMode)
{
      //  OpenGLRenderQuadStrip,
     //   OpenGLRenderPolygon

    switch (graphicRenderMode)
    {
        case PXGraphicRenderModePoint: return OpenGLRenderPoints;
        case PXGraphicRenderModeLine: return OpenGLRenderLines;
        case PXGraphicRenderModeLineLoop: return OpenGLRenderLineLoop;
        case PXGraphicRenderModeLineStrip: return OpenGLRenderLineStrip;
        //case PXGraphicRenderModeLineStripAdjacency: return xxxxx;
        //case PXGraphicRenderModeLineAdjacency: return xxxxx;
        case PXGraphicRenderModeTriangle: return OpenGLRenderTriangles;
        //case PXGraphicRenderModeTriangleAdjacency: return xxxxx;
        case PXGraphicRenderModeTriangleFAN: return OpenGLRenderTriangleFan;
        case PXGraphicRenderModeTriangleStrip: return OpenGLRenderTriangleStrip;
        //case PXGraphicRenderModeTriangleStripAdjacency: return xxxxx;
        //case PXGraphicRenderModeWireFrame: return xxxxx;
        case PXGraphicRenderModeSquare: return OpenGLRenderQuads;
        //case PXGraphicRenderModePatches: return xxxxx;
        default:
            return OpenGLRenderInvalid;
    }
}

void PXGraphicInstantiate(PXGraphicContext* const graphicContext)
{
    PXWindow* const pxWindow = graphicContext->AttachedWindow;

    PXLockCreate(&graphicContext->_resourceLock, PXLockTypeGlobal);

    PXDictionaryConstruct(&graphicContext->UIElementLookUp, sizeof(PXInt32U), sizeof(PXUIElement), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->TextureLookUp, sizeof(PXInt32U), sizeof(PXTexture), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->ModelLookUp, sizeof(PXInt32U), sizeof(PXModel), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->FontLookUp, sizeof(PXInt32U), sizeof(PXFont), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->SoundLookup, sizeof(PXInt32U), sizeof(PXSound), PXDictionaryValueLocalityExternalReference);
    PXDictionaryConstruct(&graphicContext->ShaderProgramLookup, sizeof(PXInt32U), sizeof(PXShaderProgram), PXDictionaryValueLocalityExternalReference);

    graphicContext->OpenGLInstance.AttachedWindow = pxWindow;

    OpenGLContextCreateForWindow(&graphicContext->OpenGLInstance);

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

    OpenGLContextDeselect(&graphicContext->OpenGLInstance);
}

void PXGraphicResourceRegister(PXGraphicContext* const graphicContext, PXGraphicResourceInfo* const pxGraphicResourceInfo)
{
 
}

PXBool PXGraphicImageBufferSwap(PXGraphicContext* const graphicContext)
{
    PXWindow* window = (PXWindow*)graphicContext->AttachedWindow;

   // OpenGLContextFlush(&graphicContext->OpenGLInstance);

    const PXBool successful =
#if OSUnix
        1u; // No feedback?
    glXSwapBuffers(window->DisplayCurrent, window->ID);
#elif OSWindows
        SwapBuffers(window->HandleDeviceContext);
#endif

    return successful;
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
    OpenGLShaderProgramUse(&graphicContext->OpenGLInstance, shaderID);

    return PXActionSuccessful;
}

PXActionResult PXGraphicShaderProgramLoadGLSLA(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderProgram, const PXTextASCII vertexShaderFilePath, const PXTextASCII fragmentShaderFilePath)
{
    PXShader vertexShader; PXShaderConstruct(&vertexShader);
    PXShader fragmentShader; PXShaderConstruct(&fragmentShader);
    PXFile vertexShaderFile; PXFileConstruct(&vertexShaderFile);
    PXFile fragmentFile; PXFileConstruct(&fragmentFile);

    {
        const PXBool isAlreadyLoaded = shaderProgram->ID != -1;
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
        const PXActionResult actionResult = PXFileMapToMemoryA(&vertexShaderFile, vertexShaderFilePath, 0, PXMemoryAccessModeReadOnly);
        const PXBool sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        PXShaderDataSet(&vertexShader, PXShaderTypeVertex, vertexShaderFile.Data, vertexShaderFile.DataSize);
    }


    {
        const PXActionResult actionResult = PXFileMapToMemoryA(&fragmentFile, fragmentShaderFilePath, 0, PXMemoryAccessModeReadOnly);
        const PXBool successful = PXActionSuccessful == actionResult;

        if (!successful)
        {
            return actionResult;
        }

        PXShaderDataSet(&fragmentShader, PXShaderTypeFragment, fragmentFile.Data, fragmentFile.DataSize);
    }
    //-----

    PXGraphicShaderProgramLoadGLSL(graphicContext, shaderProgram, &vertexShader, &fragmentShader);


    PXFileDestruct(&vertexShaderFile);
    PXFileDestruct(&fragmentFile);
}

PXActionResult PXGraphicShaderProgramLoadGLSLW(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderProgram, const PXTextUNICODE vertexShaderFilePath, const PXTextUNICODE fragmentShaderFilePath)
{
    PXShader vertexShader;
    PXShader fragmentShader;
    PXFile vertexShaderFile;
    PXFile fragmentFile;

    PXFileConstruct(&vertexShaderFile);
    PXFileConstruct(&fragmentFile);

    {
        const unsigned char isAlreadyLoaded = shaderProgram->ID != -1;
        const unsigned char hasEmptyPaths = !vertexShaderFilePath || !fragmentShaderFilePath;

        if (isAlreadyLoaded)
        {
            return PXActionSuccessful;
        }

        if (hasEmptyPaths)
        {
            return PXActionSuccessful;
        }
    }

    {
        const PXActionResult actionResult = PXFileMapToMemoryW(&vertexShaderFile, vertexShaderFilePath, 0, PXMemoryAccessModeReadOnly);
        const unsigned char sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        vertexShader.Type = PXShaderTypeVertex;
        vertexShader.Content = (char*)vertexShaderFile.Data;
        vertexShader.ContentSize = vertexShaderFile.DataSize;
    }


    {
        const PXActionResult actionResult = PXFileMapToMemoryW(&fragmentFile, fragmentShaderFilePath, 0, PXMemoryAccessModeReadOnly);
        const unsigned char sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        fragmentShader.Type = PXShaderTypeFragment;
        fragmentShader.Content = (char*)fragmentFile.Data;
        fragmentShader.ContentSize = fragmentFile.DataSize;
    }
    //-----

    PXGraphicShaderProgramLoadGLSL(graphicContext, shaderProgram, &vertexShader, &fragmentShader);


    PXFileDestruct(&vertexShaderFile);
    PXFileDestruct(&fragmentFile);
}

PXActionResult PXGraphicShaderProgramLoadGLSL(PXGraphicContext* const graphicContext, PXShaderProgram* const shaderProgram, PXShader* const vertexShader, PXShader* const fragmentShader)
{
    const PXSize shaderListSize = 2;
    PXShader* const shaderList[2] = { vertexShader, fragmentShader };
    const OpenGLID shaderProgrammID = OpenGLShaderProgramCreate(&graphicContext->OpenGLInstance);
    unsigned int  sucessfulCounter = 0;
    unsigned char isValidShader = 1;

    for (PXSize i = 0; i < shaderListSize; ++i)
    {
        PXShader* const shader = shaderList[i];
        const OpenGLShaderType openGLShaderType = PXGraphicShaderFromOpenGL(shader->Type);
        const unsigned int shaderID = OpenGLShaderCreate(&graphicContext->OpenGLInstance, openGLShaderType);

        OpenGLShaderSource(&graphicContext->OpenGLInstance, shaderID, 1u, &shader->Content, &shader->ContentSize);

        const unsigned char compileSuccessful = OpenGLShaderCompile(&graphicContext->OpenGLInstance, shaderID);

        if (!compileSuccessful)
        {
            isValidShader = 0;
            break;
        }

        OpenGLShaderProgramAttach(&graphicContext->OpenGLInstance, shaderProgrammID, shaderID);

        shader->ID = shaderID;
    }

    if (isValidShader)
    {
        OpenGLShaderProgramLink(&graphicContext->OpenGLInstance, shaderProgrammID);
        OpenGLShaderProgramValidate(&graphicContext->OpenGLInstance, shaderProgrammID);

        shaderProgram->ID = shaderProgrammID;
    }

    // We used the Shaders above to compile, these elements are not used anymore.
    for (PXSize i = 0; i < shaderListSize; ++i)
    {
        PXShader* const shader = shaderList[i];
        const unsigned char isLoaded = shader->ID != -1;

        if (isLoaded)
        {
            OpenGLShaderDelete(&graphicContext->OpenGLInstance, shader->ID);
        }
    }

    if (!isValidShader)
    {
        OpenGLShaderProgramDelete(&graphicContext->OpenGLInstance, shaderProgrammID);
    }

    if (!isValidShader)
    {
        return PXActionInvalid;
    }

    PXLockEngage(&graphicContext->_resourceLock);
    shaderProgram->ID = PXGraphicGenerateUniqeID(graphicContext);
    PXDictionaryAdd(&graphicContext->ShaderProgramLookup, shaderProgram->ID, shaderProgram);
    PXLockRelease(&graphicContext->_resourceLock);

    return PXActionSuccessful;
}

PXActionResult PXGraphicRender(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}

void PXGraphicShaderUpdateMatrix4x4F(PXGraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4)
{
    OpenGLShaderVariableMatrix4fv(&graphicContext->OpenGLInstance, locationID, 1, 0, matrix4x4);
}

unsigned int PXGraphicShaderVariableIDFetch(PXGraphicContext* const graphicContext, const unsigned int shaderID, const char* const name)
{
    const unsigned int locationID = OpenGLShaderVariableIDGet(&graphicContext->OpenGLInstance, shaderID, name);

    return locationID;
}

void PXGraphicShaderProgramUse(PXGraphicContext* const graphicContext, const unsigned int shaderID)
{
    OpenGLShaderProgramUse(&graphicContext->OpenGLInstance, shaderID);
}

PXActionResult PXGraphicRenderElement(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}

PXActionResult PXGraphicRenderList(PXGraphicContext* const graphicContext, PXGraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}
