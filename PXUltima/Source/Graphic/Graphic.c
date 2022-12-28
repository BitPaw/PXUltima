#include "Graphic.h"

#include <Graphic/OpenGL/OpenGL.h>
#include <File/PXDataStream.h>
#include <OS/PXWindow.h>
#include <Format/PXFont.h>
#include <Container/ClusterValue.h>
#include <Text/Text.h>
#include <Format/FNT/FNT.h>

PXActionResult GraphicTextureScreenShot(GraphicContext* const graphicContext, Image* const image)
{
  //  OpenGLPixelDataRead(graphicContext, 0, 0, image->Width, image->Height, OpenGLImageFormatRGB, OpenGLTypeByteUnsigned, image->PixelData);

	return PXActionSuccessful;
}

PXActionResult GraphicTextureUse(GraphicContext* const graphicContext, PXTexture* const texture)
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

PXActionResult GraphicTextureRegisterA(GraphicContext* const graphicContext, PXTexture* const texture, const char* const filePath)
{
    // Load texture..
    {
        const PXActionResult loadResult = ImageLoadA(&texture->Image, filePath);

        PXActionExitOnError(loadResult);
    }

    // Register as normal 
    {
        const PXActionResult registerResult = GraphicTextureRegister(graphicContext, texture);

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


            GraphicTextureRegister(&_mainWindow.GraphicInstance, &texture);
        }

        return imageLoadResult;
    }

    */


    return PXActionSuccessful;
}

PXActionResult GraphicTextureRegisterW(GraphicContext* const graphicContext, PXTexture* const texture, const wchar_t* const filePath)
{
    // Load texture..
    {
        const PXActionResult loadResult = ImageLoadW(&texture->Image, filePath);

        PXActionExitOnError(loadResult);
    }

    // Register as normal 
    {
        const PXActionResult registerResult = GraphicTextureRegister(graphicContext, texture);

        PXActionExitOnError(registerResult);
    }

    return PXActionSuccessful;
}

PXActionResult GraphicTextureRegister(GraphicContext* const graphicContext, PXTexture* const texture)
{
    PXLockEngage(&graphicContext->_resourceLock);
    PXLinkedListFixedNodeAdd(&graphicContext->_textureList, texture);
    PXLockRelease(&graphicContext->_resourceLock);

    const OpenGLTextureType openGLTextureType = ImageTypeGraphicToOpenGL(texture->Type);
    const unsigned int openGLTextureTypeID = OpenGLTextureTypeToID(openGLTextureType);

    Image* const image = &texture->Image;

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
       // glTexParameteri(openGLTextureTypeID, GL_GENERATE_MIPMAP, GL_FALSE);
    }

    //glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    // ToDO: erro?
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    OpenGLTextureData2D(graphicContext, openGLTextureType, 0, OpenGLImageFormatRGBA, image->Width, image->Height, OpenGLImageFormatRGB, OpenGLTypeByteUnsigned, image->PixelData);

    // glTexImage2D(textureType, 0, GL_RGBA, image->Width, image->Height, 0, format, OpenGLTypeByteUnsigned, image->PixelData);

    //glGenerateMipmap(openGLTextureTypeID);


    OpenGLTextureBind(graphicContext, openGLTextureType, 0);

    return PXActionSuccessful;
}

PXActionResult GraphicTextureRelease(GraphicContext* const graphicContext, PXTexture* const texture)
{
    return PXActionInvalid;
}

PXActionResult GraphicTextureCubeRegister(GraphicContext* const graphicContext, PXTextureCube* const textureCube)
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
        const Image* const image = &textureCube->ImageList[i];
        const OpenGLTextureType textureTypeID = openGLTextureTypeList[i];
        const OpenGLImageFormat openGLImageFormat = GraphicImageFormatToOpenGL(image->Format);
        const OpenGLDataType openGLDataType = GraphicDataTypeToOpenGL(image->Format);        
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

PXActionResult GraphicTextureCubeRegisterUse(GraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    return PXActionInvalid;
}

PXActionResult GraphicTextureCubeRelease(GraphicContext* const graphicContext, PXTextureCube* const textureCube)
{
    return PXActionInvalid;
}

PXActionResult GraphicSkyboxRegister(GraphicContext* const graphicContext, PXSkyBox* const skyBox)
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

        ModelConstruct(&pxModel);
        
        pxModel.Data = bufferData;
        
        ModelSegmentsAdd(&pxModel, 3u, vertexDataSize, -1);       

        pxModel.DataVertexList = vertexData;
        pxModel.DataVertexListSize = vertexDataSize;

        pxModel.DataVertexWidth = 3u;
        pxModel.DataVertexSize = vertexDataSize;

        pxModel.IndexList = indexList;
        pxModel.DataIndexWidth = 4u;
        pxModel.DataIndexSize = indexListSize;

        GraphicModelRegisterFromModel(graphicContext, &skyBox->Renderable, &pxModel);
    }

    // Register Cube Texture
    GraphicTextureCubeRegister(graphicContext, &skyBox->TextureCube);

    graphicContext->_currentSkyBox = skyBox;

    return PXActionSuccessful;
}

PXActionResult GraphicSkyboxRegisterA
(
    GraphicContext* const graphicContext, 
    PXSkyBox** skyBox, 
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
    PXSkyBox* skyBoxCurrent = *skyBox;

    // Load Textures
    {
        const char* const filePathList[6] = { textureRight, textureLeft, textureTop, textureBottom, textureBack, textureFront };
        PXActionResult resultList[6];    

        {
            const PXBool hasSkybox = skyBoxCurrent;

            if (!hasSkybox)// Generate Skybox object 
            {


                *skyBox = 0;
            }
        }

        for (PXSize i = 0; i < 6u; ++i)
        {
            Image* const image = &skyBoxCurrent->TextureCube.ImageList[i];
            const char* const filePath = filePathList[i];
            const PXActionResult textureRightResult = ImageLoadA(image, filePath);

            resultList[i] = textureRightResult;
        }
    }     

    // Register skybox
    {
        const PXActionResult registerResult = GraphicSkyboxRegister(graphicContext, skyBoxCurrent);

    }

    // Register Shaders
    {
        ShaderProgram shaderProgram;
        shaderProgram.ID = -1;

        const PXActionResult shaderResult = GraphicShaderProgramCreateVFPathA(graphicContext, &shaderProgram, shaderVertex, shaderFragment);

        skyBoxCurrent->Renderable.MeshSegmentList[0].ShaderID = shaderProgram.ID;
    }

    return PXActionSuccessful;
}

PXActionResult GraphicSkyboxUse(GraphicContext* const graphicContext, PXSkyBox* const skyBox)
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

PXSize GraphicModelListSize(const GraphicContext* const graphicContext)
{


    return PXYes;
}

PXBool GraphicModelListGetFromIndex(const GraphicContext* const graphicContext, PXModel** pxModel, const PXSize index)
{
    return PXYes;
}

PXSize GraphicRenderableListSize(const GraphicContext* const graphicContext)
{       
    return graphicContext->_renderList.NodeListSizeCurrent;
}

PXBool GraphicRenderableListGetFromIndex(const GraphicContext* const graphicContext, PXRenderable** pxRenderable, const PXSize index)
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

PXActionResult GraphicModelCreate(GraphicContext* const graphicContext, PXModel** const pxModel)
{   
    PXModel* const model = (PXModel* const)MemoryAllocate(sizeof(PXModel) * 1u);

    if (!model)
    {
        return PXActionFailedAllocation;
    }

    ModelConstruct(model);

    GraphicModelRegister(graphicContext, model);

    *pxModel = model;

    return PXActionSuccessful;
}

PXBool GraphicModelRegister(GraphicContext* const graphicContext, PXModel* const pxModel)
{
    PXLockEngage(&graphicContext->_resourceLock);
    PXLinkedListFixedNodeAdd(&graphicContext->_pxModelList, pxModel);
    PXLockRelease(&graphicContext->_resourceLock);

    return PXTrue;
}

PXActionResult GraphicRenderableCreate(GraphicContext* const graphicContext, PXRenderable** const pxRenderable)
{
    PXRenderable* const renderable = (PXRenderable* const)MemoryAllocate(sizeof(PXRenderable) * 1u);

    if (!renderable)
    {
        return PXActionFailedAllocation;
    }

    MemoryClear(renderable, sizeof(PXRenderable));

    GraphicRenderableRegister(graphicContext, pxRenderable);  

    *pxRenderable = renderable;  

    return PXActionSuccessful;
}

PXBool GraphicRenderableRegister(GraphicContext* const graphicContext, PXRenderable* const pxRenderable)
{
    PXLockEngage(&graphicContext->_resourceLock);
    PXLinkedListFixedNodeAdd(&graphicContext->_renderList, pxRenderable);
    PXLockRelease(&graphicContext->_resourceLock);

    return PXTrue;
}

void GraphicModelShaderSet(GraphicContext* const graphicContext, PXRenderable* const renderable, const ShaderProgram* const shaderProgram)
{
    for (PXSize i = 0; i < renderable->MeshSegmentListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];

        pxRenderableMeshSegment->ShaderID = shaderProgram->ID;
    }  
}

PXActionResult GraphicModelRegisterA(GraphicContext* const graphicContext, PXRenderable** const renderable, const char* const filePath)
{
    PXModel* model = 0;
    PXRenderable* pxRenderable = 0;

    // Create model
    {
        const PXActionResult createResult = GraphicModelCreate(graphicContext, &model);
       
        PXActionExitOnError(createResult);       
    }

    // create PXRenderable
    {
        if (renderable) // Owner is caller
        {
            pxRenderable = *renderable;

            PXLockEngage(&graphicContext->_resourceLock);
            PXLinkedListFixedNodeAdd(&graphicContext->_renderList, pxRenderable);
            PXLockRelease(&graphicContext->_resourceLock);
        }
        else // No model, make one
        {
            const PXActionResult createResult = GraphicRenderableCreate(graphicContext, &pxRenderable);

            PXActionExitOnError(createResult);

            *renderable = pxRenderable; // Write to return back to caller
        }      
    }

    // Load model
    {
        const PXActionResult loadResult = ModelLoadA(model, filePath);

        PXActionExitOnError(loadResult);
    }

    // Register model into renderable
    {
        const PXActionResult loadResult = GraphicModelRegisterFromModel(graphicContext, pxRenderable, model);

        PXActionExitOnError(loadResult);
    }

    return PXActionSuccessful;
}

PXActionResult GraphicModelRegisterFromModel(GraphicContext* const graphicContext, PXRenderable* const renderable, const PXModel* const model)
{       
    OpenGLContext* const openGLContext = &graphicContext->OpenGLInstance;

    PXRenderableConstruct(renderable);   

    OpenGLVertexArrayGenerate(openGLContext, 1u, &renderable->VAO);

    OpenGLID vbo = 0;

    // Create VBO Buffers
    OpenGLBufferGenerate(openGLContext, 1u, &vbo);
    renderable->VBO = vbo;



    OpenGLVertexArrayBind(openGLContext, renderable->VAO);

    const PXSize stride = ModelVertexDataStride(model);
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
    const PXSize segmentsListSize = ModelSegmentsAmount(model);
    const PXSize modelListSize = PXModelMaterialAmount(model);

    renderable->MeshSegmentListSize = segmentsListSize;
    renderable->MeshSegmentList = (PXRenderableMeshSegment*)MemoryAllocate(sizeof(PXRenderableMeshSegment) * segmentsListSize);

    for (PXSize i = 0; i < segmentsListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];
        MeshSegment meshSegment;

        PXRenderableMeshSegmentConstruct(pxRenderableMeshSegment);
        ModelSegmentsGet(model, i, &meshSegment);    

        pxRenderableMeshSegment->RenderMode = GraphicRenderModeTriangle;
        pxRenderableMeshSegment->NumberOfVertices = meshSegment.DrawClusterSize;

        PXMaterial material;
        const PXBool fetchMaterialSuccess = PXModelMaterialGet(model, meshSegment.TextureID, &material);

        if (fetchMaterialSuccess)
        {
            PXTexture pxTexture;

            PXTextureConstruct(&pxTexture);

            pxTexture.Type = GraphicImageTypeTexture2D;
            pxTexture.Filter = GraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = GraphicImageLayoutNearest;
            pxTexture.LayoutFar = GraphicImageLayoutNearest;
            pxTexture.WrapHeight = GraphicImageWrapRepeat;
            pxTexture.WrapWidth = GraphicImageWrapRepeat;

            GraphicTextureRegisterA(graphicContext, &pxTexture, material.DiffuseTextureFilePath);

            pxRenderableMeshSegment->TextureID = pxTexture.ID;
        }
      
        pxRenderableMeshSegment->DoRendering = PXYes;
    }
    //-------------------------------------------------------------------------

    // Model is not fully registered and ready to be rendered
    renderable->DoRendering = 1u;

    return PXActionSuccessful;
}

PXActionResult GraphicModelRegisterFromData(GraphicContext* const graphicContext, PXRenderable* const renderable, const float* vertexData, const PXSize vertexDataSize, const unsigned int* indexList, const PXSize indexListSize)
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

PXActionResult GraphicUIRectangleCreate(GraphicContext* const graphicContext, PXRenderable* const renderable, const PXSize x, const PXSize y, const PXSize sidth, const PXSize height)
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

void PXShaderConstruct(Shader* const shader)
{
    MemoryClear(shader, sizeof(Shader));

    shader->ID = PXShaderNotRegisterd;
}

void ShaderDataSet(Shader* const shader, const ShaderType type, const char* data, const PXSize size)
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

PXActionResult GraphicUIPanelRegister(GraphicContext* const graphicContext, PXUIPanel* const pxUIPanel)
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
        const PXActionResult actionResult = GraphicModelRegisterFromModel(graphicContext, &pxUIPanel->Renderable, &model);

        PXActionExitOnError(actionResult);
    }
    
    GraphicRenderableRegister(graphicContext, &pxUIPanel->Renderable);

    return PXActionSuccessful;
}

PXActionResult GraphicUIPanelUpdate(GraphicContext* const graphicContext, PXUIPanel* const pxUIPanel)
{
    return PXActionInvalid;
}

PXActionResult GraphicUIPanelUnregister(GraphicContext* const graphicContext, PXUIPanel* const pxUIPanel)
{
    return PXActionInvalid;
}

PXActionResult GraphicUITextRegister(GraphicContext* const graphicContext, PXUIText* const pxUIText, const PXSize x, const PXSize y, const PXSize sidth, const PXSize height, const PXTextUTF8 text)
{
    const PXSize textSize = TextLengthA(text, 256);
    const PXSize vertexDataSize = textSize * 4u * (3u +2u);

    float* vertexData = MemoryAllocate(vertexDataSize * sizeof(float));

    PXSize index = 0;
    float xoffset = 0;    

    float imgwidth = pxUIText->TextFont->FontElement->FontPageList[0].FontTextureMap.Width;
    float imgheight = pxUIText->TextFont->FontElement->FontPageList[0].FontTextureMap.Height;

    for (size_t i = 0; i < textSize; i++)
    {
        char character = text[i];

        FNTCharacter* fntChar = FNTGetCharacter(pxUIText->TextFont->FontElement, character);

        float texturePositionX = fntChar->Position[0] / imgwidth;
        float texturePositionY = fntChar->Position[1] / imgheight;
        float texturePositionWidth = fntChar->Size[0] / imgwidth;
        float texturePositionHeight = fntChar->Size[1] / imgheight;

        vertexData[index++] = xoffset;
        vertexData[index++] = 0;
        vertexData[index++] = 0;

        vertexData[index++] = texturePositionX;
        vertexData[index++] = texturePositionY + texturePositionHeight;

        vertexData[index++] = fntChar->Size[0] + xoffset;
        vertexData[index++] = 0;
        vertexData[index++] = 0;

        vertexData[index++] = texturePositionX + texturePositionWidth;
        vertexData[index++] = texturePositionY + texturePositionHeight;

        vertexData[index++] = fntChar->Size[0] + xoffset;
        vertexData[index++] = fntChar->Size[1];
        vertexData[index++] = 0;

        vertexData[index++] = texturePositionX + texturePositionWidth;
        vertexData[index++] = texturePositionY;

        vertexData[index++] = xoffset;
        vertexData[index++] = fntChar->Size[1];
        vertexData[index++] = 0;

        vertexData[index++] = texturePositionX;
        vertexData[index++] = texturePositionY;

     

        xoffset += fntChar->XAdvance + 10;


        //vertexData[index++] = fntChar->Position[0];
        //vertexData[index++] = fntChar->Position[1];

        //vertexData[index++] = fntChar->Position[0];
        //vertexData[index++] = fntChar->Position[1];   
        //vertexData[index++] = fntChar->Position[0];
        //vertexData[index++] = fntChar->Position[1];

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
        const PXActionResult actionResult = GraphicModelRegisterFromModel(graphicContext, &pxUIText->Renderable, &model);

        PXActionExitOnError(actionResult);
    }

    GraphicRenderableRegister(graphicContext, &pxUIText->Renderable);



    PXTexture pxTexture;

    PXTextureConstruct(&pxTexture);

    MemoryCopy(&pxUIText->TextFont->FontElement[0].FontPageList[0].FontTextureMap, sizeof(Image), &pxTexture.Image,sizeof(Image));

    pxTexture.Type = GraphicImageTypeTexture2D;
    pxTexture.Filter = GraphicRenderFilterNoFilter;
    pxTexture.LayoutNear = GraphicImageLayoutNearest;
    pxTexture.LayoutFar = GraphicImageLayoutNearest;
    pxTexture.WrapHeight = GraphicImageWrapStrechEdges;
    pxTexture.WrapWidth = GraphicImageWrapStrechEdges;

    GraphicTextureRegister(graphicContext, &pxTexture, pxTexture);

    pxUIText->Renderable.MeshSegmentList[0].TextureID = pxTexture.ID;


    return PXActionSuccessful;




    /*
    //---------------------------------------
    unsigned int index = 0;

    // Vertex Position
    OpenGLVertexArrayAttributeDefine(&graphicContext->OpenGLInstance, index, 3, OpenGLTypeFloat, 0, stride, sizeof(float) * (0));
    OpenGLVertexArrayEnable(&graphicContext->OpenGLInstance, index++);
    
    // Color of vertex
    OpenGLVertexArrayAttributeDefine(&graphicContext->OpenGLInstance, index, 4, OpenGLTypeFloat, 0, stride, sizeof(float) * (3u));
    OpenGLVertexArrayEnable(&graphicContext->OpenGLInstance, index++);
    OpenGLVertexAttributeDivisor(&graphicContext->OpenGLInstance, 2, 1);

    // Size of character
    OpenGLVertexArrayAttributeDefine(&graphicContext->OpenGLInstance, index, 2, OpenGLTypeFloat, 0, stride, sizeof(float) * (3u + 4u));
    OpenGLVertexArrayEnable(&graphicContext->OpenGLInstance, index++);

    // Offset
    OpenGLVertexArrayAttributeDefine(&graphicContext->OpenGLInstance, index, 2, OpenGLTypeFloat, 0, stride, sizeof(float) * (3u + 4u + 2u));
    OpenGLVertexArrayEnable(&graphicContext->OpenGLInstance, index++);

    //---------------------------------------

    OpenGLBufferUnbind(&graphicContext->OpenGLInstance, OpenGLBufferArray);
    OpenGLVertexArrayUnbind(&graphicContext->OpenGLInstance);
    //-------------------------------------------------------------------------

    //---<Register all textures>-----------------------------------------------
    const PXSize segmentsListSize = ModelSegmentsAmount(model);
    const PXSize modelListSize = PXModelMaterialAmount(model);

    renderable->MeshSegmentListSize = segmentsListSize;
    renderable->MeshSegmentList = (PXRenderableMeshSegment*)MemoryAllocate(sizeof(PXRenderableMeshSegment) * segmentsListSize);

    for (PXSize i = 0; i < segmentsListSize; ++i)
    {
        PXRenderableMeshSegment* const pxRenderableMeshSegment = &renderable->MeshSegmentList[i];
        MeshSegment meshSegment;

        PXRenderableMeshSegmentConstruct(pxRenderableMeshSegment);
        ModelSegmentsGet(model, i, &meshSegment);

        pxRenderableMeshSegment->RenderMode = GraphicRenderModeTriangle;
        pxRenderableMeshSegment->NumberOfVertices = meshSegment.DrawClusterSize;

        PXMaterial material;
        const PXBool fetchMaterialSuccess = PXModelMaterialGet(model, meshSegment.TextureID, &material);

        if (fetchMaterialSuccess)
        {
            PXTexture pxTexture;

            PXTextureConstruct(&pxTexture);

            pxTexture.Type = GraphicImageTypeTexture2D;
            pxTexture.Filter = GraphicRenderFilterNoFilter;
            pxTexture.LayoutNear = GraphicImageLayoutNearest;
            pxTexture.LayoutFar = GraphicImageLayoutNearest;
            pxTexture.WrapHeight = GraphicImageWrapRepeat;
            pxTexture.WrapWidth = GraphicImageWrapRepeat;

            GraphicTextureRegisterA(graphicContext, &pxTexture, material.DiffuseTextureFilePath);

            pxRenderableMeshSegment->TextureID = pxTexture.ID;
        }

        pxRenderableMeshSegment->DoRendering = PXYes;
    }
    //-------------------------------------------------------------------------

    // Model is not fully registered and ready to be rendered
    renderable->DoRendering = 1u;

    //----------------------

    GraphicRenderableRegister(graphicContext, &pxUIText->Renderable);




    return PXActionInvalid;
        */
}

void PXRenderableMeshSegmentConstruct(PXRenderableMeshSegment* const pxRenderableMeshSegment)
{
    pxRenderableMeshSegment->NumberOfVertices = 0;
    pxRenderableMeshSegment->TextureID = (unsigned int)-1;
    pxRenderableMeshSegment->ShaderID = (unsigned int)-1;
    pxRenderableMeshSegment->RenderMode = GraphicRenderModeInvalid;
    pxRenderableMeshSegment->DoRendering = PXNo;
}

OpenGLDataType GraphicDataTypeToOpenGL(const ImageDataFormat imageDataFormat)
{
    switch (imageDataFormat)
    {       
        case ImageDataFormatAlphaMaskBinary: 
        case ImageDataFormatAlphaMask: 
            return OpenGLTypeInvalid;

        case ImageDataFormatRGB8:
        case ImageDataFormatRGBA8: 
        case ImageDataFormatBGR8: 
        case ImageDataFormatBGRA8: 
            return OpenGLTypeByteUnsigned;

        case ImageDataFormatRGB16:
        case ImageDataFormatRGBA16: 
            return OpenGLTypeShortUnsigned;

        case ImageDataFormatRGBFloat:
        case ImageDataFormatRGBAFloat: 
            return OpenGLTypeFloat;

        case ImageDataFormatInvalid:
        default:
            return OpenGLTypeInvalid;
    }
}

OpenGLImageFormat GraphicImageFormatToOpenGL(const ImageDataFormat imageDataFormat)
{
    switch (imageDataFormat)
    {               
        case ImageDataFormatRGB8:
        case ImageDataFormatRGB16:
        case ImageDataFormatRGBFloat:
            return OpenGLImageFormatRGB;

        case ImageDataFormatRGBA8:
        case ImageDataFormatRGBA16:
        case ImageDataFormatRGBAFloat:
            return OpenGLImageFormatRGBA;

        case ImageDataFormatBGR8:
            return OpenGLImageFormatBGR;

        case ImageDataFormatBGRA8:
            return OpenGLImageFormatBGRA;

        case ImageDataFormatInvalid:
        default:
            return OpenGLImageFormatInvalid;
    }
}

OpenGLShaderType GraphicShaderFromOpenGL(const ShaderType shaderType)
{
    switch (shaderType)
    {
    default:
    case ShaderTypeInvalid:
        return OpenGLShaderInvalid;

    case ShaderTypeVertex:
        return OpenGLShaderVertex;

    case ShaderTypeFragment:
        return OpenGLShaderFragment;

    case ShaderTypeTessellationControl:
        return OpenGLShaderTessellationControl;

    case ShaderTypeTessellationEvaluation:
        return OpenGLShaderTessellationEvaluation;

    case ShaderTypeGeometry:
        return OpenGLShaderGeometry;

    case ShaderTypeCompute:
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

OpenGLTextureType ImageTypeGraphicToOpenGL(const GraphicImageType graphicImageType)
{
    switch (graphicImageType)
    {
        default:
        case GraphicImageTypeInvalid: return OpenGLTextureTypeInvalid;
        case GraphicImageTypeTexture2D: return OpenGLTextureType2D;
        case GraphicImageTypeTexture3D: return OpenGLTextureType3D;
        case GraphicImageTypeTextureCubeContainer: return OpenGLTextureTypeCubeMap;
        case GraphicImageTypeTextureCubeRight: return OpenGLTextureTypeCubeMapRight;
        case GraphicImageTypeTextureCubeLeft: return OpenGLTextureTypeCubeMapLeft;
        case GraphicImageTypeTextureCubeTop: return OpenGLTextureTypeCubeMapTop;
        case GraphicImageTypeTextureCubeDown: return OpenGLTextureTypeCubeMapBottom;
        case GraphicImageTypeTextureCubeBack: return OpenGLTextureTypeCubeMapBack;
        case GraphicImageTypeTextureCubeFront: return OpenGLTextureTypeCubeMapFront;
    }
}

OpenGLRenderMode GraphicRenderModeToOpenGL(const GraphicRenderMode graphicRenderMode)
{
      //  OpenGLRenderQuadStrip,
     //   OpenGLRenderPolygon

    switch (graphicRenderMode)
    {
        case GraphicRenderModePoint: return OpenGLRenderPoints;
        case GraphicRenderModeLine: return OpenGLRenderLines;
        case GraphicRenderModeLineLoop: return OpenGLRenderLineLoop;
        case GraphicRenderModeLineStrip: return OpenGLRenderLineStrip;
        //case GraphicRenderModeLineStripAdjacency: return xxxxx;
        //case GraphicRenderModeLineAdjacency: return xxxxx;
        case GraphicRenderModeTriangle: return OpenGLRenderTriangles;
        //case GraphicRenderModeTriangleAdjacency: return xxxxx;
        case GraphicRenderModeTriangleFAN: return OpenGLRenderTriangleFan;
        case GraphicRenderModeTriangleStrip: return OpenGLRenderTriangleStrip;
        //case GraphicRenderModeTriangleStripAdjacency: return xxxxx;
        //case GraphicRenderModeWireFrame: return xxxxx;
        case GraphicRenderModeSquare: return OpenGLRenderQuads;
        //case GraphicRenderModePatches: return xxxxx;
        default:
            return OpenGLRenderInvalid;
    }
}

void GraphicInstantiate(GraphicContext* const graphicContext)
{
    PXLockCreate(&graphicContext->_resourceLock);

    char* memww = (char*)MemoryAllocate(12048); // TODO: Fix this

    PXLinkedListFixedNodeSet(&graphicContext->_renderList, memww, 100, PXLinkedListUseAdress);
    PXLinkedListFixedNodeSet(&graphicContext->_pxModelList, memww + 164, 100, PXLinkedListUseAdress);
    PXLinkedListFixedNodeSet(&graphicContext->_textureList, memww + 1128, 100, PXLinkedListUseAdress);
    PXLinkedListFixedNodeSet(&graphicContext->_fontList, memww + 1256, 100, PXLinkedListUseAdress);
    PXLinkedListFixedNodeSet(&graphicContext->_shaderProgramList, memww + 11024, 100, PXLinkedListUseAdress);



    graphicContext->OpenGLInstance.AttachedWindow = graphicContext->AttachedWindow;

    OpenGLContextCreate(&graphicContext->OpenGLInstance);

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

    OpenGLContextDeselect(&graphicContext->OpenGLInstance);
}

unsigned char GraphicImageBufferSwap(GraphicContext* const graphicContext)
{
    PXWindow* window = (PXWindow*)graphicContext->AttachedWindow;

   // OpenGLContextFlush(&graphicContext->OpenGLInstance);

    const unsigned char successful =
#if OSUnix
        1u; // No feedback?
    glXSwapBuffers(window->DisplayCurrent, window->ID);
#elif OSWindows
        SwapBuffers(window->HandleDeviceContext);
#endif

    return successful;
}

PXActionResult GraphicShaderProgramCreate(GraphicContext* const graphicContext)
{
    return PXActionInvalid;
}

PXActionResult GraphicShaderCompile(GraphicContext* const graphicContext)
{
    return PXActionInvalid;
}

PXActionResult GraphicShaderUse(GraphicContext* const graphicContext, const unsigned int shaderID)
{
    OpenGLShaderProgramUse(&graphicContext->OpenGLInstance, shaderID);

    return PXActionSuccessful;
}

PXActionResult GraphicShaderProgramCreateVFPathA(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, const char* vertexShaderFilePath, const char* fragmentShaderFilePath)
{
    Shader vertexShader; PXShaderConstruct(&vertexShader);
    Shader fragmentShader; PXShaderConstruct(&fragmentShader);
    PXDataStream vertexShaderFile; PXDataStreamConstruct(&vertexShaderFile);
    PXDataStream fragmentFile; PXDataStreamConstruct(&fragmentFile);  

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
        const PXActionResult actionResult = PXDataStreamMapToMemoryA(&vertexShaderFile, vertexShaderFilePath, 0, MemoryReadOnly);
        const PXBool sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        ShaderDataSet(&vertexShader, ShaderTypeVertex, vertexShaderFile.Data, vertexShaderFile.DataSize);
    }


    {
        const PXActionResult actionResult = PXDataStreamMapToMemoryA(&fragmentFile, fragmentShaderFilePath, 0, MemoryReadOnly);
        const PXBool sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        ShaderDataSet(&fragmentShader, ShaderTypeFragment, fragmentFile.Data, fragmentFile.DataSize);
    }
    //-----

    GraphicShaderProgramCreateVFData(graphicContext, shaderProgram, &vertexShader, &fragmentShader);


    PXDataStreamDestruct(&vertexShaderFile);
    PXDataStreamDestruct(&fragmentFile);
}

PXActionResult GraphicShaderProgramCreateVFPathW(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath)
{
    Shader vertexShader;
    Shader fragmentShader;
    PXDataStream vertexShaderFile;
    PXDataStream fragmentFile;

    PXDataStreamConstruct(&vertexShaderFile);
    PXDataStreamConstruct(&fragmentFile);

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
        const PXActionResult actionResult = PXDataStreamMapToMemoryW(&vertexShaderFile, vertexShaderFilePath, 0, MemoryReadOnly);
        const unsigned char sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        vertexShader.Type = ShaderTypeVertex;
        vertexShader.Content = (char*)vertexShaderFile.Data;
        vertexShader.ContentSize = vertexShaderFile.DataSize;
    }


    {
        const PXActionResult actionResult = PXDataStreamMapToMemoryW(&fragmentFile, fragmentShaderFilePath, 0, MemoryReadOnly);
        const unsigned char sucessful = PXActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        fragmentShader.Type = ShaderTypeFragment;
        fragmentShader.Content = (char*)fragmentFile.Data;
        fragmentShader.ContentSize = fragmentFile.DataSize;
    }
    //-----

    GraphicShaderProgramCreateVFData(graphicContext, shaderProgram, &vertexShader, &fragmentShader);


    PXDataStreamDestruct(&vertexShaderFile);
    PXDataStreamDestruct(&fragmentFile);
}

PXActionResult GraphicShaderProgramCreateVFData(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, Shader* vertexShader, Shader* fragmentShader)
{
    const PXSize shaderListSize = 2;
    Shader* const shaderList[2] = { vertexShader, fragmentShader };
    const OpenGLID shaderProgrammID = OpenGLShaderProgramCreate(&graphicContext->OpenGLInstance);
    unsigned int  sucessfulCounter = 0;
    unsigned char isValidShader = 1;

    for (PXSize i = 0; i < shaderListSize; ++i)
    {
        Shader* const shader = shaderList[i];
        const OpenGLShaderType openGLShaderType = GraphicShaderFromOpenGL(shader->Type);
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
        Shader* const shader = shaderList[i];
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
    PXLinkedListFixedNodeAdd(&graphicContext->_shaderProgramList, shaderProgram);
    PXLockRelease(&graphicContext->_resourceLock);

    return PXActionSuccessful;
}

PXActionResult GraphicRender(GraphicContext* const graphicContext, GraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}

void GraphicShaderUpdateMatrix4x4F(GraphicContext* const graphicContext, const unsigned int locationID, const float* const matrix4x4)
{   
    OpenGLShaderVariableMatrix4fv(&graphicContext->OpenGLInstance, locationID, 1, 0, matrix4x4);
}

unsigned int GraphicShaderVariableIDFetch(GraphicContext* const graphicContext, const unsigned int shaderID, const char* const name)
{
    const unsigned int locationID = OpenGLShaderVariableIDGet(&graphicContext->OpenGLInstance, shaderID, name);

    return locationID;
}

void GraphicShaderProgramUse(GraphicContext* const graphicContext, const unsigned int shaderID)
{
    OpenGLShaderProgramUse(&graphicContext->OpenGLInstance, shaderID);
}

PXActionResult GraphicRenderElement(GraphicContext* const graphicContext, GraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}

PXActionResult GraphicRenderList(GraphicContext* const graphicContext, GraphicRenderMode renderMode, PXSize start, PXSize amount)
{
    return PXActionInvalid;
}