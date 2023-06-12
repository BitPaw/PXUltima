#include "PXDEFLATE.hpp"

PX::ActionResult PX::Deflate::Parse(PX::File& pxInputStream, PX::File& pxOutputStream)
{
	return (PX::ActionResult)PXDEFLATEParse(&pxInputStream, &pxOutputStream);
}

PX::ActionResult PX::Deflate::Serialize(PX::File& pxInputStream, PX::File& pxOutputStream)
{
	return (PX::ActionResult)PXDEFLATESerialize(&pxInputStream, &pxOutputStream);
}