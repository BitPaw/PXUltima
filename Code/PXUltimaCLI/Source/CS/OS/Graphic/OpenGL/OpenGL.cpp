#include "PXOpenGL.hpp"

PX::PXOpenGL::PXOpenGL()
{
    Context = new PXOpenGLContext();
    PXOpenGLContextConstruct(Context);
}

PX::PXOpenGL::~PXOpenGL()
{
    PXOpenGLContextDestruct(Context);
    delete Context;
}

bool PX::PXOpenGL::ContextCreateForWindow()
{
    return PXOpenGLContextCreateForWindow(Context);
}

void PX::PXOpenGL::ContextCreateWindowless(const PXSize width, const PXSize height)
{
    PXOpenGLContextCreateWindowless(Context, width, height);
}