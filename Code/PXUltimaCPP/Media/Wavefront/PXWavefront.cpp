#include "PXWavefront.hpp"

PX::Wavefront::Wavefront()
{
	PXWavefrontConstruct(this);
}

PX::Wavefront::~Wavefront()
{
	PXWavefrontDestruct(this);
}

PX::ActionResult PX::Wavefront::FileCompile(PX::File& inputStream, PX::File& outputStream)
{
	return (PX::ActionResult)PXWavefrontFileCompile(&inputStream, &outputStream);
}

PX::ActionResult PX::Wavefront::ParseToModel(PX::File& inputStream, PX::Model& model)
{
	return (PX::ActionResult)PXWavefrontParseToModel(&inputStream, &model);
}