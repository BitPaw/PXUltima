#include "PXGIF.hpp"

PXSize PX::GIF::FilePredictSize(const PXSize width, const PXSize height, const PXSize bbp)
{
    return PXGIFFilePredictSize(width, height, bbp);
}

PX::ActionResult PX::GIF::ParseToImage(PX::Image& image, PX::File& file)
{
    return (PX::ActionResult)PXGIFParseToImage(&image, &file);;
}

PX::ActionResult PX::GIF::SerializeFromImage(PX::Image& image, PX::File& file)
{
    return (PX::ActionResult)PXGIFSerializeFromImage(&image, &file);;
}