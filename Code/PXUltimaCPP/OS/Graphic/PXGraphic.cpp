#include "PXGraphic.hpp"

PX::Graphic::Graphic()
{
}

PX::Graphic::~Graphic()
{
}

PX::ActionResult PX::Graphic::Initialize(const PX::GraphicSystem graphicSystem)
{
    return PX::ActionResult();
}

PX::ActionResult PX::Graphic::ViewPortSet(const PXViewPort& pxViewPort)
{
    return (PX::ActionResult)this->ViewPortSet(this, &pxViewPort);
}

PX::ActionResult PX::Graphic::ViewPortGet(PXViewPort& pxViewPort)
{
    return (PX::ActionResult)this->ViewPortGet(this, &pxViewPort);
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::ImageLoad(PX::Image& pxImage, PX::Text& pxImageFilePath)
{
    return this->Image
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::TextureScreenShot(PXImage* const pxImage)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::Texture2DLoad(PXTexture2D* const texture, const PXText* const filePath)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::Texture2DLoadA(PXTexture2D* const texture, const char* const filePath)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::FontLoad(PXFont* const pxFont, const PXText* const filePath)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::FontRegister(PXFont* const pxFont)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::FontRelease(PXFont* const pxFont)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::FontUse(PXFont* const pxFont)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::SkyboxRegister(PX::SkyBox& const skyBox)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::SkyboxRegister(PX::SkyBox& const skyBox, const PX::Text& const shaderVertex, const PX::Text& const shaderFragment, const PX::Text& const textureRight, const PX::Text& const textureLeft, const PX::Text& const textureTop, const PX::Text& const textureBottom, const PX::Text& const textureBack, const PX::Text& const textureFront)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::SkyboxRegister(PX::SkyBox& const skyBox, const char* const shaderVertex, const char* const shaderFragment, const char* const textureRight, const char* const textureLeft, const char* const textureTop, const char* const textureBottom, const char* const textureBack, const char* const textureFront)
{
    return PXDLLExport PX::ActionResult PXAPI();
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::SkyboxUse(PX::SkyBox& skyBox)
{
    return (PX::ActionResult)PXGraphicSkyboxUse(this, &skyBox);
}

PXDLLExport PX::ActionResult PXAPI PX::Graphic::SkyboxRelease(PX::SkyBox& skyBox)
{
    return (PX::ActionResult)PXGraphicSkyboxRelease(this, &skyBox);
}
