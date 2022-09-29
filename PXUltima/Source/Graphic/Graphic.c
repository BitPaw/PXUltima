#include "Graphic.h"

#include <Graphic/OpenGL/OpenGL.h>
#include <File/DataStream.h>
#include <OS/PXWindow.h>

ActionResult GraphicRegisterTexture(GraphicContext* const graphicContext, CTexture* const texture)
{
    const OpenGLTextureType openGLTextureType = ImageTypeGraphicToOpenGL(texture->Type);

    Image* image = &texture->Image;

    if (!image->PixelData)
    {
        return; // No image data
    }

    // Register and select
    {
        OpenGLID textureID = 0;

        OpenGLTextureCreate(graphicContext, 1u, &textureID);

        if (textureID == -1)
        {
            return ActionInvalid;
        }

        OpenGLTextureBind(graphicContext, openGLTextureType, textureID);

        texture->ID = textureID;
    }

    // Texture Style
    {/*
        const int textureWrapWidth = OpenGLToImageWrap(texture->WrapWidth);
        const int textureWrapHeight = OpenGLToImageWrap(texture->WrapHeight);
        const int textueFilterNear = OpenGLToImageLayout(texture->LayoutNear);
        const int textueFilterFar = OpenGLToImageLayout(texture->LayoutFar);

        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, textureWrapWidth);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, textureWrapHeight);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, textueFilterNear);
       // glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, textueFilterFar);
       // glTexParameteri(textureType, GL_GENERATE_MIPMAP, GL_FALSE);*/
    }

    //glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    // ToDO: erro?
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    OpenGLTextureData2D(graphicContext, openGLTextureType, 0, OpenGLImageFormatRGBA, image->Width, image->Height, 0, OpenGLImageFormatRGBA, OpenGLTypeByteUnsigned, image->PixelData);

   // glTexImage2D(textureType, 0, GL_RGBA, image->Width, image->Height, 0, format, OpenGLTypeByteUnsigned, image->PixelData);

   // glGenerateMipmap(textureType);
      

    OpenGLTextureBind(graphicContext, openGLTextureType, 0);

    return ActionSuccessful;
}

ActionResult GraphicTextureUse(GraphicContext* const graphicContext, CTexture* const texture)
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
    return ActionInvalid;
}

ActionResult GraphicTextureRegister(GraphicContext* const graphicContext, CTexture* const texture)
{
    return ActionInvalid;
}

ActionResult GraphicTextureRelease(GraphicContext* const graphicContext, CTexture* const texture)
{
    return ActionInvalid;
}

ActionResult GraphicTextureCubeRegister(GraphicContext* const graphicContext, CTextureCube* const textureCube)
{
    OpenGLID textureID = -1;

    // Check
    {
        const unsigned char isValid = 1;// textureCube.HasTextures();

        if (!isValid)
        {
            return ActionInvalid;
        }
    }

    OpenGLTextureCreate(&graphicContext->OpenGLInstance, 1u, &textureID);
    OpenGLTextureBind(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, textureID);


    /*

    typedef enum OpenGLTextureSettingName_
    {
        OpenGLTextureSettingNameInvalid,

        OpenGLTextureWrapAxisS,
        OpenGLTextureWrapAxisT,
        OpenGLTextureWrapAxisR,

        OpenGLTextureFilterTypeMinimation,
        OpenGLTextureFilterTypeMagnification

    }
    OpenGLTextureSettingName;

    typedef enum OpenGLTextureSettingValue_
    {
        OpenGLTextureFilterInvalid,
        OpenGLTextureFilterNear,
        OpenGLTextureFilterLiniar,
        OpenGLTextureFilterBiliniar,
        OpenGLTextureFilterTriliniar,

        OpenGLTextureWrapClampToEdge,

        OpenGLTextureBaseLevel,
        OpenGLTextureMaxLevel
    }
    OpenGLTextureSettingValue;





    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureWrapAxisS, OpenGLTextureWrapClampToEdge);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureWrapAxisT, OpenGLTextureWrapClampToEdge);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureWrapAxisR, OpenGLTextureWrapClampToEdge);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureFilterTypeMagnification, OpenGLTextureFilterLiniar);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureFilterTypeMinimation, OpenGLTextureFilterLiniar);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureBaseLevel, 0);
    OpenGLTextureParameterI(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, OpenGLTextureMaxLevel, 0);

    OpenGLEnable(&graphicContext->OpenGLInstance, OpenGLToggleTextureCubeMapSeamless);

    for (size_t i = 0; i < 6; i++)
    {
        Image* image = &textureCube->ImageList[i];
        const unsigned int textureTypeID = (unsigned int)GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
        const unsigned int imageFormat = OpenGLToImageFormat(image->Format);
        //const OpenGLTextureType imageType = OpenGLTextureTypeToID(textureTypeID);
        
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(textureTypeID, 0, GL_RGB, image->Width, image->Height, 0, imageFormat, GL_UNSIGNED_BYTE, image->PixelData);
    }*/

    OpenGLTextureBind(&graphicContext->OpenGLInstance, OpenGLTextureTypeCubeMap, 0);

    textureCube->ID = textureID;

    return ActionInvalid;
}

ActionResult GraphicTextureCubeRegisterUse(GraphicContext* const graphicContext, CTextureCube* const textureCube)
{
    return ActionInvalid;
}

ActionResult GraphicTextureCubeRelease(GraphicContext* const graphicContext, CTextureCube* const textureCube)
{
    return ActionInvalid;
}

ActionResult GraphicSkyboxRegister(GraphicContext* const graphicContext, CSkyBox* const skyBox)
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
    const size_t vertexDataSize = sizeof(vertexData) / sizeof(float);
    const size_t indexListSize = sizeof(indexList) / sizeof(unsigned int);
    //CTextureCube* textureCube = skyBox->Texture;
    //Renderable* renderable = &skyBox->RenderInfo;

    //GraphicModelRegisterFromData(renderable, vertexData, vertexDataSize, indexList, indexListSize);
    //GraphicTextureCubeRegister(textureCube);

    //renderable.TextureUse(textureCube.ID);

    return ActionInvalid;
}

ActionResult GraphicSkyboxUse(GraphicContext* const graphicContext, CSkyBox* const skyBox)
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

    return ActionInvalid;
}

ActionResult GraphicModelRegisterFromModel(GraphicContext* const graphicContext, Renderable* const renderable, const Model* const model)
{
    /*
    //float vaoIDList[128u];
      //memset(vaoIDList, -1, 128u * sizeof(unsigned int));

      //glGenVertexArrays(numberOfMeshes, vaoIDList); // Generate VAOs

    glGenVertexArrays(1, &renderable.ID); // Create VAO

    glBindVertexArray(renderable.ID);

    // VBO creator
    {
        const size_t numberOfMeshes = model.MeshListSize;
        size_t meshIndexCounter = 0;

        OpenGLID meshIDList[128u];
        MemorySet(meshIDList, 128u * sizeof(OpenGLID), -1);

        glGenBuffers(numberOfMeshes, meshIDList); // Create VBO Buffers

        renderable.ChunkListSize = numberOfMeshes;
        renderable.ChunkList = new RenderableChunk[numberOfMeshes];

        for (size_t i = 0; i < numberOfMeshes; ++i)
        {
            const Mesh& mesh = model.MeshList[i];
            const size_t segmentListSize = mesh.SegmentListSize;
            const OpenGLID vertexBufferID = meshIDList[meshIndexCounter++];

            assert(vertexBufferID != -1);

            RenderableChunk& chunk = renderable.ChunkList[i];

            chunk.ID = vertexBufferID;
            chunk.SegmentListSize = segmentListSize;
            chunk.SegmentList = new RenderableSegment[segmentListSize];

            // Select VBO
            glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID); // Select Buffer
            glBufferData(GL_ARRAY_BUFFER, mesh.VertexDataListSize * sizeof(float), mesh.VertexDataList, GL_STATIC_DRAW);

            OpenGL::VertexAttributeArrayDefine(sizeof(float), mesh.VertexDataStructureListSize, mesh.VertexDataStructureList);

#if 0
            printf("+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n");

            printf
            (
                "| %7s | %7s | %7s | %7s | %7s | %7s | %7s | %7s | %7s | %7s | %7s | %7s |\n",
                "x",
                "y",
                "z",
                "nx",
                "ny",
                "nz",
                "r",
                "g",
                "b",
                "a",
                "u",
                "v"
            );

            for (size_t i = 0; i < mesh.VertexDataListSize; )
            {
                float x = mesh.VertexDataList[i++];
                float y = mesh.VertexDataList[i++];
                float z = mesh.VertexDataList[i++];

                float nx = mesh.VertexDataList[i++];
                float ny = mesh.VertexDataList[i++];
                float nz = mesh.VertexDataList[i++];

                float r = mesh.VertexDataList[i++];
                float g = mesh.VertexDataList[i++];
                float b = mesh.VertexDataList[i++];
                float a = mesh.VertexDataList[i++];

                float u = mesh.VertexDataList[i++];
                float v = mesh.VertexDataList[i++];

                printf
                (
                    "| %7.2f | %7.2f | %7.2f | %7.2f | %7.2f | %7.2f | %7.2f | %7.2f | %7.2f | %7.2f | %7.2f | %7.2f |\n",
                    x,
                    y,
                    z,
                    nx,
                    ny,
                    nz,
                    r,
                    g,
                    b,
                    a,
                    u,
                    v
                );
            }

            printf("+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+---------+\n");
#endif
            size_t segmentIndexCounter = 0;

            OpenGLID segmentIDList[128u];
            MemorySet(segmentIDList, 128u * sizeof(OpenGLID), -1);

            glGenBuffers(segmentListSize, segmentIDList); // Generate IBO

            for (size_t segmentIndex = 0; segmentIndex < segmentListSize; ++segmentIndex)
            {
                const MeshSegment& meshSegment = mesh.SegmentList[segmentIndex];
                const OpenGLID indexBufferID = segmentIDList[segmentIndexCounter++];
                RenderableSegment& segment = chunk.SegmentList[segmentIndex];

                assert(indexBufferID != -1);

                segment.ID = indexBufferID;
                segment.Size = meshSegment.IndexDataListSize;


                segment.MaterialRangeSize = meshSegment.MaterialInfoSize;
                segment.MaterialRange = new SegmentMaterialRange[meshSegment.MaterialInfoSize];

                for (size_t i = 0; i < meshSegment.MaterialInfoSize; i++)
                {
                    const OBJElementMaterialInfo& info = meshSegment.MaterialInfo[i];
                    SegmentMaterialRange& range = segment.MaterialRange[i];

                    range.Size = info.Size;
                    range.TextureID = 3;

                }

#if 0

                for (size_t i = 0; i < meshSegment.IndexDataListSize; )
                {
                    unsigned int x = meshSegment.IndexDataList[i++];
                    unsigned int y = meshSegment.IndexDataList[i++];
                    unsigned int z = meshSegment.IndexDataList[i++];

                    printf("| %5i | %5i | %5i |\n", x, y, z);
                }
#endif
                // Select IBÒ
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshSegment.IndexDataListSize * sizeof(unsigned int), meshSegment.IndexDataList, GL_STATIC_DRAW);
            }
        }
    }

    // printf("[OpenGL] Register Mesh <%ls> from <%ls>. Sub-Meshes <%zu>\n", , model.FilePath, model.MeshListSize);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    */

    // Merge
}

ActionResult GraphicModelRegisterFromData(GraphicContext* const graphicContext, Renderable* const renderable, const float* vertexData, const size_t vertexDataSize, const unsigned int* indexList, const size_t indexListSize)
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

    return ActionInvalid;
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
        //case GraphicImageTypeTextureCubeRight: return OpenGLTextureTypeInvalid;
        //case GraphicImageTypeTextureCubeLeft: return OpenGLTextureTypeInvalid;
        //case GraphicImageTypeTextureCubeTop: return OpenGLTextureTypeInvalid;
        //case GraphicImageTypeTextureCubeDown: return OpenGLTextureTypeInvalid;
        //case GraphicImageTypeTextureCubeBack: return OpenGLTextureTypeInvalid;
        //case GraphicImageTypeTextureCubeFront: return OpenGLTextureTypeInvalid;
    }
}

void GraphicInstantiate(GraphicContext* const graphicContext)
{
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

ActionResult GraphicShaderProgramCreate(GraphicContext* const graphicContext)
{
    return ActionInvalid;
}

ActionResult GraphicShaderCompile(GraphicContext* const graphicContext)
{
    return ActionInvalid;
}

ActionResult GraphicShaderUse(GraphicContext* const graphicContext, const unsigned int shaderID)
{
    return ActionInvalid;
}

ActionResult GraphicShaderProgramCreateVF(GraphicContext* const graphicContext, ShaderProgram* const shaderProgram, const wchar_t* vertexShaderFilePath, const wchar_t* fragmentShaderFilePath)
{   
    Shader vertexShader;
    Shader fragmentShader;
    DataStream vertexShaderFile;
    DataStream fragmentFile;

    DataStreamConstruct(&vertexShaderFile);
    DataStreamConstruct(&fragmentFile);

    {
        const unsigned char isAlreadyLoaded = shaderProgram->ID != -1;
        const unsigned char hasEmptyPaths = !vertexShaderFilePath || !fragmentShaderFilePath;

        if (isAlreadyLoaded)
        {
            return ActionSuccessful;
        }

        if (hasEmptyPaths)
        {
            return ActionSuccessful;
        }
    }

    {
        const ActionResult actionResult = DataStreamMapToMemoryW(&vertexShaderFile, vertexShaderFilePath, 0, MemoryReadOnly);
        const unsigned char sucessful = ActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        vertexShader.Type = ShaderTypeVertex;
        vertexShader.Content = (char*)vertexShaderFile.Data;
        vertexShader.ContentSize = vertexShaderFile.DataSize;
    }


    {
        const ActionResult actionResult = DataStreamMapToMemoryW(&fragmentFile, fragmentShaderFilePath, 0, MemoryReadOnly);
        const unsigned char sucessful = ActionSuccessful == actionResult;

        if (!sucessful)
        {
            return actionResult;
        }

        fragmentShader.Type = ShaderTypeFragment;
        fragmentShader.Content = (char*)fragmentFile.Data;
        fragmentShader.ContentSize = fragmentFile.DataSize;
    }
    //-----

    const size_t shaderListSize = 2;
    Shader* const shaderList[2] = { &vertexShader, &fragmentShader };
    const OpenGLID shaderProgrammID = OpenGLShaderProgramCreate(&graphicContext->OpenGLInstance);
    unsigned int  sucessfulCounter = 0;
    unsigned char isValidShader = 1;

    for (size_t i = 0; i < shaderListSize; ++i)
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
    for (size_t i = 0; i < shaderListSize; ++i)
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

    DataStreamDestruct(&vertexShaderFile);
    DataStreamDestruct(&fragmentFile);

    if (!isValidShader)
    {
        return ActionInvalid;
    }

    return ActionSuccessful;
}

ActionResult GraphicRender(GraphicContext* const graphicContext, GraphicRenderMode renderMode, size_t start, size_t amount)
{
    return ActionInvalid;
}

void GraphicShaderUpdateMatrix4x4F(GraphicContext* const graphicContext, const unsigned int locationID, float* matrix4x4)
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

ActionResult GraphicRenderElement(GraphicContext* const graphicContext, GraphicRenderMode renderMode, size_t start, size_t amount)
{
    return ActionInvalid;
}

ActionResult GraphicRenderList(GraphicContext* const graphicContext, GraphicRenderMode renderMode, size_t start, size_t amount)
{
    return ActionInvalid;
}