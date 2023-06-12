#include "PXXingInfo.hpp"

PX::ActionResult PX::XingInfo::Parse(PX::File& file)
{
	return (PX::ActionResult)PXXingInfoParse(this, &file);
}