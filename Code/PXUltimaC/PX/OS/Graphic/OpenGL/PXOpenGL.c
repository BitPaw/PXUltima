#include "PXOpenGL.h"

// wingdi.h
#include <stdio.h>
#include <assert.h>

#include <PX/Media/PXText.h>
#include <PX/OS/Error/PXActionResult.h>
#include <PX/OS/Memory/PXMemory.h>
#include <PX/OS/GUI/PXGUI.h>
#include <PX/OS/Async/PXAwait.h>
#include <PX/OS/PXOS.h>
#include <PX/OS/Console/PXConsole.h>

#if 0
#include <GLEW/glew.h>
#endif

#if OSUnix
#pragma comment(lib, "libGL.o") // ToDo: Is this valid?
#elif OSWindows
#pragma comment(lib, "opengl32.lib")
#endif


// glXGetProcAddress
   // glXChooseVisual
   // glXCreateContext

const char PXOpenglLibraryName[]
=
#if OSUnix
"libGL.so"
#elif OSWindows
"opengl32.dll"
#endif
;

const char PXOpenGLName[] = "OpenGL";
const char PXOpenGLModelName[] = "Model";

const PXInt16U PXOpenGLBindingSize = sizeof(PXOpenGLBinding) / sizeof(void*);
const char PXOpenGLBindingList[] =
{
"glAccum\0"
"glActiveTexture\0"
"glAlphaFunc\0"
"glAreTexturesResident\0"
"glArrayElement\0"
"glAttachShader\0"
"glBegin\0"
"glBindBuffer\0"
"glBindFramebuffer\0"
"glBindRenderbuffer\0"
"glBindTexture\0"
"glBindVertexArray\0"
"glBitmap\0"
"glBlendFunc\0"
"glBufferData\0"
"glCallList\0"
"glCallLists\0"
"glClear\0"
"glClearAccum\0"
"glClearColor\0"
"glClearDepth\0"
"glClearIndex\0"
"glClearStencil\0"
"glClipPlane\0"
"glColor3b\0"
"glColor3bv\0"
"glColor3d\0"
"glColor3dv\0"
"glColor3f\0"
"glColor3fv\0"
"glColor3i\0"
"glColor3iv\0"
"glColor3s\0"
"glColor3sv\0"
"glColor3ub\0"
"glColor3ubv\0"
"glColor3ui\0"
"glColor3uiv\0"
"glColor3us\0"
"glColor3usv\0"
"glColor4b\0"
"glColor4bv\0"
"glColor4d\0"
"glColor4dv\0"
"glColor4f\0"
"glColor4fv\0"
"glColor4i\0"
"glColor4iv\0"
"glColor4s\0"
"glColor4sv\0"
"glColor4ub\0"
"glColor4ubv\0"
"glColor4ui\0"
"glColor4uiv\0"
"glColor4us\0"
"glColor4usv\0"
"glColorMask\0"
"glColorMaterial\0"
"glColorPointer\0"
"glCompileShader\0"
"glCopyPixels\0"
"glCopyTexImage1D\0"
"glCopyTexImage2D\0"
"glCopyTexSubImage1D\0"
"glCopyTexSubImage2D\0"
"glCreateProgram\0"
"glCreateShader\0"
"glCullFace\0"
"glDebugMessageCallback\0"
"glDeleteFramebuffers\0"
"glDeleteLists\0"
"glDeleteProgram\0"
"glDeleteRenderbuffers\0"
"glDeleteShader\0"
"glDeleteTextures\0"
"glDepthFunc\0"
"glDepthMask\0"
"glDepthRange\0"
"glDisable\0"
"glDisableClientState\0"
"glDisableVertexArrayAttrib\0"
"glDisableVertexAttribArray\0"
"glDrawArrays\0"
"glDrawArraysInstanced\0"
"glDrawBuffer\0"
"glDrawElements\0"
"glDrawPixels\0"
"glEdgeFlag\0"
"glEdgeFlagPointer\0"
"glEdgeFlagv\0"
"glEnable\0"
"glEnableClientState\0"
"glEnableVertexAttribArray\0"
"glEnd\0"
"glEndList\0"
"glEvalCoord1d\0"
"glEvalCoord1dv\0"
"glEvalCoord1f\0"
"glEvalCoord1fv\0"
"glEvalCoord2d\0"
"glEvalCoord2dv\0"
"glEvalCoord2f\0"
"glEvalCoord2fv\0"
"glEvalMesh1\0"
"glEvalMesh2\0"
"glEvalPoint1\0"
"glEvalPoint2\0"
"glFeedbackBuffer\0"
"glFinish\0"
"glFlush\0"
"glFogf\0"
"glFogfv\0"
"glFogi\0"
"glFogiv\0"
"glFramebufferRenderbuffer\0"
"glFramebufferTexture2D\0"
"glFrontFace\0"
"glFrustum\0"
"glGenBuffers\0"
"glGenFramebuffers\0"
"glGenLists\0"
"glGenRenderbuffers\0"
"glGenTextures\0"
"glGenVertexArrays\0"
"glGetActiveAttrib\0"
"glGetActiveUniform\0"
"glGetBooleani_v\0"
"glGetBooleanv\0"
//"glGetBooleanv\0"
"glGetClipPlane\0"
"glGetDoublei_v\0"
"glGetDoublev\0"
//"glGetDoublev\0"
"glGetError\0"
"glGetPXF32i_v\0"
"glGetPXF32v\0"
//"glGetPXF32v\0"
"glGetInteger64i_v\0"
"glGetInteger64v\0"
"glGetIntegeri_v\0"
"glGetIntegerv\0"
//"glGetIntegerv\0"
"glGetLightfv\0"
"glGetLightiv\0"
"glGetMapdv\0"
"glGetMapfv\0"
"glGetMapiv\0"
"glGetMaterialfv\0"
"glGetMaterialiv\0"
"glGetPixelMapfv\0"
"glGetPixelMapuiv\0"
"glGetPixelMapusv\0"
"glGetPointerv\0"
"glGetPolygonStipple\0"
"glGetProgramInfoLog\0"
"glGetProgramiv\0"
"glGetShaderInfoLog\0"
"glGetShaderiv\0"
"glGetString\0"
"glGetStringi\0"
"glGetTexEnvfv\0"
"glGetTexEnviv\0"
"glGetTexGendv\0"
"glGetTexGenfv\0"
"glGetTexGeniv\0"
"glGetTexImage\0"
"glGetTexLevelParameterfv\0"
"glGetTexLevelParameteriv\0"
"glGetTexParameterfv\0"
"glGetTexParameteriv\0"
"glGetUniformLocation\0"
"glHint\0"
"glIndexd\0"
"glIndexdv\0"
"glIndexf\0"
"glIndexfv\0"
"glIndexi\0"
"glIndexiv\0"
"glIndexMask\0"
"glIndexPointer\0"
"glIndexs\0"
"glIndexsv\0"
"glIndexub\0"
"glIndexubv\0"
"glInitNames\0"
"glInterleavedArrays\0"
"glIsEnabled\0"
"glIsList\0"
"glIsShader\0"
"glIsTexture\0"
"glLightf\0"
"glLightfv\0"
"glLighti\0"
"glLightiv\0"
"glLightModelf\0"
"glLightModelfv\0"
"glLightModeli\0"
"glLightModeliv\0"
"glLineStipple\0"
"glLineWidth\0"
"glLinkProgram\0"
"glListBase\0"
"glLoadIdentity\0"
"glLoadMatrixd\0"
"glLoadMatrixf\0"
"glLoadName\0"
"glLogicOp\0"
"glMap1d\0"
"glMap1f\0"
"glMap2d\0"
"glMap2f\0"
"glMapGrid1d\0"
"glMapGrid1f\0"
"glMapGrid2d\0"
"glMapGrid2f\0"
"glMaterialf\0"
"glMaterialfv\0"
"glMateriali\0"
"glMaterialiv\0"
"glMatrixMode\0"
"glMultMatrixd\0"
"glMultMatrixf\0"
"glNewList\0"
"glNormal3b\0"
"glNormal3bv\0"
"glNormal3d\0"
"glNormal3dv\0"
"glNormal3f\0"
"glNormal3fv\0"
"glNormal3i\0"
"glNormal3iv\0"
"glNormal3s\0"
"glNormal3sv\0"
"glNormalPointer\0"
"glOrtho\0"
"glPassThrough\0"
"glPixelMapfv\0"
"glPixelMapuiv\0"
"glPixelMapusv\0"
"glPixelStoref\0"
"glPixelStorei\0"
"glPixelTransferf\0"
"glPixelTransferi\0"
"glPixelZoom\0"
"glPointSize\0"
"glPolygonMode\0"
"glPolygonOffset\0"
"glPolygonStipple\0"
"glPopAttrib\0"
"glPopClientAttrib\0"
"glPopMatrix\0"
"glPopName\0"
"glPrioritizeTextures\0"
"glPushAttrib\0"
"glPushClientAttrib\0"
"glPushMatrix\0"
"glPushName\0"
"glRasterPos2d\0"
"glRasterPos2dv\0"
"glRasterPos2f\0"
"glRasterPos2fv\0"
"glRasterPos2i\0"
"glRasterPos2iv\0"
"glRasterPos2s\0"
"glRasterPos2sv\0"
"glRasterPos3d\0"
"glRasterPos3dv\0"
"glRasterPos3f\0"
"glRasterPos3fv\0"
"glRasterPos3i\0"
"glRasterPos3iv\0"
"glRasterPos3s\0"
"glRasterPos3sv\0"
"glRasterPos4d\0"
"glRasterPos4dv\0"
"glRasterPos4f\0"
"glRasterPos4fv\0"
"glRasterPos4i\0"
"glRasterPos4iv\0"
"glRasterPos4s\0"
"glRasterPos4sv\0"
"glReadBuffer\0"
"glReadPixels\0"
"glRectd\0"
"glRectdv\0"
"glRectf\0"
"glRectfv\0"
"glRecti\0"
"glRectiv\0"
"glRects\0"
"glRectsv\0"
"glRenderbufferStorage\0"
"glRenderMode\0"
"glRotated\0"
"glRotatef\0"
"glScaled\0"
"glScalef\0"
"glScissor\0"
"glSelectBuffer\0"
"glShadeModel\0"
"glShaderSource\0"
"glStencilFunc\0"
"glStencilMask\0"
"glStencilOp\0"
"glTexCoord1d\0"
"glTexCoord1dv\0"
"glTexCoord1f\0"
"glTexCoord1fv\0"
"glTexCoord1i\0"
"glTexCoord1iv\0"
"glTexCoord1s\0"
"glTexCoord1sv\0"
"glTexCoord2d\0"
"glTexCoord2dv\0"
"glTexCoord2f\0"
"glTexCoord2fv\0"
"glTexCoord2i\0"
"glTexCoord2iv\0"
"glTexCoord2s\0"
"glTexCoord2sv\0"
"glTexCoord3d\0"
"glTexCoord3dv\0"
"glTexCoord3f\0"
"glTexCoord3fv\0"
"glTexCoord3i\0"
"glTexCoord3iv\0"
"glTexCoord3s\0"
"glTexCoord3sv\0"
"glTexCoord4d\0"
"glTexCoord4dv\0"
"glTexCoord4f\0"
"glTexCoord4fv\0"
"glTexCoord4i\0"
"glTexCoord4iv\0"
"glTexCoord4s\0"
"glTexCoord4sv\0"
"glTexCoordPointer\0"
"glTexEnvf\0"
"glTexEnvfv\0"
"glTexEnvi\0"
"glTexEnviv\0"
"glTexGend\0"
"glTexGendv\0"
"glTexGenf\0"
"glTexGenfv\0"
"glTexGeni\0"
"glTexGeniv\0"
"glTexImage1D\0"
"glTexImage2D\0"
"glTexParameterf\0"
"glTexParameterfv\0"
"glTexParameteri\0"
"glTexParameteriv\0"
"glTexSubImage1D\0"
"glTexSubImage2D\0"
"glTranslated\0"
"glTranslatef\0"
"glUniform1f\0"
"glUniform1fv\0"
"glUniform1i\0"
"glUniform1iv\0"
"glUniform2f\0"
"glUniform2fv\0"
"glUniform2i\0"
"glUniform2iv\0"
"glUniform3f\0"
"glUniform3fv\0"
"glUniform3i\0"
"glUniform3iv\0"
"glUniform4f\0"
"glUniform4fv\0"
"glUniform4i\0"
"glUniform4iv\0"
"glUniformMatrix2fv\0"
"glUniformMatrix3fv\0"
"glUniformMatrix4fv\0"
"glUseProgram\0"
"glValidateProgram\0"
"glVertex2d\0"
"glVertex2dv\0"
"glVertex2f\0"
"glVertex2fv\0"
"glVertex2i\0"
"glVertex2iv\0"
"glVertex2s\0"
"glVertex2sv\0"
"glVertex3d\0"
"glVertex3dv\0"
"glVertex3f\0"
"glVertex3fv\0"
"glVertex3i\0"
"glVertex3iv\0"
"glVertex3s\0"
"glVertex3sv\0"
"glVertex4d\0"
"glVertex4dv\0"
"glVertex4f\0"
"glVertex4fv\0"
"glVertex4i\0"
"glVertex4iv\0"
"glVertex4s\0"
"glVertex4sv\0"
"glVertexAttribDivisor\0"
"glVertexAttribIPointer\0"
"glVertexAttribLPointer\0"
"glVertexAttribPointer\0"
"glVertexPointer\0"
"glViewport\0"
"glShaderStorageBlockBinding\0"
"glBindBufferBase\0"
"glBufferSubData\0"

#if OSUnix // Linux - X-System
"glXChooseVisual\0"
"glXCopyContext\0"
"glXCreateContext\0"
"glXCreateGLXPixmap\0"
"glXDestroyContext\0"
"glXDestroyGLXPixmap\0"
"glXGetConfig\0"
"glXGetCurrentContext\0"
"glXGetCurrentDrawable\0"
"glXGetProcAddress\0"
"glXIsDirect\0"
"glXMakeCurrent\0"
"glXQueryExtension\0"
"glXQueryVersion\0"
"glXSwapBuffers\0"
"glXUseXFont\0"
"glXWaitGL\0"
"glXWaitX\0"
// Linux- Extensions
"glXSwapIntervalEXT\0"
"glxGetSwapIntervalEXT\0"
"glxGetExtensionsStringARB\0"


#elif OSWindows // Windows
"wglGetPixelFormatAttribivARB\0"
"wglGetPixelFormatAttribfvARB\0"
"wglChoosePixelFormatARB\0"
"wglChoosePixelFormat\0"
"wglCopyContext\0"
"wglCreateAffinityDCNV\0"
"wglCreateContext\0"
"wglCreateContextAttribsARB\0"
"wglCreateLayerContext\0"
"wglDeleteContext\0"
"wglDeleteDCNV\0"
"wglDescribeLayerPlane\0"
"wglDescribePixelFormat\0"
"wglEnumGpuDevicesNV\0"
"wglEnumGpusFromAffinityDCNV\0"
"wglEnumGpusNV\0"
"wglGetCurrentContext\0"
"wglGetCurrentDC\0"
"wglGetDefaultProcAddress\0"
"wglGetExtensionsStringARB\0"
"wglGetLayerPaletteEntries\0"
"wglGetPixelFormat\0"
"wglGetProcAddress\0"
"wglGetSwapIntervalEXT\0"
"wglMakeCurrent\0"
"wglRealizeLayerPalette\0"
"wglSetLayerPaletteEntries\0"
"wglSetPixelFormat\0"
"wglShareLists\0"
"wglSwapBuffers\0"
"wglSwapIntervalEXT\0"
"wglSwapLayerBuffers\0"
"wglSwapMultipleBuffers\0"
"wglUseFontBitmapsA\0"
"wglUseFontBitmapsW\0"
"wglUseFontOutlinesA\0"
"wglUseFontOutlinesW\0"
#endif

"\0"
};









#define GL_CW 0x0900
#define GL_CCW 0x0901

//---<Version 1.2>------------------
#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_RESCALE_NORMAL 0x803A
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SINGLE_COLOR 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E
//----------------------------------

//---<Version 1.3>------------------
#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_SUBTRACT 0x84E7
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_NORMAL_MAP 0x8511
#define GL_REFLECTION_MAP 0x8512
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE2_ALPHA 0x858A
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#define GL_MULTISAMPLE_BIT 0x20000000
//----------------------------------


//---<Version 1.4>------------------
#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_MIRRORED_REPEAT 0x8370
#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORDINATE 0x8451
#define GL_FRAGMENT_DEPTH 0x8452
#define GL_CURRENT_FOG_COORDINATE 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#define GL_FOG_COORDINATE_ARRAY 0x8457
#define GL_COLOR_SUM 0x8458
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_COMPARE_R_TO_TEXTURE 0x884E
//----------------------------------

//---------------------------------- 1.5

#define GL_CURRENT_FOG_COORD GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD GL_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY GL_FOG_COORDINATE_ARRAY
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_FOG_COORD_ARRAY_POINTER GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY_STRIDE GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_FOG_COORD_ARRAY_TYPE GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_FOG_COORD_SRC GL_FOG_COORDINATE_SOURCE
#define GL_SRC0_ALPHA GL_SOURCE0_ALPHA
#define GL_SRC0_RGB GL_SOURCE0_RGB
#define GL_SRC1_ALPHA GL_SOURCE1_ALPHA
#define GL_SRC1_RGB GL_SOURCE1_RGB
#define GL_SRC2_ALPHA GL_SOURCE2_ALPHA
#define GL_SRC2_RGB GL_SOURCE2_RGB
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_SAMPLES_PASSED 0x8914
//----------------------------------


//---------------------------------- Version 2
#define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_POINT_SPRITE 0x8861
#define GL_COORD_REPLACE 0x8862
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_COORDS 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VARYING_PXF32S 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE 0x8B4F
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5


// Extensions for 2.0

#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46


#define GL_DEPTH_STENCIL_TEXTURE_MODE 0x90EA

#define GL_MIRROR_CLAMP_TO_EDGE 0x8743
//----------------------------------



//---------------------------------- 2.1
#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB_ALPHA 0x8C42
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_SLUMINANCE_ALPHA 0x8C44
#define GL_SLUMINANCE8_ALPHA8 0x8C45
#define GL_SLUMINANCE 0x8C46
#define GL_SLUMINANCE8 0x8C47
#define GL_COMPRESSED_SRGB 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#define GL_COMPRESSED_SLUMINANCE 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA 0x8C4B


//----------------------------------



//---------- Version 3.0

#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
#define GL_CLIP_DISTANCE1 GL_CLIP_PLANE1
#define GL_CLIP_DISTANCE2 GL_CLIP_PLANE2
#define GL_CLIP_DISTANCE3 GL_CLIP_PLANE3
#define GL_CLIP_DISTANCE4 GL_CLIP_PLANE4
#define GL_CLIP_DISTANCE5 GL_CLIP_PLANE5
#define GL_COMPARE_REF_TO_TEXTURE GL_COMPARE_R_TO_TEXTURE_ARB
#define GL_MAX_CLIP_DISTANCES GL_MAX_CLIP_PLANES
#define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_PXF32S
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_DEPTH_BUFFER 0x8223
#define GL_STENCIL_BUFFER 0x8224
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_ALPHA_INTEGER 0x8D97
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16

//---------------------3.1
#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT 0x8C2E
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_RED_SNORM 0x8F90
#define GL_RG_SNORM 0x8F91
#define GL_RGB_SNORM 0x8F92
#define GL_RGBA_SNORM 0x8F93
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_PRIMITIVE_RESTART 0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121


//---------------------3.2
#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_CONTEXT_PROFILE_MASK 0x9126
//----------------------------

//---------------------------- 3.3

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_RGB10_A2UI 0x906F
//----------------------------



// 4.0
#define GL_SAMPLE_SHADING 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS 0x8F9F
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F
//-------------------------------------------------------------------------


//------------------------------------------------------------------------- 4.1

// Nothing

//-------------------------------------------------------------------------


//------------------------------------------------------------------------- 4.2

#define GL_TRANSFORM_FEEDBACK_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_ACTIVE 0x8E24
#define GL_COMPRESSED_RGBA_BPTC_UNORM 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_PXF32 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_PXF32 0x8E8F
#define GL_COPY_READ_BUFFER_BINDING 0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING 0x8F37
//-------------------------------------------------------------------------


//------------------------------------------------------------------------- 4.3
#define GL_NUM_SHADING_LANGUAGE_VERSIONS 0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG 0x874E

//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
// GL_ARB_shader_storage_buffer_object
//-------------------------------------------------------------------------
#define GL_SHADER_STORAGE_BARRIER_BIT 0x2000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF
//-------------------------------------------------------------------------


//------------------------------------------------------------------------- 4.3
#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LPXOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_DISPLAY_LIST 0x82E7
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LPXOGGED_MESSAGES 0x9144
#define GL_DEBUG_LPXOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_OUTPUT 0x92E0
//-------------------------------------------------------------------------



//------------------------------------------------------------------------- 4.5
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT 0x00000004
//-------------------------------------------------------------------------

//------------------------------------------------------------------------- 4.6
#define GL_CONTEXT_FLAG_NO_ERROR_BIT 0x00000008
#define GL_PARAMETER_BUFFER 0x80EE
#define GL_PARAMETER_BUFFER_BINDING 0x80EF
#define GL_TRANSFORM_FEEDBACK_OVERFLOW 0x82EC
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW 0x82ED
#define GL_VERTICES_SUBMITTED 0x82EE
#define GL_PRIMITIVES_SUBMITTED 0x82EF
#define GL_VERTEX_SHADER_INVOCATIONS 0x82F0
#define GL_TESS_CONTROL_SHADER_PATCHES 0x82F1
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS 0x82F2
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED 0x82F3
#define GL_FRAGMENT_SHADER_INVOCATIONS 0x82F4
#define GL_COMPUTE_SHADER_INVOCATIONS 0x82F5
#define GL_CLIPPING_INPUT_PRIMITIVES 0x82F6
#define GL_CLIPPING_OUTPUT_PRIMITIVES 0x82F7
#define GL_TEXTURE_MAX_ANISOTROPY 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF
#define GL_POLYGON_OFFSET_CLAMP 0x8E1B
#define GL_SHADER_BINARY_FORMAT_SPIR_V 0x9551
#define GL_SPIR_V_BINARY 0x9552
#define GL_SPIR_V_EXTENSIONS 0x9553
#define GL_NUM_SPIR_V_EXTENSIONS 0x9554
//-------------------------------------------------------------------------











// extension sparse

#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x806F
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_CUBE_MAP_ARRAY_OES 0x9009
#define GL_VIRTUAL_PAGE_SIZE_X_EXT 0x9195
#define GL_VIRTUAL_PAGE_SIZE_Y_EXT 0x9196
#define GL_VIRTUAL_PAGE_SIZE_Z_EXT 0x9197
#define GL_MAX_SPARSE_TEXTURE_SIZE_EXT 0x9198
#define GL_MAX_SPARSE_3D_TEXTURE_SIZE_EXT 0x9199
#define GL_MAX_SPARSE_ARRAY_TEXTURE_LAYERS_EXT 0x919A
#define GL_TEXTURE_SPARSE_EXT 0x91A6
#define GL_VIRTUAL_PAGE_SIZE_INDEX_EXT 0x91A7
#define GL_NUM_VIRTUAL_PAGE_SIZES_EXT 0x91A8
#define GL_SPARSE_TEXTURE_FULL_ARRAY_CUBE_MIPMAPS_EXT 0x91A9
#define GL_NUM_SPARSE_LEVELS_EXT 0x91AA


// extension framebuffer
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_INDEX 0x8222
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_SRGB 0x8C40
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57
//-------------------



// extension imaging
#define GL_CONSTANT_COLOR 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_BLEND_COLOR 0x8005
#define GL_FUNC_ADD 0x8006
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_BLEND_EQUATION 0x8009
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_CONVOLUTION_1D 0x8010
#define GL_CONVOLUTION_2D 0x8011
#define GL_SEPARABLE_2D 0x8012
#define GL_CONVOLUTION_BORDER_MODE 0x8013
#define GL_CONVOLUTION_FILTER_SCALE 0x8014
#define GL_CONVOLUTION_FILTER_BIAS 0x8015
#define GL_REDUCE 0x8016
#define GL_CONVOLUTION_FORMAT 0x8017
#define GL_CONVOLUTION_WIDTH 0x8018
#define GL_CONVOLUTION_HEIGHT 0x8019
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#define GL_POST_CONVOLUTION_RED_SCALE 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS 0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS 0x8023
#define GL_HISTOGRAM 0x8024
#define GL_PROXY_HISTOGRAM 0x8025
#define GL_HISTOGRAM_WIDTH 0x8026
#define GL_HISTOGRAM_FORMAT 0x8027
#define GL_HISTOGRAM_RED_SIZE 0x8028
#define GL_HISTOGRAM_GREEN_SIZE 0x8029
#define GL_HISTOGRAM_BLUE_SIZE 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE 0x802C
#define GL_HISTOGRAM_SINK 0x802D
#define GL_MINMAX 0x802E
#define GL_MINMAX_FORMAT 0x802F
#define GL_MINMAX_SINK 0x8030
#define GL_TABLE_TOO_LARGE 0x8031
#define GL_COLOR_MATRIX 0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH 0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH 0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE 0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE 0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE 0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE 0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS 0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS 0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS 0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS 0x80BB
#define GL_COLOR_TABLE 0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE 0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE 0x80D2
#define GL_PROXY_COLOR_TABLE 0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_COLOR_TABLE_SCALE 0x80D6
#define GL_COLOR_TABLE_BIAS 0x80D7
#define GL_COLOR_TABLE_FORMAT 0x80D8
#define GL_COLOR_TABLE_WIDTH 0x80D9
#define GL_COLOR_TABLE_RED_SIZE 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE 0x80DF
#define GL_IGNORE_BORDER 0x8150
#define GL_CONSTANT_BORDER 0x8151
#define GL_WRAP_BORDER 0x8152
#define GL_REPLICATE_BORDER 0x8153
#define GL_CONVOLUTION_BORDER_COLOR 0x8154




// Seemless texture
#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F



// extensions multisample
#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_INTEGER_SAMPLES 0x9110


// Atomic couinrer
#define GL_ATOMIC_COUNTER_BUFFER 0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING 0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START 0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE 0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS 0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER 0x92DB
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC



// Compute shader
#define GL_COMPUTE_SHADER_BIT 0x00000020
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_COMPUTE_WORK_GROUP_SIZE 0x8267
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER 0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF


// Transform Buffer
#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX 0xFFFFFFFFu


//---------------------------------------------------------
// GL_NVX_gpu_memory_info
//---------------------------------------------------------
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B
//---------------------------------------------------------





#define WGL_NUMBER_PIXEL_FORMATS_ARB            0x2000
#define WGL_DRAW_TO_WINDOW_ARB                  0x2001
#define WGL_DRAW_TO_BITMAP_ARB                  0x2002
#define WGL_ACCELERATION_ARB                    0x2003
#define WGL_NEED_PALETTE_ARB                    0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB             0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB              0x2006
#define WGL_SWAP_METHOD_ARB                     0x2007
#define WGL_NUMBER_OVERLAYS_ARB                 0x2008
#define WGL_NUMBER_UNDERLAYS_ARB                0x2009
#define WGL_TRANSPARENT_ARB                     0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB           0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB         0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB          0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB         0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB         0x203B
#define WGL_SHARE_DEPTH_ARB                     0x200C
#define WGL_SHARE_STENCIL_ARB                   0x200D
#define WGL_SHARE_ACCUM_ARB                     0x200E
#define WGL_SUPPORT_GDI_ARB                     0x200F
#define WGL_SUPPORT_OPENGL_ARB                  0x2010
#define WGL_DOUBLE_BUFFER_ARB                   0x2011
#define WGL_STEREO_ARB                          0x2012
#define WGL_PIXEL_TYPE_ARB                      0x2013
#define WGL_COLOR_BITS_ARB                      0x2014
#define WGL_RED_BITS_ARB                        0x2015
#define WGL_RED_SHIFT_ARB                       0x2016
#define WGL_GREEN_BITS_ARB                      0x2017
#define WGL_GREEN_SHIFT_ARB                     0x2018
#define WGL_BLUE_BITS_ARB                       0x2019
#define WGL_BLUE_SHIFT_ARB                      0x201A
#define WGL_ALPHA_BITS_ARB                      0x201B
#define WGL_ALPHA_SHIFT_ARB                     0x201C
#define WGL_ACCUM_BITS_ARB                      0x201D
#define WGL_ACCUM_RED_BITS_ARB                  0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                0x2021
#define WGL_DEPTH_BITS_ARB                      0x2022
#define WGL_STENCIL_BITS_ARB                    0x2023
#define WGL_AUX_BUFFERS_ARB                     0x2024

#define WGL_NO_ACCELERATION_ARB                 0x2025
#define WGL_GENERIC_ACCELERATION_ARB            0x2026
#define WGL_FULL_ACCELERATION_ARB               0x2027

#define WGL_SWAP_EXCHANGE_ARB                   0x2028
#define WGL_SWAP_COPY_ARB                       0x2029
#define WGL_SWAP_UNDEFINED_ARB                  0x202A

#define WGL_TYPE_RGBA_ARB                       0x202B
#define WGL_TYPE_COLORINDEX_ARB                 0x202C







unsigned int PXAPI PXOpenGLRenderBufferAttachmentPointToID(const PXOpenGLRenderBufferAttachmentPoint renderBufferAttachmentPoint)
{
    switch(renderBufferAttachmentPoint)
    {
        case PXOpenGLRenderBufferAttachmentPointColor:
            return GL_COLOR_ATTACHMENT0;
        case PXOpenGLRenderBufferAttachmentPointDepth:
            return GL_DEPTH_ATTACHMENT;
        case PXOpenGLRenderBufferAttachmentPointStencil:
            return GL_STENCIL_ATTACHMENT;

        case PXOpenGLRenderBufferAttachmentPointInvalid:
        default:
            return -1;
    }
}

GLenum PXAPI PXOpenGLPolygonRenderOrderModeToID(const PXOpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode)
{
    switch(openGLPolygonRenderOrderMode)
    {
        case PXOpenGLPolygonRenderOrderModeClockwise:
            return GL_CW;
        case PXOpenGLPolygonRenderOrderModeCounterClockwise:
            return GL_CCW;

        default:
            return -1;
    }
}
int PXAPI PXOpenGLRenderBufferFormatToID(const PXOpenGLRenderBufferFormat dataType)
{
    switch(dataType)
    {
        case PXOpenGLRenderBufferFormatDepthComponent:
            return GL_DEPTH_COMPONENT;
        case PXOpenGLRenderBufferFormatDepth24Stencil8:
            return GL_DEPTH24_STENCIL8;
        case PXOpenGLRenderBufferFormatInvalid:
        default:
            return -1;
    }
}

GLenum PXAPI PXOpenGLTextureParameterModeToID(const PXOpenGLTextureParameterMode textureParameterMode)
{
    switch(textureParameterMode)
    {
        case PXOpenGLDEPTH_STENCIL_TextureMODE:
            return GL_DEPTH_STENCIL_TEXTURE_MODE;
        case PXOpenGLTextureBASE_LEVEL:
            return  GL_TEXTURE_BASE_LEVEL;
        case PXOpenGLTextureCOMPARE_FUNC:
            return  GL_TEXTURE_COMPARE_FUNC;
        case PXOpenGLTextureCOMPARE_MODE:
            return GL_TEXTURE_COMPARE_MODE;
        case PXOpenGLTextureLOD_BIAS:
            return  GL_TEXTURE_LOD_BIAS;
        case PXOpenGLTextureMIN_FILTER:
            return  GL_TEXTURE_MIN_FILTER;
        case PXOpenGLTextureMAG_FILTER:
            return  GL_TEXTURE_MAG_FILTER;
        case PXOpenGLTextureMIN_LOD:
            return  GL_TEXTURE_MIN_LOD;
        case PXOpenGLTextureMAX_LOD:
            return  GL_TEXTURE_MAX_LOD;
        case PXOpenGLTextureMAX_LEVEL:
            return  GL_TEXTURE_MAX_LEVEL;
        case PXOpenGLTextureSWIZZLE_R:
            return  GL_TEXTURE_SWIZZLE_R;
        case PXOpenGLTextureSWIZZLE_G:
            return GL_TEXTURE_SWIZZLE_G;
        case PXOpenGLTextureSWIZZLE_B:
            return GL_TEXTURE_SWIZZLE_B;
        case PXOpenGLTextureSWIZZLE_A:
            return  GL_TEXTURE_SWIZZLE_A;
        case PXOpenGLTextureWRAP_S:
            return  GL_TEXTURE_WRAP_S;
        case PXOpenGLTextureWRAP_T:
            return  GL_TEXTURE_WRAP_T;
        case PXOpenGLTextureWRAP_R:
            return  GL_TEXTURE_WRAP_R;
        case PXOpenGLTextureBORDER_COLOR:
            return  GL_TEXTURE_BORDER_COLOR;
        case PXOpenGLTextureSWIZZLE_RGBA:
            return  GL_TEXTURE_SWIZZLE_RGBA;

        case PXOpenGLTextureParameterModoInvalid:
        default:
            return -1;
    }
}

PXInt32U PXAPI PXOpenGLRenderModeToID(const PXGraphicDrawMode pxGraphicDrawMode)
{
    switch(pxGraphicDrawMode)
    {
        case PXGraphicDrawModePoint:
            return GL_POINTS;
        case PXGraphicDrawModeLine:
            return GL_LINES;
        case PXGraphicDrawModeLineLoop:
            return GL_LINE_LOOP;
        case PXGraphicDrawModeLineStrip:
            return GL_LINE_STRIP;
        case PXGraphicDrawModeTriangle:
            return GL_TRIANGLES;
        case PXGraphicDrawModeTriangleStrip:
            return GL_TRIANGLE_STRIP;
        case PXGraphicDrawModeTriangleFAN:
            return GL_TRIANGLE_FAN;
        case PXGraphicDrawModeSquare:
            return GL_QUADS;
        case PXGraphicDrawModeSquareStrip:
            return GL_QUAD_STRIP;
        case PXGraphicDrawModePatches:
            return GL_POLYGON;

        default:
            return -1;
    }
}

int PXAPI PXOpenGLBufferTypeToID(const PXOpenGLBufferType openGLBufferType)
{
    switch(openGLBufferType)
    {
        case PXOpenGLBufferArray:
            return GL_ARRAY_BUFFER;
        case PXOpenGLBufferAtomicCounter:
            return GL_ATOMIC_COUNTER_BUFFER;
            //  case PXOpenGLBufferCopyRead: return GL_COPY_READ_BUFFER;
            // case PXOpenGLBufferCopyWrite: return GL_COPY_WRITE_BUFFER;
        case PXOpenGLBufferDispatchIndirect:
            return GL_DISPATCH_INDIRECT_BUFFER;
            //  case PXOpenGLBufferDrawIndirect: return GL_DRAW_INDIRECT_BUFFER;
        case PXOpenGLBufferElementArray:
            return GL_ELEMENT_ARRAY_BUFFER;
        case PXOpenGLBufferPixelPack:
            return GL_PIXEL_PACK_BUFFER;
        case PXOpenGLBufferPixelUnpack:
            return GL_PIXEL_UNPACK_BUFFER;
            //  case PXOpenGLBufferQuery: return GL_QUERY_BUFFER;
            //case PXOpenGLBufferShaderStorage: return GL_SHADER_STORAGE_BUFFER;
        case PXOpenGLBufferTexture:
            return GL_TEXTURE_BUFFER;
        case PXOpenGLBufferTransformFeedback:
            return GL_TRANSFORM_FEEDBACK_BUFFER;
        case PXOpenGLBufferUniform:
            return GL_UNIFORM_BUFFER;

        default:
            return -1;
    }
}

GLint PXAPI PXOpenGLTextureParameterValueToID(const PXOpenGLTextureParameterValue openGLTextureParameterValue)
{
    switch(openGLTextureParameterValue)
    {
        case PXOpenGLTextureParameterValueNEAREST:
            return GL_NEAREST;
        case PXOpenGLTextureParameterValueLINEAR:
            return GL_LINEAR;

        case PXOpenGLTextureParameterValueClampToEdge:
            return GL_CLAMP_TO_EDGE;
        case PXOpenGLTextureParameterValueClampToBorder:
            return GL_CLAMP_TO_BORDER;
        case PXOpenGLTextureParameterValueMirroredRepeat:
            return  GL_MIRRORED_REPEAT;
        case PXOpenGLTextureParameterValueRepeat:
            return GL_REPEAT;
        case PXOpenGLTextureParameterValueMirrorClampToEdge:
            return  GL_MIRROR_CLAMP_TO_EDGE;
        case PXOpenGLTextureParameterValueWrapS:
            return GL_TEXTURE_WRAP_S;
        case PXOpenGLTextureParameterValueWrapR:
            return GL_TEXTURE_WRAP_T;

        case PXOpenGLTextureParameterValueInvalid:
        default:
            return -1;
    }
}

int PXAPI PXOpenGLStoreModeToID(const PXOpenGLStoreMode openGLStoreMode)
{
    switch(openGLStoreMode)
    {
        case PXOpenGLStoreStreamDraw:
            return GL_STREAM_DRAW;
        case PXOpenGLStoreStreamRead:
            return GL_STREAM_READ;
        case PXOpenGLStoreStreamCopy:
            return GL_STREAM_COPY;
        case PXOpenGLStoreStaticDraw:
            return GL_STATIC_DRAW;
        case PXOpenGLStoreStaticREAD:
            return GL_STATIC_READ;
        case PXOpenGLStoreStaticCOPY:
            return GL_STATIC_COPY;
        case PXOpenGLStoreDynamicDraw:
            return GL_DYNAMIC_DRAW;
        case PXOpenGLStoreDynamicRead:
            return GL_DYNAMIC_READ;
        case PXOpenGLStoreDynamicCopy:
            return GL_DYNAMIC_COPY;

        default:
            return -1;
    }
}

GLenum PXAPI PXOpenGLDrawOrderToID(const PXOpenGLDrawOrderMode openGLDrawOrderMode)
{
    switch(openGLDrawOrderMode)
    {
        case PXOpenGLDrawOrderModeClockwise:
            return GL_CW;
        case PXOpenGLDrawOrderModeCounterClockwise:
            return GL_CCW;

        case PXOpenGLDrawOrderModeInvalid:
            return -1;
    }
}

int PXAPI PXOpenGLToggleToID(const PXOpenGLToggle openGLToggle)
{
    switch(openGLToggle)
    {
        case PXOpenGLToggleTextureCubeMapSeamless:
            return GL_TEXTURE_CUBE_MAP_SEAMLESS;

        case PXOpenGLALPHA_TEST:
            return GL_ALPHA_TEST;
        case PXOpenGLAUTO_NORMAL:
            return GL_AUTO_NORMAL;
        case PXOpenGLBLEND:
            return GL_BLEND;
            //case PXOpenGLCLIP_PLANEi :return GL_CLIP_PLANEi;
        case PXOpenGLCOLOR_LOGIC_OP:
            return GL_COLOR_LOGIC_OP;
        case PXOpenGLCOLOR_MATERIAL:
            return GL_COLOR_MATERIAL;
            //case PXOpenGLCOLOR_SUM :return GL_COLOR_SUM;
        case PXOpenGLCOLOR_TABLE:
            return GL_COLOR_TABLE;
        case PXOpenGLCONVOLUTION_1D:
            return GL_CONVOLUTION_1D;
        case PXOpenGLCONVOLUTION_2D:
            return GL_CONVOLUTION_2D;
        case PXOpenGLCULL_FACE:
            return GL_CULL_FACE;
        case PXOpenGLDEPTH_TEST:
            return GL_DEPTH_TEST;
        case PXOpenGLDITHER:
            return GL_DITHER;
        case PXOpenGLFOG:
            return GL_FOG;
        case PXOpenGLHISTOGRAM:
            return GL_HISTOGRAM;
        case PXOpenGLINDEX_LOGIC_OP:
            return GL_INDEX_LOGIC_OP;
            //case PXOpenGLLIGHTi :return GL_LIGHTi;
        case PXOpenGLLIGHTING:
            return GL_LIGHTING;
        case PXOpenGLLINE_SMOOTH:
            return GL_LINE_SMOOTH;
        case PXOpenGLLINE_STIPPLE:
            return GL_LINE_STIPPLE;
        case PXOpenGLMAP1_COLOR_4:
            return GL_MAP1_COLOR_4;
        case PXOpenGLMAP1_INDEX:
            return GL_MAP1_INDEX;
        case PXOpenGLMAP1_NORMAL:
            return GL_MAP1_NORMAL;
        case PXOpenGLMAP1_TEXTURE_COORD_1:
            return GL_MAP1_TEXTURE_COORD_1;
        case PXOpenGLMAP1_TEXTURE_COORD_2:
            return GL_MAP1_TEXTURE_COORD_2;
        case PXOpenGLMAP1_TEXTURE_COORD_3:
            return GL_MAP1_TEXTURE_COORD_3;
        case PXOpenGLMAP1_TEXTURE_COORD_4:
            return GL_MAP1_TEXTURE_COORD_4;
        case PXOpenGLMAP1_VERTEX_3:
            return GL_MAP1_VERTEX_3;
        case PXOpenGLMAP1_VERTEX_4:
            return GL_MAP1_VERTEX_4;
        case PXOpenGLMAP2_COLOR_4:
            return GL_MAP2_COLOR_4;
        case PXOpenGLMAP2_INDEX:
            return GL_MAP2_INDEX;
        case PXOpenGLMAP2_NORMAL:
            return GL_MAP2_NORMAL;
        case PXOpenGLMAP2_TEXTURE_COORD_1:
            return GL_MAP2_TEXTURE_COORD_1;
        case PXOpenGLMAP2_TEXTURE_COORD_2:
            return GL_MAP2_TEXTURE_COORD_2;
        case PXOpenGLMAP2_TEXTURE_COORD_3:
            return GL_MAP2_TEXTURE_COORD_3;
        case PXOpenGLMAP2_TEXTURE_COORD_4:
            return GL_MAP2_TEXTURE_COORD_4;
        case PXOpenGLMAP2_VERTEX_3:
            return GL_MAP2_VERTEX_3;
        case PXOpenGLMAP2_VERTEX_4:
            return GL_MAP2_VERTEX_4;
        case PXOpenGLMINMAX:
            return GL_MINMAX;
        case PXOpenGLMULTISAMPLE:
            return GL_MULTISAMPLE;
        case PXOpenGLNORMALIZE:
            return GL_NORMALIZE;
        case PXOpenGLPOINT_SMOOTH:
            return GL_POINT_SMOOTH;
        case PXOpenGLPOINT_SPRITE:
            return GL_POINT_SPRITE;
        case PXOpenGLPOLYGON_OFFSET_FILL:
            return GL_POLYGON_OFFSET_FILL;
        case PXOpenGLPOLYGON_OFFSET_LINE:
            return GL_POLYGON_OFFSET_LINE;
        case PXOpenGLPOLYGON_OFFSET_POINT:
            return GL_POLYGON_OFFSET_POINT;
        case PXOpenGLPOLYGON_SMOOTH:
            return GL_POLYGON_SMOOTH;
        case PXOpenGLPOLYGON_STIPPLE:
            return GL_POLYGON_STIPPLE;
        case PXOpenGLPOST_COLOR_MATRIX_COLOR_TABLE:
            return GL_POST_COLOR_MATRIX_COLOR_TABLE;
        case PXOpenGLPOST_CONVOLUTION_COLOR_TABLE:
            return GL_POST_CONVOLUTION_COLOR_TABLE;
            //case PXOpenGLRESCALE_NORMAL :return GL_RESCALE_NORMAL;
        case PXOpenGLSAMPLE_ALPHA_TO_COVERAGE:
            return GL_SAMPLE_ALPHA_TO_COVERAGE;
        case PXOpenGLSAMPLE_ALPHA_TO_ONE:
            return GL_SAMPLE_ALPHA_TO_ONE;
        case PXOpenGLSAMPLE_COVERAGE:
            return GL_SAMPLE_COVERAGE;
        case PXOpenGLSEPARABLE_2D:
            return GL_SEPARABLE_2D;
        case PXOpenGLSCISSOR_TEST:
            return GL_SCISSOR_TEST;
        case PXOpenGLSTENCIL_TEST:
            return GL_STENCIL_TEST;
        case PXOpenGLTEXTURE_1D:
            return GL_TEXTURE_1D;
        case PXOpenGLTEXTURE_2D:
            return GL_TEXTURE_2D;
        case PXOpenGLTEXTURE_3D:
            return GL_TEXTURE_3D;
        case PXOpenGLTEXTURE_CUBE_MAP:
            return GL_TEXTURE_CUBE_MAP;
        case PXOpenGLTEXTURE_GEN_Q:
            return GL_TEXTURE_GEN_Q;
        case PXOpenGLTEXTURE_GEN_R:
            return GL_TEXTURE_GEN_R;
        case PXOpenGLTEXTURE_GEN_S:
            return GL_TEXTURE_GEN_S;
        case PXOpenGLTEXTURE_GEN_T:
            return GL_TEXTURE_GEN_T;
        case PXOpenGLVERTEX_PROGRAM_POINT_SIZE:
            return GL_VERTEX_PROGRAM_POINT_SIZE;
        case PXOpenGLVERTEX_PROGRAM_TWO_SIDE:
            return GL_VERTEX_PROGRAM_TWO_SIDE;

        default:
            return -1;
    }
}

PXInt32U PXAPI PXOpenGLImageFormatToID(const PXColorFormat pxColorFormat, PXInt32U* const imageFormat, PXInt32U* const dataFormat)
{
    switch(pxColorFormat)
    {
        //case PXOpenGLImageFormatColorIndex: return GL_COLOR_INDEX;
        //case PXOpenGLImageFormatStencilIndex: return GL_STENCIL_INDEX;
        //case PXOpenGLImageFormatDepthComponent: return GL_DEPTH_COMPONENT;
        case PXColorFormatR8:
            *imageFormat = GL_RED;
            *dataFormat = GL_UNSIGNED_BYTE;
            break;
        case PXColorFormatG8:
            *imageFormat = GL_GREEN;
            *dataFormat = GL_UNSIGNED_BYTE;
            break;

        case PXColorFormatB8:
            *imageFormat = GL_BLUE;
            *dataFormat = GL_UNSIGNED_BYTE;
            break;

        case PXColorFormatA8:
            *imageFormat = GL_ALPHA;
            *dataFormat = GL_UNSIGNED_BYTE;
            break;



        case PXColorFormatR16:
            *imageFormat = GL_ALPHA;
            *dataFormat = GL_UNSIGNED_SHORT;
            break;

        case PXColorFormatG16:
            *imageFormat = GL_GREEN;
            *dataFormat = GL_UNSIGNED_SHORT;
            break;

        case PXColorFormatB16:
            *imageFormat = GL_BLUE;
            *dataFormat = GL_UNSIGNED_SHORT;
            break;

        case PXColorFormatA16:
            *imageFormat = GL_ALPHA;
            *dataFormat = GL_UNSIGNED_SHORT;
            break;


        case PXColorFormatRGBI8:
            *imageFormat = GL_RGB;
            *dataFormat = GL_UNSIGNED_BYTE;
            break;


        case PXColorFormatRGBF:
            *imageFormat = GL_RGB;
            *dataFormat = GL_FLOAT;
            break;

        case PXColorFormatRGBD:
            *imageFormat = GL_RGB;
            *dataFormat = GL_DOUBLE;
            break;


        case PXColorFormatRGBAI8:
            *imageFormat = GL_RGBA;
            *dataFormat = GL_UNSIGNED_BYTE;
            break;



        case PXColorFormatRGBAF:
            *imageFormat = GL_RGBA;
            *dataFormat = GL_FLOAT;
            break;

        case PXColorFormatRGBAD:
            *imageFormat = GL_RGBA;
            *dataFormat = GL_DOUBLE;
            break;


            //case PXOpenGLImageFormatBGR: return GL_BGR;
            //case PXOpenGLImageFormatBGRA: return GL_BGRA;
            // case PXOpenGLImageFormatLuminance: return GL_LUMINANCE;
            // case PXOpenGLImageFormatLuminanceAlpha: return GL_LUMINANCE_ALPHA;
        default:
            *imageFormat = -1;
            *dataFormat = -1;
            return PXFalse;
    }

    return PXTrue;
}

PXActionResult PXAPI PXOpenGLSwapIntervalSet(PXOpenGL* const pxOpenGL, const PXInt32U interval)
{
    if(!pxOpenGL->Binding.SwapIntervalSet)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOpenGLName,
            "SwapInterval",
            "Set not supported"
        );
#endif

        return PXActionRefusedNotSupportedByLibrary;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "SwapInterval",
        "Set to <%i>",
        interval
    );
#endif

#if OSUnix
    pxOpenGL->Binding.SwapIntervalSet(pxOpenGL->DisplayHandle, pxOpenGL->WindowHandle, interval);
#elif OSWindows
    pxOpenGL->Binding.SwapIntervalSet(interval);
#endif
    const PXActionResult swapSetResult = PXOpenGLErrorCurrent(pxOpenGL, 0);

    return swapSetResult;
}

PXActionResult PXAPI PXOpenGLSwapIntervalGet(PXOpenGL* const pxOpenGL, PXInt32U* const interval)
{
    *interval = pxOpenGL->Binding.SwapIntervalGet();

    return PXOpenGLErrorCurrent(pxOpenGL, 0);
}

PXInt32U PXAPI PXOpenGLTextureTypeToID(const PXGraphicTextureType pxGraphicTextureType)
{
    switch(pxGraphicTextureType)
    {
        case PXGraphicTextureType1D:
            return  GL_TEXTURE_1D;
        case PXGraphicTextureType2D:
            return  GL_TEXTURE_2D;
        case PXGraphicTextureType2DProxy:
            return  GL_PROXY_TEXTURE_2D;
        case PXGraphicTextureType3D:
            return   GL_TEXTURE_3D;
        case PXGraphicTextureType1DArray:
            return   GL_TEXTURE_1D_ARRAY;
        case PXGraphicTextureType1DBuffer:
            return   GL_PROXY_TEXTURE_1D_ARRAY;
        case PXGraphicTextureType2DArray:
            return   GL_TEXTURE_2D_ARRAY;
        case PXGraphicTextureTypeRectangle:
            return GL_TEXTURE_RECTANGLE;
        case PXGraphicTextureTypeRectangleProxy:
            return GL_PROXY_TEXTURE_RECTANGLE;
        case PXGraphicTextureTypeCubeContainer:
            return  GL_TEXTURE_CUBE_MAP;
        case PXGraphicTextureTypeCubeProxy:
            return GL_PROXY_TEXTURE_CUBE_MAP;
        case PXGraphicTextureTypeCubeTop:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
        case PXGraphicTextureTypeCubeDown:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
        case PXGraphicTextureTypeCubeFront:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
        case PXGraphicTextureTypeCubeBack:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
        case PXGraphicTextureTypeCubeLeft:
            return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
        case PXGraphicTextureTypeCubeRight:
            return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
        case PXGraphicTextureTypeCubeArray:
            return   GL_TEXTURE_CUBE_MAP_ARRAY;
        case PXGraphicTextureTypeBuffer:
            return   GL_TEXTURE_BUFFER;
        case PXGraphicTextureType2DMultiSample:
            return   GL_TEXTURE_2D_MULTISAMPLE;
        case PXGraphicTextureType2DMultiSampleArray:
            return   GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
        default:
            return -1;
    }
}

void PXAPI PXOpenGLBlendingMode(PXOpenGL* const pxOpenGL, const PXBlendingMode pxBlendingMode)
{
    GLenum sfactor = 0;
    GLenum dfactor = 0;

    switch(pxBlendingMode)
    {
        case PXBlendingModeOneToOne:
        {
            sfactor = GL_ONE;
            dfactor = GL_ONE;
            break;
        }
        case PXBlendingModeNone:
        {
            pxOpenGL->Binding.Disable(GL_BLEND);
            return;
        }
        case PXBlendingModeSoureAlphaOnly:
        {
            sfactor = GL_SRC_ALPHA;
            dfactor = GL_ONE_MINUS_SRC_ALPHA;
            break;
        }

        default:
            return;
    }

    pxOpenGL->Binding.Enable(GL_BLEND);
    pxOpenGL->Binding.BlendFunc(sfactor, dfactor);
}

PXOpenGLVersion PXAPI PXOpenGLVersionParse(const PXInt32U versionID)
{
    switch(versionID)
    {
        case PXInt24Make(1, 0, 0):
            return PXOpenGLVersion1x0x0;
        case PXInt24Make(1, 1, 0):
            return PXOpenGLVersion1x1x0;
        case PXInt24Make(1, 2, 0):
            return PXOpenGLVersion1x2x0;
        case PXInt24Make(1, 2, 1):
            return PXOpenGLVersion1x2x1;
        case PXInt24Make(1, 3, 0):
            return PXOpenGLVersion1x3x0;
        case PXInt24Make(1, 4, 0):
            return PXOpenGLVersion1x4x0;
        case PXInt24Make(1, 5, 0):
            return PXOpenGLVersion1x5x0;
        case PXInt24Make(2, 0, 0):
            return PXOpenGLVersion2x0x0;
        case PXInt24Make(2, 1, 0):
            return PXOpenGLVersion2x1x0;
        case PXInt24Make(3, 0, 0):
            return PXOpenGLVersion3x0x0;
        case PXInt24Make(3, 1, 0):
            return PXOpenGLVersion3x1x0;
        case PXInt24Make(3, 2, 0):
            return PXOpenGLVersion3x2x0;
        case PXInt24Make(3, 3, 0):
            return PXOpenGLVersion3x3x0;
        case PXInt24Make(4, 0, 0):
            return PXOpenGLVersion4x0x0;
        case PXInt24Make(4, 1, 0):
            return PXOpenGLVersion4x1x0;
        case PXInt24Make(4, 2, 0):
            return PXOpenGLVersion4x2x0;
        case PXInt24Make(4, 3, 0):
            return PXOpenGLVersion4x3x0;
        case PXInt24Make(4, 4, 0):
            return PXOpenGLVersion4x4x0;
        case PXInt24Make(4, 5, 0):
            return PXOpenGLVersion4x5x0;
        case PXInt24Make(4, 6, 0):
            return PXOpenGLVersion4x6x0;

        default:
            return  PXOpenGLVersionInvalid;
    }
}

void PXAPI PXOpenGLConstruct(PXOpenGL* const pxOpenGL)
{
    PXClear(PXOpenGL, pxOpenGL);
}

void PXAPI PXOpenGLDestruct(PXOpenGL* const pxOpenGL)
{

}

PXActionResult PXAPI PXOpenGLErrorCurrent(PXOpenGL* const pxOpenGL, PXBool wasSuccessultCall)
{
    for(;;)
    {
        const GLenum openGLErrorID = pxOpenGL->Binding.ErrorGet();
        PXActionResult openGLError = PXActionSuccessful;
        const char* errorText = 0;

        // Translate
        {
            switch(openGLErrorID)
            {
                case GL_NO_ERROR:
                    return openGLError;

                case GL_INVALID_ENUM:
                    openGLError = PXActionRefusedArgumentInvalid;
                    errorText = "GL_INVALID_ENUM";
                    break;

                case GL_INVALID_VALUE:
                    openGLError = PXActionRefusedArgumentInvalid;
                    errorText = "GL_INVALID_VALUE";
                    break;

                case GL_INVALID_OPERATION:
                    openGLError = PXActionRefusedInvalidOperationSpecified;
                    errorText = "GL_INVALID_OPERATION";
                    break;

                case GL_STACK_OVERFLOW:
                    openGLError = PXActionFailedStackOverflow;
                    errorText = "GL_STACK_OVERFLOW";
                    break;

                case GL_STACK_UNDERFLOW:
                    openGLError = PXActionFailedStackUnderflow;
                    errorText = "GL_STACK_UNDERFLOW";
                    break;

                case GL_OUT_OF_MEMORY:
                    openGLError = PXActionFailedMemoryAllocation;
                    errorText = "GL_OUT_OF_MEMORY";
                    break;
                    //case GL_INVALID_FRAMEBUFFER_OPERATION:    return 00;

                    /*
                    case GL_CONTEXT_LOST:
                    case GL_TABLE_TOO_LARGE1:
                    */

                default:
                    openGLError = PXActionInvalid;
                    errorText = "Invalid/Unkown";
                    break;
            }
        }

        if(wasSuccessultCall)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                PXOpenGLName,
                "Error",
                "Uncaught error detected! Please check previous calls and add checks."
            );
#endif

            wasSuccessultCall = PXFalse;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOpenGLName,
            "Error",
            "%s",
            errorText
        );
#endif
    }
}

void PXAPI PXOpenGLSet(PXOpenGL* const pxOpenGL, const PXOpenGL* const pxOpenGLSoure)
{
    PXCopy(PXOpenGL, pxOpenGLSoure, pxOpenGL);
}

void PXAPI PXOpenGLCopy(PXOpenGL* const pxOpenGL, const PXOpenGL* const pxOpenGLSoure)
{

}

#if OSWindows 


// Function to create an OpenGL context on a specific GPU
HGLRC CreateOpenGLContextOnGPUNV(PXOpenGL* const pxOpenGL, HWND hwnd, int gpuIndex)
{
    HGPUNV gpu;
    HGPUNV gpuList[8] = { NULL, NULL };

    pxOpenGL->Binding.DevicePhysicalList(gpuIndex, &gpu);

    gpuList[0] = gpu;
    HDC affinityDC = pxOpenGL->Binding.DeviceAffinityCreate(gpuList);
   

    HDC hdc = GetDC(hwnd);

    int pixelFormat = 0;
    UINT numFormats = 0;
    const int pixelFormatAttribs[] = 
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        0
    };

    pxOpenGL->Binding.PixelFormatChooseARB(affinityDC, pixelFormatAttribs, NULL, 1, &pixelFormat, &numFormats);
    //CHECK_WGL_ERROR(numFormats > 0);
    SetPixelFormat(hdc, pixelFormat, NULL);

    const int contextAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HGLRC hglrc = pxOpenGL->Binding.ContextCreateAttributes(affinityDC, NULL, contextAttribs);
   // CHECK_WGL_ERROR(hglrc != NULL);

    //ReleaseDC(hwnd, hdc);

    return hglrc;
}


// Function to create an OpenGL context on a specific GPU
HGLRC CreateOpenGLContextOnGPU(PXOpenGL* const pxOpenGL, HDC hdc, int gpuIndex)
{
    int pixelFormat = 0;
    UINT numFormats = 0;
    const int pixelFormatAttribs[] = 
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 24,
        0
    };

    pxOpenGL->Binding.PixelFormatChooseARB(hdc, pixelFormatAttribs, NULL, 1, &pixelFormat, &numFormats);

    SetPixelFormat(hdc, pixelFormat, NULL);

    const int contextAttribs[] = 
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
      //  WGL_GPU_ARB, gpuIndex, // Does not exist???
        0
    };

    HGLRC hglrc = pxOpenGL->Binding.ContextCreateAttributes(hdc, NULL, contextAttribs);

    return hglrc;
}
#endif


PXActionResult PXAPI PXOpenGLInitialize(PXOpenGL* const pxOpenGL, PXGraphicInitializeInfo* const pxGraphicInitializeInfo)
{
    PXWindow* const pxWindow = pxGraphicInitializeInfo->WindowReference;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Initialize",
        "Start..."
    );
#endif

    // Safety
    {
        if(!(pxOpenGL && pxGraphicInitializeInfo))
        {
            return PXActionRefusedArgumentNull;
        }
    }

    pxOpenGL->WindowRenderTarget = pxWindow;

#if OSUnix
    pxOpenGL->DisplayHandle = pxGraphicInitializeInfo->DisplayConnection;
#elif OSWindows   

#endif


#if 0
    pxOpenGL->Binding.AttachedWindow = pxGraphicInitializeInfo->WindowReference;

    if(pxOpenGL->Binding.AttachedWindow)
    {
        // TODO: bad Spinninglock
        if(!pxOpenGL->Binding.AttachedWindow->IsRunning)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                PXOpenGLName,
                "Initialize",
                "Wait for window..."
            );
#endif
            PXBool expected = PXTrue;
            PXAwaitInfo pxAwaitInfo;
            pxAwaitInfo.DataTarget = &pxOpenGL->Binding.AttachedWindow->IsRunning;
            pxAwaitInfo.DataExpect = &expected;
            pxAwaitInfo.DataSize = sizeof(PXBool);

            PXAwaitChange(&pxAwaitInfo);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                "Initialize",
                "Window done, proceed"
            );
#endif
        }
    }
#endif

    // Open Library
    {
        const PXActionResult libOpenResult = PXLibraryOpenA(&pxOpenGL->LibraryOpenGL, PXOpenglLibraryName);

        if(PXActionSuccessful != libOpenResult)
        {
            return PXActionRefusedNotSupportedByOperatingSystem;
        }

        // Fetch starting funcions
        PXLibraryGetSymbolBinding(&pxOpenGL->LibraryOpenGL, &pxOpenGL->Binding, PXOpenGLBindingList, PXOpenGLBindingSize, PXFalse);
    }

    // Link functions
    {
        PXGraphic* pxGraphic = pxGraphicInitializeInfo->Graphic;
        pxGraphic->TextureAction = PXOpenGLTextureAction;
        pxGraphic->ShaderVariableSet = PXOpenGLShaderVariableSet;
        pxGraphic->ScreenBufferRead = PXOpenGLScreenBufferRead;
        pxGraphic->DrawModeSet = PXOpenGLDrawMode;
        pxGraphic->DrawColorRGBAF = PXOpenGLDrawColorRGBAF;
        pxGraphic->RectangleDraw = PXOpenGLRectangleDraw;
        pxGraphic->RectangleDrawTx = PXOpenGLRectangleDrawTx;
        pxGraphic->SwapIntervalSet = PXOpenGLSwapIntervalSet;
        pxGraphic->SwapIntervalGet = PXOpenGLSwapIntervalGet;
        pxGraphic->DevicePhysicalListAmount = PXOpenGLDevicePhysicalListAmount;
        pxGraphic->DevicePhysicalListFetch = PXOpenGLDevicePhysicalListFetch;
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
        pxGraphic->ModelRegister = PXOpenGLModelRegister;
        pxGraphic->ModelDraw = PXOpenGLModelDraw;
        pxGraphic->ModelSelect = PXNull;
        pxGraphic->ModelRelease = PXNull;
        pxGraphic->LightSet = PXOpenGLLightSet;
        pxGraphic->LightGet = PXOpenGLLightGet;
        pxGraphic->LightEnableSet = PXNull;
        pxGraphic->LightEnableGet = PXNull;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Initialize",
        "Context create try.."
    );
#endif


    // OpenGL - Context Create
    {


#if OSUnix
        int screen = DefaultScreen(pxOpenGL->DisplayHandle);
        const int attributeList[] =
        {
            GLX_RGBA,
            GLX_DEPTH_SIZE,
            24,
            GLX_DOUBLEBUFFER,
            None
        };
        const XVisualInfo* const visualInfo = glXChooseVisual(pxOpenGL->DisplayHandle, screen, attributeList);

        {
            const PXBool successful = visualInfo != 0;

            if(!successful)
            {
                return; // no appropriate visual found
            }
        }

        pxOpenGL->ContextHandle = glXCreateContext(pxOpenGL->DisplayHandle, visualInfo, NULL, GL_TRUE);

#elif OSWindows
        pxOpenGL->ContextHandle = pxOpenGL->Binding.ContextCreate(pxWindow->DeviceContextHandle);
#endif

        // glError invalid here;

        if(!pxOpenGL->ContextHandle) // Failed context create
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXOpenGLName,
                "Initialize",
                "Context create failed!"
            );
#endif

            return PXActionFailedInitialization;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Initialize",
            "Context creation successful"
        );
#endif

    }


    PXOpenGLSelect(pxOpenGL);


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Initialize",
        "Fetching newer function adresses with <%p>",
        pxOpenGL->Binding.FunctionFetch
    );
#endif

    // Fetch extended functions and override functions if newer ones exist
    if(pxOpenGL->Binding.FunctionFetch)
    {
        PXSize position = 0;

        void** bindingObject = &pxOpenGL->Binding;

        for(PXSize i = 0; ; ++i)
        {
            const char* const cursor = &PXOpenGLBindingList[position];
            const PXSize length = PXTextLengthA(cursor, PXTextUnkownLength);
            const PXBool invalidToProceed = !cursor || (i >= PXOpenGLBindingSize) || length == 0;

            if(invalidToProceed)
            {
                break;
            }

            position += length + 1;

            void* function = pxOpenGL->Binding.FunctionFetch(cursor);

            if(function)
            {
                bindingObject[i] = function;
            }
        }
    }




    // Fetch functions
    {
#if OSWindows
        // Make multi GOU context
        {
            if(pxOpenGL->Binding.ContextCreateAttributes)
            {
                const int attributeList[] =
                {
                    WGL_CONTEXT_MULTIGPU_ATTRIB_NV,
                    WGL_CONTEXT_MULTIGPU_ATTRIB_MULTI_DISPLAY_MULTICAST_NV,
                    0,
                    0
                };

                const HGLRC contextAttributes = pxOpenGL->Binding.ContextCreateAttributes
                (
                    pxWindow->DeviceContextHandle,
                    pxOpenGL->ContextHandle,
                    attributeList
                );

                const PXActionResult xx = PXOpenGLErrorCurrent(pxOpenGL, contextAttributes);
            }
        }
#endif
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Initialize",
        "Done fetching funtions"
    );
#endif



#if OSWindows // print extensions

    //-----------------------------------------------------
    // WGL-Extensions (Wiggle OpenGL)
    //-----------------------------------------------------
    {
#if 0
        if(pxOpenGL->Binding.ExtensionStringGet)
        {
          
            char* extensionString = pxOpenGL->Binding.ExtensionStringGet(pxOpenGL->WindowDeviceContextHandle);
            char* cursor = extensionString;
            PXInt32U wglExtenionAmount = 0;
            
            for(;;)
            {
                PXSize length = PXTextLengthUntilA(cursor, -1, ' ');

                if(0 == length)
                {
                    break;
                }

                ++wglExtenionAmount;

#if PXLogEnable
                char buffer[128];

                PXTextCopyA(cursor, length, buffer, 128);

                PXLogPrint
                (
                    PXLoggingInfo,
                    PXOpenGLName,
                    "Extension",
                    "%4i - %s",
                    wglExtenionAmount,
                    buffer
                );
#endif

                cursor = &cursor[length+1];
            }
        }
        else
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingWarning,
                PXOpenGLName,
                "Extension",
                "wglGetExtensionsStringARB not deteced.."                
            );
#endif
        }
#endif
    }

    //-----------------------------------------------------
    // General GL-Extensions
    //-----------------------------------------------------
    if(pxOpenGL->Binding.GetIntegerv)
    {
        // extensions
        int numberOfExtensions = 0;
        pxOpenGL->Binding.GetIntegerv(GL_NUM_EXTENSIONS, &numberOfExtensions);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Initialize",
            "Extensions detected <%i>",
            numberOfExtensions
        );
#endif

#if 0
        for(int i = 0; i < numberOfExtensions; ++i)
        {
            const char* extension = (const char*)pxOpenGL->Binding.StringI(GL_EXTENSIONS, i);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                "Extension",
                "%4i - %s",
                i + 1,
                extension
            );
#endif
        }
#endif
    }
#endif

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Initialize",
        "Enable error callback"
    );
#endif


    if(pxOpenGL->Binding.Enable)
    {
        pxOpenGL->Binding.Enable(GL_MULTISAMPLE);
    }

    PXFunctionInvoke(pxOpenGL->Binding.DebugMessage, PXOpenGLErrorMessageCallback, 0);
    PXFunctionInvoke(pxOpenGL->Binding.Enable, GL_DEBUG_OUTPUT);

    // Update view
    {
        // PXWindowSizeInfo pxWindowSizeInfo;



        //PXWindowSizeGet(pxOpenGL->Binding.WindowHandle, &pxWindowSizeInfo);

        //pxOpenGL->Binding.Viewport(0, 0, pxWindowSizeInfo.Width, pxWindowSizeInfo.Height);
    }


#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Initialize",
        "List devices"
    );
#endif

    // List devices
    {
        PXGraphicDevicePhysical pxGraphicDevicePhysical[4];
        PXClearList(PXGraphicDevicePhysical, &pxGraphicDevicePhysical, 4);

        PXInt32U devices = 2;

       // PXOpenGLDevicePhysicalListAmount(pxOpenGL, &devices);

        PXActionResult fetchResult = PXOpenGLDevicePhysicalListFetch(pxOpenGL, devices, pxGraphicDevicePhysical);

        if(PXActionSuccessful != fetchResult)
        {
            devices = 0;
        }

        for(PXInt32U i = 0; i < devices; i++)
        {
            PXText pxTextVideoMemoryCurrent;
            PXTextConstructNamedBufferA(&pxTextVideoMemoryCurrent, pxTextVideoMemoryCurrentBuffer, 32);
            PXTextFormatSize(&pxTextVideoMemoryCurrent, pxGraphicDevicePhysical[i].VideoMemoryCurrent);

            PXText pxTextVideoMemoryTotal;
            PXTextConstructNamedBufferA(&pxTextVideoMemoryTotal, pxTextVideoMemoryTotalBuffer, 32);
            PXTextFormatSize(&pxTextVideoMemoryTotal, pxGraphicDevicePhysical[i].VideoMemoryTotal);

            PXText pxTextVideoMemoryDedicated;
            PXTextConstructNamedBufferA(&pxTextVideoMemoryDedicated, pxTextVideoMemoryDedicatedBuffer, 32);
            PXTextFormatSize(&pxTextVideoMemoryDedicated, pxGraphicDevicePhysical[i].VideoMemoryDedicated);

            PXText pxTextVideoMemorySize;
            PXTextConstructNamedBufferA(&pxTextVideoMemorySize, pxTextVideoMemorySizeBuffer, 32);
            PXTextFormatSize(&pxTextVideoMemorySize, pxGraphicDevicePhysical[i].VideoMemoryEvictionSize);

            int percent = 0;

            if(0 != pxGraphicDevicePhysical[i].VideoMemoryTotal)
            {
                percent = (pxGraphicDevicePhysical[i].VideoMemoryCurrent * 100) / pxGraphicDevicePhysical[i].VideoMemoryTotal;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                "Initialize",
                "Device\n"
                "%12s : %s\n"
                "%12s : %s\n"
                "%12s : %s / %s %i%% from (%s)\n"
                "%12s : %i / %s",
                "Vendor", pxGraphicDevicePhysical[i].Vendor,
                "Renderer", pxGraphicDevicePhysical[i].Renderer,

                "Memory",
                pxTextVideoMemoryCurrent.TextA,
                pxTextVideoMemoryTotal.TextA,
                percent,
                pxTextVideoMemoryDedicated.TextA,

                "Evicted",
                pxGraphicDevicePhysical[i].VideoMemoryEvictionCount,
                pxTextVideoMemorySize.TextA
            );
#endif
        }
    }

    PXOpenGLDeselect(pxOpenGL);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Initialize",
        "Finished"
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLSelect(PXOpenGL* const pxOpenGL)
{
    PXWindow* const pxWindow = pxOpenGL->WindowRenderTarget;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Context-Select",
        "Bind to current thread\n"
#if OSUnix
        "%10s %30s <%p>\n"
        "%10s %30s <%p>\n"
        "%10s %30s <%p>",
        "Display*", "DisplayHandle", pxOpenGL->DisplayHandle,
        "Window", "WindowRootHandle", pxOpenGL->WindowHandle,
        "GC", "GraphicContent", pxOpenGL->ContextHandle
#elif OSWindows
        "%10s %30s <%p>\n"
        "%10s %30s <%p>",
        "HDC", "WindowDeviceContextHandle", pxWindow->DeviceContextHandle,
        "HGLRC", "ContextHandle", pxOpenGL->ContextHandle
#endif
    );
#endif

    if(!pxOpenGL)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
    const int result = glXMakeCurrent(pxOpenGL->DisplayHandle, pxOpenGL->WindowHandle, pxOpenGL->ContextHandle);
#elif OSWindows
    const BOOL result = pxOpenGL->Binding.MakeCurrent(pxWindow->DeviceContextHandle, pxOpenGL->ContextHandle);
#endif

    if(!result)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOpenGLName,
            "Context",
            "Failed binding to current thread"
        );
#endif

        pxOpenGL->Flags &= ~PXOpenGLStateIsBoundToThread;

        return PXActionFailedContextBinding;
    }

    pxOpenGL->Flags |= PXOpenGLStateIsBoundToThread;

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Context",
        "OK"
    );
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLDeselect(PXOpenGL* const pxOpenGL)
{
#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Context",
        "Unbind from thread"
    );
#endif

    if(!pxOpenGL)
    {
        return PXActionRefusedArgumentNull;
    }

    const PXBool successful =
#if OSUnix
        glXMakeCurrent(PXNull, pxOpenGL->WindowHandle, pxOpenGL->ContextHandle);
#elif OSWindows
        pxOpenGL->Binding.MakeCurrent(0, 0);
#endif

    // glError invalid here- no context anymore
    if(!successful)
    {
        return PXActionFailedContextRelease;
    }

    pxOpenGL->Flags &= ~PXOpenGLStateIsBoundToThread;

    return PXActionSuccessful;
}

PXInt64S PXAPI PXOpenGLIntergetGet(PXOpenGL* const pxOpenGL, const GLenum enumID)
{
    PXInt64S result = 0;

    if(pxOpenGL->Binding.GetInteger64v)
    {
        PXInt64S currentMemory = 0;

        pxOpenGL->Binding.GetInteger64v(enumID, &currentMemory);

        result = currentMemory;
    }
    else if(pxOpenGL->Binding.GetIntegerv)
    {
        PXInt32S currentMemory = 0;

        pxOpenGL->Binding.GetIntegerv(enumID, &currentMemory);

        result = currentMemory;
    }
    else
    {
        return result;
    }


    switch(enumID)
    {
        case GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX:
        case GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX:
        case GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX:
        case GPU_MEMORY_INFO_EVICTION_COUNT_NVX:
        case GPU_MEMORY_INFO_EVICTED_MEMORY_NVX:
        {
            result *= 1000;
            break;
        }
    }


    return result; // Returning -1 is not a good idea
}

PXActionResult PXAPI PXOpenGLDevicePhysicalListAmount(PXOpenGL* const pxOpenGL, PXInt32U* const amount)
{
    if(!(pxOpenGL && amount))
    {
        return PXActionRefusedArgumentNull;
    }

    *amount = 0;

#if OSUnix

    *amount = 1; // ToDo: Bad Hack, get the correct amount!

    return PXActionRefusedNotImplemented;

#elif OSWindows

    HGPUNV listOfHandles[64];

    if(!pxOpenGL->Binding.DevicePhysicalList)
    {
        return PXActionRefusedNotSupportedByLibrary;
    }

    for(PXSize gpuID = 0; pxOpenGL->Binding.DevicePhysicalList(gpuID, &listOfHandles[gpuID]); ++gpuID)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Device",
            "Detected <%i>",
            gpuID
        );
#endif

        GPU_DEVICE gpuDEVICE;
        PXClear(GPU_DEVICE, &gpuDEVICE);
        gpuDEVICE.cb = sizeof(GPU_DEVICE);

        for(PXSize displayID = 0; displayID < pxOpenGL->Binding.DevicePhysicalListB(listOfHandles[gpuID], displayID, &gpuDEVICE); ++displayID)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                "Device",
                "DeviceName : %s, %s (%i, %i)",
                gpuDEVICE.DeviceName,
                gpuDEVICE.DeviceString,
                gpuDEVICE.rcVirtualScreen.left,
                gpuDEVICE.rcVirtualScreen.bottom
            );
#endif
            *amount = displayID + 1;
        }
    }

    return PXActionSuccessful;
#endif
}

PXActionResult PXAPI PXOpenGLDevicePhysicalListFetch(PXOpenGL* const pxOpenGL, const PXInt32U amount, PXGraphicDevicePhysical* const pxGraphicDevicePhysicalList)
{
    if(!(pxOpenGL && pxGraphicDevicePhysicalList))
    {
        return PXActionRefusedArgumentNull;
    }

    //if(amount == 0)
   // {
    //    return PXActionSuccessful;
   // }

    if(!(PXOpenGLStateIsBoundToThread & pxOpenGL->Flags))
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOpenGLName,
            "Device-Fetch",
            "Context not bound to thread!"
        );
#endif

        return PXActionRefusedContextNotBound;
    }

    PXClearList(PXGraphicDevicePhysical, pxGraphicDevicePhysicalList, amount);

    if(!pxOpenGL->Binding.GetString)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Device-Fetch",
            "GetString missing, cant resolve names"
        );
#endif

        PXTextCopyA("???", 3, pxGraphicDevicePhysicalList->Vendor, PXDeviceOpenGLVendorSize);
        PXTextCopyA("???", 3, pxGraphicDevicePhysicalList->Renderer, PXDeviceOpenGLRendererSize);
        PXTextCopyA("???", 3, pxGraphicDevicePhysicalList->Shader, PXDeviceOpenGLShaderSize);

        return PXActionInvalid;
    }

    // Returns the company responsible for this GL implementation.
    // This name does not change from release to release.
    const char* vendor = pxOpenGL->Binding.GetString(GL_VENDOR);

    PXTextCopyA(vendor, PXTextUnkownLength, pxGraphicDevicePhysicalList->Vendor, PXDeviceOpenGLVendorSize);

    // Returns the name of the renderer.
    // This name is typically specific to a particular configuration of a hardware platform.It does not change from release to release.
    const char* renderer = pxOpenGL->Binding.GetString(GL_RENDERER);

    PXTextCopyA(renderer, PXTextUnkownLength, pxGraphicDevicePhysicalList->Renderer, PXDeviceOpenGLRendererSize);

    const char* glslVersion = pxOpenGL->Binding.GetString(GL_SHADING_LANGUAGE_VERSION); //    Returns a version or release number.

    PXTextCopyA(glslVersion, PXTextUnkownLength, pxGraphicDevicePhysicalList->Shader, PXDeviceOpenGLShaderSize);

    // Parse version, Returns a version or release number.
    {
        const char* version = pxOpenGL->Binding.GetString(GL_VERSION);
        PXVersion pxVersion;

        PXVersionFromString(&pxVersion, version);

        const PXInt32U id = PXInt24Make(pxVersion.Major, pxVersion.Minor, pxVersion.Build);
        pxOpenGL->Version = PXOpenGLVersionParse(id);
    }

    pxGraphicDevicePhysicalList->VideoMemoryDedicated = PXOpenGLIntergetGet(pxOpenGL, GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX);
    pxGraphicDevicePhysicalList->VideoMemoryTotal = PXOpenGLIntergetGet(pxOpenGL, GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX);
    pxGraphicDevicePhysicalList->VideoMemoryCurrent = PXOpenGLIntergetGet(pxOpenGL, GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX);
    pxGraphicDevicePhysicalList->VideoMemoryEvictionCount = PXOpenGLIntergetGet(pxOpenGL, GPU_MEMORY_INFO_EVICTION_COUNT_NVX);
    pxGraphicDevicePhysicalList->VideoMemoryEvictionSize = PXOpenGLIntergetGet(pxOpenGL, GPU_MEMORY_INFO_EVICTED_MEMORY_NVX);

    // Nvidea gives us the free memory, not the used one
    pxGraphicDevicePhysicalList->VideoMemoryCurrent = pxGraphicDevicePhysicalList->VideoMemoryTotal - pxGraphicDevicePhysicalList->VideoMemoryCurrent;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLScreenBufferRead(PXOpenGL* const pxOpenGL, PXImage* const pxImage)
{
    GLenum formatStructure = 0;
    GLenum formatData = 0;

    PXOpenGLImageFormatToID(pxImage->Format, &formatStructure, &formatData);

    PXImageResize(pxImage, pxImage->Format, pxImage->Width, pxImage->Height);

    pxOpenGL->Binding.ReadPixels(0, 0, pxImage->Width, pxImage->Height, formatStructure, formatData, pxImage->PixelData);

    const PXActionResult pxActionResult = PXOpenGLErrorCurrent(pxOpenGL, 0);

    return pxActionResult;
}

PXActionResult PXAPI PXOpenGLRelease(PXOpenGL* const pxOpenGL)
{
    if(!pxOpenGL)
    {
        return PXActionRefusedArgumentNull;
    }

#if OSUnix
    const int resultID = glXMakeCurrent(pxOpenGL->DisplayHandle, PXNull, PXNull);
#elif OSWindows
    const BOOL resultID = pxOpenGL->Binding.ContextDelete(pxOpenGL->ContextHandle);
#else
    return PXActionRefusedNotSupported;
#endif


#if OSUnix || OSWindows

    const PXActionResult result = PXOpenGLErrorCurrent(pxOpenGL, resultID);

    if(PXActionSuccessful != result)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOpenGLName,
            "Release",
            "Failed!"
        );
#endif

        return result;
    }

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Release",
        "OK"
    );
#endif

    // Remove reference
    pxOpenGL->ContextHandle = PXNull;
#endif

    return PXActionSuccessful;
}

void PXAPI PXOpenGLFlush(PXOpenGL* const pxOpenGL)
{
    pxOpenGL->Binding.Flush();
}

void PXAPI PXOpenGLViewPortSet(PXOpenGL* const pxOpenGL, const PXViewPort* const pxViewPort)
{
    pxOpenGL->Binding.Viewport(pxViewPort->X, pxViewPort->Y, pxViewPort->Width, pxViewPort->Height);
}

void PXAPI PXOpenGLViewPortGet(PXOpenGL* const pxOpenGL, PXViewPort* const pxViewPort)
{

}

void PXAPI PXOpenGLPolygonRenderOrder(const PXOpenGL* const pxOpenGL, const PXOpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode)
{
    const GLenum openGLPolygonRenderOrderModeID = PXOpenGLPolygonRenderOrderModeToID(openGLPolygonRenderOrderMode);

    pxOpenGL->Binding.FrontFace(openGLPolygonRenderOrderModeID);
}

void PXAPI PXOpenGLSettingChange(PXOpenGL* const pxOpenGL, const PXOpenGLToggle toggle, const PXBool state)
{
    const GLenum settingID = PXOpenGLToggleToID(toggle);

    if(state)
    {
        pxOpenGL->Binding.Enable(settingID);
    }
    else
    {
        pxOpenGL->Binding.Disable(settingID);
    }
}

void PXAPI PXOpenGLDrawOrder(PXOpenGL* const pxOpenGL, const PXOpenGLDrawOrderMode openGLDrawOrderMode)
{

}

void PXAPI PXOpenGLDrawMode(PXOpenGL* const pxOpenGL, const PXGraphicDrawFillMode pxGraphicDrawFillMode, const int cullingMode)
{
    GLenum fillMode;

    switch(pxGraphicDrawFillMode)
    {
        case PXGraphicDrawFillModePoints:
            fillMode = GL_POINT;
            break;
        case PXGraphicDrawFillModeLines:
            fillMode = GL_LINE;
            break;
        case PXGraphicDrawFillModeFill:
            fillMode = GL_FILL;
            break;
        case PXGraphicDrawFillModeInvalid:
        default:
            return;
    }

    pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, fillMode);
}

void PXAPI PXOpenGLClear(PXOpenGL* const pxOpenGL, const PXColorRGBAF* const pxColorRGBAF)
{
    pxOpenGL->Binding.ClearColor(pxColorRGBAF->Red, pxColorRGBAF->Green, pxColorRGBAF->Blue, pxColorRGBAF->Alpha);
    pxOpenGL->Binding.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

PXBool PXAPI PXOpenGLSceneDeploy(PXOpenGL* const pxOpenGL)
{
#if 0
    PXWindow* const pxWindow = pxOpenGL->Binding.AttachedWindow;

    if(pxWindow->HasSizeChanged) // Check if view has changed.
    {
        // pxOpenGL->Binding.Viewport(0, 0, pxWindow->Width, pxWindow->Height); // Map view to full screen
        //  pxWindow->HasSizeChanged = PXFalse; // No change needed after this change
    }

    // NOTE: Possible improvement? Using 'wglSwapLayerBuffers()'

    const PXBool success = PXWindowFrameBufferSwap(pxWindow);
#endif

    // SwapBuffers(pxOpenGL->Binding.WindowDeviceContextHandle);

    return PXActionSuccessful;
}

void PXAPI PXOpenGLDrawScaleF(PXOpenGL* const pxOpenGL, const PXF32 x, const PXF32 y, const PXF32 z)
{
    pxOpenGL->Binding.Scaled(x, y, z);
}

void PXAPI PXOpenGLDrawBegin(PXOpenGL* const pxOpenGL, const PXGraphicDrawMode openGLRenderMode)
{
    const int openGLRenderModeID = PXOpenGLRenderModeToID(openGLRenderMode);

    pxOpenGL->Binding.Begin(openGLRenderModeID);
}

void PXAPI PXOpenGLDrawVertexXYZF(PXOpenGL* const pxOpenGL, const PXF32 x, const PXF32 y, const PXF32 z)
{
    pxOpenGL->Binding.Vertex3f(x, y, z);
}

void PXAPI PXOpenGLDrawColorRGBF(PXOpenGL* const pxOpenGL, const PXF32 red, const PXF32 green, const PXF32 blue)
{
    pxOpenGL->Binding.Color3f(red, green, blue);
}

void PXAPI PXOpenGLDrawColorRGBAF(PXOpenGL* const pxOpenGL, const PXF32 red, const PXF32 green, const PXF32 blue, const PXF32 alpha)
{
    pxOpenGL->Binding.Color4f(red, green, blue, alpha);
}

void PXAPI PXOpenGLDrawEnd(PXOpenGL* const pxOpenGL)
{
    pxOpenGL->Binding.End();
}

PXSize indexxxx = 0;

PXActionResult PXAPI PXOpenGLModelDraw(PXOpenGL* const pxOpenGL, const PXRenderEntity* const pxRenderEntity)
{
    if(!(pxOpenGL && pxRenderEntity))
    {
        return PXActionRefusedArgumentNull;
    }


    PXCamera* const pxCamera = pxRenderEntity->CameraReference;
    PXMatrix4x4F* const matrixModel = &pxRenderEntity->MatrixModel;
    PXMatrix4x4F* const matrixView = &pxCamera->MatrixView;
    PXMatrix4x4F* const matrixProjection = &pxCamera->MatrixProjection;

    PXShaderProgram* const pxShaderProgram = pxRenderEntity->ShaderProgramReference;
    PXModel* const pxModel = (PXModel*)pxRenderEntity->ObjectReference;
    PXMesh* const pxMesh = &pxModel->Mesh;
   
    //PXVertexBuffer* const pxVertexBuffer = &pxModel->Mesh.VertexBuffer;

    PXIndexBuffer* const pxIndexBuffer = &pxModel->Mesh.IndexBuffer;

 //   if(!pxVertexBuffer->VertexData) // Has data?
  //  {
   //     return PXActionRefusedArgumentInvalid;
  //  }

    const PXBool canUseShader = pxShaderProgram && pxOpenGL->Binding.ShaderProgramUse;
    const PXBool supportVAO = pxOpenGL->Binding.VertexArrayBind != PXNull;
    const PXBool supportBuffers = pxOpenGL->Binding.BufferBind != PXNull;
    const PXInt32U indexBufferTypeID = PXOpenGLTypeToID(pxIndexBuffer->IndexDataType);
    const PXBool hasIndexBuffer = PXIndexBufferIsUsed(pxIndexBuffer);

    void* indexData = 0;

   // indexData = pxMesh->IndexBuffer.DataIndexPosition;

    if(supportVAO)
    {
        pxOpenGL->Binding.VertexArrayBind(pxModel->Mesh.Info.Handle.OpenGLID); // Select VAO

       /*
        if(pxVertexBuffer->VertexData)
        {
            pxOpenGL->Binding.BufferBind(GL_ARRAY_BUFFER, pxVertexBuffer->Info.Handle.OpenGLID);
        }*/
        
        if(hasIndexBuffer)
        {
            pxOpenGL->Binding.BufferBind(GL_ELEMENT_ARRAY_BUFFER, pxIndexBuffer->Info.Handle.OpenGLID);
        }
        else
        {
            pxOpenGL->Binding.BufferBind(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }
    else
    {
        if(supportBuffers)
        {
            //pxOpenGL->Binding.BufferBind(GL_ARRAY_BUFFER, pxVertexBuffer->Info.Handle.OpenGLID); // Select VBO
           //// pxOpenGL->Binding.BufferBind(GL_ELEMENT_ARRAY_BUFFER, pxIndexBuffer->Info.Handle.OpenGLID); // Select IBO

           // assert(indexBufferTypeID != -1);

            indexData = 0;
        }
        else
        {
            /*
            // Setup vertex data client side
            switch(pxVertexBuffer->Format)
            {
                case PXVertexBufferFormatT2F_XYZ:
                {
                    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    //glEnableClientState(GL_VERTEX_ARRAY);
                    pxOpenGL->Binding.InterleavedArrays(GL_T2F_V3F, 0, pxVertexBuffer->VertexData);
                    break;
                }
                case PXVertexBufferFormatT2F_N3F_XYZ:
                {
                    //glEnableClientState(GL_TEXTURE_COORD_ARRAY);
                    //glEnableClientState(GL_NORMAL_ARRAY);
                    //glEnableClientState(GL_VERTEX_ARRAY);
                    pxOpenGL->Binding.InterleavedArrays(GL_T2F_N3F_V3F, 0, pxVertexBuffer->VertexData);
                    break;
                }
                default:
                    break;
            }
            */

            // Setup index data client side
            //glEnableClientState(GL_INDEX_ARRAY);

            GLenum indexTypeID = 0;

            switch(pxIndexBuffer->IndexDataType)
            {
                default:
                    break;
            }

            //glIndexPointer(indexBufferTypeID, 0, pxModel->IndexBuffer.IndexData);

            indexData = pxIndexBuffer->DataIndexPosition;
        }
    }

    //-----------------------------------------------------
    // ShaderSetup
    //-----------------------------------------------------
    PXMatrix4x4F modifiedViewMatrix;
    PXMatrix4x4F modifiedModelMatrix;

    PXMatrix4x4FCopy(matrixView, &modifiedViewMatrix);
    PXMatrix4x4FCopy(matrixModel, &modifiedModelMatrix);

    if(pxModel->IgnoreViewPosition)
    {
        PXMatrix4x4FResetAxisW(&modifiedViewMatrix);
    }

    if(pxModel->IgnoreViewRotation)
    {
        PXMatrix4x4FIdentity(&modifiedViewMatrix);

        PXMatrix4x4FMoveXYZ(&modifiedModelMatrix, 0, 0, -0.5);
        // PXMatrix4x4FScaleByXY(&modifiedModelMatrix, 0.5, 0.5);
    }

    // PXMatrix4x4FScaleByMargin(&modifiedModelMatrix, &pxModel->Margin);

    if(canUseShader)
    {
        PXOpenGLShaderProgramSelect(pxOpenGL, pxShaderProgram);

        PXShaderVariable pxShaderVariableList[6];
        PXClearList(PXShaderVariable, pxShaderVariableList, 6);
        PXTextCopyA("MatrixModel", 11, pxShaderVariableList[0].Name, 64);
        pxShaderVariableList[0].Amount = 1;
        pxShaderVariableList[0].Data = modifiedModelMatrix.Data;
        pxShaderVariableList[0].DataType = PXShaderVariableTypeMatrix4x4;

        PXTextCopyA("MatrixView", 11, pxShaderVariableList[1].Name, 64);
        pxShaderVariableList[1].Amount = 1;
        pxShaderVariableList[1].Data = modifiedViewMatrix.Data;
        pxShaderVariableList[1].DataType = PXShaderVariableTypeMatrix4x4;

        PXTextCopyA("MatrixProjection", 16, pxShaderVariableList[2].Name, 64);
        pxShaderVariableList[2].Amount = 1;
        pxShaderVariableList[2].Data = matrixProjection->Data;
        pxShaderVariableList[2].DataType = PXShaderVariableTypeMatrix4x4;


        PXF32 dummyValue[4] = { 1.0f,1.0f,1.0f,1.0f };

        PXF32* materialDiffuse = dummyValue;

        if(pxModel->Mesh.IndexBuffer.SegmentPrime.Material)
        {
            materialDiffuse = &pxModel->Mesh.IndexBuffer.SegmentPrime.Material->Diffuse;
        }




        PXTextCopyA("Material.Ambient", 17, pxShaderVariableList[3].Name, 64);
        pxShaderVariableList[3].Amount = 1;
        pxShaderVariableList[3].Data = dummyValue;
        pxShaderVariableList[3].DataType = PXShaderVariableTypePXF32Vector4;

        PXTextCopyA("Material.Diffuse", 17, pxShaderVariableList[4].Name, 64);
        pxShaderVariableList[4].Amount = 1;
        pxShaderVariableList[4].Data = materialDiffuse;
        pxShaderVariableList[4].DataType = PXShaderVariableTypePXF32Vector4;

        PXTextCopyA("Material.Specular", 18, pxShaderVariableList[5].Name, 64);
        pxShaderVariableList[5].Amount = 1;
        pxShaderVariableList[5].Data = dummyValue;
        pxShaderVariableList[5].DataType = PXShaderVariableTypePXF32Vector4;

        PXOpenGLShaderVariableSet(pxOpenGL, pxShaderProgram, pxShaderVariableList, 6);

#if 0
        PXConsoleGoToXY(0, 0);
        PXConsoleWrite(7, "Model\n");
        PXConsoleWriteTablePXF32(modifiedModelMatrix.Data, 4 * 4, 4);
        PXConsoleWrite(6, "View\n");
        PXConsoleWriteTablePXF32(modifiedViewMatrix.Data, 4 * 4, 4);
        PXConsoleWrite(12, "Projection\n");
        PXConsoleWriteTablePXF32(matrixProjection->Data, 4 * 4, 4);
#endif
    }
    else // Legacy matrix stuff
    {
        PXOpenGLShaderProgramSelect(pxOpenGL, 0);

        pxOpenGL->Binding.MatrixMode(GL_MODELVIEW);
        pxOpenGL->Binding.LoadMatrixf(pxCamera->MatrixProjection.Data);
        pxOpenGL->Binding.MultMatrixf(modifiedViewMatrix.Data);
        pxOpenGL->Binding.MultMatrixf(modifiedModelMatrix.Data);
        pxOpenGL->Binding.PushMatrix();
    }


    //-----------------------------------------------------
    // Draw
    //-----------------------------------------------------


    pxOpenGL->Binding.DepthMask(GL_TRUE);
    pxOpenGL->Binding.Enable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_ONE, GL_ONE);
    //glCullFace(GL_BACK);

    PXOpenGLDrawOrder(pxOpenGL, PXOpenGLDrawOrderModeClockwise);

    pxOpenGL->Binding.Clear(GL_DEPTH_BUFFER_BIT);

    // glPolygonMode(GL_NONE, GL_FILL);

    pxOpenGL->Binding.Enable(GL_DEPTH_TEST);

    if(!pxModel->RenderBothSides)
    {
        pxOpenGL->Binding.Enable(GL_CULL_FACE);
    }
    else
    {
        pxOpenGL->Binding.Disable(GL_CULL_FACE);
    }

    pxOpenGL->Binding.Enable(GL_BLEND);
    // glBlendFunc(GL_SRC_COLOR, GL_ZERO);
    //glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    pxOpenGL->Binding.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pxOpenGL->Binding.FrontFace(GL_CCW);
    pxOpenGL->Binding.CullFace(GL_BACK);
    // glDepthMask(GL_FALSE);


    pxOpenGL->Binding.PointSize(5);
    pxOpenGL->Binding.LineWidth(4);





    if(pxOpenGL->Binding.ShaderStorageBlockBinding)
    {
      //  pxOpenGL->Binding.BufferBindBase(GL_SHADER_STORAGE_BUFFER, 0, pxMesh->VertexBufferList[1].DataBuffer.Info.Handle.OpenGLID);
     //   pxOpenGL->Binding.BufferBindBase(GL_SHADER_STORAGE_BUFFER, 1, pxMesh->VertexBufferList[2].DataBuffer.Info.Handle.OpenGLID);

        /*
        // Bind SSBO to binding point 0
        pxOpenGL->Binding.ShaderStorageBlockBinding
        (
            pxShaderProgram->Info.Handle.OpenGLID,
            0,
            pxMesh->VertexBufferList[1].DataBuffer.Info.Handle.OpenGLID
        );
        pxOpenGL->Binding.ShaderStorageBlockBinding
        (
            pxShaderProgram->Info.Handle.OpenGLID,
            1,
            pxMesh->VertexBufferList[2].DataBuffer.Info.Handle.OpenGLID
        );*/
    }
    else
    {
        // Activate the first texture unit and bind the normals texture
     //   pxOpenGL->Binding.TextureSlotActive(GL_TEXTURE1);       // Texture unit 0
      //  pxOpenGL->Binding.TextureBind(GL_TEXTURE_1D, pxMesh->VertexBufferList[1].DataBuffer.Info.Handle.OpenGLID); // Bind the normals texture

        // Activate the second texture unit and bind the texture coordinates texture
       // pxOpenGL->Binding.TextureSlotActive(GL_TEXTURE2);       // Texture unit 1
        //pxOpenGL->Binding.TextureBind(GL_TEXTURE_1D, pxMesh->VertexBufferList[2].DataBuffer.Info.Handle.OpenGLID); // Bind the texture coordinates texture


        // Set the uniform sampler locations in the shader
      //  pxOpenGL->Binding.Uniform1i(pxOpenGL->Binding.GetUniformLocation(pxShaderProgram->Info.Handle.OpenGLID, "VertexAttributeNormal"), 1);   // Corresponds to GL_TEXTURE0
       // pxOpenGL->Binding.Uniform1i(pxOpenGL->Binding.GetUniformLocation(pxShaderProgram->Info.Handle.OpenGLID, "VertexAttributeTexCoords"), 2); // Corresponds to GL_TEXTURE1

    }



    const GLsizei drawElementsCount = pxIndexBuffer->DataIndexSizeSegment;// / PXTypeSizeGet(pxIndexBuffer->IndexDataType);
    const GLenum indexDataType = PXOpenGLTypeToID(pxIndexBuffer->IndexDataType);
    const PXInt8U indexTypeSize = PXTypeSizeGet(pxIndexBuffer->IndexDataType);

    const PXBool renderSegmented = pxIndexBuffer->SegmentListAmount > 1;


    unsigned short modeAmount = 0;
    unsigned short modeCache[5];

    if(PXDrawModeIDTriangle & pxIndexBuffer->DrawModeID)
    {
        modeCache[modeAmount] = GL_TRIANGLES;
        ++modeAmount;
    }

    if(PXDrawModeIDLineLoop & pxIndexBuffer->DrawModeID)
    {
        modeCache[modeAmount] = GL_LINE_LOOP;
        ++modeAmount;
    }

    if(PXDrawModeIDPoint & pxIndexBuffer->DrawModeID)
    {
        modeCache[modeAmount] = GL_POINTS;
        ++modeAmount;
    }

    if(renderSegmented)
    {
        PXSize renderOffset = 0;

        for(PXSize i = 0; i < pxIndexBuffer->SegmentListAmount; ++i)
        {
            PXIndexSegment* const pxIndexSegment = &pxIndexBuffer->SegmentList[i];
            PXTexture2D* pxTexture = 0;

            if(pxIndexSegment->Material)
            {
                pxTexture = pxIndexSegment->Material->DiffuseTexture;
            }

            if(pxTexture)
            {
                pxOpenGL->Binding.Enable(GL_TEXTURE_2D);
                pxOpenGL->Binding.TextureSlotActive(GL_TEXTURE0);
                pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, pxTexture->Info.Handle.OpenGLID);
            }
            else
            {
                pxOpenGL->Binding.TextureSlotActive(GL_TEXTURE0);
                pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, PXNull);
                pxOpenGL->Binding.Disable(GL_TEXTURE_2D);
            }


            const unsigned short drawModeFill[] = { GL_FILL, GL_LINE };


            for(size_t f = 0; f < 2; f++)
            {
                pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, drawModeFill[f]);

                if(f == 1)
                {
                    pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, PXNull);
                    pxOpenGL->Binding.Disable(GL_TEXTURE_2D);
                }

                for(PXSize r = 0; r < modeAmount; ++r)
                {
                    if(hasIndexBuffer) // Does this index array exist?
                    {
                        // Render with an index buffer
                        pxOpenGL->Binding.DrawElements(modeCache[r], pxIndexSegment->DataRange * indexTypeSize, indexDataType, renderOffset* indexTypeSize);
                    }
                    else
                    {
                        // Render witho�t an index buffer
                        pxOpenGL->Binding.DrawArrays(modeCache[r], renderOffset, pxIndexSegment->DataRange);
                    }
                }
            }

            renderOffset += pxIndexSegment->DataRange;
        }
    }
    else
    {
        const PXIndexSegment* const pxIndexSegment = &pxIndexBuffer->SegmentPrime;

        PXMaterial* pxMaterial = pxIndexSegment->Material;

        if(pxRenderEntity->MaterialOverride)
        {
            pxMaterial = pxRenderEntity->MaterialOverride;
        }

        if(pxMaterial)
        {
            PXShaderVariable pxShaderVariableList[3];
            PXClearList(PXShaderVariable, pxShaderVariableList, 3);

            PXTextCopyA("Material.Ambient", 17, pxShaderVariableList[0].Name, 64);
            pxShaderVariableList[0].Amount = 1;
            pxShaderVariableList[0].Data = pxMaterial->Ambient;
            pxShaderVariableList[0].DataType = PXShaderVariableTypePXF32Vector4;

            PXTextCopyA("Material.Diffuse", 17, pxShaderVariableList[1].Name, 64);
            pxShaderVariableList[1].Amount = 1;
            pxShaderVariableList[1].Data = pxMaterial->Diffuse;
            pxShaderVariableList[1].DataType = PXShaderVariableTypePXF32Vector4;

            PXTextCopyA("Material.Specular", 18, pxShaderVariableList[2].Name, 64);
            pxShaderVariableList[2].Amount = 1;
            pxShaderVariableList[2].Data = pxMaterial->Specular;
            pxShaderVariableList[2].DataType = PXShaderVariableTypePXF32Vector4;

            // PXTextCopyA("MaterialTexture", 16, pxShaderVariableList[2].Name, 64);
            // pxShaderVariableList[3].Amount = 1;
            //  pxShaderVariableList[3].Data = pxMaterial->Specular;
            // pxShaderVariableList[3].DataType = PXShaderVariableTypeSampler2DF;

            PXOpenGLShaderVariableSet(pxOpenGL, pxShaderProgram, pxShaderVariableList, 3);


            PXTexture2D* pxTexture = pxMaterial->DiffuseTexture;

            if(pxRenderEntity->Texture2DOverride)
            {
                pxTexture = pxRenderEntity->Texture2DOverride;
            }

            if(pxTexture)
            {
                pxOpenGL->Binding.Enable(GL_TEXTURE_2D);
                pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, pxTexture->Info.Handle.OpenGLID);
            }
            else
            {
                pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, PXNull);
                pxOpenGL->Binding.Disable(GL_TEXTURE_2D);
            }
        }
        else
        {
            // Has no material
        }

        if(hasIndexBuffer)
        {
            pxOpenGL->Binding.DrawArrays(GL_TRIANGLES, 0, drawElementsCount);
        }
        else
        {
            pxOpenGL->Binding.DrawElements(GL_TRIANGLES, drawElementsCount, indexBufferTypeID, 0);
        }
    }   
    //-----------------------------------------------------


    pxOpenGL->Binding.PolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //-----------------------------------------------------
    // Demap
    //-----------------------------------------------------
    if(supportVAO)
    {
        pxOpenGL->Binding.VertexArrayBind(0); // Deselect VAO
    }
    else
    {
        if(supportBuffers)
        {
            pxOpenGL->Binding.BufferBind(GL_ELEMENT_ARRAY_BUFFER, 0);
            pxOpenGL->Binding.BufferBind(GL_ARRAY_BUFFER, 0);
        }
        else
        {
            pxOpenGL->Binding.DisableClientState(GL_VERTEX_ARRAY);
            pxOpenGL->Binding.DisableClientState(GL_TEXTURE_COORD_ARRAY);
            pxOpenGL->Binding.DisableClientState(GL_INDEX_ARRAY);
        }
    }

    if(!canUseShader)
    {
        pxOpenGL->Binding.PopMatrix();
    }

    pxOpenGL->Binding.Disable(GL_BLEND);
    pxOpenGL->Binding.Disable(GL_CULL_FACE);
    pxOpenGL->Binding.Disable(GL_DEPTH_TEST);

    pxOpenGL->Binding.TextureSlotActive(GL_TEXTURE0);
    pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, 0);


    if(pxOpenGL->Binding.ShaderStorageBlockBinding)
    {
        pxOpenGL->Binding.BufferBind(GL_SHADER_STORAGE_BUFFER, 0);
    }
    else
    {
        pxOpenGL->Binding.TextureSlotActive(GL_TEXTURE1);
        pxOpenGL->Binding.TextureBind(GL_TEXTURE_1D, 0);
        pxOpenGL->Binding.TextureSlotActive(GL_TEXTURE2);
        pxOpenGL->Binding.TextureBind(GL_TEXTURE_1D, 0);
    }



    return PXActionSuccessful;
}

void PXAPI PXOpenGLTextureParameter(PXOpenGL* const pxOpenGL, const PXGraphicTextureType textureType, const PXOpenGLTextureParameterMode pname, const PXOpenGLTextureParameterValue openGLTextureParameterValue)
{
    const GLint openGLTextureParameterValueID = PXOpenGLTextureParameterValueToID(openGLTextureParameterValue);

    PXOpenGLTextureParameterI(pxOpenGL, textureType, pname, openGLTextureParameterValueID);
}

void PXAPI PXOpenGLTextureParameterI(PXOpenGL* const pxOpenGL, const PXGraphicTextureType textureType, const PXOpenGLTextureParameterMode pname, const int param)
{
    const GLenum textureTypeID = PXOpenGLTextureTypeToID(textureType);
    const GLenum pnameID = PXOpenGLTextureParameterModeToID(pname);

    pxOpenGL->Binding.TextureParameterI(textureTypeID, pnameID, param);
}

void PXAPI PXOpenGLTextureParameterF(PXOpenGL* const pxOpenGL, const PXGraphicTextureType textureType, const PXOpenGLTextureParameterMode pname, const PXF32 param)
{
    const GLenum textureTypeID = PXOpenGLTextureTypeToID(textureType);
    const GLenum pnameID = PXOpenGLTextureParameterModeToID(pname);

    pxOpenGL->Binding.TextureParameterF(textureTypeID, pnameID, param);
}

void PXOpenGLAPI PXOpenGLErrorMessageCallback(const GLenum source, const GLenum type, const GLuint id, const GLenum severity, const GLsizei length, const char* const message, const void* const userParam)
{
    unsigned char openGLspecific = type == GL_DEBUG_TYPE_ERROR;
    const char* sourceText = 0;
    const char* typeText = 0;

    switch(source)
    {
        case GL_DEBUG_SOURCE_API:
            sourceText = "API";
            break;

        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            sourceText = "Window";
            break;

        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            sourceText = "Shader";
            break;

        case GL_DEBUG_SOURCE_THIRD_PARTY:
            sourceText = "3rd Party";
            break;

        case GL_DEBUG_SOURCE_APPLICATION:
            sourceText = "Application";
            break;

        case GL_DEBUG_SOURCE_OTHER:
            sourceText = "Other";
            break;

        default:
            break;
    }

    switch(type)
    {
        case GL_DEBUG_TYPE_ERROR:
            typeText = "Error";
            break;

        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            typeText = "DEPRECATED_BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            typeText = "UNDEFINED_BEHAVIOR";
            break;

        case GL_DEBUG_TYPE_PORTABILITY:
            typeText = "PORTABILITY";
            break;

        case GL_DEBUG_TYPE_PERFORMANCE:
            typeText = "PERFORMANCE";
            break;

        case GL_DEBUG_TYPE_OTHER:
            typeText = "OTHER";
            break;

        case GL_DEBUG_TYPE_MARKER:
            typeText = "MARKER";
            break;

        case GL_DEBUG_TYPE_PUSH_GROUP:
            typeText = "PUSH_GROUP";
            break;

        case GL_DEBUG_TYPE_POP_GROUP:
            typeText = "POP_GROUP";
            break;

        default:
            break;
    }

    PXLoggingType pxLoggingType;

    switch(severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            pxLoggingType = PXLoggingError;
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            pxLoggingType = PXLoggingWarning;
            break;

        case GL_DEBUG_SEVERITY_LOW:
            pxLoggingType = PXLoggingInfo;
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            pxLoggingType = PXLoggingInfo;
            break;

        default:
            pxLoggingType = PXLoggingTypeInvalid;
            break;
    }

#if PXLogEnable
    PXLogPrint
    (
        pxLoggingType,
        PXOpenGLName,
        "Log",
        "(%s.%s)\n%s",
        sourceText,
        typeText,
        message
    );
#endif
}




/*

PXOpenGLID PXOpenGLToRenderMode(const PXGraphicDrawMode renderMode)
{
    switch (renderMode)
    {
    case PXGraphicDrawModePoint:
        return GL_POINTS;

    case PXGraphicDrawModeLine:
        return GL_LINES;

        // case PXGraphicDrawModeLineAdjacency:
          //   return GL_LINES_ADJACENCY;

       //  case PXGraphicDrawModeLineStripAdjacency:
        //     return GL_LINE_STRIP_ADJACENCY;

    case PXGraphicDrawModeLineLoop:
        return GL_LINE_LOOP;

    case PXGraphicDrawModeLineStrip:
        return GL_LINE_STRIP;

    case PXGraphicDrawModeTriangle:
        return GL_TRIANGLES;

        // case PXGraphicDrawModeTriangleAdjacency:
        //     return GL_TRIANGLES_ADJACENCY;

    case PXGraphicDrawModeTriangleFAN:
        return GL_TRIANGLE_FAN;

    case PXGraphicDrawModeTriangleStrip:
        return GL_TRIANGLE_STRIP;

        //   case PXGraphicDrawModeTriangleStripAdjacency:
          //     return GL_TRIANGLE_STRIP_ADJACENCY;

    case PXGraphicDrawModeSquare:
        return GL_QUADS;

        //  case PXGraphicDrawModePatches:
         //     return GL_PATCHES;

    default:
    case PXGraphicDrawModeInvalid:
        return -1;
    };
}

*/



































































/*



/*


GlmfBeginGlsBlock
GlmfCloseMetaFile
GlmfEndGlsBlock
GlmfEndPlayback
GlmfInitPlayback
GlmfPlayGlsRecord

wglChoosePixelFormat
wglCopyContext
wglCreateContext
wglCreateLayerContext
wglDeleteContext
wglDescribeLayerPlane
wglDescribePixelFormat
wglGetCurrentContext
wglGetCurrentDC
wglGetDefaultProcAddress
wglGetLayerPaletteEntries
wglGetPixelFormat
wglGetProcAddress
wglMakeCurrent
wglRealizeLayerPalette
wglSetLayerPaletteEntries
wglSetPixelFormat
wglShareLists
wglSwapBuffers
wglSwapLayerBuffers
wglSwapMultipleBuffers
wglUseFontBitmapsA
wglUseFontBitmapsW
wglUseFontOutlinesA
wglUseFontOutlinesW




// Opengl32.lib

    wglCopyContext
   -- wglCreateContext
    wglCreateLayerContext
   -- wglDeleteContext
    wglDescribeLayerPlane
    wglGetCurrentContext
    wglGetCurrentDC
    wglGetLayerPaletteEntries
    wglGetProcAddress
    wglMakeCurrent
    wglRealizeLayerPalette
    wglSetLayerPaletteEntries
    wglShareLists
    wglSwapLayerBuffers
    wglUseFontBitmaps
    wglUseFontOutlines


*/


/*
windows only


    ChoosePixelFormat
    DescribePixelFormat
    GetEnhMetaFilePixelFormat
    GetPixelFormat
    SetPixelFormat
    SwapBuffers



    GLYPHMETRICSPXF32
    LAYERPLANEDESCRIPTOR
    PIXELFORMATDESCRIPTOR
    POINTPXF32


* /

void PXOpenGLAddFunction(void** loadList, PXSize* currentSize, char* name, void* functionADress)
{
    PXSize index = *currentSize;

    loadList[index] = functionADress;
    loadList[index + 1] = name;

    *currentSize += 2u;
}
/*
PXOpenGLID PXOpenGLToShaderType(const ShaderType shaderType)
{
    switch (shaderType)
    {
    //case ShaderTypeVertex:
    //    return GL_VERTEX_SHADER;

    case   ShaderTypeTessellationControl:
        return -1; // ???

    case   ShaderTypeTessellationEvaluation:
        return -1; // ???

   // case   ShaderTypeGeometry:
  //      return GL_GEOMETRY_SHADER;

  //  case   ShaderTypeFragment:
  //      return GL_FRAGMENT_SHADER;

//    case  ShaderTypeCompute:
 //       return GL_COMPUTE_SHADER;

    case ShaderTypeUnkown:
    default:
        return -1;
    }
}

PXOpenGLID PXOpenGLToImageFormat(const ImageDataFormat imageFormat)
{
    switch (imageFormat)
    {
   // case ImageDataFormatBGR8:
   //     return GL_BGR;

  //  case ImageDataFormatBGRA8:
  //      return GL_BGRA;

    case ImageDataFormatRGB8:
        return GL_RGB;

    case ImageDataFormatRGBA8:
        return GL_RGBA;

    case ImageDataFormatAlphaMask:
    default:
        return -1;
    }
}

PXOpenGLID PXOpenGLToImageType(const PXGraphicTextureType imageType)
{
    switch (imageType)
    {
    case PXGraphicTextureType2D:
        return GL_TEXTURE_2D;

    //case PXGraphicTextureType3D:
   //     return GL_TEXTURE_3D;

    case PXGraphicTextureTypeCubeContainer:
        return GL_TEXTURE_CUBE_MAP;

    case PXGraphicTextureTypeCubeRight:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    case PXGraphicTextureTypeCubeLeft:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;

    case PXGraphicTextureTypeCubeTop:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;

    case PXGraphicTextureTypeCubeDown:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;

    case PXGraphicTextureTypeCubeBack:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;

    case PXGraphicTextureTypeCubeFront:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    default:
        return -1;
    }
}

PXOpenGLID PXOpenGLToImageWrap(const PXGraphicImageWrap imageWrap)
{
    switch (imageWrap)
    {
    case PXGraphicImageWrapNoModification:
        return GL_CLAMP_TO_BORDER;

   // case PXGraphicImageWrapStrechEdges:
    //    return GL_CLAMP_TO_EDGE;

   // case PXGraphicImageWrapStrechEdgesAndMirror:
 //       return GL_MIRROR_CLAMP_TO_EDGE;

  //  caseGraphicImageWrapRepeat:
  //      return GL_REPEAT;

   // case PXGraphicImageWrapRepeatAndMirror:
   //     return GL_MIRRORED_REPEAT;
    default:
        return -1;
    }
}

PXOpenGLID PXOpenGLToImageLayout(const PXGraphicImageLayout layout)
{
    switch (layout)
    {
    case PXGraphicImageLayoutNearest:
        return GL_NEAREST;

    case PXGraphicImageLayoutLinear:
        return GL_LINEAR;

    case PXGraphicImageLayoutMipMapNearestNearest:
        return GL_NEAREST_MIPMAP_NEAREST;

    case PXGraphicImageLayoutMipMapLinearNearest:
        return GL_LINEAR_MIPMAP_NEAREST;

    case PXGraphicImageLayoutMipMapNNearestLinear:
        return GL_NEAREST_MIPMAP_LINEAR;

    case PXGraphicImageLayoutMipMapLinearLinear:
        return GL_LINEAR_MIPMAP_LINEAR;

    default:
        return -1;
    }
}















void PXOpenGLContextSelect(PXOpenGL* const openGL)
{

}

void PXOpenGLContextDeselect()
{
#if OSUnix

#elif OSWindows
    wglMakeCurrent(NULL, NULL);
#endif
}

void PXOpenGLContextRelease(PXOpenGL* const openGL)
{
#if OSUnix

#elif OSWindows
    const BOOL result = wglDeleteContext(openGL->PXOpenGLConext);

    if (result)
    {
        openGL->PXOpenGLConext = 0;
    }

#endif
}

void PXOpenGLContextFlush()
{
    glFlush(); // Flush drawing command buffer to make drawing happen as soon as possible.
}

PXOpenGLShaderPXProgramID PXOpenGLShaderPXProgramCreate(PXOpenGL* const openGL)
{
    return (*openGL->PXOpenGLShaderPXProgramCreateCallBack)();
}

void PXOpenGLShaderPXProgramUse(PXOpenGL* const openGL, const PXOpenGLShaderPXProgramID shaderPXProgramID)
{
    (openGL->PXOpenGLShaderPXProgramUseCallBack)(shaderPXProgramID);
}

void PXOpenGLShaderSource(PXOpenGL* const openGL, unsigned int shader, int count, const char** string, const int* length)
{
    (openGL->PXOpenGLShaderSourceCallBack)(shader, count, string, length);
}

void PXOpenGLShaderCompile(PXOpenGL* const openGL, const PXOpenGLShaderID shaderID)
{
    (openGL->PXOpenGLCompileShaderCallBack)(shaderID);
}

void PXOpenGLShaderGetiv(PXOpenGL* const openGL, const PXOpenGLShaderID shaderID, GLenum pname, GLint* params)
{
    (openGL->PXOpenGLShaderGetivCallBack)(shaderID, pname, params);
}

void PXOpenGLShaderLogInfoGet(PXOpenGL* const openGL, const PXOpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog)
{
    (openGL->PXOpenGLShaderLogInfoGetCallBack)(shaderID, maxLength, length, infoLog);
}

void PXOpenGLShaderDelete(PXOpenGL* const openGL, const PXOpenGLShaderID shaderID)
{
    (openGL->PXOpenGLShaderDeleteCallBack)(shaderID);
}

void PXOpenGLVertexAttributeArrayDefine(PXOpenGL* const openGL, const PXSize sizeOfElement, const PXSize listSize, const unsigned int* list)
{
    PXSize offset = 0;
    PXSize wholeBlockSize = 0;
    PXSize currentBlockSize = 0;

    for (PXSize vertexAtributeIndex = 0; vertexAtributeIndex < listSize; vertexAtributeIndex++)
    {
        const unsigned int vertexSize = list[vertexAtributeIndex];

        wholeBlockSize += vertexSize;
    }

    wholeBlockSize *= sizeOfElement;

    for (PXSize vertexAtributeIndex = 0; vertexAtributeIndex < listSize; vertexAtributeIndex++)
    {
        const unsigned int  vertexSize = list[vertexAtributeIndex];

        //glEnableVertexAttribArray(vertexAtributeIndex);
        //glVertexAttribPointer(vertexAtributeIndex, vertexSize, GL_FLOAT, GL_FALSE, wholeBlockSize, (void*)offset);

        currentBlockSize += vertexSize;
        offset = sizeOfElement * currentBlockSize;
    }
}

void VertexArrayDefine(unsigned int* vertexArrayID)
{
    //glGenVertexArrays(1, vertexArrayID);
}

void VertexDataDefine(unsigned int* vertexID, int size, void* data)
{
    //glGenBuffers(1, vertexID); // Get BufferID
    //glBindBuffer(GL_ARRAY_BUFFER, *vertexID); // Select Buffer
    //glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void IndexDataDefine(unsigned int* indexID, int size, void* data)
{
    //glGenBuffers(1, indexID);
   // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexID);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void VertexArrayUpdate(int vertexArrayID, int size, void* data)
{
   // glBindVertexArray(vertexArrayID);
   // glBindBuffer(GL_ARRAY_BUFFER, vertexArrayID);
    //glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)size, data);
}

PXOpenGLID BF::PXOpenGL::ShaderPXProgramCreate()
{
    return PXOpenGLShaderPXProgramCreate();
}

void BF::PXOpenGL::DepthMaskEnable(bool enable)
{
    if (enable)
    {
        glDepthMask(GL_TRUE);
    }
    else
    {
        glDepthMask(GL_FALSE);
    }
}

void BF::PXOpenGL::DrawOrder(bool clockwise)
{
    if (clockwise)
    {
        glFrontFace(GL_CW);
    }
    else
    {
        glFrontFace(GL_CCW);
    }
}

void BF::PXOpenGL::RenderBothSides(bool renderBothSides)
{
    if (renderBothSides)
    {
        glDisable(GL_CULL_FACE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
    }
}

char BF::PXOpenGL::UseShaderPXProgram(int shaderPXProgramID)
{
    const bool valid = shaderPXProgramID != -1;

    if(!valid)
    {
        return false;
    }

    glUsePXProgram(shaderPXProgramID);

    return true;
}

void BF::PXOpenGL::VertexArrayBind(int vertexArrayID)
{

    (vertexArrayID != -1);

    glBindVertexArray(vertexArrayID);
}

void BF::PXOpenGL::VertexBufferBind(int vertexBufferID, int indexBuffer)
{
    assert(vertexBufferID != -1);
    assert(indexBuffer != -1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

int BF::PXOpenGL::TextureMaxSlots()
{
    int value = -1;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&value);

    return value;
}

int BF::PXOpenGL::TextureMaxLoaded()
{
    int value = -1;

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint*)&value);

    return value;
}

const char* BF::PXOpenGL::GLSLVersionPrimary()
{
    return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void BF::PXOpenGL::GLSLVersionsSupported(const char*** shaderList, int shaderListSize)
{
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &shaderListSize);

    (*shaderList) = Memory::Allocate<const char*>(shaderListSize);

    for (PXSize i = 0; i < shaderListSize; i++)
    {
        const char* shaderVersion = (const char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);

        (*shaderList)[i] = shaderVersion;
    }
}

unsigned int BF::PXOpenGL::ShaderCompile(unsigned int type, char* shaderString)
{
    unsigned int id = glCreateShader(type);

    glShaderSource(id, 1, &shaderString, nullptr);
    glCompileShader(id);

    // Error handling
    {
        int result;

        glGetShaderiv(id, GL_COMPILE_STATUS, &result);

        if (result == GL_FALSE)
        {
            int lengh = 0;

            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lengh);
            char* message = Memory::Allocate<char>(lengh);

            glGetShaderInfoLog(id, lengh, &lengh, message);

            printf
            (
                "================================================================================\n"
                "+------------------------------------------------------------------------------+\n"
                "| GLSL - PXOpenGL Shader - compile error log                                     |\n"
                "+------------------------------------------------------------------------------+\n"
                "%s"
                "+------------------------------------------------------------------------------+\n"
                "================================================================================\n",
                message
            );

            MemoryRelease(message, lengh);

            glDeleteShader(id);

            return -1;
        }
    }

    return id;
}

void BF::PXOpenGL::VertexAttributeArrayDefine(const unsigned int sizeOfElement, const unsigned int listSize, const unsigned int* list)
{
    int offset = 0;
    int wholeBlockSize = 0;
    int currentBlockSize = 0;

    for (unsigned int vertexAtributeIndex = 0; vertexAtributeIndex < listSize; vertexAtributeIndex++)
    {
        int vertexSize = list[vertexAtributeIndex];

        wholeBlockSize += vertexSize;
    }

    wholeBlockSize *= sizeOfElement;

    for (unsigned int vertexAtributeIndex = 0; vertexAtributeIndex < listSize; vertexAtributeIndex++)
    {
        int vertexSize = list[vertexAtributeIndex];

        glEnableVertexAttribArray(vertexAtributeIndex);
        glVertexAttribPointer(vertexAtributeIndex, vertexSize, GL_FLOAT, GL_FALSE, wholeBlockSize, (void*)offset);

        currentBlockSize += vertexSize;
        offset = sizeOfElement * currentBlockSize;
    }
}

void BF::PXOpenGL::VertexArrayDefine(unsigned int* vertexArrayID)
{
    glGenVertexArrays(1, vertexArrayID);
}

void BF::PXOpenGL::VertexDataDefine(unsigned int* vertexID, int size, void* data)
{
    glGenBuffers(1, vertexID); // Get BufferID
    glBindBuffer(GL_ARRAY_BUFFER, *vertexID); // Select Buffer
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void BF::PXOpenGL::IndexDataDefine(unsigned int* indexID, int size, void* data)
{
    glGenBuffers(1, indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

int BF::PXOpenGL::ShaderGetUniformLocationID(int shaderID, const char* UniformName)
{
    const bool valid = (shaderID != -1) && (UniformName != nullptr);

    if(!valid)
    {
        return -1;
    }

    {
        const int uniformLocationID = glGetUniformLocation(shaderID, UniformName);

        return uniformLocationID;
    }
}

void BF::PXOpenGL::ShaderSetUniformMatrix4x4(const int matrixUniformID, const PXF32* matrix)
{
    const bool valid = matrixUniformID != -1;

    if (!valid)
    {
        return;
    }

    glUniformMatrix4fv(matrixUniformID, 1, GL_FALSE, matrix);
}

void BF::PXOpenGL::ShaderSetUniformVector3(int vector3UniformID, PXF32 x, PXF32 y, PXF32 z)
{
    if (vector3UniformID != -1)
    {
        glUniform3f(vector3UniformID, x, y, z);
    }
}

void BF::PXOpenGL::ShaderSetUniformVector4(int vector3UniformID, PXF32 x, PXF32 y, PXF32 z, PXF32 w)
{
    if (vector3UniformID != -1)
    {
        glUniform4f(vector3UniformID, x, y, z, w);
    }
}

#endif


*/

PXActionResult PXAPI PXOpenGLShaderProgramCreate(PXOpenGL* const pxOpenGL, PXShaderProgram* const pxShaderProgram, PXShader* const shaderList, const PXSize amount)
{
    if(!pxOpenGL->Binding.ShaderProgramCreate)
    {
        //PXResourceIDMarkAsUnused(&pxShaderProgram->Info);
        return PXActionRefusedNotSupportedByLibrary;
    }

    // Check if we have data to even compile shader
    {
        for(PXSize i = 0; i < amount; ++i)
        {
            const PXShader* const shader = &shaderList[i];

            const PXBool validCall = shader->ShaderFile->Data && (shader->ShaderFile->DataUsed > 0);

            if(!validCall)
            {
#if PXLogEnable
                const char* shaderTypeName = PXGraphicShaderTypeToString(shader->Type);

                PXLogPrint
                (
                    PXLoggingError,
                    PXOpenGLName,
                    "Shader-Error",
                    "No data attached!\n"
                    "%25s : [PX:%4i] %i\n"
                    "%25s : [PX:%4i] %i (%s) \n"
                    "%25s : %p, Size:%i",
                    "ProgramID OpenGL", pxShaderProgram->Info.ID, pxShaderProgram->Info.Handle.OpenGLID,
                    "ShaderID OpenGL", shader->Info.ID,shader->Info.Handle.OpenGLID, shaderTypeName,
                    "Shader Data", shader->ShaderFile->Data, shader->ShaderFile->DataUsed
                );
#endif

                return PXActionRefusedArgumentInvalid;
            }
        }
    }


    // Create shader program
    {
        pxShaderProgram->Info.Handle.OpenGLID = pxOpenGL->Binding.ShaderProgramCreate(); // Generate an opengl resource

        const PXBool shaderProgrammCreateSuccessful = -1 != pxShaderProgram->Info.Handle.OpenGLID;

        if(!shaderProgrammCreateSuccessful)
        {
            const PXActionResult createResult = PXOpenGLErrorCurrent(pxOpenGL, shaderProgrammCreateSuccessful);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingError,
                PXOpenGLName,
                "Shader-Create",
                "Program creation failed"
            );
#endif

            return createResult;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Shader-Create",
            "Program created <%i>",
            pxShaderProgram->Info.Handle.OpenGLID
        );
#endif
    }

    // Create and compile shaders
    {
        unsigned int sucessfulCounter = 0;
        PXBool isValidShader = 1;
        PXBool compiledSuccessFully = 0;

        for(PXSize i = 0; i < amount; ++i)
        {
            PXShader* const shader = &shaderList[i];

            const char* shaderTypeName = PXGraphicShaderTypeToString(shader->Type);
            const PXInt32U shaderTypeID = PXOpenGLShaderTypeToID(shader->Type);
            const char* const shaderData = (char*)shader->ShaderFile->Data;
            PXInt32S shaderLength = shader->ShaderFile->DataUsed;

            shader->Info.Handle.OpenGLID = pxOpenGL->Binding.ShaderCreate(shaderTypeID); // Create shader

            // Sanity check?
            GLboolean iss =  pxOpenGL->Binding.IsShader(shader->Info.Handle.OpenGLID);


            pxOpenGL->Binding.ShaderSource(shader->Info.Handle.OpenGLID, 1u, &shaderData, &shaderLength); // Upload data

            pxOpenGL->Binding.ShaderCompile(shader->Info.Handle.OpenGLID);

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                "Shader-Compile",
                "%s <%i>",
                shaderTypeName,
                shader->Info.Handle.OpenGLID
            );
#endif

            {
                GLint isCompiled = 0;

                pxOpenGL->Binding.ShaderGetiv(shader->Info.Handle.OpenGLID, GL_COMPILE_STATUS, &isCompiled);

                compiledSuccessFully = isCompiled;
            }

            if(!compiledSuccessFully)
            {
                isValidShader = 0;

                PXInt32S shaderErrorLengthMaximal = 0;
                PXInt32S shaderErrorLengthCurrent = 0;


                pxOpenGL->Binding.ShaderGetiv(shader->Info.Handle.OpenGLID, GL_INFO_LOG_LENGTH, &shaderErrorLengthMaximal);

                if(shaderErrorLengthMaximal == 0)
                {
                    // No message

#if PXLogEnable
                    PXLogPrint
                    (
                        PXLoggingError,
                        PXOpenGLName,
                        "Shader-Compile",
                        "GLSL - %s - Error - No message can be loaded"
                    );
#endif

                    break;
                }


                char* const shaderErrorLengthData = PXMemoryHeapCallocT(char, shaderErrorLengthMaximal);

                // API* PXOpenGLShaderLogInfoGetFunction)(GLuint shader, GLsizei maxLength, GLsizei* length, char* infoLog);

                pxOpenGL->Binding.ShaderLogInfoGet(shader->Info.Handle.OpenGLID, shaderErrorLengthMaximal, &shaderErrorLengthCurrent, shaderErrorLengthData);

                shaderErrorLengthData[shaderErrorLengthCurrent - 1] = '\0';

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXOpenGLName,
                    "Shader-Compile",
                    "GLSL - %s - Error\n%s",
                    shaderTypeName,
                    shaderErrorLengthData
                );
#endif

                PXMemoryHeapFree(PXNull, shaderErrorLengthData);

                break;
            }

            pxOpenGL->Binding.ShaderAttach(pxShaderProgram->Info.Handle.OpenGLID, shader->Info.Handle.OpenGLID);

            // This shader is now valid
            shader->Info.Behaviour |= PXResourceInfoExist;

            // Are all shaders valid?
            pxShaderProgram->Info.Behaviour |= PXResourceInfoExist * (sucessfulCounter == i);
        }
    }


    if(pxShaderProgram->Info.Behaviour & PXResourceInfoExist)
    {

        // Link shaders together
        {
            pxOpenGL->Binding.ShaderProgramLink(pxShaderProgram->Info.Handle.OpenGLID);

            GLint linkStatus = 0;

            pxOpenGL->Binding.ShaderProgramGetiv(pxShaderProgram->Info.Handle.OpenGLID, GL_LINK_STATUS, &linkStatus);

            if(GL_TRUE != linkStatus)
            {
                PXInt32S shaderErrorLengthMaximal = 0;
                PXInt32S shaderErrorLengthCurrent = 0;

                pxOpenGL->Binding.ShaderProgramGetiv(pxShaderProgram->Info.Handle.OpenGLID, GL_INFO_LOG_LENGTH, &shaderErrorLengthMaximal);

                char* shaderErrorLengthData = PXMemoryHeapCallocT(char, shaderErrorLengthMaximal);

                // API* PXOpenGLShaderLogInfoGetFunction)(GLuint shader, GLsizei maxLength, GLsizei* length, char* infoLog);

                pxOpenGL->Binding.ProgramInfoLogGet(pxShaderProgram->Info.Handle.OpenGLID, shaderErrorLengthMaximal, &shaderErrorLengthCurrent, shaderErrorLengthData);

                shaderErrorLengthData[shaderErrorLengthCurrent - 1] = '\0';

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXOpenGLName,
                    "Shader-Link",
                    "GLSL - Error\n%s",
                    shaderErrorLengthData
                );
#endif

                PXMemoryHeapFree(PXNull, shaderErrorLengthData);

                pxShaderProgram->Info.Behaviour &= ~PXResourceInfoExist; // Invalidate current shader
            }
        }


        pxOpenGL->Binding.ShaderProgramValidate(pxShaderProgram->Info.Handle.OpenGLID);
    }

    // We used the Shaders above to compile, these elements are not used anymore.
    for(PXSize i = 0; i < amount; ++i)
    {
        PXShader* const shader = &shaderList[i];
        const PXBool isLoaded = shader->Info.Handle.OpenGLID != -1;

        if(isLoaded)
        {
            pxOpenGL->Binding.ShaderDelete(shader->Info.Handle.OpenGLID);
        }
    }

    if(!(PXResourceInfoExist & pxShaderProgram->Info.Behaviour))
    {
        PXOpenGLShaderProgramDelete(pxOpenGL, pxShaderProgram);

        return PXActionFailedResourceCompile;
    }

    // Get list of all attributes and uniforms
    {
        GLint numberOfUniforms = 0;
        GLint numberOfAttributes = 0;

        pxOpenGL->Binding.ShaderProgramGetiv(pxShaderProgram->Info.Handle.OpenGLID, GL_ACTIVE_ATTRIBUTES, &numberOfAttributes);
        pxOpenGL->Binding.ShaderProgramGetiv(pxShaderProgram->Info.Handle.OpenGLID, GL_ACTIVE_UNIFORMS, &numberOfUniforms);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Shader-GLSL",
            "Active Uniforms: %i Attributes: %i",
            numberOfUniforms,
            numberOfAttributes
        );
#endif

        // Create space
        pxShaderProgram->VariableListAmount = numberOfUniforms + numberOfAttributes;
        pxShaderProgram->VariableListData = PXMemoryHeapCallocT(PXShaderVariable, pxShaderProgram->VariableListAmount);

        PXSize currentIndex = 0;

        for(GLuint i = 0; i < numberOfAttributes; i++)
        {
            PXShaderVariable* const pxShaderVariable = &pxShaderProgram->VariableListData[currentIndex];

            GLsizei writtenNameSize = 0;
            GLint typeSize = 0;
            GLenum type = 0;

            pxOpenGL->Binding.ActiveAttributeGet
            (
                pxShaderProgram->Info.Handle.OpenGLID,
                (GLuint)i,
                PXShaderVariableNameSize,
                &writtenNameSize,
                &typeSize,
                &type,
                pxShaderVariable->Name
            );

            pxShaderVariable->Locallity = PXShaderVariableLocallityVertexOnly;
            pxShaderVariable->DataType = PXOpenGLShaderVariableTypeFromID(type);
            pxShaderVariable->RegisterIndex = i;
            pxShaderVariable->DataTypeSize = typeSize;
            pxShaderVariable->NameSize = writtenNameSize;

            ++currentIndex;
        }

        for(GLuint i = 0; i < numberOfUniforms; ++i)
        {
            PXShaderVariable* const pxShaderVariable = &pxShaderProgram->VariableListData[currentIndex];

            GLsizei writtenNameSize = 0;
            GLint typeSize = 0;
            GLenum type = 0;

            pxOpenGL->Binding.ActiveUniformGet
            (
                pxShaderProgram->Info.Handle.OpenGLID,
                i,
                PXShaderVariableNameSize,
                &writtenNameSize,
                &typeSize,
                &type,
                pxShaderVariable->Name
            );

            pxShaderVariable->Locallity = PXShaderVariableLocallityGlobal;
            pxShaderVariable->DataType = PXOpenGLShaderVariableTypeFromID(type);
            pxShaderVariable->RegisterIndex = i;
            pxShaderVariable->DataTypeSize = typeSize;
            pxShaderVariable->NameSize = writtenNameSize;

            ++currentIndex;
        }
    }

#if PXLogEnable
    for(PXSize i = 0; i < pxShaderProgram->VariableListAmount; i++)
    {
        PXShaderVariable* const pxShaderVariable = &pxShaderProgram->VariableListData[i];

        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Shader-GLSL",
            "ID:%i %s",
            pxShaderVariable->RegisterIndex,
            pxShaderVariable->Name
        );
    }
#endif


    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLShaderProgramSelect(PXOpenGL* const pxOpenGL, PXShaderProgram* const pxShaderProgram)
{
    if(!pxOpenGL->Binding.ShaderProgramUse)
    {
        return PXActionRefusedNotSupportedByLibrary;
    }

    if(!pxShaderProgram)
    {
        pxOpenGL->Binding.ShaderProgramUse(0); // unbind shader
        return PXActionSuccessful;
    }

    // if (PXResourceIDIsUnused(&pxShaderProgram->Info))
    //     return PXActionRefusedObjectNotReady;

#if PXLogEnable && 0
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Shaderprogram",
        "Select <%i>",
        pxShaderProgram->ResourceID.OpenGLID
    );
#endif

    // TODO: Catch if id is -1 or just invalid

    pxOpenGL->Binding.ShaderProgramUse(pxShaderProgram->Info.Handle.OpenGLID);

    const PXActionResult createResult = PXOpenGLErrorCurrent(pxOpenGL, 0);

    if(PXActionSuccessful != createResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOpenGLName,
            "Shaderprogram",
            "Select failed <%i>",
            pxShaderProgram->Info.Handle.OpenGLID
        );
#endif
    }

    return createResult;
}

PXActionResult PXAPI PXOpenGLShaderProgramDelete(PXOpenGL* const pxOpenGL, PXShaderProgram* const pxShaderProgram)
{
    pxOpenGL->Binding.ShaderProgramDelete(pxShaderProgram->Info.Handle.OpenGLID);

    const PXActionResult createResult = PXOpenGLErrorCurrent(pxOpenGL, 0);

    if(PXActionSuccessful == createResult)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            "Shader-Delete",
            "ID:%i OpenGLID:%i",
            pxShaderProgram->Info.ID,
            pxShaderProgram->Info.Handle.OpenGLID
        );
#endif

        pxShaderProgram->Info.Handle.OpenGLID = -1;
    }

    return createResult;
}

PXInt32U PXAPI PXOpenGLShaderTypeToID(const PXGraphicShaderType pxGraphicShaderType)
{
    switch(pxGraphicShaderType)
    {
        case PXShaderTypeVertex:
            return GL_VERTEX_SHADER;

        case PXShaderTypePixel:
            return GL_FRAGMENT_SHADER;

        case PXShaderTypeTessellationControl:
            return 0;// GL_TESS_CONTROL_SHADER;

        case PXShaderTypeTessellationEvaluation:
            return 0;// GL_TESS_EVALUATION_SHADER;

        case PXShaderTypeGeometry:
            return GL_GEOMETRY_SHADER;

        case PXShaderTypeCompute:
            return 0;//GL_COMPUTE_SHADER;

        default:
        case PXShaderTypeInvalid:
            return -1;
    }
}

void PXAPI PXOpenGLDrawArrays(const PXOpenGL* const pxOpenGL, const PXGraphicDrawMode renderMode, const PXSize offset, const PXSize amount)
{
    const GLenum renderModeID = PXOpenGLRenderModeToID(renderMode);

    pxOpenGL->Binding.DrawArrays(renderModeID, offset, amount);
}

void PXAPI PXOpenGLDrawElements(const PXOpenGL* const pxOpenGL, const PXGraphicDrawMode renderMode, const PXSize amount, const PXInt32U pxDataType, const void* const indexList)
{
    const GLenum renderModeID = PXOpenGLRenderModeToID(renderMode);
    const GLenum openGLDataTypeID = PXOpenGLTypeToID(pxDataType);

    pxOpenGL->Binding.DrawElements(renderModeID, amount, openGLDataTypeID, indexList);
}

void PXAPI PXOpenGLDrawArraysInstanced(const PXOpenGL* const pxOpenGL, const PXGraphicDrawMode renderMode, const PXSize startOffset, const PXSize amount, const PXSize instanceAmount)
{
    const PXBool isSupported = pxOpenGL->Binding.DrawArraysInstanced != PXNull;
    const GLenum renderModeID = PXOpenGLRenderModeToID(renderMode);

    if(isSupported)
    {
        pxOpenGL->Binding.DrawArraysInstanced(renderModeID, startOffset, amount, instanceAmount);
    }
    else
    {
        for(PXSize i = 0; i < instanceAmount; ++i)
        {
            pxOpenGL->Binding.DrawArrays(renderModeID, startOffset, amount);
        }
    }
}

PXActionResult PXAPI PXOpenGLTextureAction(PXOpenGL* const pxOpenGL, PXGraphicTexturInfo* const pxGraphicTexturInfo)
{
    switch(pxGraphicTexturInfo->Action)
    {
        case PXResourceActionCreate:
        {
            // Batch create textures
            PXInt32U* const openGLTextureIDListData = PXMemoryHeapCallocT(PXInt32U, pxGraphicTexturInfo->Amount);

            pxOpenGL->Binding.TextureCreate(pxGraphicTexturInfo->Amount, openGLTextureIDListData);

            const PXActionResult createResult = PXOpenGLErrorCurrent(pxOpenGL, 0);

            if(createResult != PXActionSuccessful)
            {
#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingError,
                    PXOpenGLName,
                    "Texture2D",
                    "Batch <%i> creation failed",
                    pxGraphicTexturInfo->Amount
                );
#endif

                return createResult;
            }

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                "Texture2D",
                "Batch <%i> created",
                pxGraphicTexturInfo->Amount
            );
#endif

            for(PXSize textureIndex = 0; textureIndex < pxGraphicTexturInfo->Amount; textureIndex++)
            {
                const PXInt32U textureID = openGLTextureIDListData[textureIndex];
                void* textureAdress = pxGraphicTexturInfo->TextureReference[textureIndex];

                assert(textureAdress);

                switch(pxGraphicTexturInfo->Type)
                {
                    case PXGraphicTextureType2D:
                    {
                        PXTexture2D* const pxTexture2D = (PXTexture2D*)textureAdress;
                        pxTexture2D->Info.Handle.OpenGLID = textureID;

                        // Bind resource
                        {
                            pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, pxTexture2D->Info.Handle.OpenGLID);

                            const PXActionResult createResult = PXOpenGLErrorCurrent(pxOpenGL, 0);

                            if(PXActionSuccessful != createResult)
                            {
                                return createResult;
                            }
                        }

                        // Texture Style setup
                        {
                            const GLint textureWrapWidth = PXOpenGLGraphicImageWrapToID(pxTexture2D->WrapWidth);
                            const GLint textureWrapHeight = PXOpenGLGraphicImageWrapToID(pxTexture2D->WrapHeight);
                            const GLint textueFilterNear = PXOpenGLGraphicImageLayoutToID(pxTexture2D->LayoutNear);
                            const GLint textueFilterFar = PXOpenGLGraphicImageLayoutToID(pxTexture2D->LayoutFar);

                            pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapWidth);
                            pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapHeight);
                            pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textueFilterNear); // Remember! This stuff is required for some reason, its not optional!
                            pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textueFilterFar); // if not done, textures might be black.
                            //glTexParameteri(openGLTextureTypeID, GL_GENERATE_MIPMAP, GL_FALSE);
                        }

                        //glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

                        // Image upload
                        {
                            PXImage* pxImage = pxTexture2D->Image;

                            if(!pxImage)
                            {
                                return PXActionSuccessful; // No image
                            }

                            if(!pxImage->PixelData)
                            {
                                return PXActionSuccessful; // No image data
                            }

                            // image data upload
                            PXOpenGLTexture2DDataWrite(pxOpenGL, pxTexture2D);
                        }

                        pxOpenGL->Binding.TextureBind(GL_TEXTURE_2D, 0);

                        break;
                    }
                    case PXGraphicTextureType3D:
                    {
                        PXTexture3D* const pxTexture3D = (PXTexture3D*)textureAdress;
                        pxTexture3D->Info.Handle.OpenGLID = textureID;

                        pxOpenGL->Binding.TextureBind(GL_TEXTURE_3D, textureID);

                        // Do stuff

                        pxOpenGL->Binding.TextureBind(GL_TEXTURE_3D, 0);

                        break;
                    }
                    case PXGraphicTextureTypeCubeContainer:
                    {
                        PXTextureCube* const pxTextureCube = (PXTextureCube*)textureAdress;
                        pxTextureCube->Info.Handle.OpenGLID = textureID;

                        pxOpenGL->Binding.TextureBind(GL_TEXTURE_CUBE_MAP, textureID);

                        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
                        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 0);

                        pxOpenGL->Binding.Enable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

                        const PXInt16U openGLTextureTypeList[6] =
                        {
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                            GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
                        };

                        PXImage* imageList[6] =
                        {
                            pxTextureCube->ImageA,
                            pxTextureCube->ImageB,
                            pxTextureCube->ImageC,
                            pxTextureCube->ImageD,
                            pxTextureCube->ImageE,
                            pxTextureCube->ImageF,
                        };

                        for(PXSize i = 0; i < 6u; ++i)
                        {
                            PXImage* image = imageList[i];
                            const PXInt16U textureTypeID = openGLTextureTypeList[i];
                            const int levelOfDetail = 0;

                            PXTexture2D pxTexture2D;
                            pxTexture2D.Image = image;

                            PXInt32U imageFormat;
                            PXInt32U imageFormatType;
                            PXInt32U internalFormat;
                            PXInt32U internalType;

                            const PXBool successA = PXOpenGLImageFormatToID(image->Format, &imageFormat, &imageFormatType);
                            const PXBool successB = PXOpenGLImageFormatToID(PXColorFormatRGBAF, &internalFormat, &internalType);

                            pxOpenGL->Binding.PixelStorei(GL_UNPACK_ALIGNMENT, 1);

                            pxOpenGL->Binding.TextureData2D
                            (
                                textureTypeID,
                                0, // detail
                                internalFormat,
                                image->Width,
                                image->Height,
                                0,
                                imageFormat,
                                imageFormatType,
                                image->PixelData
                            );
                        }

                        pxOpenGL->Binding.TextureBind(GL_TEXTURE_CUBE_MAP, 0);

                        break;
                    }

                    default:
                        break;
                }
            }

            PXMemoryHeapFree(PXNull, openGLTextureIDListData);

            break;
        }
        case PXResourceActionDelete:
        {
            //pxOpenGL->Binding.TextureDelete(amount, textureIDList);

            break;
        }
        case PXResourceActionUpdate:
        {
            break;
        }
        case PXResourceActionSelect:
        {
            PXInt32U textureType = 0;
            PXInt32U textureID = 0;
            PXInt32U enable = pxGraphicTexturInfo->TextureReference;

            switch(pxGraphicTexturInfo->Type)
            {
                case PXGraphicTextureType2D:
                {
                    PXTexture2D* const pxTexture2D = (PXTexture2D*)pxGraphicTexturInfo->TextureReference;

                    if(pxTexture2D)
                    {
                        textureID = pxTexture2D->Info.Handle.OpenGLID;
                    }

                    textureType = GL_TEXTURE_2D;

                    break;
                }
                case PXGraphicTextureType3D:
                {
                    PXTexture3D* const pxTexture3D = (PXTexture3D*)pxGraphicTexturInfo->TextureReference;

                    if(pxTexture3D)
                    {
                        textureID = pxTexture3D->Info.Handle.OpenGLID;
                    }

                    textureType = GL_TEXTURE_3D;

                    break;
                }
                case PXGraphicTextureTypeCubeContainer:
                {
                    PXTextureCube* const pxTextureCube = (PXTextureCube*)pxGraphicTexturInfo->TextureReference;

                    if(pxTextureCube)
                    {
                        textureID = pxTextureCube->Info.Handle.OpenGLID;
                    }

                    textureType = GL_TEXTURE_CUBE_MAP;

                    break;
                }
                default:
                    break;
            }

            if(enable)
            {
                pxOpenGL->Binding.Enable(textureType);
                pxOpenGL->Flags |= PXOpenGLStateIsTexture2DEnabled;
                pxOpenGL->Binding.TextureBind(textureType, textureID);
            }
            else
            {
                pxOpenGL->Binding.TextureBind(textureType, 0);
                pxOpenGL->Flags &= ~PXOpenGLStateIsTexture2DEnabled;
                pxOpenGL->Binding.Disable(textureType);
            }

            break;
        }
        default:
            break;
    }

    return PXActionSuccessful;
}

void PXAPI PXOpenGLTextureActivate(PXOpenGL* const pxOpenGL, const unsigned int index)
{
    unsigned int indexID = GL_TEXTURE0 + index;

    pxOpenGL->Binding.TextureSlotActive(indexID);
}

void PXAPI PXOpenGLTexture2DDataWrite(PXOpenGL* const pxOpenGL, PXTexture2D* const pxTexture2D)
{
    GLenum imageFormat = 0;
    GLenum imageFormatType = 0;
    GLenum internalFormat = 0;
    GLenum internalType = 0;

    const PXBool successA = PXOpenGLImageFormatToID(pxTexture2D->Image->Format, &imageFormat, &imageFormatType);
    const PXBool successB = PXOpenGLImageFormatToID(PXColorFormatRGBAF, &internalFormat, &internalType);

    pxOpenGL->Binding.PixelStorei(GL_UNPACK_ALIGNMENT, 1); // This is needed to state we have tightly packed image data. If not, the GPU expects padding bytes and will crash.

    PXText pxText;
    PXTextConstructBufferA(&pxText, 32);
    PXTextFormatSize(&pxText, pxTexture2D->Image->PixelDataSize);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Texture2D",
        "%8s  Data upload <%i> (%ix%i)",
        pxText.TextA,
        pxTexture2D->Info.Handle.OpenGLID,
        pxTexture2D->Image->Width,
        pxTexture2D->Image->Height
    );
#endif

    pxOpenGL->Binding.TextureData2D
    (
        GL_TEXTURE_2D,
        0, // detail
        internalFormat,
        pxTexture2D->Image->Width,
        pxTexture2D->Image->Height,
        0,
        imageFormat,
        imageFormatType,
        pxTexture2D->Image->PixelData
    );
}

void PXAPI PXOpenGLSkyboxDraw(PXOpenGL* const pxOpenGL, const PXRenderEntity* const pxRenderEntity)
{
    if(!(pxOpenGL && pxRenderEntity))
    {
        return;
    }

    PXSkyBox* const pxSkyBox = (PXSkyBox*)pxRenderEntity->ObjectReference;

    if(!pxSkyBox)
    {
        return;
    }

    PXCamera* const pxCamera = pxRenderEntity->CameraReference;
    const PXMatrix4x4F* const matrixModel = &pxRenderEntity->MatrixModel;
    PXMatrix4x4F* const matrixView = &pxCamera->MatrixView;
    PXVertexBuffer* const pxVertexBuffer = &pxSkyBox->Model->Mesh.VertexBufferPrime;
    PXIndexBuffer* const pxIndexBuffer = &pxSkyBox->Model->Mesh.IndexBuffer;

    PXModel* const pxModel = pxSkyBox->Model;
    PXTextureCube* const pxTextureCube = pxSkyBox->TextureCube;

    //assert(pxModel);
    //assert(pxTextureCube);

    void* indexBuffer = 0;

    PXMatrix4x4F viewTri;

    pxOpenGL->Binding.Enable(GL_TEXTURE_2D);
    pxOpenGL->Binding.LineWidth(5);
    pxOpenGL->Binding.PointSize(10);


    PXMatrix4x4FCopy(matrixView, &viewTri);
    PXMatrix4x4FResetAxisW(&viewTri); // if removed, you can move out of the skybox

    // PXOpenGLPolygonRenderOrder(pxOpenGL, PXOpenGLPolygonRenderOrderModeCounterClockwise);

    pxOpenGL->Binding.DepthMask(GL_FALSE);
    pxOpenGL->Binding.Enable(GL_BLEND);

    //PXOpenGLPolygonRenderOrder(pxOpenGL, PXOpenGLPolygonRenderOrderModeClockwise);

    // ShaderSetup
    if(pxOpenGL->Binding.ShaderCreate && pxSkyBox->ShaderProgramReference)
    {
        PXOpenGLShaderProgramSelect(pxOpenGL, pxSkyBox->ShaderProgramReference);

        PXShaderVariable pxShaderVariableList[2];
        PXClearList(PXShaderVariable, pxShaderVariableList, 2);

        PXTextCopyA("MatrixView", 11, pxShaderVariableList[0].Name, 64);
        pxShaderVariableList[0].Amount = 1;
        pxShaderVariableList[0].Data = viewTri.Data;
        pxShaderVariableList[0].DataType = PXShaderVariableTypeMatrix4x4;

        PXTextCopyA("MatrixProjection", 16, pxShaderVariableList[1].Name, 64);
        pxShaderVariableList[1].Amount = 1;
        pxShaderVariableList[1].Data = pxCamera->MatrixProjection.Data;
        pxShaderVariableList[1].DataType = PXShaderVariableTypeMatrix4x4;

        PXOpenGLShaderVariableSet(pxOpenGL, pxSkyBox->ShaderProgramReference, pxShaderVariableList, 2);
    }
    else
    {
        PXOpenGLShaderProgramSelect(pxOpenGL, 0);

    //    pxOpenGL->Binding.InterleavedArrays(GL_V3F, pxVertexBuffer->VertexDataSize, pxVertexBuffer->VertexData);

        pxOpenGL->Binding.Color4f(0.5f, 0.2f, 0.2f, 1.0f);

        pxOpenGL->Binding.MatrixMode(GL_MODELVIEW);
        pxOpenGL->Binding.LoadMatrixf(pxCamera->MatrixProjection.Data);
        pxOpenGL->Binding.MultMatrixf(viewTri.Data);
        //glMultMatrixf(pxSkyBox->Model.ModelMatrix.Data);
        pxOpenGL->Binding.PushMatrix();

        indexBuffer = pxIndexBuffer->DataIndexPosition;
    }

    if(pxOpenGL->Binding.VertexArrayBind)
    {
        pxOpenGL->Binding.VertexArrayBind(pxModel->Mesh.Info.Handle.OpenGLID);
    }


    // pxOpenGL->Binding.PXOpenGLBindBufferCallBack(GL_ARRAY_BUFFER, pxSkyBox->Model.VertexBuffer.ResourceID.OpenGLID);
    // pxOpenGL->Binding.PXOpenGLBindBufferCallBack(GL_ELEMENT_ARRAY_BUFFER, pxSkyBox->Model.IndexBuffer.ResourceID.OpenGLID);
    if(pxTextureCube)
    {
        pxOpenGL->Binding.TextureBind(GL_TEXTURE_CUBE_MAP, pxTextureCube->Info.Handle.OpenGLID);
    }



    // glBindTexture(GL_TEXTURE_2D, pxSkyBox->TextureCube.ResourceID.OpenGLID);

    //PXOpenGLBufferBind(pxOpenGL, PXOpenGLBufferElementArray, skybox->Model.IndexBuffer.ResourceID.OpenGLID);
    // PXOpenGLTextureBind(pxOpenGL, PXOpenGLTextureTypeCubeMap, skybox->TextureCube.ResourceID.OpenGLID);

    const GLsizei drawElementsCount = pxIndexBuffer->DataIndexSizeSegment / (pxIndexBuffer->IndexDataType & PXTypeSizeMask);

    pxOpenGL->Binding.BufferBind(GL_ARRAY_BUFFER, pxVertexBuffer->Info.Handle.OpenGLID);
    pxOpenGL->Binding.BufferBind(GL_ELEMENT_ARRAY_BUFFER, pxIndexBuffer->Info.Handle.OpenGLID);

    if(pxIndexBuffer->DrawModeID & PXDrawModeIDPoint || 1)
    {
        pxOpenGL->Binding.DrawArrays(GL_POINTS, 0, drawElementsCount);
    }
    if(pxIndexBuffer->DrawModeID & PXDrawModeIDLineLoop)
    {
        pxOpenGL->Binding.DrawElements(GL_LINE_LOOP, drawElementsCount, GL_UNSIGNED_BYTE, indexBuffer);
    }

    if(pxIndexBuffer->DrawModeID & PXDrawModeIDSquare)
    {
        //pxOpenGL->Binding.DrawElements(GL_QUADS, 0, pxSkyBox->Model.IndexBuffer.IndexDataAmount, GL_UNSIGNED_BYTE, indexBuffer); // Not supported?
    }

    if(pxIndexBuffer->DrawModeID & PXDrawModeIDTriangle)
    {
        pxOpenGL->Binding.DrawElements(GL_TRIANGLES, drawElementsCount, GL_UNSIGNED_BYTE, indexBuffer);
    }




    //glDrawElements(GL_POINTS, 1, GL_UNSIGNED_BYTE, pxSkyBox->Model.IndexBuffer.IndexData);

    // glDrawElements();

    // PXOpenGLDrawElements(pxOpenGL, PXOpenGLRenderLines, 24u, PXTypeInt8U, skybox->Model.IndexBuffer.IndexData);



    // PXOpenGLTextureUnbind(pxOpenGL, PXOpenGLTextureTypeCubeMap);

    //PXOpenGLBufferUnbind(pxOpenGL, PXOpenGLBufferElementArray);


    // PXOpenGLSettingChange(pxOpenGL, PXOpenGLCULL_FACE, PXTrue);

    pxOpenGL->Binding.TextureBind(GL_TEXTURE_CUBE_MAP, 0);
    // pxOpenGL->Binding.PXOpenGLBindBufferCallBack(GL_ELEMENT_ARRAY_BUFFER, 0);
    // pxOpenGL->Binding.PXOpenGLBindBufferCallBack(GL_ARRAY_BUFFER, 0);

    if(pxOpenGL->Binding.VertexArrayBind)
    {
        pxOpenGL->Binding.VertexArrayBind(0);
    }

    pxOpenGL->Binding.DepthMask(GL_TRUE);

    pxOpenGL->Binding.Disable(GL_TEXTURE_2D);

    //PXOpenGLPolygonRenderOrder(pxOpenGL, PXOpenGLPolygonRenderOrderModeCounterClockwise);

}

void PXAPI PXOpenGLFrameBufferCreate(PXOpenGL* const pxOpenGL, const unsigned int amount, unsigned int* const framebufferIDList)
{
    pxOpenGL->Binding.FrameBufferCreate(amount, framebufferIDList);

#if PXLogEnable
    PXLogPrint
    (
        PXLoggingInfo,
        PXOpenGLName,
        "Framebuffer",
        "Created <%i>",
        framebufferIDList[0]
    );
#endif
}

void PXAPI PXOpenGLRenderBufferStorage(PXOpenGL* const pxOpenGL, const PXOpenGLRenderBufferFormat internalformat, const int width, const int height)
{
    const int internalformatID = PXOpenGLRenderBufferFormatToID(internalformat);

    pxOpenGL->Binding.RenderBufferStorage(GL_RENDERBUFFER, internalformatID, width, height);
}

void PXAPI PXOpenGLFrameBufferBind(PXOpenGL* const pxOpenGL, const PXOpenGLFrameBufferMode target, const unsigned int framebufferID)
{
    unsigned int targetID = 0;

    switch(target)
    {
        default:
        case PXOpenGLFrameBufferModeInvalid:
            targetID = -1;
            break;

        case PXOpenGLFrameBufferModeDraw:
            targetID = GL_DRAW_FRAMEBUFFER;
            break;

        case PXOpenGLFrameBufferModeRead:
            targetID = GL_READ_FRAMEBUFFER;
            break;

        case PXOpenGLFrameBufferModeDrawAndRead:
            targetID = GL_FRAMEBUFFER;
            break;
    }

    pxOpenGL->Binding.FrameBufferBind(targetID, framebufferID); // GL_FRAMEBUFFER
}

void PXAPI PXOpenGLFrameBufferDestroy(PXOpenGL* const pxOpenGL, const unsigned int amount, unsigned int* const framebufferIDList)
{
    pxOpenGL->Binding.FrameBufferDelete(amount, framebufferIDList);
}

void PXAPI PXOpenGLRenderBufferCreate(PXOpenGL* const pxOpenGL, GLsizei n, GLuint* renderbuffers)
{
    pxOpenGL->Binding.RenderBufferCreate(n, renderbuffers);
}

void PXAPI PXOpenGLRenderBufferBind(PXOpenGL* const pxOpenGL, const unsigned int renderbuffer)
{
    pxOpenGL->Binding.RenderBufferBind(GL_RENDERBUFFER, renderbuffer);
}

void PXAPI PXOpenGLRenderBufferDelete(PXOpenGL* const pxOpenGL, GLsizei n, GLuint* renderbuffers)
{
    pxOpenGL->Binding.RenderBufferDelete(n, renderbuffers);
}

void PXAPI PXOpenGLFrameBufferLinkTexture2D(PXOpenGL* const pxOpenGL, const PXOpenGLRenderBufferAttachmentPoint attachment, const PXGraphicTextureType textarget, const unsigned int textureID, const int level)
{
    const unsigned int attachmentID = PXOpenGLRenderBufferAttachmentPointToID(attachment);
    const unsigned int textureTypeID = PXOpenGLTextureTypeToID(textarget);

    pxOpenGL->Binding.FrameBufferLinkTexture2D(GL_FRAMEBUFFER, attachmentID, textureTypeID, textureID, level);
}

GLuint PXAPI PXOpenGLFrameBufferLinkRenderBuffer(PXOpenGL* const pxOpenGL, const PXOpenGLRenderBufferAttachmentPoint attachment, const unsigned int renderbuffer)
{
    const unsigned int attachmentID = PXOpenGLRenderBufferAttachmentPointToID(attachment);

    return pxOpenGL->Binding.FrameBufferLinkRenderBuffer(GL_FRAMEBUFFER, attachmentID, GL_RENDERBUFFER, renderbuffer);
}

#define PXOpenGLShaderVariableSetDebug 0

PXActionResult PXAPI PXOpenGLShaderVariableSet(PXOpenGL* const pxOpenGL, const PXShaderProgram* const pxShaderProgram, PXShaderVariable* const pxShaderVariableList, const PXSize amount)
{
    if(!(pxOpenGL && pxShaderProgram && pxShaderVariableList))
    {
        return PXActionRefusedArgumentNull;
    }

    if(amount == 0)
    {
        return PXActionRefusedArgumentInvalid;
    }

    for(PXSize i = 0; i < amount; ++i)
    {
        PXShaderVariable* const pxShaderVariable = &pxShaderVariableList[i];

        const PXBool skip =
            (0 == pxShaderVariable->Amount) ||
            (PXShaderVariableBehaviourFailedFetch & pxShaderVariable->Info.Behaviour);

        if(skip)
        {
            continue;
        }

        // Fetch IDs if not done already
        if(pxShaderVariable->RegisterIndex == 0)
        {
            // Try hyper cached first
            for(PXSize w = 0; w < pxShaderProgram->VariableListAmount; ++w)
            {
                /// ?
            }

            pxShaderVariable->RegisterIndex = pxOpenGL->Binding.GetUniformLocation(pxShaderProgram->Info.Handle.OpenGLID, pxShaderVariable->Name);

            // If no error, exit
            {
                const PXBool fetchSuccessful = pxShaderVariable->RegisterIndex != (PXInt32U)-1;

                if(!fetchSuccessful)
                {
                    const PXActionResult createResult = PXOpenGLErrorCurrent(pxOpenGL, fetchSuccessful);

#if PXLogEnable && PXOpenGLShaderVariableSetDebug
                    PXLogPrint
                    (
                        PXLoggingError,
                        PXOpenGLName,
                        "Shader-Uniform",
                        "Failed name resolve to ID! <%s>, PXID:%i, OpenGLID:%i",
                        pxShaderVariable->Name,
                        pxShaderProgram->Info.ID,
                        pxShaderProgram->Info.Handle.OpenGLID
                    );
#endif

                    pxShaderVariable->Info.Behaviour |= PXShaderVariableBehaviourFailedFetch;

                    continue;
                }
            }
        }

        if(!pxShaderVariable->Data) // If we dont have data, we can't do more.
        {
            //return PXActionSuccessful;
            continue;
        }


#if PXLogEnable && PXOpenGLShaderVariableSetDebug
        char nameBuffer[128];
        PXTextPrintA
        (
            nameBuffer,
            128,
            "%20s : ID:%i, <%s>"
            "%20s : PX-ID:%i, GL-ID:%i",
           // "%20s : PX-ID:%i, GL-ID:%i",
           // "%20s : PX-ID:%i, GL-ID:%i",
            "Uniform", pxShaderVariable->RegisterIndex, pxShaderVariable->Name,
            "Shader-Program", pxShaderProgram->Info.ID, pxShaderProgram->Info.Handle.OpenGLID
           // "Shader-Vertex", pxShaderProgram->Info.ID, pxShaderProgram->Info.Handle.OpenGLID,
            //"Shader-Pixel", pxShaderProgram->Info.ID, pxShaderProgram->Info.Handle.OpenGLID
        );
#endif


        switch(pxShaderVariable->DataType)
        {
            case PXShaderVariableTypeInt32SSingle:
            {
                const int* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform1iv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform1i(pxShaderVariable->RegisterIndex, data[0]);
                }

                break;
            }
            case PXShaderVariableTypeInt32SVector2:
            {
                const int* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform2iv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform2i(pxShaderVariable->RegisterIndex, data[0], data[1]);
                }

                break;
            }
            case PXShaderVariableTypeInt32SVector3:
            {
                const int* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform3iv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform3i(pxShaderVariable->RegisterIndex, data[0], data[1], data[2]);
                }

                break;
            }
            case PXShaderVariableTypeInt32SVector4:
            {
                const int* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform4iv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform4i(pxShaderVariable->RegisterIndex, data[0], data[1], data[2], data[3]);
                }

                break;
            }
            case PXShaderVariableTypePXF32Single:
            {
                const PXF32* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform1fv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform1f(pxShaderVariable->RegisterIndex, data[0]);
                }

                break;
            }
            case PXShaderVariableTypePXF32Vector2:
            {
                const PXF32* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform2fv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform2f(pxShaderVariable->RegisterIndex, data[0], data[1]);
                }

                break;
            }
            case PXShaderVariableTypePXF32Vector3:
            {
                const PXF32* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform3fv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform3f(pxShaderVariable->RegisterIndex, data[0], data[1], data[2]);

#if PXLogEnable && PXOpenGLShaderVariableSetDebug
                    const PXF32* const matrixData = (PXF32*)pxShaderVariable->Data;

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXOpenGLName,
                        "Shader-Uniform",
                        "Upload Vector3 -> %s\n"
                        "| %8.2f | %8.2f | %8.2f |",
                        nameBuffer,
                        matrixData[0],
                        matrixData[1],
                        matrixData[2]
                    );
#endif
                }

                break;
            }
            case PXShaderVariableTypePXF32Vector4:
            {
                const PXF32* const data = pxShaderVariable->Data;

                if(pxShaderVariable->Amount > 1)
                {
                    pxOpenGL->Binding.Uniform4fv
                    (
                        pxShaderVariable->RegisterIndex,
                        pxShaderVariable->Amount,
                        pxShaderVariable->Data
                    );
                }
                else
                {
                    pxOpenGL->Binding.Uniform4f(pxShaderVariable->RegisterIndex, data[0], data[1], data[2], data[3]);

#if PXLogEnable && PXOpenGLShaderVariableSetDebug
                    const PXF32* const matrixData = (PXF32*)pxShaderVariable->Data;

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXOpenGLName,
                        "Shader-Uniform",
                        "Upload Vector4 -> %s\n"
                        "| %8.2f | %8.2f | %8.2f | %8.2f |",
                        nameBuffer,
                        matrixData[0],
                        matrixData[1],
                        matrixData[2],
                        matrixData[3]
                    );
#endif
                }

                break;
            }
            case PXShaderVariableTypeMatrix2x2:
            {
                pxOpenGL->Binding.UniformMatrix2fv
                (
                    pxShaderVariable->RegisterIndex,
                    pxShaderVariable->Amount,
                    PXFalse,
                    pxShaderVariable->Data
                );

                break;
            }
            case PXShaderVariableTypeMatrix3x3:
            {
                pxOpenGL->Binding.UniformMatrix3fv
                (
                    pxShaderVariable->RegisterIndex,
                    pxShaderVariable->Amount,
                    PXFalse,
                    pxShaderVariable->Data
                );

                break;
            }
            case PXShaderVariableTypeMatrix4x4:
            {
                pxOpenGL->Binding.UniformMatrix4fv
                (
                    pxShaderVariable->RegisterIndex,
                    pxShaderVariable->Amount,
                    PXFalse,
                    pxShaderVariable->Data
                );

#if PXLogEnable && PXOpenGLShaderVariableSetDebug
                const PXF32* const matrixData = (PXF32*)pxShaderVariable->Data;

                PXLogPrint
                (
                    PXLoggingInfo,
                    PXOpenGLName,
                    "Shader-Uniform",
                    "Upload Matrix4x4 -> %s\n"
                    "| %8.2f | %8.2f | %8.2f | %8.2f |\n"
                    "| %8.2f | %8.2f | %8.2f | %8.2f |\n"
                    "| %8.2f | %8.2f | %8.2f | %8.2f |\n"
                    "| %8.2f | %8.2f | %8.2f | %8.2f |",
                    nameBuffer,
                    matrixData[0],
                    matrixData[1],
                    matrixData[2],
                    matrixData[3],
                    matrixData[4],
                    matrixData[5],
                    matrixData[6],
                    matrixData[7],
                    matrixData[8],
                    matrixData[9],
                    matrixData[10],
                    matrixData[11],
                    matrixData[12],
                    matrixData[13],
                    matrixData[14],
                    matrixData[15]
                );
#endif


                break;
            }
            default:
                return PXActionRefusedArgumentInvalid;
        }

        // Fetch error
        {
            const PXActionResult uniformResult = PXOpenGLErrorCurrent(pxOpenGL, 0);

            //return uniformResult;
        }
    }

    return PXActionSuccessful;
}

void PXAPI PXOpenGLVertexArrayGenerate(PXOpenGL* const pxOpenGL, const unsigned int amount, unsigned int* const vaoList)
{
    pxOpenGL->Binding.VertexArraysGenerate(amount, vaoList);
}

void PXAPI PXOpenGLVertexArrayBind(PXOpenGL* const pxOpenGL, const unsigned int vaoID)
{
    pxOpenGL->Binding.VertexArrayBind(vaoID);
}

void PXAPI PXOpenGLVertexArrayUnbind(PXOpenGL* const pxOpenGL)
{
    pxOpenGL->Binding.VertexArrayBind(0);
}

void PXAPI PXOpenGLVertexArrayAttributeDefine
(
    PXOpenGL* const pxOpenGL,
    const PXInt32U index,
    const PXInt32U size,
    const PXInt32U datatype,
    const PXBool normalized,
    const PXInt32U stride,
    const PXSize offset
)
{
    const GLenum openGLType = PXOpenGLTypeToID(datatype);

    pxOpenGL->Binding.VertexAttribPointer(index, size, openGLType, normalized, stride, (void*)offset);
}

void PXAPI PXOpenGLVertexAttributeDivisor(PXOpenGL* const pxOpenGL, const PXSize index, const PXSize divisor)
{
    pxOpenGL->Binding.VertexAttribDivisor(index, divisor);
}

void PXAPI PXOpenGLVertexArrayEnable(PXOpenGL* const pxOpenGL, const unsigned int vertexArrayAtributeID)
{
    pxOpenGL->Binding.VertexAttribArrayEnable(vertexArrayAtributeID);
}

void PXAPI PXOpenGLVertexArrayDisable(PXOpenGL* const pxOpenGL, const unsigned int vertexArrayAtributeID)
{
    pxOpenGL->Binding.VertexAttribArrayDisable(vertexArrayAtributeID);
}

void PXAPI PXOpenGLBufferGenerate(PXOpenGL* const pxOpenGL, const unsigned int amount, unsigned int* const bufferIDList)
{
    pxOpenGL->Binding.BufferGenerate(amount, bufferIDList);
}

void PXAPI PXOpenGLBufferBind(PXOpenGL* const pxOpenGL, const PXOpenGLBufferType bufferType, const unsigned int bufferID)
{
    const unsigned int bufferTypeID = PXOpenGLBufferTypeToID(bufferType);

    pxOpenGL->Binding.BufferBind(bufferTypeID, bufferID);
}

void PXAPI PXOpenGLBufferData(PXOpenGL* const pxOpenGL, const PXOpenGLBufferType bufferType, const unsigned int size, const void* const data, const PXOpenGLStoreMode openGLStoreMode)
{
    const unsigned int bufferTypeID = PXOpenGLBufferTypeToID(bufferType);
    const unsigned int openGLStoreModeID = PXOpenGLStoreModeToID(openGLStoreMode);

    pxOpenGL->Binding.BufferData(bufferTypeID, size, data, openGLStoreModeID);
}

void PXAPI PXOpenGLBufferUnbind(PXOpenGL* const pxOpenGL, const PXOpenGLBufferType bufferType)
{
    const unsigned int bufferTypeID = PXOpenGLBufferTypeToID(bufferType);

    pxOpenGL->Binding.BufferBind(bufferTypeID, 0);
}

void PXAPI PXOpenGLTexture2DDataRead(PXOpenGL* const pxOpenGL, PXTexture2D* const pxTexture2D)
{
    GLint viewPort[4];

    pxOpenGL->Binding.GetIntegerv(GL_VIEWPORT, viewPort);

    PXOpenGLTexture2DDataReadFrom(pxOpenGL, pxTexture2D, viewPort[0], viewPort[0], viewPort[2], viewPort[3]);
}

void PXAPI PXOpenGLTexture2DDataReadFrom(PXOpenGL* const pxOpenGL, PXTexture2D* const pxTexture2D, const PXInt32U x, const PXInt32U y, const PXInt32U width, const PXInt32U height)
{
    PXInt32U imageFormatID = 0;
    PXInt32U dataTypeID = 0;

    PXOpenGLImageFormatToID(pxTexture2D->Image->Format, &imageFormatID, &dataTypeID);

    PXImageResize(pxTexture2D->Image, pxTexture2D->Image->Format, width, height);

    pxOpenGL->Binding.ReadPixels(x, y, width, height, imageFormatID, dataTypeID, pxTexture2D->Image->PixelData);
}

PXInt32U PXAPI PXOpenGLTypeToID(const PXInt32U pxDataType)
{
    switch(pxDataType)
    {
        case PXTypeInt08S:
            return GL_BYTE;

        case PXTypeInt08U:
            return GL_UNSIGNED_BYTE;

        case PXTypeInt16SLE:
        case PXTypeInt16SBE:
        case PXTypeInt16S:
            return GL_SHORT;

        case PXTypeInt16ULE:
        case PXTypeInt16UBE:
        case PXTypeInt16U:
            return GL_UNSIGNED_SHORT;

        case PXTypeInt32SLE:
        case PXTypeInt32SBE:
        case PXTypeInt32S:
            return GL_INT;

        case PXTypeInt32ULE:
        case PXTypeInt32UBE:
        case PXTypeInt32U:
            return GL_UNSIGNED_INT;

        case PXTypeF32:
            return GL_FLOAT;

        case PXTypeF64:
            return GL_DOUBLE;

        default:
            return -1;
    }
}

PXInt32U PXAPI PXOpenGLTypeIDFromFormat(const PXVertexBufferFormat pxVertexBufferFormat)
{
    switch(pxVertexBufferFormat)
    {
        case PXVertexBufferFormatP3F16:
        case PXVertexBufferFormatN3F16:
        case PXVertexBufferFormatT2F16:
            return GL_2_BYTES; // GL_HALF_FLOAT

        case PXVertexBufferFormatP3F32:
        case PXVertexBufferFormatN3F32:
        case PXVertexBufferFormatT2F32:
            return GL_FLOAT;

        default:
            return 0;
    }
}

PXInt32U PXAPI PXOpenGLGraphicImageLayoutToID(const PXGraphicImageLayout pxGraphicImageLayout)
{
    switch(pxGraphicImageLayout)
    {
        case PXGraphicImageLayoutNearest:
            return GL_NEAREST;
        case PXGraphicImageLayoutLinear:
            return GL_LINEAR;
        case PXGraphicImageLayoutMipMapNearestNearest:
            return GL_NEAREST_MIPMAP_NEAREST;
        case PXGraphicImageLayoutMipMapLinearNearest:
            return GL_LINEAR_MIPMAP_NEAREST;
        case PXGraphicImageLayoutMipMapNNearestLinear:
            return GL_NEAREST_MIPMAP_LINEAR;
        case PXGraphicImageLayoutMipMapLinearLinear:
            return GL_LINEAR_MIPMAP_LINEAR;

        default:
            return -1;
    }
}

PXInt32U PXAPI PXOpenGLGraphicImageWrapToID(const PXGraphicImageWrap pxGraphicImageWrap)
{
    switch(pxGraphicImageWrap)
    {
        case PXGraphicImageWrapNoModification:
            return GL_CLAMP;
        case PXGraphicImageWrapStrechEdges:
            return GL_CLAMP;
        case PXGraphicImageWrapStrechEdgesAndMirror:
            return GL_CLAMP;
        case PXGraphicImageWrapRepeat:
            return GL_REPEAT;
        case PXGraphicImageWrapRepeatAndMirror:
            return GL_REPEAT;

        default:
            return -1;
    }
}

PXShaderVariableType PXAPI PXOpenGLShaderVariableTypeFromID(const PXInt16U openGLShaderVariableTypeID)
{
    switch(openGLShaderVariableTypeID)
    {
        case GL_FLOAT:
            return PXShaderVariableTypePXF32Single; // PXF32
        case GL_FLOAT_VEC2:
            return PXShaderVariableTypePXF32Vector2; //      vec2
        case GL_FLOAT_VEC3:
            return PXShaderVariableTypePXF32Vector3; //     vec3
        case GL_FLOAT_VEC4:
            return PXShaderVariableTypePXF32Vector4; //     vec4
        case GL_INT:
            return PXShaderVariableTypeInt32SSingle; //     int
        case GL_INT_VEC2:
            return PXShaderVariableTypeInt32SVector2; //     ivec2
        case GL_INT_VEC3:
            return PXShaderVariableTypeInt32SVector3; //     ivec3
        case GL_INT_VEC4:
            return PXShaderVariableTypeInt32SVector4; //     ivec4
        case GL_UNSIGNED_INT:
            return PXShaderVariableTypeInt32USingle; // unsigned int
        case GL_UNSIGNED_INT_VEC2:
            return PXShaderVariableTypeInt32UVector2; // uvec2
        case GL_UNSIGNED_INT_VEC3:
            return PXShaderVariableTypeInt32UVector3; //     uvec3
        case GL_UNSIGNED_INT_VEC4:
            return PXShaderVariableTypeInt32UVector4; //     uvec4
        case GL_BOOL:
            return PXShaderVariableTypeBoolSignle; //     bool
        case GL_BOOL_VEC2:
            return PXShaderVariableTypeBoolVector2; // bvec2
        case GL_BOOL_VEC3:
            return PXShaderVariableTypeBoolVector3; //     bvec3
        case GL_BOOL_VEC4:
            return PXShaderVariableTypeBoolVector4; //     bvec4
        case GL_FLOAT_MAT2:
            return PXShaderVariableTypeMatrix2x2; //     mat2
        case GL_FLOAT_MAT3:
            return PXShaderVariableTypeMatrix3x3; //     mat3
        case GL_FLOAT_MAT4:
            return PXShaderVariableTypeMatrix4x4; //      mat4
        case GL_FLOAT_MAT2x3:
            return PXShaderVariableTypeMatrix2x3; //     mat2x3
        case GL_FLOAT_MAT2x4:
            return PXShaderVariableTypeMatrix2x4; //     mat2x4
        case GL_FLOAT_MAT3x2:
            return PXShaderVariableTypeMatrix3x2; //     mat3x2
        case GL_FLOAT_MAT3x4:
            return PXShaderVariableTypeMatrix3x4; //     mat3x4
        case GL_FLOAT_MAT4x2:
            return PXShaderVariableTypeMatrix4x2; //     mat4x2
        case GL_FLOAT_MAT4x3:
            return PXShaderVariableTypeMatrix4x3; //     mat4x3
        case GL_SAMPLER_2D:
            return PXShaderVariableTypeSampler2DF; // sampler2D
        case GL_SAMPLER_3D:
            return PXShaderVariableTypeSampler3DF; //     sampler3D
        case GL_SAMPLER_CUBE:
            return PXShaderVariableTypeSamplerCubeF; //     samplerCube
        case GL_SAMPLER_2D_SHADOW:
            return PXShaderVariableTypeSamplerF2DShadow; //     sampler2DShadow
        case GL_SAMPLER_2D_ARRAY:
            return PXShaderVariableTypeSampler2DArrayF; //     sampler2DArray
        case GL_SAMPLER_2D_ARRAY_SHADOW:
            return PXShaderVariableTypeSamplerF2DArrayShadow; //     sampler2DArrayShadow
        case GL_SAMPLER_CUBE_SHADOW:
            return PXShaderVariableTypeSamplerFCubeShadow; //     samplerCubeShadow
        case GL_INT_SAMPLER_2D:
            return PXShaderVariableTypeSampler2DI32S; //     isampler2D
        case GL_INT_SAMPLER_3D:
            return PXShaderVariableTypeSampler3DI32S; //     isampler3D
        case GL_INT_SAMPLER_CUBE:
            return PXShaderVariableTypeSamplerCubeI32S; //     isamplerCube
        case GL_INT_SAMPLER_2D_ARRAY:
            return PXShaderVariableTypeSampler2DArrayI32S; //      isampler2DArray
        case GL_UNSIGNED_INT_SAMPLER_2D:
            return PXShaderVariableTypeSampler2DI32U; //     usampler2D
        case GL_UNSIGNED_INT_SAMPLER_3D:
            return PXShaderVariableTypeSampler3DI32U; //     usampler3D
        case GL_UNSIGNED_INT_SAMPLER_CUBE:
            return PXShaderVariableTypeSamplerCubeI32U; //     usamplerCube
        case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY:
            return PXShaderVariableTypeSampler2DArrayI32U; // usampler2DArray

        default:
            return PXShaderVariableTypeInvalid;
    }
}

PXActionResult PXAPI PXOpenGLSpriteRegister(PXOpenGL* const pxOpenGL, PXSprite* const pxSprite)
{
#if 0
    const PXBool hasScaling = pxSprite->TextureScalePositionOffset.X != 0 || pxSprite->TextureScalePositionOffset.Y != 0;

    if(!hasScaling)
    {
        const PXF32 vertexData[] =
        {
            0, 1, -1, -1, 0,// 01
            1, 1,  1, -1, 0,// 00
            1, 0,  1,  1, 0,// 10
            0, 0, -1,  1, 0,
        };
        const PXInt8U indexData[] = { 0, 1, 2, 2, 3, 0 };
        const PXInt8U indexAmount = sizeof(indexData) / sizeof(PXInt8U);

        pxSprite->Model.VertexBuffer.Format = PXVertexBufferFormatT2F_XYZ;
        pxSprite->Model.VertexBuffer.VertexData = (void*)vertexData;
        pxSprite->Model.VertexBuffer.VertexDataRowSize = (sizeof(vertexData) / sizeof(PXF32)) / 4u;
        pxSprite->Model.VertexBuffer.VertexDataSize = sizeof(vertexData);

        pxSprite->Model.IndexBuffer.IndexTypeSize = 1u;
        pxSprite->Model.IndexBuffer.IndexData = (void*)indexData;
        pxSprite->Model.IndexBuffer.IndexDataSize = sizeof(indexData);
        pxSprite->Model.IndexBuffer.IndexDataAmount = indexAmount;
        pxSprite->Model.IndexBuffer.DataType = PXTypeInt08U;
        pxSprite->Model.IndexBuffer.DrawModeID = PXDrawModeIDTriangle;// | PXDrawModeIDPoint | PXDrawModeIDLineLoop;

        PXOpenGLModelRegister(pxOpenGL, &pxSprite->Model);
    }
    else
    {
        const PXF32 textureWidth = pxSprite->Model.IndexBuffer.SegmentPrime.Material->DiffuseTexture->Image->Width;
        const PXF32 textureHeight = pxSprite->Model.IndexBuffer.SegmentPrime.Material->DiffuseTexture->Image->Height;
        PXF32 offset[2] =
        {

            //pxSprite->Position.XX, pxSprite->Position.YY
            // 0.05f,  0.05f
            pxSprite->TextureScalePositionOffset.X,
            pxSprite->TextureScalePositionOffset.Y
            //textureWidth / (PXF32)pxOpenGL->Binding.AttachedWindow->Width,
            //textureHeight / (PXF32)pxOpenGL->Binding.AttachedWindow->Height
        };
        // PXF32 tx[2] = { (pxSprite->Texture.Image.Width + 200)/ (PXF32)window->Width, (pxSprite->Texture.Image.Height + 200) / (PXF32)window->Height };

        const PXF32 tx[2] =
        {
            pxSprite->TextureScalePointOffset.X,
            pxSprite->TextureScalePointOffset.Y
        };

        PXVector2F32 vxOffset[4];



        const PXF32 vertexData[] =
        {
            // Left-Lower-Quadrant
            0,          1,                  -1,                -1,                0,  // 00
            tx[0],  1,                  -1 + offset[0],    -1,                0,// 10
            tx[0],  1 - tx[1],      -1 + offset[0],    -1 + offset[1],    0,// 11
            0,          1 - tx[1],      -1,                -1 + offset[1],    0,// 01

            // Left-Upper-Quadrant
            0,          tx[1],          -1,                 1 - offset[1],    0, // 00
            tx[0],  tx[1],          -1 + offset[0],     1 - offset[1],    0,// 10
            tx[0],  0,                  -1 + offset[0],     1,                0,// 11
            0,          0,                  -1,                 1,                0, // 01

            // Right-Lower-Quadrant
            1 - tx[0],  1,              1 - offset[0],    -1,                0,
            1,              1,              1,                -1,                0, // OK
            1,              1 - tx[1],  1,                -1 + offset[1],    0,
            1 - tx[0],  1 - tx[1],  1 - offset[0],    -1 + offset[1],    0,

            // Right-Upper-Quadrant
            1 - tx[0],  tx[1],      1 - offset[0],     1 - offset[1],    0,
            1,              tx[1],      1,                 1 - offset[1],    0,
            1,              0,              1,                 1,                0,
            1 - tx[0],  0,              1 - offset[0],     1,                0
        };
        const PXInt8U indexData[] =
        {
#if Quad
            0,1,2,3, // Left-Lower
            3,2,5,4, // Left-Middle
            4,5,6,7, // Left-Upper

            5,12,15,6, // Middle-Top
            2,11,12,5, // Middle-Middle
            1,8,11,2, // Middle-Bot

            12,13,14,15, // Right-Upper
            12,11,10,13, // Right-Middle
            8,9,10,11 // Right-Lower
#else
            0,1,2, // Left-Lower
            2,3,0,

            3,2,5, // Left-Middle
            5,4,3,

            4,5,6, // Left-Upper
            6,7,4,

            5,12,15, // Middle-Top
            15,6,5,

            2,11,12, // Middle-Middle
            12,5,2,

            1,8,11, // Middle-Bot
            11,2,1,

            12,13,14, // Right-Upper
            14,15,12,

            12,11,10, // Right-Middle
            10,13,12,

            8,9,10, // Right-Lower
            10,11,8

#endif
        };
        const PXInt8U indexAmount = sizeof(indexData) / sizeof(PXInt8U);

        pxSprite->Model.VertexBuffer.Format = PXVertexBufferFormatT2F_XYZ;
        pxSprite->Model.VertexBuffer.VertexData = (void*)vertexData;
        pxSprite->Model.VertexBuffer.VertexDataRowSize = (sizeof(vertexData) / sizeof(PXF32)) / 4u;
        pxSprite->Model.VertexBuffer.VertexDataSize = sizeof(vertexData);

        pxSprite->Model.IndexBuffer.IndexTypeSize = 1u;
        pxSprite->Model.IndexBuffer.IndexData = (void*)indexData;
        pxSprite->Model.IndexBuffer.IndexDataSize = sizeof(indexData);
        pxSprite->Model.IndexBuffer.IndexDataAmount = indexAmount;
        pxSprite->Model.IndexBuffer.DataType = PXTypeInt08U;
        pxSprite->Model.IndexBuffer.DrawModeID = PXDrawModeIDPoint | PXDrawModeIDLine | PXDrawModeIDTriangle;

        PXOpenGLModelRegister(pxOpenGL, &pxSprite->Model);
    }
#endif

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLDrawScriptCreate(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript)
{
    // Create one display list
    pxDrawScript->Info.Handle.OpenGLID = pxOpenGL->Binding.GenLists(1);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLDrawScriptBegin(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript)
{
    pxOpenGL->Binding.NewList(pxDrawScript->Info.Handle.OpenGLID, GL_COMPILE);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLDrawScriptEnd(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript)
{
    pxOpenGL->Binding.EndList();

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLDrawScriptDelete(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript)
{
    pxOpenGL->Binding.DeleteLists(pxDrawScript->Info.Handle.OpenGLID, 1);

    pxDrawScript->Info.Handle.OpenGLID = -1;

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLDrawScriptExecute(PXOpenGL* const pxOpenGL, PXDrawScript* const pxDrawScript)
{
    pxOpenGL->Binding.CallList(pxDrawScript->Info.Handle.OpenGLID);

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLLightSet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index)
{
    pxOpenGL->Binding.Lightf(index, GL_SPOT_EXPONENT, pxLight->Falloff);
    pxOpenGL->Binding.Lightf(index, GL_SPOT_CUTOFF, pxLight->CutoffRange);
    pxOpenGL->Binding.Lightf(index, GL_CONSTANT_ATTENUATION, pxLight->AttenuationConstant);
    pxOpenGL->Binding.Lightf(index, GL_LINEAR_ATTENUATION, pxLight->AttenuationLinear);
    pxOpenGL->Binding.Lightf(index, GL_QUADRATIC_ATTENUATION, pxLight->AttenuationQuadratic);

    return PXActionSuccessful;
}
PXActionResult PXAPI PXOpenGLLightGet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index)
{
    pxOpenGL->Binding.GetLightfv(index, GL_SPOT_EXPONENT, &pxLight->Falloff);
    pxOpenGL->Binding.GetLightfv(index, GL_SPOT_CUTOFF, &pxLight->CutoffRange);
    pxOpenGL->Binding.GetLightfv(index, GL_CONSTANT_ATTENUATION, &pxLight->AttenuationConstant);
    pxOpenGL->Binding.GetLightfv(index, GL_LINEAR_ATTENUATION, &pxLight->AttenuationLinear);
    pxOpenGL->Binding.GetLightfv(index, GL_QUADRATIC_ATTENUATION, &pxLight->AttenuationQuadratic);

    return PXActionSuccessful;
}
PXActionResult PXAPI PXOpenGLLightEnableSet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index, const PXBool enable)
{
    return PXActionSuccessful;
}
PXActionResult PXAPI PXOpenGLLightEnableGet(PXOpenGL* const pxOpenGL, PXLight* const pxLight, const PXInt32U index, PXBool* const enable)
{
    return PXActionSuccessful;
}






void PXOpenGKShaderDataBufferCreate(PXOpenGL* const pxOpenGL, PXShaderDataBuffer* const pxShaderDataBuffer, const PXSize sizeOfData, void* data)
{
    // if we support SSBOs (v.4.3), then use those
    if(pxOpenGL->Binding.ShaderStorageBlockBinding)
    {
        PXSize maxAmount = 0;
        PXSize maxSize = 0;

        if(pxOpenGL->Binding.GetInteger64v)
        {
            PXInt64S maxAmount64 = 0;
            PXInt64S maxSize64 = 0;
            pxOpenGL->Binding.GetInteger64v(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxAmount64);
            pxOpenGL->Binding.GetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSize64);
            maxAmount = maxAmount64;
            maxSize = maxSize64;
        }
        else
        {
            PXSize maxAmount32 = 0;
            PXSize maxSize32 = 0;
            pxOpenGL->Binding.GetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &maxAmount32);
            pxOpenGL->Binding.GetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxSize32);
            maxAmount = maxAmount32;
            maxSize = maxSize32;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXOpenGLName,
            "Texture1D",
            "Max supported SSBO binings:%li, size: %li",
            maxAmount,
            maxSize
        );
#endif

        // Generate and bind buffer
        GLuint ssbo;
        pxOpenGL->Binding.BufferGenerate(1, &ssbo);
        pxOpenGL->Binding.BufferBind(GL_SHADER_STORAGE_BUFFER, ssbo);

        // Allocate and populate data
        pxOpenGL->Binding.BufferData(GL_SHADER_STORAGE_BUFFER, sizeOfData, data, GL_STATIC_DRAW);

        pxShaderDataBuffer->Info.Handle.OpenGLID = ssbo;

        pxOpenGL->Binding.BufferBind(GL_SHADER_STORAGE_BUFFER, 0);
    }
    else
    {
        // Example: Creating a 1D texture for normals
        PXSize maxTextureSize = 0;

        if(pxOpenGL->Binding.GetInteger64v)
        {
            PXInt64S maxTextureSize64;
            pxOpenGL->Binding.GetInteger64v(GL_MAX_TEXTURE_SIZE, &maxTextureSize64);
            maxTextureSize = maxTextureSize64;
        }
        else
        {
            GLint maxTextureSize32;
            pxOpenGL->Binding.GetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize32);
            maxTextureSize = maxTextureSize32;
        }

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXOpenGLName,
            "Texture1D",
            "Max supported size: %li",
            maxTextureSize
        );
#endif


        GLuint normalsTexture;
        pxOpenGL->Binding.TextureCreate(1, &normalsTexture);
        pxOpenGL->Binding.TextureBind(GL_TEXTURE_1D, normalsTexture);

        pxShaderDataBuffer->Info.Handle.OpenGLID = normalsTexture;

        // Upload data to the texture
        pxOpenGL->Binding.TextureData1D(GL_TEXTURE_1D, 0, GL_RED, sizeOfData / sizeof(PXF32), 0, GL_RED, GL_FLOAT, data);


        // Set texture parameters
        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        pxOpenGL->Binding.TextureParameterI(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

        // Repeat for texture coordinates or any other data
    }   
}


PXActionResult PXAPI PXOpenGLModelRegister(PXOpenGL* const pxOpenGL, PXModel* const pxModel)
{
    if(!(pxOpenGL && pxModel))
    {
        return PXActionRefusedArgumentNull;
    }

   // PXVertexBuffer* const pxVertexBuffer = &pxModel->Mesh.VertexBuffer;
    PXIndexBuffer* const pxIndexBuffer = &pxModel->Mesh.IndexBuffer;
    PXMesh* const pxMesh = &pxModel->Mesh;

    // Check if the model is valid
    if(0 == pxMesh->VertexBufferListAmount)
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingError,
            PXOpenGLName,
            PXOpenGLModelName,
            "Invalid model! No vertex mesh data.",
            pxMesh->Info.Handle.OpenGLID
        );
#endif
        return PXActionInvalid;
    }


    if(pxOpenGL->Binding.VertexArraysGenerate)
    {
        pxOpenGL->Binding.VertexArraysGenerate(1, &(pxMesh->Info.Handle.OpenGLID)); // VAO
        pxOpenGL->Binding.VertexArrayBind(pxMesh->Info.Handle.OpenGLID);

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            PXOpenGLModelName,
            "VAO created <%i>",
            pxMesh->Info.Handle.OpenGLID
        );
#endif
    }
    else
    {
#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXOpenGLName,
            PXOpenGLModelName,
            "VAO not supported"
        );
#endif
    }


    if(!pxOpenGL->Binding.BufferBind) // Is it possible to have a buffer?
    {
        // Declare IDs as unused as we can't use buffers
        //PXResourceIDMarkAsUnused(&pxModel->VertexBuffer.Info);
        //PXResourceIDMarkAsUnused(&pxModel->IndexBuffer.Info);

        /*
        // Copy references, we can't trust if these are not on the stack.
        void* vertexData = pxVertexBuffer->VertexData;
        void* indexData = pxIndexBuffer->IndexData;

        // Copy vertex data
        pxVertexBuffer->VertexData = PXMemoryHeapCallocT(PXByte, pxVertexBuffer->VertexDataSize);
        PXMemoryCopy(vertexData, pxVertexBuffer->VertexDataSize, pxVertexBuffer->VertexData, pxVertexBuffer->VertexDataSize);

        // Copy index data
        pxIndexBuffer->IndexData = PXMemoryHeapCallocT(PXByte, pxIndexBuffer->IndexDataSize);
        PXMemoryCopy(indexData, pxIndexBuffer->IndexDataSize, pxIndexBuffer->IndexData, pxIndexBuffer->IndexDataSize);

        */

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingWarning,
            PXOpenGLName,
            PXOpenGLModelName,
            "VBO not supported, Copy for Client-Buffer use."
        );
#endif

        return PXActionSuccessful;
    }


    const PXBool hasIndexData = 
        pxIndexBuffer->DataIndexSizeSegment > 0 && 
        pxMesh->VertexBufferListAmount == 1;


    // Registering of array buffers
    {

        const PXSize bufferAmountRequest = pxMesh->VertexBufferListAmount + hasIndexData;


        //-------------------------------------------------
        // Create IDs
        //-------------------------------------------------

        GLuint bufferIDs[32];
        PXSize indexCounter = 0;

#if PXLogEnable
        PXLogPrint
        (
            PXLoggingInfo,
            PXOpenGLName,
            PXOpenGLModelName,
            "Create array buffers: <%i>",
            bufferAmountRequest
        );
#endif

        if(bufferAmountRequest < 32)
        {
            pxOpenGL->Binding.BufferGenerate(bufferAmountRequest, bufferIDs);
        }
        else
        {
            // Handle more data with allocating
        }

        //-------------------------------------------------
        // Store BufferIDs
        //-------------------------------------------------
        if(hasIndexData)
        {
            pxIndexBuffer->Info.Handle.OpenGLID = bufferIDs[indexCounter++];

#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                PXOpenGLModelName,
                "IBO:<%i>",
                pxIndexBuffer->Info.Handle.OpenGLID
            );
#endif
        }

        if(pxMesh->VertexBufferListAmount == 1)
        {
            pxMesh->VertexBufferPrime.Info.Handle.OpenGLID = bufferIDs[indexCounter++];
        }
        else
        {
            for(PXSize i = 0; i < pxMesh->VertexBufferListAmount; ++i)
            {
                PXVertexBuffer* const pxVertexBuffer = &pxMesh->VertexBufferList[i];

                pxVertexBuffer->Info.Handle.OpenGLID = bufferIDs[indexCounter++];

                const char* formatName = PXVertexBufferFormatToString(pxVertexBuffer->Format);

#if PXLogEnable
                PXLogPrint
                (
                    PXLoggingInfo,
                    PXOpenGLName,
                    PXOpenGLModelName,
                    "VBO:<%i>, <%s>",
                    pxVertexBuffer->Info.Handle.OpenGLID,
                    formatName
                );
#endif
            }
        }
    }





    // Make sure current format is supported.
    // Transform data is needed from current format to supported one
    {
        PXVertexBufferFormat pxVertexBufferFormat[] =
        {
            PXVertexBufferFormatP3F32,
            PXVertexBufferFormatT2F_XYZ,
            PXVertexBufferFormatT2F_N3F_XYZ
        };

        PXModelFormatTransmuteInfo pxModelFormatTransmuteInfo;
        pxModelFormatTransmuteInfo.VertexFormatAmount = 3;
        pxModelFormatTransmuteInfo.VertexFormatList = pxVertexBufferFormat;

        PXModelFormatTransmute(pxModel, &pxModelFormatTransmuteInfo);
    }







    // Upload IBO
    {
        if(hasIndexData)
        {
#if PXLogEnable
            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                PXOpenGLModelName,
                "IBO:<%i> upload <%p> with %i B, (TypeWidth:%i)",
                pxIndexBuffer->Info.Handle.OpenGLID,
                pxIndexBuffer->DataIndexPosition,
                pxIndexBuffer->DataIndexSizeSegment,
                PXTypeSizeGet(pxIndexBuffer->IndexDataType)
            );
#endif

#if PXLogEnable && 0
            for(PXSize i = 0; i < pxIndexBuffer->IndexDataSize; i += 3)
            {
                PXInt8U* data = &((PXInt8U*)pxIndexBuffer->IndexData)[i];

                PXLogPrint
                (
                    PXLoggingInfo,
                    PXOpenGLName,
                    PXOpenGLModelName,
                    "%2i, %2i, %2i",
                    data[0],
                    data[1],
                    data[2]
                );
            }
#endif

            // Select
            pxOpenGL->Binding.BufferBind(GL_ELEMENT_ARRAY_BUFFER, pxIndexBuffer->Info.Handle.OpenGLID);

            // Define data layout?
            // NO!, we actually define the type when we draw. Because of opengl design

            // Upload
            pxOpenGL->Binding.BufferData(GL_ELEMENT_ARRAY_BUFFER, pxIndexBuffer->DataIndexSizeSegment, pxIndexBuffer->DataIndexPosition, GL_STATIC_DRAW);

            // Unselect. not needed if VAO are supported
           pxOpenGL->Binding.BufferBind(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
    }

    // Upload VBO
    {
        if(1 == pxMesh->VertexBufferListAmount)
        {
            PXVertexBuffer* const pxVertexBuffer = &pxMesh->VertexBufferPrime;

            pxOpenGL->Binding.BufferBind(GL_ARRAY_BUFFER, pxVertexBuffer->Info.Handle.OpenGLID);

            // Upload
            pxOpenGL->Binding.BufferData(GL_ARRAY_BUFFER, pxVertexBuffer->VertexDataSize, pxVertexBuffer->VertexData, GL_STATIC_DRAW);

            // Define layout   
            const PXInt8U sizePerVertex = PXVertexBufferFormatSizePerVertex(pxVertexBuffer->Format);
            const GLenum openGLType = PXOpenGLTypeIDFromFormat(pxVertexBuffer->Format);

            pxOpenGL->Binding.VertexAttribPointer
            (
                0,
                sizePerVertex,
                openGLType,
                GL_FALSE,
                0,
                0
            );
            pxOpenGL->Binding.VertexAttribArrayEnable(0);

#if PXLogEnable
            const char* vertexFormatName = PXVertexBufferFormatToString(pxVertexBuffer->Format);

            PXLogPrint
            (
                PXLoggingInfo,
                PXOpenGLName,
                PXOpenGLModelName,
                "VBO:<%i>, %s,  upload <%p> with %i B",
                pxVertexBuffer->Info.Handle.OpenGLID,
                vertexFormatName,
                pxVertexBuffer->VertexData,
                pxVertexBuffer->VertexDataSize
            );
#endif

            //PXConsoleWriteTablePXF32(pxVertexBuffer->VertexData, pxVertexBuffer->VertexDataSize / sizeof(PXF32), 3);
        }
        else
        {
            PXVertexBuffer* const pxVertexBufferPosition = &pxMesh->VertexBufferList[0];
            PXVertexBuffer* const pxVertexBufferNormal = &pxMesh->VertexBufferList[1];
            PXVertexBuffer* const pxVertexBufferTexture = &pxMesh->VertexBufferList[2];
      
            PXIndexBuffer* const pxIndexBuffer = &pxModel->Mesh.IndexBuffer;

            const PXSize vertexTypeSize = 4;
            const PXSize vertexStride = vertexTypeSize *(3+3+2);
            const PXSize indexTypeSize = PXTypeSizeGet(pxIndexBuffer->IndexDataType);

            void* indexVertex = pxIndexBuffer->DataIndexPosition;
            void* indexNormal = pxIndexBuffer->DataIndexNormal;
            void* indexTexture = pxIndexBuffer->DataIndexTexturePos;


            PXSize vertexDataInterleavedSize = pxIndexBuffer->DataIndexAmount * vertexStride;

            pxOpenGL->Binding.BufferBind(GL_ARRAY_BUFFER, pxVertexBufferPosition->Info.Handle.OpenGLID);
            pxOpenGL->Binding.BufferData
            (
                GL_ARRAY_BUFFER, 
                vertexDataInterleavedSize,
                0,
                GL_STATIC_DRAW
            );

           // const PXInt8U sizePerVertex = PXVertexBufferFormatSizePerVertex(pxVertexBufferPosition->Format);
            const GLenum openGLType = PXOpenGLTypeIDFromFormat(pxVertexBufferPosition->Format);

            pxOpenGL->Binding.VertexAttribPointer(0, 3, openGLType, GL_FALSE, vertexStride, 0);
            pxOpenGL->Binding.VertexAttribArrayEnable(0);

            pxOpenGL->Binding.VertexAttribPointer(1, 3, openGLType, GL_FALSE, vertexStride, vertexTypeSize *3);
            pxOpenGL->Binding.VertexAttribArrayEnable(1);

            pxOpenGL->Binding.VertexAttribPointer(2, 2, openGLType, GL_FALSE, vertexStride, vertexTypeSize * (3+3));
            pxOpenGL->Binding.VertexAttribArrayEnable(2);
       


            //PXByte vertexDataCache[(3 + 3 + 2) * sizeof(PXF32)];
            PXF32 vertexDataCache[(3 + 3 + 2)];
            PXSize vertexDataCacheOffset = 0;

            PXSize globalOffset = 0;
            PXSize indexOffset = 0;

            for(PXSize segmentIndex = 0; segmentIndex < pxIndexBuffer->SegmentListAmount; ++segmentIndex)
            {
                PXIndexSegment* const pxIndexSegment = &pxIndexBuffer->SegmentList[segmentIndex];

                const PXSize range = pxIndexSegment->DataRange;

                for(PXSize i = 0; i < range; ++i)
                {    
                    PXInt16U dataIndexVertex = *(PXInt16U*)((PXByte*)indexVertex + (indexTypeSize * indexOffset));
                    PXInt16U dataIndexNormal = *(PXInt16U*)((PXByte*)indexNormal + (indexTypeSize * indexOffset));
                    PXInt16U dataIndexTexture = *(PXInt16U*)((PXByte*)indexTexture + (indexTypeSize * indexOffset));

                    void* dataVertex = (PXByte*)pxVertexBufferPosition->VertexData + (vertexTypeSize * dataIndexVertex * 3);
                    void* dataNormal = (PXByte*)pxVertexBufferNormal->VertexData + (vertexTypeSize * dataIndexNormal * 3);
                    void* dataTexture = (PXByte*)pxVertexBufferTexture->VertexData + (vertexTypeSize * dataIndexTexture * 2);

                    vertexDataCacheOffset += PXMemoryCopy(dataVertex, vertexTypeSize * 3, (PXByte*)vertexDataCache + vertexDataCacheOffset, 100);
                    vertexDataCacheOffset += PXMemoryCopy(dataNormal, vertexTypeSize * 3, (PXByte*)vertexDataCache + vertexDataCacheOffset, 100);
                    vertexDataCacheOffset += PXMemoryCopy(dataTexture, vertexTypeSize * 2, (PXByte*)vertexDataCache + vertexDataCacheOffset, 100);

                    pxOpenGL->Binding.BufferDataSub(GL_ARRAY_BUFFER, globalOffset, (GLsizeiptr)vertexDataCacheOffset, vertexDataCache);

                    globalOffset += vertexDataCacheOffset;
                    vertexDataCacheOffset = 0;
                    ++indexOffset;

                    // Mark as usable!                      
                }        

#if PXLogEnable
                int progress = (globalOffset / (float)vertexDataInterleavedSize) * 100;

                PXLogPrint
                (
                    PXLoggingInfo,
                    PXOpenGLName,
                    PXOpenGLModelName,
                    "Segment:<%2i/%2i> ready! %7i/%7i (%3i)",
                    segmentIndex+1,
                    pxIndexBuffer->SegmentListAmount,
                    globalOffset,
                    vertexDataInterleavedSize,
                    progress
                );
#endif
            }
       

        
          
        }
    }








    /*
    
     PXVertexBuffer* const pxVertexBuffer = &pxMesh->VertexBufferList[i];

                if(i == 0)
                {
                    // Create as VBO

                    // Select
                    pxOpenGL->Binding.BufferBind(GL_ARRAY_BUFFER, pxVertexBuffer->Info.Handle.OpenGLID);

                    // Upload
                    pxOpenGL->Binding.BufferData(GL_ARRAY_BUFFER, pxVertexBuffer->VertexDataSize, pxVertexBuffer->VertexData, GL_STATIC_DRAW);

                    // Define layout   
                    const PXInt8U sizePerVertex = PXVertexBufferFormatSizePerVertex(pxVertexBuffer->Format);
                    // const GLenum openGLType = PXOpenGLTypeToID(pxVertexElementCurrent->Type);
                     // TODO: enable VBO to have other types besides PXF32s!!!

                    pxOpenGL->Binding.VertexAttribPointer
                    (
                        i,
                        sizePerVertex,
                        GL_FLOAT,
                        GL_FALSE,
                        0,
                        0
                    );
                    pxOpenGL->Binding.VertexAttribArrayEnable(i);

#if PXLogEnable
                    const char* vertexFormatName = PXVertexBufferFormatToString(pxVertexBuffer->Format);

                    PXLogPrint
                    (
                        PXLoggingInfo,
                        PXOpenGLName,
                        PXOpenGLModelName,
                        "(%i/%i) VBO:<%i>, %s,  upload <%p> with %i B",
                        i + 1,
                        pxMesh->VertexBufferListAmount,
                        pxVertexBuffer->Info.Handle.OpenGLID,
                        vertexFormatName,
                        pxVertexBuffer->VertexData,
                        pxVertexBuffer->VertexDataSize
                    );
#endif
                }
                else
                {
                    PXOpenGKShaderDataBufferCreate();

                 
                }


    */

















/*

    {
        PXInt8U pxVertexElementLength = 0;
        PXVertexElement pxVertexElementList[20];
        PXClearList(PXVertexElement, pxVertexElementList, 20);

        // make
        {
            const PXInt8U stride = PXVertexBufferFormatStrideSize(pxVertexBuffer->Format) * sizeof(PXF32);

            switch(pxVertexBuffer->Format)
            {
                case PXVertexBufferFormatXYZPXF32:
                {
                    PXVertexElement* pxVertexElement = &pxVertexElementList[0];

                    pxVertexElement->Type = PXTypePXF32;
                    pxVertexElement->Length = 3u; // X, Y, Z => 3 Elemets
                    pxVertexElement->Stride = stride; // Size of a whole vertex
                    pxVertexElement->StartAdress = 0; // No offset, we start at 0

                    pxVertexElementLength = 1;

                    break;
                }
                case PXVertexBufferFormatT2F_XYZ:
                {
                    pxVertexElementList[0].Type = PXTypePXF32;
                    pxVertexElementList[0].Length = 2u; // X, Y, Z => 3 Elemets
                    pxVertexElementList[0].Stride = stride;
                    pxVertexElementList[0].StartAdress = 0; // No offset, we start at 0

                    pxVertexElementList[1].Type = PXTypePXF32;
                    pxVertexElementList[1].Length = 3u; // X, Y, Z => 3 Elemets
                    pxVertexElementList[1].Stride = stride;
                    pxVertexElementList[1].StartAdress = 2 * sizeof(PXF32); // No offset, we start at 0

                    pxVertexElementLength = 2;

                    break;
                }
                case PXVertexBufferFormatT2F_N3F_XYZ:
                {
                    pxVertexElementList[0].Type = PXTypePXF32;
                    pxVertexElementList[0].Length = 2u; // X, Y, Z => 3 Elemets
                    pxVertexElementList[0].Stride = stride;
                    pxVertexElementList[0].StartAdress = 0; // No offset, we start at 0

                    pxVertexElementList[1].Type = PXTypePXF32;
                    pxVertexElementList[1].Length = 3u; // X, Y, Z => 3 Elemets
                    pxVertexElementList[1].Stride = stride;
                    pxVertexElementList[1].StartAdress = 2 * sizeof(PXF32); // No offset, we start at 0

                    pxVertexElementList[2].Type = PXTypePXF32;
                    pxVertexElementList[2].Length = 3u; // X, Y, Z => 3 Elemets
                    pxVertexElementList[2].Stride = stride;
                    pxVertexElementList[2].StartAdress = (2 + 3) * sizeof(PXF32); // No offset, we start at 0

                    pxVertexElementLength = 3;

                    break;
                }
                default:
                { 
                    break; 
                }
            }
        }

        for(PXSize i = 0; i < pxVertexElementLength; ++i)
        {
            const PXVertexElement* const pxVertexElementCurrent = &pxVertexElementList[i];
            const GLenum openGLType = PXOpenGLTypeToID(pxVertexElementCurrent->Type);

            pxOpenGL->Binding.VertexAttribArrayEnable(i);
            pxOpenGL->Binding.VertexAttribPointer(i, pxVertexElementCurrent->Length, openGLType, GL_FALSE, pxVertexElementCurrent->Stride, (void*)pxVertexElementCurrent->StartAdress);
        }
        */
    

    /*
    Now, here is how we would do it using the new APIs:

    glBindVertexBuffer(0, buff, baseOffset, sizeof(Vertex));

    glEnableVertexAttribArray(0);
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexAttribBinding(0, 0);
    glEnableVertexAttribArray(1);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, normal));
    glVertexAttribBinding(1, 0);
    glEnableVertexAttribArray(2);
    glVertexAttribFormat(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, offsetof(Vertex, color));
    glVertexAttribBinding(2, 0);
    */

    pxOpenGL->Binding.VertexArrayBind(0);

    // Additional textures
    {
        for(PXSize containerID = 0; containerID < pxModel->MaterialContaierListAmount; ++containerID)
        {
            PXMaterialContainer* const pxMaterialContainer = &pxModel->MaterialContaierList[containerID];

            PXSize pxTextureListCounter = 0;
            PXTexture2D** pxTextureList = PXMemoryHeapCallocT(PXTexture2D*, pxMaterialContainer->MaterialListAmount);

            for(PXSize materialID = 0; materialID < pxMaterialContainer->MaterialListAmount; ++materialID)
            {
                PXMaterial* const pxMaterial = &pxMaterialContainer->MaterialList[materialID];

                if(pxMaterial->DiffuseTexture)
                {
                    pxTextureList[pxTextureListCounter] = pxMaterial->DiffuseTexture;

                    ++pxTextureListCounter;
                }
            }


            PXGraphicTexturInfo pxGraphicTexturInfo;
            pxGraphicTexturInfo.TextureReference = (void**)pxTextureList;
            pxGraphicTexturInfo.Amount = pxTextureListCounter;
            pxGraphicTexturInfo.Type = PXGraphicTextureType2D;
            pxGraphicTexturInfo.Action = PXResourceActionCreate;

            PXOpenGLTextureAction(pxOpenGL, &pxGraphicTexturInfo);


            PXMemoryHeapFree(PXNull, pxTextureList);
        }
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLModelDeregister(PXOpenGL* const pxOpenGL, PXModel* const pxModel)
{
    //pxOpenGL->Binding.Vertex(1, pxModel->ResourceID.OpenGLID); glDeleteVertexArrays

    return PXActionRefusedNotImplemented;
}

PXActionResult PXAPI PXOpenGLRectangleDraw(PXOpenGL* const pxOpenGL, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const PXInt8U mode)
{
    switch(mode)
    {
        case 1:
        {
            pxOpenGL->Binding.Rectf(xA, yA, xB, yB);

            break;
        }
        case 2:
        {
            // OpenGL works in a normalizes space. Ranging from -1 to +1.
            // [0,0] is middle of the screen, [-1,-1] lower left, [+1,+1] upper right

            pxOpenGL->Binding.Rectf(-1 + xA, -1 + yA, 1 - xB, 1 - yB);

            break;
        }
        default:
            return PXActionRefusedNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}

PXActionResult PXAPI PXOpenGLRectangleDrawTx(PXOpenGL* const pxOpenGL, const PXF32 xA, const PXF32 yA, const PXF32 xB, const PXF32 yB, const PXF32 txA, const PXF32 tyA, const PXF32 txB, const PXF32 tyB, const PXInt8U mode)
{
    switch(mode)
    {
        case 1:
        {
            pxOpenGL->Binding.Begin(GL_QUADS);
            pxOpenGL->Binding.TexCoord2f(txA, tyB);
            pxOpenGL->Binding.Vertex2f(xA, yA);// 11
            pxOpenGL->Binding.TexCoord2f(txB, tyB);
            pxOpenGL->Binding.Vertex2f(xB, yA);// 10
            pxOpenGL->Binding.TexCoord2f(txB, tyA);
            pxOpenGL->Binding.Vertex2f(xB, yB);// 00
            pxOpenGL->Binding.TexCoord2f(txA, tyA);
            pxOpenGL->Binding.Vertex2f(xA, yB);// 01
            pxOpenGL->Binding.End();
            break;
        }
        case 2:
        {
            pxOpenGL->Binding.Begin(GL_QUADS);
            pxOpenGL->Binding.TexCoord2f(txA, tyB);
            pxOpenGL->Binding.Vertex2f(-1 + xA, -1 + yA);// 11
            pxOpenGL->Binding.TexCoord2f(txB, tyB);
            pxOpenGL->Binding.Vertex2f(+1 - xB, -1 + yA);// 10
            pxOpenGL->Binding.TexCoord2f(txB, tyA);
            pxOpenGL->Binding.Vertex2f(+1 - xB, +1 - yB);// 00
            pxOpenGL->Binding.TexCoord2f(txA, tyA);
            pxOpenGL->Binding.Vertex2f(-1 + xA, +1 - yB);// 01
            pxOpenGL->Binding.End();
        }
        default:
            return PXActionRefusedNotSupportedByLibrary;
    }

    return PXActionSuccessful;
}
