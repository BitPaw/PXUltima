#include "PXGraphic.hpp"

PX::Graphic::Graphic()
{
}

PX::Graphic::~Graphic()
{
}

PX::ActionResult PXAPI PX::Graphic::Initialize(const PX::GraphicSystem graphicSystem)
{
    return  (PX::ActionResult)_internal.Initialize(this, 0);
}

PX::ActionResult PXAPI PX::Graphic::ViewPortSet(const PX::ViewPort& pxViewPort)
{
    return (PX::ActionResult)_internal.ViewPortSet(this, &pxViewPort);
}

PX::ActionResult PXAPI PX::Graphic::ViewPortGet(PX::ViewPort& pxViewPort)
{
    return (PX::ActionResult)_internal.ViewPortGet(this, &pxViewPort);
}

PX::ActionResult PXAPI PX::Graphic::Texture2DLoad(PX::Image& pxImage, PX::Text& pxImageFilePath)
{
    return (PX::ActionResult)PXGraphicTexture2DLoad(this, &pxImage, pxImageFilePath);
}

PX::ActionResult PXAPI PX::Graphic::TextureScreenShot(PX::Image& pxImage)
{
    return (PX::ActionResult)_internal.TextureScreenShot(this, &pxImage);
}

PX::ActionResult PXAPI PX::Graphic::Texture2DLoad(PX::Texture2D& texture, const PX::Text& filePath)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::Texture2DLoad(PX::Texture2D& texture, const char* const filePath)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::FontLoad(PX::Font& pxFont, const PX::Text& filePath)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::FontRegister(PX::Font& pxFont)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::FontRelease(PX::Font& pxFont)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::FontUse(PX::Font& pxFont)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::SkyboxRegister(PX::SkyBox& skyBox)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::SkyboxRegister(PX::SkyBox& const skyBox, const PX::Text& const shaderVertex, const PX::Text& const shaderFragment, const PX::Text& const textureRight, const PX::Text& const textureLeft, const PX::Text& const textureTop, const PX::Text& const textureBottom, const PX::Text& const textureBack, const PX::Text& const textureFront)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::SkyboxRegister(PX::SkyBox& const skyBox, const char* const shaderVertex, const char* const shaderFragment, const char* const textureRight, const char* const textureLeft, const char* const textureTop, const char* const textureBottom, const char* const textureBack, const char* const textureFront)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PX::ActionResult PXAPI PX::Graphic::SkyboxUse(PX::SkyBox& skyBox)
{
    return (PX::ActionResult)PXGraphicSkyboxUse(this, &skyBox);
}

PX::ActionResult PXAPI PX::Graphic::SkyboxRelease(PX::SkyBox& skyBox)
{
    return (PX::ActionResult)PXGraphicSkyboxRelease(this, &skyBox);
}
