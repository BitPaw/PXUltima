#include "OpenGL.h"

// wingdi.h

#include <Error/ActionResult.h>
#include <Memory/Memory.h>
#include <Text/Text.h>
#include <Container/ClusterValue.h>


#if defined(OSUnix)

#elif defined(OSWindows)
#pragma comment(lib, "opengl32.lib")
#endif

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


*/

void OpenGLAddFunction(void** loadList, size_t* currentSize, char* name, void* functionADress)
{
    size_t index = *currentSize;

    loadList[index] = functionADress;
    loadList[index + 1] = name;

    *currentSize += 2u;
}

void OpenGLContextConstruct(OpenGL* const openGL)
{
    MemorySet(openGL, sizeof(OpenGL), 0);
}

void OpenGLContextDestruct(OpenGL* const openGL)
{
}

void OpenGLFunctionsFetchAll(OpenGL* const openGL)
{
    size_t length = 0;
    void* functionNameList[128];

    switch (openGL->Version)
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
            // Fall though1
        }
        case OpenGLVersion4x2:
        {
            // Fall though1
        }
        case OpenGLVersion4x1:
        {
            OpenGLAddFunction(functionNameList, &length, "glVertexAttribLPointer", &openGL->OpenGLVertexAttribLPointerCallBack);
           
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
            OpenGLAddFunction(functionNameList, &length, "glGenVertexArrays", &openGL->OpenGLGenVertexArraysCallBack);
            OpenGLAddFunction(functionNameList, &length, "glBindVertexArray", &openGL->OpenGLBindVertexArrayCallBack);
            OpenGLAddFunction(functionNameList, &length, "glVertexAttribIPointer", &openGL->OpenGLVertexAttribIPointerCallBack);
     
            // Fall though1
        }
        case OpenGLVersion2x1:
        {
            // Fall though1
        }
        case OpenGLVersion2x0:
        {
            OpenGLAddFunction(functionNameList, &length, "glCreateProgram", &openGL->OpenGLShaderProgramCreateCallBack);           
            OpenGLAddFunction(functionNameList, &length, "glUseProgram", &openGL->OpenGLShaderProgramUseCallBack);
            OpenGLAddFunction(functionNameList, &length, "glShaderSource", &openGL->OpenGLShaderSourceCallBack);
            OpenGLAddFunction(functionNameList, &length, "glCompileShader", &openGL->OpenGLCompileShaderCallBack);
            OpenGLAddFunction(functionNameList, &length, "glGetShaderiv", &openGL->OpenGLShaderGetivCallBack);
            OpenGLAddFunction(functionNameList, &length, "glGetShaderInfoLog", &openGL->OpenGLShaderLogInfoGetCallBack);
            OpenGLAddFunction(functionNameList, &length, "glDeleteShader", &openGL->OpenGLShaderDeleteCallBack);
            OpenGLAddFunction(functionNameList, &length, "glAttachShader", &openGL->OpenGLAttachShaderCallBack);
            OpenGLAddFunction(functionNameList, &length, "glLinkProgram", &openGL->OpenGLLinkProgramCallBack);
            OpenGLAddFunction(functionNameList, &length, "glValidateProgram", &openGL->OpenGLValidateProgramCallBack);
            OpenGLAddFunction(functionNameList, &length, "glActiveTexture", &openGL->OpenGLActiveTextureCallBack);
            OpenGLAddFunction(functionNameList, &length, "glGenBuffers", &openGL->OpenGLGenBuffersCallBack);
            OpenGLAddFunction(functionNameList, &length, "glBindBuffer", &openGL->OpenGLBindBufferCallBack);
            OpenGLAddFunction(functionNameList, &length, "glBufferData", &openGL->OpenGLBufferDataCallBack);
            OpenGLAddFunction(functionNameList, &length, "glVertexAttribPointer", &openGL->OpenGLVertexAttribPointerCallBack);
            OpenGLAddFunction(functionNameList, &length, "glDisableVertexAttribArray", &openGL->OpenGLDisableVertexAttribArrayCallBack);
            OpenGLAddFunction(functionNameList, &length, "glDisableVertexArrayAttrib", &openGL->OpenGLDisableVertexArrayAttribCallBack);
            OpenGLAddFunction(functionNameList, &length, "glGetUniformLocation", &openGL->OpenGLGetUniformLocation);
            OpenGLAddFunction(functionNameList, &length, "glUniform1f", &openGL->OpenGLUniform1fCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform1fv", &openGL->OpenGLUniform1fvCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform1i", &openGL->OpenGLUniform1iCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform1iv", &openGL->OpenGLUniform1ivCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform2f", &openGL->OpenGLUniform2fCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform2fv", &openGL->OpenGLUniform2fvCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform2i", &openGL->OpenGLUniform2iCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform2iv", &openGL->OpenGLUniform2ivCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform3f", &openGL->OpenGLUniform3fCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform3fv", &openGL->OpenGLUniform3fvCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform3i", &openGL->OpenGLUniform3iCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform3iv", &openGL->OpenGLUniform3ivCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform4f", &openGL->OpenGLUniform4fCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform4fv", &openGL->OpenGLUniform4fvCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform4i", &openGL->OpenGLUniform4iCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniform4iv", &openGL->OpenGLUniform4ivCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniformMatrix2fv", &openGL->OpenGLUniformMatrix2fvCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniformMatrix3fv", &openGL->OpenGLUniformMatrix3fvCallBack);
            OpenGLAddFunction(functionNameList, &length, "glUniformMatrix4fv", &openGL->OpenGLUniformMatrix4fvCallBack);
                         
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

        *functionAdress = OpenGLFunctionFetch(functionName);
    }
}

void* OpenGLFunctionFetch(const char* const fuctionName)
{
#if defined(OSUnix)

#elif defined(OSWindows)
    void* p = (void*)wglGetProcAddress(fuctionName);
  
    if (p == 0 ||
        (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
        (p == (void*)-1))
    {
        HMODULE module = LoadLibraryA("opengl32.dll");
        p = (void*)GetProcAddress(module, fuctionName);
    }
#endif

    return p;
}

void OpenGLInitialize(OpenGL* const openGL)
{

#if defined(OSUnix)

#elif defined(OSWindows)
   
#endif  
}

void OpenGLContextCreate(OpenGL* const openGL)
{
#if defined(OSUnix)
    glXMakeCurrent(window->DisplayCurrent, window->ID, window->OpenGLConext);


#elif defined(OSWindows)
    const HGLRC handle = wglCreateContext(openGL->WindowsDeviceContext);

    // Check if failed
    {
        const unsigned char successful = handle;

        if (!successful)
        {
            const ActionResult actionResult = GetCurrentError();

           // return actionResult;
        }
    }

    openGL->OpenGLConext = handle;

#endif      

    OpenGLContextSelect(openGL);


    const char* vendor = glGetString(GL_VENDOR); // Returns the company responsible for this GL implementation.This name does not change from release to release.
   
    TextCopyA(vendor, -1, openGL->Vendor, 64);
    
   
    const char* renderer = glGetString(GL_RENDERER); //   Returns the name of the renderer.This name is typically specific to a particular configuration of a hardware platform.It does not change from release to release.
    
    TextCopyA(renderer, -1, openGL->Renderer, 64);
    
    const char* version = glGetString(GL_VERSION); //    Returns a version or release number.
  
    TextCopyA(version, -1, openGL->VersionText, 64);
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

        switch (id)
        {
        case MakeInt(0, 1, 0, 0): openGL->Version = OpenGLVersion1x0; break;
        case MakeInt(0, 1, 1, 0): openGL->Version = OpenGLVersion1x1; break;
        case MakeInt(0, 1, 2, 0): openGL->Version = OpenGLVersion1x2; break;
        case MakeInt(0, 1, 2, 1): openGL->Version = OpenGLVersion1x2x1; break;
        case MakeInt(0, 1, 3, 0): openGL->Version = OpenGLVersion1x3; break;
        case MakeInt(0, 1, 4, 0): openGL->Version = OpenGLVersion1x4; break;
        case MakeInt(0, 1, 5, 0): openGL->Version = OpenGLVersion1x5; break;
        case MakeInt(0, 2, 0, 0): openGL->Version = OpenGLVersion2x0; break;
        case MakeInt(0, 2, 1, 0): openGL->Version = OpenGLVersion2x1; break;
        case MakeInt(0, 3, 0, 0): openGL->Version = OpenGLVersion3x0; break;
        case MakeInt(0, 3, 1, 0): openGL->Version = OpenGLVersion3x1; break;
        case MakeInt(0, 3, 2, 0): openGL->Version = OpenGLVersion3x2; break;
        case MakeInt(0, 3, 3, 0): openGL->Version = OpenGLVersion3x3; break;
        case MakeInt(0, 4, 0, 0): openGL->Version = OpenGLVersion4x0; break;
        case MakeInt(0, 4, 1, 0): openGL->Version = OpenGLVersion4x1; break;
        case MakeInt(0, 4, 2, 0): openGL->Version = OpenGLVersion4x2; break;
        case MakeInt(0, 4, 3, 0): openGL->Version = OpenGLVersion4x3; break;
        case MakeInt(0, 4, 4, 0): openGL->Version = OpenGLVersion4x4; break;
        case MakeInt(0, 4, 5, 0): openGL->Version = OpenGLVersion4x5; break;
        case MakeInt(0, 4, 6, 0): openGL->Version = OpenGLVersion4x6; break;
        default: openGL->Version = OpenGLVersionInvalid; break;
        }
    }

    OpenGLFunctionsFetchAll(openGL);
}

void OpenGLContextSelect(OpenGL* const openGL)
{
#if defined(OSUnix)

#elif defined(OSWindows)
    const BOOL result = wglMakeCurrent(openGL->WindowsDeviceContext, openGL->OpenGLConext);
#endif
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

CPublic void OpenGLShaderProgramUse(OpenGL* const openGL, const OpenGLShaderProgramID shaderProgramID)
{
    (*openGL->OpenGLShaderProgramUseCallBack)(shaderProgramID);
}

CPublic void OpenGLShaderSource(OpenGL* const openGL, unsigned int shader, int count, const char** string, const int* length)
{
    (*openGL->OpenGLShaderSourceCallBack)(shader, count, string, length);
}

CPublic void OpenGLCompileShader(OpenGL* const openGL, const OpenGLShaderID shaderID)
{
    (*openGL->OpenGLCompileShaderCallBack)(shaderID);
}

CPublic void OpenGLShaderGetiv(OpenGL* const openGL, const OpenGLShaderID shaderID, GLenum pname, GLint* params)
{
    (*openGL->OpenGLShaderGetivCallBack)(shaderID, pname, params);
}

CPublic void OpenGLShaderLogInfoGet(OpenGL* const openGL, const OpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog)
{
    (*openGL->OpenGLShaderLogInfoGetCallBack)(shaderID, maxLength, length, infoLog);
}

CPublic void OpenGLShaderDelete(OpenGL* const openGL, const OpenGLShaderID shaderID)
{
    (*openGL->OpenGLShaderDeleteCallBack)(shaderID);
}
