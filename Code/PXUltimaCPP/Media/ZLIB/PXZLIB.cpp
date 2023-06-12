#include "PXZLIB.hpp"

PX::ActionResult PX::ZLIB::Decompress(PX::File& pxInputSteam, PX::File& pxOutputSteam)
{
	return (PX::ActionResult)PXZLIBDecompress(&pxInputSteam, &pxOutputSteam);
}

PX::ActionResult PX::ZLIB::Compress(PX::File& pxInputSteam, PX::File& pxOutputSteam)
{
	return (PX::ActionResult)PXZLIBCompress(&pxInputSteam, &pxOutputSteam);
}