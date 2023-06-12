#include "PXAVI.hpp"

PX::ActionResult PX::AVI::Parse(PX::File& file)
{
	return (PX::ActionResult)PXAVIParse(this, &file);
}