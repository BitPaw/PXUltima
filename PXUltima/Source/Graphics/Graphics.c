#include "Graphics.h"

//#include <Graphics/OpenGL/OpenGL.h>

ActionResult GraphicRegisterTexture(CTexture* const texture)
{
    Image* image = &texture->Image;
    /*

    const OpenGLID format = OpenGLToImageFormat(image->Format);
    const OpenGLID textureType = OpenGLToImageType(texture->Type);

    if (!image->PixelData)
    {
        return; // No image data
    }

    // Register and select
    {
        OpenGLID textureID = 0;

        glGenTextures(1, &textureID);

        glBindTexture(textureType, textureID);

        texture->ID = textureID;
    }

    // Texture Style
    {
        const int textureWrapWidth = OpenGLToImageWrap(texture->WrapWidth);
        const int textureWrapHeight = OpenGLToImageWrap(texture->WrapHeight);
        const int textueFilterNear = OpenGLToImageLayout(texture->LayoutNear);
        const int textueFilterFar = OpenGLToImageLayout(texture->LayoutFar);

        glTexParameteri(textureType, GL_TEXTURE_WRAP_S, textureWrapWidth);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, textureWrapHeight);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, textueFilterNear);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, textueFilterFar);
       // glTexParameteri(textureType, GL_GENERATE_MIPMAP, GL_FALSE);
    }

    //glTexParameterf(textureType, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

    // ToDO: erro?
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(textureType, 0, GL_RGBA, image->Width, image->Height, 0, format, GL_UNSIGNED_BYTE, image->PixelData);

   // glGenerateMipmap(textureType);

    glBindTexture(textureType, 0);*/

    return ActionSuccessful;
}

ActionResult GraphicTextureRegister(CTexture* const texture)
{
    return ActionInvalid;
}

ActionResult GraphicTextureRelease(CTexture* const texture)
{
    return ActionInvalid;
}

ActionResult GraphicTextureCubeRegister(CTextureCube* const textureCube)
{
    return ActionInvalid;
}

ActionResult GraphicTextureCubeRelease(CTextureCube* const textureCube)
{
    return ActionInvalid;
}

ActionResult GraphicShaderProgramCreate()
{
    return ActionInvalid;
}

ActionResult GraphicShaderCompile()
{
    return ActionInvalid;
}

ActionResult GraphicShaderUse(const unsigned int shaderID)
{
    return ActionInvalid;
}

ActionResult GraphicRender(GraphicRenderMode renderMode, size_t start, size_t amount)
{
    return ActionInvalid;
}