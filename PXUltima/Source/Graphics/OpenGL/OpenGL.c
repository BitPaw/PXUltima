#include "OpenGL.h"

// wingdi.h
#include <stdio.h>

#include <Error/ActionResult.h>
#include <Memory/Memory.h>
#include <Text/Text.h>
#include <Container/ClusterValue.h>

#if defined(OSUnix)

#elif defined(OSWindows)
#pragma comment(lib, "opengl32.lib")
#endif


CPrivate OpenGLVersion OpenGLVersionParse(const unsigned int versionID)
{
    switch (versionID)
    {
    case MakeInt(0, 1, 0, 0): return OpenGLVersion1x0;
    case MakeInt(0, 1, 1, 0): return  OpenGLVersion1x1;
    case MakeInt(0, 1, 2, 0): return  OpenGLVersion1x2;
    case MakeInt(0, 1, 2, 1): return OpenGLVersion1x2x1;
    case MakeInt(0, 1, 3, 0): return OpenGLVersion1x3;
    case MakeInt(0, 1, 4, 0): return OpenGLVersion1x4;
    case MakeInt(0, 1, 5, 0):return OpenGLVersion1x5;
    case MakeInt(0, 2, 0, 0): return  OpenGLVersion2x0;
    case MakeInt(0, 2, 1, 0):return OpenGLVersion2x1;
    case MakeInt(0, 3, 0, 0): return OpenGLVersion3x0;
    case MakeInt(0, 3, 1, 0):return  OpenGLVersion3x1;
    case MakeInt(0, 3, 2, 0): return OpenGLVersion3x2;
    case MakeInt(0, 3, 3, 0): return  OpenGLVersion3x3;
    case MakeInt(0, 4, 0, 0): return  OpenGLVersion4x0;
    case MakeInt(0, 4, 1, 0): return  OpenGLVersion4x1;
    case MakeInt(0, 4, 2, 0): return OpenGLVersion4x2;
    case MakeInt(0, 4, 3, 0): return OpenGLVersion4x3;
    case MakeInt(0, 4, 4, 0):return  OpenGLVersion4x4;
    case MakeInt(0, 4, 5, 0): return  OpenGLVersion4x5;
    case MakeInt(0, 4, 6, 0): return OpenGLVersion4x6;
    default: return  OpenGLVersionInvalid;
    }
}

void OpenGLCacheFunction(void** loadList, size_t* currentSize, char* name, void* functionADress)
{
    size_t index = *currentSize;

    loadList[index] = functionADress;
    loadList[index + 1] = name;

    *currentSize += 2u;
}

void OpenGLContextConstruct(OpenGLContext* const openGLContext)
{
    MemorySet(openGLContext, sizeof(OpenGLContext), 0);
}

void OpenGLContextDestruct(OpenGLContext* const openGLContext)
{

}

void OpenGLContextCreate(OpenGLContext* const openGLContext)
{
#if defined(OSUnix)
    glXMakeCurrent(window->DisplayCurrent, window->ID, window->OpenGLConext);


#elif defined(OSWindows)
    const HGLRC handle = wglCreateContext(openGLContext->WindowsDeviceContext);

    // Check if failed
    {
        const unsigned char successful = handle != 0;

        if (!successful)
        {
            const ActionResult actionResult = GetCurrentError();

            // return actionResult;
        }
    }

    openGLContext->OpenGLConext = handle;

#endif      

    OpenGLContextSelect(openGLContext);


    const char* vendor = glGetString(GL_VENDOR); // Returns the company responsible for this GL implementation.This name does not change from release to release.

    TextCopyA(vendor, -1, openGLContext->Vendor, 64);

    const char* renderer = glGetString(GL_RENDERER); //   Returns the name of the renderer.This name is typically specific to a particular configuration of a hardware platform.It does not change from release to release.

    TextCopyA(renderer, -1, openGLContext->Renderer, 64);

    const char* version = glGetString(GL_VERSION); //    Returns a version or release number.

    TextCopyA(version, -1, openGLContext->VersionText, 64);
    // glGetString(GL_SHADING_LANGUAGE_VERSION); //   Returns a version or release number for the shading language.

// Parse version
    {
        int versionMajor = 0;
        int versionMinor = 0;
        int versionPatch = 0;
        size_t offset = 0;

        offset += TextToIntA(version + offset, 64, &versionMajor);
        offset += 1u; // dot
        offset += TextToIntA(version + offset, 64, &versionMinor);
        offset += 1u; // dot
        offset += TextToIntA(version + offset, 64, &versionPatch);

        const unsigned int id = MakeInt(0, versionMajor, versionMinor, versionPatch);

        openGLContext->Version = OpenGLVersionParse(id);      
    }

    // Fetch functions

    size_t length = 0;
    void* functionNameList[128];

    switch (openGLContext->Version)
    {
    default: return;

    case OpenGLVersion4x6:
    {
        // Fall though1
    }
    case OpenGLVersion4x5:
    {
        // Fall though1
    }
    case OpenGLVersion4x4:
    {
        // Fall though1
    }
    case OpenGLVersion4x3:
    {
        OpenGLCacheFunction(functionNameList, &length, "glDebugMessageCallback", &openGLContext->OpenGLDebugMessageCallback);

        // Fall though1
    }
    case OpenGLVersion4x2:
    {
        // Fall though1
    }
    case OpenGLVersion4x1:
    {
        OpenGLCacheFunction(functionNameList, &length, "glVertexAttribLPointer", &openGLContext->OpenGLVertexAttribLPointerCallBack);

        // Fall though1
    }
    case OpenGLVersion4x0:
    {
        // Fall though1
    }
    case OpenGLVersion3x3:
    {
        // Fall though1
    }
    case OpenGLVersion3x2:
    {
        // Fall though1
    }
    case OpenGLVersion3x1:
    {
        // Fall though1
    }
    case OpenGLVersion3x0:
    {
        OpenGLCacheFunction(functionNameList, &length, "glGenVertexArrays", &openGLContext->OpenGLGenVertexArraysCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glBindVertexArray", &openGLContext->OpenGLBindVertexArrayCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glVertexAttribIPointer", &openGLContext->OpenGLVertexAttribIPointerCallBack);

        // Fall though1
    }
    case OpenGLVersion2x1:
    {
        // Fall though1
    }
    case OpenGLVersion2x0:
    {
        OpenGLCacheFunction(functionNameList, &length, "glCreateProgram", &openGLContext->OpenGLShaderProgramCreateCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUseProgram", &openGLContext->OpenGLShaderProgramUseCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glShaderSource", &openGLContext->OpenGLShaderSourceCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glCompileShader", &openGLContext->OpenGLCompileShaderCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glGetShaderiv", &openGLContext->OpenGLShaderGetivCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glGetShaderInfoLog", &openGLContext->OpenGLShaderLogInfoGetCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glDeleteShader", &openGLContext->OpenGLShaderDeleteCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glAttachShader", &openGLContext->OpenGLAttachShaderCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glLinkProgram", &openGLContext->OpenGLLinkProgramCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glValidateProgram", &openGLContext->OpenGLValidateProgramCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glActiveTexture", &openGLContext->OpenGLActiveTextureCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glGenBuffers", &openGLContext->OpenGLGenBuffersCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glBindBuffer", &openGLContext->OpenGLBindBufferCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glBufferData", &openGLContext->OpenGLBufferDataCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glVertexAttribPointer", &openGLContext->OpenGLVertexAttribPointerCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glDisableVertexAttribArray", &openGLContext->OpenGLDisableVertexAttribArrayCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glDisableVertexArrayAttrib", &openGLContext->OpenGLDisableVertexArrayAttribCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glGetUniformLocation", &openGLContext->OpenGLGetUniformLocation);
        OpenGLCacheFunction(functionNameList, &length, "glUniform1f", &openGLContext->OpenGLUniform1fCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform1fv", &openGLContext->OpenGLUniform1fvCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform1i", &openGLContext->OpenGLUniform1iCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform1iv", &openGLContext->OpenGLUniform1ivCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform2f", &openGLContext->OpenGLUniform2fCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform2fv", &openGLContext->OpenGLUniform2fvCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform2i", &openGLContext->OpenGLUniform2iCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform2iv", &openGLContext->OpenGLUniform2ivCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform3f", &openGLContext->OpenGLUniform3fCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform3fv", &openGLContext->OpenGLUniform3fvCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform3i", &openGLContext->OpenGLUniform3iCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform3iv", &openGLContext->OpenGLUniform3ivCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform4f", &openGLContext->OpenGLUniform4fCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform4fv", &openGLContext->OpenGLUniform4fvCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform4i", &openGLContext->OpenGLUniform4iCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniform4iv", &openGLContext->OpenGLUniform4ivCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniformMatrix2fv", &openGLContext->OpenGLUniformMatrix2fvCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniformMatrix3fv", &openGLContext->OpenGLUniformMatrix3fvCallBack);
        OpenGLCacheFunction(functionNameList, &length, "glUniformMatrix4fv", &openGLContext->OpenGLUniformMatrix4fvCallBack);

        // Fall though1
    }
    case OpenGLVersion1x5:
    {
        // Fall though1
    }
    case OpenGLVersion1x4:
    {
        // Fall though1
    }
    case OpenGLVersion1x3:
    {
        // Fall though1
    }
    case OpenGLVersion1x2x1:
    {
        // Fall though1
    }
    case OpenGLVersion1x2:
    {
        // Fall though1
    }
    case OpenGLVersion1x1:
    {
        /*
        const char* functionNameList[50];

        functionNameList[0] = "glActiveTexture";
        functionNameList[0] = "glAlphaFunc";
        functionNameList[0] = "glBindBuffer";
        functionNameList[0] = "glBindTexture";
        functionNameList[0] = "glBlendFunc";
        functionNameList[0] = "glBufferData";
        functionNameList[0] = "glBufferSubData";
        functionNameList[0] = "glClear";
        functionNameList[0] = "glClearColor";
        functionNameList[0] = "glClearDepth";
        functionNameList[0] = "glClearStencil";
        functionNameList[0] = "glClientActiveTexture";
        functionNameList[0] = "glClipPlane";
        functionNameList[0] = "glColor";
        functionNameList[0] = "glColorMask";
        functionNameList[0] = "glColorPointer";
        functionNameList[0] = "glCompressedTexImage2D";
        functionNameList[0] = "glCompressedTexSubImage2D";
        functionNameList[0] = "glCopyTexImage2D";
        functionNameList[0] = "glCopyTexSubImage2D";
        functionNameList[0] = "glCullFace";
        functionNameList[0] = "glCurrentPaletteMatrix";
        functionNameList[0] = "glDeleteBuffers";
        functionNameList[0] = "glDeleteTextures";
        functionNameList[0] = "glDepthFunc";
        functionNameList[0] = "glDepthMask";
        functionNameList[0] = "glDepthRange";
        functionNameList[0] = "glDrawArrays";
        functionNameList[0] = "glDrawElements";
        functionNameList[0] = "glDrawTex";
        functionNameList[0] = "glEnable";
        functionNameList[0] = "glEnableClientState";
        functionNameList[0] = "glFinish";
        functionNameList[0] = "glFlush";
        functionNameList[0] = "glFog";
        functionNameList[0] = "glFrontFace";
        functionNameList[0] = "glFrustum";
        functionNameList[0] = "glGenBuffers";
        functionNameList[0] = "glGenTextures";
        functionNameList[0] = "glGet";
        functionNameList[0] = "glGetBufferParameteriv";
        functionNameList[0] = "glGetClipPlane";
        functionNameList[0] = "glGetError";
        functionNameList[0] = "glGetLight";
        functionNameList[0] = "glGetMaterial";
        functionNameList[0] = "glGetPointerv";
        functionNameList[0] = "glGetString";
        functionNameList[0] = "glGetTexEnv";
        functionNameList[0] = "glGetTexParameter";
        functionNameList[0] = "glHint";
        functionNameList[0] = "glIntro";
        functionNameList[0] = "glIsBuffer";
        functionNameList[0] = "glIsEnabled";
        functionNameList[0] = "glIsTexture";
        functionNameList[0] = "glLight";
        functionNameList[0] = "glLightModel";
        functionNameList[0] = "glLineWidth";
        functionNameList[0] = "glLoadIdentity";
        functionNameList[0] = "glLoadMatrix";
        functionNameList[0] = "glLoadPaletteFromModelViewMatrix";
        functionNameList[0] = "glLogicOp";
        functionNameList[0] = "glMaterial";
        functionNameList[0] = "glMatrixIndexPointer";
        functionNameList[0] = "glMatrixMode";
        functionNameList[0] = "glMultMatrix";
        functionNameList[0] = "glMultiTexCoord";
        functionNameList[0] = "glNormal";
        functionNameList[0] = "glNormalPointer";
        functionNameList[0] = "glOrtho";
        functionNameList[0] = "glPixelStorei";
        functionNameList[0] = "glPointParameter";
        functionNameList[0] = "glPointSize";
        functionNameList[0] = "glPointSizePointerOES";
        functionNameList[0] = "glPolygonOffset";
        functionNameList[0] = "glPushMatrix";
        functionNameList[0] = "glQueryMatrix";
        functionNameList[0] = "glReadPixels";
        functionNameList[0] = "glRotate";
        functionNameList[0] = "glSampleCoverage";
        functionNameList[0] = "glScale";
        functionNameList[0] = "glScissor";
        functionNameList[0] = "glShadeModel";
        functionNameList[0] = "glStencilFunc";
        functionNameList[0] = "glStencilMask";
        functionNameList[0] = "glStencilOp";
        functionNameList[0] = "glTexCoordPointer";
        functionNameList[0] = "glTexEnv";
        functionNameList[0] = "glTexImage2D";
        functionNameList[0] = "glTexParameter";
        functionNameList[0] = "glTexSubImage2D";
        functionNameList[0] = "glTranslate";
        functionNameList[0] = "glVertexPointer";
        functionNameList[0] = "glViewport";
        functionNameList[0] = "glWeightPointer";

        for (size_t i = 0; i < length; ++i)
        {
            const char* const functioName = functionNameList[i];

            OpenGLFunctionFetch();
        }
        */

        // Fall though1
    }
    case OpenGLVersion1x0:
    {
        // Is connected staticly, no fetching needed.
    }
    }

    for (size_t i = 0; i < length; i += 2)
    {
        void** functionAdress = functionNameList[i];
        const char* functionName = functionNameList[i + 1];

#if defined(OSUnix)

#elif defined(OSWindows)
        void* p = (void*)wglGetProcAddress(functionName);

        if (p == 0 || (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) || (p == (void*)-1))
        {
            HMODULE module = LoadLibraryA("opengl32.dll");
            p = (void*)GetProcAddress(module, functionName);
        }
#endif

        *functionAdress = p;
    }

    if (openGLContext->OpenGLDebugMessageCallback)
    {
        (openGLContext->OpenGLDebugMessageCallback)(OpenGLErrorMessageCallback, 0);
    }
}

void OpenGLContextSelect(OpenGLContext* const openGLContext)
{
#if defined(OSUnix)

#elif defined(OSWindows)
    const BOOL result = wglMakeCurrent(openGLContext->WindowsDeviceContext, openGLContext->OpenGLConext);
#endif
}

void OpenGLContextDeselect(OpenGLContext* const openGLContext)
{
#if defined(OSUnix)

#elif defined(OSWindows)
    const BOOL result = wglMakeCurrent(0, 0);
#endif
}

void OpenGLContextRelease(OpenGLContext* const openGLContext)
{

}

void OpenGLContextFlush()
{
    glFlush();
}

void OpenGLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    unsigned char openGLspecific = type == GL_DEBUG_TYPE_ERROR;
    const char* sourceText = 0;
    const char* typeText = 0;
    const char* servertyText = 0;

    switch (source)
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

    switch (type)
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

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        servertyText = "High";
        break;

    case GL_DEBUG_SEVERITY_MEDIUM:
        servertyText = "Medium";
        break;

    case GL_DEBUG_SEVERITY_LOW:
        servertyText = "Low";
        break;

    case GL_DEBUG_SEVERITY_NOTIFICATION:
        servertyText = "Info";
        break;

    default:
        break;
    }

    fprintf
    (
        stderr,
        "[x][OpenGL][%s][%s][%s] %s\n",
        sourceText,
        typeText,
        servertyText,
        message
    );
}




/*

OpenGLID OpenGLToRenderMode(const GraphicRenderMode renderMode)
{
    switch (renderMode)
    {
    case GraphicRenderModePoint:
        return GL_POINTS;

    case GraphicRenderModeLine:
        return GL_LINES;

        // case GraphicRenderModeLineAdjacency:
          //   return GL_LINES_ADJACENCY;

       //  case GraphicRenderModeLineStripAdjacency:
        //     return GL_LINE_STRIP_ADJACENCY;

    case GraphicRenderModeLineLoop:
        return GL_LINE_LOOP;

    case GraphicRenderModeLineStrip:
        return GL_LINE_STRIP;

    case GraphicRenderModeTriangle:
        return GL_TRIANGLES;

        // case GraphicRenderModeTriangleAdjacency:
        //     return GL_TRIANGLES_ADJACENCY;

    case GraphicRenderModeTriangleFAN:
        return GL_TRIANGLE_FAN;

    case GraphicRenderModeTriangleStrip:
        return GL_TRIANGLE_STRIP;

        //   case GraphicRenderModeTriangleStripAdjacency:
          //     return GL_TRIANGLE_STRIP_ADJACENCY;

    case GraphicRenderModeSquare:
        return GL_QUADS;

        //  case GraphicRenderModePatches:
         //     return GL_PATCHES;

    default:
    case GraphicRenderModeInvalid:
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



    GLYPHMETRICSFLOAT
    LAYERPLANEDESCRIPTOR
    PIXELFORMATDESCRIPTOR
    POINTFLOAT


* /

void OpenGLAddFunction(void** loadList, size_t* currentSize, char* name, void* functionADress)
{
    size_t index = *currentSize;

    loadList[index] = functionADress;
    loadList[index + 1] = name;

    *currentSize += 2u;
}
/*
OpenGLID OpenGLToShaderType(const ShaderType shaderType)
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

OpenGLID OpenGLToImageFormat(const ImageDataFormat imageFormat)
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

OpenGLID OpenGLToImageType(const GraphicImageType imageType)
{
    switch (imageType)
    {
    case GraphicImageTypeTexture2D:
        return GL_TEXTURE_2D;

    //case GraphicImageTypeTexture3D:
   //     return GL_TEXTURE_3D;

    case GraphicImageTypeTextureCubeContainer:
        return GL_TEXTURE_CUBE_MAP;

    case GraphicImageTypeTextureCubeRight:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_X;

    case GraphicImageTypeTextureCubeLeft:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;

    case GraphicImageTypeTextureCubeTop:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;

    case GraphicImageTypeTextureCubeDown:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;

    case GraphicImageTypeTextureCubeBack:
        return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;

    case GraphicImageTypeTextureCubeFront:
        return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;

    default:
        return -1;
    }
}

OpenGLID OpenGLToImageWrap(const GraphicImageWrap imageWrap)
{
    switch (imageWrap)
    {
    case GraphicImageWrapNoModification:
        return GL_CLAMP_TO_BORDER;

   // case GraphicImageWrapStrechEdges:
    //    return GL_CLAMP_TO_EDGE;

   // case GraphicImageWrapStrechEdgesAndMirror:
 //       return GL_MIRROR_CLAMP_TO_EDGE;

  //  caseGraphicImageWrapRepeat:
  //      return GL_REPEAT;

   // case GraphicImageWrapRepeatAndMirror:
   //     return GL_MIRRORED_REPEAT;
    default:
        return -1;
    }
}

OpenGLID OpenGLToImageLayout(const GraphicImageLayout layout)
{
    switch (layout)
    {
    case GraphicImageLayoutNearest:
        return GL_NEAREST;

    case GraphicImageLayoutLinear:
        return GL_LINEAR;

    case GraphicImageLayoutMipMapNearestNearest:
        return GL_NEAREST_MIPMAP_NEAREST;

    case GraphicImageLayoutMipMapLinearNearest:
        return GL_LINEAR_MIPMAP_NEAREST;

    case GraphicImageLayoutMipMapNNearestLinear:
        return GL_NEAREST_MIPMAP_LINEAR;

    case GraphicImageLayoutMipMapLinearLinear:
        return GL_LINEAR_MIPMAP_LINEAR;

    default:
        return -1;
    }
}















void OpenGLContextSelect(OpenGL* const openGL)
{

}

void OpenGLContextDeselect()
{
#if defined(OSUnix)

#elif defined(OSWindows)
    wglMakeCurrent(NULL, NULL);
#endif
}

void OpenGLContextRelease(OpenGL* const openGL)
{
#if defined(OSUnix)

#elif defined(OSWindows)
    const BOOL result = wglDeleteContext(openGL->OpenGLConext);

    if (result)
    {
        openGL->OpenGLConext = 0;
    }

#endif
}

void OpenGLContextFlush()
{
    glFlush(); // Flush drawing command buffer to make drawing happen as soon as possible.
}

OpenGLShaderProgramID OpenGLShaderProgramCreate(OpenGL* const openGL)
{
    return (*openGL->OpenGLShaderProgramCreateCallBack)();
}

void OpenGLShaderProgramUse(OpenGL* const openGL, const OpenGLShaderProgramID shaderProgramID)
{
    (openGL->OpenGLShaderProgramUseCallBack)(shaderProgramID);
}

void OpenGLShaderSource(OpenGL* const openGL, unsigned int shader, int count, const char** string, const int* length)
{
    (openGL->OpenGLShaderSourceCallBack)(shader, count, string, length);
}

void OpenGLShaderCompile(OpenGL* const openGL, const OpenGLShaderID shaderID)
{
    (openGL->OpenGLCompileShaderCallBack)(shaderID);
}

void OpenGLShaderGetiv(OpenGL* const openGL, const OpenGLShaderID shaderID, GLenum pname, GLint* params)
{
    (openGL->OpenGLShaderGetivCallBack)(shaderID, pname, params);
}

void OpenGLShaderLogInfoGet(OpenGL* const openGL, const OpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog)
{
    (openGL->OpenGLShaderLogInfoGetCallBack)(shaderID, maxLength, length, infoLog);
}

void OpenGLShaderDelete(OpenGL* const openGL, const OpenGLShaderID shaderID)
{
    (openGL->OpenGLShaderDeleteCallBack)(shaderID);
}

void OpenGLVertexAttributeArrayDefine(OpenGL* const openGL, const size_t sizeOfElement, const size_t listSize, const unsigned int* list)
{
    size_t offset = 0;
    size_t wholeBlockSize = 0;
    size_t currentBlockSize = 0;

    for (size_t vertexAtributeIndex = 0; vertexAtributeIndex < listSize; vertexAtributeIndex++)
    {
        const unsigned int vertexSize = list[vertexAtributeIndex];

        wholeBlockSize += vertexSize;
    }

    wholeBlockSize *= sizeOfElement;

    for (size_t vertexAtributeIndex = 0; vertexAtributeIndex < listSize; vertexAtributeIndex++)
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

OpenGLID BF::OpenGL::ShaderProgramCreate()
{
    return OpenGLShaderProgramCreate();
}

void BF::OpenGL::DepthMaskEnable(bool enable)
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

void BF::OpenGL::DrawOrder(bool clockwise)
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

void BF::OpenGL::RenderBothSides(bool renderBothSides)
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

char BF::OpenGL::UseShaderProgram(int shaderProgramID)
{
    const bool valid = shaderProgramID != -1;

    if(!valid)
    {
        return false;
    }

    glUseProgram(shaderProgramID);

    return true;
}

void BF::OpenGL::VertexArrayBind(int vertexArrayID)
{
    assert(vertexArrayID != -1);

    glBindVertexArray(vertexArrayID);
}

void BF::OpenGL::VertexBufferBind(int vertexBufferID, int indexBuffer)
{
    assert(vertexBufferID != -1);
    assert(indexBuffer != -1);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
}

int BF::OpenGL::TextureMaxSlots()
{
    int value = -1;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, (GLint*)&value);

    return value;
}

int BF::OpenGL::TextureMaxLoaded()
{
    int value = -1;

    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, (GLint*)&value);

    return value;
}

const char* BF::OpenGL::GLSLVersionPrimary()
{
    return (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);
}

void BF::OpenGL::GLSLVersionsSupported(const char*** shaderList, int shaderListSize)
{
    glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &shaderListSize);

    (*shaderList) = Memory::Allocate<const char*>(shaderListSize);

    for (size_t i = 0; i < shaderListSize; i++)
    {
        const char* shaderVersion = (const char*)glGetStringi(GL_SHADING_LANGUAGE_VERSION, i);

        (*shaderList)[i] = shaderVersion;
    }
}

unsigned int BF::OpenGL::ShaderCompile(unsigned int type, char* shaderString)
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
                "| GLSL - OpenGL Shader - compile error log                                     |\n"
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

void BF::OpenGL::VertexAttributeArrayDefine(const unsigned int sizeOfElement, const unsigned int listSize, const unsigned int* list)
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

void BF::OpenGL::VertexArrayDefine(unsigned int* vertexArrayID)
{
    glGenVertexArrays(1, vertexArrayID);
}

void BF::OpenGL::VertexDataDefine(unsigned int* vertexID, int size, void* data)
{
    glGenBuffers(1, vertexID); // Get BufferID
    glBindBuffer(GL_ARRAY_BUFFER, *vertexID); // Select Buffer
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

void BF::OpenGL::IndexDataDefine(unsigned int* indexID, int size, void* data)
{
    glGenBuffers(1, indexID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *indexID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW);
}

int BF::OpenGL::ShaderGetUniformLocationID(int shaderID, const char* UniformName)
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

void BF::OpenGL::ShaderSetUniformMatrix4x4(const int matrixUniformID, const float* matrix)
{
    const bool valid = matrixUniformID != -1;

    if (!valid)
    {
        return;
    }

    glUniformMatrix4fv(matrixUniformID, 1, GL_FALSE, matrix);
}

void BF::OpenGL::ShaderSetUniformVector3(int vector3UniformID, float x, float y, float z)
{
    if (vector3UniformID != -1)
    {
        glUniform3f(vector3UniformID, x, y, z);
    }
}

void BF::OpenGL::ShaderSetUniformVector4(int vector3UniformID, float x, float y, float z, float w)
{
    if (vector3UniformID != -1)
    {
        glUniform4f(vector3UniformID, x, y, z, w);
    }
}

#endif


*/