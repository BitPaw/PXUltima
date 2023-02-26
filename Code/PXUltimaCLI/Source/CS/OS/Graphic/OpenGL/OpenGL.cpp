#include "OpenGL.hpp"

PX::OpenGL::OpenGL()
{
    Context = new OpenGLContext();
    OpenGLContextConstruct(Context);
}

PX::OpenGL::~OpenGL()
{
    OpenGLContextDestruct(Context);
    delete Context;
}

bool PX::OpenGL::ContextCreateForWindow()
{
    return OpenGLContextCreateForWindow(Context);
}

void PX::OpenGL::ContextCreateWindowless(const PXSize width, const PXSize height)
{
    OpenGLContextCreateWindowless(Context, width, height);
}