#include "PXRIFF.hpp"

PX::ActionResult PX::RIFF::Parse(PX::File& file)
{
	return (PX::ActionResult)PXRIFFParse(this, &file);
}

PX::ActionResult PX::RIFF::Serialize(PX::File& file)
{
	return (PX::ActionResult)PXRIFFSerialize(this, &file);
}