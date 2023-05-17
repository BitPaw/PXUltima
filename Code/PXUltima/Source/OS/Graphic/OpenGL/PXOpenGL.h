#ifndef PXOpenGLINCLUDE
#define PXOpenGLINCLUDE

#include <OS/System/OSVersion.h>

#if OSUnix

#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/glx.h>

typedef XID PXWindowID;// XID is PXWindow
typedef GLXContext PXOpenGLConextID;

#elif OSWindows
#include <Windows.h>
typedef HWND PXWindowID;
typedef HGLRC PXOpenGLConextID;
#endif

//---<PXOpenGL>----------------
#include <GL/gl.h>

#if OSUnix
#include <GL/glx.h>
#define PXOpenGLAPICallType
#elif OSWindows
#define PXOpenGLAPICallType APIENTRY
#endif
//---------------------------

#include <Media/PXType.h>

typedef unsigned int PXOpenGLID;
typedef unsigned int PXOpenGLShaderPXProgramID;
typedef unsigned int PXOpenGLShaderID;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

#ifdef __cplusplus
extern "C"
{
#endif

	//-------------------------------------------------------------------------
	typedef enum PXOpenGLVersion_
	{
		PXOpenGLVersionInvalid,
		PXOpenGLVersionNone,
		PXOpenGLVersion1x0x0,
		PXOpenGLVersion1x1x0, // 1995              Texture objects, Vertex Arrays
		PXOpenGLVersion1x2x0, // 1998 March    16. 3D textures, BGRA and packed pixel formats, introduction of the imaging subset useful to image - processing applications
		PXOpenGLVersion1x2x1, // 1998 October  14. A concept of ARB extensions
		PXOpenGLVersion1x3x0, // 2001 August   14. Multitexturing, multisampling, texture compression
		PXOpenGLVersion1x4x0, // 2002 July     24. Depth textures, GLSlang
		PXOpenGLVersion1x5x0, // 2003 July     29. Vertex Buffer Object(VBO), Occlusion Queries
		PXOpenGLVersion2x0x0, // 2004 September 7. GLSL 1.10, MRT, Non Power of Two textures, Point Sprites, Two - sided stencil
		PXOpenGLVersion2x1x0, // 2006 July      2. GLSL 1.20, Pixel Buffer Object(PBO), sRGB Textures
		PXOpenGLVersion3x0x0, // 2008 August   11. GLSL 1.30, Texture Arrays, Conditional rendering, Frame Buffer Object(FBO)
		PXOpenGLVersion3x1x0, // 2009 March    24. GLSL 1.40, Instancing, Texture Buffer Object, Uniform Buffer Object, Primitive restart
		PXOpenGLVersion3x2x0, // 2009 August    3. GLSL 1.50, Geometry Shader, Multi - sampled textures
		PXOpenGLVersion3x3x0, // 2010 March    11. GLSL 3.30, Backports as much function as possible from the PXOpenGL 4.0 specification
		PXOpenGLVersion4x0x0, // 2010 March    11. GLSL 4.00, Tessellation on GPU, shaders with 64 - bit precision
		PXOpenGLVersion4x1x0, // 2010 July     26. GLSL 4.10, Developer - friendly debug outputs, compatibility with PXOpenGL ES 2.0
		PXOpenGLVersion4x2x0, // 2011 August    8. GLSL 4.20, Shaders with atomic counters, draw transform feedback instanced, shader packing, performance improvements
		PXOpenGLVersion4x3x0, // 2012 August    6. GLSL 4.30, Compute shaders leveraging GPU parallelism, shader storage buffer objects, high - quality ETC2 / EAC texture compression, increased memory security, a multi - application robustness extension, compatibility with PXOpenGL ES 3.0
		PXOpenGLVersion4x4x0, // 2013 July     22. GLSL 4.40, Buffer Placement Control, Efficient Asynchronous Queries, Shader Variable Layout, Efficient Multiple Object Binding, Streamlined Porting of Direct3D applications, Bindless Texture Extension, Sparse Texture Extension
		PXOpenGLVersion4x5x0,	// 2014 August   11. GLSL 4.50, Direct State Access(DSA), Flush Control, Robustness, PXOpenGL ES 3.1 API and shader compatibility, DX11 emulation features
		PXOpenGLVersion4x6x0	// 2017 July     31. GLSL 4.60, More efficient geometry processing and shader execution, more information, no error context, polygon offset clamp, SPIR-V, anisotropic filtering
	}
	PXOpenGLVersion;

	typedef enum PXOpenGLTextureType_
	{
		PXOpenGLTextureTypeInvalid,
		PXOpenGLTextureType1D,
		PXOpenGLTextureType2D,
		PXOpenGLTextureType2DProxy,
		PXOpenGLTextureType3D,
		PXOpenGLTextureType1DArray,
		PXOpenGLTextureType1DArrayProxy,
		PXOpenGLTextureType2DArray,
		PXOpenGLTextureTypeRectangle,
		PXOpenGLTextureTypeRectangleProxy,


		PXOpenGLTextureTypeCubeMap,
		PXOpenGLTextureTypeCubeMapProxy,
		PXOpenGLTextureTypeCubeMapTop,
		PXOpenGLTextureTypeCubeMapBottom,
		PXOpenGLTextureTypeCubeMapFront,
		PXOpenGLTextureTypeCubeMapBack,
		PXOpenGLTextureTypeCubeMapLeft,
		PXOpenGLTextureTypeCubeMapRight,


		PXOpenGLTextureTypeCubleMapArray,
		PXOpenGLTextureTypeBuffer,
		PXOpenGLTextureType2DMultiSample,
		PXOpenGLTextureType2DMultiSampleArray
	}
	PXOpenGLTextureType;

	typedef enum PXOpenGLFrameBufferMode_
	{
		PXOpenGLFrameBufferModeInvalid,
		PXOpenGLFrameBufferModeDraw,
		PXOpenGLFrameBufferModeRead,
		PXOpenGLFrameBufferModeDrawAndRead
	}
	PXOpenGLFrameBufferMode;

	typedef enum PXOpenGLShaderType_
	{
		PXOpenGLShaderInvalid,
		PXOpenGLShaderVertex,
		PXOpenGLShaderFragment,
		PXOpenGLShaderTessellationControl,
		PXOpenGLShaderTessellationEvaluation,
		PXOpenGLShaderGeometry,
		PXOpenGLShaderCompute
	}
	PXOpenGLShaderType;

	typedef enum PXOpenGLRenderBufferAttachmentPoint_
	{
		PXOpenGLRenderBufferAttachmentPointInvalid,
		PXOpenGLRenderBufferAttachmentPointColor,
		PXOpenGLRenderBufferAttachmentPointDepth,
		PXOpenGLRenderBufferAttachmentPointStencil
	}
	PXOpenGLRenderBufferAttachmentPoint;

	PXPrivate unsigned int PXOpenGLRenderBufferAttachmentPointToID(const PXOpenGLRenderBufferAttachmentPoint renderBufferAttachmentPoint);


	typedef enum PXOpenGLPolygonRenderOrderMode_
	{
		PXOpenGLPolygonRenderOrderModeInvalid,
		PXOpenGLPolygonRenderOrderModeClockwise,
		PXOpenGLPolygonRenderOrderModeCounterClockwise,
	}
	PXOpenGLPolygonRenderOrderMode;

	PXPrivate GLenum PXOpenGLPolygonRenderOrderModeToID(const PXOpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode);


	typedef enum PXOpenGLImageFormat_
	{
		PXOpenGLImageFormatInvalid,
		PXOpenGLImageFormatRGB,
		PXOpenGLImageFormatRGBA,
		PXOpenGLImageFormatBGR,
		PXOpenGLImageFormatBGRA,
		PXOpenGLImageFormatRed,
		PXOpenGLImageFormatGreen,
		PXOpenGLImageFormatBlue,
		PXOpenGLImageFormatAlpha,
		PXOpenGLImageFormatLuminance,
		PXOpenGLImageFormatLuminanceAlpha,
		PXOpenGLImageFormatColorIndex,
		PXOpenGLImageFormatStencilIndex,
		PXOpenGLImageFormatDepthComponent
	}
	PXOpenGLImageFormat;

	PXPrivate int PXOpenGLImageFormatToID(const PXOpenGLImageFormat imageFormat);

	typedef enum PXOpenGLDataType_
	{
		PXOpenGLTypeInvalid,

		PXOpenGLTypeByteSigned,  // 1 Byte, signed
		PXOpenGLTypeByteUnsigned, // 1 Byte, unsigned

		PXOpenGLTypeShortSigned,  // 2 Byte, signed
		PXOpenGLTypeShortUnsigned,  // 2 Byte, unsigned

		PXOpenGLTypeIntegerSigned,  // 4 Byte, signed
		PXOpenGLTypeIntegerUnsigned,  // 4 Byte, unsigned

		PXOpenGLTypeFloat,  // 4 Byte
		PXOpenGLTypeDouble,  // 8 Byte
	}
	PXOpenGLDataType;

	PXPrivate int PXOpenGLDataTypeToID(const PXOpenGLDataType dataType);

	typedef enum PXOpenGLRenderBufferFormat_
	{
		PXOpenGLRenderBufferFormatInvalid,
		PXOpenGLRenderBufferFormatDepth24Stencil8
	}
	PXOpenGLRenderBufferFormat;

	PXPrivate int PXOpenGLRenderBufferFormatToID(const PXOpenGLRenderBufferFormat dataType);


	typedef enum PXOpenGLTextureParameterMode_
	{
		PXOpenGLTextureParameterModoInvalid,
		PXOpenGLDEPTH_STENCIL_TextureMODE,
		PXOpenGLTextureBASE_LEVEL,
		PXOpenGLTextureCOMPARE_FUNC,
		PXOpenGLTextureCOMPARE_MODE,
		PXOpenGLTextureLOD_BIAS,
		PXOpenGLTextureMIN_FILTER,
		PXOpenGLTextureMAG_FILTER,
		PXOpenGLTextureMIN_LOD,
		PXOpenGLTextureMAX_LOD,
		PXOpenGLTextureMAX_LEVEL,
		PXOpenGLTextureSWIZZLE_R,
		PXOpenGLTextureSWIZZLE_G,
		PXOpenGLTextureSWIZZLE_B,
		PXOpenGLTextureSWIZZLE_A,
		PXOpenGLTextureWRAP_S,
		PXOpenGLTextureWRAP_T,
		PXOpenGLTextureWRAP_R,
		PXOpenGLTextureBORDER_COLOR,
		PXOpenGLTextureSWIZZLE_RGBA,
	}
	PXOpenGLTextureParameterMode;

	PXPrivate GLenum PXOpenGLTextureParameterModeToID(const PXOpenGLTextureParameterMode textureParameterMode);

	typedef enum PXOpenGLRenderMode_
	{
		PXOpenGLRenderInvalid,
		PXOpenGLRenderPoints,
		PXOpenGLRenderLines,
		PXOpenGLRenderLineLoop,
		PXOpenGLRenderLineStrip,
		PXOpenGLRenderTriangles,
		PXOpenGLRenderTriangleStrip,
		PXOpenGLRenderTriangleFan,
		PXOpenGLRenderQuads,
		PXOpenGLRenderQuadStrip,
		PXOpenGLRenderPolygon
	}
	PXOpenGLRenderMode;

	PXPrivate int PXOpenGLRenderModeToID(const PXOpenGLRenderMode openGLRenderMode);



	typedef enum PXOpenGLBufferType_
	{
		PXOpenGLBufferArray,//Vertex attributes
		PXOpenGLBufferAtomicCounter, 	//Atomic counter storage
		PXOpenGLBufferCopyRead 	,//Buffer copy source
		PXOpenGLBufferCopyWrite 	,//Buffer copy destination
		PXOpenGLBufferDispatchIndirect, //	Indirect compute dispatch commands
		PXOpenGLBufferDrawIndirect,	//Indirect command arguments
		PXOpenGLBufferElementArray,	//Vertex array indices
		PXOpenGLBufferPixelPack 	,//Pixel read target
		PXOpenGLBufferPixelUnpack ,//	Texture data source
		PXOpenGLBufferQuery,//Query result buffer
		PXOpenGLBufferShaderStorage ,	//Read - write storage for shaders
		PXOpenGLBufferTexture 	,// Texture data buffer
		PXOpenGLBufferTransformFeedback,//	Transform feedback buffer
		PXOpenGLBufferUniform
	}
	PXOpenGLBufferType;

	PXPrivate int PXOpenGLBufferTypeToID(const PXOpenGLBufferType openGLBufferType);

	typedef enum PXOpenGLTextureParameterValue_
	{
		PXOpenGLTextureParameterValueInvalid,

		PXOpenGLTextureParameterValueNEAREST,
		PXOpenGLTextureParameterValueLINEAR,

		PXOpenGLTextureParameterValueClampToEdge,
		PXOpenGLTextureParameterValueClampToBorder,
		PXOpenGLTextureParameterValueMirroredRepeat,
		PXOpenGLTextureParameterValueRepeat,
		PXOpenGLTextureParameterValueMirrorClampToEdge,
		PXOpenGLTextureParameterValueWrapS,
		PXOpenGLTextureParameterValueWrapR
	}
	PXOpenGLTextureParameterValue;

	PXPrivate GLint PXOpenGLTextureParameterValueToID(const PXOpenGLTextureParameterValue openGLTextureParameterValue);


	typedef enum PXOpenGLStoreMode_
	{
		PXOpenGLStoreInvalid,
		PXOpenGLStoreStreamDraw,
		PXOpenGLStoreStreamRead,
		PXOpenGLStoreStreamCopy,
		PXOpenGLStoreStaticDraw,
		PXOpenGLStoreStaticREAD,
		PXOpenGLStoreStaticCOPY,
		PXOpenGLStoreDynamicDraw,
		PXOpenGLStoreDynamicRead,
		PXOpenGLStoreDynamicCopy
	}
	PXOpenGLStoreMode;

	PXPrivate int PXOpenGLStoreModeToID(const PXOpenGLStoreMode openGLStoreMode);


	typedef enum PXOpenGLDrawOrderMode_
	{
		PXOpenGLDrawOrderModeInvalid,
		PXOpenGLDrawOrderModeClockwise,
		PXOpenGLDrawOrderModeCounterClockwise
	}
	PXOpenGLDrawOrderMode;


	PXPrivate GLenum PXOpenGLDrawOrderToID(const PXOpenGLDrawOrderMode openGLDrawOrderMode);


	typedef enum PXOpenGLToggle_
	{
		PXOpenGLToggleInvalid,

		PXOpenGLToggleTextureCubeMapSeamless,

		PXOpenGLALPHA_TEST, // If enabled, do alpha testing. See glAlphaFunc.
		PXOpenGLAUTO_NORMAL, // If enabled, generate normal vectors when either PXOpenGLMAP2_VERTEX_3 or PXOpenGLMAP2_VERTEX_4 is used to generate vertices. See glMap2.
		PXOpenGLBLEND, // If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
		PXOpenGLCLIP_PLANEi, // If enabled, clip geometry against user-defined clipping plane i. See glClipPlane.
		PXOpenGLCOLOR_LOGIC_OP, // If enabled, apply the currently selected logical operation to the computed fragment color and color buffer values. See glLogicOp.
		PXOpenGLCOLOR_MATERIAL, // If enabled, have one or more material parameters track the current color. See glColorMaterial.
		PXOpenGLCOLOR_SUM, // If enabled and no fragment shader is active, add the secondary color value to the computed fragment color. See glSecondaryColor.
		PXOpenGLCOLOR_TABLE, // If enabled, perform a color table lookup on the incoming RGBA color values. See glColorTable.
		PXOpenGLCONVOLUTION_1D, // If enabled, perform a 1D convolution operation on incoming RGBA color values. See glConvolutionFilter1D.
		PXOpenGLCONVOLUTION_2D, // If enabled, perform a 2D convolution operation on incoming RGBA color values. See glConvolutionFilter2D.
		PXOpenGLCULL_FACE, // If enabled, cull polygons based on their winding in window coordinates. See glCullFace.
		PXOpenGLDEPTH_TEST, // If enabled, do depth comparisons and update the depth buffer. Note that even , // If the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated , // If the depth test is disabled. See glDepthFunc and glDepthRange.
		PXOpenGLDITHER, // If enabled, dither color components or indices before they are written to the color buffer.
		PXOpenGLFOG, // If enabled and no fragment shader is active, blend a fog color into the post-texturing color. See glFog.
		PXOpenGLHISTOGRAM, // If enabled, histogram incoming RGBA color values. See glHistogram.
		PXOpenGLINDEX_LOGIC_OP, // If enabled, apply the currently selected logical operation to the incoming index and color buffer indices. See glLogicOp.
		PXOpenGLLIGHTi, // If enabled, include light i in the evaluation of the lighting equation. See glLightModel and glLight.
		PXOpenGLLIGHTING, // If enabled and no vertex shader is active, use the current lighting parameters to compute the vertex color or index. Otherwise, simply associate the current color or index with each vertex. See glMaterial, glLightModel, and glLight.
		PXOpenGLLINE_SMOOTH, // If enabled, draw lines with correct filtering. Otherwise, draw aliased lines. See glLineWidth.
		PXOpenGLLINE_STIPPLE, // If enabled, use the current line stipple pattern when drawing lines. See glLineStipple.
		PXOpenGLMAP1_COLOR_4, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate RGBA values. See glMap1.
		PXOpenGLMAP1_INDEX, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate color indices. See glMap1.
		PXOpenGLMAP1_NORMAL, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate normals. See glMap1.
		PXOpenGLMAP1_TEXTURE_COORD_1, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s texture coordinates. See glMap1.
		PXOpenGLMAP1_TEXTURE_COORD_2, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s and t texture coordinates. See glMap1.
		PXOpenGLMAP1_TEXTURE_COORD_3, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s, t, and r texture coordinates. See glMap1.
		PXOpenGLMAP1_TEXTURE_COORD_4, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate s, t, r, and q texture coordinates. See glMap1.
		PXOpenGLMAP1_VERTEX_3, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate x, y, and z vertex coordinates. See glMap1.
		PXOpenGLMAP1_VERTEX_4, // If enabled, calls to glEvalCoord1, glEvalMesh1, and glEvalPoint1 generate homogeneous x, y, z, and w vertex coordinates. See glMap1.
		PXOpenGLMAP2_COLOR_4, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate RGBA values. See glMap2.
		PXOpenGLMAP2_INDEX, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate color indices. See glMap2.
		PXOpenGLMAP2_NORMAL, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate normals. See glMap2.
		PXOpenGLMAP2_TEXTURE_COORD_1, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s texture coordinates. See glMap2.
		PXOpenGLMAP2_TEXTURE_COORD_2, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s and t texture coordinates. See glMap2.
		PXOpenGLMAP2_TEXTURE_COORD_3, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s, t, and r texture coordinates. See glMap2.
		PXOpenGLMAP2_TEXTURE_COORD_4, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate s, t, r, and q texture coordinates. See glMap2.
		PXOpenGLMAP2_VERTEX_3, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate x, y, and z vertex coordinates. See glMap2.
		PXOpenGLMAP2_VERTEX_4, // If enabled, calls to glEvalCoord2, glEvalMesh2, and glEvalPoint2 generate homogeneous x, y, z, and w vertex coordinates. See glMap2.
		PXOpenGLMINMAX, // If enabled, compute the minimum and maximum values of incoming RGBA color values. See glMinmax.
		PXOpenGLMULTISAMPLE, // If enabled, use multiple fragment samples in computing the final color of a pixel. See glSampleCoverage.
		PXOpenGLNORMALIZE, // If enabled and no vertex shader is active, normal vectors are normalized to unit length after transformation and before lighting. This method is generally less efficient than PXOpenGLRESCALE_NORMAL. See glNormal and glNormalPointer.
		PXOpenGLPOINT_SMOOTH, // If enabled, draw points with proper filtering. Otherwise, draw aliased points. See glPointSize.
		PXOpenGLPOINT_SPRITE, // If enabled, calculate texture coordinates for points based on texture environment and point parameter settings. Otherwise texture coordinates are constant across points.
		PXOpenGLPOLYGON_OFFSET_FILL, // If enabled, and , // If the polygon is rendered in PXOpenGLFILL mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset.
		PXOpenGLPOLYGON_OFFSET_LINE, // If enabled, and , // If the polygon is rendered in PXOpenGLLINE mode, an offset is added to depth values of a polygon's fragments before the depth comparison is performed. See glPolygonOffset.
		PXOpenGLPOLYGON_OFFSET_POINT, // If enabled, an offset is added to depth values of a polygon's fragments before the depth comparison is performed, , // If the polygon is rendered in PXOpenGLPOINT mode. See glPolygonOffset.
		PXOpenGLPOLYGON_SMOOTH, // If enabled, draw polygons with proper filtering. Otherwise, draw aliased polygons. For correct antialiased polygons, an alpha buffer is needed and the polygons must be sorted front to back.
		PXOpenGLPOLYGON_STIPPLE, // If enabled, use the current polygon stipple pattern when rendering polygons. See glPolygonStipple.
		PXOpenGLPOST_COLOR_MATRIX_COLOR_TABLE, // If enabled, perform a color table lookup on RGBA color values after color matrix transformation. See glColorTable.
		PXOpenGLPOST_CONVOLUTION_COLOR_TABLE, // If enabled, perform a color table lookup on RGBA color values after convolution. See glColorTable.
		PXOpenGLRESCALE_NORMAL, // If enabled and no vertex shader is active, normal vectors are scaled after transformation and before lighting by a factor computed from the modelview matrix. , // If the modelview matrix scales space un, // Iformly, this has the effect of restoring the transformed normal to unit length. This method is generally more efficient than PXOpenGLNORMALIZE. See glNormal and glNormalPointer.
		PXOpenGLSAMPLE_ALPHA_TO_COVERAGE, // If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location. The temporary coverage value is then ANDed with the fragment coverage value.
		PXOpenGLSAMPLE_ALPHA_TO_ONE, // If enabled, each sample alpha value is replaced by the maximum representable alpha value.
		PXOpenGLSAMPLE_COVERAGE, // If enabled, the fragment's coverage is ANDed with the temporary coverage value. , // If PXOpenGLSAMPLE_COVERAGE_INVERT is set to PXOpenGLTRUE, invert the coverage value. See glSampleCoverage.
		PXOpenGLSEPARABLE_2D, // If enabled, perform a two-dimensional convolution operation using a separable convolution filter on incoming RGBA color values. See glSeparableFilter2D.
		PXOpenGLSCISSOR_TEST, // If enabled, discard fragments that are outside the scissor rectangle. See glScissor.
		PXOpenGLSTENCIL_TEST, // If enabled, do stencil testing and update the stencil buffer. See glStencilFunc and glStencilOp.
		PXOpenGLTEXTURE_1D, // If enabled and no fragment shader is active, one-dimensional texturing is performed (unless two- or three-dimensional or cube-mapped texturing is also enabled). See glTexImage1D.
		PXOpenGLTEXTURE_2D, // If enabled and no fragment shader is active, two-dimensional texturing is performed (unless three-dimensional or cube-mapped texturing is also enabled). See glTexImage2D.
		PXOpenGLTEXTURE_3D, // If enabled and no fragment shader is active, three-dimensional texturing is performed (unless cube-mapped texturing is also enabled). See glTexImage3D.
		PXOpenGLTEXTURE_CUBE_MAP, // If enabled and no fragment shader is active, cube-mapped texturing is performed. See glTexImage2D.
		PXOpenGLTEXTURE_GEN_Q, // If enabled and no vertex shader is active, the q texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current q texture coordinate is used. See glTexGen.
		PXOpenGLTEXTURE_GEN_R, // If enabled and no vertex shader is active, the r texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current r texture coordinate is used. See glTexGen.
		PXOpenGLTEXTURE_GEN_S, // If enabled and no vertex shader is active, the s texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current s texture coordinate is used. See glTexGen.
		PXOpenGLTEXTURE_GEN_T, // If enabled and no vertex shader is active, the t texture coordinate is computed using the texture generation function defined with glTexGen. Otherwise, the current t texture coordinate is used. See glTexGen.
		PXOpenGLVERTEX_PROGRAM_POINT_SIZE, // If enabled and a vertex shader is active, then the derived point size is taken from the (potentially clipped) shader builtin PXOpenGLPointSize and clamped to the implementation-dependent point size range.
		PXOpenGLVERTEX_PROGRAM_TWO_SIDE// If enabled and a vertex shader is active, it spec, // Ifies that the GL will choose between front and back colors based on the polygon's face direction of which the vertex being shaded is a part. It has no effect on points or lines.
	}
	PXOpenGLToggle;

	PXPrivate int PXOpenGLToggleToID(const PXOpenGLToggle openGLToggle);


	typedef enum PXOpenGLStringName_
	{
		PXOpenGLStringNameInvalid,
		PXOpenGLStringNameVendor,
		PXOpenGLStringNameRenderer,
		PXOpenGLStringNameVersion,
		PXOpenGLStringNameShadingLanguage,
		PXOpenGLStringNameExtensions
	}
	PXOpenGLStringName;

	typedef const GLubyte* (*PXOpenGLStringFunction)(GLenum name); // glGetString

	PXPrivate unsigned int PXOpenGLStringNameToID(const PXOpenGLStringName stringName);

	PXPublic const char* PXOpenGLStringGet(const PXOpenGLStringName stringName);


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
	PXPublic unsigned int PXOpenGLTextureTypeToID(const PXOpenGLTextureType openGLTextureType);

	typedef unsigned int (PXOpenGLAPICallType* PXOpenGLShaderPXProgramCreateFunction)();
	typedef void (PXOpenGLAPICallType* PXOpenGLShaderPXProgramUseFunction)(unsigned int program);
	typedef void (PXOpenGLAPICallType* PXOpenGLShaderPXProgramDeleteFunction)(GLuint program); // glDeletePXProgram
	typedef void (PXOpenGLAPICallType* PXOpenGLShaderSourceFunction)(unsigned int shader, int count, const char** string, const int* length);
	typedef GLuint(PXOpenGLAPICallType* PXOpenGLShaderCreateFunction)(GLenum shaderType); // glCreateShader
	typedef void (PXOpenGLAPICallType* PXOpenGLShaderCompileFunction)(GLuint shader);
	typedef void (PXOpenGLAPICallType* PXOpenGLShaderGetivFunction)(GLuint shader, GLenum pname, GLint* params);
	typedef void (PXOpenGLAPICallType* PXOpenGLShaderLogInfoGetFunction)(GLuint shader, GLsizei maxLength, GLsizei* length, char* infoLog);
	typedef void (PXOpenGLAPICallType* PXOpenGLShaderDeleteFunction)(GLuint shader);
	typedef void (PXOpenGLAPICallType* PXOpenGLAttachShaderFunction)(GLuint program, GLuint shader); // glAttachShader
	typedef void (PXOpenGLAPICallType* PXOpenGLLinkPXProgramFunction)(GLuint program); // glLinkPXProgram
	typedef void (PXOpenGLAPICallType* PXOpenGLValidatePXProgramFunction)(GLuint program); // glValidatePXProgram
	typedef void (PXOpenGLAPICallType* PXOpenGLActiveTextureFunction)(GLenum texture); // glActiveTexture
	typedef void (PXOpenGLAPICallType* PXOpenGLGenBuffersFunction)(GLsizei n, GLuint* buffers); // glGenBuffers
	typedef void (PXOpenGLAPICallType* PXOpenGLBindBufferFunction)(GLenum target, GLuint buffer);
	typedef void (PXOpenGLAPICallType* PXOpenGLBufferDataFunction)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

	typedef void (PXOpenGLAPICallType* PXOpenGLVertexAttribPointerFunction)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	typedef void (PXOpenGLAPICallType* PXOpenGLVertexAttribArrayEnableFunction)(GLuint index); // glEnableVertexAttribArray
	typedef void (PXOpenGLAPICallType* PXOpenGLVertexAttribArrayDisableFunction)(GLuint index); // glDisableVertexAttribArray

	typedef void (PXOpenGLAPICallType* PXOpenGLDisableVertexArrayAttribFunction)(GLuint vaobj, GLuint index);
	typedef GLint(PXOpenGLAPICallType* PXOpenGLGetUniformLocation)(GLuint program, const char* name);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform1fFunction)(GLint location, GLfloat v0);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform1fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform1iFunction)(GLint location, GLint v0);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform1ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform2fFunction)(GLint location, GLfloat v0, GLfloat v1);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform2fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform2iFunction)(GLint location, GLint v0, GLint v1);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform2ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform3fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform3fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform3iFunction)(GLint location, GLint v0, GLint v1, GLint v2);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform3ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform4fFunction)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform4fvFunction)(GLint location, GLsizei count, const GLfloat* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform4iFunction)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniform4ivFunction)(GLint location, GLsizei count, const GLint* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniformMatrix2fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniformMatrix3fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	typedef void (PXOpenGLAPICallType* PXOpenGLUniformMatrix4fvFunction)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

	typedef void (PXOpenGLAPICallType* PXOpenGLTextureCreateFunction)(GLsizei n, GLuint* textures); // glGenTextures
	typedef void (PXOpenGLAPICallType* PXOpenGLTextureBindFunction)(GLenum target, GLuint texture); // glBindTexture
	typedef void (PXOpenGLAPICallType* PXOpenGLTextureDeleteFunction)(GLsizei n, const GLuint* textures); // glDeleteTextures
    //-------------------------------------------------------------------------

	//---<PXOpenGL v.2.1.0>------------------------------------------------------
    //-------------------------------------------------------------------------

    //---<PXOpenGL v.3.0.0>------------------------------------------------------
	typedef void (PXOpenGLAPICallType* PXOpenGLFrameBufferCreateFunction)(GLsizei n, GLuint* ids); // glGenFramebuffers
	typedef void (PXOpenGLAPICallType* PXOpenGLFrameBufferDeleteFunction)(GLsizei n, GLuint* framebuffers); // glDeleteFramebuffers
	typedef void (PXOpenGLAPICallType* PXOpenGLFrameBufferBindFunction)(GLenum target, GLuint framebuffer); // glBindFramebuffer

	typedef void (PXOpenGLAPICallType* PXOpenGLRenderBufferCreateFunction)(GLsizei n, GLuint* renderbuffers); // glGenRenderbuffers
	typedef void (PXOpenGLAPICallType* PXOpenGLRenderBufferBindFunction)(GLenum target, GLuint renderbuffer); // glBindRenderbuffer
	typedef void (PXOpenGLAPICallType* PXOpenGLRenderBufferDeleteFunction)(GLsizei n, GLuint* renderbuffers); // glDeleteRenderbuffers
	typedef void (PXOpenGLAPICallType* PXOpenGLRenderBufferStorageFunction)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height); // glRenderbufferStorage

	typedef void (PXOpenGLAPICallType* PXOpenGLFrameBufferLinkTexture2DFunction)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef GLuint (PXOpenGLAPICallType* PXOpenGLFrameBufferLinkRenderBufferFunction)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

	typedef void (PXOpenGLAPICallType* PXOpenGLGenVertexArraysFunction)(GLsizei n, GLuint* arrays);
	typedef void (PXOpenGLAPICallType* PXOpenGLBindVertexArrayFunction)(GLuint arrayID);
	typedef void (PXOpenGLAPICallType* PXOpenGLVertexAttribIPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
	typedef const GLubyte* (PXOpenGLAPICallType* PXOpenGLStringIFunction)(GLenum name, GLuint index); // glGetStringi

	//-------------------------------------------------------------------------

	//---<PXOpenGL v.3.1.0>------------------------------------------------------
	typedef void (PXOpenGLAPICallType* PXOpenGLDrawArraysInstancedFunction)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount); // glDrawArraysInstanced
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.3.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.3.3.0>------------------------------------------------------
	typedef void  (PXOpenGLAPICallType* PXOpenGLVertexAttribDivisorFunction)(GLuint index, GLuint divisor); // glVertexAttribDivisor
	//-------------------------------------------------------------------------



	//---<PXOpenGL v.4.0.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.1.0>------------------------------------------------------
	typedef void (PXOpenGLAPICallType * PXOpenGLVertexAttribLPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.3.0>------------------------------------------------------
	typedef void (PXOpenGLAPICallType*DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	typedef void (PXOpenGLAPICallType*PXOpenGLDebugMessageFunction)(DEBUGPROC callback, const void* userParam); // Set
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.4.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.5.0>------------------------------------------------------
	typedef void (PXOpenGLAPICallType*PXOpenGLNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
	typedef void (PXOpenGLAPICallType*PXOpenGLEnableVertexAttribArray)(GLuint index);
	typedef void (PXOpenGLAPICallType*PXOpenGLEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.6.0>------------------------------------------------------
	//-------------------------------------------------------------------------


	//---<Extensions>----------------------------------------------------------
#if OSUnix

#elif OSWindows
	typedef const char* (PXOpenGLAPICallType* PXOpenGLStringGetExtensionsARB)(HDC hdc); // wglGetExtensionsStringARB
#endif
















	typedef struct PXOpenGLContext_
	{
		PXOpenGLConextID PXOpenGLConext;

		char Vendor[64];
		char Renderer[64];
		char VersionText[64];
		char GLSLVersionText[64];
		PXOpenGLVersion Version;

        void* AttachedWindow;




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
		PXOpenGLShaderPXProgramCreateFunction	PXOpenGLShaderPXProgramCreateCallBack;
		PXOpenGLShaderPXProgramUseFunction	PXOpenGLShaderPXProgramUseCallBack;
		PXOpenGLShaderPXProgramDeleteFunction PXOpenGLShaderPXProgramDeleteCallBack;
		PXOpenGLShaderCreateFunction PXOpenGLShaderCreateCallBack;
		PXOpenGLShaderSourceFunction	PXOpenGLShaderSourceCallBack;
		PXOpenGLShaderCompileFunction	PXOpenGLShaderCompileCallBack;
		PXOpenGLShaderGetivFunction	PXOpenGLShaderGetivCallBack;
		PXOpenGLShaderLogInfoGetFunction	PXOpenGLShaderLogInfoGetCallBack;
		PXOpenGLShaderDeleteFunction	PXOpenGLShaderDeleteCallBack;
		PXOpenGLAttachShaderFunction	PXOpenGLAttachShaderCallBack;
		PXOpenGLLinkPXProgramFunction	PXOpenGLLinkPXProgramCallBack;
		PXOpenGLValidatePXProgramFunction	PXOpenGLValidatePXProgramCallBack;
		PXOpenGLActiveTextureFunction	PXOpenGLActiveTextureCallBack;
		PXOpenGLGenBuffersFunction	PXOpenGLGenBuffersCallBack;
		PXOpenGLBindBufferFunction	PXOpenGLBindBufferCallBack;
		PXOpenGLBufferDataFunction	PXOpenGLBufferDataCallBack;
		PXOpenGLVertexAttribPointerFunction PXOpenGLVertexAttribPointerCallBack;
		PXOpenGLVertexAttribArrayEnableFunction PXOpenGLVertexAttribArrayEnableCallBack;
		PXOpenGLVertexAttribArrayDisableFunction PXOpenGLVertexAttribArrayDisableCallBack;
		PXOpenGLDisableVertexArrayAttribFunction	PXOpenGLDisableVertexArrayAttribCallBack;
		PXOpenGLGetUniformLocation	PXOpenGLGetUniformLocation;
		PXOpenGLUniform1fFunction	PXOpenGLUniform1fCallBack;
		PXOpenGLUniform1fvFunction	PXOpenGLUniform1fvCallBack;
		PXOpenGLUniform1iFunction	PXOpenGLUniform1iCallBack;
		PXOpenGLUniform1ivFunction	PXOpenGLUniform1ivCallBack;
		PXOpenGLUniform2fFunction	PXOpenGLUniform2fCallBack;
		PXOpenGLUniform2fvFunction	PXOpenGLUniform2fvCallBack;
		PXOpenGLUniform2iFunction	PXOpenGLUniform2iCallBack;
		PXOpenGLUniform2ivFunction	PXOpenGLUniform2ivCallBack;
		PXOpenGLUniform3fFunction	PXOpenGLUniform3fCallBack;
		PXOpenGLUniform3fvFunction	PXOpenGLUniform3fvCallBack;
		PXOpenGLUniform3iFunction	PXOpenGLUniform3iCallBack;
		PXOpenGLUniform3ivFunction	PXOpenGLUniform3ivCallBack;
		PXOpenGLUniform4fFunction	PXOpenGLUniform4fCallBack;
		PXOpenGLUniform4fvFunction	PXOpenGLUniform4fvCallBack;
		PXOpenGLUniform4iFunction	PXOpenGLUniform4iCallBack;
		PXOpenGLUniform4ivFunction	PXOpenGLUniform4ivCallBack;
		PXOpenGLUniformMatrix2fvFunction	PXOpenGLUniformMatrix2fvCallBack;
		PXOpenGLUniformMatrix3fvFunction	PXOpenGLUniformMatrix3fvCallBack;
		PXOpenGLUniformMatrix4fvFunction PXOpenGLUniformMatrix4fvCallBack;

		PXOpenGLTextureCreateFunction PXOpenGLTextureCreateCallBack;
		PXOpenGLTextureBindFunction PXOpenGLTextureBindCallBack;
		PXOpenGLTextureDeleteFunction PXOpenGLTextureDeleteCallBack;
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.2.1.0>------------------------------------------------------
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.3.0.0>------------------------------------------------------
		PXOpenGLFrameBufferCreateFunction PXOpenGLFrameBufferCreateCallBack;
		PXOpenGLFrameBufferDeleteFunction PXOpenGLFrameBufferDeleteCallBack;
		PXOpenGLFrameBufferBindFunction PXOpenGLFrameBufferBindCallBack;

		PXOpenGLRenderBufferCreateFunction PXOpenGLRenderBufferCreateCallBack;
		PXOpenGLRenderBufferBindFunction  PXOpenGLRenderBufferBindCallBack;
		PXOpenGLRenderBufferDeleteFunction PXOpenGLRenderBufferDeleteCallBack;
		PXOpenGLRenderBufferStorageFunction PXOpenGLRenderBufferStorageCallBack;
		PXOpenGLFrameBufferLinkTexture2DFunction PXOpenGLFrameBufferLinkTexture2DCallBack;
		PXOpenGLFrameBufferLinkRenderBufferFunction PXOpenGLFrameBufferLinkRenderBufferCallBack;

		PXOpenGLGenVertexArraysFunction PXOpenGLGenVertexArraysCallBack;
		PXOpenGLBindVertexArrayFunction PXOpenGLBindVertexArrayCallBack;
		PXOpenGLVertexAttribIPointerFunction PXOpenGLVertexAttribIPointerCallBack;

		PXOpenGLStringIFunction PXOpenGLStringICallBack;
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.3.1.0>------------------------------------------------------
		PXOpenGLDrawArraysInstancedFunction PXOpenGLDrawArraysInstancedCallBack;
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.3.2.0>------------------------------------------------------
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.3.3.0>------------------------------------------------------
		PXOpenGLVertexAttribDivisorFunction PXOpenGLVertexAttribDivisorCallBack;
		//-------------------------------------------------------------------------		

		//---<PXOpenGL v.4.0.0>------------------------------------------------------
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.4.1.0>------------------------------------------------------
		PXOpenGLVertexAttribLPointerFunction	PXOpenGLVertexAttribLPointerCallBack;
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.4.2.0>------------------------------------------------------
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.4.3.0>------------------------------------------------------
		PXOpenGLDebugMessageFunction PXOpenGLDebugMessageCallback;
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.4.4.0>------------------------------------------------------
		//-------------------------------------------------------------------------

		//---<PXOpenGL v.4.5.0>------------------------------------------------------

		//-------------------------------------------------------------------------

		//---<PXOpenGL v.4.6.0>------------------------------------------------------
		//-------------------------------------------------------------------------
	}
	PXOpenGLContext;

	//---<Utility>-------------------------------------------------------------
	PXPublic void PXOpenGLContextConstruct(PXOpenGLContext* const openGLContext);
	PXPublic void PXOpenGLContextDestruct(PXOpenGLContext* const openGLContext);


	PXPublic void PXOpenGLContextSet(PXOpenGLContext* const openGLContext, const PXOpenGLContext* const openGLContextSoure);
	PXPublic void PXOpenGLContextCopy(PXOpenGLContext* const openGLContext, const PXOpenGLContext* const openGLContextSoure);

	PXPublic PXBool PXOpenGLContextCreateForWindow(PXOpenGLContext* const openGLContext);
	PXPublic void PXOpenGLContextCreateWindowless(PXOpenGLContext* const openGLContext, const PXSize width, const PXSize height);
	PXPublic void PXOpenGLContextSelect(PXOpenGLContext* const openGLContext);
	PXPublic PXBool PXOpenGLContextDeselect(PXOpenGLContext* const openGLContext);
	PXPublic void PXOpenGLContextRelease(PXOpenGLContext* const openGLContext);

	PXPublic void PXOpenGLRenderBufferSwap(PXOpenGLContext* const openGLContext);

	PXPublic void PXOpenGLFlush(PXOpenGLContext* const openGLContext);
	PXPublic void PXOpenGLViewSize(PXOpenGLContext* const openGLContext, const PXSize x, const PXSize y, const PXSize width, const PXSize height);


	PXPublic void PXOpenGLPolygonRenderOrder(const PXOpenGLContext* const openGLContext, const PXOpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode);

	PXPublic void PXOpenGLSettingChange(PXOpenGLContext* const openGLContext, const PXOpenGLToggle toggle, const PXBool state);

	PXPublic void PXOpenGLDrawOrder(PXOpenGLContext* const openGLContext, const PXOpenGLDrawOrderMode openGLDrawOrderMode);

	PXPublic void PXOpenGLClearColor(PXOpenGLContext* const openGLContext, const float red, const float green, const float blue, const float alpha);
	PXPublic void PXOpenGLClear(PXOpenGLContext* const openGLContext, const unsigned int clearID);
	PXPublic void PXOpenGLDrawScaleF(PXOpenGLContext* const openGLContext, const float x, const float y, const float z);
	PXPublic void PXOpenGLDrawBegin(PXOpenGLContext* const openGLContext, const PXOpenGLRenderMode openGLRenderMode);
	PXPublic void PXOpenGLDrawVertexXYZF(PXOpenGLContext* const openGLContext, const float x, const float y, const float z);
	PXPublic void PXOpenGLDrawColorRGBF(PXOpenGLContext* const openGLContext, const float red, const float green, const float blue);
	PXPublic void PXOpenGLDrawEnd(PXOpenGLContext* const openGLContext);

	PXPublic void PXOpenGLTextureParameter(PXOpenGLContext* const openGLContext, const PXOpenGLTextureType textureType, const PXOpenGLTextureParameterMode pname, const PXOpenGLTextureParameterValue openGLTextureParameterValue);
	PXPublic void PXOpenGLTextureParameterI(PXOpenGLContext* const openGLContext, const PXOpenGLTextureType textureType, const PXOpenGLTextureParameterMode pname, const int param);
	PXPublic void PXOpenGLTextureParameterF(PXOpenGLContext* const openGLContext, const PXOpenGLTextureType textureType, const PXOpenGLTextureParameterMode pname, const float param);

	PXPrivate PXOpenGLVersion PXOpenGLVersionParse(const PXInt32U versionID);
	PXPrivate void PXOpenGLCacheFunction(void** loadList, PXSize* currentSize, char* name, void* functionADress);
	PXPrivate const void* const PXOpenGLFunctionAdressFetch(const char* const functionName);

	PXPublic void PXOpenGLAPICallType PXOpenGLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	//-------------------------------------------------------------------------

	// render

	PXPublic void PXOpenGLDrawArrays(const PXOpenGLContext* const openGLContext, const PXOpenGLRenderMode renderMode, const PXSize offset, const PXSize amount);
	PXPublic void PXOpenGLDrawElements(const PXOpenGLContext* const openGLContext, const PXOpenGLRenderMode renderMode, const PXSize amount, const PXOpenGLDataType openGLDataType, const void* const indexList);


	
	PXPublic void PXOpenGLDrawArraysInstanced(const PXOpenGLContext* const openGLContext, const PXOpenGLRenderMode renderMode, const PXSize startOffset, const PXSize amount, const PXSize instanceAmount);

	//---<Texture>-------------------------------------------------------------
	PXPublic void PXOpenGLTextureActivate(PXOpenGLContext* const openGLContext, const unsigned int index);
	PXPublic void PXOpenGLTextureCreate(PXOpenGLContext* const openGLContext, GLsizei n, GLuint* textures); // glGenTextures
	PXPublic void PXOpenGLTextureBind(PXOpenGLContext* const openGLContext, const PXOpenGLTextureType textureType, GLuint texture); // glBindTexture
	PXPublic void PXOpenGLTextureUnbind(PXOpenGLContext* const openGLContext, const PXOpenGLTextureType textureType);
	PXPublic void PXOpenGLTextureDelete(PXOpenGLContext* const openGLContext, GLsizei n, const GLuint* textures); // glDeleteTextures
	PXPublic void PXOpenGLTextureData2D
	(
		PXOpenGLContext* const openGLContext,
		const PXOpenGLTextureType glTextureType,
		const unsigned int level,
		const PXOpenGLImageFormat glImageFormatInternal,
		const PXSize width,
		const PXSize height,
		const PXOpenGLImageFormat glImageFormatInput,
		const PXOpenGLDataType glDataType,
		const void* const imageData
	);
	//-------------------------------------------------------------------------


	//---<Shader>-----------------------------------------------------------------
	PXPublic unsigned int PXOpenGLShaderTypeToID(const PXOpenGLShaderType openGLShaderType);
	PXPublic PXOpenGLShaderID PXOpenGLShaderCreate(PXOpenGLContext* const openGLContext, const PXOpenGLShaderType openGLShaderType);
	PXPublic void PXOpenGLShaderSource(PXOpenGLContext* const openGLContext, const PXOpenGLShaderID shaderID, const PXSize count, const char** string, PXSize* const length);
	PXPublic unsigned char PXOpenGLShaderCompile(PXOpenGLContext* const openGLContext, const PXOpenGLShaderID shaderID);
	PXPublic void PXOpenGLShaderGetiv(PXOpenGLContext* const openGLContext, const PXOpenGLShaderID shaderID, GLenum pname, GLint* params);
	PXPublic void PXOpenGLShaderLogInfoGet(PXOpenGLContext* const openGLContext, const PXOpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog);
	PXPublic void PXOpenGLShaderDelete(PXOpenGLContext* const openGLContext, const PXOpenGLShaderID shaderID);

	PXPublic PXOpenGLShaderPXProgramID PXOpenGLShaderPXProgramCreate(PXOpenGLContext* const openGLContext);
	PXPublic void PXOpenGLShaderPXProgramUse(PXOpenGLContext* const openGLContext, const PXOpenGLShaderPXProgramID shaderPXProgramID);
	PXPublic void PXOpenGLShaderPXProgramDelete(PXOpenGLContext* const openGLContext, const PXOpenGLShaderPXProgramID shaderPXProgramID);

	PXPublic void PXOpenGLShaderPXProgramAttach(PXOpenGLContext* const openGLContext, const PXOpenGLShaderPXProgramID shaderPXProgramID, const PXOpenGLShaderID shaderID);
	PXPublic void PXOpenGLShaderPXProgramLink(PXOpenGLContext* const openGLContext, const PXOpenGLShaderID shaderID);
	PXPublic void PXOpenGLShaderPXProgramValidate(PXOpenGLContext* const openGLContext, const PXOpenGLShaderID shaderID);

	PXPublic GLint PXOpenGLShaderVariableIDGet(PXOpenGLContext* const openGLContext, GLuint program, const char* name);
	PXPublic void PXOpenGLShaderVariableFx1(PXOpenGLContext* const openGLContext, GLint location, GLfloat v0);
	PXPublic void PXOpenGLShaderVariableFx1xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	PXPublic void PXOpenGLShaderVariableIx1(PXOpenGLContext* const openGLContext, GLint location, GLint v0);
	PXPublic void PXOpenGLShaderVariableIx1xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	PXPublic void PXOpenGLShaderVariableFx2(PXOpenGLContext* const openGLContext, GLint location, GLfloat v0, GLfloat v1);
	PXPublic void PXOpenGLShaderVariableFx2xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	PXPublic void PXOpenGLShaderVariableIx2(PXOpenGLContext* const openGLContext, GLint location, GLint v0, GLint v1);
	PXPublic void PXOpenGLShaderVariableIx2xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	PXPublic void PXOpenGLShaderVariableFx3(PXOpenGLContext* const openGLContext, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
	PXPublic void PXOpenGLShaderVariableFx3xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	PXPublic void PXOpenGLShaderVariableIx3(PXOpenGLContext* const openGLContext, GLint location, GLint v0, GLint v1, GLint v2);
	PXPublic void PXOpenGLShaderVariableIx3xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	PXPublic void PXOpenGLShaderVariableFx4(PXOpenGLContext* const openGLContext, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
	PXPublic void PXOpenGLShaderVariableFx4xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLfloat* value);
	PXPublic void PXOpenGLShaderVariableIx4(PXOpenGLContext* const openGLContext, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
	PXPublic void PXOpenGLShaderVariableIx4xN(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, const GLint* value);
	PXPublic void PXOpenGLShaderVariableMatrix2fv(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	PXPublic void PXOpenGLShaderVariableMatrix3fv(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	PXPublic void PXOpenGLShaderVariableMatrix4fv(PXOpenGLContext* const openGLContext, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
	//-------------------------------------------------------------------------

	//---<Buffers>-------------------------------------------------------------
	PXPublic void PXOpenGLVertexArrayGenerate(PXOpenGLContext* const openGLContext, const unsigned int amount, unsigned int* const vaoList);
	PXPublic void PXOpenGLVertexArrayBind(PXOpenGLContext* const openGLContext, const unsigned int vaoID);
	PXPublic void PXOpenGLVertexArrayUnbind(PXOpenGLContext* const openGLContext);
	PXPublic void PXOpenGLVertexArrayAttributeDefine
	(
		PXOpenGLContext* const openGLContext,
		const unsigned int index,
		const unsigned int size,
		const PXOpenGLDataType datatype,
		const unsigned char normalized,
		const unsigned int stride,
		const PXSize offset
	);
	PXPublic void PXOpenGLVertexAttributeDivisor(PXOpenGLContext* const openGLContext, const PXSize index, const PXSize divisor);
	PXPublic void PXOpenGLVertexArrayEnable(PXOpenGLContext* const openGLContext, const unsigned int vertexArrayAtributeID);
	PXPublic void PXOpenGLVertexArrayDisable(PXOpenGLContext* const openGLContext, const unsigned int vertexArrayAtributeID);

	PXPublic void PXOpenGLBufferGenerate(PXOpenGLContext* const openGLContext, const unsigned int amount, unsigned int* const bufferIDList);
	PXPublic void PXOpenGLBufferBind(PXOpenGLContext* const openGLContext, const PXOpenGLBufferType bufferType, const unsigned int bufferID);
	PXPublic void PXOpenGLBufferData(PXOpenGLContext* const openGLContext, const PXOpenGLBufferType bufferType, const unsigned int size, const void* const data, const PXOpenGLStoreMode openGLStoreMode);
	PXPublic void PXOpenGLBufferUnbind(PXOpenGLContext* const openGLContext, const PXOpenGLBufferType bufferType);
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	PXPublic void PXOpenGLFrameBufferCreate(PXOpenGLContext* const openGLContext, const unsigned int amount, unsigned int* const framebufferIDList);
	PXPublic void PXOpenGLRenderBufferStorage(PXOpenGLContext* const openGLContext, const PXOpenGLRenderBufferFormat internalformat, const int width, const int height);

	// If we bind to 0, we select the "main window"
	PXPublic void PXOpenGLFrameBufferBind(PXOpenGLContext* const openGLContext, const PXOpenGLFrameBufferMode target, const unsigned int framebufferID);

	PXPublic void PXOpenGLFrameBufferDestroy(PXOpenGLContext* const openGLContext, const unsigned int amount, unsigned int* const framebufferIDList);

	PXPublic void PXOpenGLRenderBufferCreate(PXOpenGLContext* const openGLContext, GLsizei n, GLuint* renderbuffers);


	PXPublic void PXOpenGLRenderBufferBind(PXOpenGLContext* const openGLContext, const unsigned int renderbuffer);

	PXPublic void PXOpenGLRenderBufferDelete(PXOpenGLContext* const openGLContext, GLsizei n, GLuint* renderbuffers);


	PXPublic void PXOpenGLFrameBufferLinkTexture2D(PXOpenGLContext* const openGLContext, const PXOpenGLRenderBufferAttachmentPoint attachment, const PXOpenGLTextureType textarget, const unsigned int textureID, const int level);
	PXPublic GLuint PXOpenGLFrameBufferLinkRenderBuffer(PXOpenGLContext* const openGLContext, const PXOpenGLRenderBufferAttachmentPoint attachment, const unsigned int renderbuffer);
	//-------------------------------------------------------------------------


	//---<PXOpenGL v.1.0.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.1.1.0>------------------------------------------------------
	PXPublic void PXOpenGLPixelDataRead
	(
		PXOpenGLContext* const openGLContext,
		const int x,
		const int y,
		const int width,
		const int height,
		PXOpenGLImageFormat imageFormat,
		PXOpenGLDataType openGLDataType,
		void* const pixelData
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
PXPublic const char* PXOpenGLStringGetI(PXOpenGLContext* const openGLContext, const PXOpenGLStringName stringName, const unsigned int index);
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






	/*
	CPublic PXOpenGLID PXOpenGLToRenderMode(const PXGraphicRenderMode renderMode);
	CPublic PXOpenGLID PXOpenGLToShaderType(const ShaderType shaderType);
	CPublic PXOpenGLID PXOpenGLToImageFormat(const ImageDataFormat imageFormat);
	CPublic PXOpenGLID PXOpenGLToImageType(const PXGraphicImageType imageType);
	CPublic PXOpenGLID PXOpenGLToImageWrap(const PXGraphicImageWrap imageWrap);
	CPublic PXOpenGLID PXOpenGLToImageLayout(const PXGraphicImageLayout layout);











	CPublic void PXOpenGLVertexAttributeArrayDefine(PXOpenGL* const openGL, const PXSize sizeOfElement, const PXSize listSize, const unsigned int* list);

	CPublic void VertexArrayDefine(unsigned int* vertexArrayID);
	CPublic void VertexDataDefine(unsigned int* vertexID, int size, void* data);
	CPublic void IndexDataDefine(unsigned int* indexID, int size, void* data);
	CPublic void VertexArrayUpdate(int vertexArrayID, int size, void* data);
	*/

#ifdef __cplusplus
}
#endif

#endif
