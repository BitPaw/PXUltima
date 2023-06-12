#include "PXPNG.hpp"

PX::PNG::PNG()
{
	PXPNGConstruct(this);
}

PX::PNG::~PNG()
{
	PXPNGDestruct(this);
}

PXInt8U PX::PNG::BitsPerPixel()
{
	return PXPNGBitsPerPixel(this);
}

PXSize PX::PNG::FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
	return PXPNGFilePredictSize(width, height, bbp);
}

PX::ActionResult PX::PNG::ParseToImage(PX::Image& image, PX::File& dataStream)
{
	return (PX::ActionResult)PXPNGParseToImage(&image, &dataStream);
}

PX::ActionResult PX::PNG::SerializeFromImage(PX::Image& image, PX::File& dataStream)
{
	return (PX::ActionResult)PXPNGSerializeFromImage(&image, &dataStream);
}