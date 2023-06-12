#include "PXOGG.hpp"

PX::ActionResult PX::OGG::Parse(PX::File& file)
{
	return (PX::ActionResult)PXOGGParse(this, &file);
}