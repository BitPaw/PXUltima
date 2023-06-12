#include "PXTTF.hpp"

PX::TTF::TTF()
{
	PXTTFConstruct(this);
}

PX::TTF::~TTF()
{
	PXTTFDestruct(this);
}

PX::ActionResult PX::TTF::Parse(PX::File& const file)
{
	return (PX::ActionResult)PXTTFParse(this, &file);
}