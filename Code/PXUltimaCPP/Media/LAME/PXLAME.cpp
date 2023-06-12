#include "PXLAME.hpp"

PX::ActionResult PX::LAME::Parse(PX::File& file)
{
	return (PX::ActionResult)PXLAMEParse(this, &file);;
}