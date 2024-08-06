using System;
using System.Runtime.InteropServices;

namespace PX
{
    //-------------------------------------------------------------------------
    public enum OpenGLVersion
    {
        VersionInvalid,
        VersionNone,
        Version1x0x0,
        Version1x1x0, // 1995              Texture objects, Vertex Arrays
        Version1x2x0, // 1998 March    16. 3D textures, BGRA and packed pixel formats, introduction of the imaging subset useful to image - processing applications
        Version1x2x1, // 1998 October  14. A concept of ARB extensions
        Version1x3x0, // 2001 August   14. Multitexturing, multisampling, texture compression
        Version1x4x0, // 2002 July     24. Depth textures, GLSlang
        Version1x5x0, // 2003 July     29. Vertex Buffer Object(VBO), Occlusion Queries
        Version2x0x0, // 2004 September 7. GLSL 1.10, MRT, Non Power of Two textures, Point Sprites, Two - sided stencil
        Version2x1x0, // 2006 July      2. GLSL 1.20, Pixel Buffer Object(PBO), sRGB Textures
        Version3x0x0, // 2008 August   11. GLSL 1.30, Texture Arrays, Conditional rendering, Frame Buffer Object(FBO)
        Version3x1x0, // 2009 March    24. GLSL 1.40, Instancing, Texture Buffer Object, Uniform Buffer Object, Primitive restart
        Version3x2x0, // 2009 August    3. GLSL 1.50, Geometry Shader, Multi - sampled textures
        Version3x3x0, // 2010 March    11. GLSL 3.30, Backports as much function as possible from the  4.0 specification
        Version4x0x0, // 2010 March    11. GLSL 4.00, Tessellation on GPU, shaders with 64 - bit precision
        Version4x1x0, // 2010 July     26. GLSL 4.10, Developer - friendly debug outputs, compatibility with  ES 2.0
        Version4x2x0, // 2011 August    8. GLSL 4.20, Shaders with atomic counters, draw transform feedback instanced, shader packing, performance improvements
        Version4x3x0, // 2012 August    6. GLSL 4.30, Compute shaders leveraging GPU parallelism, shader storage buffer objects, high - quality ETC2 / EAC texture compression, increased memory security, a multi - application robustness extension, compatibility with  ES 3.0
        Version4x4x0, // 2013 July     22. GLSL 4.40, Buffer Placement Control, Efficient Asynchronous Queries, Shader Variable Layout, Efficient Multiple Object Binding, Streamlined Porting of Direct3D applications, Bindless Texture Extension, Sparse Texture Extension
        Version4x5x0,   // 2014 August   11. GLSL 4.50, Direct State Access(DSA), Flush Control, Robustness,  ES 3.1 API and shader compatibility, DX11 emulation features
        Version4x6x0    // 2017 July     31. GLSL 4.60, More efficient geometry processing and shader execution, more information, no error context, polygon offset clamp, SPIR-V, anisotropic filtering
    }

    public enum OpenGLTextureType
    {
        TextureTypeInvalid,
        TextureType1D,
        TextureType2D,
        TextureType2DProxy,
        TextureType3D,
        TextureType1DArray,
        TextureType1DArrayProxy,
        TextureType2DArray,
        TextureTypeRectangle,
        TextureTypeRectangleProxy,


        TextureTypeCubeMap,
        TextureTypeCubeMapProxy,
        TextureTypeCubeMapTop,
        TextureTypeCubeMapBottom,
        TextureTypeCubeMapFront,
        TextureTypeCubeMapBack,
        TextureTypeCubeMapLeft,
        TextureTypeCubeMapRight,


        TextureTypeCubleMapArray,
        TextureTypeBuffer,
        TextureType2DMultiSample,
        TextureType2DMultiSampleArray
    }


    public enum OpenGLFrameBufferMode
    {
        FrameBufferModeInvalid,
        FrameBufferModeDraw,
        FrameBufferModeRead,
        FrameBufferModeDrawAndRead
    }


    public enum OpenGLShaderType
    {
        ShaderInvalid,
        ShaderVertex,
        ShaderFragment,
        ShaderTessellationControl,
        ShaderTessellationEvaluation,
        ShaderGeometry,
        ShaderCompute
    }
    public enum OpenGLRenderBufferAttachmentPoint
    {
        RenderBufferAttachmentPointInvalid,
        RenderBufferAttachmentPointColor,
        RenderBufferAttachmentPointDepth,
        RenderBufferAttachmentPointStencil
    }
    public enum OpenGLPolygonRenderOrderMode
    {
        PolygonRenderOrderModeInvalid,
        PolygonRenderOrderModeClockwise,
        PolygonRenderOrderModeCounterClockwise,
    }

    public enum OpenGLImageFormat
    {
        ImageFormatInvalid,
        ImageFormatRGB,
        ImageFormatRGBA,
        ImageFormatBGR,
        ImageFormatBGRA,
        ImageFormatRed,
        ImageFormatGreen,
        ImageFormatBlue,
        ImageFormatAlpha,
        ImageFormatLuminance,
        ImageFormatLuminanceAlpha,
        ImageFormatColorIndex,
        ImageFormatStencilIndex,
        ImageFormatDepthComponent
    }

    public enum OpenGLDataType
    {
        TypeInvalid,

        TypeByteSigned,  // 1 Byte, signed
        TypeByteUnsigned, // 1 Byte, unsigned

        TypeShortSigned,  // 2 Byte, signed
        TypeShortUnsigned,  // 2 Byte, unsigned

        TypeIntegerSigned,  // 4 Byte, signed
        TypeIntegerUnsigned,  // 4 Byte, unsigned

        TypeFloat,  // 4 Byte
        TypeDouble,  // 8 Byte
    }


    public enum OpenGLRenderBufferFormat
    {
        RenderBufferFormatInvalid,
        RenderBufferFormatDepth24Stencil8
    }

    public enum OpenGLTextureParameterMode
    {
        TextureParameterModoInvalid,
        DEPTH_STENCIL_TextureMODE,
        TextureBASE_LEVEL,
        TextureCOMPARE_FUNC,
        TextureCOMPARE_MODE,
        TextureLOD_BIAS,
        TextureMIN_FILTER,
        TextureMAG_FILTER,
        TextureMIN_LOD,
        TextureMAX_LOD,
        TextureMAX_LEVEL,
        TextureSWIZZLE_R,
        TextureSWIZZLE_G,
        TextureSWIZZLE_B,
        TextureSWIZZLE_A,
        TextureWRAP_S,
        TextureWRAP_T,
        TextureWRAP_R,
        TextureBORDER_COLOR,
        TextureSWIZZLE_RGBA,
    }

    public enum OpenGLRenderMode
    {
        RenderInvalid,
        RenderPoints,
        RenderLines,
        RenderLineLoop,
        RenderLineStrip,
        RenderTriangles,
        RenderTriangleStrip,
        RenderTriangleFan,
        RenderQuads,
        RenderQuadStrip,
        RenderPolygon
    }

    public enum OpenGLBufferType
    {
        BufferArray,//Vertex attributes
        BufferAtomicCounter,    //Atomic counter storage
        BufferCopyRead,//Buffer copy source
        BufferCopyWrite,//Buffer copy destination
        BufferDispatchIndirect, //    Indirect compute dispatch commands
        BufferDrawIndirect, //Indirect command arguments
        BufferElementArray, //Vertex array indices
        BufferPixelPack,//Pixel read target
        BufferPixelUnpack,//    Texture data source
        BufferQuery,//Query result buffer
        BufferShaderStorage,    //Read - write storage for shaders
        BufferTexture,// Texture data buffer
        BufferTransformFeedback,//    Transform feedback buffer
        BufferUniform
    }

    public enum OpenGLTextureParameterValue
    {
        TextureParameterValueInvalid,

        TextureParameterValueNEAREST,
        TextureParameterValueLINEAR,

        TextureParameterValueClampToEdge,
        TextureParameterValueClampToBorder,
        TextureParameterValueMirroredRepeat,
        TextureParameterValueRepeat,
        TextureParameterValueMirrorClampToEdge,
        TextureParameterValueWrapS,
        TextureParameterValueWrapR
    }

    public enum OpenGLStoreMode
    {
        StoreInvalid,
        StoreStreamDraw,
        StoreStreamRead,
        StoreStreamCopy,
        StoreStaticDraw,
        StoreStaticREAD,
        StoreStaticCOPY,
        StoreDynamicDraw,
        StoreDynamicRead,
        StoreDynamicCopy
    }


    public enum OpenGLDrawOrderMode
    {
        DrawOrderModeInvalid,
        DrawOrderModeClockwise,
        DrawOrderModeCounterClockwise
    }

    public enum OpenGLToggle
    {
        ToggleInvalid,

        ToggleTextureCubeMapSeamless,

        ALPHA_TEST, // If enabled, do alpha testing. See glAlphaFunc.
        AUTO_NORMAL, // If enabled, generate normal vectors when either MAP2_VERTEX_3 or MAP2_VERTEX_4 is used to generate vertices. See glMap2.
        BLEND, // If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
        CLIP_PLANEi, // If enabled, clip geometry against user-defined clipping plane i. See glClipPlane.
        COLOR_LOGIC_OP, // If enabled, apply the currently selected logical operation to the computed fragment color and color buffer values. See glLogicOp.
        COLOR_MATERIAL, // If enabled, have one or more material parameters track the current color. See glColorMaterial.
        COLOR_SUM, // If enabled and no fragment shader is active, add the secondary color value to the computed fragment color. See glSecondaryColor.
        COLOR_TABLE, // If enabled, perform a color table lookup on the incoming RGBA color values. See glColorTable.
        CONVOLUTION_1D, // If enabled, perform a 1D convolution operation on incoming RGBA color values. See glConvolutionFilter1D.
        CONVOLUTION_2D, // If enabled, perform a 2D convolution operation on incoming RGBA color values. See glConvolutionFilter2D.
        CULL_FACE, // If enabled, cull polygons based on their winding in window coordinates. See glCullFace.
        DEPTH_TEST, // If enabled, do depth comparisons and update the depth buffer. Note that even , // If the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated , // If the depth test is disabled. See glDepthFunc and glDepthRange.
        DITHER, // If enabled, dither color components or indices before they are written to the color buffer.
        FOG, // If enabled and no fragment shader is active, blend a fog color into the post-texturing color. See glFog.
        HISTOGRAM, // If enabled, histogram incoming RGBA color values. See glHistogram.
        INDEX_LOGIC_OP, // If enabled, apply the currently selected logical operation to the incoming index and color buffer indices. See glLogicOp.
        LIGHTi, // If enabled, include light i in the evaluation of the lighting equation. See glLightModel and glLight.
        LIGHTING, // If enabled and no vertex shader is active, use the current lighting parameters to compute the vertex color or index. Otherwise, simply associate the current color or index with each vertex. See glMaterial, glLightModel, and glLight.
        LINE_SMOOTH, // If enabled, draw lines with correct filtering. Otherwise, draw aliased lines. See glLineWidth.
        LINE_STIPPLE, // If enabled, use the current line stipple pattern when drawing lines. See glLineStipple.
        MAP1_COLOR_4, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate RGBA values. See glMap1.
        MAP1_INDEX, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate color indices. See glMap1.
        MAP1_NORMAL, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate normals. See glMap1.
        MAP1_TEXTURE_COORD_1, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s texture coordinates. See glMap1.
        MAP1_TEXTURE_COORD_2, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s and t texture coordinates. See glMap1.
        MAP1_TEXTURE_COORD_3, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s, t, and r texture coordinates. See glMap1.
        MAP1_TEXTURE_COORD_4, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s, t, r, and q texture coordinates. See glMap1.
        MAP1_VERTEX_3, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate x, y, and z vertex coordinates. See glMap1.
        MAP1_VERTEX_4, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate homogeneous x, y, z, and w vertex coordinates. See glMap1.
        MAP2_COLOR_4, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate RGBA values. See glMap2.
        MAP2_INDEX, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate color indices. See glMap2.
        MAP2_NORMAL, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate normals. See glMap2.
        MAP2_TEXTURE_COORD_1, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s texture coordinates. See glMap2.
        MAP2_TEXTURE_COORD_2, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s and t texture coordinates. See glMap2.
        MAP2_TEXTURE_COORD_3, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s, t, and r texture coordinates. See glMap2.
        MAP2_TEXTURE_COORD_4, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s, t, r, and q texture coordinates. See glMap2.
        MAP2_VERTEX_3, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate x, y, and z vertex coordinates. See glMap2.
        MAP2_VERTEX_4, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate homogeneous x, y, z, and w vertex coordinates. See glMap2.
        MINMAX, // If enabled, compute the minimum and maximum values of incoming RGBA color values. See glMinmax.
        MULTISAMPLE, // If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage.
        NORMALIZE, // If enabled and no vertex shader is active, normal vectors are normalized to unit length after transformation and before lighting. This method is generally less efficient than RESCALE_NORMAL. See glNormal and glNormalPointer.
        POINT_SMOOTH, // If enabled, draw points with proper filtering. Otherwise, draw aliased points. See glPointSize.
        POINT_SPRITE, // If enabled, calculate texture coordinates for points based on texture environment and point parameter settings. Otherwise texture coordinates are constant across points.
        POLYGON_OFFSET_FILL, // If enabled, and , // If the polygon is rendered in FILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset.
        POLYGON_OFFSET_LINE, // If enabled, and , // If the polygon is rendered in LINE mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset.
        POLYGON_OFFSET_POINT, // If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, , // If the polygon is rendered in POINT mode. See glPolygonOffset.
        POLYGON_SMOOTH, // If enabled, draw polygons with proper filtering. Otherwise, draw aliased polygons. For correct antialiased polygons, an alpha buffer is needed and the polygons must be sorted front to back.
        POLYGON_STIPPLE, // If enabled, use the current polygon stipple pattern when rendering polygons. See glPolygonStipple.
        POST_COLOR_MATRIX_COLOR_TABLE, // If enabled, perform a color table lookup on RGBA color values after color matrix transformation. See glColorTable.
        POST_CONVOLUTION_COLOR_TABLE, // If enabled, perform a color table lookup on RGBA color values after convolution. See glColorTable.
        RESCALE_NORMAL, // If enabled and no vertex shader is active, normal vectors are scaled after transformation and before lighting by a factor computed from the modelview matrix. , // If the modelview matrix scales space un, // Iformly, this has the effect of restoring the transformed normal to unit length. This method is generally more efficient than NORMALIZE. See glNormal and glNormalPointer.
        SAMPLE_ALPHA_TO_COVERAGE, // If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location. The temporary coverage value is then ANDed with the fragment coverage value.
        SAMPLE_ALPHA_TO_ONE, // If enabled, each sample alpha value is replaced by the maximum representable alpha value.
        SAMPLE_COVERAGE, // If enabled, the fragment's coverage is ANDed with the temporary coverage value. , // If SAMPLE_COVERAGE_INVERT is set to TRUE, invert the coverage value. See glSampleCoverage.
        SEPARABLE_2D, // If enabled, perform a two-dimensional convolution operation using a separable convolution filter on incoming RGBA color values. See glSeparableFilter2D.
        SCISSOR_TEST, // If enabled, discard fragments that are outside the scissor rectangle. See glScissor.
        STENCIL_TEST, // If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.
        TEXTURE_1D, // If enabled and no fragment shader is active, one-dimensional texturing is performed (unless two- or three-dimensional or cube-mapped texturing is also enabled). See glTexImage1D.
        TEXTURE_2D, // If enabled and no fragment shader is active, two-dimensional texturing is performed (unless three-dimensional or cube-mapped texturing is also enabled). See glTexImage2D.
        TEXTURE_3D, // If enabled and no fragment shader is active, three-dimensional texturing is performed (unless cube-mapped texturing is also enabled). See glTexImage3D.
        TEXTURE_CUBE_MAP, // If enabled and no fragment shader is active, cube-mapped texturing is performed. See glTexImage2D.
        TEXTURE_GEN_Q, // If enabled and no vertex shader is active, the q texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current q texture coordinate is used. See glTexGen.
        TEXTURE_GEN_R, // If enabled and no vertex shader is active, the r texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current r texture coordinate is used. See glTexGen.
        TEXTURE_GEN_S, // If enabled and no vertex shader is active, the s texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current s texture coordinate is used. See glTexGen.
        TEXTURE_GEN_T, // If enabled and no vertex shader is active, the t texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current t texture coordinate is used. See glTexGen.
        VERTEX_PROGRAM_POINT_SIZE, // If enabled and a vertex shader is active, then the derived point size is taken from the (potentially clipped) shader builtin PointSize and clamped to the implementation-dependent point size range.
        VERTEX_PROGRAM_TWO_SIDE// If enabled and a vertex shader is active, it spec, // Ifies that the GL will choose between front and back colors based on the polygon's face direction of which the vertex being shaded is a part. It has no effect on points or lines.
    }

    public enum OpenGLStringName
    {
        StringNameInvalid,
        StringNameVendor,
        StringNameRenderer,
        StringNameVersion,
        StringNameShadingLanguage,
        StringNameExtensions
    }




    [StructLayout(LayoutKind.Sequential, Size = 520)]
    internal unsafe struct PXOpenGL
    {
        public void* PXOpenGLConext;

        public fixed sbyte Vendor[64];
        public fixed sbyte Renderer[64];
        public fixed sbyte VersionText[64];
        public fixed sbyte GLSLVersionText[64];
        public OpenGLVersion Version;

        public void* AttachedWindow;
    }

    public class OpenGL : IDisposable
    {
        //---<Utility>-------------------------------------------------------------
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLConstruct(ref PXOpenGL openGLContext);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDestruct(ref PXOpenGL openGLContext);


        [DllImport("PXUltima.dll")] private static extern void PXOpenGLSet(ref PXOpenGL openGLContext, ref PXOpenGL openGLContextSoure);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLCopy(ref PXOpenGL openGLContext, ref PXOpenGL openGLContextSoure);

        [DllImport("PXUltima.dll")] private static extern byte PXOpenGLCreateForWindow(ref PXOpenGL openGLContext);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLCreateWindowless(ref PXOpenGL openGLContext, UIntPtr width, UIntPtr height);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLSelect(ref PXOpenGL openGLContext);
        [DllImport("PXUltima.dll")] private static extern byte PXOpenGLDeselect(ref PXOpenGL openGLContext);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLRelease(ref PXOpenGL openGLContext);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLRenderBufferSwap(ref PXOpenGL openGLContext);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLFlush(ref PXOpenGL openGLContext);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLViewSize(ref PXOpenGL openGLContext, UIntPtr x, UIntPtr y, UIntPtr width, UIntPtr height);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLPolygonRenderOrder(ref PXOpenGL openGLContext, OpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLSettingChange(ref PXOpenGL openGLContext, OpenGLToggle toggle, byte state);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawOrder(ref PXOpenGL openGLContext, OpenGLDrawOrderMode openGLDrawOrderMode);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLClearColor(ref PXOpenGL openGLContext, float red, float green, float blue, float alpha);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLClear(ref PXOpenGL openGLContext, ushort clearID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawScaleF(ref PXOpenGL openGLContext, float x, float y, float z);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawBegin(ref PXOpenGL openGLContext, OpenGLRenderMode openGLRenderMode);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawVertexXYZF(ref PXOpenGL openGLContext, float x, float y, float z);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawColorRGBF(ref PXOpenGL openGLContext, float red, float green, float blue);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawEnd(ref PXOpenGL openGLContext);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureParameter(ref PXOpenGL openGLContext, OpenGLTextureType textureType, OpenGLTextureParameterMode pname, OpenGLTextureParameterValue openGLTextureParameterValue);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureParameterI(ref PXOpenGL openGLContext, OpenGLTextureType textureType, OpenGLTextureParameterMode pname, int param);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureParameterF(ref PXOpenGL openGLContext, OpenGLTextureType textureType, OpenGLTextureParameterMode pname, float param);

        //[DllImport("PXUltima.dll")] private static extern void PXOpenGLAPICallType PXOpenGLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
        //-------------------------------------------------------------------------

        // render

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawArrays(ref PXOpenGL openGLContext, OpenGLRenderMode renderMode, UIntPtr offset, UIntPtr amount);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawElements(ref PXOpenGL openGLContext, OpenGLRenderMode renderMode, UIntPtr amount, OpenGLDataType openGLDataType, UIntPtr indexList);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLDrawArraysInstanced(ref PXOpenGL openGLContext, OpenGLRenderMode renderMode, UIntPtr startOffset, UIntPtr amount, UIntPtr instanceAmount);

        //---<Texture>-------------------------------------------------------------
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureActivate(ref PXOpenGL openGLContext, uint index);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureCreate(ref PXOpenGL openGLContext, uint n, ref uint textures); // glGenTextures
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureBind(ref PXOpenGL openGLContext, OpenGLTextureType textureType, uint texture); // glBindTexture
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureUnbind(ref PXOpenGL openGLContext, OpenGLTextureType textureType);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLTextureDelete(ref PXOpenGL openGLContext, uint n, ref uint textures); // glDeleteTextures
        [DllImport("PXUltima.dll")]
        private static extern void PXOpenGLTextureData2D
        (
            ref PXOpenGL openGLContext,
            OpenGLTextureType glTextureType,
            uint level,
            OpenGLImageFormat glImageFormatInternal,
            UIntPtr width,
            UIntPtr height,
            OpenGLImageFormat glImageFormatInput,
            OpenGLDataType glDataType,
            UIntPtr imageData
        );
        //-------------------------------------------------------------------------


        //---<Shader>-----------------------------------------------------------------
        [DllImport("PXUltima.dll")] private static extern uint PXOpenGLShaderTypeToID(OpenGLShaderType openGLShaderType);
        [DllImport("PXUltima.dll")] private static extern uint PXOpenGLShaderCreate(ref PXOpenGL openGLContext, OpenGLShaderType openGLShaderType);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderSource(ref PXOpenGL openGLContext, uint shaderID, UIntPtr count, UIntPtr shaderData, ref UIntPtr length);
        [DllImport("PXUltima.dll")] private static extern byte PXOpenGLShaderCompile(ref PXOpenGL openGLContext, uint shaderID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderGetiv(ref PXOpenGL openGLContext, uint shaderID, uint pname, ref uint parameter);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderLogInfoGet(ref PXOpenGL openGLContext, uint shaderID, uint maxLength, ref uint length, UIntPtr infoLog);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderDelete(ref PXOpenGL openGLContext, uint shaderID);

        [DllImport("PXUltima.dll")] private static extern uint PXOpenGLShaderProgramCreate(ref PXOpenGL openGLContext);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderProgramUse(ref PXOpenGL openGLContext, uint shaderPXProgramID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderProgramDelete(ref PXOpenGL openGLContext, uint shaderPXProgramID);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderProgramAttach(ref PXOpenGL openGLContext, uint shaderPXProgramID, uint shaderID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderProgramLink(ref PXOpenGL openGLContext, uint shaderID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderProgramValidate(ref PXOpenGL openGLContext, uint shaderID);

        [DllImport("PXUltima.dll")] private static extern uint PXOpenGLShaderVariableIDGet(ref PXOpenGL openGLContext, uint program, char name);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx1(ref PXOpenGL openGLContext, uint location, float v0);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx1xN(ref PXOpenGL openGLContext, uint location, uint count, ref float value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx1(ref PXOpenGL openGLContext, uint location, uint v0);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx1xN(ref PXOpenGL openGLContext, uint location, uint count, ref uint value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx2(ref PXOpenGL openGLContext, uint location, float v0, float v1);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx2xN(ref PXOpenGL openGLContext, uint location, uint count, ref float value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx2(ref PXOpenGL openGLContext, uint location, uint v0, uint v1);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx2xN(ref PXOpenGL openGLContext, uint location, uint count, ref uint value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx3(ref PXOpenGL openGLContext, uint location, float v0, float v1, float v2);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx3xN(ref PXOpenGL openGLContext, uint location, uint count, ref float value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx3(ref PXOpenGL openGLContext, uint location, uint v0, uint v1, uint v2);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx3xN(ref PXOpenGL openGLContext, uint location, uint count, ref uint value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx4(ref PXOpenGL openGLContext, uint location, float v0, float v1, float v2, float v3);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableFx4xN(ref PXOpenGL openGLContext, uint location, uint count, ref float value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx4(ref PXOpenGL openGLContext, uint location, uint v0, uint v1, uint v2, uint v3);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableIx4xN(ref PXOpenGL openGLContext, uint location, uint count, ref uint value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableMatrix2fv(ref PXOpenGL openGLContext, uint location, uint count, byte transpose, ref float value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableMatrix3fv(ref PXOpenGL openGLContext, uint location, uint count, byte transpose, ref float value);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLShaderVariableMatrix4fv(ref PXOpenGL openGLContext, uint location, uint count, byte transpose, ref float value);
        //-------------------------------------------------------------------------

        //---<Buffers>-------------------------------------------------------------
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLVertexArrayGenerate(ref PXOpenGL openGLContext, uint amount, ref uint vaoList);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLVertexArrayBind(ref PXOpenGL openGLContext, uint vaoID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLVertexArrayUnbind(ref PXOpenGL openGLContext);
        [DllImport("PXUltima.dll")]
        private static extern void PXOpenGLVertexArrayAttributeDefine
        (
            ref PXOpenGL openGLContext,
            uint index,
            uint size,
            OpenGLDataType datatype,
            byte normalized,
            uint stride,
            UIntPtr offset
        );
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLVertexAttributeDivisor(ref PXOpenGL openGLContext, UIntPtr index, UIntPtr divisor);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLVertexArrayEnable(ref PXOpenGL openGLContext, uint vertexArrayAtributeID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLVertexArrayDisable(ref PXOpenGL openGLContext, uint vertexArrayAtributeID);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLBufferGenerate(ref PXOpenGL openGLContext, uint amount, ref uint bufferIDList);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLBufferBind(ref PXOpenGL openGLContext, OpenGLBufferType bufferType, uint bufferID);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLBufferData(ref PXOpenGL openGLContext, OpenGLBufferType bufferType, uint size, ref uint data, OpenGLStoreMode openGLStoreMode);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLBufferUnbind(ref PXOpenGL openGLContext, OpenGLBufferType bufferType);
        //-------------------------------------------------------------------------

        //-------------------------------------------------------------------------
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLFrameBufferCreate(ref PXOpenGL openGLContext, uint amount, ref uint framebufferIDList);
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLRenderBufferStorage(ref PXOpenGL openGLContext, OpenGLRenderBufferFormat internalformat, int width, int height);

        // If we bind to 0, we select the "main window"
        [DllImport("PXUltima.dll")] private static extern void PXOpenGLFrameBufferBind(ref PXOpenGL openGLContext, OpenGLFrameBufferMode target, uint framebufferID);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLFrameBufferDestroy(ref PXOpenGL openGLContext, uint amount, ref uint framebufferIDList);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLRenderBufferCreate(ref PXOpenGL openGLContext, uint n, ref uint renderbuffers);


        [DllImport("PXUltima.dll")] private static extern void PXOpenGLRenderBufferBind(ref PXOpenGL openGLContext, uint renderbuffer);

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLRenderBufferDelete(ref PXOpenGL openGLContext, uint n, ref uint renderbuffers);


        [DllImport("PXUltima.dll")] private static extern void PXOpenGLFrameBufferLinkTexture2D(ref PXOpenGL openGLContext, OpenGLRenderBufferAttachmentPoint attachment, OpenGLTextureType textarget, uint textureID, uint level);
        [DllImport("PXUltima.dll")] private static extern uint PXOpenGLFrameBufferLinkRenderBuffer(ref PXOpenGL openGLContext, OpenGLRenderBufferAttachmentPoint attachment, uint renderbuffer);
        //-------------------------------------------------------------------------


        //---<PXOpenGL v.1.0.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.1.0>------------------------------------------------------
        [DllImport("PXUltima.dll")]
        private static extern void PXOpenGLPixelDataRead
        (
            ref PXOpenGL openGLContext,
        int x,
        int y,
        int width,
        int height,
        OpenGLImageFormat imageFormat,
        OpenGLDataType openGLDataType,
        UIntPtr pixelData
    );
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.2.1>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.3.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.4.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.1.5.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.2.0.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.2.1.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.3.0.0>------------------------------------------------------
        [DllImport("PXUltima.dll")] private static extern unsafe byte* PXOpenGLStringGetI(OpenGLStringName stringName, uint index);
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.3.1.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.3.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------


        //---<PXOpenGL v.4.0.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.1.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.3.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.4.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.5.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.6.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        [DllImport("PXUltima.dll")] private static extern void PXOpenGLSwapIntervalSet(ref PXOpenGL openGLContext, uint interval);
        [DllImport("PXUltima.dll")] private static extern uint PXOpenGLSwapIntervalGet(ref PXOpenGL openGLContext);


        private PXOpenGL _pxOpenGL = new PXOpenGL();

        public unsafe string Vendor
        {
            get
            {
                fixed(sbyte* adress = _pxOpenGL.Vendor)
                {
                    string vendor = new string(adress, 0, 64);

                    return vendor;
                }           
            }
        }
        public unsafe string Renderer
        {
            get
            {
                fixed (sbyte* adress = _pxOpenGL.Renderer)
                {
                    string vendor = new string(adress, 0, 64);

                    return vendor;
                }
            }
        }
        public unsafe string VersionText
        {
            get
            {
                fixed (sbyte* adress = _pxOpenGL.VersionText)
                {
                    string vendor = new string(adress, 0, 64);

                    return vendor;
                }
            }
        }
        public unsafe string GLSLVersionText
        {
            get
            {
                fixed (sbyte* adress = _pxOpenGL.GLSLVersionText)
                {
                    string vendor = new string(adress, 0, 64);

                    return vendor;
                }
            }
        }
        //  public PXOpenGLVersion Version;



        public uint SwapInterval
        {
            get
            {
                return PXOpenGLSwapIntervalGet(ref _pxOpenGL);
            }
            set
            {
                PXOpenGLSwapIntervalSet(ref _pxOpenGL, value);
            }
        }

        public OpenGL()
        {
            PXOpenGLConstruct(ref _pxOpenGL);
        }

        public void Dispose()
        {
            PXOpenGLDestruct(ref _pxOpenGL);
        }


        internal void Set(ref PXOpenGL openGLContextSoure)
        {

        }

        internal void Copy(ref PXOpenGL openGLContextSoure)
        {

        }

        public bool CreateForWindow()
        {
            return PXOpenGLCreateForWindow(ref _pxOpenGL) != 0;
        }

        public void CreateWindowless(int width, int height)
        {
            PXOpenGLCreateWindowless(ref _pxOpenGL, (UIntPtr)width, (UIntPtr)height);
        }

        public void Select()
        {
            PXOpenGLSelect(ref _pxOpenGL);
        }

        public bool Deselect()
        {
            return PXOpenGLDeselect(ref _pxOpenGL) != 0;
        }

        public void Release()
        {
            PXOpenGLRelease(ref _pxOpenGL);
        }

        public void RenderBufferSwap()
        {
            PXOpenGLRenderBufferSwap(ref _pxOpenGL);
        }

        public void Flush()
        {
            PXOpenGLFlush(ref _pxOpenGL);
        }

        public void ViewSize(int x, int y, int width, int height)
        {
            PXOpenGLViewSize(ref _pxOpenGL, (UIntPtr)x, (UIntPtr)y, (UIntPtr)width, (UIntPtr)height);
        }

        public void PolygonRenderOrder(OpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode)
        {

        }

        public void SettingChange(OpenGLToggle toggle, bool state)
        {

        }

        public void DrawOrder(OpenGLDrawOrderMode openGLDrawOrderMode)
        {
            PXOpenGLDrawOrder(ref _pxOpenGL, openGLDrawOrderMode);
        }

        public void ClearColor(float red, float green, float blue, float alpha)
        {
            PXOpenGLClearColor(ref _pxOpenGL, red, green, blue, alpha);
        }

        public void Clear(ushort clearID)
        {
            PXOpenGLClear(ref _pxOpenGL, clearID);
        }

        public void DrawScale(float x, float y, float z)
        {
            PXOpenGLDrawScaleF(ref _pxOpenGL, x, y, z);
        }

        public void DrawBegin(OpenGLRenderMode openGLRenderMode)
        {
            PXOpenGLDrawBegin(ref _pxOpenGL, openGLRenderMode);
        }

        public void DrawVertexXYZ(float x, float y, float z)
        {
            PXOpenGLDrawVertexXYZF(ref _pxOpenGL, x, y, z);
        }

        public void DrawColorRGB(float red, float green, float blue)
        {
            PXOpenGLDrawColorRGBF(ref _pxOpenGL, red, green, blue);
        }

        public void DrawEnd()
        {
            PXOpenGLDrawEnd(ref _pxOpenGL);
        }

        public void TextureParameter(OpenGLTextureType textureType, OpenGLTextureParameterMode pname, OpenGLTextureParameterValue openGLTextureParameterValue)
        {
            PXOpenGLTextureParameter(ref _pxOpenGL, textureType, pname, openGLTextureParameterValue);
        }

        public void TextureParameter(OpenGLTextureType textureType, OpenGLTextureParameterMode pname, int param)
        {
            PXOpenGLTextureParameterI(ref _pxOpenGL, textureType, pname, param);
        }

        public void TextureParameter(OpenGLTextureType textureType, OpenGLTextureParameterMode pname, float param)
        {
            PXOpenGLTextureParameterF(ref _pxOpenGL, textureType, pname, param);
        }


        // public void PXOpenGLAPICallType PXOpenGLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
        //-------------------------------------------------------------------------

        // render

        public void DrawArrays(OpenGLRenderMode renderMode, UIntPtr offset, UIntPtr amount)
        {
            PXOpenGLDrawArrays(ref _pxOpenGL, renderMode, offset, amount);
        }

        public void DrawElements(OpenGLRenderMode renderMode, UIntPtr amount, OpenGLDataType openGLDataType, UIntPtr indexList)
        {
            PXOpenGLDrawElements(ref _pxOpenGL, renderMode, amount, openGLDataType, indexList);
        }

        public void DrawArraysInstanced(OpenGLRenderMode renderMode, UIntPtr startOffset, UIntPtr amount, UIntPtr instanceAmount)
        {
            PXOpenGLDrawArraysInstanced(ref _pxOpenGL, renderMode, startOffset, amount, instanceAmount);
        }

        //---<Texture>-------------------------------------------------------------
        public void TextureActivate(uint index)
        {
            PXOpenGLTextureActivate(ref _pxOpenGL, index);
        }

        public void TextureCreate(uint n, ref uint textures)
        {
            PXOpenGLTextureCreate(ref _pxOpenGL, n, ref textures);
        }

        public void TextureBind(OpenGLTextureType textureType, uint texture)
        {
            PXOpenGLTextureBind(ref _pxOpenGL, textureType, texture);
        }

        public void TextureUnbind(OpenGLTextureType textureType)
        {
            PXOpenGLTextureUnbind(ref _pxOpenGL, textureType);
        }

        public void TextureDelete(uint n, ref uint textures)
        {
            PXOpenGLTextureDelete(ref _pxOpenGL, n, ref textures);
        }

        public void TextureData2D
        (
            OpenGLTextureType glTextureType,
            uint level,
            OpenGLImageFormat glImageFormatInternal,
            UIntPtr width,
            UIntPtr height,
            OpenGLImageFormat glImageFormatInput,
            OpenGLDataType glDataType,
            UIntPtr imageData
        )
        {
            PXOpenGLTextureData2D(ref _pxOpenGL, glTextureType, level, glImageFormatInternal, width, height, glImageFormatInput, glDataType, imageData);
        }
        //-------------------------------------------------------------------------


        //---<Shader>-----------------------------------------------------------------

        public uint ShaderCreate(OpenGLShaderType openGLShaderType)
        {
            return PXOpenGLShaderCreate(ref _pxOpenGL, openGLShaderType);
        }
        public void ShaderSource(uint shaderID, string stringData)
        {
            // PXOpenGLShaderSource(ref _pxOpenGL, shaderID, stringData);
        }
        public bool ShaderCompile(uint shaderID)
        {
            return PXOpenGLShaderCompile(ref _pxOpenGL, shaderID) != 0;
        }
        public void ShaderGetiv(uint shaderID, uint pname, ref uint parameter)
        {

        }
        public void ShaderLogInfoGet(uint shaderID, uint maxLength, ref uint length, byte infoLog)
        {

        }
        public void ShaderDelete(uint shaderID)
        {
            PXOpenGLShaderDelete(ref _pxOpenGL, shaderID);
        }

        public uint ShaderProgramCreate()
        {
            return PXOpenGLShaderProgramCreate(ref _pxOpenGL);
        }
        public void ShaderProgramUse(uint shaderPXProgramID)
        {
            PXOpenGLShaderProgramUse(ref _pxOpenGL, shaderPXProgramID);
        }
        public void ShaderProgramDelete(uint shaderPXProgramID)
        {
            PXOpenGLShaderProgramDelete(ref _pxOpenGL, shaderPXProgramID);
        }

        public void ShaderProgramAttach(uint shaderPXProgramID, uint shaderID)
        {

        }
        public void ShaderProgramLink(uint shaderID)
        {

        }
        public void ShaderProgramValidate(uint shaderID)
        {
            PXOpenGLShaderProgramValidate(ref _pxOpenGL, shaderID);
        }

        //public GLint ShaderVariableIDGet( uint program, const char* name);
        public void ShaderVariable(uint location, float v0)
        {

        }
        public void ShaderVariable(uint location, uint count, ref float value)
        {

        }
        public void ShaderVariable(uint location, uint v0)
        {

        }
        public void ShaderVariable(uint location, uint count, ref uint value)
        {

        }
        public void ShaderVariable(uint location, float v0, float v1)
        {

        }
        /*
        public void ShaderVariable(uint location, uint count, ref float value)
        {

        }*/
        public void ShaderVariable(uint location, uint v0, uint v1)
        {

        }
        /*
        public void ShaderVariable(uint location, uint count, ref uint value)
        {

        }*/
        public void ShaderVariable(uint location, float v0, float v1, float v2)
        {

        }
        /*
        public void ShaderVariable(uint location, uint count, ref float value)
        {

        }*/
        public void ShaderVariable(uint location, uint v0, uint v1, uint v2)
        {

        }
        /*
        public void ShaderVariable(uint location, uint count, ref uint value)
        {

        }*/
        public void ShaderVariable(uint location, float v0, float v1, float v2, float v3)
        {

        }
        /*
        public void ShaderVariable(uint location, uint count, ref float value)
        {

        }*/
        public void ShaderVariable(uint location, uint v0, uint v1, uint v2, uint v3)
        {

        }
        /*
        public void ShaderVariable(uint location, uint count, ref uint value)
        {

        }*/
        //-------------------------------------------------------------------------

        //---<Buffers>-------------------------------------------------------------
        public void VertexArrayGenerate(uint amount, ref uint vaoList)
        {

        }
        public void VertexArrayBind(uint vaoID)
        {

        }
        public void VertexArrayUnbind()
        {

        }
        public void VertexArrayAttributeDefine
            (


            uint index,
            uint size,
            OpenGLDataType datatype,
            bool normalized,
          uint stride,
            UIntPtr offset
        )
        {

        }
        public void VertexAttributeDivisor(UIntPtr index, UIntPtr divisor)
        {

        }
        public void VertexArrayEnable(uint vertexArrayAtributeID)
        {

        }
        public void VertexArrayDisable(uint vertexArrayAtributeID)
        {

        }

        public void BufferGenerate(uint amount, UIntPtr bufferIDList)
        {

        }
        public void BufferBind(OpenGLBufferType bufferType, uint bufferID)
        {

        }
        public void BufferData(OpenGLBufferType bufferType, uint size, UIntPtr data, OpenGLStoreMode openGLStoreMode)
        {

        }
        public void BufferUnbind(OpenGLBufferType bufferType)
        {

        }
        //-------------------------------------------------------------------------

        //-------------------------------------------------------------------------
        public void FrameBufferCreate(uint amount, UIntPtr framebufferIDList)
        {

        }
        public void RenderBufferStorage(OpenGLRenderBufferFormat internalformat, int width, int height)
        {

        }

        // If we bind to 0, we select the "main window"
        public void FrameBufferBind(OpenGLFrameBufferMode target, ref uint framebufferID)
        {

        }

        public void FrameBufferDestroy(uint amount, UIntPtr framebufferIDList)
        {

        }

        public void RenderBufferCreate(uint n, UIntPtr renderbuffers)
        {

        }


        public void RenderBufferBind(uint renderbuffer)
        {

        }

        public void RenderBufferDelete(uint n, UIntPtr renderbuffers)
        {

        }


        public void FrameBufferLinkTexture2D(OpenGLRenderBufferAttachmentPoint attachment, OpenGLTextureType textarget, uint textureID, uint level)
        {

        }
        public uint FrameBufferLinkRenderBuffer(OpenGLRenderBufferAttachmentPoint attachment, uint renderbuffer)
        {
            return PXOpenGLFrameBufferLinkRenderBuffer(ref _pxOpenGL, attachment, renderbuffer);
        }
        //-------------------------------------------------------------------------


        //---< v.1.0.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.1.1.0>------------------------------------------------------
        public void PixelDataRead
        (
            int x,
            int y,
            int width,
            int height,
            OpenGLImageFormat imageFormat,
            OpenGLDataType openGLDataType,
            UIntPtr pixelData
        )
        {

        }
        //-------------------------------------------------------------------------

        //---< v.1.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.1.2.1>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.1.3.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.1.4.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.1.5.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.2.0.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---< v.2.1.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.3.0.0>------------------------------------------------------
        public string StringGet(OpenGLStringName stringName, uint index)
        {
            return string.Empty;
        }
        //-------------------------------------------------------------------------

        //---< v.3.1.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---< v.3.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------


        //---< v.4.0.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.1.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.2.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.3.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.4.0>------------------------------------------------------
        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.5.0>------------------------------------------------------

        //-------------------------------------------------------------------------

        //---<PXOpenGL v.4.6.0>------------------------------------------------------
        //-------------------------------------------------------------------------


        //---<Extensions>----------------------------------------------------------

        //-------------------------------------------------------------------------
    }
}
