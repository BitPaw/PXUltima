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
    return (PX::ActionResult)this->ViewPortGet(this, &pxViewPort);
}

PX::ActionResult PX::Graphic::ViewPortGet(PXViewPort& pxViewPort)
{
    return (PX::ActionResult)PXGraphicViewPortGet(this, &pxViewPort);
}