#include "PXC.hpp"

PX::ActionResult PX::C::LexicalAnalysis(PX::File& inputStream, PX::File& outputStream)
{
	return (PX::ActionResult)PXCFileLexicalAnalysis(&inputStream, &outputStream);
}

PX::ActionResult PX::C::Compile(PX::File& inputStream, PX::File& outputStream)
{
	return (PX::ActionResult)PXCFileCompile(&inputStream, &outputStream);
}