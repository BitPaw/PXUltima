#include "PXMTL.hpp"

PX::MTL::MTL()
{
	PXMTLConstruct(this);
}

PX::MTL::~MTL()
{
	PXMTLDestruct(this);
}

PXSize PX::MTL::FetchAmount(const void* const data, const PXSize dataSize)
{
	return PXMTLFetchAmount(data, dataSize);
}

bool PX::MTL::FetchMaterial(const void* const data, const PXSize dataSize, const PXSize materialID, PXMTLMaterial* const mtlMaterial)
{
	return PXMTLFetchMaterial(data, dataSize, materialID, mtlMaterial);
}

PX::ActionResult PX::MTL::FileCompile(PX::File& const inputStream, PX::File& const outputStream)
{
	return (PX::ActionResult)PXMTLFileCompile(&inputStream, &outputStream);
}

PX::ActionResult PX::MTL::ParseToMaterial(PX::File& const inputStream, PXMaterialList* const pxMaterialList)
{
	return (PX::ActionResult)PXMTLParseToMaterial(&inputStream, pxMaterialList);
}