#include "PXImage.hpp"

PX::Image::Image()
{
	PXImageConstruct(this);
}

PX::Image::~Image()
{
	PXImageDestruct(this);
}

bool PX::Image::Resize(const PXColorFormat format, const PXSize width, const PXSize height)
{
	return PXImageResize(this, format, width, height);;
}

void PX::Image::FlipHorizontal()
{
	PXImageFlipHorizontal(this);
}

void PX::Image::FlipVertical()
{
	PXImageFlipVertical(this);
}

void PX::Image::RemoveColor(unsigned char red, unsigned char green, unsigned char blue)
{
	PXImageRemoveColor(this, red, green, blue);
}

void PX::Image::FillColorRGBA8(const PXByte red, const PXByte green, const PXByte blue, const PXByte alpha)
{
	PXImageFillColorRGBA8(this, red, green, blue, alpha);
}

void* PX::Image::DataPoint(const PXSize x, const PXSize y)
{
	return nullptr;
}

PXSize PX::Image::PixelPosition(const PXSize x, const PXSize y)
{
	return 0;
}

void PX::Image::PixelSetRGB8(const PXSize x, const PXSize y, const unsigned char red, const unsigned char green, const unsigned char blue)
{
}