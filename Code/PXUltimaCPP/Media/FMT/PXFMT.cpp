#include "PXFMT.hpp"

PX::FMT::FMT()
{
	PXFMTConstruct(this);
}

PX::ActionResult PX::FMT::Parse(PX::File& file, const PXEndian endian)
{
	return (PX::ActionResult)PXFMTParse(this, &file, endian);
}

PX::ActionResult PX::FMT::Serialize(PX::File& file, const PXEndian endian)
{
	return (PX::ActionResult)PXFMTSerialize(this, &file, endian);
}