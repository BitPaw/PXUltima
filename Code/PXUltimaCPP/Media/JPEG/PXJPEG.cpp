#include "PXJPEG.hpp"

PX::JPEG::JPEG()
{
	PXJPEGConstruct(this);
}

PX::JPEG::~JPEG()
{
	PXJPEGDestruct(this);
}

PXSize PX::JPEG::FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
	return PXJPEGFilePredictSize(width, height, bbp);
}

PX::ActionResult PX::JPEG::ParseToImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXJPEGParseToImage(&image, &file);
}

PX::ActionResult PX::JPEG::SerializeFromImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXJPEGSerializeFromImage(&image, &file);
}