#ifndef PXOpenGLINCLUDE
#define PXOpenGLINCLUDE

#include <Media/PXResource.h>
#include <OS/Library/PXLibrary.h>

#define PXOpenGLForceLegacy 0

//---<PXOpenGL>----------------
#include <GL/gl.h>

#if OSUnix
#include <GL/glx.h>
#define PXOpenGLAPI GLAPIENTRY
typedef GLXContext PXOpenGLConextID;
#elif OSWindows
#define PXOpenGLAPI APIENTRY
typedef HGLRC PXOpenGLConextID;
#endif
//---------------------------


typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;


typedef struct PXGraphicInitializeInfo_ PXGraphicInitializeInfo;


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
	PXOpenGLVersion4x5x0, // 2014 August   11. GLSL 4.50, Direct State Access(DSA), Flush Control, Robustness, PXOpenGL ES 3.1 API and shader compatibility, DX11 emulation features
	PXOpenGLVersion4x6x0  // 2017 July     31. GLSL 4.60, More efficient geometry processing and shader execution, more information, no error context, polygon offset clamp, SPIR-V, anisotropic filtering
}
PXOpenGLVersion;


typedef enum PXOpenGLFrameBufferMode_
{
	PXOpenGLFrameBufferModeInvalid,
	PXOpenGLFrameBufferModeDraw,
	PXOpenGLFrameBufferModeRead,
	PXOpenGLFrameBufferModeDrawAndRead
}
PXOpenGLFrameBufferMode;


typedef enum PXOpenGLRenderBufferAttachmentPoint_
{
	PXOpenGLRenderBufferAttachmentPointInvalid,
	PXOpenGLRenderBufferAttachmentPointColor,
	PXOpenGLRenderBufferAttachmentPointDepth,
	PXOpenGLRenderBufferAttachmentPointStencil
}
PXOpenGLRenderBufferAttachmentPoint;

PXPrivate unsigned int PXAPI PXOpenGLRenderBufferAttachmentPointToID(const PXOpenGLRenderBufferAttachmentPoint renderBufferAttachmentPoint);


typedef enum PXOpenGLPolygonRenderOrderMode_
{
	PXOpenGLPolygonRenderOrderModeInvalid,
	PXOpenGLPolygonRenderOrderModeClockwise,
	PXOpenGLPolygonRenderOrderModeCounterClockwise,
}
PXOpenGLPolygonRenderOrderMode;

PXPrivate GLenum PXAPI PXOpenGLPolygonRenderOrderModeToID(const PXOpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode);

typedef enum PXOpenGLRenderBufferFormat_
{
	PXOpenGLRenderBufferFormatInvalid,
	PXOpenGLRenderBufferFormatDepthComponent,
	PXOpenGLRenderBufferFormatDepth24Stencil8
}
PXOpenGLRenderBufferFormat;

PXPrivate int PXAPI PXOpenGLRenderBufferFormatToID(const PXOpenGLRenderBufferFormat dataType);


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

PXPrivate GLenum PXAPI PXOpenGLTextureParameterModeToID(const PXOpenGLTextureParameterMode textureParameterMode);






typedef enum PXOpenGLBufferType_
{
	PXOpenGLBufferArray,//Vertex attributes
	PXOpenGLBufferAtomicCounter, 	//Atomic counter storage
	PXOpenGLBufferCopyRead,//Buffer copy source
	PXOpenGLBufferCopyWrite,//Buffer copy destination
	PXOpenGLBufferDispatchIndirect, //	Indirect compute dispatch commands
	PXOpenGLBufferDrawIndirect,	//Indirect command arguments
	PXOpenGLBufferElementArray,	//Vertex array indices
	PXOpenGLBufferPixelPack,//Pixel read target
	PXOpenGLBufferPixelUnpack,//	Texture data source
	PXOpenGLBufferQuery,//Query result buffer
	PXOpenGLBufferShaderStorage,	//Read - write storage for shaders
	PXOpenGLBufferTexture,// Texture data buffer
	PXOpenGLBufferTransformFeedback,//	Transform feedback buffer
	PXOpenGLBufferUniform
}
PXOpenGLBufferType;

PXPrivate int PXAPI PXOpenGLBufferTypeToID(const PXOpenGLBufferType openGLBufferType);

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

PXPrivate GLint PXAPI PXOpenGLTextureParameterValueToID(const PXOpenGLTextureParameterValue openGLTextureParameterValue);


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

PXPrivate int PXAPI PXOpenGLStoreModeToID(const PXOpenGLStoreMode openGLStoreMode);


typedef enum PXOpenGLDrawOrderMode_
{
	PXOpenGLDrawOrderModeInvalid,
	PXOpenGLDrawOrderModeClockwise,
	PXOpenGLDrawOrderModeCounterClockwise
}
PXOpenGLDrawOrderMode;


PXPrivate GLenum PXAPI PXOpenGLDrawOrderToID(const PXOpenGLDrawOrderMode openGLDrawOrderMode);


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

PXPrivate int PXAPI PXOpenGLToggleToID(const PXOpenGLToggle openGLToggle);




#if 0
DECLARE_HANDLE(HGPUNV);

typedef struct _GPU_DEVICE
{
	DWORD  cb;
	CHAR   DeviceName[32];
	CHAR   DeviceString[128];
	DWORD  Flags;
	RECT   rcVirtualScreen;
}
GPU_DEVICE,
* PGPU_DEVICE;

typedef BOOL(PXOpenGLAPI* wglEnumGpusNVFunction)(UINT iGpuIndex, HGPUNV* phGpu);
typedef BOOL(PXOpenGLAPI* wglEnumGpuDevicesNVFunction)(HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice);
typedef HDC(PXOpenGLAPI* wglCreateAffinityDCNVFunction)(const HGPUNV* phGpuList);
typedef BOOL(PXOpenGLAPI* wglEnumGpusFromAffinityDCNVFunction)(HDC hAffinityDC, UINT iGpuIndex, HGPUNV* hGpu);
typedef BOOL(PXOpenGLAPI* wglDeleteDCNVFunction)(HDC hdc);
#endif




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

//---<PXOpenGL v.2.0.0>-----------------------------------------------------
typedef unsigned int (PXOpenGLAPI* PXOpenGLShaderPXProgramCreateFunction)();
typedef void (PXOpenGLAPI* PXOpenGLShaderPXProgramUseFunction)(unsigned int program);
typedef void (PXOpenGLAPI* PXOpenGLShaderPXProgramDeleteFunction)(GLuint program); // glDeletePXProgram
typedef void (PXOpenGLAPI* PXOpenGLShaderSourceFunction)(unsigned int shader, int count, const char** string, const int* length);
typedef GLuint(PXOpenGLAPI* PXOpenGLShaderCreateFunction)(GLenum shaderType); // glCreateShader
typedef void (PXOpenGLAPI* PXOpenGLShaderCompileFunction)(GLuint shader);
typedef void (PXOpenGLAPI* PXOpenGLShaderGetivFunction)(GLuint shader, GLenum pname, GLint* params);

typedef void  (PXOpenGLAPI* PXOpenGLActiveAttributeGet)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, char* name);

typedef void (PXOpenGLAPI* PXOpenGLGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLint* size, GLenum* type, char* name);

typedef void (PXOpenGLAPI* PXOpenGLGetProgramiv)(GLuint program, GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLProgramInfoLogGet)(GLuint program, GLsizei maxLength, GLsizei* length, char* infoLog); // glGetProgramInfoLog


typedef void (PXOpenGLAPI* PXOpenGLShaderLogInfoGetFunction)(GLuint shader, GLsizei maxLength, GLsizei* length, char* infoLog);
typedef void (PXOpenGLAPI* PXOpenGLShaderDeleteFunction)(GLuint shader);
typedef void (PXOpenGLAPI* PXOpenGLAttachShaderFunction)(GLuint program, GLuint shader); // glAttachShader
typedef void (PXOpenGLAPI* PXOpenGLLinkPXProgramFunction)(GLuint program); // glLinkPXProgram
typedef void (PXOpenGLAPI* PXOpenGLValidatePXProgramFunction)(GLuint program); // glValidatePXProgram
typedef void (PXOpenGLAPI* PXOpenGLActiveTextureFunction)(GLenum texture); // glActiveTexture
typedef void (PXOpenGLAPI* PXOpenGLGenBuffersFunction)(GLsizei n, GLuint* buffers); // glGenBuffers
typedef void (PXOpenGLAPI* PXOpenGLBindBufferFunction)(GLenum target, GLuint buffer);
typedef void (PXOpenGLAPI* PXOpenGLBufferDataFunction)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);

typedef void (PXOpenGLAPI* PXOpenGLVertexAttribPointerFunction)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void (PXOpenGLAPI* PXOpenGLVertexAttribArrayEnableFunction)(GLuint index); // glEnableVertexAttribArray
typedef void (PXOpenGLAPI* PXOpenGLVertexAttribArrayDisableFunction)(GLuint index); // glDisableVertexAttribArray

typedef void (PXOpenGLAPI* PXOpenGLDisableVertexArrayAttribFunction)(GLuint vaobj, GLuint index);
typedef GLint(PXOpenGLAPI* PXOpenGLGetUniformLocation)(GLuint program, const char* name);
typedef void (PXOpenGLAPI* PXOpenGLUniform1fFunction)(GLint location, float v0);
typedef void (PXOpenGLAPI* PXOpenGLUniform1fvFunction)(GLint location, GLsizei count, const float* value);
typedef void (PXOpenGLAPI* PXOpenGLUniform1iFunction)(GLint location, GLint v0);
typedef void (PXOpenGLAPI* PXOpenGLUniform1ivFunction)(GLint location, GLsizei count, const GLint* value);
typedef void (PXOpenGLAPI* PXOpenGLUniform2fFunction)(GLint location, float v0, float v1);
typedef void (PXOpenGLAPI* PXOpenGLUniform2fvFunction)(GLint location, GLsizei count, const float* value);
typedef void (PXOpenGLAPI* PXOpenGLUniform2iFunction)(GLint location, GLint v0, GLint v1);
typedef void (PXOpenGLAPI* PXOpenGLUniform2ivFunction)(GLint location, GLsizei count, const GLint* value);
typedef void (PXOpenGLAPI* PXOpenGLUniform3fFunction)(GLint location, float v0, float v1, float v2);
typedef void (PXOpenGLAPI* PXOpenGLUniform3fvFunction)(GLint location, GLsizei count, const float* value);
typedef void (PXOpenGLAPI* PXOpenGLUniform3iFunction)(GLint location, GLint v0, GLint v1, GLint v2);
typedef void (PXOpenGLAPI* PXOpenGLUniform3ivFunction)(GLint location, GLsizei count, const GLint* value);
typedef void (PXOpenGLAPI* PXOpenGLUniform4fFunction)(GLint location, float v0, float v1, float v2, float v3);
typedef void (PXOpenGLAPI* PXOpenGLUniform4fvFunction)(GLint location, GLsizei count, const float* value);
typedef void (PXOpenGLAPI* PXOpenGLUniform4iFunction)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (PXOpenGLAPI* PXOpenGLUniform4ivFunction)(GLint location, GLsizei count, const GLint* value);
typedef void (PXOpenGLAPI* PXOpenGLUniformMatrix2fvFunction)(GLint location, GLsizei count, GLboolean transpose, const float* value);
typedef void (PXOpenGLAPI* PXOpenGLUniformMatrix3fvFunction)(GLint location, GLsizei count, GLboolean transpose, const float* value);
typedef void (PXOpenGLAPI* PXOpenGLUniformMatrix4fvFunction)(GLint location, GLsizei count, GLboolean transpose, const float* value);

//typedef void (PXOpenGLAPI* PXOpenGLTextureCreateFunction)(GLsizei n, GLuint* textures); // glGenTextures
//typedef void (PXOpenGLAPI* PXOpenGLTextureBindFunction)(GLenum target, GLuint texture); // glBindTexture
//typedef void (PXOpenGLAPI* PXOpenGLTextureDeleteFunction)(GLsizei n, const GLuint* textures); // glDeleteTextures
//-------------------------------------------------------------------------

//---<PXOpenGL v.2.1.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<PXOpenGL v.3.0.0>------------------------------------------------------
typedef void (PXOpenGLAPI* PXOpenGLFrameBufferCreateFunction)(GLsizei n, GLuint* ids); // glGenFramebuffers
typedef void (PXOpenGLAPI* PXOpenGLFrameBufferDeleteFunction)(GLsizei n, GLuint* framebuffers); // glDeleteFramebuffers
typedef void (PXOpenGLAPI* PXOpenGLFrameBufferBindFunction)(GLenum target, GLuint framebuffer); // glBindFramebuffer

typedef void (PXOpenGLAPI* PXOpenGLRenderBufferCreateFunction)(GLsizei n, GLuint* renderbuffers); // glGenRenderbuffers
typedef void (PXOpenGLAPI* PXOpenGLRenderBufferBindFunction)(GLenum target, GLuint renderbuffer); // glBindRenderbuffer
typedef void (PXOpenGLAPI* PXOpenGLRenderBufferDeleteFunction)(GLsizei n, GLuint* renderbuffers); // glDeleteRenderbuffers
typedef void (PXOpenGLAPI* PXOpenGLRenderBufferStorageFunction)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height); // glRenderbufferStorage

typedef void (PXOpenGLAPI* PXOpenGLFrameBufferLinkTexture2DFunction)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef GLuint(PXOpenGLAPI* PXOpenGLFrameBufferLinkRenderBufferFunction)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

typedef void (PXOpenGLAPI* PXOpenGLGenVertexArraysFunction)(GLsizei n, GLuint* arrays);
typedef void (PXOpenGLAPI* PXOpenGLBindVertexArrayFunction)(GLuint arrayID);
typedef void (PXOpenGLAPI* PXOpenGLVertexAttribIPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef const GLubyte* (PXOpenGLAPI* PXOpenGLStringIFunction)(GLenum name, GLuint index); // glGetStringi

//-------------------------------------------------------------------------

//---<PXOpenGL v.3.1.0>------------------------------------------------------
typedef void (PXOpenGLAPI* PXOpenGLDrawArraysInstancedFunction)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount); // glDrawArraysInstanced
//-------------------------------------------------------------------------

//---<PXOpenGL v.3.2.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<PXOpenGL v.3.3.0>------------------------------------------------------
typedef void  (PXOpenGLAPI* PXOpenGLVertexAttribDivisorFunction)(GLuint index, GLuint divisor); // glVertexAttribDivisor
//-------------------------------------------------------------------------



//---<PXOpenGL v.4.0.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<PXOpenGL v.4.1.0>------------------------------------------------------
typedef void (PXOpenGLAPI* PXOpenGLVertexAttribLPointerFunction)(GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
//-------------------------------------------------------------------------

//---<PXOpenGL v.4.2.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<PXOpenGL v.4.3.0>------------------------------------------------------
typedef void (PXOpenGLAPI* DEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam);
typedef void (PXOpenGLAPI* PXOpenGLDebugMessageFunction)(DEBUGPROC callback, const void* userParam); // Set
//-------------------------------------------------------------------------

//---<PXOpenGL v.4.4.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<PXOpenGL v.4.5.0>------------------------------------------------------
typedef void (PXOpenGLAPI* PXOpenGLNamedBufferData)(GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
typedef void (PXOpenGLAPI* PXOpenGLEnableVertexAttribArray)(GLuint index);
typedef void (PXOpenGLAPI* PXOpenGLEnableVertexArrayAttrib)(GLuint vaobj, GLuint index);
//-------------------------------------------------------------------------

//---<PXOpenGL v.4.6.0>------------------------------------------------------
//-------------------------------------------------------------------------


//---<Extensions>----------------------------------------------------------



typedef void (PXOpenGLAPI* PXOpenGLSwapIntervalSetFunction)(GLint interval); // wglSwapIntervalEXT, glXSwapIntervalEXT
typedef GLint(PXOpenGLAPI* PXOpenGLSwapIntervalGetFunction)(); // wglGetSwapIntervalEXT, glxGetSwapIntervalEXT

#if OSUnix
typedef const char* (PXOpenGLAPI* PXOpenGLStringGetExtensionsARB)(void* hdc); // wglGetExtensionsStringARB
#elif OSWindows
typedef const char* (PXOpenGLAPI* PXOpenGLStringGetExtensionsARB)(HDC hdc); // wglGetExtensionsStringARB
#endif


typedef void (PXOpenGLAPI* PXOpenGLAccumFunction)(GLenum op, float value);
typedef void (PXOpenGLAPI* PXOpenGLAlphaFuncFunction)(GLenum func, GLclampf ref);
typedef GLboolean(PXOpenGLAPI* PXOpenGLAreTexturesResidentFunction)(GLsizei n, const GLuint* textures, GLboolean* residences);
typedef void (PXOpenGLAPI* PXOpenGLArrayElementFunction)(GLint i);
typedef void (PXOpenGLAPI* PXOpenGLBeginFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLBindTextureFunction)(GLenum target, GLuint texture);
typedef void (PXOpenGLAPI* PXOpenGLBitmapFunction)(GLsizei width, GLsizei height, float xorig, float yorig, float xmove, float ymove, const GLubyte* bitmap);
typedef void (PXOpenGLAPI* PXOpenGLBlendFuncFunction)(GLenum sfactor, GLenum dfactor);
typedef void (PXOpenGLAPI* PXOpenGLCallListFunction)(GLuint list);
typedef void (PXOpenGLAPI* PXOpenGLCallListsFunction)(GLsizei n, GLenum type, const GLvoid* lists);
typedef void (PXOpenGLAPI* PXOpenGLClearFunction)(GLbitfield mask);
typedef void (PXOpenGLAPI* PXOpenGLClearAccumFunction)(float red, float green, float blue, float alpha);
typedef void (PXOpenGLAPI* PXOpenGLClearColorFunction)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (PXOpenGLAPI* PXOpenGLClearDepthFunction)(GLclampd depth);
typedef void (PXOpenGLAPI* PXOpenGLClearIndexFunction)(float c);
typedef void (PXOpenGLAPI* PXOpenGLClearStencilFunction)(GLint s);
typedef void (PXOpenGLAPI* PXOpenGLClipPlaneFunction)(GLenum plane, const GLdouble* equation);
typedef void (PXOpenGLAPI* PXOpenGLColor3bFunction)(GLbyte red, GLbyte green, GLbyte blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3bvFunction)(const GLbyte* v);
typedef void (PXOpenGLAPI* PXOpenGLColor3dFunction)(GLdouble red, GLdouble green, GLdouble blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLColor3fFunction)(float red, float green, float blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLColor3iFunction)(GLint red, GLint green, GLint blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLColor3sFunction)(GLshort red, GLshort green, GLshort blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLColor3ubFunction)(GLubyte red, GLubyte green, GLubyte blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3ubvFunction)(const GLubyte* v);
typedef void (PXOpenGLAPI* PXOpenGLColor3uiFunction)(GLuint red, GLuint green, GLuint blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3uivFunction)(const GLuint* v);
typedef void (PXOpenGLAPI* PXOpenGLColor3usFunction)(GLushort red, GLushort green, GLushort blue);
typedef void (PXOpenGLAPI* PXOpenGLColor3usvFunction)(const GLushort* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4bFunction)(GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4bvFunction)(const GLbyte* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4dFunction)(GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4fFunction)(float red, float green, float blue, float alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4iFunction)(GLint red, GLint green, GLint blue, GLint alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4sFunction)(GLshort red, GLshort green, GLshort blue, GLshort alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4ubFunction)(GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4ubvFunction)(const GLubyte* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4uiFunction)(GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4uivFunction)(const GLuint* v);
typedef void (PXOpenGLAPI* PXOpenGLColor4usFunction)(GLushort red, GLushort green, GLushort blue, GLushort alpha);
typedef void (PXOpenGLAPI* PXOpenGLColor4usvFunction)(const GLushort* v);
typedef void (PXOpenGLAPI* PXOpenGLColorMaskFunction)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void (PXOpenGLAPI* PXOpenGLColorMaterialFunction)(GLenum face, GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLColorPointerFunction)(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void (PXOpenGLAPI* PXOpenGLCopyPixelsFunction)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
typedef void (PXOpenGLAPI* PXOpenGLCopyTexImage1DFunction)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (PXOpenGLAPI* PXOpenGLCopyTexImage2DFunction)(GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (PXOpenGLAPI* PXOpenGLCopyTexSubImage1DFunction)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (PXOpenGLAPI* PXOpenGLCopyTexSubImage2DFunction)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (PXOpenGLAPI* PXOpenGLCullFaceFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLDeleteListsFunction)(GLuint list, GLsizei range);
typedef void (PXOpenGLAPI* PXOpenGLDeleteTexturesFunction)(GLsizei n, const GLuint* textures);
typedef void (PXOpenGLAPI* PXOpenGLDepthFuncFunction)(GLenum func);
typedef void (PXOpenGLAPI* PXOpenGLDepthMaskFunction)(GLboolean flag);
typedef void (PXOpenGLAPI* PXOpenGLDepthRangeFunction)(GLclampd zNear, GLclampd zFar);
typedef void (PXOpenGLAPI* PXOpenGLDisableFunction)(GLenum cap);
typedef void (PXOpenGLAPI* PXOpenGLDisableClientStateFunction)(GLenum array);
typedef void (PXOpenGLAPI* PXOpenGLDrawArraysFunction)(GLenum mode, GLint first, GLsizei count);
typedef void (PXOpenGLAPI* PXOpenGLDrawBufferFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLDrawElementsFunction)(GLenum mode, GLsizei count, GLenum type, const GLvoid* indices);
typedef void (PXOpenGLAPI* PXOpenGLDrawPixelsFunction)(GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (PXOpenGLAPI* PXOpenGLEdgeFlagFunction)(GLboolean flag);
typedef void (PXOpenGLAPI* PXOpenGLEdgeFlagPointerFunction)(GLsizei stride, const GLvoid* pointer);
typedef void (PXOpenGLAPI* PXOpenGLEdgeFlagvFunction)(const GLboolean* flag);
typedef void (PXOpenGLAPI* PXOpenGLEnableFunction)(GLenum cap);
typedef void (PXOpenGLAPI* PXOpenGLEnableClientStateFunction)(GLenum array);
typedef void (PXOpenGLAPI* PXOpenGLEndFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLEndListFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord1dFunction)(GLdouble u);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord1dvFunction)(const GLdouble* u);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord1fFunction)(float u);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord1fvFunction)(const float* u);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord2dFunction)(GLdouble u, GLdouble v);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord2dvFunction)(const GLdouble* u);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord2fFunction)(float u, float v);
typedef void (PXOpenGLAPI* PXOpenGLEvalCoord2fvFunction)(const float* u);
typedef void (PXOpenGLAPI* PXOpenGLEvalMesh1Function)(GLenum mode, GLint i1, GLint i2);
typedef void (PXOpenGLAPI* PXOpenGLEvalMesh2Function)(GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
typedef void (PXOpenGLAPI* PXOpenGLEvalPoint1Function)(GLint i);
typedef void (PXOpenGLAPI* PXOpenGLEvalPoint2Function)(GLint i, GLint j);
typedef void (PXOpenGLAPI* PXOpenGLFeedbackBufferFunction)(GLsizei size, GLenum type, float* buffer);
typedef void (PXOpenGLAPI* PXOpenGLFinishFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLFlushFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLFogfFunction)(GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLFogfvFunction)(GLenum pname, const float* params);
typedef void (PXOpenGLAPI* PXOpenGLFogiFunction)(GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLFogivFunction)(GLenum pname, const GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLFrontFaceFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLFrustumFunction)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef GLuint(PXOpenGLAPI* PXOpenGLGenListsFunction)(GLsizei range);
typedef void (PXOpenGLAPI* PXOpenGLGenTexturesFunction)(GLsizei n, GLuint* textures);
typedef void (PXOpenGLAPI* PXOpenGLGetBooleanvFunction)(GLenum pname, GLboolean* params);
typedef void (PXOpenGLAPI* PXOpenGLGetClipPlaneFunction)(GLenum plane, GLdouble* equation);
typedef void (PXOpenGLAPI* PXOpenGLGetDoublevFunction)(GLenum pname, GLdouble* params);
typedef GLenum(PXOpenGLAPI* PXOpenGLGetErrorFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLGetFloatvFunction)(GLenum pname, float* params);
typedef void (PXOpenGLAPI* PXOpenGLGetIntegervFunction)(GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLGetLightfvFunction)(GLenum light, GLenum pname, float* params);
typedef void (PXOpenGLAPI* PXOpenGLGetLightivFunction)(GLenum light, GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLGetMapdvFunction)(GLenum target, GLenum query, GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLGetMapfvFunction)(GLenum target, GLenum query, float* v);
typedef void (PXOpenGLAPI* PXOpenGLGetMapivFunction)(GLenum target, GLenum query, GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLGetMaterialfvFunction)(GLenum face, GLenum pname, float* params);
typedef void (PXOpenGLAPI* PXOpenGLGetMaterialivFunction)(GLenum face, GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLGetPixelMapfvFunction)(GLenum map, float* values);
typedef void (PXOpenGLAPI* PXOpenGLGetPixelMapuivFunction)(GLenum map, GLuint* values);
typedef void (PXOpenGLAPI* PXOpenGLGetPixelMapusvFunction)(GLenum map, GLushort* values);
typedef void (PXOpenGLAPI* PXOpenGLGetPointervFunction)(GLenum pname, GLvoid** params);
typedef void (PXOpenGLAPI* PXOpenGLGetPolygonStippleFunction)(GLubyte* mask);
typedef const char* (PXOpenGLAPI* PXOpenGLGetStringFunction)(GLenum name);
typedef void (PXOpenGLAPI* PXOpenGLGetTexEnvfvFunction)(GLenum target, GLenum pname, float* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexEnvivFunction)(GLenum target, GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexGendvFunction)(GLenum coord, GLenum pname, GLdouble* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexGenfvFunction)(GLenum coord, GLenum pname, float* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexGenivFunction)(GLenum coord, GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexImageFunction)(GLenum target, GLint level, GLenum format, GLenum type, GLvoid* pixels);
typedef void (PXOpenGLAPI* PXOpenGLGetTexLevelParameterfvFunction)(GLenum target, GLint level, GLenum pname, float* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexLevelParameterivFunction)(GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexParameterfvFunction)(GLenum target, GLenum pname, float* params);
typedef void (PXOpenGLAPI* PXOpenGLGetTexParameterivFunction)(GLenum target, GLenum pname, GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLHintFunction)(GLenum target, GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLIndexMaskFunction)(GLuint mask);
typedef void (PXOpenGLAPI* PXOpenGLIndexPointerFunction)(GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void (PXOpenGLAPI* PXOpenGLIndexdFunction)(GLdouble c);
typedef void (PXOpenGLAPI* PXOpenGLIndexdvFunction)(const GLdouble* c);
typedef void (PXOpenGLAPI* PXOpenGLIndexfFunction)(float c);
typedef void (PXOpenGLAPI* PXOpenGLIndexfvFunction)(const float* c);
typedef void (PXOpenGLAPI* PXOpenGLIndexiFunction)(GLint c);
typedef void (PXOpenGLAPI* PXOpenGLIndexivFunction)(const GLint* c);
typedef void (PXOpenGLAPI* PXOpenGLIndexsFunction)(GLshort c);
typedef void (PXOpenGLAPI* PXOpenGLIndexsvFunction)(const GLshort* c);
typedef void (PXOpenGLAPI* PXOpenGLIndexubFunction)(GLubyte c);
typedef void (PXOpenGLAPI* PXOpenGLIndexubvFunction)(const GLubyte* c);
typedef void (PXOpenGLAPI* PXOpenGLInitNamesFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLInterleavedArraysFunction)(GLenum format, GLsizei stride, const GLvoid* pointer);
typedef GLboolean(PXOpenGLAPI* PXOpenGLIsEnabledFunction)(GLenum cap);
typedef GLboolean(PXOpenGLAPI* PXOpenGLIsListFunction)(GLuint list);
typedef GLboolean(PXOpenGLAPI* PXOpenGLIsTextureFunction)(GLuint texture);
typedef void (PXOpenGLAPI* PXOpenGLLightModelfFunction)(GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLLightModelfvFunction)(GLenum pname, const float* params);
typedef void (PXOpenGLAPI* PXOpenGLLightModeliFunction)(GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLLightModelivFunction)(GLenum pname, const GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLLightfFunction)(GLenum light, GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLLightfvFunction)(GLenum light, GLenum pname, const float* params);
typedef void (PXOpenGLAPI* PXOpenGLLightiFunction)(GLenum light, GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLLightivFunction)(GLenum light, GLenum pname, const GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLLineStippleFunction)(GLint factor, GLushort pattern);
typedef void (PXOpenGLAPI* PXOpenGLLineWidthFunction)(float width);
typedef void (PXOpenGLAPI* PXOpenGLListBaseFunction)(GLuint base);
typedef void (PXOpenGLAPI* PXOpenGLLoadIdentityFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLLoadMatrixdFunction)(const GLdouble* m);
typedef void (PXOpenGLAPI* PXOpenGLLoadMatrixfFunction)(const float* m);
typedef void (PXOpenGLAPI* PXOpenGLLoadNameFunction)(GLuint name);
typedef void (PXOpenGLAPI* PXOpenGLLogicOpFunction)(GLenum opcode);
typedef void (PXOpenGLAPI* PXOpenGLMap1dFunction)(GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
typedef void (PXOpenGLAPI* PXOpenGLMap1fFunction)(GLenum target, float u1, float u2, GLint stride, GLint order, const float* points);
typedef void (PXOpenGLAPI* PXOpenGLMap2dFunction)(GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
typedef void (PXOpenGLAPI* PXOpenGLMap2fFunction)(GLenum target, float u1, float u2, GLint ustride, GLint uorder, float v1, float v2, GLint vstride, GLint vorder, const float* points);
typedef void (PXOpenGLAPI* PXOpenGLMapGrid1dFunction)(GLint un, GLdouble u1, GLdouble u2);
typedef void (PXOpenGLAPI* PXOpenGLMapGrid1fFunction)(GLint un, float u1, float u2);
typedef void (PXOpenGLAPI* PXOpenGLMapGrid2dFunction)(GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
typedef void (PXOpenGLAPI* PXOpenGLMapGrid2fFunction)(GLint un, float u1, float u2, GLint vn, float v1, float v2);
typedef void (PXOpenGLAPI* PXOpenGLMaterialfFunction)(GLenum face, GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLMaterialfvFunction)(GLenum face, GLenum pname, const float* params);
typedef void (PXOpenGLAPI* PXOpenGLMaterialiFunction)(GLenum face, GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLMaterialivFunction)(GLenum face, GLenum pname, const GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLMatrixModeFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLMultMatrixdFunction)(const GLdouble* m);
typedef void (PXOpenGLAPI* PXOpenGLMultMatrixfFunction)(const float* m);
typedef void (PXOpenGLAPI* PXOpenGLNewListFunction)(GLuint list, GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLNormal3bFunction)(GLbyte nx, GLbyte ny, GLbyte nz);
typedef void (PXOpenGLAPI* PXOpenGLNormal3bvFunction)(const GLbyte* v);
typedef void (PXOpenGLAPI* PXOpenGLNormal3dFunction)(GLdouble nx, GLdouble ny, GLdouble nz);
typedef void (PXOpenGLAPI* PXOpenGLNormal3dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLNormal3fFunction)(float nx, float ny, float nz);
typedef void (PXOpenGLAPI* PXOpenGLNormal3fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLNormal3iFunction)(GLint nx, GLint ny, GLint nz);
typedef void (PXOpenGLAPI* PXOpenGLNormal3ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLNormal3sFunction)(GLshort nx, GLshort ny, GLshort nz);
typedef void (PXOpenGLAPI* PXOpenGLNormal3svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLNormalPointerFunction)(GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void (PXOpenGLAPI* PXOpenGLOrthoFunction)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (PXOpenGLAPI* PXOpenGLPassThroughFunction)(float token);
typedef void (PXOpenGLAPI* PXOpenGLPixelMapfvFunction)(GLenum map, GLsizei mapsize, const float* values);
typedef void (PXOpenGLAPI* PXOpenGLPixelMapuivFunction)(GLenum map, GLsizei mapsize, const GLuint* values);
typedef void (PXOpenGLAPI* PXOpenGLPixelMapusvFunction)(GLenum map, GLsizei mapsize, const GLushort* values);
typedef void (PXOpenGLAPI* PXOpenGLPixelStorefFunction)(GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLPixelStoreiFunction)(GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLPixelTransferfFunction)(GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLPixelTransferiFunction)(GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLPixelZoomFunction)(float xfactor, float yfactor);
typedef void (PXOpenGLAPI* PXOpenGLPointSizeFunction)(float size);
typedef void (PXOpenGLAPI* PXOpenGLPolygonModeFunction)(GLenum face, GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLPolygonOffsetFunction)(float factor, float units);
typedef void (PXOpenGLAPI* PXOpenGLPolygonStippleFunction)(const GLubyte* mask);
typedef void (PXOpenGLAPI* PXOpenGLPopAttribFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLPopClientAttribFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLPopMatrixFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLPopNameFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLPrioritizeTexturesFunction)(GLsizei n, const GLuint* textures, const GLclampf* priorities);
typedef void (PXOpenGLAPI* PXOpenGLPushAttribFunction)(GLbitfield mask);
typedef void (PXOpenGLAPI* PXOpenGLPushClientAttribFunction)(GLbitfield mask);
typedef void (PXOpenGLAPI* PXOpenGLPushMatrixFunction)(void);
typedef void (PXOpenGLAPI* PXOpenGLPushNameFunction)(GLuint name);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2dFunction)(GLdouble x, GLdouble y);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2fFunction)(float x, float y);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2iFunction)(GLint x, GLint y);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2sFunction)(GLshort x, GLshort y);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos2svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3dFunction)(GLdouble x, GLdouble y, GLdouble z);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3fFunction)(float x, float y, float z);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3iFunction)(GLint x, GLint y, GLint z);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3sFunction)(GLshort x, GLshort y, GLshort z);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos3svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4dFunction)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4fFunction)(float x, float y, float z, float w);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4iFunction)(GLint x, GLint y, GLint z, GLint w);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4sFunction)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (PXOpenGLAPI* PXOpenGLRasterPos4svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLReadBufferFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLReadPixelsFunction)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);
typedef void (PXOpenGLAPI* PXOpenGLRectdFunction)(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
typedef void (PXOpenGLAPI* PXOpenGLRectdvFunction)(const GLdouble* v1, const GLdouble* v2);
typedef void (PXOpenGLAPI* PXOpenGLRectfFunction)(float x1, float y1, float x2, float y2);
typedef void (PXOpenGLAPI* PXOpenGLRectfvFunction)(const float* v1, const float* v2);
typedef void (PXOpenGLAPI* PXOpenGLRectiFunction)(GLint x1, GLint y1, GLint x2, GLint y2);
typedef void (PXOpenGLAPI* PXOpenGLRectivFunction)(const GLint* v1, const GLint* v2);
typedef void (PXOpenGLAPI* PXOpenGLRectsFunction)(GLshort x1, GLshort y1, GLshort x2, GLshort y2);
typedef void (PXOpenGLAPI* PXOpenGLRectsvFunction)(const GLshort* v1, const GLshort* v2);
typedef GLint(PXOpenGLAPI* PXOpenGLRenderModeFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLRotatedFunction)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (PXOpenGLAPI* PXOpenGLRotatefFunction)(float angle, float x, float y, float z);
typedef void (PXOpenGLAPI* PXOpenGLScaledFunction)(GLdouble x, GLdouble y, GLdouble z);
typedef void (PXOpenGLAPI* PXOpenGLScalefFunction)(float x, float y, float z);
typedef void (PXOpenGLAPI* PXOpenGLScissorFunction)(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (PXOpenGLAPI* PXOpenGLSelectBufferFunction)(GLsizei size, GLuint* buffer);
typedef void (PXOpenGLAPI* PXOpenGLShadeModelFunction)(GLenum mode);
typedef void (PXOpenGLAPI* PXOpenGLStencilFuncFunction)(GLenum func, GLint ref, GLuint mask);
typedef void (PXOpenGLAPI* PXOpenGLStencilMaskFunction)(GLuint mask);
typedef void (PXOpenGLAPI* PXOpenGLStencilOpFunction)(GLenum fail, GLenum zfail, GLenum zpass);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1dFunction)(GLdouble s);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1fFunction)(float s);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1iFunction)(GLint s);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1sFunction)(GLshort s);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord1svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2dFunction)(GLdouble s, GLdouble t);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2fFunction)(float s, float t);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2iFunction)(GLint s, GLint t);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2sFunction)(GLshort s, GLshort t);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord2svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3dFunction)(GLdouble s, GLdouble t, GLdouble r);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3fFunction)(float s, float t, float r);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3iFunction)(GLint s, GLint t, GLint r);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3sFunction)(GLshort s, GLshort t, GLshort r);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord3svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4dFunction)(GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4fFunction)(float s, float t, float r, float q);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4iFunction)(GLint s, GLint t, GLint r, GLint q);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4sFunction)(GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (PXOpenGLAPI* PXOpenGLTexCoord4svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLTexCoordPointerFunction)(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void (PXOpenGLAPI* PXOpenGLTexEnvfFunction)(GLenum target, GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLTexEnvfvFunction)(GLenum target, GLenum pname, const float* params);
typedef void (PXOpenGLAPI* PXOpenGLTexEnviFunction)(GLenum target, GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLTexEnvivFunction)(GLenum target, GLenum pname, const GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLTexGendFunction)(GLenum coord, GLenum pname, GLdouble param);
typedef void (PXOpenGLAPI* PXOpenGLTexGendvFunction)(GLenum coord, GLenum pname, const GLdouble* params);
typedef void (PXOpenGLAPI* PXOpenGLTexGenfFunction)(GLenum coord, GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLTexGenfvFunction)(GLenum coord, GLenum pname, const float* params);
typedef void (PXOpenGLAPI* PXOpenGLTexGeniFunction)(GLenum coord, GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLTexGenivFunction)(GLenum coord, GLenum pname, const GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLTexImage1DFunction)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (PXOpenGLAPI* PXOpenGLTexImage2DFunction)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (PXOpenGLAPI* PXOpenGLTexParameterfFunction)(GLenum target, GLenum pname, float param);
typedef void (PXOpenGLAPI* PXOpenGLTexParameterfvFunction)(GLenum target, GLenum pname, const float* params);
typedef void (PXOpenGLAPI* PXOpenGLTexParameteriFunction)(GLenum target, GLenum pname, GLint param);
typedef void (PXOpenGLAPI* PXOpenGLTexParameterivFunction)(GLenum target, GLenum pname, const GLint* params);
typedef void (PXOpenGLAPI* PXOpenGLTexSubImage1DFunction)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (PXOpenGLAPI* PXOpenGLTexSubImage2DFunction)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid* pixels);
typedef void (PXOpenGLAPI* PXOpenGLTranslatedFunction)(GLdouble x, GLdouble y, GLdouble z);
typedef void (PXOpenGLAPI* PXOpenGLTranslatefFunction)(float x, float y, float z);
typedef void (PXOpenGLAPI* PXOpenGLVertex2dFunction)(GLdouble x, GLdouble y);
typedef void (PXOpenGLAPI* PXOpenGLVertex2dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex2fFunction)(float x, float y);
typedef void (PXOpenGLAPI* PXOpenGLVertex2fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex2iFunction)(GLint x, GLint y);
typedef void (PXOpenGLAPI* PXOpenGLVertex2ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex2sFunction)(GLshort x, GLshort y);
typedef void (PXOpenGLAPI* PXOpenGLVertex2svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex3dFunction)(GLdouble x, GLdouble y, GLdouble z);
typedef void (PXOpenGLAPI* PXOpenGLVertex3dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex3fFunction)(float x, float y, float z);
typedef void (PXOpenGLAPI* PXOpenGLVertex3fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex3iFunction)(GLint x, GLint y, GLint z);
typedef void (PXOpenGLAPI* PXOpenGLVertex3ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex3sFunction)(GLshort x, GLshort y, GLshort z);
typedef void (PXOpenGLAPI* PXOpenGLVertex3svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex4dFunction)(GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (PXOpenGLAPI* PXOpenGLVertex4dvFunction)(const GLdouble* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex4fFunction)(float x, float y, float z, float w);
typedef void (PXOpenGLAPI* PXOpenGLVertex4fvFunction)(const float* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex4iFunction)(GLint x, GLint y, GLint z, GLint w);
typedef void (PXOpenGLAPI* PXOpenGLVertex4ivFunction)(const GLint* v);
typedef void (PXOpenGLAPI* PXOpenGLVertex4sFunction)(GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (PXOpenGLAPI* PXOpenGLVertex4svFunction)(const GLshort* v);
typedef void (PXOpenGLAPI* PXOpenGLVertexPointerFunction)(GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
typedef void (PXOpenGLAPI* PXOpenGLViewportFunction)(GLint x, GLint y, GLsizei width, GLsizei height);


typedef void (PXOpenGLAPI* glGetBooleanvFunction)(GLenum pname, GLboolean* data);
typedef void (PXOpenGLAPI* glGetDoublevFunction)(GLenum pname, GLdouble* data);
typedef void (PXOpenGLAPI* glGetFloatvFunction)(GLenum pname, float* data);
typedef void (PXOpenGLAPI* glGetIntegervFunction)(GLenum pname, GLint* data);
typedef void (PXOpenGLAPI* glGetInteger64vFunction)(GLenum pname, PXInt64S* data);
typedef void (PXOpenGLAPI* glGetBooleani_vFunction)(GLenum target, GLuint index, GLboolean* data);
typedef void (PXOpenGLAPI* glGetIntegeri_vFunction)(GLenum target, GLuint index, GLint* data);
typedef void (PXOpenGLAPI* glGetFloati_vFunction)(GLenum target, GLuint index, float* data);
typedef void (PXOpenGLAPI* glGetDoublei_vFunction)(GLenum target, GLuint index, GLdouble* data);
typedef void (PXOpenGLAPI* glGetInteger64i_vFunction)(GLenum target, GLuint index, PXInt64U* data);


typedef void* (PXOpenGLAPI* PXOpenGLFunctionPointerGet)(char* functionName); // wglGetProcAddress, glXGetProcAddress 

typedef struct PXGraphicDevicePhysical_	PXGraphicDevicePhysical;



#if OSWindows
//----------------------------------------------
// WGL_ARB_create_context
//----------------------------------------------
typedef HGLRC(PXOpenGLAPI* PXOpenGLContextCreateAttributes)(HDC hDC, HGLRC hshareContext, const int* attribList); // wglCreateContextAttribsARB

//----------------------------------------------
// NV_multigpu_context
//----------------------------------------------
#define WGL_CONTEXT_MULTIGPU_ATTRIB_NV                         0x20AA
#define WGL_CONTEXT_MULTIGPU_ATTRIB_SINGLE_NV                  0x20AB
#define WGL_CONTEXT_MULTIGPU_ATTRIB_AFR_NV                     0x20AC
#define WGL_CONTEXT_MULTIGPU_ATTRIB_MULTICAST_NV               0x20AD
#define WGL_CONTEXT_MULTIGPU_ATTRIB_MULTI_DISPLAY_MULTICAST_NV 0x20AE




	// OUTDATED ??

DECLARE_HANDLE(HGPUNV);

typedef struct _GPU_DEVICE
{
	DWORD  cb;
	CHAR   DeviceName[32];
	CHAR   DeviceString[128];
	DWORD  Flags;
	RECT   rcVirtualScreen;
} GPU_DEVICE, * PGPU_DEVICE;

typedef BOOL(PXOpenGLAPI* PXOpenGLDevicePhysicalList)(UINT iGpuIndex, HGPUNV* phGpu); // wglEnumGpusNV
typedef BOOL(PXOpenGLAPI* PXOpenGLDevicePhysicalListB)(HGPUNV hGpu, UINT iDeviceIndex, PGPU_DEVICE lpGpuDevice); // wglEnumGpuDevicesNV
typedef HDC(PXOpenGLAPI* PXOpenGLDeviceAffinityCreate)(const HGPUNV* phGpuList); // wglCreateAffinityDCNV
typedef BOOL(PXOpenGLAPI* PXOpenGLDeviceAffinityList)(HDC hAffinityDC, UINT iGpuIndex, HGPUNV* hGpu); // wglEnumGpusFromAffinityDCNV
typedef BOOL(PXOpenGLAPI* PXOpenGLDeviceAffinityDelete)(HDC hdc); // wglDeleteDCNV






#if OSWindows
typedef BOOL(WINAPI* PXOpenGLCopyContext)(HGLRC, HGLRC, UINT); // wglCopyContext
typedef HGLRC(WINAPI* PXOpenGLCreateContext)(HDC); // wglCreateContext
typedef HGLRC(WINAPI* PXOpenGLCreateLayerContext)(HDC, int); // wglCreateLayerContext
typedef BOOL(WINAPI* PXOpenGLDeleteContext)(HGLRC); // wglDeleteContext
typedef HGLRC(WINAPI* PXOpenGLCurrentContextGet)(VOID); // wglGetCurrentContext
typedef HDC(WINAPI* PXOpenGLCurrentDCGet)(VOID); // wglGetCurrentDC

typedef BOOL(WINAPI* PXOpenGLMakeCurrent)(HDC, HGLRC); // wglMakeCurrent
typedef BOOL(WINAPI* PXOpenGLShareLists)(HGLRC, HGLRC); // wglShareLists
typedef BOOL(WINAPI* PXOpenGLUseFontBitmapsA)(HDC, DWORD, DWORD, DWORD); // wglUseFontBitmapsA
typedef BOOL(WINAPI* PXOpenGLUseFontBitmapsW)(HDC, DWORD, DWORD, DWORD); // wglUseFontBitmapsW
#endif


#endif



typedef struct PXOpenGL_
{
	PXLibrary LibraryOpenGL;

	PXOpenGLConextID ContextHandle;

	PXOpenGLVersion Version;

#if OSUnix
	Display* DisplayHandle;
	GLXDrawable DrawableHandle;
#elif OSWindows
	HWND WindowHandle;
	HDC WindowDeviceContextHandle;
#endif

	PXBool IsTexture2DEnabled;

	PXOpenGLAccumFunction	Accum;
	PXOpenGLAlphaFuncFunction	AlphaFunc;
	PXOpenGLAreTexturesResidentFunction   	AreTexturesResident;
	PXOpenGLArrayElementFunction	ArrayElement;
	PXOpenGLBeginFunction   	Begin;
	PXOpenGLBindTextureFunction   	TextureBind;
	PXOpenGLBitmapFunction  	Bitmap;
	PXOpenGLBlendFuncFunction  	BlendFunc;
	PXOpenGLCallListFunction	CallList;
	PXOpenGLCallListsFunction	CallLists;
	PXOpenGLClearFunction	Clear;
	PXOpenGLClearAccumFunction	ClearAccum;
	PXOpenGLClearColorFunction	ClearColor;
	PXOpenGLClearDepthFunction	ClearDepth;
	PXOpenGLClearIndexFunction	ClearIndex;
	PXOpenGLClearStencilFunction	ClearStencil;
	PXOpenGLClipPlaneFunction	ClipPlane;
	PXOpenGLColor3bFunction	Color3b;
	PXOpenGLColor3bvFunction	Color3bv;
	PXOpenGLColor3dFunction	Color3d;
	PXOpenGLColor3dvFunction	Color3dv;
	PXOpenGLColor3fFunction	Color3f;
	PXOpenGLColor3fvFunction	Color3fv;
	PXOpenGLColor3iFunction	Color3i;
	PXOpenGLColor3ivFunction	Color3iv;
	PXOpenGLColor3sFunction	Color3s;
	PXOpenGLColor3svFunction	Color3sv;
	PXOpenGLColor3ubFunction	Color3ub;
	PXOpenGLColor3ubvFunction	Color3ubv;
	PXOpenGLColor3uiFunction	Color3ui;
	PXOpenGLColor3uivFunction	Color3uiv;
	PXOpenGLColor3usFunction	Color3us;
	PXOpenGLColor3usvFunction	Color3usv;
	PXOpenGLColor4bFunction	Color4b;
	PXOpenGLColor4bvFunction	Color4bv;
	PXOpenGLColor4dFunction	Color4d;
	PXOpenGLColor4dvFunction	Color4dv;
	PXOpenGLColor4fFunction	Color4f;
	PXOpenGLColor4fvFunction	Color4fv;
	PXOpenGLColor4iFunction	Color4i;
	PXOpenGLColor4ivFunction	Color4iv;
	PXOpenGLColor4sFunction	Color4s;
	PXOpenGLColor4svFunction	Color4sv;
	PXOpenGLColor4ubFunction	Color4ub;
	PXOpenGLColor4ubvFunction	Color4ubv;
	PXOpenGLColor4uiFunction	Color4ui;
	PXOpenGLColor4uivFunction	Color4uiv;
	PXOpenGLColor4usFunction	Color4us;
	PXOpenGLColor4usvFunction	Color4usv;
	PXOpenGLColorMaskFunction	ColorMask;
	PXOpenGLColorMaterialFunction	ColorMaterial;
	PXOpenGLColorPointerFunction	ColorPointer;
	PXOpenGLCopyPixelsFunction	CopyPixels;
	PXOpenGLCopyTexImage1DFunction	CopyTexImage1D;
	PXOpenGLCopyTexImage2DFunction	CopyTexImage2D;
	PXOpenGLCopyTexSubImage1DFunction	CopyTexSubImage1D;
	PXOpenGLCopyTexSubImage2DFunction	CopyTexSubImage2D;
	PXOpenGLCullFaceFunction	CullFace;
	PXOpenGLDeleteListsFunction	DeleteLists;
	PXOpenGLDeleteTexturesFunction	TextureDelete;
	PXOpenGLDepthFuncFunction	DepthFunc;
	PXOpenGLDepthMaskFunction	DepthMask;
	PXOpenGLDepthRangeFunction	DepthRange;
	PXOpenGLDisableFunction	Disable;
	PXOpenGLDisableClientStateFunction	DisableClientState;
	PXOpenGLDrawArraysFunction	DrawArrays;
	PXOpenGLDrawBufferFunction	DrawBuffer;
	PXOpenGLDrawElementsFunction	DrawElements;
	PXOpenGLDrawPixelsFunction	DrawPixels;
	PXOpenGLEdgeFlagFunction	EdgeFlag;
	PXOpenGLEdgeFlagPointerFunction	EdgeFlagPointer;
	PXOpenGLEdgeFlagvFunction	EdgeFlagv;
	PXOpenGLEnableFunction	Enable;
	PXOpenGLEnableClientStateFunction	EnableClientState;
	PXOpenGLEndFunction	End;
	PXOpenGLEndListFunction	EndList;
	PXOpenGLEvalCoord1dFunction	EvalCoord1d;
	PXOpenGLEvalCoord1dvFunction	EvalCoord1dv;
	PXOpenGLEvalCoord1fFunction	EvalCoord1f;
	PXOpenGLEvalCoord1fvFunction	EvalCoord1fv;
	PXOpenGLEvalCoord2dFunction	EvalCoord2d;
	PXOpenGLEvalCoord2dvFunction	EvalCoord2dv;
	PXOpenGLEvalCoord2fFunction	EvalCoord2f;
	PXOpenGLEvalCoord2fvFunction	EvalCoord2fv;
	PXOpenGLEvalMesh1Function	EvalMesh1;
	PXOpenGLEvalMesh2Function	EvalMesh2;
	PXOpenGLEvalPoint1Function	EvalPoint1;
	PXOpenGLEvalPoint2Function	EvalPoint2;
	PXOpenGLFeedbackBufferFunction	FeedbackBuffer;
	PXOpenGLFinishFunction	Finish;
	PXOpenGLFlushFunction	Flush;
	PXOpenGLFogfFunction	Fogf;
	PXOpenGLFogfvFunction	Fogfv;
	PXOpenGLFogiFunction	Fogi;
	PXOpenGLFogivFunction	Fogiv;
	PXOpenGLFrontFaceFunction	FrontFace;
	PXOpenGLFrustumFunction	Frustum;
	PXOpenGLGenListsFunction	 GenLists;
	PXOpenGLGenTexturesFunction	TextureCreate;
	PXOpenGLGetBooleanvFunction	GetBooleanv;
	PXOpenGLGetClipPlaneFunction	GetClipPlane;
	PXOpenGLGetDoublevFunction	GetDoublev;
	PXOpenGLGetErrorFunction	GetError;
	PXOpenGLGetFloatvFunction	GetFloatv;
	PXOpenGLGetIntegervFunction	GetIntegerv;
	PXOpenGLGetLightfvFunction	GetLightfv;
	PXOpenGLGetLightivFunction	GetLightiv;
	PXOpenGLGetMapdvFunction	GetMapdv;
	PXOpenGLGetMapfvFunction	GetMapfv;
	PXOpenGLGetMapivFunction	GetMapiv;
	PXOpenGLGetMaterialfvFunction	GetMaterialfv;
	PXOpenGLGetMaterialivFunction	GetMaterialiv;
	PXOpenGLGetPixelMapfvFunction	GetPixelMapfv;
	PXOpenGLGetPixelMapuivFunction	GetPixelMapuiv;
	PXOpenGLGetPixelMapusvFunction	GetPixelMapusv;
	PXOpenGLGetPointervFunction	GetPointerv;
	PXOpenGLGetPolygonStippleFunction	GetPolygonStipple;
	PXOpenGLGetStringFunction	GetString;
	PXOpenGLGetTexEnvfvFunction	GetTexEnvfv;
	PXOpenGLGetTexEnvivFunction	GetTexEnviv;
	PXOpenGLGetTexGendvFunction	GetTexGendv;
	PXOpenGLGetTexGenfvFunction	GetTexGenfv;
	PXOpenGLGetTexGenivFunction	GetTexGeniv;
	PXOpenGLGetTexImageFunction	GetTexImage;
	PXOpenGLGetTexLevelParameterfvFunction	GetTexLevelParameterfv;
	PXOpenGLGetTexLevelParameterivFunction	GetTexLevelParameteriv;
	PXOpenGLGetTexParameterfvFunction	GetTexParameterfv;
	PXOpenGLGetTexParameterivFunction	GetTexParameteriv;
	PXOpenGLHintFunction	Hint;
	PXOpenGLIndexMaskFunction	IndexMask;
	PXOpenGLIndexPointerFunction	IndexPointer;
	PXOpenGLIndexdFunction	Indexd;
	PXOpenGLIndexdvFunction	Indexdv;
	PXOpenGLIndexfFunction	Indexf;
	PXOpenGLIndexfvFunction	Indexfv;
	PXOpenGLIndexiFunction	Indexi;
	PXOpenGLIndexivFunction	Indexiv;
	PXOpenGLIndexsFunction	Indexs;
	PXOpenGLIndexsvFunction	Indexsv;
	PXOpenGLIndexubFunction	Indexub;
	PXOpenGLIndexubvFunction	Indexubv;
	PXOpenGLInitNamesFunction	InitNames;
	PXOpenGLInterleavedArraysFunction	InterleavedArrays;
	PXOpenGLIsEnabledFunction	 PXOpenGLIsEnabled;
	PXOpenGLIsListFunction	IsList;
	PXOpenGLIsTextureFunction	IsTexture;
	PXOpenGLLightModelfFunction	LightModelf;
	PXOpenGLLightModelfvFunction	LightModelfv;
	PXOpenGLLightModeliFunction	LightModeli;
	PXOpenGLLightModelivFunction	LightModeliv;
	PXOpenGLLightfFunction	Lightf;
	PXOpenGLLightfvFunction	Lightfv;
	PXOpenGLLightiFunction	Lighti;
	PXOpenGLLightivFunction	Lightiv;
	PXOpenGLLineStippleFunction	LineStipple;
	PXOpenGLLineWidthFunction	LineWidth;
	PXOpenGLListBaseFunction	ListBase;
	PXOpenGLLoadIdentityFunction	LoadIdentity;
	PXOpenGLLoadMatrixdFunction	LoadMatrixd;
	PXOpenGLLoadMatrixfFunction	LoadMatrixf;
	PXOpenGLLoadNameFunction	LoadName;
	PXOpenGLLogicOpFunction	LogicOp;
	PXOpenGLMap1dFunction	Map1d;
	PXOpenGLMap1fFunction	Map1f;
	PXOpenGLMap2dFunction	Map2d;
	PXOpenGLMap2fFunction	Map2f;
	PXOpenGLMapGrid1dFunction	MapGrid1d;
	PXOpenGLMapGrid1fFunction	MapGrid1f;
	PXOpenGLMapGrid2dFunction	MapGrid2d;
	PXOpenGLMapGrid2fFunction	MapGrid2f;
	PXOpenGLMaterialfFunction	Materialf;
	PXOpenGLMaterialfvFunction	Materialfv;
	PXOpenGLMaterialiFunction	Materiali;
	PXOpenGLMaterialivFunction	Materialiv;
	PXOpenGLMatrixModeFunction	MatrixMode;
	PXOpenGLMultMatrixdFunction	MultMatrixd;
	PXOpenGLMultMatrixfFunction	MultMatrixf;
	PXOpenGLNewListFunction	NewList;
	PXOpenGLNormal3bFunction	Normal3b;
	PXOpenGLNormal3bvFunction	Normal3bv;
	PXOpenGLNormal3dFunction	Normal3d;
	PXOpenGLNormal3dvFunction	Normal3dv;
	PXOpenGLNormal3fFunction	Normal3f;
	PXOpenGLNormal3fvFunction	Normal3fv;
	PXOpenGLNormal3iFunction	Normal3i;
	PXOpenGLNormal3ivFunction	Normal3iv;
	PXOpenGLNormal3sFunction	Normal3s;
	PXOpenGLNormal3svFunction	Normal3sv;
	PXOpenGLNormalPointerFunction	NormalPointer;
	PXOpenGLOrthoFunction	Ortho;
	PXOpenGLPassThroughFunction	PassThrough;
	PXOpenGLPixelMapfvFunction	PixelMapfv;
	PXOpenGLPixelMapuivFunction	PixelMapuiv;
	PXOpenGLPixelMapusvFunction	PixelMapusv;
	PXOpenGLPixelStorefFunction	PixelStoref;
	PXOpenGLPixelStoreiFunction	PixelStorei;
	PXOpenGLPixelTransferfFunction	PixelTransferf;
	PXOpenGLPixelTransferiFunction	PixelTransferi;
	PXOpenGLPixelZoomFunction	PixelZoom;
	PXOpenGLPointSizeFunction	PointSize;
	PXOpenGLPolygonModeFunction	PolygonMode;
	PXOpenGLPolygonOffsetFunction	PolygonOffset;
	PXOpenGLPolygonStippleFunction	PolygonStipple;
	PXOpenGLPopAttribFunction	PopAttrib;
	PXOpenGLPopClientAttribFunction	PopClientAttrib;
	PXOpenGLPopMatrixFunction	PopMatrix;
	PXOpenGLPopNameFunction	PopName;
	PXOpenGLPrioritizeTexturesFunction	PrioritizeTextures;
	PXOpenGLPushAttribFunction	PushAttrib;
	PXOpenGLPushClientAttribFunction	PushClientAttrib;
	PXOpenGLPushMatrixFunction	PushMatrix;
	PXOpenGLPushNameFunction	PushName;
	PXOpenGLRasterPos2dFunction	RasterPos2d;
	PXOpenGLRasterPos2dvFunction	RasterPos2dv;
	PXOpenGLRasterPos2fFunction	RasterPos2f;
	PXOpenGLRasterPos2fvFunction	RasterPos2fv;
	PXOpenGLRasterPos2iFunction	RasterPos2i;
	PXOpenGLRasterPos2ivFunction	RasterPos2iv;
	PXOpenGLRasterPos2sFunction	RasterPos2s;
	PXOpenGLRasterPos2svFunction	RasterPos2sv;
	PXOpenGLRasterPos3dFunction	RasterPos3d;
	PXOpenGLRasterPos3dvFunction	RasterPos3dv;
	PXOpenGLRasterPos3fFunction	RasterPos3f;
	PXOpenGLRasterPos3fvFunction	RasterPos3fv;
	PXOpenGLRasterPos3iFunction	RasterPos3i;
	PXOpenGLRasterPos3ivFunction	RasterPos3iv;
	PXOpenGLRasterPos3sFunction	RasterPos3s;
	PXOpenGLRasterPos3svFunction	RasterPos3sv;
	PXOpenGLRasterPos4dFunction	RasterPos4d;
	PXOpenGLRasterPos4dvFunction	RasterPos4dv;
	PXOpenGLRasterPos4fFunction	RasterPos4f;
	PXOpenGLRasterPos4fvFunction	RasterPos4fv;
	PXOpenGLRasterPos4iFunction	RasterPos4i;
	PXOpenGLRasterPos4ivFunction	RasterPos4iv;
	PXOpenGLRasterPos4sFunction	RasterPos4s;
	PXOpenGLRasterPos4svFunction	RasterPos4sv;
	PXOpenGLReadBufferFunction	ReadBuffer;
	PXOpenGLReadPixelsFunction	ReadPixels;
	PXOpenGLRectdFunction	Rectd;
	PXOpenGLRectdvFunction	Rectdv;
	PXOpenGLRectfFunction	Rectf;
	PXOpenGLRectfvFunction	Rectfv;
	PXOpenGLRectiFunction	Recti;
	PXOpenGLRectivFunction	Rectiv;
	PXOpenGLRectsFunction	Rects;
	PXOpenGLRectsvFunction	Rectsv;
	PXOpenGLRenderModeFunction RenderMode;
	PXOpenGLRotatedFunction	Rotated;
	PXOpenGLRotatefFunction	Rotatef;
	PXOpenGLScaledFunction	Scaled;
	PXOpenGLScalefFunction	Scalef;
	PXOpenGLScissorFunction	Scissor;
	PXOpenGLSelectBufferFunction	SelectBuffer;
	PXOpenGLShadeModelFunction	ShadeModel;
	PXOpenGLStencilFuncFunction	StencilFunc;
	PXOpenGLStencilMaskFunction	StencilMask;
	PXOpenGLStencilOpFunction	StencilOp;
	PXOpenGLTexCoord1dFunction	TexCoord1d;
	PXOpenGLTexCoord1dvFunction	TexCoord1dv;
	PXOpenGLTexCoord1fFunction	TexCoord1f;
	PXOpenGLTexCoord1fvFunction	TexCoord1fv;
	PXOpenGLTexCoord1iFunction	TexCoord1i;
	PXOpenGLTexCoord1ivFunction	TexCoord1iv;
	PXOpenGLTexCoord1sFunction	TexCoord1s;
	PXOpenGLTexCoord1svFunction	TexCoord1sv;
	PXOpenGLTexCoord2dFunction	TexCoord2d;
	PXOpenGLTexCoord2dvFunction	TexCoord2dv;
	PXOpenGLTexCoord2fFunction	TexCoord2f;
	PXOpenGLTexCoord2fvFunction	TexCoord2fv;
	PXOpenGLTexCoord2iFunction	TexCoord2i;
	PXOpenGLTexCoord2ivFunction	TexCoord2iv;
	PXOpenGLTexCoord2sFunction	TexCoord2s;
	PXOpenGLTexCoord2svFunction	TexCoord2sv;
	PXOpenGLTexCoord3dFunction	TexCoord3d;
	PXOpenGLTexCoord3dvFunction	TexCoord3dv;
	PXOpenGLTexCoord3fFunction	TexCoord3f;
	PXOpenGLTexCoord3fvFunction	TexCoord3fv;
	PXOpenGLTexCoord3iFunction	TexCoord3i;
	PXOpenGLTexCoord3ivFunction	TexCoord3iv;
	PXOpenGLTexCoord3sFunction	TexCoord3s;
	PXOpenGLTexCoord3svFunction	TexCoord3sv;
	PXOpenGLTexCoord4dFunction	TexCoord4d;
	PXOpenGLTexCoord4dvFunction	TexCoord4dv;
	PXOpenGLTexCoord4fFunction	TexCoord4f;
	PXOpenGLTexCoord4fvFunction	TexCoord4fv;
	PXOpenGLTexCoord4iFunction	TexCoord4i;
	PXOpenGLTexCoord4ivFunction	TexCoord4iv;
	PXOpenGLTexCoord4sFunction	TexCoord4s;
	PXOpenGLTexCoord4svFunction	TexCoord4sv;
	PXOpenGLTexCoordPointerFunction	TexCoordPointer;
	PXOpenGLTexEnvfFunction	TexEnvf;
	PXOpenGLTexEnvfvFunction	TexEnvfv;
	PXOpenGLTexEnviFunction	TexEnvi;
	PXOpenGLTexEnvivFunction	TexEnviv;
	PXOpenGLTexGendFunction	TexGend;
	PXOpenGLTexGendvFunction	TexGendv;
	PXOpenGLTexGenfFunction	TexGenf;
	PXOpenGLTexGenfvFunction	TexGenfv;
	PXOpenGLTexGeniFunction	TexGeni;
	PXOpenGLTexGenivFunction	TexGeniv;
	PXOpenGLTexImage1DFunction	TextureData1D;
	PXOpenGLTexImage2DFunction	TextureData2D;
	PXOpenGLTexParameterfFunction	TextureParameterF;
	PXOpenGLTexParameterfvFunction	TextureParameterListF;
	PXOpenGLTexParameteriFunction	TextureParameterI;
	PXOpenGLTexParameterivFunction	TextureParameterListI;
	PXOpenGLTexSubImage1DFunction	TexSubImage1D;
	PXOpenGLTexSubImage2DFunction	TexSubImage2D;
	PXOpenGLTranslatedFunction	Translated;
	PXOpenGLTranslatefFunction	Translatef;
	PXOpenGLVertex2dFunction	Vertex2d;
	PXOpenGLVertex2dvFunction	Vertex2dv;
	PXOpenGLVertex2fFunction	Vertex2f;
	PXOpenGLVertex2fvFunction	Vertex2fv;
	PXOpenGLVertex2iFunction	Vertex2i;
	PXOpenGLVertex2ivFunction	Vertex2iv;
	PXOpenGLVertex2sFunction	Vertex2s;
	PXOpenGLVertex2svFunction	Vertex2sv;
	PXOpenGLVertex3dFunction	Vertex3d;
	PXOpenGLVertex3dvFunction	Vertex3dv;
	PXOpenGLVertex3fFunction	Vertex3f;
	PXOpenGLVertex3fvFunction	Vertex3fv;
	PXOpenGLVertex3iFunction	Vertex3i;
	PXOpenGLVertex3ivFunction	Vertex3iv;
	PXOpenGLVertex3sFunction	Vertex3s;
	PXOpenGLVertex3svFunction	Vertex3sv;
	PXOpenGLVertex4dFunction	Vertex4d;
	PXOpenGLVertex4dvFunction	Vertex4dv;
	PXOpenGLVertex4fFunction	Vertex4f;
	PXOpenGLVertex4fvFunction	Vertex4fv;
	PXOpenGLVertex4iFunction   	Vertex4i;
	PXOpenGLVertex4ivFunction 	Vertex4iv;
	PXOpenGLVertex4sFunction    	Vertex4s;
	PXOpenGLVertex4svFunction 	Vertex4sv;
	PXOpenGLVertexPointerFunction   	VertexPointer;
	PXOpenGLViewportFunction   	Viewport;

#if !PXOpenGLForceLegacy

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
	PXOpenGLShaderPXProgramCreateFunction	ShaderProgramCreate;
	PXOpenGLShaderPXProgramUseFunction	ShaderProgramUse;
	PXOpenGLShaderPXProgramDeleteFunction ShaderProgramDelete;
	PXOpenGLShaderCreateFunction ShaderCreate;
	PXOpenGLShaderSourceFunction	ShaderSource;
	PXOpenGLShaderCompileFunction	ShaderCompile;
	PXOpenGLGetProgramiv ShaderProgramGetiv;
	PXOpenGLActiveAttributeGet ActiveAttributeGet;
	PXOpenGLGetActiveUniform ActiveUniformGet;
	PXOpenGLShaderGetivFunction	ShaderGetiv;
	PXOpenGLShaderLogInfoGetFunction	ShaderLogInfoGet;
	PXOpenGLProgramInfoLogGet ProgramInfoLogGet;
	PXOpenGLShaderDeleteFunction	ShaderDelete;
	PXOpenGLAttachShaderFunction	ShaderAttach;
	PXOpenGLLinkPXProgramFunction	ShaderProgramLink;
	PXOpenGLValidatePXProgramFunction	ShaderProgramValidate;
	PXOpenGLActiveTextureFunction	TextureSlotActive;
	PXOpenGLGenBuffersFunction	BufferGenerate;
	PXOpenGLBindBufferFunction	BufferBind;
	PXOpenGLBufferDataFunction	BufferData;
	PXOpenGLVertexAttribPointerFunction VertexAttribPointer;
	PXOpenGLVertexAttribArrayEnableFunction VertexAttribArrayEnable;
	PXOpenGLVertexAttribArrayDisableFunction VertexAttribArrayDisable;
	PXOpenGLDisableVertexArrayAttribFunction	DisableVertexArrayAttrib;
	PXOpenGLGetUniformLocation	GetUniformLocation;
	PXOpenGLUniform1fFunction	Uniform1f;
	PXOpenGLUniform1fvFunction	Uniform1fv;
	PXOpenGLUniform1iFunction	Uniform1i;
	PXOpenGLUniform1ivFunction	Uniform1iv;
	PXOpenGLUniform2fFunction	Uniform2f;
	PXOpenGLUniform2fvFunction	Uniform2fv;
	PXOpenGLUniform2iFunction	Uniform2i;
	PXOpenGLUniform2ivFunction	Uniform2iv;
	PXOpenGLUniform3fFunction	Uniform3f;
	PXOpenGLUniform3fvFunction	Uniform3fv;
	PXOpenGLUniform3iFunction	Uniform3i;
	PXOpenGLUniform3ivFunction	Uniform3iv;
	PXOpenGLUniform4fFunction	Uniform4f;
	PXOpenGLUniform4fvFunction	Uniform4fv;
	PXOpenGLUniform4iFunction	Uniform4i;
	PXOpenGLUniform4ivFunction	Uniform4iv;
	PXOpenGLUniformMatrix2fvFunction	UniformMatrix2fv;
	PXOpenGLUniformMatrix3fvFunction	UniformMatrix3fv;
	PXOpenGLUniformMatrix4fvFunction UniformMatrix4fv;
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.2.1.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.3.0.0>------------------------------------------------------
	PXOpenGLFrameBufferCreateFunction FrameBufferCreate;
	PXOpenGLFrameBufferDeleteFunction FrameBufferDelete;
	PXOpenGLFrameBufferBindFunction FrameBufferBind;

	PXOpenGLRenderBufferCreateFunction RenderBufferCreate;
	PXOpenGLRenderBufferBindFunction  RenderBufferBind;
	PXOpenGLRenderBufferDeleteFunction RenderBufferDelete;
	PXOpenGLRenderBufferStorageFunction RenderBufferStorage;
	PXOpenGLFrameBufferLinkTexture2DFunction FrameBufferLinkTexture2D;
	PXOpenGLFrameBufferLinkRenderBufferFunction FrameBufferLinkRenderBuffer;

	PXOpenGLGenVertexArraysFunction VertexArraysGenerate;
	PXOpenGLBindVertexArrayFunction VertexArrayBind;
	PXOpenGLVertexAttribIPointerFunction VertexAttribIPointer;

	PXOpenGLStringIFunction StringI;
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.3.1.0>------------------------------------------------------
	PXOpenGLDrawArraysInstancedFunction DrawArraysInstanced;
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.3.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.3.3.0>------------------------------------------------------
	PXOpenGLVertexAttribDivisorFunction VertexAttribDivisor;
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.0.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.1.0>------------------------------------------------------
	PXOpenGLVertexAttribLPointerFunction	VertexAttribLPointer;
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.2.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.3.0>------------------------------------------------------
	PXOpenGLDebugMessageFunction DebugMessage;
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.4.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.5.0>------------------------------------------------------

	//-------------------------------------------------------------------------

	//---<PXOpenGL v.4.6.0>------------------------------------------------------
	//-------------------------------------------------------------------------

	//---<Extensions>------------------------------------------------------------
	PXOpenGLSwapIntervalSetFunction SwapIntervalSet;
	PXOpenGLSwapIntervalGetFunction SwapIntervalGet;
	//---------------------------------------------------------------------------


	glGetBooleanvFunction glGetBooleanv;
	glGetDoublevFunction glGetDoublev;
	glGetFloatvFunction glGetFloatv;
	glGetIntegervFunction glGetIntegerv;
	glGetInteger64vFunction glGetInteger64v;
	glGetBooleani_vFunction glGetBooleani_v;
	glGetIntegeri_vFunction glGetIntegeri_v;
	glGetFloati_vFunction glGetFloati_v;
	glGetDoublei_vFunction glGetDoublei_v;
	glGetInteger64i_vFunction glGetInteger64i_v;

	PXOpenGLStringGetExtensionsARB StringGetExtensions;

	PXOpenGLFunctionPointerGet FunctionPointerGet;

#if OSWindows
	PXOpenGLContextCreateAttributes ContextCreateAttributes;

	PXOpenGLDevicePhysicalList DevicePhysicalList;
	PXOpenGLDevicePhysicalListB DevicePhysicalListB;
	PXOpenGLDeviceAffinityCreate DeviceAffinityCreate;
	PXOpenGLDeviceAffinityList DeviceAffinityList;
	PXOpenGLDeviceAffinityDelete DeviceAffinityDelete;

	PXOpenGLCopyContext CopyContext;
	PXOpenGLCreateContext CreateContext;
	PXOpenGLCreateLayerContext CreateLayerContext;
	PXOpenGLDeleteContext DeleteContext;
	PXOpenGLCurrentContextGet GetCurrentContext;
	PXOpenGLCurrentDCGet GetCurrentDC;

	PXOpenGLMakeCurrent MakeCurrent;
	PXOpenGLShareLists ShareLists;
	PXOpenGLUseFontBitmapsA UseFontBitmapsA;
	PXOpenGLUseFontBitmapsW UseFontBitmapsW;

#endif

#endif
}
PXOpenGL;


//--------------------------------------------------------
// Internal conversion
//--------------------------------------------------------
PXPrivate PXInt32U PXAPI PXOpenGLImageFormatToID(const PXColorFormat pxColorFormat, PXInt32U* const imageFormat, PXInt32U* const dataFormat);
PXPrivate PXInt32U PXAPI PXOpenGLRenderModeToID(const PXGraphicDrawMode pxGraphicDrawMode);
PXPrivate PXInt32U PXAPI PXOpenGLTextureTypeToID(const PXGraphicTextureType pxGraphicTextureType);
PXPrivate PXInt32U PXAPI PXOpenGLShaderTypeToID(const PXGraphicShaderType pxGraphicShaderType);
PXPrivate PXInt32U PXAPI PXOpenGLTypeToID(const PXInt32U pxDataType);
PXPrivate PXInt32U PXAPI PXOpenGLGraphicImageLayoutToID(const PXGraphicImageLayout pxGraphicImageLayout);
PXPrivate PXInt32U PXAPI PXOpenGLGraphicImageWrapToID(const PXGraphicImageWrap pxGraphicImageWrap);


PXPrivate PXShaderVariableType PXAPI PXOpenGLShaderVariableTypeFromID(const PXInt16U openGLShaderVariableTypeID);

PXPrivate PXActionResult PXAPI PXOpenGLErrorCurrent(PXOpenGL* const pxOpenGL);
PXPrivate PXOpenGLVersion PXAPI PXOpenGLVersionParse(const PXInt32U versionID);
//--------------------------------------------------------

//---<Utility>-------------------------------------------------------------
PXPublic void PXAPI PXOpenGLConstruct(PXOpenGL* const openGLContext);
PXPublic void PXAPI PXOpenGLDestruct(PXOpenGL* const openGLContext);

PXPublic void PXAPI PXOpenGLSet(PXOpenGL* const openGLContext, const PXOpenGL* const openGLContextSoure);
PXPublic void PXAPI PXOpenGLCopy(PXOpenGL* const openGLContext, const PXOpenGL* const openGLContextSoure);

PXPublic PXActionResult PXAPI PXOpenGLInitialize(PXOpenGL* const openGLContext, PXGraphicInitializeInfo* const pxGraphicInitializeInfo);
PXPublic PXActionResult PXAPI PXOpenGLRelease(PXOpenGL* const openGLContext);

PXPublic void PXAPI PXOpenGLSelect(PXOpenGL* const openGLContext);
PXPublic PXBool PXAPI PXOpenGLDeselect(PXOpenGL* const openGLContext);


PXPublic PXInt64S PXAPI PXOpenGLIntergetGet(PXOpenGL* const openGLContext, const GLenum enumID);

PXPublic PXActionResult PXAPI PXOpenGLDevicePhysicalListAmount(PXOpenGL* const pxOpenGL, PXInt32U* const amount);
PXPublic PXActionResult PXAPI PXOpenGLDevicePhysicalListFetch(PXOpenGL* const pxOpenGL, const PXInt32U amount, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList);

PXPublic PXActionResult PXAPI PXOpenGLScreenBufferRead(PXOpenGL* const pxOpenGL, PXImage* const pxImage);

PXPublic void PXAPI PXOpenGLFlush(PXOpenGL* const openGLContext);
PXPublic void PXAPI PXOpenGLViewPortSet(PXOpenGL* const openGLContext, const PXViewPort* const pxViewPort);
PXPublic void PXAPI PXOpenGLViewPortGet(PXOpenGL* const openGLContext, PXViewPort* const pxViewPort);

PXPublic void PXAPI PXOpenGLPolygonRenderOrder(const PXOpenGL* const openGLContext, const PXOpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode);

PXPublic void PXAPI PXOpenGLSettingChange(PXOpenGL* const openGLContext, const PXOpenGLToggle toggle, const PXBool state);

PXPublic void PXAPI PXOpenGLDrawOrder(PXOpenGL* const openGLContext, const PXOpenGLDrawOrderMode openGLDrawOrderMode);

PXPublic void PXAPI PXOpenGLDrawMode(PXOpenGL* const openGLContext, const PXGraphicDrawFillMode pxGraphicDrawFillMode, const int cullingMode);

PXPublic void PXAPI PXOpenGLClear(PXOpenGL* const openGLContext, const PXColorRGBAF* const pxColorRGBAF);

PXPublic PXBool PXAPI PXOpenGLSceneDeploy(PXOpenGL* const openGLContext);

PXPublic void PXAPI PXOpenGLDrawScaleF(PXOpenGL* const openGLContext, const float x, const float y, const float z);
PXPublic void PXAPI PXOpenGLDrawBegin(PXOpenGL* const openGLContext, const PXGraphicDrawMode openGLRenderMode);
PXPublic void PXAPI PXOpenGLDrawVertexXYZF(PXOpenGL* const openGLContext, const float x, const float y, const float z);
PXPublic void PXAPI PXOpenGLDrawColorRGBF(PXOpenGL* const openGLContext, const float red, const float green, const float blue);
PXPublic void PXAPI PXOpenGLDrawColorRGBAF(PXOpenGL* const openGLContext, const float red, const float green, const float blue, const float alpha);
PXPublic void PXAPI PXOpenGLDrawEnd(PXOpenGL* const openGLContext);




PXPublic void PXAPI PXOpenGLTextureParameter(PXOpenGL* const openGLContext, const PXGraphicTextureType textureType, const PXOpenGLTextureParameterMode pname, const PXOpenGLTextureParameterValue openGLTextureParameterValue);
PXPublic void PXAPI PXOpenGLTextureParameterI(PXOpenGL* const openGLContext, const PXGraphicTextureType textureType, const PXOpenGLTextureParameterMode pname, const int param);
PXPublic void PXAPI PXOpenGLTextureParameterF(PXOpenGL* const openGLContext, const PXGraphicTextureType textureType, const PXOpenGLTextureParameterMode pname, const float param);


PXPublic void PXAPI PXOpenGLBlendingMode(PXOpenGL* const pxOpenGL, const PXBlendingMode pxBlendingMode);

PXPublic void PXOpenGLAPI PXOpenGLErrorMessageCallback(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const char* const message, const void* const userParam);
//-------------------------------------------------------------------------

// render

PXPublic void PXAPI PXOpenGLDrawArrays(const PXOpenGL* const openGLContext, const PXGraphicDrawMode renderMode, const PXSize offset, const PXSize amount);
PXPublic void PXAPI PXOpenGLDrawElements(const PXOpenGL* const openGLContext, const PXGraphicDrawMode renderMode, const PXSize amount, const PXInt32U pxDataType, const void* const indexList);



PXPublic void PXAPI PXOpenGLDrawArraysInstanced(const PXOpenGL* const openGLContext, const PXGraphicDrawMode renderMode, const PXSize startOffset, const PXSize amount, const PXSize instanceAmount);

//-----------------------------------------------------
// OpenGL - Texture
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXOpenGLTextureAction(PXOpenGL* const pxOpenGL, PXGraphicTexturInfo* const pxGraphicTexturInfo);


PXPublic void PXAPI PXOpenGLTextureActivate(PXOpenGL* const openGLContext, const unsigned int index);
PXPublic void PXAPI PXOpenGLTexture2DDataWrite(PXOpenGL* const openGLContext, PXTexture2D* const pxTexture2D);
//-------------------------------------------------------------------------


PXPublic void PXAPI PXOpenGLSkyboxDraw(PXOpenGL* const pxOpenGL, const PXRenderEntity* const pxRenderEntity);


//-----------------------------------------------------
// OpenGL - Shader
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXOpenGLShaderProgramCreate(PXOpenGL* const pxOpenGL, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount);
PXPublic PXActionResult PXAPI PXOpenGLShaderProgramSelect(PXOpenGL* const pxOpenGL, PXShaderProgram* const pxShaderProgram);
PXPublic PXActionResult PXAPI PXOpenGLShaderProgramDelete(PXOpenGL* const pxOpenGL, PXShaderProgram* const pxShaderProgram);

PXPublic PXActionResult PXAPI PXOpenGLShaderVariableSet(PXOpenGL* const pxOpenGL, const PXShaderProgram* const pxShaderProgram, PXShaderVariable* const pxShaderVariableList, const PXSize amount);
//-------------------------------------------------------------------------

//---<Buffers>-------------------------------------------------------------
PXPublic void PXAPI PXOpenGLVertexArrayGenerate(PXOpenGL* const openGLContext, const unsigned int amount, unsigned int* const vaoList);
PXPublic void PXAPI PXOpenGLVertexArrayBind(PXOpenGL* const openGLContext, const unsigned int vaoID);
PXPublic void PXAPI PXOpenGLVertexArrayUnbind(PXOpenGL* const openGLContext);
PXPublic void PXAPI PXOpenGLVertexArrayAttributeDefine
(
	PXOpenGL* const openGLContext,
	const PXInt32U index,
	const PXInt32U size,
	const PXInt32U datatype,
	const PXBool normalized,
	const PXInt32U stride,
	const PXSize offset
);
PXPublic void PXAPI PXOpenGLVertexAttributeDivisor(PXOpenGL* const openGLContext, const PXSize index, const PXSize divisor);
PXPublic void PXAPI PXOpenGLVertexArrayEnable(PXOpenGL* const openGLContext, const unsigned int vertexArrayAtributeID);
PXPublic void PXAPI PXOpenGLVertexArrayDisable(PXOpenGL* const openGLContext, const unsigned int vertexArrayAtributeID);

PXPublic void PXAPI PXOpenGLBufferGenerate(PXOpenGL* const openGLContext, const unsigned int amount, unsigned int* const bufferIDList);
PXPublic void PXAPI PXOpenGLBufferBind(PXOpenGL* const openGLContext, const PXOpenGLBufferType bufferType, const unsigned int bufferID);
PXPublic void PXAPI PXOpenGLBufferData(PXOpenGL* const openGLContext, const PXOpenGLBufferType bufferType, const unsigned int size, const void* const data, const PXOpenGLStoreMode openGLStoreMode);
PXPublic void PXAPI PXOpenGLBufferUnbind(PXOpenGL* const openGLContext, const PXOpenGLBufferType bufferType);
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
PXPublic void PXAPI PXOpenGLFrameBufferCreate(PXOpenGL* const openGLContext, const unsigned int amount, unsigned int* const framebufferIDList);
PXPublic void PXAPI PXOpenGLRenderBufferStorage(PXOpenGL* const openGLContext, const PXOpenGLRenderBufferFormat internalformat, const int width, const int height);

// If we bind to 0, we select the "main window"
PXPublic void PXAPI PXOpenGLFrameBufferBind(PXOpenGL* const openGLContext, const PXOpenGLFrameBufferMode target, const unsigned int framebufferID);

PXPublic void PXAPI PXOpenGLFrameBufferDestroy(PXOpenGL* const openGLContext, const unsigned int amount, unsigned int* const framebufferIDList);

PXPublic void PXAPI PXOpenGLRenderBufferCreate(PXOpenGL* const openGLContext, GLsizei n, GLuint* renderbuffers);


PXPublic void PXAPI PXOpenGLRenderBufferBind(PXOpenGL* const openGLContext, const unsigned int renderbuffer);

PXPublic void PXAPI PXOpenGLRenderBufferDelete(PXOpenGL* const openGLContext, GLsizei n, GLuint* renderbuffers);


PXPublic void PXAPI PXOpenGLFrameBufferLinkTexture2D(PXOpenGL* const openGLContext, const PXOpenGLRenderBufferAttachmentPoint attachment, const PXGraphicTextureType textarget, const unsigned int textureID, const int level);
PXPublic GLuint PXAPI PXOpenGLFrameBufferLinkRenderBuffer(PXOpenGL* const openGLContext, const PXOpenGLRenderBufferAttachmentPoint attachment, const unsigned int renderbuffer);
//-------------------------------------------------------------------------


//---<PXOpenGL v.1.0.0>------------------------------------------------------
//-------------------------------------------------------------------------

//---<PXOpenGL v.1.1.0>------------------------------------------------------
PXPublic void PXAPI PXOpenGLTexture2DDataRead(PXOpenGL* const openGLContext, PXTexture2D* const pxTexture2D);
PXPublic void PXAPI PXOpenGLTexture2DDataReadFrom(PXOpenGL* const openGLContext, PXTexture2D* const pxTexture2D, const PXInt32U x, const PXInt32U y, const PXInt32U width, const PXInt32U height);
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


	//---<Extensions>----------------------------------------------------------
PXPublic PXActionResult PXAPI PXOpenGLSwapIntervalSet(PXOpenGL* const openGLContext, const PXInt32U interval);
PXPublic PXActionResult PXAPI PXOpenGLSwapIntervalGet(PXOpenGL* const openGLContext, PXInt32U* const interval);
//-------------------------------------------------------------------------



PXPublic PXActionResult PXAPI PXOpenGLSpriteRegister(PXOpenGL* const pxOpenGL, PXSprite* const pxSprite);

//-----------------------------------------------------
// Fixed Scripting
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXOpenGLDrawScriptCreate(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript);
PXPublic PXActionResult PXAPI PXOpenGLDrawScriptBegin(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript);
PXPublic PXActionResult PXAPI PXOpenGLDrawScriptEnd(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript);
PXPublic PXActionResult PXAPI PXOpenGLDrawScriptDelete(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript);
PXPublic PXActionResult PXAPI PXOpenGLDrawScriptExecute(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript);

//-----------------------------------------------------
// Light
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXOpenGLLightSet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index);
PXPublic PXActionResult PXAPI PXOpenGLLightGet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index);
PXPublic PXActionResult PXAPI PXOpenGLLightEnableSet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index, const PXBool enable);
PXPublic PXActionResult PXAPI PXOpenGLLightEnableGet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index, PXBool* const enable);



//-----------------------------------------------------
// PX-Extrended
//-----------------------------------------------------
PXPublic PXActionResult PXAPI PXOpenGLModelRegister(PXOpenGL* const pxOpenGL, PXModel* const pxModel);
PXPublic PXActionResult PXAPI PXOpenGLModelDraw(PXOpenGL* const pxOpenGL, const PXRenderEntity* const pxRenderEntity);
PXPublic PXActionResult PXAPI PXOpenGLModelDeregister(PXOpenGL* const pxOpenGL, PXModel* const pxModel);


PXPublic PXActionResult PXAPI PXOpenGLRectangleDraw(PXOpenGL* const pxOpenGL, const float xA, const float yA, const float xB, const float yB, const PXInt8U mode);
PXPublic PXActionResult PXAPI PXOpenGLRectangleDrawTx(PXOpenGL* const pxOpenGL, const float xA, const float yA, const float xB, const float yB, const float txA, const float tyA, const float txB, const float tyB, const PXInt8U mode);

#endif