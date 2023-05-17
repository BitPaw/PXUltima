#ifndef TestPXOpenGLDEFINE
#define TestPXOpenGLDEFINE

#include <OS/Error/PXActionResult.h>
#include <Media/PXImage.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern void TestPXOpenGLAll();

    extern void TestPXOpenGLRenderToTexture();
    extern void TestPXOpenGLTextDraw();

    extern void TestPXOpenGLVAO();
    extern void TestPXOpenGLVBO();
#ifdef __cplusplus
}
#endif

#endif