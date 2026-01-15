#pragma once

#ifndef PXFrameBufferIncluded
#define PXFrameBufferIncluded

#include <PX/Media/PXType.h>
#include <PX/OS/Error/PXResult.h>
#include <PX/Engine/PXResource.h>


typedef struct PXFrameBufferGDI_
{
#if OSWindows
    HDC MemoryDeviceContext; // Memory registered redircect fot the bitmap
    HBITMAP FrameBufferTexture; // Render sink where the rendered data is stored
#else
    int Empty;
#endif
}
PXFrameBufferGDI;

typedef struct PXFrameBufferOpenGL_
{
    // This does not belong here, its per thread!
    //HGLRC RenderingContext; // The Opengl context itself
    //HDC MemoryDeviceContext; // Memory registered redircect for opengl context.

    PXI32U FBO; // GLuint, if 0, then we use the default window
    PXI32U TextureID; // GLuint
}
PXFrameBufferOpenGL;

typedef struct PXFrameBuffer_
{
    union
    {
        PXFrameBufferGDI GDI;
        PXFrameBufferOpenGL OpenGL;
    };

    PXI32S Width;
    PXI32S Height;
}
PXFrameBuffer;

typedef struct PXFrameBufferCreateInfo_
{
    PXGraphicSystem System;

    PXI32S Width;
    PXI32S Height;

    PXWindow* WindowTarget;

#if OSWindows
    //HWND WindowHandle;
    //HDC WindowDeviceContext;
#endif
}
PXFrameBufferCreateInfo;

PXPublic PXResult PXAPI PXFrameBufferRegisterToECS();

PXPublic PXResult PXAPI PXFrameBufferCreate(PXFrameBuffer** pxFrameBufferREF, PXFrameBufferCreateInfo PXREF pxFrameBufferCreateInfo);

#endif