#pragma once

#include <Media/MTL/PXMTL.h>
#include <OS/File/PXFile.hpp>

namespace PX
{
	class MTL : public PXMTL
	{
		public:
		MTL();
		~MTL();

		PXSize FetchAmount(const void* const data, const PXSize dataSize);
		bool FetchMaterial(const void* const data, const PXSize dataSize, const PXSize materialID, PXMTLMaterial* const mtlMaterial);

		PX::ActionResult FileCompile(PX::File& const inputStream, PX::File& const outputStream);
		PX::ActionResult ParseToMaterial(PX::File& const inputStream, PXMaterialList* const pxMaterialList);
	};
}