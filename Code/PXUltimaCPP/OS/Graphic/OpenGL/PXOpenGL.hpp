#ifndef PXCPPOpenGLINCLUDE
#define PXCPPOpenGLINCLUDE

#include <OS/Graphic/OpenGL/PXOpenGL.h>

namespace PX
{
	//-------------------------------------------------------------------------
	enum class OpenGLVersion
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
		Version4x5x0,	// 2014 August   11. GLSL 4.50, Direct State Access(DSA), Flush Control, Robustness,  ES 3.1 API and shader compatibility, DX11 emulation features
		Version4x6x0	// 2017 July     31. GLSL 4.60, More efficient geometry processing and shader execution, more information, no error context, polygon offset clamp, SPIR-V, anisotropic filtering
	};

	enum class OpenGLTextureType
	{
		TypeInvalid,
		Type1D,
		Type2D,
		Type2DProxy,
		Type3D,
		Type1DArray,
		Type1DArrayProxy,
		Type2DArray,
		TypeRectangle,
		TypeRectangleProxy,


		TypeCubeMap,
		TypeCubeMapProxy,
		TypeCubeMapTop,
		TypeCubeMapBottom,
		TypeCubeMapFront,
		TypeCubeMapBack,
		TypeCubeMapLeft,
		TypeCubeMapRight,


		TypeCubleMapArray,
		TypeBuffer,
		Type2DMultiSample,
		Type2DMultiSampleArray
	};

	enum class OpenGLFrameBufferMode
	{
		Invalid,
		Draw,
		Read,
		DrawAndRead
	};

	enum class OpenGLShaderType
	{
		Invalid,
		Vertex,
		Fragment,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		Compute
	};

	enum class OpenGLRenderBufferAttachmentPoint
	{
		Invalid,
		Color,
		Depth,
		Stencil
	};

	enum class OpenGLPolygonRenderOrderMode
	{
		Invalid,
		Clockwise,
		CounterClockwise,
	};

	enum class OpenGLImageFormat
	{
		Invalid,
		RGB,
		RGBA,
		BGR,
		BGRA,
		Red,
		Green,
		Blue,
		Alpha,
		Luminance,
		LuminanceAlpha,
		ColorIndex,
		StencilIndex,
		DepthComponent
	};

	enum class OpenGLDataType
	{
		Invalid,

		ByteSigned,  // 1 Byte, signed
		ByteUnsigned, // 1 Byte, unsigned

		ShortSigned,  // 2 Byte, signed
		ShortUnsigned,  // 2 Byte, unsigned

		IntegerSigned,  // 4 Byte, signed
		IntegerUnsigned,  // 4 Byte, unsigned

		Float,  // 4 Byte
		Double,  // 8 Byte
	};

	enum class OpenGLRenderBufferFormat
	{
		Invalid,
		Depth24Stencil8
	};

	enum class OpenGLTextureParameterMode
	{
		ParameterModoInvalid,
		PXOpenGLDEPTH_STENCIL_TextureMODE,
		BASE_LEVEL,
		COMPARE_FUNC,
		COMPARE_MODE,
		LOD_BIAS,
		MIN_FILTER,
		MAG_FILTER,
		MIN_LOD,
		MAX_LOD,
		MAX_LEVEL,
		SWIZZLE_R,
		SWIZZLE_G,
		SWIZZLE_B,
		SWIZZLE_A,
		WRAP_S,
		WRAP_T,
		WRAP_R,
		BORDER_COLOR,
		SWIZZLE_RGBA,
	};

	enum class OpenGLRenderMode
	{
		Invalid,
		Points,
		Lines,
		LineLoop,
		LineStrip,
		Triangles,
		TriangleStrip,
		TriangleFan,
		Quads,
		QuadStrip,
		Polygon
	};

	enum class OpenGLBufferType
	{
		Array,//Vertex attributes
		AtomicCounter, 	//Atomic counter storage
		CopyRead,//Buffer copy source
		CopyWrite,//Buffer copy destination
		DispatchIndirect, //	Indirect compute dispatch commands
		DrawIndirect,	//Indirect command arguments
		ElementArray,	//Vertex array indices
		PixelPack,//Pixel read target
		PixelUnpack,//	Texture data source
		Query,//Query result buffer
		ShaderStorage,	//Read - write storage for shaders
		Texture,// Texture data buffer
		TransformFeedback,//	Transform feedback buffer
		Uniform
	};

	enum class OpenGLTextureParameterValue
	{
		Invalid,

		NEAREST,
		LINEAR,

		ClampToEdge,
		ClampToBorder,
		MirroredRepeat,
		Repeat,
		MirrorClampToEdge,
		WrapS,
		WrapR
	};

	enum class OpenGLStoreMode
	{
		Invalid,
		StreamDraw,
		StreamRead,
		StreamCopy,
		StaticDraw,
		StaticREAD,
		StaticCOPY,
		DynamicDraw,
		DynamicRead,
		DynamicCopy
	};

	enum class OpenGLDrawOrderMode
	{
		Invalid,
		Clockwise,
		CounterClockwise
	};

	enum class OpenGLToggle
	{
		Invalid,

		TextureCubeMapSeamless,

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
	};

	enum class OpenGLStringName
	{
		Invalid,
		Vendor,
		Renderer,
		Version,
		ShadingLanguage,
		Extensions
	};

	class OpenGL : public PXOpenGL
	{
		public:
		//---<Utility>-------------------------------------------------------------
		PXDLLExport OpenGL();
		PXDLLExport ~OpenGL();

		PXDLLExport void Set(OpenGL& pxOpenGL);
		PXDLLExport void Copy(OpenGL& pxOpenGL);

		PXDLLExport bool CreateForWindow();
		PXDLLExport void CreateWindowless(const PXSize width, const PXSize height);
		PXDLLExport void Select();
		PXDLLExport bool Deselect();
		PXDLLExport void Release();

		PXDLLExport void RenderBufferSwap();

		PXDLLExport void Flush();
		PXDLLExport void ViewSize(const PXSize x, const PXSize y, const PXSize width, const PXSize height);


		PXDLLExport void PolygonRenderOrder(const OpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode);

		PXDLLExport void SettingChange(const OpenGLToggle toggle, const bool state);

		PXDLLExport void DrawOrder(const OpenGLDrawOrderMode openGLDrawOrderMode);

		PXDLLExport void ClearColor(const float red, const float green, const float blue, const float alpha);
		PXDLLExport void Clear(const unsigned int clearID);
		PXDLLExport void DrawScaleF(const float x, const float y, const float z);
		PXDLLExport void DrawBegin(const OpenGLRenderMode openGLRenderMode);
		PXDLLExport void DrawVertexXYZF(const float x, const float y, const float z);
		PXDLLExport void DrawColorRGBF(const float red, const float green, const float blue);
		PXDLLExport void DrawEnd();
		PXDLLExport void Rectangle(const float xA, const float yA, const float xB, const float yB);

		PXDLLExport void TextureParameter(const OpenGLTextureType textureType, const OpenGLTextureParameterMode pname, const OpenGLTextureParameterValue openGLTextureParameterValue);
		PXDLLExport void TextureParameterI(const OpenGLTextureType textureType, const OpenGLTextureParameterMode pname, const int param);
		PXDLLExport void TextureParameterF(const OpenGLTextureType textureType, const OpenGLTextureParameterMode pname, const float param);

		// render

		PXDLLExport void DrawArrays(const OpenGLRenderMode renderMode, const PXSize offset, const PXSize amount);
		PXDLLExport void DrawElements(const OpenGLRenderMode renderMode, const PXSize amount, const OpenGLDataType openGLDataType, const void* const indexList);



		PXDLLExport void DrawArraysInstanced(const OpenGLRenderMode renderMode, const PXSize startOffset, const PXSize amount, const PXSize instanceAmount);

		//---<Texture>-------------------------------------------------------------
		PXDLLExport void TextureActivate(const unsigned int index);
		PXDLLExport void TextureCreate(GLsizei n, GLuint* textures); // glGenTextures
		PXDLLExport void TextureBind(const OpenGLTextureType textureType, GLuint texture); // glBindTexture
		PXDLLExport void TextureUnbind(const OpenGLTextureType textureType);
		PXDLLExport void TextureDelete(GLsizei n, const GLuint* textures); // glDeleteTextures
		PXDLLExport void TextureData2D
		(

			const OpenGLTextureType glTextureType,
			const unsigned int level,
			const OpenGLImageFormat glImageFormatInternal,
			const PXSize width,
			const PXSize height,
			const OpenGLImageFormat glImageFormatInput,
			const OpenGLDataType glDataType,
			const void* const imageData
		);
		//-------------------------------------------------------------------------


		//---<Shader>-----------------------------------------------------------------
		PXDLLExport unsigned int ShaderTypeToID(const OpenGLShaderType openGLShaderType);
		PXDLLExport PXOpenGLShaderID ShaderCreate(const OpenGLShaderType openGLShaderType);
		PXDLLExport void ShaderSource(const PXOpenGLShaderID shaderID, const PXSize count, const char** string, PXSize* const length);
		PXDLLExport unsigned char ShaderCompile(const PXOpenGLShaderID shaderID);
		PXDLLExport void ShaderGetiv(const PXOpenGLShaderID shaderID, GLenum pname, GLint* params);
		PXDLLExport void ShaderLogInfoGet(const PXOpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog);
		PXDLLExport void ShaderDelete(const PXOpenGLShaderID shaderID);

		PXDLLExport PXOpenGLShaderPXProgramID ShaderProgramCreate();
		PXDLLExport void ShaderProgramUse(const PXOpenGLShaderPXProgramID shaderPXProgramID);
		PXDLLExport void ShaderProgramDelete(const PXOpenGLShaderPXProgramID shaderPXProgramID);

		PXDLLExport void ShaderProgramAttach(const PXOpenGLShaderPXProgramID shaderPXProgramID, const PXOpenGLShaderID shaderID);
		PXDLLExport void ShaderProgramLink(const PXOpenGLShaderID shaderID);
		PXDLLExport void ShaderProgramValidate(const PXOpenGLShaderID shaderID);

		PXDLLExport GLint ShaderVariableIDGet(GLuint program, const char* name);
		PXDLLExport void ShaderVariableFx1(GLint location, GLfloat v0);
		PXDLLExport void ShaderVariableFx1xN(GLint location, GLsizei count, const GLfloat* value);
		PXDLLExport void ShaderVariableIx1(GLint location, GLint v0);
		PXDLLExport void ShaderVariableIx1xN(GLint location, GLsizei count, const GLint* value);
		PXDLLExport void ShaderVariableFx2(GLint location, GLfloat v0, GLfloat v1);
		PXDLLExport void ShaderVariableFx2xN(GLint location, GLsizei count, const GLfloat* value);
		PXDLLExport void ShaderVariableIx2(GLint location, GLint v0, GLint v1);
		PXDLLExport void ShaderVariableIx2xN(GLint location, GLsizei count, const GLint* value);
		PXDLLExport void ShaderVariableFx3(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
		PXDLLExport void ShaderVariableFx3xN(GLint location, GLsizei count, const GLfloat* value);
		PXDLLExport void ShaderVariableIx3(GLint location, GLint v0, GLint v1, GLint v2);
		PXDLLExport void ShaderVariableIx3xN(GLint location, GLsizei count, const GLint* value);
		PXDLLExport void ShaderVariableFx4(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
		PXDLLExport void ShaderVariableFx4xN(GLint location, GLsizei count, const GLfloat* value);
		PXDLLExport void ShaderVariableIx4(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
		PXDLLExport void ShaderVariableIx4xN(GLint location, GLsizei count, const GLint* value);
		PXDLLExport void ShaderVariableMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		PXDLLExport void ShaderVariableMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		PXDLLExport void ShaderVariableMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
		//-------------------------------------------------------------------------

		//---<Buffers>-------------------------------------------------------------
		PXDLLExport void VertexArrayGenerate(const unsigned int amount, unsigned int* const vaoList);
		PXDLLExport void VertexArrayBind(const unsigned int vaoID);
		PXDLLExport void VertexArrayUnbind();
		PXDLLExport void VertexArrayAttributeDefine
		(

			const unsigned int index,
			const unsigned int size,
			const OpenGLDataType datatype,
			const unsigned char normalized,
			const unsigned int stride,
			const PXSize offset
		);
		PXDLLExport void VertexAttributeDivisor(const PXSize index, const PXSize divisor);
		PXDLLExport void VertexArrayEnable(const unsigned int vertexArrayAtributeID);
		PXDLLExport void VertexArrayDisable(const unsigned int vertexArrayAtributeID);

		PXDLLExport void BufferGenerate(const unsigned int amount, unsigned int* const bufferIDList);
		PXDLLExport void BufferBind(const OpenGLBufferType bufferType, const unsigned int bufferID);
		PXDLLExport void BufferData(const OpenGLBufferType bufferType, const unsigned int size, const void* const data, const OpenGLStoreMode openGLStoreMode);
		PXDLLExport void BufferUnbind(const OpenGLBufferType bufferType);
		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		PXDLLExport void FrameBufferCreate(const unsigned int amount, unsigned int* const framebufferIDList);
		PXDLLExport void RenderBufferStorage(const OpenGLRenderBufferFormat internalformat, const int width, const int height);

		// If we bind to 0, we select the "main window"
		PXDLLExport void FrameBufferBind(const OpenGLFrameBufferMode target, const unsigned int framebufferID);

		PXDLLExport void FrameBufferDestroy(const unsigned int amount, unsigned int* const framebufferIDList);

		PXDLLExport void RenderBufferCreate(GLsizei n, GLuint* renderbuffers);


		PXDLLExport void RenderBufferBind(const unsigned int renderbuffer);

		PXDLLExport void RenderBufferDelete(GLsizei n, GLuint* renderbuffers);


		PXDLLExport void FrameBufferLinkTexture2D(const OpenGLRenderBufferAttachmentPoint attachment, const OpenGLTextureType textarget, const unsigned int textureID, const int level);
		PXDLLExport GLuint FrameBufferLinkRenderBuffer(const OpenGLRenderBufferAttachmentPoint attachment, const unsigned int renderbuffer);
		//-------------------------------------------------------------------------


		//---<PXOpenGL v.1.0.0>------------------------------------------------------
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.1.1.0>------------------------------------------------------
		PXDLLExport void PixelDataRead
		(

			const int x,
			const int y,
			const int width,
			const int height,
			PXOpenGLImageFormat imageFormat,
			PXOpenGLDataType openGLDataType,
			void* const pixelData
		);
		//-------------------------------------------------------------------------


	//---<PXOpenGL v.3.0.0>------------------------------------------------------
		PXDLLExport const char* StringGetI(const OpenGLStringName stringName, const unsigned int index);
		//-------------------------------------------------------------------------




		//---<Extensions>----------------------------------------------------------
		PXDLLExport void SwapIntervalSet(const PXInt32U interval);
		PXDLLExport PXInt32U SwapIntervalGet();
		//-------------------------------------------------------------------------

	};
}

#endif