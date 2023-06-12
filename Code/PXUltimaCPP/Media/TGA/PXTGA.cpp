#include "PXTGA.hpp"

PX::TGA::TGA()
{
	PXTGAConstruct(this);
}

PX::TGA::~TGA()
{
	PXTGADestruct(this);
}

PXSize PX::TGA::FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
	return PXTGAFilePredictSize(width, height, bbp);
}

PX::ActionResult PX::TGA::Parse(PX::File& file)
{
	return (PX::ActionResult)PXTGAParse(this, &file);
}

PX::ActionResult PX::TGA::ParseToImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXTGAParseToImage(&image, &file);
}

PX::ActionResult PX::TGA::SerializeFromImage(PX::Image& image, PX::File& file)
{
	return (PX::ActionResult)PXTGASerializeFromImage(&image, &file);
}