#include "OpenGL.h"

// wingdi.h

#include <Error/ActionResult.h>
#include <Memory/Memory.h>
#include <Text/Text.h>


#if defined(OSUnix)

#elif defined(OSWindows)
#pragma comment(lib, "opengl32.lib")
#endif


// Extended functions

GLuint (*glCreateProgram)(void);


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

void OpenGLContextConstruct(OpenGL* const openGL)
{
    MemorySet(openGL, sizeof(OpenGL), 0);
}

void OpenGLContextDestruct(OpenGL* const openGL)
{
}

void OpenGLFunctionsFetchAll()
{
#if defined(OSUnix)

#elif defined(OSWindows)
   
#endif
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
   
    TextCopyA(vendor, -1, openGL->Vendor, 32);
    
   
    const char* renderer = glGetString(GL_RENDERER); //   Returns the name of the renderer.This name is typically specific to a particular configuration of a hardware platform.It does not change from release to release.
    
    TextCopyA(renderer, -1, openGL->Renderer, 32);
    
    const char* version = glGetString(GL_VERSION); //    Returns a version or release number.
  
    TextCopyA(version, -1, openGL->Version, 32);
                                                   // glGetString(GL_SHADING_LANGUAGE_VERSION); //   Returns a version or release number for the shading language.

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

unsigned int OpenGLShaderProgramCreate()
{
    glCreateProgram = OpenGLFunctionFetch("glCreateProgram");

    return glCreateProgram();
}
