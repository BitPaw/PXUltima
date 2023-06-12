#include "PXTIFF.hpp"

PX::TIFF::TIFF()
{
	
}

PX::TIFF::~TIFF()
{
}

PXSize PX::TIFF::FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
	return PXTIFFFilePredictSize(width, height, bbp);
}

PXActionResult PX::TIFF::Parse(PX::File& file)
{
	return (PXActionResult)PXTIFFParse(this, &file);
}

PX::ActionResult PX::TIFF::ParseToImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXTIFFParseToImage(&image, &file);
}

PX::ActionResult PX::TIFF::SerializeFromImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXTIFFSerializeFromImage(&image, &file);
}