#include "PXPDF.hpp"

PX::ActionResult PX::PDF::Compile(PX::File& fileStream)
{
	return (PX::ActionResult)PXPDFCompile(this, &fileStream);;
}