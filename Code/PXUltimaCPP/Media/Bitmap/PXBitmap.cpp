#include "PXBitmap.hpp"

PX::Bitmap::Bitmap()
{
	PXBitmapConstruct(this);
}

PX::Bitmap::~Bitmap()
{
	PXBitmapDestruct(this);
}

PXSize PX::Bitmap::FilePredictSize(const PXSize width, const PXSize height, const PXSize bitsPerPixel)
{
	return PXBitmapFilePredictSize(width, height, bitsPerPixel);
}

PX::ActionResult PX::Bitmap::ParseToImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXBitmapParseToImage(&image, &file);
}

PX::ActionResult PX::Bitmap::SerializeFromImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXBitmapSerializeFromImage(&image, &file);
}