#include "PXOpenGL.hpp"

PX::OpenGL::OpenGL()
{
    PXOpenGLConstruct(this);
}

PX::OpenGL::~OpenGL()
{
    PXOpenGLDestruct(this);
}

void PX::OpenGL::Set(OpenGL& pxOpenGL)
{
    PXOpenGLSet(this, &pxOpenGL);
}

void PX::OpenGL::Copy(OpenGL& pxOpenGL)
{
    PXOpenGLCopy(this, &pxOpenGL);
}

bool PX::OpenGL::CreateForWindow()
{
    return PXOpenGLCreateForWindow(this);
}

void PX::OpenGL::CreateWindowless(const PXSize width, const PXSize height)
{
    PXOpenGLCreateWindowless(this, width, height);
}

void PX::OpenGL::Select()
{
    PXOpenGLSelect(this);
}

bool PX::OpenGL::Deselect()
{
    return PXOpenGLDeselect(this);
}

void PX::OpenGL::Release()
{
    PXOpenGLRelease(this);
}

void PX::OpenGL::RenderBufferSwap()
{
    PXOpenGLRenderBufferSwap(this);
}

void PX::OpenGL::Flush()
{
    PXOpenGLFlush(this);
}

void PX::OpenGL::ViewSize(const PXSize x, const PXSize y, const PXSize width, const PXSize height)
{
    PXOpenGLViewSize(this, x, y, width, height);
}

void PX::OpenGL::PolygonRenderOrder(const OpenGLPolygonRenderOrderMode openGLPolygonRenderOrderMode)
{
    PXOpenGLPolygonRenderOrder(this, (PXOpenGLPolygonRenderOrderMode)openGLPolygonRenderOrderMode);
}

void PX::OpenGL::SettingChange(const OpenGLToggle toggle, const bool state)
{
    PXOpenGLSettingChange(this, (PXOpenGLToggle)toggle, state);
}

void PX::OpenGL::DrawOrder(const OpenGLDrawOrderMode openGLDrawOrderMode)
{
    PXOpenGLDrawOrder(this, (PXOpenGLDrawOrderMode)openGLDrawOrderMode);
}

void PX::OpenGL::ClearColor(const float red, const float green, const float blue, const float alpha)
{
    PXOpenGLClearColor(this, red, green, blue, alpha);
}

void PX::OpenGL::Clear(const unsigned int clearID)
{
    PXOpenGLClear(this, clearID);
}

void PX::OpenGL::DrawScaleF(const float x, const float y, const float z)
{
    PXOpenGLDrawScaleF(this, x, y, z);
}

void PX::OpenGL::DrawBegin(const OpenGLRenderMode openGLRenderMode)
{
    PXOpenGLDrawBegin(this, (PXOpenGLRenderMode)openGLRenderMode);
}

void PX::OpenGL::DrawVertexXYZF(const float x, const float y, const float z)
{
    PXOpenGLDrawVertexXYZF(this, x, y, z);
}

void PX::OpenGL::DrawColorRGBF(const float red, const float green, const float blue)
{

}

void PX::OpenGL::DrawEnd()
{
    PXOpenGLDrawEnd(this);
}

void PX::OpenGL::Rectangle(const float xA, const float yA, const float xB, const float yB)
{
    PXOpenGLRectangleF(this, xA, yA, xB, yB);
}

void PX::OpenGL::TextureParameter(const OpenGLTextureType textureType, const OpenGLTextureParameterMode pname, const OpenGLTextureParameterValue openGLTextureParameterValue)
{
}

void PX::OpenGL::TextureParameterI(const OpenGLTextureType textureType, const OpenGLTextureParameterMode pname, const int param)
{
}

void PX::OpenGL::TextureParameterF(const OpenGLTextureType textureType, const OpenGLTextureParameterMode pname, const float param)
{
}

void PX::OpenGL::DrawArrays(const OpenGLRenderMode renderMode, const PXSize offset, const PXSize amount)
{
}

void PX::OpenGL::DrawElements(const OpenGLRenderMode renderMode, const PXSize amount, const OpenGLDataType openGLDataType, const void* const indexList)
{
}

void PX::OpenGL::DrawArraysInstanced(const OpenGLRenderMode renderMode, const PXSize startOffset, const PXSize amount, const PXSize instanceAmount)
{
}

void PX::OpenGL::TextureActivate(const unsigned int index)
{
}

void PX::OpenGL::TextureCreate(GLsizei n, GLuint* textures)
{
}

void PX::OpenGL::TextureBind(const OpenGLTextureType textureType, GLuint texture)
{
}

void PX::OpenGL::TextureUnbind(const OpenGLTextureType textureType)
{
}

void PX::OpenGL::TextureDelete(GLsizei n, const GLuint* textures)
{
}

void PX::OpenGL::TextureData2D(const OpenGLTextureType glTextureType, const unsigned int level, const OpenGLImageFormat glImageFormatInternal, const PXSize width, const PXSize height, const OpenGLImageFormat glImageFormatInput, const OpenGLDataType glDataType, const void* const imageData)
{
}

unsigned int PX::OpenGL::ShaderTypeToID(const OpenGLShaderType openGLShaderType)
{
    return 0;
}

PXOpenGLShaderID PX::OpenGL::ShaderCreate(const OpenGLShaderType openGLShaderType)
{
    return PXOpenGLShaderID();
}

void PX::OpenGL::ShaderSource(const PXOpenGLShaderID shaderID, const PXSize count, const char** string, PXSize* const length)
{
}

unsigned char PX::OpenGL::ShaderCompile(const PXOpenGLShaderID shaderID)
{
    return 0;
}

void PX::OpenGL::ShaderGetiv(const PXOpenGLShaderID shaderID, GLenum pname, GLint* params)
{
}

void PX::OpenGL::ShaderLogInfoGet(const PXOpenGLShaderID shaderID, GLsizei maxLength, GLsizei* length, char* infoLog)
{
}

void PX::OpenGL::ShaderDelete(const PXOpenGLShaderID shaderID)
{
}

PXOpenGLShaderPXProgramID PX::OpenGL::ShaderProgramCreate()
{
    return PXOpenGLShaderPXProgramID();
}

void PX::OpenGL::ShaderProgramUse(const PXOpenGLShaderPXProgramID shaderPXProgramID)
{
}

void PX::OpenGL::ShaderProgramDelete(const PXOpenGLShaderPXProgramID shaderPXProgramID)
{
}

void PX::OpenGL::ShaderProgramAttach(const PXOpenGLShaderPXProgramID shaderPXProgramID, const PXOpenGLShaderID shaderID)
{
}

void PX::OpenGL::ShaderProgramLink(const PXOpenGLShaderID shaderID)
{
}

void PX::OpenGL::ShaderProgramValidate(const PXOpenGLShaderID shaderID)
{
}

GLint PX::OpenGL::ShaderVariableIDGet(GLuint program, const char* name)
{
    return GLint();
}

void PX::OpenGL::ShaderVariableFx1(GLint location, GLfloat v0)
{
}

void PX::OpenGL::ShaderVariableFx1xN(GLint location, GLsizei count, const GLfloat* value)
{
}

void PX::OpenGL::ShaderVariableIx1(GLint location, GLint v0)
{
}

void PX::OpenGL::ShaderVariableIx1xN(GLint location, GLsizei count, const GLint* value)
{
}

void PX::OpenGL::ShaderVariableFx2(GLint location, GLfloat v0, GLfloat v1)
{
}

void PX::OpenGL::ShaderVariableFx2xN(GLint location, GLsizei count, const GLfloat* value)
{
}

void PX::OpenGL::ShaderVariableIx2(GLint location, GLint v0, GLint v1)
{
}

void PX::OpenGL::ShaderVariableIx2xN(GLint location, GLsizei count, const GLint* value)
{
}

void PX::OpenGL::ShaderVariableFx3(GLint location, GLfloat v0, GLfloat v1, GLfloat v2)
{
}

void PX::OpenGL::ShaderVariableFx3xN(GLint location, GLsizei count, const GLfloat* value)
{
}

void PX::OpenGL::ShaderVariableIx3(GLint location, GLint v0, GLint v1, GLint v2)
{
}

void PX::OpenGL::ShaderVariableIx3xN(GLint location, GLsizei count, const GLint* value)
{
}

void PX::OpenGL::ShaderVariableFx4(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)
{
}

void PX::OpenGL::ShaderVariableFx4xN(GLint location, GLsizei count, const GLfloat* value)
{
}

void PX::OpenGL::ShaderVariableIx4(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
{
}

void PX::OpenGL::ShaderVariableIx4xN(GLint location, GLsizei count, const GLint* value)
{
}

void PX::OpenGL::ShaderVariableMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
}

void PX::OpenGL::ShaderVariableMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
}

void PX::OpenGL::ShaderVariableMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value)
{
}

void PX::OpenGL::VertexArrayGenerate(const unsigned int amount, unsigned int* const vaoList)
{
}

void PX::OpenGL::VertexArrayBind(const unsigned int vaoID)
{
}

void PX::OpenGL::VertexArrayUnbind()
{
}

void PX::OpenGL::VertexArrayAttributeDefine(const unsigned int index, const unsigned int size, const OpenGLDataType datatype, const unsigned char normalized, const unsigned int stride, const PXSize offset)
{
}

void PX::OpenGL::VertexAttributeDivisor(const PXSize index, const PXSize divisor)
{
}

void PX::OpenGL::VertexArrayEnable(const unsigned int vertexArrayAtributeID)
{
}

void PX::OpenGL::VertexArrayDisable(const unsigned int vertexArrayAtributeID)
{
}

void PX::OpenGL::BufferGenerate(const unsigned int amount, unsigned int* const bufferIDList)
{
}

void PX::OpenGL::BufferBind(const OpenGLBufferType bufferType, const unsigned int bufferID)
{
}

void PX::OpenGL::BufferData(const OpenGLBufferType bufferType, const unsigned int size, const void* const data, const OpenGLStoreMode openGLStoreMode)
{
}

void PX::OpenGL::BufferUnbind(const OpenGLBufferType bufferType)
{
}

void PX::OpenGL::FrameBufferCreate(const unsigned int amount, unsigned int* const framebufferIDList)
{
}

void PX::OpenGL::RenderBufferStorage(const OpenGLRenderBufferFormat internalformat, const int width, const int height)
{
}

void PX::OpenGL::FrameBufferBind(const OpenGLFrameBufferMode target, const unsigned int framebufferID)
{
}

void PX::OpenGL::FrameBufferDestroy(const unsigned int amount, unsigned int* const framebufferIDList)
{
}

void PX::OpenGL::RenderBufferCreate(GLsizei n, GLuint* renderbuffers)
{
}

void PX::OpenGL::RenderBufferBind(const unsigned int renderbuffer)
{
}

void PX::OpenGL::RenderBufferDelete(GLsizei n, GLuint* renderbuffers)
{
}

void PX::OpenGL::FrameBufferLinkTexture2D(const OpenGLRenderBufferAttachmentPoint attachment, const OpenGLTextureType textarget, const unsigned int textureID, const int level)
{
}

GLuint PX::OpenGL::FrameBufferLinkRenderBuffer(const OpenGLRenderBufferAttachmentPoint attachment, const unsigned int renderbuffer)
{
    return GLuint();
}

void PX::OpenGL::PixelDataRead(const int x, const int y, const int width, const int height, PXOpenGLImageFormat imageFormat, PXOpenGLDataType openGLDataType, void* const pixelData)
{
}

const char* PX::OpenGL::StringGetI(const OpenGLStringName stringName, const unsigned int index)
{
    return nullptr;
}

void PX::OpenGL::SwapIntervalSet(const PXInt32U interval)
{
}

PXInt32U PX::OpenGL::SwapIntervalGet()
{
    return PXInt32U();
}