#ifndef TestOpenGLDEFINE
#define TestOpenGLDEFINE

#include <OS/Error/PXActionResult.h>
#include <Media/Image.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern void TestOpenGLAll();

    extern void TestOpenGLRenderToTexture();
    extern void TestOpenGLTextDraw();

    extern void TestOpenGLVAO();
    extern void TestOpenGLVBO();
#ifdef __cplusplus
}
#endif

#endif