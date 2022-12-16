#ifndef TestOpenGLDEFINE
#define TestOpenGLDEFINE

#include <Error/PXActionResult.h>
#include <Format/Image.h>

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